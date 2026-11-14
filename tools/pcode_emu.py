#!/usr/bin/env python3
"""Differential execution of a function and its semantic lift, as Ghidra P-code.

The oracle of tools/port_lift.py (docs/PORT_PLAN.md, oracle (b)): the original instructions and the
lift compiled for AArch64 by the project compiler run side by side from the same inputs, every
instruction executed as the raw P-code Ghidra's AArch64 specification gives it
(tools/ghidra/pcode.py), so the lifter under test never decides what an instruction means. Only
`b`, `bl`, `adr`, `adrp`, the scalar fmax/fmin forms (whose P-code is wrong when Rd is Rm) and the
scalar float-to-integer converts (whose TRUNC saturates signed, also for fcvt*u) and roundings (frint*,
whose P-code is a float-to-integer TRUNC) are executed here directly, and P-code is cached per instruction word (branch targets relative to the
word's address are relocated when it runs).

A run starts from registers and memory derived from a seed, and records what a caller or callee
could observe:
  calls     every call that leaves the function (direct, indirect, tail), in order: its target
            (an address; an import by name) and the argument registers the callee reads
            (tools/abi_facts.py; an indirect call: x0 and the registers written since the last
            call). Callees are not executed: a call returns a value derived from its position in
            the sequence, clobbers the caller-saved registers, and writes a pattern through every
            argument that points into the caller's stack (out-parameters). memcpy, memmove,
            memset, memcmp, strlen and strcmp are executed instead (the lift may inline them).
  memory    every byte written outside the function's own frame and outside the lift's own data
            (its guard variables), compared at every call and at the end. A byte stored from
            stack the run never wrote (a struct copy's padding) is undefined and not compared.
  result    x0 (by the lift's return width) or s0/d0, when the lift returns a value and callers
            may use it (abi_facts ret_used).
  end       return, a fault (a load or store in the first 64 KiB), or a time-out.
Memory the run did not write reads as the original image (its .data and .rodata, relocations
applied), or, anywhere else (including .bss, the heap and the caller's frame), as words derived
from the address and the seed: small integers, pointers to more such memory, or noise. A stack
address passed to a callee is compared by the bytes the function wrote around it, since the two
frames are laid out differently.

A snapshot (load_snapshot: the registers and memory pages a real run held at the function's entry, captured
from the Dread recomp by tools/recomp_trace.py, or from the port itself: port_link --capture, the "port-core"
format, whose pages are its writable memory read from a core file on demand) can stand in for the seed's made-up state: its pages read as
they were (the module's own pages too), anything it did not capture as the seed gives, and the recomp's
addresses of the module are moved to ours. Its calls file (load_calls) replays the real calls in order:
their results, the out-parameters they wrote in the frame and their other memory writes, while each call
goes where the recorded one went; from the first that does not, calls are made up as usual.

Standard library and pyelftools; the P-code comes from a pickle tools/ghidra/pcode.py writes.
"""

from __future__ import annotations

import math
import os
import random
import struct
from dataclasses import dataclass, field

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

BASE = 0x7100000000
LIFT_BASE = 0x70C0000000          # within 4 GB of the image (adrp), outside it and its .bss
ARENA = 0x0000500000000000        # pointer-valued memory words point here
STACK_TOP = 0x00007FF000000000
ENTRY_SP = STACK_TOP - 0x100000
STACK_LO = STACK_TOP - 0x400000
SENTINEL = 0x00007FFFDEAD0000     # the caller's return address
NULL_PAGE = 0x10000
PAGE = 4096
MAX_STEPS = 60000
RETRY_SCALE = 8                   # a cut-off run is retried with this many times each limit below (port_lift)
SLOWER = 20                       # a lift this many times slower than an original that ended is wrong (compare)
MAX_EVENTS = 256                  # calls recorded per run; then it ends as a time-out (checkpoint)
MAX_SNAPSHOT = 1 << 21            # bytes of written memory copied into its events, in all; the same
MAX_RESULTS_KEPT = 64             # call results a run keeps (Machine.results)
STEPS_PER_CALL = 2000             # a run from a snapshot's recorded calls: this many more steps per call
WINDOW = 64                       # bytes compared and written around a stack pointer argument

X0, Q0, SP, NG, ZR, CY, OV = 0x4000, 0x5000, 8, 0x100, 0x101, 0x102, 0x103
QSTRIDE = 0x20  # Ghidra spaces q<n> 32 bytes apart (room for SVE z<n>): q1 is at 0x5020
REG_SIZE = 0x6000
CALLER_SAVED_X = range(19)
CALLER_SAVED_Q = list(range(8)) + list(range(16, 32))
# With Machine.junk_scratch (port_lift --junk-scratch): the register bytes a call leaves undefined
# under AAPCS64 that no argument or result convention gives a meaning: x9-x18, v16-v31 whole and the
# upper halves of v8-v15 (only d8-d15 are callee-saved). x0-x8 and v0-v7 are left as they are (a
# callee's results: x1 of a pair, v1-v3 of an HFA).
JUNK_BYTES = frozenset([(2, X0 + 8 * i + j) for i in range(9, 19) for j in range(8)]
                       + [(2, Q0 + QSTRIDE * i + j) for i in range(16, 32) for j in range(QSTRIDE)]
                       + [(2, Q0 + QSTRIDE * i + j) for i in range(8, 16) for j in range(8, QSTRIDE)])
# A modelled C function's (clib, math): every caller-saved register byte but its result's (x0 for
# clib, s0/d0 for math, Machine.junk_clobber); x0-x8 and v0-v7 included, as their signatures are known
JUNK_LIBC = JUNK_BYTES | frozenset([(2, X0 + j) for j in range(8, 72)] + [(2, Q0 + j) for j in range(8 * QSTRIDE)])
# The bytes a call clears of that taint: x0-x8, v0-v7 and the flags hold what the emulated call put there
JUNK_CLEARED = frozenset([(2, X0 + j) for j in range(72)] + [(2, Q0 + j) for j in range(8 * QSTRIDE)]
                         + [(2, k) for k in range(NG, OV + 1)])
# Imports that read and write no memory (their arguments and result are registers)
PURE_MATH = frozenset(f + sfx for f in ("sqrt", "sin", "cos", "tan", "asin", "acos", "atan", "atan2", "exp", "exp2",
                                        "log", "log2", "log10", "pow", "fmod", "floor", "ceil", "round", "trunc",
                                        "fabs", "hypot", "sinh", "cosh", "tanh", "nextafter") for sfx in ("", "f"))
MODELED = frozenset({"memcpy", "memmove", "memset", "memcmp", "strlen", "strcmp"})
NOOP_USEROPS = frozenset({"DataMemoryBarrier", "DataSynchronizationBarrier", "InstructionSynchronizationBarrier",
                          "Hint_Prefetch", "SpeculationBarrier", "Yield", "SendEvent", "SendEventLocally",
                          "WaitForEvent", "ClearExclusiveLocal", "LOAcquire", "LORelease"})


TRAP_WORD = 0xE7FFDEFE  # the trap word of this binary's unreachable code (unallocated in A64)


class Fault(Exception):
    pass


def vector_copy(w: int) -> tuple[int, int] | None:
    """(d, n) for an instruction copying v<n>'s low 64 bits or more whole into v<d>: `mov v<d>.16b,
    v<n>.16b` or `.8b` (orr with both sources the same) and `fmov d<d>, d<n>`; else None."""
    if (w & 0xBFE0FC00) == 0x0EA01C00 and (w >> 16) & 31 == (w >> 5) & 31:
        return w & 31, (w >> 5) & 31
    if (w & 0xFFFFFC00) == 0x1E604000:
        return w & 31, (w >> 5) & 31
    return None


class Inconclusive(Exception):
    pass


def mix(*vals: int) -> int:
    """A deterministic 64-bit hash of integers (splitmix64 steps)."""
    h = 0x9E3779B97F4A7C15
    for v in vals:
        h = (h ^ (v & 0xFFFFFFFFFFFFFFFF)) * 0xBF58476D1CE4E5B9 & 0xFFFFFFFFFFFFFFFF
        h = (h ^ (h >> 27)) * 0x94D049BB133111EB & 0xFFFFFFFFFFFFFFFF
        h ^= h >> 31
    return h


# Memory profiles by seed (seed % 4): the shares of zero, small integers, pointers and floats, the
# rest noise. Several profiles reach more paths than one (null checks, counts, flags).
PROFILES = [(5, 35, 40, 10), (55, 15, 20, 5), (5, 10, 75, 5), (10, 60, 20, 5)]
DICT_PROFILE = 3  # every third seed also draws words from the function's constants (Machine.word)


def word_value(h: int, profile: int = 0) -> int:
    """A memory word for hash h: zero, a small integer, a pointer into the arena, a float, noise."""
    z, s, p, f = PROFILES[profile % len(PROFILES)]
    k = h % 100
    if k < z:
        return 0
    if k < z + s:
        return 1 + (h >> 8) % 7
    if k < z + s + p:
        return ARENA + ((h >> 8) & 0xFFFFFF0) * 16
    if k < z + s + p + f:
        return 0x3F800000 | (((h >> 8) & 0x3FF) << 13)  # a float near 1-2 in the low word
    return h >> 1


def sext(v: int, bits: int) -> int:
    return v - (1 << bits) if v >> (bits - 1) & 1 else v


def f_from(bits: int, size: int) -> float:
    if size == 4:
        return struct.unpack("<f", struct.pack("<I", bits & 0xFFFFFFFF))[0]
    if size == 8:
        return struct.unpack("<d", struct.pack("<Q", bits & 0xFFFFFFFFFFFFFFFF))[0]
    raise Inconclusive(f"float of {size} bytes")


def nextafterf(a: float, b: float) -> float:
    """C's nextafterf: the next single-precision value after a towards b."""
    if math.isnan(a) or math.isnan(b):
        return math.nan
    if a == b:
        return b
    if a == 0:
        return f_from(1, 4) if b > 0 else -f_from(1, 4)
    bits = f_to(a, 4)
    return f_from(bits + 1 if (a < b) == (a > 0) else bits - 1, 4)


def f_to(v: float, size: int) -> int:
    if size == 4:
        try:
            return struct.unpack("<I", struct.pack("<f", v))[0]
        except OverflowError:
            return 0xFF800000 if v < 0 else 0x7F800000
    if size == 8:
        return struct.unpack("<Q", struct.pack("<d", v))[0]
    raise Inconclusive(f"float of {size} bytes")


# --- the image and the lift's object -------------------------------------------------------------

class ImageMemory:
    """The original module's bytes with RELATIVE relocations applied (pointers are BASE-based)."""

    def __init__(self, img):
        data = bytearray(img.data)
        for off, r in img.relocs.items():
            if r.type == 1027 and off + 8 <= len(data):  # R_AARCH64_RELATIVE
                struct.pack_into("<Q", data, off, BASE + r.addend)
        self.data = bytes(data)
        self.text = (BASE + img.text[0], BASE + img.text[1])
        self.end = BASE + len(self.data)
        self.bss_end = BASE + img.rwdata[1]


RECOMP_MAIN = 0x8005000  # where the Dread recomp places the module (tools/recomp_trace.py)
SNAP_MAGIC = b"DSNAP1\n"
CALLS_MAGIC = b"DCALLS1\n"


@dataclass
class CallRecord:
    """One of the function's calls in that run: where it went, x0, x1 and q0-q3 when it returned, and
    the memory it changed: in the function's frame by offset from its entry sp, elsewhere by address."""
    target: int
    x: list
    q: list
    frame: list  # (offset from the entry sp, bytes)
    mem: list    # (address, bytes)


@dataclass
class Snapshot:
    """A real run's state at a function's entry, in our addresses: x0-x28, q0-q31, the bytes above
    sp (stack arguments), memory pages by page number, sp itself, and (from the .calls file beside
    it) its direct calls in order."""
    x: list
    q: list
    stack: bytes
    pages: dict
    sp: int = 0
    calls: list = field(default_factory=list)


_snapshots: dict[str, Snapshot] = {}


class CorePages:
    """The pages of a port snapshot (format "port-core", port/runtime/src/capture.cpp), read from its
    core file on demand: `n in pages` and `pages[n]` by page number, as Snapshot.pages. The core file
    holds each captured region at its offset (start, end, offset); pages it left as holes read as
    zero, as they were."""

    def __init__(self, path: str, regions: list[tuple[int, int, int]]):
        import mmap
        self._file = open(path, "rb")
        self._map = mmap.mmap(self._file.fileno(), 0, access=mmap.ACCESS_READ)
        self.regions = sorted(regions)
        self._starts = [r[0] for r in self.regions]

    def _offset(self, n: int) -> int | None:
        import bisect
        a = n * PAGE
        i = bisect.bisect_right(self._starts, a) - 1
        if i >= 0:
            start, end, off = self.regions[i]
            if start <= a < end:
                return off + (a - start)
        return None

    def __contains__(self, n: int) -> bool:
        return self._offset(n) is not None

    def __getitem__(self, n: int) -> bytes:
        off = self._offset(n)
        if off is None:
            raise KeyError(n)
        return self._map[off:off + PAGE].ljust(PAGE, b"\0")


def load_snapshot(path: str, module_size: int) -> Snapshot:
    """A capture file (tools/recomp_trace.py snapshot): the magic, one JSON header line ({"x": 31
    values, "q": 32, "sp", "stack": hex bytes above sp, "pages": n}), then n records of an 8-byte page
    address and PAGE bytes, all in the recomp's addresses. Module addresses (page keys, registers and
    every aligned word that falls in the recomp's module) become ours."""
    if path in _snapshots:
        return _snapshots[path]
    import json
    import zlib
    raw = open(path, "rb").read()
    if raw[:2] == b"\x78\x9c" or raw[:2] == b"\x78\xda":
        raw = zlib.decompress(raw)
    if not raw.startswith(SNAP_MAGIC):
        raise ValueError(f"{path}: not a snapshot")
    nl = raw.index(b"\n", len(SNAP_MAGIC))
    head = json.loads(raw[len(SNAP_MAGIC):nl])
    if head.get("format") == "port-core":
        # The port's own capture: the image at our addresses (nothing to translate), its writable
        # memory in the core file beside it, read on demand.
        regions = [(int(a, 16), int(b, 16), int(o, 16)) for a, b, o in head["regions"]]
        pages = CorePages(os.path.join(os.path.dirname(path), head["core"]), regions)
        snap = Snapshot([int(v, 16) for v in head["x"]], [int(v, 16) for v in head["q"]],
                        bytes.fromhex(head.get("stack", "")), pages, int(head.get("sp", "0"), 16))
        _snapshots[path] = snap
        return snap
    lo, hi = RECOMP_MAIN, RECOMP_MAIN + module_size

    def tr(v: int) -> int:
        return v - lo + BASE if lo <= v < hi else v

    pages, off = {}, nl + 1
    for _ in range(head["pages"]):
        a = int.from_bytes(raw[off:off + 8], "little")
        p = bytearray(raw[off + 8:off + 8 + PAGE])
        for k in range(0, PAGE, 8):
            w = int.from_bytes(p[k:k + 8], "little")
            if lo <= w < hi:
                p[k:k + 8] = tr(w).to_bytes(8, "little")
        pages[tr(a) // PAGE] = bytes(p)
        off += 8 + PAGE
    snap = Snapshot([tr(int(v, 16)) for v in head["x"]], [int(v, 16) for v in head["q"]],
                    bytes.fromhex(head.get("stack", "")), pages, int(head.get("sp", "0"), 16))
    calls = path[:-5] + ".calls" if path.endswith(".snap") else path + ".calls"
    if os.path.isfile(calls):
        snap.calls = load_calls(calls, tr)
    _snapshots[path] = snap
    return snap


def load_calls(path: str, tr) -> list:
    """The calls file beside a snapshot: the magic, then one JSON line per direct call, in order
    ({"target", "x": [x0, x1], "q": [q0-q3], "frame": [[offset, hex]], "mem": [[address, hex]]}; each
    run covers whole aligned words, so module addresses in it become ours as in the pages). A line
    cut short (the game closed during the call) ends the list."""
    import json
    raw = open(path, "rb").read()
    if not raw.startswith(CALLS_MAGIC):
        raise ValueError(f"{path}: not a calls file")

    def words(b: bytes) -> bytes:
        b = bytearray(b)
        for k in range(0, len(b) - 7, 8):
            b[k:k + 8] = tr(int.from_bytes(b[k:k + 8], "little")).to_bytes(8, "little")
        return bytes(b)

    out = []
    for line in raw[len(CALLS_MAGIC):].split(b"\n"):
        try:
            r = json.loads(line)
        except ValueError:
            break
        out.append(CallRecord(tr(int(r["target"], 16)), [tr(int(v, 16)) for v in r["x"]],
                              [int(v, 16) for v in r["q"]],
                              [(off, words(bytes.fromhex(h))) for off, h in r["frame"]],
                              [(tr(int(a, 16)), words(bytes.fromhex(h))) for a, h in r["mem"]]))
    return out


@dataclass
class LiftObject:
    """An object file laid out at LIFT_BASE: its sections' bytes, symbol addresses, far branch
    targets and the lift's own data range."""
    mem: dict = field(default_factory=dict)        # page -> bytearray
    functions: dict = field(default_factory=dict)  # address of fn_<addr> -> (start, size)
    branch: dict = field(default_factory=dict)     # pc of a b/bl -> target (resolved symbol)
    text: tuple = (0, 0)
    data: tuple = (0, 0)
    guards: list = field(default_factory=list)     # addresses of guard variables (set to 1)
    unresolved: dict = field(default_factory=dict)  # pc range start -> symbol names (inconclusive)
    words: set = field(default_factory=set)


def load_object(path, resolve, demangled: dict[str, str], anchor=None, address_of=None) -> LiftObject:
    """Lay out every allocatable section of an AArch64 ELF object (text first, then data), apply
    its relocations, and resolve undefined symbols with `resolve(demangled name) -> address`.

    `anchor`, for an object whose own data and functions stand for the original's (a hand-written
    source; a lift names the original's addresses itself): asked for every relocation against
    something the object defines, `anchor(symbol, section, offset, kind)` with the mangled name of
    the symbol referred to (None for an unnamed place in a section, a string literal say), its
    ELF section, the offset within the symbol (within the section when unnamed) and the symbol's
    type ("STT_FUNC", "STT_OBJECT", ...). It returns the address in the original image that place
    stands for, None to keep the object's own copy, or False when the place must be the
    original's and its address is not known: that relocation is then listed in `unresolved`, so
    the function containing it is inconclusive. A reference from a function into itself, or to
    the middle of a function (a jump table's), is never asked about: it stays the object's own.

    `address_of(mangled name) -> address or None` registers a defined function that is not named
    fn_<addr> (a constructor or destructor, whose name Clang 9 never takes from its asm label)."""
    lo = LiftObject()
    with open(path, "rb") as fh:
        elf = ELFFile(fh)
        all_secs = [elf.get_section(i) for i in range(elf.num_sections())]
        secs = [(i, s) for i, s in enumerate(all_secs) if s["sh_flags"] & 2 and s.name != ".eh_frame"]
        texts = [(i, s) for i, s in secs if s["sh_flags"] & 4]
        datas = [(i, s) for i, s in secs if not s["sh_flags"] & 4]
        addr = LIFT_BASE
        place: dict[int, int] = {}
        for group in (texts, datas):
            start = addr
            for i, s in group:
                al = max(s["sh_addralign"], 1)
                addr = (addr + al - 1) // al * al
                place[i] = addr
                addr += s["sh_size"]
            if group is texts:
                lo.text = (start, addr)
                addr = (addr + 0xFFFF) & ~0xFFFF
            else:
                lo.data = (start, addr)
        buf = bytearray(addr - LIFT_BASE)
        for i, s in secs:
            if s["sh_type"] != "SHT_NOBITS":
                d = s.data()
                o = place[i] - LIFT_BASE
                buf[o:o + len(d)] = d
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        sym_addr: list[int | None] = []
        for s in syms:
            sh = s["st_shndx"]
            if isinstance(sh, int) and sh in place:
                sym_addr.append(place[sh] + s["st_value"])
            elif sh == "SHN_ABS":
                sym_addr.append(s["st_value"])
            elif sh == "SHN_UNDEF" and s.name:
                sym_addr.append(resolve(demangled.get(s.name, s.name)))
            else:
                sym_addr.append(None)
            if s["st_info"]["type"] == "STT_FUNC" and isinstance(sh, int) and sh in place:
                name = demangled.get(s.name, s.name)
                m = name.split("(")[0]
                if m.startswith("fn_") and len(m) == 13:
                    lo.functions[int(m[3:], 16)] = (sym_addr[-1], s["st_size"])
                elif address_of is not None:
                    there = address_of(s.name)
                    if there is not None and there not in lo.functions:
                        lo.functions[there] = (sym_addr[-1], s["st_size"])
            if s.name.startswith("_ZGV") and isinstance(sh, int) and sh in place:
                lo.guards.append(sym_addr[-1])
        # Named symbols by section, to name what a section-relative reference points at (anchor).
        named: dict[int, list] = {}
        if anchor is not None:
            for s in syms:
                sh = s["st_shndx"]
                if isinstance(sh, int) and sh in place and s.name and not s.name.startswith("$") \
                        and s["st_info"]["type"] != "STT_SECTION":
                    named.setdefault(sh, []).append((s["st_value"], max(s["st_size"], 1), s.name, s["st_info"]["type"]))

        def covering(sh: int, off: int) -> tuple:
            """(symbol, offset within it, type) of the named symbol covering `off` in section `sh`,
            (None, off, None) when none does. A function is preferred to a label inside it."""
            best = None
            for value, size, name, kind in named.get(sh, ()):
                if value <= off < value + size and (best is None or kind == "STT_FUNC"):
                    best = (name, off - value, kind)
            return best or (None, off, None)

        got: dict[int, int] = {}
        got_base = (addr + 15) & ~15
        got_words: list[int] = []

        def got_slot(target: int) -> int:
            if target not in got:
                got[target] = got_base + 8 * len(got_words)
                got_words.append(target)
            return got[target]

        for rs in elf.iter_sections():
            if not isinstance(rs, RelocationSection) or rs["sh_info"] not in place:
                continue
            base = place[rs["sh_info"]]
            for r in rs.iter_relocations():
                t, P = r["r_info_type"], base + r["r_offset"]
                s = syms[r["r_info_sym"]]
                S = sym_addr[r["r_info_sym"]]
                o = P - LIFT_BASE
                if S is None:
                    lo.unresolved.setdefault(P, []).append(demangled.get(s.name, s.name))
                    continue
                v = S + r["r_addend"]
                sh = s["st_shndx"]
                if anchor is not None and isinstance(sh, int) and sh in place:
                    if s["st_info"]["type"] == "STT_SECTION":
                        name, off, kind = covering(sh, r["r_addend"])
                    else:
                        name, off, kind = s.name, r["r_addend"], s["st_info"]["type"]
                    here = covering(rs["sh_info"], r["r_offset"])[0]
                    if not (kind == "STT_FUNC" and (name == here or off != 0)):
                        there = anchor(name, all_secs[sh], off, kind)
                        if there is False:
                            what = demangled.get(name, name) if name else f"{all_secs[sh].name}+{off:#x}"
                            lo.unresolved.setdefault(P, []).append(f"{what} (no address in the original)")
                            continue
                        if there is not None:
                            v = there
                ins = struct.unpack_from("<I", buf, o)[0] if o + 4 <= len(buf) else 0
                if t in (282, 283):  # JUMP26, CALL26: the target is kept aside (it may be far)
                    lo.branch[P] = v
                elif t == 275:  # ADR_PREL_PG_HI21
                    d = ((v & ~0xFFF) - (P & ~0xFFF)) >> 12
                    ins = (ins & 0x9F00001F) | ((d & 3) << 29) | (((d >> 2) & 0x7FFFF) << 5)
                    struct.pack_into("<I", buf, o, ins)
                elif t == 274:  # ADR_PREL_LO21
                    d = v - P
                    ins = (ins & 0x9F00001F) | ((d & 3) << 29) | (((d >> 2) & 0x7FFFF) << 5)
                    struct.pack_into("<I", buf, o, ins)
                elif t in (277, 278, 284, 285, 286, 299):  # ADD/LDST*_ABS_LO12_NC
                    scale = {277: 0, 278: 0, 284: 1, 285: 2, 286: 3, 299: 4}[t]
                    ins = (ins & ~(0xFFF << 10)) | ((((v & 0xFFF) >> scale) & 0xFFF) << 10)
                    struct.pack_into("<I", buf, o, ins)
                elif t == 311:  # ADR_GOT_PAGE
                    slot = got_slot(v)
                    d = ((slot & ~0xFFF) - (P & ~0xFFF)) >> 12
                    ins = (ins & 0x9F00001F) | ((d & 3) << 29) | (((d >> 2) & 0x7FFFF) << 5)
                    struct.pack_into("<I", buf, o, ins)
                elif t == 312:  # LD64_GOT_LO12_NC
                    slot = got_slot(v)
                    ins = (ins & ~(0xFFF << 10)) | ((((slot & 0xFFF) >> 3) & 0xFFF) << 10)
                    struct.pack_into("<I", buf, o, ins)
                elif t == 257:  # ABS64
                    struct.pack_into("<Q", buf, o, v & 0xFFFFFFFFFFFFFFFF)
                elif t in (279, 280):  # TSTBR14, CONDBR19 (local)
                    d = (v - P) >> 2
                    if t == 279:
                        ins = (ins & ~(0x3FFF << 5)) | ((d & 0x3FFF) << 5)
                    else:
                        ins = (ins & ~(0x7FFFF << 5)) | ((d & 0x7FFFF) << 5)
                    struct.pack_into("<I", buf, o, ins)
                else:
                    lo.unresolved.setdefault(P, []).append(f"relocation type {t}")
        if got_words:
            buf.extend(b"\0" * (got_base - addr))
            for w in got_words:
                buf.extend(struct.pack("<Q", w & 0xFFFFFFFFFFFFFFFF))
            lo.data = (lo.data[0], got_base + 8 * len(got_words))
    for k in range(0, len(buf), PAGE):
        lo.mem[(LIFT_BASE + k) // PAGE] = bytearray(buf[k:k + PAGE].ljust(PAGE, b"\0"))
    for start, size in lo.functions.values():
        for a in range(start, start + size, 4):
            lo.words.add(_word(lo, a))
    return lo


def _word(lo: LiftObject, a: int) -> int:
    p = lo.mem[a // PAGE]
    return struct.unpack_from("<I", p, a % PAGE)[0]


def native(w: int) -> bool:
    """b, bl, br, blr, ret, adr, adrp and literal loads are executed here, not as P-code."""
    return (w & 0x7C000000) == 0x14000000 or (w & 0x1F000000) == 0x10000000 or (w & 0x3B000000) == 0x18000000 \
        or (w & 0xFF9FFC1F) == 0xD61F0000


# --- the machine -----------------------------------------------------------------------------------

MAX_COMPARES = 64  # register compares a tracked run records (Machine.compares)

class Machine:
    res_read: set | None = None  # (__init__; a Machine built bare, as the tests do, tracks nothing)
    res_written: set = frozenset()
    res_sret = False  # the last call returned through x8
    # The function under test's own abi_facts (port_lift.machine, the original's run): an indirect
    # call with x8 still as it came in is passed x8 when the function's callers set it (Facts.fwd).
    own_facts = None
    # port_lift --small-out-counts (off: both stay as here, and nothing below changes a run). The
    # original's run tracks which made-up out-parameter words (keys (call, argument, offset), as
    # ("o", key) overrides) its code then compares with a non-constant or scales into an address,
    # and never dereferences (count_keys); a run given small_outs fills those words with the small
    # value there instead of the seed's, the lift's run the same (Oracle.run_original, check).
    count_track = False
    small_outs: dict = {}
    # Junk tracking (port_lift --junk-scratch; off: nothing below is ever filled, the runs are as
    # without it). After every call leaving the function, JUNK_BYTES are undefined in that run: its
    # code, compiled for AAPCS64, may not rely on them, and a value it computes from them is
    # undefined too (junk_vn: register and unique bytes, junk_mem: memory bytes, carried by every
    # P-code op; exec). Where such a value would decide something compared (a call's argument, a
    # stored byte outside the frame, a branch, a load or store address, a modelled C function's
    # input, the result) the run records the event it would reach (junk_at, junk_use): compare()
    # skips the input from the original's such event on, never agrees or rejects on it (the lift's
    # are recorded, not used: its run is compared as without tracking).
    junk_scratch = False
    junk_vn: set = frozenset()  # (__init__; a bare Machine tracks nothing)
    junk_mem: set = frozenset()
    junk_at: int | None = None
    junk_why: str | None = None
    sreads: set | None = None  # (__init__; the arguments read as s<n>, Machine.call)
    entry_v: dict = {}  # (setup; never written in place)
    vtag: dict = {}  # (setup; a bare Machine tracks no argument copies)
    vwide: set = frozenset()
    _vcopy = False

    def __init__(self, db: dict, image: ImageMemory, seed: int, inside, code_word, lift: LiftObject | None,
                 imports: dict[int, str], facts, preset: dict[int, int], dictionary: tuple = (),
                 overrides: dict | None = None, track: bool = False):
        self.dictionary = dictionary  # constants of the original (DICT_PROFILE seeds draw from them)
        # An input changed from what the seed gives (the oracle's mutations): ("r", key) -> register
        # value, ("m", address of an 8-byte word) -> memory word, ("c", n) -> the n-th call's x0
        # result; and, with track, the memory words the run read before writing them, in order
        # (what a mutation can change next).
        self.overrides = overrides or {}
        # ("s", path): the input starts from that snapshot instead of the seed's registers and memory
        path = next((k for kind, k in self.overrides if kind == "s"), None)
        self.snap = load_snapshot(path, max(image.end, image.bss_end) - BASE) if path else None  # (.bss too)
        # The snapshot's recorded calls, replayed in order while each call goes where the recorded one
        # went (replay_end: why replaying stopped, the calls after it made up as without a snapshot)
        self.replay_i, self.replay_end = 0, None
        # (room for every recorded call: the real run made them all, the last before its return)
        self.calls_room = len(self.snap.calls) if self.snap is not None else 0
        self.mem_ovr: dict[int, list[tuple[int, int]]] = {}
        for (kind, k), v in self.overrides.items():
            if kind == "m":
                self.mem_ovr.setdefault(k // PAGE, []).append((k % PAGE, v))
        self.track, self.read_words, self._read_seen = track, [], set()
        self.read_at: dict[int, int] = {}  # each read word's byte where it was first read (a mutation goes there)
        # (each read word's coverage when it was first read: a run cut off in a loop read its bound
        # before its last new instruction, the loop's own reads after it, Oracle.originals)
        self.read_cov: list[int] = []
        self.ops_db, self.userops, self.op = db["ops"], db["userops"], db["opcodes"]
        self.image, self.seed, self.inside, self.code_word, self.lift = image, seed, inside, code_word, lift
        self.imports, self.facts = imports, facts
        self.regs = bytearray(REG_SIZE)
        self.uniq: dict[int, int] = {}
        self.pages: dict[int, bytearray] = {}
        self.dirty: dict[int, int] = {}
        # a written byte's reference (its value before the stretch's first write, None: none) and whether
        # it was the function's write then (its dirty value, None: unchanged), store
        self.before: dict[int, tuple[int | None, int | None]] = {}
        # The bytes the function loaded since the last call event (None: no call event yet). A value
        # loaded before a call and stored back after it is a write: the callee may have changed the
        # byte in between (store).
        self._loaded: set[int] | None = None
        # The bytes a put-back after a call left unchanged, with the value stored (one run may have
        # loaded a byte and stored a constant equal to it, the other only stored the constant: store
        # cannot tell a put-back from a coincidence), and those held at each event (compare).
        self.putback: dict[int, int] = {}
        self.putback_ev: dict[int, dict[int, int]] = {}
        # Every byte the function loaded, and the bytes written this stretch that it loaded only before
        # the last call (perhaps a stale value stored back), at each event: such a write is no match
        # for the other run's put-back (compare).
        self._all_loads: set[int] = set()
        self.stale: set[int] = set()
        self.stale_ev: dict[int, set[int]] = {}
        # The bytes loaded before the last call (all stretches but this one), and the bytes written
        # this stretch with a value store cannot tell from no change (a put-back of a byte loaded both
        # before the call and since: the value may be the stale one), at each event: a difference only
        # there, against a run that left the byte alone, decides nothing (compare, putback_only).
        self._loaded_before: set[int] = set()
        self.unsure: set[int] = set()
        self.unsure_ev: dict[int, set[int]] = {}
        self._ckpt: dict[int, int] = {}  # dirty as the last checkpoint saw it, and the bytes touched since
        self._touched: set[int] = set()
        self.cut: str | None = None  # why the run ended as a time-out: steps, events or snapshot (compare)
        self.clipped = False  # a modelled memset or memcpy stopped at 64 KiB (compare)
        self.fault: int | None = None  # the address a run faulted on
        self.fault_pc: int | None = None  # and the instruction that did (the oracle's repairs)
        self.fault_access: tuple | None = None  # and the access: ("load" | "store", its size) (compare)
        self.noret_call: int | None = None  # the call (its number) the run ended on, a callee that never returns
        self.noret_at: int | None = None  # the lift's run: the original's noret_call
        self.stack_written: set[int] = set()
        # Undefined bytes: loaded from stack the run never wrote (a struct copy's padding), and
        # where they were stored. Register and unique bytes holding one ((space, offset) keys);
        # memory bytes stored from one, which compare() leaves out (undefined in C: a lift that
        # copies the fields alone, or writes the padding, is the same program).
        self.undef_vn: set[tuple[int, int]] = set()
        self.undef_mem: set[int] = set()
        self.junk_vn: set[tuple[int, int]] = set()  # (Machine.junk_scratch)
        self.junk_mem: set[int] = set()
        self.junk_at, self.junk_why = None, None
        self.frame_record: set[int] = set()  # stack words holding the saved return address (store)
        self.saves: set[int] = set()  # stack words holding a callee-saved register's entry value (store)
        self.entry_saved: set[int] = set()  # those entry values (setup: x19-x28, d8-d15)
        # The save slots themselves (exec STORE): a word the function stored from x19-x28 or d8-d15
        # (or a unique copied from one) while the register held its entry value, with that value. Not
        # by value alone: a snapshot's entry x19 or d8 is often 0, and a zeroed local is no save.
        self.entry_regs: dict[int, int] = {}  # callee-saved register offset -> its entry value (setup)
        self.save_slots: dict[int, int] = {}
        self.save_src: dict[int, int] = {}  # unique offset -> the entry value it copied from such a register
        self.taken: set[int] = set()  # stack addresses the code made (`add xN, sp, #k`): local objects' starts
        self.out_at: dict[int, int] = {}  # stack bytes an out-parameter filled: the argument's address (call)
        self.own_stack: set[int] = set()  # stack bytes the function's own code stored (exec)
        # The original's calls, for the lift's run: which arguments it wrote out-parameters
        # through and how far (call_log); the lift's Machine gets them as `guide` (port_lift).
        self.call_log: list[tuple[frozenset, dict]] = []
        self.guide: list | None = None
        self.arg_slots: int | None = None  # the function's stack argument slots (load: the caller's frame)
        self.state = {}  # argument register key -> "entry" | "written" | "junk"
        # Since the last call: which of its result registers (x0: 0, v0: 100) the run read while they
        # still held what the call left, and which it has written since (None before any call).
        # compare(): a result in the class the function itself did not read is garbage (AAPCS64).
        self.res_read: set | None = None
        self.res_written: set = set()
        self.vnarrow: dict[int, bool] = {}  # v<n> -> its last write was s<n> (4 bytes at its start)
        self.vnarrow_at: dict[int, int] = {}  # v<n> -> the instruction (step) of that s<n> write
        self.entry_v: dict[int, int] = {}  # v<n> (n < 8) -> its low 64 bits at the entry (setup)
        # Where the function's own float arguments went: v<n> -> j while v<n> holds v<j> as it came in,
        # unchanged or through whole copies only (`mov v8.16b, v0.16b`, `fmov d8, d0`: vector_copy);
        # any other write to v<n> ends it. The arguments j such a register was read as s<n> (4 bytes
        # at its start) and those read otherwise (8 bytes or more, a lane above the first: a double
        # or a vector, not an f32) other than by a whole copy (Machine.call).
        self.vtag: dict[int, int] = {}
        self.sreads: set = set()
        self.vwide: set = set()
        self.out_words: list = []  # ((call, argument, offset), word): the out-parameter words written
        # (count_track: per byte, (space, offset) of a register or unique and an address of memory,
        # the out-parameter words its value came from, key -> 0 as it was or 1 scaled (an index);
        # the keys compared with a non-constant or scaled into an address, and those dereferenced)
        self.ct_vn: dict = {}
        self.ct_mem: dict = {}
        self.count_uses: set = set()
        self.count_deref: set = set()
        self.results: list = []  # each call's x0, the first MAX_RESULTS_KEPT (a crash on one: Oracle.originals)
        self.events: list = []
        self.snapshot_bytes = 0  # written bytes copied into events so far (checkpoint)
        self.calls = 0
        self.steps = 0
        self.scale = 1  # MAX_STEPS, MAX_EVENTS and MAX_SNAPSHOT times this (port_lift raises it to retry a cut-off run)
        self.visited: set[int] = set()
        self.outcomes: set[tuple[int, bool]] = set()  # (address of a conditional branch, taken)
        # (address, first value, second value) of each register-to-register compare (`cmp x, x`), the first
        # MAX_COMPARES, when reads are tracked: what a caller sets equal to reach a pointer-equality branch.
        self.compares: list[tuple[int, int, int]] = []
        for a, v in preset.items():
            self.store(a, 1, v, track=False)
        self.own = lift.data if lift else (0, 0)
        self.lift_starts = {s for s, _ in lift.functions.values()} if lift else set()
        self.target_reg = None

    # memory
    def page(self, n: int) -> bytearray:
        p = self.pages.get(n)
        if p is not None:
            return p
        a = n * PAGE
        if self.lift is not None and n in self.lift.mem:
            p = bytearray(self.lift.mem[n])
        elif self.snap is not None and n in self.snap.pages and not STACK_LO <= a < STACK_TOP:
            p = bytearray(self.snap.pages[n])
        elif self.image.text[0] <= a < self.image.end:
            off = a - BASE
            p = bytearray(self.image.data[off:off + PAGE].ljust(PAGE, b"\0"))
        elif STACK_LO <= a < ENTRY_SP:
            p = bytearray(PAGE)
        else:
            p = bytearray(PAGE)
            for k in range(0, PAGE, 8):
                struct.pack_into("<Q", p, k, self.word(mix(self.seed, a + k)) & 0xFFFFFFFFFFFFFFFF)
        for off, v in self.mem_ovr.get(n, ()):
            if not (self.lift is not None and n in self.lift.mem) and not STACK_LO <= a < ENTRY_SP:
                struct.pack_into("<Q", p, off, v & 0xFFFFFFFFFFFFFFFF)
        self.pages[n] = p
        return p

    def word(self, h: int) -> int:
        """A memory word for hash h under this seed's profile; every DICT_PROFILE-th seed takes
        two words in five from the function's own constants (paths behind `if (kind == 0x2c)`)."""
        if self.dictionary and self.seed % DICT_PROFILE == DICT_PROFILE - 1 and h % 5 < 2:
            return self.dictionary[(h >> 8) % len(self.dictionary)]
        return word_value(h, self.seed)

    def load(self, a: int, size: int) -> int:
        a &= 0xFFFFFFFFFFFFFFFF
        if a < NULL_PAGE:
            raise Fault(a, "load", size)
        if self.arg_slots is not None and ENTRY_SP + 8 * self.arg_slots <= a < STACK_TOP + (1 << 36):
            # Above the stack arguments: the caller's frame, which the two runs place differently
            # (an index from a callee's made-up result, a loop run past its array)
            raise Inconclusive("read in the caller's frame")
        self._all_loads.update(range(a, a + size))
        if self._loaded is not None:
            self._loaded.update(range(a, a + size))
        if self.track:
            w = a & ~7
            if w not in self._read_seen and not STACK_LO <= w < ENTRY_SP and not self.own[0] <= w < self.own[1]:
                self._read_seen.add(w)
                if w not in self.dirty and w not in self.stack_written:
                    self.read_words.append(w)
                    self.read_at[w] = a - w
                    self.read_cov.append(len(self.visited))
        o = a % PAGE
        if o + size <= PAGE:
            return int.from_bytes(self.page(a // PAGE)[o:o + size], "little")
        return int.from_bytes(bytes(self.page((a + i) // PAGE)[(a + i) % PAGE] for i in range(size)), "little")

    def peek(self, a: int, size: int) -> int:
        """Memory as load reads it, recording nothing: the oracle's repairs look at a finished run's
        words (port_lift), whose read_at, read_words and read_cov must stay as the run left them."""
        a &= 0xFFFFFFFFFFFFFFFF
        if a < NULL_PAGE:
            raise Fault(a, "load", size)
        return int.from_bytes(bytes(self.page((a + i) // PAGE)[(a + i) % PAGE] for i in range(size)), "little")

    def store(self, a: int, size: int, v: int, track: bool = True) -> None:
        a &= 0xFFFFFFFFFFFFFFFF
        if a < NULL_PAGE:
            raise Fault(a, "store", size)
        b = (v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        prior = [self.page(x // PAGE)[x % PAGE] for x in range(a, a + size)]
        for i in range(size):
            x = a + i
            self.page(x // PAGE)[x % PAGE] = b[i]
        if self.count_track and self.ct_mem:
            for x in range(a, a + size):
                self.ct_mem.pop(x, None)
        if self.junk_mem:  # (a defined value now: exec's STORE marks the bytes again when it was not)
            self.junk_mem.difference_update(range(a, a + size))
        if not track:
            return
        if STACK_LO <= a < ENTRY_SP:
            self.stack_written.update(range(a, a + size))
            if size == 8 and v & 0xFFFFFFFFFFFFFFFF == SENTINEL:
                self.frame_record.add(a)  # the saved return address: out-parameters never reach it
            elif size == 8 and v & 0xFFFFFFFFFFFFFFFF in self.entry_saved:
                self.saves.add(a)  # a callee-saved register's entry value: a save slot, not a local
        elif self.arg_slots and ENTRY_SP <= a < ENTRY_SP + 8 * self.arg_slots:
            pass  # (the function's own stack arguments: the callee's to change, which the lift keeps elsewhere)
        elif self.arg_slots is not None and STACK_TOP - (1 << 36) <= a < STACK_LO:
            # Below the stack, an sp-based index gone wild (fn_71010b98d0: a histogram indexed by a
            # made-up result, 4 GiB down): each run's lands relative to its own sp
            raise Inconclusive("store below the stack")
        elif self.arg_slots is not None and ENTRY_SP <= a < STACK_TOP:
            # Past its frame into the caller's: a local array overrun on a made-up count (fn_710014d1a0),
            # undefined, and the two frames are laid out differently (as load: the caller's frame)
            raise Inconclusive("store in the caller's frame")
        elif not (self.own[0] <= a < self.own[1]):
            # A byte holding again what it held before the function's first write to it is
            # unchanged: no write at all as far as anything after can tell. C lets a compiler drop a
            # store of the value just loaded from the same place (`ldp s0, s1, [x11, #4]` ...
            # `str s0, [x8, #4]` at the same address, fn_71002aa518), so a write counts only where
            # it changed the byte. Only within one stretch between call events, though: the reference
            # (the byte when this stretch first wrote it, or a value it loaded) must not predate a call
            # (checkpoint clears them), or `v = *p; f(p); *p = v` would let a lift drop the restoring
            # store (or add one) under a callee that is not recorded. None: no reference. A write
            # that puts back the reference leaves the byte as the stretch found it: changed or not as
            # it was then (a byte the function wrote before a call, loaded after it and stored back
            # is still the function's write, not a byte back at its value before the function).
            # After a call the reference is a value loaded since, which one run may have loaded and the
            # other not: a put-back there is the whole store's (a value loaded and stored back agrees in
            # every byte; a constant that shares a byte with what was loaded, `p->f = NAN` after reading
            # p->f, 710073ee48, is a write in each byte, as in a run that never loaded it).
            refs = []
            for i in range(size):
                x = a + i
                if x not in self.before:
                    ref = prior[i] if self._loaded is None or x in self._loaded else None
                    self.before[x] = ref, self.dirty.get(x)
                refs.append(self.before[x])
            whole = self._loaded is None or all(r is not None and b[i] == r for i, (r, _) in enumerate(refs))
            for i, (ref, was) in enumerate(refs):
                x = a + i
                if whole and ref is not None and b[i] == ref and was is None and self._loaded is not None \
                        and x in self._loaded_before:
                    # (loaded before the call and again since: `old = *p; f(); (void)*p; *p = old`
                    # stores what the re-read found only because the emulated callee left *p alone;
                    # the real one may change it. A write, and one no input can decide on.)
                    self.dirty[x] = b[i]
                    self.putback.pop(x, None)
                    self.unsure.add(x)
                elif whole and ref is not None and b[i] == ref:
                    self.unsure.discard(x)
                    if was is None:
                        self.dirty.pop(x, None)
                        if self._loaded is not None:
                            self.putback[x] = b[i]
                    else:
                        self.dirty[x] = was
                        self.putback.pop(x, None)
                else:
                    self.dirty[x] = b[i]
                    self.putback.pop(x, None)
                    if self._loaded is not None and x in self._all_loads and x not in self._loaded:
                        self.stale.add(x)
                    if self._loaded is not None and ref is not None and b[i] == ref and was is None:
                        # (a byte stored back as loaded since the call, in a store that changed
                        # others: `ldr w; orr #1; str w` against a lift's ldrb/orrb/strb, or a constant
                        # sharing a loaded byte, 710073ee48. A write, but the other run's leaving the
                        # byte alone is no difference an input can show: unsure, compare.)
                        self.unsure.add(x)
                    else:
                        self.unsure.discard(x)
            self._touched.update(range(a, a + size))

    # registers
    def reg(self, off: int, size: int) -> int:
        return int.from_bytes(self.regs[off:off + size], "little")

    def set_reg(self, off: int, size: int, v: int) -> None:
        self.regs[off:off + size] = (v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        if self.vtag and Q0 <= off + size and off < Q0 + 32 * QSTRIDE:
            for n in range(max(off - Q0, 0) // QSTRIDE, (off + size - 1 - Q0) // QSTRIDE + 1):
                self.vtag.pop(n, None)
        if self.count_track and self.ct_vn:
            for i in range(size):
                self.ct_vn.pop((2, off + i), None)
        if self.undef_vn:
            self.undef_vn.difference_update((2, off + i) for i in range(size))
        if self.junk_vn:
            self.junk_vn.difference_update((2, off + i) for i in range(size))
        if X0 <= off < X0 + 72:
            self.state[(off - X0) // 8] = "written"
            if off < X0 + 8 and self.res_read is not None:
                self.res_written.add(0)
        elif Q0 <= off < Q0 + 8 * QSTRIDE:
            self.state[100 + (off - Q0) // QSTRIDE] = "written"
            if off < Q0 + QSTRIDE and self.res_read is not None:
                self.res_written.add(100)
            n, rel = divmod(off - Q0, QSTRIDE)
            if rel == 0:
                self.vnarrow[n] = size == 4
                if size == 4:
                    self.vnarrow_at[n] = self.steps
            elif not (4 <= rel < QSTRIDE and v == 0 and self.vnarrow_at.get(n) == self.steps):
                # A lane insert after s<n> (ld1 {v0.s}[1], ins v0.d[1]) makes v<n> wide again; only the
                # zeroing of the upper part by the s<n> write's own instruction keeps it narrow (after
                # d<n> it is False). Ghidra zeroes it in four pieces: 4 bytes at +4, then 8 at +8, +16
                # and +24 (the SVE part). A later instruction's write there is a lane insert even when
                # its value is 0 (ins v0.s[1], wzr), and may leave anything in bits 32-63.
                self.vnarrow[n] = False

    def x(self, n: int) -> int:
        return self.reg(X0 + 8 * n, 8)

    def read(self, vn) -> int:
        sp, off, size = vn
        if sp == 0:
            return off
        if sp == 2:
            if self.res_read is not None and X0 <= off < Q0 + QSTRIDE:
                if off < X0 + 8 and 0 not in self.res_written:
                    self.res_read.add(0)
                elif off >= Q0 and 100 not in self.res_written:
                    self.res_read.add(100)
            v = int.from_bytes(self.regs[off:off + size], "little")
            if self.vtag and not self._vcopy and Q0 <= off < Q0 + 32 * QSTRIDE:
                n, rel = divmod(off - Q0, QSTRIDE)
                j = self.vtag.get(n)
                if j is not None and self.reg(Q0 + QSTRIDE * n, 8) == self.entry_v.get(j):
                    # (an argument's s<n> read, or any other: Machine.call, a whole copy of an f32)
                    (self.sreads if rel == 0 and size <= 4 else self.vwide).add(j)
            return v
        if sp == 1:
            return self.uniq.get(off, 0) & ((1 << (8 * size)) - 1)
        return self.load(off, size)

    def write(self, vn, v: int) -> None:
        sp, off, size = vn
        v &= (1 << (8 * size)) - 1
        if sp == 2:
            self.set_reg(off, size, v)
        elif sp == 1:
            self.uniq[off] = v
            if self.count_track and self.ct_vn:
                for i in range(size):
                    self.ct_vn.pop((1, off + i), None)
            if self.save_src:
                self.save_src.pop(off, None)
            if self.undef_vn:
                self.undef_vn.difference_update((1, off + i) for i in range(size))
            if self.junk_vn:
                self.junk_vn.difference_update((1, off + i) for i in range(size))
        elif sp == 3:
            self.store(off, size, v)
        else:
            raise Inconclusive("write to a constant")

    # -- setup and calls ------------------------------------------------------------------------

    def setup(self) -> None:
        rnd = random.Random(mix(self.seed, 1))
        mode = self.seed % 3
        for i in range(31):
            v = mix(self.seed, 100 + i) >> 1
            if i <= 8:
                if mode == 0 or i in (0, 8) or (mode == 2 and rnd.random() < 0.5):
                    v = ARENA + 0x100000 * (self.seed * 16 + i + 1) + 0x100
                elif self.dictionary and rnd.random() < 0.5:
                    v = rnd.choice(self.dictionary)
                else:
                    v = rnd.randrange(8)
            self.regs[X0 + 8 * i:X0 + 8 * i + 8] = v.to_bytes(8, "little")
        for i in range(32):
            if self.seed % 2:
                bits = f_to(rnd.uniform(-100, 100), 4)
            else:
                bits = f_to(rnd.uniform(-100, 100), 8)
            self.regs[Q0 + QSTRIDE * i:Q0 + QSTRIDE * i + 16] = bits.to_bytes(16, "little")
        if self.snap is not None:
            for i in range(29):
                self.regs[X0 + 8 * i:X0 + 8 * i + 8] = (self.snap.x[i] & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "little")
            for i in range(32):
                self.regs[Q0 + QSTRIDE * i:Q0 + QSTRIDE * i + 16] = (self.snap.q[i] & (1 << 128) - 1).to_bytes(16, "little")
            for k, b in enumerate(self.snap.stack):  # (the stack arguments, at our entry sp)
                self.page((ENTRY_SP + k) // PAGE)[(ENTRY_SP + k) % PAGE] = b
        self.regs[X0 + 8 * 30:X0 + 8 * 31] = SENTINEL.to_bytes(8, "little")
        self.regs[X0 + 8 * 29:X0 + 8 * 30] = bytes(8)
        self.regs[SP:SP + 8] = ENTRY_SP.to_bytes(8, "little")
        for (kind, k), v in self.overrides.items():
            if kind == "r" and k < 100:
                self.regs[X0 + 8 * k:X0 + 8 * k + 8] = (v & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "little")
            elif kind == "r":
                self.regs[Q0 + QSTRIDE * (k - 100):Q0 + QSTRIDE * (k - 100) + 16] = (v & (1 << 128) - 1).to_bytes(16, "little")
        self.state = {k: "entry" for k in list(range(9)) + list(range(100, 108))}
        self.entry_saved = {self.x(i) for i in range(19, 29)} | {self.reg(Q0 + QSTRIDE * i, 8) for i in range(8, 16)}
        self.entry_regs = {X0 + 8 * i: self.x(i) for i in range(19, 29)}
        self.entry_regs.update({Q0 + QSTRIDE * i: self.reg(Q0 + QSTRIDE * i, 8) for i in range(8, 16)})
        self.entry_v = {i: self.reg(Q0 + QSTRIDE * i, 8) for i in range(8)}
        self.vtag = {i: i for i in range(8)}

    def canonical(self, target: int):
        name = self.imports(target)
        return ("import", name) if name else target

    def call(self, target: int) -> None:
        """A call leaving the function: modeled C library functions run; others are recorded."""
        t = self.canonical(target)
        modeled = isinstance(t, tuple) and (t[1] in MODELED or t[1] in PURE_MATH)
        rec = self.recorded(target, t, modeled)
        if modeled:
            if t[1] in PURE_MATH:
                self.math(t[1])
            else:
                self.clib(t[1])
            self.res_read, self.res_written, self.res_sret = set(), set(), False
            return
        # Every argument register's value; `sel` the ones this run's caller meant for the callee.
        # compare() takes the original's selection, so registers the lift's compiled code used as
        # scratch do not count, and one the original set must hold the same value in the lift.
        f = self.facts(target)
        args, sel, raw = [], [], {}  # (raw: the integer arguments' values, for the out-parameters)
        compare_only = set()  # (selected to be compared, never written through: see x8 below)
        for k in list(range(9)) + list(range(100, 108)):
            st = self.state.get(k, "junk")
            if k < 100:
                raw[k] = self.x(k)
                args.append((k, self.stackref(raw[k])))
            else:
                args.append((k, self.reg(Q0 + QSTRIDE * (k - 100), 8)))
            if st == "junk" or k == self.target_reg:
                continue
            if f is not None:  # a known callee: exactly the registers it reads
                if k not in f.args:
                    continue
            elif k == 8:  # unknown: x8 when it was set to a local (a result built there)
                if st == "entry" and 8 in getattr(self.own_facts, "fwd", ()):
                    # (or x8 as the function was given it, when its callers point it at their frame:
                    # a virtual-call wrapper's indirect result, passed on. Compared only: the
                    # out-parameters, the guide and res_sret stay as without it.)
                    compare_only.add(8)
                elif st != "written" or not STACK_LO <= self.x(8) < ENTRY_SP:
                    continue
            elif st in ("entry", "stale") and k != 0:  # unknown: x0 and what this block set for it
                continue
            elif k >= 100 and isinstance(t, tuple):  # an import of unknown signature: integers only
                continue
            # A register the callee reads only as w<n> or s<n> has an unspecified upper half; so has
            # one this run last wrote as s<n> (the caller's f32: a lift may pass anything above it
            # even where the callee copies the register whole into an indirect call, 7100656f3c).
            narrow = 100 <= k < 108 and st == "written" and self.vnarrow.get(k - 100, False)
            if 100 <= k < 108 and st == "written" and not narrow and self.own_facts is not None and self.sreads:
                # So has a whole copy (`mov v0.16b, v8.16b`) of the function's own float argument
                # v<j> as it came in, when this run read that argument (itself or a whole copy of
                # it) as s<n> and never otherwise (an f32: the caller's bits above it are
                # unspecified; 71010df1fc passes its s0 on through v8). By where the register's
                # value came from (vtag), not by its value: an unrelated s<n> read holding the same
                # low word decides nothing, and an argument also read as a double or as a vector's
                # other lanes is not an f32, compared whole when the callee reads it whole.
                j = self.vtag.get(k - 100)
                narrow = j is not None and j in self.sreads and j not in self.vwide \
                    and 100 + j in self.own_facts.args \
                    and self.reg(Q0 + QSTRIDE * (k - 100), 8) == self.entry_v.get(j)
            sel.append((k, 32 if narrow or (f is not None and (k < 9 or 100 <= k < 108) and k not in f.wide) else 64))
        # Arguments beyond the eight registers: the callee's stack slots at [sp + 8i] (key 200 + i),
        # 32 bits each (a 32-bit argument leaves the upper half of its slot unspecified).
        sp = self.reg(SP, 8)
        for i in range(getattr(f, "stack_slots", 0) if f is not None else 0):
            raw[200 + i] = self.load(sp + 8 * i, 8)
            args.append((200 + i, self.stackref(raw[200 + i])))
            sel.append((200 + i, 32))
        if self.junk_vn or self.junk_mem:
            self.junk_args(sel, raw, args)
        self.checkpoint(("call", t, tuple(sel), tuple(args)))
        self.target_reg = None
        self.calls += 1
        n = self.calls
        chosen = {k for k, _ in sel} - compare_only
        extent = {k: v[2] for k, v in args if isinstance(v, tuple) and v[0] == "stack"}
        # A recorded call's out-parameters: the bytes it changed in this frame, by argument and offset
        # from it (the original's run lays its frame out as the real one did; the lift's takes them from
        # the guide, as it does the arguments)
        outs: dict = {}
        if rec is not None and self.lift is None:
            fb = {off + i: b for off, run in rec.frame for i, b in enumerate(run)}
            for k, v in args:
                if k in chosen and isinstance(v, tuple) and v[0] == "stack" and k in extent:
                    o = raw[k] - ENTRY_SP
                    outs[k] = [(j, fb[o + j]) for j in range(max(extent[k], 8)) if o + j in fb and o + j < 0]
        self.call_log.append((frozenset(chosen), extent, outs))
        if self.guide is not None and n <= len(self.guide):
            # (the lift's run: through the arguments the original's call chose, as far as its
            # objects reach, since this frame's layout and the scratch values it passes differ)
            chosen, extent = self.guide[n - 1][0], {**extent, **self.guide[n - 1][1]}
            outs = self.guide[n - 1][2] if len(self.guide[n - 1]) > 2 else {}
        for k, v in args:  # out-parameters: through the arguments only (scratch registers differ)
            if k in chosen and isinstance(v, tuple) and v[0] == "stack" and k in extent:
                p = raw[k]
                if rec is not None:  # (what the real callee wrote, in place of the pattern)
                    for j, b in outs.get(k, ()):
                        if p + j < ENTRY_SP and not any(p + j - 7 <= r <= p + j + 7 for r in self.frame_record):
                            self.store(p + j, 1, b)
                            self.out_at[p + j] = p
                    continue
                # (within the object, stackref's extent, and never over the frame record: a small
                # local below it would let the pattern overwrite the saved return address). Words of
                # the seed's memory profile: a returned {pointer, count} gets a count a loop ends on.
                # (x8, a result built in place: the struct may be larger than the extent the frame
                # suggests, and the function copies all of it out, 7100128810. The window past the
                # extent holds what the callee wrote there, unknown: undefined, so what is copied
                # from it is not compared; filling it would overwrite locals placed differently. It
                # stops where this run's own frame holds another object: the next taken address, a save
                # slot or the frame record (the guide's extent is the other frame's), or a byte this run
                # filled as another argument's out-parameter: cleared, a lift's different store to a
                # local placed there would go uncompared. Nor past a byte this run's own code stored
                # with nothing filled through x8 over it since: a local whose address is taken only
                # later, 710082a500's float passed by pointer after the call, sits there in the lift.)
                if k == 8:
                    stop = min([t for t in self.taken | self.frame_record | self.saves if t > p] + [p + WINDOW, ENTRY_SP])
                    own = getattr(self, "own_stack", ())
                    for q in range(p + max(extent[k], 8), stop):
                        if self.out_at.get(q, p) != p or (q in own and q not in self.out_at):
                            break
                        self.stack_written.discard(q)
                for j in range(0, max(extent[k], 8), 8):
                    if p + j >= ENTRY_SP or any(p + j - 7 <= r <= p + j + 7 for r in self.frame_record):
                        break
                    # (every other seed 64 random bits a slot, as before: a run that ends on those;
                    # ("o", (n, k, j)) overrides one, for the oracle's repairs, out_words lists them)
                    v = self.overrides.get(("o", (n, k, j)),
                                           self.word(mix(self.seed, n, k, j)) if self.seed % 2 else mix(n, k, j))
                    if self.small_outs and ("o", (n, k, j)) not in self.overrides:
                        v = self.small_outs.get((n, k, j), v)  # (--small-out-counts: a count, small)
                    self.store(p + j, 8, v)
                    self.out_at.update((p + j + i, p) for i in range(8))
                    if self.count_track:
                        t = {(n, k, j): 0}
                        for i in range(8):
                            self.ct_mem[p + j + i] = t
                    if len(self.out_words) < 64:
                        self.out_words.append(((n, k, j), v))
        if self.count_track and self.ct_vn:  # (the registers the callee leaves garbage, and x0/v0)
            for key in [key for key in self.ct_vn if key[0] == 2 and (X0 <= key[1] < X0 + 8 * 19 or Q0 <= key[1]
                                                                     or NG <= key[1] <= OV)]:
                del self.ct_vn[key]
        for i in CALLER_SAVED_X:
            self.regs[X0 + 8 * i:X0 + 8 * i + 8] = (mix(n, 7, i) >> 1).to_bytes(8, "little")
            if i <= 8:
                self.state[i] = "junk"
        for i in CALLER_SAVED_Q:
            self.regs[Q0 + QSTRIDE * i:Q0 + QSTRIDE * i + 16] = mix(n, 9, i).to_bytes(16, "little")
            if i < 8:
                self.state[100 + i] = "junk"
        for k in range(NG, OV + 1):
            self.regs[k] = 0
        self.junk_clobber()
        # (a mutation may set it)
        ret = self.overrides.get(("c", n), rec.x[0] if rec is not None else self.word(mix(n, 11, self.seed)))
        self.regs[X0:X0 + 8] = (ret & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "little")
        if len(self.results) < MAX_RESULTS_KEPT:
            self.results.append(ret & 0xFFFFFFFFFFFFFFFF)
        if rec is not None:
            self.regs[X0 + 8:X0 + 16] = (rec.x[1] & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "little")
            for i, q in enumerate(rec.q):
                self.regs[Q0 + QSTRIDE * i:Q0 + QSTRIDE * i + 16] = (q & (1 << 128) - 1).to_bytes(16, "little")
            for a, run in rec.mem:  # (the callee's writes: not the function's, though they replace its bytes)
                for i, b in enumerate(run):
                    x = a + i
                    if x >= NULL_PAGE and not STACK_LO <= x < STACK_TOP:
                        self.page(x // PAGE)[x % PAGE] = b
                        self.before.pop(x, None)  # (a later write of the function's compares with this)
                        self.putback.pop(x, None)
                        if x in self.dirty:
                            self.dirty[x] = b
                            self._touched.add(x)
        else:
            self.regs[Q0:Q0 + 16] = f_to(n + 0.5, 4).to_bytes(16, "little")  # an f32 result (the game's floats)
        self.state[0] = "written"
        # A float argument counts only when the caller's own code set it: a result left in v0 and
        # read by the next callee is indistinguishable from garbage it does not read (liveness
        # over-approximates), so a float result passed straight on is not compared.
        self.state[100] = "junk"
        self.res_read, self.res_written = set(), set()
        # (a callee that reads x8, the indirect result location, returns through memory: x0 and v0
        # hold nothing after it. Only a callee whose facts say it reads x8: an unknown callee's x8
        # is chosen when it points at a local, which may be Clang's scratch for a frame address
        # before an ordinary call, and its x0 result must still be compared)
        self.res_sret = f is not None and 8 in f.args and 8 in chosen

    def recorded(self, target: int, t, modeled: bool):
        """The snapshot's next recorded call when it went to this target (and is taken), else None.
        A modelled C function the real run did not record (inlined, or run outside the recomp's code)
        leaves the list where it is; any other call elsewhere ends the replay."""
        calls = self.snap.calls if self.snap is not None else ()
        if self.replay_end is not None or self.replay_i >= len(calls):
            return None
        r = calls[self.replay_i]
        if r.target != target and self.canonical(r.target) != t:
            if not modeled:
                self.replay_end = f"call {self.calls + 1} to {target:#x}, the real run's to {r.target:#x}"
            return None
        self.replay_i += 1
        return r

    def stackref(self, v: int):
        """A stack address as ("stack", the bytes the function wrote from it on, extent, taken,
        hard); anything else as is. The extent is how far its object may reach: to the next local
        whose address the code took, a save slot or the frame record (beyond, the two frames hold
        different locals); the out-parameter pattern stays within it. `taken` holds the offsets
        from v of the addresses the code took within the window, `hard` the offset of the first
        save slot or frame record word: same_arg compares past a taken address both runs took at
        the same offset (a field's address, &s.name, passed before the whole object)."""
        if STACK_LO <= v < ENTRY_SP:
            hard = min([t for t in self.frame_record | self.saves if t > v] + [v + WINDOW])
            end = min([t for t in self.taken if t > v] + [hard])
            return ("stack", tuple(self.load(v + j, 1) if v + j in self.stack_written else None
                                   for j in range(WINDOW) if v + j < ENTRY_SP), end - v,
                    frozenset(t - v for t in self.taken if v < t < v + WINDOW), hard - v)
        return v

    def math(self, name: str) -> None:
        """A libm function computed from its arguments (s0/s1, d0/d1 without the f), as the C
        library does: no event, so a call the compiler merged, inlined (sqrtf as fsqrt) or moved
        does not shift the made-up results of the calls after it (fn_710069f728)."""
        size = 4 if name.endswith("f") else 8
        base = name[:-1] if size == 4 else name
        a, b = (f_from(self.reg(Q0 + QSTRIDE * i, size), size) for i in (0, 1))
        if self.junk_vn and any(self.junk_reg(Q0 + QSTRIDE * i, size)
                                for i in ((0, 1) if base in ("atan2", "pow", "fmod", "hypot", "nextafter") else (0,))):
            self.junk_use(f"an argument of {name}")
        two = {"atan2": math.atan2, "pow": math.pow, "fmod": math.fmod, "hypot": math.hypot,
               "nextafter": math.nextafter if size == 8 else nextafterf}
        one = {"sqrt": math.sqrt, "sin": math.sin, "cos": math.cos, "tan": math.tan, "asin": math.asin,
               "acos": math.acos, "atan": math.atan, "exp": math.exp, "exp2": lambda v: 2.0 ** v,
               "log": math.log, "log2": math.log2, "log10": math.log10, "floor": math.floor,
               "ceil": math.ceil, "round": lambda v: math.floor(abs(v) + 0.5) * (1 if v >= 0 else -1),
               "trunc": math.trunc, "fabs": abs, "sinh": math.sinh, "cosh": math.cosh, "tanh": math.tanh}
        # A NaN argument comes back with its payload, quieted (an inlined fsqrt keeps it, then calls
        # sqrtf only for the NaN: fn_7100118274), and the rounding functions return +-inf as is.
        # A two-argument function runs first: C99 gives pow(1, NaN) = 1 and hypot(inf, NaN) = inf.
        quiet = 0x00400000 if size == 4 else 0x0008000000000000
        args = (0, 1) if base in two else (0,)
        nan = next((i for i in args if math.isnan((a, b)[i])), None)
        odd = base == "pow" and math.isfinite(b) and b == int(b) and int(b) % 2 == 1
        bits = None
        if nan is not None and base not in two:
            bits = self.reg(Q0 + QSTRIDE * nan, size) | quiet
        elif base in ("floor", "ceil", "round", "trunc") and math.isinf(a):
            bits = f_to(a, size)
        else:
            try:
                v = float(two[base](a, b) if base in two else one[base](a))
            except OverflowError:  # (the true result's sign: sinh, tan by a's, pow(-x, odd) negative)
                neg = (base in ("sinh", "tan") and a < 0) or (odd and a < 0)
                v = -math.inf if neg else math.inf
            except (ValueError, ZeroDivisionError):
                if base in ("log", "log2", "log10") and a == 0:
                    v = -math.inf
                elif base == "pow" and a == 0 and b < 0:  # pow(+-0, y < 0): +inf, -inf for -0 and odd y
                    v = math.copysign(math.inf, a) if odd else math.inf
                else:
                    v = math.nan
            if base in ("floor", "ceil", "round", "trunc") and v == 0:  # (Python ints lose -0)
                v = math.copysign(0.0, a)
            bits = (self.reg(Q0 + QSTRIDE * nan, size) | quiet if nan is not None and math.isnan(v)
                    else f_to(v, size))
        for i in CALLER_SAVED_X:
            self.regs[X0 + 8 * i:X0 + 8 * i + 8] = bytes(8)
            if i <= 8:
                self.state[i] = "junk"
        for i in range(1, 8):
            self.state[100 + i] = "junk"
        # The result is computed the same way in both runs: a later call's argument (atan2f's
        # result passed on must be the same value in the lift).
        self.state[100] = "written"
        self.vnarrow[0] = size == 4
        self.regs[Q0:Q0 + 16] = bits.to_bytes(16, "little")
        # (the result's own bytes hold what the call gave; x0 and the rest of v0 are undefined)
        res = {(2, Q0 + j) for j in range(size)}
        self.junk_clobber(keep=res, junk=(JUNK_LIBC | {(2, X0 + j) for j in range(8)}) - res)

    def clib(self, name: str) -> None:
        a0, a1, a2 = self.x(0), self.x(1), self.x(2)
        if self.junk_vn and any(self.junk_reg(X0 + 8 * i, 8) for i in range({"strlen": 1, "strcmp": 2}.get(name, 3))):
            self.junk_use(f"an argument of {name}")
        # (the source bytes an undefined value was stored to: memcpy carries them, the others read them)
        jsrc = [a1 + i in self.junk_mem for i in range(min(a2, 1 << 16))] \
            if self.junk_mem and name in ("memcpy", "memmove") else None
        if name in ("memcpy", "memmove", "memset") and a2 > 1 << 16:
            self.clipped = True
        if name in ("memcpy", "memmove"):
            data = [self.load(a1 + i, 1) for i in range(min(a2, 1 << 16))]
            for i, b in enumerate(data):
                self.store(a0 + i, 1, b)
            r = a0
        elif name == "memset":
            for i in range(min(a2, 1 << 16)):
                self.store(a0 + i, 1, a1)
            r = a0
        elif name == "memcmp":
            r = 0
            for i in range(min(a2, 1 << 16)):
                d = self.load(a0 + i, 1) - self.load(a1 + i, 1)
                if d:
                    r = d
                    break
        elif name == "strlen":
            r = 0
            while self.load(a0 + r, 1) and r < 1 << 16:
                r += 1
        else:  # strcmp
            r, i = 0, 0
            while i < 1 << 16:
                c, d = self.load(a0 + i, 1), self.load(a1 + i, 1)
                if c != d or not c:
                    r = c - d
                    break
                i += 1
        # (the bytes it read from each argument, for junk tracking: an undefined one decides its result)
        nread = 0
        if self.junk_mem and name in ("memcmp", "strlen", "strcmp"):
            nread = min(a2, 1 << 16) if name == "memcmp" else r + 1 if name == "strlen" else i + 1
        for i in CALLER_SAVED_X:
            if i:
                self.regs[X0 + 8 * i:X0 + 8 * i + 8] = bytes(8)
                if i <= 8:
                    self.state[i] = "junk"
        for i in range(8):  # (a real one may use the vector registers: v0-v7 hold nothing for a later call)
            self.state[100 + i] = "junk"
        self.set_reg(X0, 8, r & 0xFFFFFFFFFFFFFFFF)
        if jsrc and any(jsrc):
            self.junk_post(("store", a0, jsrc))
        elif self.junk_mem and name in ("memcmp", "strlen", "strcmp"):
            if any(x + k in self.junk_mem for x in ((a0, a1) if name != "strlen" else (a0,)) for k in range(nread)):
                self.junk_use(f"{name} of undefined bytes")
        self.junk_clobber(keep=(), junk=JUNK_LIBC)

    def shrink(self) -> None:
        """Drop what only running needs (memory pages, the write sets): a finished run that is
        kept for comparison needs its events, registers, coverage and logs."""
        self.pages, self.dirty, self.stack_written, self.uniq, self._read_seen = {}, {}, set(), {}, set()
        self.before, self._loaded, self._all_loads, self.putback, self.stale = {}, None, set(), {}, set()
        self._ckpt, self._touched, self._loaded_before, self.unsure = {}, set(), set(), set()
        self.track = False  # (a later load, a repair's look at a word, must not move read_at: peek)
        self.__dict__.pop("exec", None)  # (track_counts' binding: the run is over)

    def checkpoint(self, event) -> None:
        if event[0] == "call" and isinstance(event[1], tuple) and event[1][1] in PURE_MATH:
            # (a libm function reads no memory: the compiler may move stores past it, fn_71001b83f8;
            # the next event compares them)
            self.events.append((event, ()))
            return
        if event[0] == "call":
            # (a callee may change any byte: what was loaded or held before it is no reference for a
            # later write's being no change, store)
            self._loaded_before |= self._all_loads if self._loaded is None else self._loaded
            self.before, self._loaded = {}, set()
        # The bytes the function's writes changed since the last checkpoint (None: no longer counted, an
        # undefined byte): the memory at an event is what all events up to it hold (written()), and
        # two runs agree at every event when they agree on each event's changes.
        d, ck = self.dirty, self._ckpt
        delta = tuple((k, d.get(k)) for k in sorted(self._touched) if d.get(k) != ck.get(k))
        for k, v in delta:
            if v is None:
                ck.pop(k, None)
            else:
                ck[k] = v
        self._touched = set()
        if self.putback:
            # (a put-back in this stretch; the next one starts after a call, which may change the byte)
            self.putback_ev[len(self.events)] = self.putback
            self.putback = {}
        if self.stale:
            self.stale_ev[len(self.events)] = self.stale
            self.stale = set()
        if self.unsure:
            self.unsure_ev[len(self.events)] = self.unsure
            self.unsure = set()
        self.events.append((event, delta))
        # (the budget counts a copy of all written memory per event, as the runs were limited before
        # events held changes; a run replaying recorded calls counts the changes)
        self.snapshot_bytes += len(delta) if self.calls_room else len(self.dirty)
        if len(self.events) >= self.max_events() or self.snapshot_bytes > MAX_SNAPSHOT * self.scale:
            # Each event keeps a copy of the memory written so far: a loop calling on every
            # iteration would hold thousands. The run ends here as a time-out (compare: what came
            # before must agree).
            self.cut = self.cut or ("events" if len(self.events) >= self.max_events() else "snapshot")
            self.steps = self.max_steps()

    def max_steps(self) -> int:
        return MAX_STEPS * self.scale + STEPS_PER_CALL * self.calls_room

    def max_events(self) -> int:
        return MAX_EVENTS * self.scale + self.calls_room

    # -- execution ------------------------------------------------------------------------------

    def run(self, entry: int) -> None:
        """Run from `entry` until the return to SENTINEL; events end with ("end", kind, detail)."""
        pc = entry
        op = self.op
        BRANCH, CBRANCH, BRANCHIND = op["BRANCH"], op["CBRANCH"], op["BRANCHIND"]
        CALL, CALLIND, RETURN = op["CALL"], op["CALLIND"], op["RETURN"]
        try:
            while True:
                self.steps += 1
                if self.steps > self.max_steps():
                    self.cut = self.cut or "steps"  # (compare: what came before the cut must agree)
                    self.finish("time-out")
                    return
                self.visited.add(pc)
                w = self.code_word(pc)
                if (w & 0x7FE0FC1F) == 0x6B00001F and self.track and len(self.compares) < MAX_COMPARES:
                    # cmp wN/xN, wM/xM (subs to the zero register, no shift): both values, as they are now
                    n, m = (w >> 5) & 31, (w >> 16) & 31
                    if n != 31 and m != 31:
                        size = 8 if w >> 31 else 4
                        self.compares.append((pc, self.reg(X0 + 8 * n, size), self.reg(X0 + 8 * m, size)))
                if w == TRAP_WORD or (w & 0xFFE0001F) == 0xD4200000:  # LLVM's trap word, brk (__builtin_trap)
                    self.finish("trap")  # (an end both runs must reach after the same events)
                    return
                kind, target = self.step(pc, w, BRANCH, CBRANCH, BRANCHIND, CALL, CALLIND, RETURN)
                if kind == "jump" and target == pc and w == 0x14000000:
                    # b . (a container's overflow check spins forever: 71001e7ec4): an end too, the
                    # lift's `L: goto L;` compiles to the same. (The target as resolved: a lift's
                    # tail call is the same word before its relocation, lift.branch.)
                    self.finish("hang")
                    return
                if (w & 0xFF000010) == 0x54000000 or (w & 0x7C000000) == 0x34000000:
                    # Which way each conditional branch went (b.cond, cbz/cbnz, tbz/tbnz): for a caller
                    # that wants both ways of every branch run before it trusts an agreement, as
                    # `visited` says which instructions ran.
                    self.outcomes.add((pc, kind == "jump"))
                if ((w & 0xFF000010) == 0x54000000 or (w & 0x7C000000) == 0x34000000) \
                        and (kind != "jump" or self.inside(target)):
                    # b.cond, cbz/cbnz/tbz/tbnz within the function: what was written for an unknown
                    # callee before a conditional branch may be the other path's (Machine.call's
                    # selection). An unconditional b carries what was written along its one path, and
                    # a tail call's own branch leaves its arguments as they are.
                    for k, st in self.state.items():
                        if st == "written" and k:
                            self.state[k] = "stale"
                if kind == "next":
                    # add/sub (immediate, 64-bit) from sp or x29 into another register: a local's address
                    if (w >> 24) in (0x91, 0xD1) and ((w >> 5) & 31) in (29, 31) and (w & 31) != 31:
                        self.taken.add(self.x(w & 31))
                    pc += 4
                elif kind == "jump":
                    if self.inside(target):
                        pc = target
                    else:  # a tail call
                        self.call(target)
                        self.finish("return")
                        return
                elif kind == "call":
                    # A call always leaves the function (a recursive call included), except a
                    # lift's call into a helper of its own object (not a lifted function).
                    if self.lift is not None and self.inside(target) and target not in self.lift_starts:
                        pc = target
                    else:
                        self.call(target)
                        ret_pc, pc = pc + 4, self.x(30)
                        # A call that ends the original's code never returns (a [[noreturn]] callee,
                        # abort or a fatal-error path): the run ends there. The lift ends after the
                        # same call (its own code after it, if any, is unreachable).
                        if (self.lift is None and pc == ret_pc and not self.inside(pc)) \
                                or (self.lift is not None and self.noret_at == self.calls):
                            self.noret_call = self.calls
                            self.finish("noreturn")
                            return
                        if not self.inside(pc):
                            raise Inconclusive("call returns outside")
                elif kind == "ret":
                    if target == SENTINEL:
                        self.finish("return")
                        return
                    if not self.inside(target):
                        raise Inconclusive("return outside")
                    pc = target
        except Fault as e:
            self.fault = e.args[0]
            self.fault_pc = pc
            self.fault_access = tuple(e.args[1:]) or None
            # (the end event holds the bytes written since the last call, as any end's does: two runs
            # crashing after the same calls are compared on them too, compare. Not a cut: the run
            # ended on its own.)
            cut, steps = self.cut, self.steps
            self.checkpoint(("end", "fault"))
            self.cut, self.steps = cut, steps

    def finish(self, kind: str) -> None:
        self.checkpoint(("end", kind))

    def step(self, pc: int, w: int, BRANCH, CBRANCH, BRANCHIND, CALL, CALLIND, RETURN):
        if (w & 0x7C000000) == 0x14000000:  # b, bl
            t = pc + sext(w & 0x3FFFFFF, 26) * 4
            if self.lift is not None and pc in self.lift.branch:
                t = self.lift.branch[pc]
            if w >> 31:
                self.set_reg(X0 + 8 * 30, 8, pc + 4)
                return "call", t
            return "jump", t
        if (w & 0x1F000000) == 0x10000000:  # adr, adrp
            imm = sext(((w >> 5) & 0x7FFFF) << 2 | ((w >> 29) & 3), 21)
            v = (pc & ~0xFFF) + (imm << 12) if w >> 31 else pc + imm
            self.set_reg(X0 + 8 * (w & 31), 8, v & 0xFFFFFFFFFFFFFFFF) if (w & 31) != 31 else None
            return "next", 0
        if (w & 0xFF9FFC1F) == 0xD61F0000:  # br, blr, ret
            t = self.x((w >> 5) & 31)
            if self.junk_vn and self.junk_reg(X0 + 8 * ((w >> 5) & 31), 8):
                self.junk_use("a branch to an undefined address")
            self.target_reg = (w >> 5) & 31  # (not an argument of the call)
            opc = (w >> 21) & 3
            if opc == 1:
                self.set_reg(X0 + 8 * 30, 8, pc + 4)
                return "call", t
            return ("ret" if opc == 2 else "jump"), t
        if (w & 0x3B000000) == 0x18000000:
            raise Inconclusive("literal load")
        if (w & 0xFFA0CC00) == 0x1E204800 and (w >> 12) & 0xF in (4, 5, 6, 7):
            # scalar fmax, fmin, fmaxnm, fminnm: Ghidra's P-code writes Rd = Rn before comparing Rd
            # with Rm, so `fmax s2, s0, s2` compares s0 with itself (fn_710122b164)
            size = 8 if (w >> 22) & 1 else 4
            op = (w >> 12) & 3  # 0 fmax, 1 fmin, 2 fmaxnm, 3 fminnm
            a = f_from(self.reg(Q0 + QSTRIDE * ((w >> 5) & 31), size), size)
            b = f_from(self.reg(Q0 + QSTRIDE * ((w >> 16) & 31), size), size)
            if op >= 2 and math.isnan(a) != math.isnan(b):
                v = b if math.isnan(a) else a
            elif math.isnan(a) or math.isnan(b):
                v = float("nan")
            elif a == b == 0:
                v = (a if math.copysign(1, a) > 0 else b) if op in (0, 2) else (a if math.copysign(1, a) < 0 else b)
            else:
                v = max(a, b) if op in (0, 2) else min(a, b)
            jk = self.junk_vn and (self.junk_reg(Q0 + QSTRIDE * ((w >> 5) & 31), size)
                                   or self.junk_reg(Q0 + QSTRIDE * ((w >> 16) & 31), size))
            d = Q0 + QSTRIDE * (w & 31)
            self.set_reg(d, 16, 0)
            self.set_reg(d, size, f_to(v, size))
            if jk:
                self.junk_vn.update((2, d + i) for i in range(size))
            return "next", 0
        if (w & 0xFF3C7C00) == 0x1E244000 and (w >> 22) & 3 in (0, 1) and (w >> 15) & 7 != 5:
            # scalar frint{n,p,m,z,a,x,i}: Ghidra's P-code is a float-to-integer TRUNC, so frintm of
            # 649.0 gave the integer bits 0x289 (7100e79fa4's lift; floorf compiles to it)
            size = 8 if (w >> 22) & 1 else 4
            a = f_from(self.reg(Q0 + QSTRIDE * ((w >> 5) & 31), size), size)
            mode = (w >> 15) & 7  # 0 nearest-even, 1 +inf, 2 -inf, 3 zero, 4 away, 6 and 7 current (nearest)
            if math.isnan(a) or math.isinf(a) or a == 0:
                v = a
            else:
                v = float({0: round(a), 1: math.ceil(a), 2: math.floor(a), 3: int(a), 6: round(a), 7: round(a),
                           4: math.floor(abs(a) + 0.5) * (1 if a >= 0 else -1)}[mode])
                v = math.copysign(v, a) if v == 0 else v  # (ceil(-0.5) is -0.0)
            jk = self.junk_vn and self.junk_reg(Q0 + QSTRIDE * ((w >> 5) & 31), size)
            d = Q0 + QSTRIDE * (w & 31)
            self.set_reg(d, 16, 0)
            self.set_reg(d, size, f_to(v, size))
            if jk:
                self.junk_vn.update((2, d + i) for i in range(size))
            return "next", 0
        if (w & 0x7F20FC00) == 0x1E200000 and (w >> 22) & 3 in (0, 1) and (w >> 16) & 7 in (0, 1, 4, 5):
            # scalar fcvt{n,p,m,z,a}{s,u} to a general register: Ghidra's TRUNC saturates signed,
            # so fcvtzu of a large float came out 0x7fffffff where the instruction gives all ones
            # (71001c9550; the lift's saturating convert is right)
            size = 8 if (w >> 22) & 1 else 4
            bits = 64 if w >> 31 else 32
            a = f_from(self.reg(Q0 + QSTRIDE * ((w >> 5) & 31), size), size)
            unsigned = (w >> 16) & 1
            mode = 4 if (w >> 16) & 4 else (w >> 19) & 3  # 0 nearest-even, 1 +inf, 2 -inf, 3 zero, 4 away
            lo, hi = (0, (1 << bits) - 1) if unsigned else (-(1 << (bits - 1)), (1 << (bits - 1)) - 1)
            if math.isnan(a):
                v = 0
            elif math.isinf(a):
                v = hi if a > 0 else lo
            else:
                v = (round(a), math.ceil(a), math.floor(a), int(a),
                     int(math.floor(abs(a) + 0.5)) * (1 if a >= 0 else -1))[mode]
                v = max(lo, min(hi, v))
            jk = self.junk_vn and self.junk_reg(Q0 + QSTRIDE * ((w >> 5) & 31), size)
            if (w & 31) != 31:
                self.set_reg(X0 + 8 * (w & 31), 8, v & ((1 << bits) - 1))
                if jk:
                    self.junk_vn.update((2, X0 + 8 * (w & 31) + i) for i in range(8))
            return "next", 0
        ops = self.ops_db.get(w, False)
        if ops is False:
            raise Inconclusive(f"no P-code for {w:08x}")
        if ops is None:
            raise Inconclusive(f"undecodable {w:08x}")
        vc = vector_copy(w) if self.vtag else None
        if vc is not None:
            tag = self.vtag.get(vc[1])
            self._vcopy = True  # (its reads are no use of the argument: read)
        self.uniq.clear()
        if self.undef_vn:
            self.undef_vn = {k for k in self.undef_vn if k[0] != 1}
        if self.count_track and self.ct_vn:
            self.ct_vn = {k: t for k, t in self.ct_vn.items() if k[0] != 1}
        if self.junk_vn:
            self.junk_vn = {k for k in self.junk_vn if k[0] != 1}
        i, n = 0, len(ops)
        while i < n:
            code, out, ins = ops[i]
            if self.junk_vn and code in (CBRANCH, BRANCHIND, CALLIND, RETURN) \
                    and self.junk_bytes(ins[1] if code == CBRANCH else ins[0]):
                self.junk_use("a branch on an undefined value")
            if code == BRANCH or code == CBRANCH:
                if code == CBRANCH and not self.read(ins[1]):
                    i += 1
                    continue
                t = ins[0]
                if t[0] == 0:  # relative to this P-code op
                    i += sext(t[1], 64) if t[1] >> 63 else t[1]
                    continue
                return "jump", pc + t[1]  # extracted at address 0: relative to this instruction
            if code == BRANCHIND:
                return "jump", self.read(ins[0])
            if code == CALL:
                return "call", pc + ins[0][1]
            if code == CALLIND:
                return "call", self.read(ins[0])
            if code == RETURN:
                return "ret", self.read(ins[0])
            self.exec(code, out, ins)
            i += 1
        if vc is not None:
            self._vcopy = False
            if tag is not None:
                self.vtag[vc[0]] = tag  # (its writes ended the destination's own: set_reg)
        return "next", 0

    # P-code ops whose output is a flag: a count compared, never a count itself
    COUNT_COMPARES = frozenset({"INT_EQUAL", "INT_NOTEQUAL", "INT_LESS", "INT_LESSEQUAL", "INT_SLESS",
                                "INT_SLESSEQUAL", "INT_CARRY", "INT_SCARRY", "INT_SBORROW"})
    # P-code ops whose output carries their inputs' out-parameter words (others' carry none)
    COUNT_CARRIES = frozenset({"COPY", "INT_ZEXT", "INT_SEXT", "SUBPIECE", "PIECE", "INT_ADD", "INT_SUB",
                               "INT_AND", "INT_OR", "INT_XOR", "INT_2COMP", "INT_NEGATE", "INT_RIGHT",
                               "INT_SRIGHT", "INT_DIV", "INT_SDIV", "INT_REM", "INT_SREM", "INT_MULT", "INT_LEFT"})

    def count_taint(self, vn) -> dict:
        """The out-parameter words vn's bytes came from (key -> the highest level)."""
        if vn[0] not in (1, 2):
            return {}
        t: dict = {}
        for i in range(vn[2]):
            for key, lv in self.ct_vn.get((vn[0], vn[1] + i), {}).items():
                if lv >= t.get(key, -1):
                    t[key] = lv
        return t

    def track_counts(self) -> None:
        """--small-out-counts' tracking for this run (the original's): count_exec runs each op."""
        self.count_track = True
        self.exec = self.count_exec

    def count_exec(self, code: int, out, ins) -> None:
        """exec with --small-out-counts' tracking (the original's run): the op as exec runs it, and
        which made-up out-parameter words its inputs carry. A word compared with a non-constant (a
        loop's `i < n`; a constant is a null test or a clamp) or multiplied or shifted by a constant
        into an address (an index, scaled) is a count; one an address holds as it is, a pointer
        dereferenced, is not (count_keys)."""
        if not (self.ct_vn or self.ct_mem):
            Machine.exec(self, code, out, ins)
            return
        name = OPNAMES[code]
        ins_t = [self.count_taint(v) for v in ins]
        if name in ("LOAD", "STORE"):
            a = self.read_quiet(ins[1])
            size = out[2] if name == "LOAD" else ins[2][2]
            for key, lv in ins_t[1].items():
                (self.count_uses if lv else self.count_deref).add(key)
        if name in self.COUNT_COMPARES and len(ins) == 2:
            for x, y in ((0, 1), (1, 0)):
                if ins_t[x] and ins[y][0] != 0:
                    self.count_uses.update(ins_t[x])
        Machine.exec(self, code, out, ins)
        t: dict = {}
        if name == "LOAD":
            for x in range(a, a + size):
                for key, lv in self.ct_mem.get(x, {}).items():
                    t[key] = max(lv, t.get(key, 0))
        elif name == "STORE":
            t = ins_t[2]
            if t:
                for x in range(a, a + size):
                    self.ct_mem[x] = t
            return
        elif name in self.COUNT_CARRIES:
            for i, ti in enumerate(ins_t):
                scaled = name == "INT_MULT" or (name == "INT_LEFT" and i == 0 and ins[1][0] == 0 and ins[1][1])
                for key, lv in ti.items():
                    lv = 1 if scaled else lv
                    t[key] = max(lv, t.get(key, 0))
        if out and out[0] in (1, 2):
            for i in range(out[2]):
                if t:
                    self.ct_vn[(out[0], out[1] + i)] = t
                else:
                    self.ct_vn.pop((out[0], out[1] + i), None)

    def read_quiet(self, vn) -> int:
        """vn's value as read() gives it, recording nothing (count_exec's look at an address)."""
        sp, off, size = vn
        if sp == 0:
            return off
        if sp == 2:
            return int.from_bytes(self.regs[off:off + size], "little")
        if sp == 1:
            return self.uniq.get(off, 0) & ((1 << (8 * size)) - 1)
        return 0  # (an address held in memory: none in AArch64's P-code)

    def count_keys(self) -> set:
        """The out-parameter words this run used as a count and never as a pointer (count_exec)."""
        return self.count_uses - self.count_deref

    def exec(self, code: int, out, ins) -> None:
        name = OPNAMES[code]
        r = self.read
        size = out[2] if out else 0
        jp = self.junk_pre(name, out, ins) if self.junk_vn or self.junk_mem else None
        if name == "COPY":
            self.write(out, r(ins[0]))
            self.carry_undef(out, ins[0], 0)
            if out[0] == 1 and out[2] == 8:
                e = self.saved_reg(ins[0])
                if e is not None:
                    self.save_src[out[1]] = e
        elif name == "LOAD":
            a = r(ins[1])
            self.write(out, self.load(a, size))
            if out[0] in (1, 2):
                self.undef_vn.update((out[0], out[1] + i) for i in range(size)
                                     if STACK_LO <= a + i < ENTRY_SP and a + i not in self.stack_written)
                # (and from a save slot still holding a callee-saved register's entry value: the
                # caller's, which no C code reads as data. Only an index past a local reaches it, a
                # made-up count run past the array (7100128810: an entry past a callee-filled table
                # copied out), and the lift's frame holds its own locals there. The epilogue's
                # restore loads it too, into the register: undefined there only until written. The
                # original's run only: compare() skips a byte either run's undef_mem holds, so the
                # lift reading its own saves, a wrong bound or offset, would decide what the original
                # is compared on; the original's undefined store already skips the same address.)
                if self.save_slots and self.lift is None and STACK_LO <= a < ENTRY_SP:
                    hit = [s for s, e in self.save_slots.items() if s < a + size and a < s + 8
                           and int.from_bytes(bytes(self.page((s + i) // PAGE)[(s + i) % PAGE] for i in range(8)),
                                              "little") == e]
                    if hit:
                        self.undef_vn.update((out[0], out[1] + i) for i in range(size)
                                             if any(s <= a + i < s + 8 for s in hit))
        elif name == "STORE":
            a = r(ins[1])
            self.store(a, ins[2][2], r(ins[2]))
            if STACK_LO <= a < ENTRY_SP:
                for s in [s for s in self.save_slots if s < a + ins[2][2] and a < s + 8]:
                    del self.save_slots[s]
                e = self.saved_reg(ins[2]) if ins[2][2] == 8 else None
                if e is not None:
                    self.save_slots[a] = e
                self.own_stack.update(range(a, a + ins[2][2]))
                for x in range(a, a + ins[2][2]):
                    self.out_at.pop(x, None)  # (the function's own now, not an out-parameter's)
            if self.undef_vn and ins[2][0] in (1, 2):
                for i in range(ins[2][2]):
                    if (ins[2][0], ins[2][1] + i) in self.undef_vn:
                        self.undef_mem.add(a + i)
                        self.dirty.pop(a + i, None)
                        self._touched.add(a + i)
                        self.stack_written.discard(a + i)
        elif name in INT_BINARY:
            a, b = r(ins[0]), r(ins[1])
            self.write(out, INT_BINARY[name](a, b, ins[0][2] * 8))
        elif name == "INT_ZEXT":
            self.write(out, r(ins[0]))
            self.carry_undef(out, ins[0], 0)
        elif name == "INT_SEXT":
            self.write(out, sext(r(ins[0]), ins[0][2] * 8))
        elif name == "INT_2COMP":
            self.write(out, -r(ins[0]))
        elif name == "INT_NEGATE":
            self.write(out, ~r(ins[0]))
        elif name == "BOOL_NEGATE":
            self.write(out, 0 if r(ins[0]) else 1)
        elif name == "PIECE":
            self.write(out, (r(ins[0]) << (8 * ins[1][2])) | r(ins[1]))
        elif name == "SUBPIECE":
            self.write(out, r(ins[0]) >> (8 * ins[1][1]))
            self.carry_undef(out, ins[0], ins[1][1])
        elif name == "POPCOUNT":
            self.write(out, bin(r(ins[0])).count("1"))
        elif name == "LZCOUNT":
            bits = ins[0][2] * 8
            v = r(ins[0])
            self.write(out, bits - v.bit_length())
        elif name.startswith("FLOAT") or name in ("INT2FLOAT", "FLOAT2FLOAT", "TRUNC", "CEIL", "FLOOR", "ROUND"):
            self.float_op(name, out, ins)
        elif name == "CALLOTHER":
            self.userop(self.userops.get(ins[0][1], "?"), out, ins[1:])
        else:
            raise Inconclusive(f"P-code {name}")
        if jp:
            self.junk_post(jp)

    def saved_reg(self, vn) -> int | None:
        """The entry value vn holds when it is a callee-saved register (x19-x28, d8-d15) still holding
        it, or a unique copied from one (save_src); else None."""
        sp, off, size = vn
        if size != 8:
            return None
        if sp == 2:
            e = self.entry_regs.get(off)
            return e if e is not None and self.reg(off, 8) == e else None
        if sp == 1:
            e = self.save_src.get(off)
            return e if e is not None and self.uniq.get(off) == e else None
        return None

    def carry_undef(self, out, src, skip: int) -> None:
        """A copy (or a zero extension, or a part from byte `skip` on) carries undefined bytes."""
        if self.undef_vn and src[0] in (1, 2) and out[0] in (1, 2):
            self.undef_vn.update((out[0], out[1] + i) for i in range(min(out[2], src[2] - skip))
                                 if (src[0], src[1] + skip + i) in self.undef_vn)

    # -- junk tracking (Machine.junk_scratch) ------------------------------------------------------

    def junk_use(self, why: str) -> None:
        """Something compared depends on an undefined value: the run decides nothing from the event
        it would reach on (the next one, len(events); compare)."""
        if self.junk_at is None:
            self.junk_at, self.junk_why = len(self.events), why

    def junk_clobber(self, keep=JUNK_CLEARED, junk=JUNK_BYTES) -> None:
        """After a call leaving the function: `keep` holds what the emulated call put there (its
        taint dropped), `junk` is undefined from now on."""
        if self.junk_scratch:
            self.junk_vn.difference_update(keep)
            self.junk_vn.update(junk)

    def junk_reg(self, off: int, n: int) -> bool:
        """Whether any of the n register bytes at off is undefined."""
        jv = self.junk_vn
        return bool(jv) and any((2, off + i) in jv for i in range(n))

    def junk_bytes(self, vn) -> list | None:
        """Which of vn's bytes are undefined (None: none)."""
        sp, off, size = vn
        if sp in (1, 2):
            if not self.junk_vn:
                return None
            r = [(sp, off + i) in self.junk_vn for i in range(size)]
        elif sp == 3:
            if not self.junk_mem:
                return None
            r = [off + i in self.junk_mem for i in range(size)]
        else:
            return None
        return r if any(r) else None

    def vn_value(self, vn) -> int | None:
        """vn's value as read() gives it, with none of read()'s side effects (None: memory)."""
        sp, off, size = vn
        if sp == 0:
            return off
        if sp == 1:
            return self.uniq.get(off, 0) & ((1 << (8 * size)) - 1)
        if sp == 2:
            return int.from_bytes(self.regs[off:off + size], "little")
        return None

    def junk_pre(self, name: str, out, ins):
        """Before exec runs a P-code op: the output bytes the op makes undefined, as (out, flags), or
        a STORE's (address, flags); records a use (junk_use) of an undefined address. Byte by byte
        through copies and parts (COPY, INT_ZEXT, SUBPIECE, PIECE, LOAD), else the whole output when
        any input byte is undefined (bitwise ops byte by byte too)."""
        if name == "STORE":
            a, vj = self.vn_value(ins[1]), self.junk_bytes(ins[2])
            if self.junk_bytes(ins[1]):
                self.junk_use("a store to an undefined address")
                vj = [True] * ins[2][2]
            return ("store", a, vj) if vj and a is not None else None
        if out is None:
            return None
        n = out[2]
        if name == "LOAD":
            if self.junk_bytes(ins[1]):
                self.junk_use("a load from an undefined address")  # (where it faults, or what it reads)
                return out, [True] * n
            a = self.vn_value(ins[1])
            if a is None or not self.junk_mem:
                return None
            j = [(a + i) & 0xFFFFFFFFFFFFFFFF in self.junk_mem for i in range(n)]
        elif name in ("COPY", "INT_ZEXT"):
            src = self.junk_bytes(ins[0])
            if not src:
                return None
            j = [i < len(src) and src[i] for i in range(n)]
        elif name == "SUBPIECE":
            src = self.junk_bytes(ins[0])
            if not src:
                return None
            k = ins[1][1]
            j = [k + i < len(src) and src[k + i] for i in range(n)]
        elif name == "PIECE":
            hi, lo = self.junk_bytes(ins[0]) or [False] * ins[0][2], self.junk_bytes(ins[1]) or [False] * ins[1][2]
            j = (lo + hi + [False] * n)[:n]
        elif name in ("INT_AND", "INT_OR", "INT_XOR", "INT_NEGATE") and all(v[2] == n for v in ins):
            # (bitwise: byte i of the result is of byte i of each input alone; `mov v0.16b, v8.16b`
            # is an orr whose upper half alone is v8's undefined one)
            srcs = [self.junk_bytes(v) for v in ins]
            j = [any(src and src[i] for src in srcs) for i in range(n)]
        else:
            j = [True] * n if any(self.junk_bytes(v) for v in ins) else None
        return (out, j) if j and any(j) else None

    def junk_post(self, plan) -> None:
        """After exec: mark what junk_pre found (a write has cleared the bytes it wrote)."""
        if plan[0] == "store":
            _, a, j = plan
            out = (3, a, len(j))
        else:
            out, j = plan
        sp, off, _ = out
        for i, b in enumerate(j):
            if not b:
                continue
            if sp == 3:
                x = (off + i) & 0xFFFFFFFFFFFFFFFF
                self.junk_mem.add(x)
                if not STACK_LO <= x < ENTRY_SP:
                    self.junk_use("a store of an undefined value")
            else:
                self.junk_vn.add((sp, off + i))

    def junk_args(self, sel, raw: dict, args) -> None:
        """A call's compared arguments (call's selection): one holding an undefined byte decides
        nothing, nor does a stack object one points at holding one."""
        val = dict(args)
        sp = self.reg(SP, 8)
        for k, bits in sel:
            n = bits // 8
            if k < 100:
                hit = self.junk_reg(X0 + 8 * k, n)
            elif k < 200:
                hit = self.junk_reg(Q0 + QSTRIDE * (k - 100), n)
            else:
                hit = any(sp + 8 * (k - 200) + i in self.junk_mem for i in range(n))
            v = val.get(k)
            if not hit and self.junk_mem and isinstance(v, tuple) and v[0] == "stack":
                hit = any(raw[k] + j in self.junk_mem for j in range(len(v[1])))
            if hit:
                self.junk_use(f"call {self.calls + 1}'s argument {regname(k)}")
                return

    def float_op(self, name: str, out, ins) -> None:
        r = self.read
        s0 = ins[0][2]
        if name == "INT2FLOAT":
            self.write(out, f_to(float(sext(r(ins[0]), 8 * s0)), out[2]))
            return
        a = f_from(r(ins[0]), s0)
        if name == "FLOAT_NAN":
            self.write(out, 1 if math.isnan(a) else 0)
            return
        if name in ("FLOAT_NEG", "FLOAT_ABS"):
            bits = r(ins[0])
            sign = 1 << (8 * s0 - 1)
            self.write(out, bits ^ sign if name == "FLOAT_NEG" else bits & ~sign)
            return
        if name == "FLOAT2FLOAT":
            self.write(out, f_to(a, out[2]))
            return
        if name == "TRUNC":
            bits = 8 * out[2]
            if math.isnan(a):
                v = 0
            elif math.isinf(a):
                v = (1 << (bits - 1)) - 1 if a > 0 else -(1 << (bits - 1))
            else:
                v = max(-(1 << (bits - 1)), min((1 << (bits - 1)) - 1, int(a)))
            self.write(out, v)
            return
        if name in ("CEIL", "FLOOR", "ROUND", "FLOAT_SQRT"):
            if math.isnan(a) or math.isinf(a):
                v = a if name != "FLOAT_SQRT" or a > 0 or math.isnan(a) else float("nan")
            elif name == "CEIL":
                v = float(math.ceil(a))
            elif name == "FLOOR":
                v = float(math.floor(a))
            elif name == "ROUND":
                v = float(math.floor(abs(a) + 0.5)) * (1 if a >= 0 else -1)
            else:
                v = math.sqrt(a) if a >= 0 else float("nan")
            if v == 0 and name != "FLOAT_SQRT":
                v = math.copysign(0.0, a)  # (frintm/frintp/frinta keep the sign of zero, as Machine.math's floorf)
            self.write(out, f_to(v, out[2]))
            return
        b = f_from(r(ins[1]), ins[1][2])
        if name == "FLOAT_EQUAL":
            self.write(out, 1 if a == b else 0)
        elif name == "FLOAT_NOTEQUAL":
            self.write(out, 1 if a != b else 0)
        elif name == "FLOAT_LESS":
            self.write(out, 1 if a < b else 0)
        elif name == "FLOAT_LESSEQUAL":
            self.write(out, 1 if a <= b else 0)
        else:
            try:
                if name in ("FLOAT_ADD", "FLOAT_MULT") and math.isnan(a) and math.isnan(b):
                    # Two NaNs: the hardware returns the first operand's, but C leaves which one
                    # unspecified and Clang commutes a + b and a * b (`fadd s8, s1, s8` lifted
                    # as `fadd s8, s8, s1`, fn_7100212794): a NaN chosen by the bits alone, so
                    # the order of the operands, which the compiled C does not keep, never shows.
                    quiet = 1 << (22 if s0 == 4 else 51)
                    v = (a if r(ins[0]) | quiet <= r(ins[1]) | quiet else b) + 0.0
                elif name == "FLOAT_ADD":
                    v = a + b
                elif name == "FLOAT_SUB":
                    v = a - b
                elif name == "FLOAT_MULT":
                    v = a * b
                elif name == "FLOAT_DIV":
                    v = a / b if b != 0 else (float("nan") if a == 0 or math.isnan(a)
                                              else math.copysign(float("inf"), a) * math.copysign(1, b))
                else:
                    raise Inconclusive(f"P-code {name}")
            except OverflowError:
                v = float("inf")
            self.write(out, f_to(v, out[2]))

    def userop(self, name: str, out, ins) -> None:
        if name in NOOP_USEROPS:
            return
        if name == "ExclusiveMonitorPass":
            if out:
                self.write(out, 1)
            return
        if name == "ExclusiveMonitorsStatus":
            if out:
                self.write(out, 0)
            return
        if name in ("MP_INT_UMULT", "MP_INT_MULT"):
            a, b = self.read(ins[0]), self.read(ins[1])
            if name == "MP_INT_MULT":
                a, b = sext(a, 8 * ins[0][2]), sext(b, 8 * ins[1][2])
            self.write(out, a * b)
            return
        if name in ("NEON_fminnm", "NEON_fmaxnm", "NEON_fmin", "NEON_fmax") and out and out[2] in (4, 8) \
                and len(ins) == 2:
            a, b = f_from(self.read(ins[0]), out[2]), f_from(self.read(ins[1]), out[2])
            if name.endswith("nm"):
                if math.isnan(a):
                    v = b
                elif math.isnan(b):
                    v = a
                else:
                    v = min(a, b) if "min" in name else max(a, b)
            else:
                v = float("nan") if math.isnan(a) or math.isnan(b) else (min(a, b) if "min" in name else max(a, b))
            self.write(out, f_to(v, out[2]))
            return
        if name == "NEON_ucvtf" and out and out[2] in (4, 8) and len(ins) == 1 and ins[0][2] == out[2]:
            # scalar unsigned integer to float, in a float register (`ucvtf s0, s0`)
            self.write(out, f_to(float(self.read(ins[0])), out[2]))
            return
        raise Inconclusive(f"user op {name}")


def _div(a, b, bits):
    return a // b if b else 0


def _sdiv(a, b, bits):
    a, b = sext(a, bits), sext(b, bits)
    if b == 0:
        return 0
    q = abs(a) // abs(b)
    return q if (a < 0) == (b < 0) else -q


def _rem(a, b, bits):
    return a % b if b else 0


def _srem(a, b, bits):
    a, b = sext(a, bits), sext(b, bits)
    if b == 0:
        return 0
    r = abs(a) % abs(b)
    return r if a >= 0 else -r


INT_BINARY = {
    "INT_ADD": lambda a, b, n: a + b, "INT_SUB": lambda a, b, n: a - b, "INT_MULT": lambda a, b, n: a * b,
    "INT_AND": lambda a, b, n: a & b, "INT_OR": lambda a, b, n: a | b, "INT_XOR": lambda a, b, n: a ^ b,
    "INT_EQUAL": lambda a, b, n: int(a == b), "INT_NOTEQUAL": lambda a, b, n: int(a != b),
    "INT_LESS": lambda a, b, n: int(a < b), "INT_LESSEQUAL": lambda a, b, n: int(a <= b),
    "INT_SLESS": lambda a, b, n: int(sext(a, n) < sext(b, n)),
    "INT_SLESSEQUAL": lambda a, b, n: int(sext(a, n) <= sext(b, n)),
    "INT_LEFT": lambda a, b, n: a << b if b < n else 0,
    "INT_RIGHT": lambda a, b, n: a >> b if b < n else 0,
    "INT_SRIGHT": lambda a, b, n: sext(a, n) >> min(b, n - 1),
    "INT_CARRY": lambda a, b, n: int(a + b >= 1 << n),
    "INT_SCARRY": lambda a, b, n: int(not -(1 << (n - 1)) <= sext(a, n) + sext(b, n) < 1 << (n - 1)),
    "INT_SBORROW": lambda a, b, n: int(not -(1 << (n - 1)) <= sext(a, n) - sext(b, n) < 1 << (n - 1)),
    "INT_DIV": _div, "INT_SDIV": _sdiv, "INT_REM": _rem, "INT_SREM": _srem,
    "BOOL_AND": lambda a, b, n: int(bool(a) and bool(b)), "BOOL_OR": lambda a, b, n: int(bool(a) or bool(b)),
    "BOOL_XOR": lambda a, b, n: int(bool(a) != bool(b)),
}
OPNAMES: dict[int, str] = {}  # filled from the P-code pickle's opcode table (set_opcodes)


def set_opcodes(opcodes: dict[str, int]) -> None:
    OPNAMES.clear()
    OPNAMES.update({v: k for k, v in opcodes.items()})


# --- comparing runs --------------------------------------------------------------------------------

@dataclass
class Verdict:
    ok: bool
    reason: str = ""
    runs: int = 0          # conclusive runs that agreed
    inconclusive: int = 0
    covered: set = field(default_factory=set)


def compare(a: Machine, b: Machine, ret: str | None) -> str:
    """'' when two runs agree, 'skip: <why>' when the input decides nothing (the original faulted
    and the lift did not fault there; a run was cut off and nothing that was recorded differs),
    else the first difference. `a` is the original's run. `ret` is the lift's result kind: None
    (not compared), 'x8', 'x16', 'x32', 'x64', 'f32', 'f64'."""
    ea, eb = a.events, b.events
    # (the caller's frame past both functions' own stack arguments: an address into the argument
    # slots names one argument, compared exactly)
    frame = ENTRY_SP + 8 * max(getattr(a, "arg_slots", None) or 0, getattr(b, "arg_slots", None) or 0, 0)
    if a.undef_mem or b.undef_mem:
        # (bytes either run stored from undefined stack bytes: not compared, in neither run)
        skip = a.undef_mem | b.undef_mem
        ea = [(v, tuple(kv for kv in m if kv[0] not in skip)) for v, m in ea]
        eb = [(v, tuple(kv for kv in m if kv[0] not in skip)) for v, m in eb]
    end_a, end_b = (ea[-1][0] if ea else None), (eb[-1][0] if eb else None)
    # (Machine.junk_scratch: the first event the original reached on a value a call left undefined,
    # None without junk tracking or when it did not. From that event on the original's behaviour is
    # made up, the callee's scratch registers as the emulator chose them, which no lift can follow
    # through a defined interface: nothing is compared, the input is a skip. The lift's own use of
    # one is not: its run is compared as without tracking, so a lift relying on a stale register
    # where the original does not is still rejected, or agrees, exactly as before.)
    jl = getattr(a, "junk_at", None)

    def same_call(va, vb) -> bool:
        # (a call to the same target with the original's arguments, as the comparison below takes
        # them: registers the callee is not passed may hold anything)
        if not (va[0] == vb[0] == "call" and va[1] == vb[1]):
            return False
        da, db = dict(va[3]), dict(vb[3])
        for k, bits in va[2]:
            u, v = da.get(k), db.get(k)
            if bits == 32 and isinstance(u, int) and isinstance(v, int):
                u, v = u & 0xFFFFFFFF, v & 0xFFFFFFFF
            if not same_arg(u, v, frame):
                return False
        return True

    # Both crashed after the same calls and writes (71004720d8: a register its callee is not passed
    # differs): no crash only the original had. The crash itself must be the same too: the same
    # access (load or store, its size) at the same address, after the same writes since the last
    # call (the end event's). Otherwise either run may have crashed anywhere after the last call,
    # the other's stores and result never reached: the input decides nothing.
    both_fault = end_a == end_b == ("end", "fault") and len(ea) == len(eb) \
        and all(x == y or (x[1] == y[1] and same_call(x[0], y[0])) for x, y in zip(ea[:-1], eb[:-1]))
    if both_fault:
        if (getattr(a, "fault", None), getattr(a, "fault_access", None)) \
                != (getattr(b, "fault", None), getattr(b, "fault_access", None)):
            return "skip: both runs faulted, on different accesses"
        if ea[-1][1] != eb[-1][1]:
            return "skip: both runs faulted, after different writes since the last call"
    if end_a == ("end", "fault") and ea != eb and not both_fault:
        # The original crashed on this input, the lift did not crash there: C lets the compiler
        # drop or move a load whose value decides nothing, so the end proves nothing (the lift
        # may crash later on what followed). What both did before the original's crash must still
        # agree (calls and writes, not their memory at the crash, which the lift may have passed).
        def arg(e, k, bits):
            # (a 32-bit argument compares its low half, as below: fn_710064fb5c's s0, a whole v8
            # copy of the incoming float in the original, an f32 in the lift)
            v = dict(e[3]).get(k)
            return v & 0xFFFFFFFF if bits == 32 and isinstance(v, int) else v

        for i in range(len(ea) - 1):
            if jl is not None and i >= jl:
                return junk_skip(a, b, jl)
            if i >= len(eb) or ea[i] != eb[i]:
                same = i < len(eb) and (ea[i][0] == eb[i][0] or (
                    ea[i][0][0] == eb[i][0][0] == "call" and ea[i][0][1] == eb[i][0][1]
                    and all(same_arg(arg(ea[i][0], k, bits), arg(eb[i][0], k, bits), frame) for k, bits in ea[i][0][2])))
                if same and ea[i][1] == eb[i][1]:
                    continue
                if same and not (a.clipped or b.clipped) and putback_only(a, b, ea, eb, i):
                    # (as below: a put-back one run made, or may have, after a call decides nothing)
                    return "skip: a byte one run put back (or may have) after a call, the other wrote or left alone"
                if i == len(eb) - 1 and end_b == ("end", "time-out"):
                    return "skip: the lift was cut off before the original's fault"
                if (a.clipped or b.clipped) and i < len(eb) and ea[i][0] == eb[i][0]:
                    return "skip: a memset or memcpy was clipped at 64 KiB"
                return f"event {i} before the original's fault"
        return "skip: the original faulted where the lift did not"
    # A run cut off (Machine.cut: out of steps, events or snapshot room) recorded a true prefix of
    # what it does: what both recorded is compared, the cut run's own end event aside.
    ta, tb = end_a == ("end", "time-out"), end_b == ("end", "time-out")
    shared = min(len(ea) - ta, len(eb) - tb)
    for i in range(shared):
        if jl is not None and i >= jl:
            return junk_skip(a, b, jl)
        (va, ma), (vb, mb) = ea[i], eb[i]
        if va != vb:
            if va[0] == "call" and vb[0] == "call":
                if va[1] != vb[1]:
                    return f"call {i}: target {fmt(va[1])} vs {fmt(vb[1])}"
                da, db = dict(va[3]), dict(vb[3])
                for k, bits in va[2]:  # the original's arguments
                    u, v = da.get(k), db.get(k)
                    if bits == 32 and isinstance(u, int) and isinstance(v, int):
                        u, v = u & 0xFFFFFFFF, v & 0xFFFFFFFF
                    if not same_arg(u, v, frame):
                        return f"call {i} to {fmt(va[1])}: argument {regname(k)} ({arg_text(u)} vs {arg_text(v)})"
            else:
                return f"event {i}: {va[0]} {va[1] if len(va) > 1 else ''} vs {vb[0]} {vb[1] if len(vb) > 1 else ''}"
        if ma != mb:
            if a.clipped or b.clipped:
                return "skip: a memset or memcpy was clipped at 64 KiB"  # (each run's own base: fn_710122e6f0)
            if putback_only(a, b, ea, eb, i):
                return "skip: a byte one run put back (or may have) after a call, the other wrote or left alone"
            return f"memory at event {i} ({va[0]}): {mem_diff(ma, mb)}"
    if jl is not None:  # (what follows, the cut-offs, the lengths and the result, is past it)
        return junk_skip(a, b, jl)
    if ta and tb:
        # Both cut off: what came after the cut was never compared (a long loop of the same calls in
        # both, or not), so the input decides nothing.
        return "skip: both runs were cut off"
    if ta or tb:
        cut, done = (a, b) if ta else (b, a)
        # The run that ended reached its next call (or returned) at index `shared`; the cut run
        # wrote, before it was cut, only memory an equivalent program writes before that event too.
        # Addresses only: the cut run may hold an intermediate value. Not a fault (C lets a compiler
        # move a store past a load that faults), nor when a modelled memset or memcpy was clipped at
        # 64 KiB in either run.
        if shared < len(done.events) and done.events[shared][0] != ("end", "fault") \
                and not (cut.clipped or done.clipped):
            extra = written(cut.events, len(cut.events) - 1) - written(done.events, shared)
            if extra:
                return f"memory written only by the {'original' if ta else 'lift'}, cut off: byte {min(extra):#x}"
        # The lift spun on where the original came to an end of its own in few steps (a return, a
        # fault, a noreturn call or a trap): too slow, or a loop the original does not have. Not a
        # hang: an original that spins forever (`b .`) is matched by a lift that spins forever in
        # any loop of its own, cut off with nothing recorded past the original's events.
        if tb and b.cut == "steps" and end_a is not None and end_a[0] == "end" \
                and end_a[1] not in ("time-out", "hang") \
                and a.steps * SLOWER <= Machine.max_steps(b):
            return f"the lift ran out of steps where the original ended after {a.steps}"
        return f"skip: the {'original' if ta else 'lift'} was cut off ({cut.cut})"
    if len(ea) != len(eb):
        return f"{len(ea)} events vs {len(eb)}"
    if ea and ea[-1][0] == ("end", "return") and ret and "*" in ret:
        # A multi-register result (x0:x1, or the first n float registers)
        kind, n = ret.split("*")
        size = int(kind[1:]) // 8
        if any(junk_result(a, kind, i) for i in range(int(n))):
            return "skip: the result holds a value a call left undefined"
        for i in range(int(n)):
            if kind == "x64" and a.x(i) != b.x(i):
                return f"result x{i}"
            if kind[0] == "f" and a.reg(Q0 + QSTRIDE * i, size) != b.reg(Q0 + QSTRIDE * i, size):
                return f"result {'s' if kind == 'f32' else 'd'}{i}"
    elif ea and ea[-1][0] == ("end", "return") and ret and abi_garbage(a, 0 if ret[0] == "x" else 100):
        pass
    elif ea and ea[-1][0] == ("end", "return") and ret:
        if junk_result(a, ret, 0):
            return "skip: the result holds a value a call left undefined"
        if ret[0] == "x":
            bits = int(ret[1:])
            if (a.x(0) ^ b.x(0)) & ((1 << bits) - 1):
                return "result x0"
        else:
            n = 4 if ret == "f32" else 8
            if a.reg(Q0, n) != b.reg(Q0, n):
                return "result s0/d0"
    return ""


def junk_skip(a: Machine, b: Machine, jl: int) -> str:
    return f"skip: the original's {a.junk_why}, a value a call left undefined (event {jl})"


def junk_result(m: Machine, kind: str, i: int) -> bool:
    """Whether the original's result register i of this kind (x<bits> or f32/f64, compare's `ret`)
    holds a byte a call left undefined (Machine.junk_scratch)."""
    jv = getattr(m, "junk_vn", None)
    if not jv:
        return False
    off, n = (X0 + 8 * i, int(kind[1:]) // 8) if kind[0] == "x" else (Q0 + QSTRIDE * i, 4 if kind == "f32" else 8)
    return any((2, off + j) in jv for j in range(n))


def abi_garbage(m: Machine, reg: int) -> bool:
    """Whether the original's result register (x0: 0, s0/d0: 100) still holds what its last call left
    while the function read that call's result in the other class (w0 after a bool callee, s0 after
    a float one) and never this one: AAPCS64 returns a value in x0-x1 or in v0-v3, never both, so
    this register is the callee's scratch, garbage in the game too (ret_used over-approximates a
    void function's callers: 71006ea9a4 returns x0 after a float callee, 7100c95f74 s0 after a bool
    one). Likewise after a call that read x8, the indirect result location: its result is in memory
    and neither register holds one (7100c75684 returns x0 after such a call). Nothing a lift returns
    there differs from the original's behaviour."""
    if getattr(m, "res_read", None) is None or reg in m.res_written or reg in m.res_read:
        return False
    return getattr(m, "res_sret", False) or (100 - reg) in m.res_read


def same_arg(u, v, frame: int = ENTRY_SP) -> bool:
    """`frame`: where the caller's frame starts past the stack arguments (compare: ENTRY_SP + 8 *
    the larger arg_slots; ENTRY_SP when unknown)."""
    if u == v:
        return True
    if isinstance(u, int) and isinstance(v, int) and frame <= u < STACK_TOP and frame <= v < STACK_TOP:
        # (both above the entry sp and the stack arguments, in the caller's frame: an address
        # computed from made-up words and each run's own frame, which the two lay out differently,
        # 71002842e4)
        return True
    if isinstance(u, tuple) and isinstance(v, tuple):
        # Stack windows: the object starts at the pointer and ends where either function stops
        # writing (beyond, the frames are laid out differently: other locals, save slots). A
        # first word written by one only is a difference. Words that are stack addresses in both
        # (a pointer to another local) agree whatever their value.
        a, b = u[1], v[1]
        # (u is the original's: its object ends at the first address the original took that the
        # lift did not take at the same offset, or at either frame's saved registers)
        n = min([len(a), len(b), u[4], v[4]] + [o for o in u[3] if o not in v[3]]) if len(u) > 4 and len(v) > 4 \
            else min(len(a), len(b), u[2] if len(u) > 2 else WINDOW)
        end = next((j for j in range(n) if a[j] is None or b[j] is None), n)
        if end < min(8, n) and (a[end] is None) != (b[end] is None):
            return False
        j = 0
        while j < end:
            if j + 8 <= end:
                wa = int.from_bytes(bytes(a[j:j + 8]), "little")
                wb = int.from_bytes(bytes(b[j:j + 8]), "little")
                if STACK_LO <= wa < ENTRY_SP and STACK_LO <= wb < ENTRY_SP:
                    j += 8
                    continue
            if a[j] != b[j]:
                return False
            j += 1
        return True
    return False


def arg_text(v) -> str:
    if isinstance(v, tuple):
        return "stack[" + " ".join("--" if b is None else f"{b:02x}" for b in v[1][:24]) + "]"
    return "?" if v is None else f"{v:#x}"


def written(events, upto: int) -> set:
    """The bytes a run's writes hold at event `upto` (its events carry changes, checkpoint)."""
    held: set = set()
    for _, m in events[:upto + 1]:
        for k, v in m:
            (held.discard if v is None else held.add)(k)
    return held


def putback_only(a: Machine, b: Machine, ea, eb, i: int) -> bool:
    """Whether the runs' memory at event `i` differs only in bytes one run left unchanged by storing,
    after a call, the value it had loaded since (Machine.putback) and the other wrote with that same
    value: `f(); (void)p->n; p->n = 0` against `f(); p->n = 0` with p->n loaded as 0 (710002a074).
    store cannot tell such a put-back from a constant that equals what was loaded, so the input decides
    nothing (a skip, never an agreement). Not where the writer loaded the byte only before the call:
    `x = *p; f(); *p = x` stores a value stale if the callee changed *p, a real difference that no
    input shows, as the emulated callee leaves *p alone (Machine.stale)."""
    pa, pb = getattr(a, "putback_ev", {}).get(i, {}), getattr(b, "putback_ev", {}).get(i, {})
    ua, ub = getattr(a, "unsure_ev", {}).get(i, set()), getattr(b, "unsure_ev", {}).get(i, set())
    if not (pa or pb or ua or ub):
        return False
    sa, sb = {}, {}
    for s, ev in ((sa, ea), (sb, eb)):
        for _, m in ev[:i + 1]:
            for k, v in m:
                if v is None:
                    s.pop(k, None)
                else:
                    s[k] = v
    # (the writer's byte must not be one it loaded only before the call: `x = *p; f(); *p = x`)
    xa, xb = getattr(a, "stale_ev", {}).get(i, set()), getattr(b, "stale_ev", {}).get(i, set())
    diff = [k for k in set(sa) | set(sb) if sa.get(k) != sb.get(k)]
    # (or a byte one run wrote with a value store could not tell from no change, Machine.unsure, and
    # the other left alone)
    return bool(diff) and all((k not in sa and k not in xb and pa.get(k) == sb.get(k))
                              or (k not in sb and k not in xa and pb.get(k) == sa.get(k))
                              or (k not in sb and k in ua) or (k not in sa and k in ub) for k in diff)


def mem_diff(ma, mb) -> str:
    da, db = dict(ma), dict(mb)
    for k in sorted(set(da) | set(db)):
        if da.get(k) != db.get(k):
            return f"byte {k:#x}: {da.get(k)} vs {db.get(k)}"
    return "?"


def fmt(t) -> str:
    return t[1] if isinstance(t, tuple) else f"{t:#x}"


def regname(k: int) -> str:
    return f"x{k}" if k < 100 else f"v{k - 100}" if k < 200 else f"[sp+{8 * (k - 200):#x}]"
