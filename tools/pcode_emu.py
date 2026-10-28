#!/usr/bin/env python3
"""Differential execution of a function and its semantic lift, as Ghidra P-code.

The oracle of tools/port_lift.py (docs/PORT_PLAN.md, oracle (b)): the original instructions and the
lift compiled for AArch64 by the project compiler run side by side from the same inputs, every
instruction executed as the raw P-code Ghidra's AArch64 specification gives it
(tools/ghidra/pcode.py), so the lifter under test never decides what an instruction means. Only
`b`, `bl`, `adr` and `adrp` are executed here directly, and P-code is cached per instruction word
(branch targets relative to the word's address are relocated when it runs).

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

Standard library and pyelftools; the P-code comes from a pickle tools/ghidra/pcode.py writes.
"""

from __future__ import annotations

import math
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
WINDOW = 64                       # bytes compared and written around a stack pointer argument

X0, Q0, SP, NG, ZR, CY, OV = 0x4000, 0x5000, 8, 0x100, 0x101, 0x102, 0x103
QSTRIDE = 0x20  # Ghidra spaces q<n> 32 bytes apart (room for SVE z<n>): q1 is at 0x5020
REG_SIZE = 0x6000
CALLER_SAVED_X = range(19)
CALLER_SAVED_Q = list(range(8)) + list(range(16, 32))
MODELED = frozenset({"memcpy", "memmove", "memset", "memcmp", "strlen", "strcmp"})
NOOP_USEROPS = frozenset({"DataMemoryBarrier", "DataSynchronizationBarrier", "InstructionSynchronizationBarrier",
                          "Hint_Prefetch", "SpeculationBarrier", "Yield", "SendEvent", "SendEventLocally",
                          "WaitForEvent", "ClearExclusiveLocal", "LOAcquire", "LORelease"})


TRAP_WORD = 0xE7FFDEFE  # the trap word of this binary's unreachable code (unallocated in A64)


class Fault(Exception):
    pass


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


def load_object(path, resolve, demangled: dict[str, str]) -> LiftObject:
    """Lay out every allocatable section of an AArch64 ELF object (text first, then data), apply
    its relocations, and resolve undefined symbols with `resolve(demangled name) -> address`."""
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
            if s.name.startswith("_ZGV") and isinstance(sh, int) and sh in place:
                lo.guards.append(sym_addr[-1])
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

class Machine:
    def __init__(self, db: dict, image: ImageMemory, seed: int, inside, code_word, lift: LiftObject | None,
                 imports: dict[int, str], facts, preset: dict[int, int], dictionary: tuple = (),
                 overrides: dict | None = None, track: bool = False):
        self.dictionary = dictionary  # constants of the original (DICT_PROFILE seeds draw from them)
        # An input changed from what the seed gives (the oracle's mutations): ("r", key) -> register
        # value, ("m", address of an 8-byte word) -> memory word, ("c", n) -> the n-th call's x0
        # result; and, with track, the memory words the run read before writing them, in order
        # (what a mutation can change next).
        self.overrides = overrides or {}
        self.mem_ovr: dict[int, list[tuple[int, int]]] = {}
        for (kind, k), v in self.overrides.items():
            if kind == "m":
                self.mem_ovr.setdefault(k // PAGE, []).append((k % PAGE, v))
        self.track, self.read_words, self._read_seen = track, [], set()
        self.read_at: dict[int, int] = {}  # each read word's byte where it was first read (a mutation goes there)
        self.ops_db, self.userops, self.op = db["ops"], db["userops"], db["opcodes"]
        self.image, self.seed, self.inside, self.code_word, self.lift = image, seed, inside, code_word, lift
        self.imports, self.facts = imports, facts
        self.regs = bytearray(REG_SIZE)
        self.uniq: dict[int, int] = {}
        self.pages: dict[int, bytearray] = {}
        self.dirty: dict[int, int] = {}
        self.cut: str | None = None  # why the run ended as a time-out: steps, events or snapshot (compare)
        self.clipped = False  # a modelled memset or memcpy stopped at 64 KiB (compare)
        self.fault: int | None = None  # the address a run faulted on
        self.stack_written: set[int] = set()
        # Undefined bytes: loaded from stack the run never wrote (a struct copy's padding), and
        # where they were stored. Register and unique bytes holding one ((space, offset) keys);
        # memory bytes stored from one, which compare() leaves out (undefined in C: a lift that
        # copies the fields alone, or writes the padding, is the same program).
        self.undef_vn: set[tuple[int, int]] = set()
        self.undef_mem: set[int] = set()
        self.frame_record: set[int] = set()  # stack words holding the saved return address (store)
        self.saves: set[int] = set()  # stack words holding a callee-saved register's entry value (store)
        self.entry_saved: set[int] = set()  # those entry values (setup: x19-x28, d8-d15)
        self.taken: set[int] = set()  # stack addresses the code made (`add xN, sp, #k`): local objects' starts
        # The original's calls, for the lift's run: which arguments it wrote out-parameters
        # through and how far (call_log); the lift's Machine gets them as `guide` (port_lift).
        self.call_log: list[tuple[frozenset, dict]] = []
        self.guide: list | None = None
        self.arg_slots: int | None = None  # the function's stack argument slots (load: the caller's frame)
        self.state = {}  # argument register key -> "entry" | "written" | "junk"
        self.events: list = []
        self.snapshot_bytes = 0  # written bytes copied into events so far (checkpoint)
        self.calls = 0
        self.steps = 0
        self.scale = 1  # MAX_STEPS, MAX_EVENTS and MAX_SNAPSHOT times this (port_lift raises it to retry a cut-off run)
        self.visited: set[int] = set()
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
            raise Fault(a)
        if self.arg_slots is not None and ENTRY_SP + 8 * self.arg_slots <= a < STACK_TOP + (1 << 36):
            # Above the stack arguments: the caller's frame, which the two runs place differently
            # (an index from a callee's made-up result, a loop run past its array)
            raise Inconclusive("read in the caller's frame")
        if self.track:
            w = a & ~7
            if w not in self._read_seen and not STACK_LO <= w < ENTRY_SP and not self.own[0] <= w < self.own[1]:
                self._read_seen.add(w)
                if w not in self.dirty and w not in self.stack_written:
                    self.read_words.append(w)
                    self.read_at[w] = a - w
        o = a % PAGE
        if o + size <= PAGE:
            return int.from_bytes(self.page(a // PAGE)[o:o + size], "little")
        return int.from_bytes(bytes(self.page((a + i) // PAGE)[(a + i) % PAGE] for i in range(size)), "little")

    def store(self, a: int, size: int, v: int, track: bool = True) -> None:
        a &= 0xFFFFFFFFFFFFFFFF
        if a < NULL_PAGE:
            raise Fault(a)
        b = (v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        for i in range(size):
            x = a + i
            self.page(x // PAGE)[x % PAGE] = b[i]
        if not track:
            return
        if STACK_LO <= a < ENTRY_SP:
            self.stack_written.update(range(a, a + size))
            if size == 8 and v & 0xFFFFFFFFFFFFFFFF == SENTINEL:
                self.frame_record.add(a)  # the saved return address: out-parameters never reach it
            elif size == 8 and v & 0xFFFFFFFFFFFFFFFF in self.entry_saved:
                self.saves.add(a)  # a callee-saved register's entry value: a save slot, not a local
        elif not (self.own[0] <= a < self.own[1]):
            for i in range(size):
                self.dirty[a + i] = b[i]

    # registers
    def reg(self, off: int, size: int) -> int:
        return int.from_bytes(self.regs[off:off + size], "little")

    def set_reg(self, off: int, size: int, v: int) -> None:
        self.regs[off:off + size] = (v & ((1 << (8 * size)) - 1)).to_bytes(size, "little")
        if self.undef_vn:
            self.undef_vn.difference_update((2, off + i) for i in range(size))
        if X0 <= off < X0 + 72:
            self.state[(off - X0) // 8] = "written"
        elif Q0 <= off < Q0 + 8 * QSTRIDE:
            self.state[100 + (off - Q0) // QSTRIDE] = "written"

    def x(self, n: int) -> int:
        return self.reg(X0 + 8 * n, 8)

    def read(self, vn) -> int:
        sp, off, size = vn
        if sp == 0:
            return off
        if sp == 2:
            return int.from_bytes(self.regs[off:off + size], "little")
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
            if self.undef_vn:
                self.undef_vn.difference_update((1, off + i) for i in range(size))
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

    def canonical(self, target: int):
        name = self.imports(target)
        return ("import", name) if name else target

    def call(self, target: int) -> None:
        """A call leaving the function: modeled C library functions run; others are recorded."""
        t = self.canonical(target)
        if isinstance(t, tuple) and t[1] in MODELED:
            self.clib(t[1])
            return
        # Every argument register's value; `sel` the ones this run's caller meant for the callee.
        # compare() takes the original's selection, so registers the lift's compiled code used as
        # scratch do not count, and one the original set must hold the same value in the lift.
        f = self.facts(target)
        args, sel, raw = [], [], {}  # (raw: the integer arguments' values, for the out-parameters)
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
                if st != "written" or not STACK_LO <= self.x(8) < ENTRY_SP:
                    continue
            elif st in ("entry", "stale") and k != 0:  # unknown: x0 and what this block set for it
                continue
            elif k >= 100 and isinstance(t, tuple):  # an import of unknown signature: integers only
                continue
            # A register the callee reads only as w<n> has an unspecified upper half.
            sel.append((k, 32 if f is not None and k < 9 and k not in f.wide else 64))
        # Arguments beyond the eight registers: the callee's stack slots at [sp + 8i] (key 200 + i),
        # 32 bits each (a 32-bit argument leaves the upper half of its slot unspecified).
        sp = self.reg(SP, 8)
        for i in range(getattr(f, "stack_slots", 0) if f is not None else 0):
            raw[200 + i] = self.load(sp + 8 * i, 8)
            args.append((200 + i, self.stackref(raw[200 + i])))
            sel.append((200 + i, 32))
        self.checkpoint(("call", t, tuple(sel), tuple(args)))
        self.target_reg = None
        self.calls += 1
        n = self.calls
        chosen = {k for k, _ in sel}
        extent = {k: v[2] for k, v in args if isinstance(v, tuple) and v[0] == "stack"}
        self.call_log.append((frozenset(chosen), extent))
        if self.guide is not None and n <= len(self.guide):
            # (the lift's run: through the arguments the original's call chose, as far as its
            # objects reach, since this frame's layout and the scratch values it passes differ)
            chosen, extent = self.guide[n - 1][0], {**extent, **self.guide[n - 1][1]}
        for k, v in args:  # out-parameters: through the arguments only (scratch registers differ)
            if k in chosen and isinstance(v, tuple) and v[0] == "stack" and k in extent:
                p = raw[k]
                # (within the object, stackref's extent, and never over the frame record: a small
                # local below it would let the pattern overwrite the saved return address)
                for j in range(0, max(extent[k], 8), 8):
                    if p + j >= ENTRY_SP or any(p + j - 7 <= r <= p + j + 7 for r in self.frame_record):
                        break
                    self.store(p + j, 8, mix(n, k, j))
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
        ret = self.overrides.get(("c", n), self.word(mix(n, 11, self.seed)))  # (a mutation may set it)
        self.regs[X0:X0 + 8] = (ret & 0xFFFFFFFFFFFFFFFF).to_bytes(8, "little")
        self.regs[Q0:Q0 + 16] = f_to(n + 0.5, 4).to_bytes(16, "little")  # an f32 result (the game's floats)
        self.state[0] = "written"
        # A float argument counts only when the caller's own code set it: a result left in v0 and
        # read by the next callee is indistinguishable from garbage it does not read (liveness
        # over-approximates), so a float result passed straight on is not compared.
        self.state[100] = "junk"

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

    def clib(self, name: str) -> None:
        a0, a1, a2 = self.x(0), self.x(1), self.x(2)
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
        for i in CALLER_SAVED_X:
            if i:
                self.regs[X0 + 8 * i:X0 + 8 * i + 8] = bytes(8)
                if i <= 8:
                    self.state[i] = "junk"
        self.set_reg(X0, 8, r & 0xFFFFFFFFFFFFFFFF)

    def shrink(self) -> None:
        """Drop what only running needs (memory pages, the write sets): a finished run that is
        kept for comparison needs its events, registers, coverage and logs."""
        self.pages, self.dirty, self.stack_written, self.uniq, self._read_seen = {}, {}, set(), {}, set()

    def checkpoint(self, event) -> None:
        self.events.append((event, tuple(sorted(self.dirty.items()))))
        self.snapshot_bytes += len(self.dirty)
        if len(self.events) >= MAX_EVENTS * self.scale or self.snapshot_bytes > MAX_SNAPSHOT * self.scale:
            # Each event keeps a copy of the memory written so far: a loop calling on every
            # iteration would hold thousands. The run ends here as a time-out (compare: what came
            # before must agree).
            self.cut = self.cut or ("events" if len(self.events) >= MAX_EVENTS * self.scale else "snapshot")
            self.steps = MAX_STEPS * self.scale

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
                if self.steps > MAX_STEPS * self.scale:
                    self.cut = self.cut or "steps"  # (compare: what came before the cut must agree)
                    self.finish("time-out")
                    return
                self.visited.add(pc)
                w = self.code_word(pc)
                if w == TRAP_WORD or (w & 0xFFE0001F) == 0xD4200000:  # LLVM's trap word, brk (__builtin_trap)
                    self.finish("trap")  # (an end both runs must reach after the same events)
                    return
                kind, target = self.step(pc, w, BRANCH, CBRANCH, BRANCHIND, CALL, CALLIND, RETURN)
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
                        pc = self.x(30)
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
            self.events.append((("end", "fault"), ()))
            self.fault = e.args[0]

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
            self.target_reg = (w >> 5) & 31  # (not an argument of the call)
            opc = (w >> 21) & 3
            if opc == 1:
                self.set_reg(X0 + 8 * 30, 8, pc + 4)
                return "call", t
            return ("ret" if opc == 2 else "jump"), t
        if (w & 0x3B000000) == 0x18000000:
            raise Inconclusive("literal load")
        ops = self.ops_db.get(w, False)
        if ops is False:
            raise Inconclusive(f"no P-code for {w:08x}")
        if ops is None:
            raise Inconclusive(f"undecodable {w:08x}")
        self.uniq.clear()
        if self.undef_vn:
            self.undef_vn = {k for k in self.undef_vn if k[0] != 1}
        i, n = 0, len(ops)
        while i < n:
            code, out, ins = ops[i]
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
        return "next", 0

    def exec(self, code: int, out, ins) -> None:
        name = OPNAMES[code]
        r = self.read
        size = out[2] if out else 0
        if name == "COPY":
            self.write(out, r(ins[0]))
            self.carry_undef(out, ins[0], 0)
        elif name == "LOAD":
            a = r(ins[1])
            self.write(out, self.load(a, size))
            if out[0] in (1, 2):
                self.undef_vn.update((out[0], out[1] + i) for i in range(size)
                                     if STACK_LO <= a + i < ENTRY_SP and a + i not in self.stack_written)
        elif name == "STORE":
            a = r(ins[1])
            self.store(a, ins[2][2], r(ins[2]))
            if self.undef_vn and ins[2][0] in (1, 2):
                for i in range(ins[2][2]):
                    if (ins[2][0], ins[2][1] + i) in self.undef_vn:
                        self.undef_mem.add(a + i)
                        self.dirty.pop(a + i, None)
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

    def carry_undef(self, out, src, skip: int) -> None:
        """A copy (or a zero extension, or a part from byte `skip` on) carries undefined bytes."""
        if self.undef_vn and src[0] in (1, 2) and out[0] in (1, 2):
            self.undef_vn.update((out[0], out[1] + i) for i in range(min(out[2], src[2] - skip))
                                 if (src[0], src[1] + skip + i) in self.undef_vn)

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
                if name == "FLOAT_ADD":
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
    if a.undef_mem or b.undef_mem:
        # (bytes either run stored from undefined stack bytes: not compared, in neither run)
        skip = a.undef_mem | b.undef_mem
        ea = [(v, tuple(kv for kv in m if kv[0] not in skip)) for v, m in ea]
        eb = [(v, tuple(kv for kv in m if kv[0] not in skip)) for v, m in eb]
    end_a, end_b = (ea[-1][0] if ea else None), (eb[-1][0] if eb else None)
    if end_a == ("end", "fault") and ea != eb:
        # The original crashed on this input, the lift did not crash there: C lets the compiler
        # drop or move a load whose value decides nothing, so the end proves nothing (the lift
        # may crash later on what followed). What both did before the original's crash must still
        # agree (calls and writes, not their memory at the crash, which the lift may have passed).
        for i in range(len(ea) - 1):
            if i >= len(eb) or ea[i] != eb[i]:
                if i < len(eb) and ea[i][0][0] == eb[i][0][0] == "call" and ea[i][0][1] == eb[i][0][1] \
                        and ea[i][1] == eb[i][1] and all(same_arg(dict(ea[i][0][3]).get(k), dict(eb[i][0][3]).get(k))
                                                         for k, _ in ea[i][0][2]):
                    continue
                if i == len(eb) - 1 and end_b == ("end", "time-out"):
                    return "skip: the lift was cut off before the original's fault"
                return f"event {i} before the original's fault"
        return "skip: the original faulted where the lift did not"
    # A run cut off (Machine.cut: out of steps, events or snapshot room) recorded a true prefix of
    # what it does: what both recorded is compared, the cut run's own end event aside.
    ta, tb = end_a == ("end", "time-out"), end_b == ("end", "time-out")
    shared = min(len(ea) - ta, len(eb) - tb)
    for i in range(shared):
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
                    if not same_arg(u, v):
                        return f"call {i} to {fmt(va[1])}: argument {regname(k)} ({arg_text(u)} vs {arg_text(v)})"
            else:
                return f"event {i}: {va[0]} {va[1] if len(va) > 1 else ''} vs {vb[0]} {vb[1] if len(vb) > 1 else ''}"
        if ma != mb:
            return f"memory at event {i} ({va[0]}): {mem_diff(ma, mb)}"
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
            extra = {k for k, _ in cut.events[-1][1]} - {k for k, _ in done.events[shared][1]}
            if extra:
                return f"memory written only by the {'original' if ta else 'lift'}, cut off: byte {min(extra):#x}"
        if tb and b.cut == "steps" and end_a in (("end", "return"), ("end", "fault")) \
                and a.steps * SLOWER <= MAX_STEPS * b.scale:
            return f"the lift ran out of steps where the original ended after {a.steps}"
        return f"skip: the {'original' if ta else 'lift'} was cut off ({cut.cut})"
    if len(ea) != len(eb):
        return f"{len(ea)} events vs {len(eb)}"
    if ea and ea[-1][0] == ("end", "return") and ret and "*" in ret:
        # A multi-register result (x0:x1, or the first n float registers)
        kind, n = ret.split("*")
        size = int(kind[1:]) // 8
        for i in range(int(n)):
            if kind == "x64" and a.x(i) != b.x(i):
                return f"result x{i}"
            if kind[0] == "f" and a.reg(Q0 + QSTRIDE * i, size) != b.reg(Q0 + QSTRIDE * i, size):
                return f"result {'s' if kind == 'f32' else 'd'}{i}"
    elif ea and ea[-1][0] == ("end", "return") and ret:
        if ret[0] == "x":
            bits = int(ret[1:])
            if (a.x(0) ^ b.x(0)) & ((1 << bits) - 1):
                return "result x0"
        else:
            n = 4 if ret == "f32" else 8
            if a.reg(Q0, n) != b.reg(Q0, n):
                return "result s0/d0"
    return ""


def same_arg(u, v) -> bool:
    if u == v:
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
