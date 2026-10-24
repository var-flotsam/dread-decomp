#!/usr/bin/env python3
"""Compare functions in a compiled AArch64 object file against the original binary.

Functions are matched by name: any symbol containing fn_<10 hex digits> is
compared against that address in the original module (size from .eh_frame).
Instruction fields that a relocation fills in (ADRP pages, lo12 offsets, BL/B
targets, conditional branch offsets) are masked on both sides.

Relocation targets are checked separately (check_relocs), so a function that
matches byte-for-byte also has to name the original's callees, globals and GOT
slots. Each relocation's symbol is resolved to an original address (fn_<addr>,
g_unk_<addr>, or whatever the caller's resolver knows) and compared with the
target the original instruction encodes:
  CALL26/JUMP26          the BL/B offset; a PLT stub is followed to the import it names
  ADRP + ADD/LDST lo12   the absolute address (pairs on the same symbol+addend; an
                         unpaired half checks only its page or its low 12 bits)
  ADR_GOT_PAGE + LD64_GOT_LO12_NC   the slot's RELATIVE relocation (or the import's name)
Conditional branches, section-relative references (switch tables, string
literals) and symbols that do not resolve are counted as unchecked by kind,
never as a mismatch.

Usage:
  python tools/asmdiff.py --version 2.1.0 build/obj/foo.o [-v] [--no-reloc]
As a library: diff_object(obj_path, original, strict=False) -> list[FunctionResult]
              check_relocs(result, original, resolve) -> RelocReport
With strict=True (what the generators use) a function whose bytes match is also checked for
its relocation targets: its symbols are demangled with llvm-cxxfilt (one call per object,
object_resolver) and resolved through fn_/g_unk_<addr> names in any namespace or through the
functions the object itself defines; a wrong target lands in FunctionResult.reloc_mismatches
and makes `matching` False (bytes_match stays True). Without strict the caller runs
check_relocs itself, as check.py does with its data/functions/ names as a further source.
The command line resolves symbols as check.py does (demangled fn_/g_unk_ names, unique
data/functions/ names) and prints a RELOC line under each matching function with a wrong target;
without check.py's tools (the toolchain modules) it demangles on its own, and without
llvm-cxxfilt it says so on stderr and resolves plain fn_/g_unk_ symbol names only.

Disassembly printed with -v is for discussion only. Never write it into
tracked files.
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import subprocess
import sys
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
import hostenv  # noqa: E402
from image import BASE, R_AARCH64_RELATIVE, Image  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
NAME_RE = re.compile(r"fn_([0-9a-fA-F]{10})")
# A symbol that *is* an original function or global (optionally namespaced: callee::, folded::,
# noret::, base::), as opposed to one that merely mentions an address (Unk_fn_<addr>::...).
# unk_<addr> is the older spelling of g_unk_<addr>.
TARGET_RE = re.compile(r"^(?:\w+::)*(?:fn|g_unk|unk)_([0-9a-f]{10})$")
QUALIFIERS = re.compile(r"(\s*(const|volatile|&&|&))+$")

# relocation type -> bits of the instruction that are *kept* when comparing
MASKS = {
    275: 0x9F00001F,                 # ADR_PREL_PG_HI21 (ADRP)
    276: 0x9F00001F,                 # ADR_PREL_PG_HI21_NC
    277: ~(0xFFF << 10) & 0xFFFFFFFF,  # ADD_ABS_LO12_NC
    278: ~(0xFFF << 10) & 0xFFFFFFFF,  # LDST8_ABS_LO12_NC
    284: ~(0xFFF << 10) & 0xFFFFFFFF,  # LDST16
    285: ~(0xFFF << 10) & 0xFFFFFFFF,  # LDST32
    286: ~(0xFFF << 10) & 0xFFFFFFFF,  # LDST64
    299: ~(0xFFF << 10) & 0xFFFFFFFF,  # LDST128
    282: 0xFC000000,                 # JUMP26
    283: 0xFC000000,                 # CALL26
    280: ~(0x7FFFF << 5) & 0xFFFFFFFF,  # CONDBR19
    279: ~(0x3FFF << 5) & 0xFFFFFFFF,   # TSTBR14
    311: 0x9F00001F,                 # ADR_GOT_PAGE
    312: ~(0xFFF << 10) & 0xFFFFFFFF,  # LD64_GOT_LO12_NC
}

ADRP_TYPES = {275, 276}
LO12_SCALE = {277: 0, 278: 0, 284: 1, 285: 2, 286: 3, 299: 4}  # imm12 is in units of the access size
JUMP26, CALL26, TSTBR14, CONDBR19, GOT_PAGE, GOT_LO12 = 282, 283, 279, 280, 311, 312


@dataclass(frozen=True)
class ObjReloc:
    """One relocation inside a function of our object."""
    type: int
    symbol: str   # the referenced symbol; for a section-relative reference, the named symbol that
                  # covers the target in that section, else the section's name (section=True)
    addend: int   # relative to `symbol`
    section: bool = False


@dataclass
class ObjFunction:
    symbol: str
    code: bytes
    relocs: dict[int, ObjReloc]  # offset within the function -> relocation


@dataclass
class FunctionResult:
    symbol: str
    address: int
    orig_size: int
    our_size: int
    diff_insns: list[int] = field(default_factory=list)  # indices of differing instructions
    relocs: dict[int, ObjReloc] = field(default_factory=dict)
    # Relocations naming a target other than the original's; filled by diff_functions(strict=True)
    # on functions whose bytes match (check_relocs), empty otherwise.
    reloc_mismatches: list[RelocMismatch] = field(default_factory=list)

    @property
    def bytes_match(self) -> bool:
        """Byte-identical with the relocated fields masked, whatever the relocations name."""
        return self.orig_size == self.our_size and not self.diff_insns

    @property
    def matching(self) -> bool:
        """bytes_match, and (strict mode) every checked relocation names the original's target."""
        return self.bytes_match and not self.reloc_mismatches

    @property
    def score(self) -> float:
        n = max(self.orig_size, self.our_size) // 4
        same = min(self.orig_size, self.our_size) // 4 - len(self.diff_insns)
        return same / n if n else 0.0


@dataclass
class RelocMismatch:
    offset: int           # of the instruction within the function
    kind: str             # call26, jump26, adrp+lo12, adrp (page only), lo12 (low bits only), got, *-plt/-import
    symbol: str           # as named in our object
    addend: int
    expected: int | None  # what our symbol+addend resolves to (None when only a name is known)
    actual: int | None    # what the original encodes (None when it names an import instead)
    actual_name: str | None = None  # the import the original's PLT stub or GOT slot names


@dataclass
class RelocReport:
    checked: Counter = field(default_factory=Counter)    # kind -> count
    unchecked: Counter = field(default_factory=Counter)  # kind -> count
    mismatches: list[RelocMismatch] = field(default_factory=list)

    def merge(self, other: RelocReport) -> None:
        self.checked.update(other.checked)
        self.unchecked.update(other.unchecked)
        self.mismatches.extend(other.mismatches)


class Original:
    def __init__(self, version: str, module: str = "main"):
        self.img = Image(ROOT / "orig" / version / "exefs" / module)
        fdes = json.loads((ROOT / "build" / "recon" / version / f"{module}_eh_functions.json").read_text())
        self.sizes = {int(a, 16): n for a, n in fdes}
        self.dynsym = {s["name"]: s for s in self.img.symbols if s["name"]}

    def function(self, address: int) -> bytes:
        size = self.sizes[address]
        off = address - BASE
        return self.img.data[off:off + size]

    def got_slot(self, slot: int) -> tuple[int | None, str | None]:
        """(target address, import name) of the GOT slot at absolute address `slot`.

        A RELATIVE relocation gives the address; GLOB_DAT/JUMP_SLOT/ABS64 give the symbol's
        name and, when the module defines it, its address too. (None, None) for an unknown slot.
        """
        r = self.img.relocs.get(slot - BASE)
        if r is None:
            return None, None
        if r.type == R_AARCH64_RELATIVE:
            return BASE + r.addend, None
        if r.sym:
            s = self.dynsym.get(r.sym)
            defined = s is not None and s["shndx"] != 0 and s["value"] != 0
            return (BASE + s["value"] + r.addend if defined else None), r.sym
        return None, None

    def plt_import(self, address: int) -> str | None:
        """The import a PLT stub at `address` jumps through (adrp x16; ldr x17, [x16, #lo12]), else None."""
        off = address - BASE
        if address in self.sizes or not self.img.in_text(off) or off + 8 > len(self.img.data):
            return None
        adrp, ldr = self.img.u32(off), self.img.u32(off + 4)
        if adrp & 0x9F00001F != 0x90000010 or ldr & 0xFFC003FF != 0xF9400211:
            return None
        slot = adrp_target(adrp, address) + ((ldr >> 10) & 0xFFF) * 8
        _, name = self.got_slot(slot)
        return name


def _sext(value: int, bits: int) -> int:
    return value - (1 << bits) if value & (1 << (bits - 1)) else value


def adrp_target(insn: int, pc: int) -> int:
    """The page an ADRP at pc computes."""
    imm = ((insn >> 5) & 0x7FFFF) << 2 | (insn >> 29) & 3
    return (pc & ~0xFFF) + (_sext(imm, 21) << 12)


def branch_target(insn: int, pc: int) -> int:
    """The target of a B/BL at pc."""
    return pc + _sext(insn & 0x3FFFFFF, 26) * 4


def lo12_field(insn: int, rtype: int) -> int:
    """The low 12 bits of the address an ADD/LDST lo12 instruction encodes."""
    return ((insn >> 10) & 0xFFF) << LO12_SCALE.get(rtype, 3)


def read_object(path: Path) -> list[ObjFunction]:
    """Every defined function in an ELF object, with its code and relocations."""
    out = []
    with path.open("rb") as f:
        elf = ELFFile(f)
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        # Named symbols per section, to turn a section-relative reference (how the assembler
        # writes a reference to a local symbol) back into symbol + addend. `$x`/`$d` are the
        # AArch64 mapping symbols.
        in_section: dict[int, list[tuple[int, int, str]]] = {}
        for s in syms:
            if (s.name and not s.name.startswith("$") and s["st_info"]["type"] != "STT_SECTION"
                    and isinstance(s["st_shndx"], int)):
                in_section.setdefault(s["st_shndx"], []).append((s["st_value"], s["st_size"], s.name))
        for v in in_section.values():
            v.sort()

        def describe(r) -> ObjReloc:
            s = syms[r["r_info_sym"]]
            addend = r["r_addend"]
            if s["st_info"]["type"] != "STT_SECTION":
                return ObjReloc(r["r_info_type"], s.name, addend)
            shndx = s["st_shndx"]
            for value, size, name in reversed(in_section.get(shndx, [])):
                if value <= addend < value + max(size, 1):
                    return ObjReloc(r["r_info_type"], name, addend - value)
            return ObjReloc(r["r_info_type"], elf.get_section(shndx).name, addend, section=True)

        relocs_by_section: dict[int, dict[int, ObjReloc]] = {}
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection):
                target = sec["sh_info"]
                for r in sec.iter_relocations():
                    relocs_by_section.setdefault(target, {})[r["r_offset"]] = describe(r)
        for sym in syms:
            if sym["st_info"]["type"] != "STT_FUNC" or sym["st_shndx"] in ("SHN_UNDEF", "SHN_ABS"):
                continue
            sec = elf.get_section(sym["st_shndx"])
            start, size = sym["st_value"], sym["st_size"]
            code = sec.data()[start:start + size]
            rel = {o - start: r for o, r in relocs_by_section.get(sym["st_shndx"], {}).items()
                   if start <= o < start + size}
            out.append(ObjFunction(sym.name, code, rel))
    return out


def object_functions(path: Path) -> list[tuple[str, bytes, dict[int, int]]]:
    """(symbol, code bytes, {offset: reloc type}) for each defined function in an ELF object."""
    return [(f.symbol, f.code, {o: r.type for o, r in f.relocs.items()}) for f in read_object(path)]


def strip_params(demangled: str) -> str:
    """Drop a trailing parameter list and qualifiers: 'a::b(int) const' -> 'a::b'.

    Unlike demangle(strip=True) it leaves names that only contain parentheses alone, so a
    function-local static 'f()::s' is not mistaken for 'f'.
    """
    name = QUALIFIERS.sub("", demangled)
    if not name.endswith(")"):
        return demangled
    depth = 0
    for i in range(len(name) - 1, -1, -1):
        depth += {")": 1, "(": -1}.get(name[i], 0)
        if depth == 0:
            return name[:i]
    return demangled


_CXXFILT: str | None = None


def cxxfilt() -> str:
    """llvm-cxxfilt's path (hostenv.cxxfilt), looked up once."""
    global _CXXFILT
    if _CXXFILT is None:
        _CXXFILT = hostenv.cxxfilt()
    return _CXXFILT


def demangle(names: list[str], strip: bool = True) -> dict[str, str]:
    """Symbol -> demangled name, through one llvm-cxxfilt subprocess for the whole batch; `strip`
    cuts at the first '(' (the qualified function name). Raises OSError when llvm-cxxfilt is missing."""
    names = [n for n in names if n]
    if not names:
        return {}
    out = subprocess.run([cxxfilt()], input="\n".join(names), capture_output=True, text=True,
                         check=True).stdout.splitlines()
    return {n: o.split("(")[0] if strip else o for n, o in zip(names, out)}


def target_resolver(full_names: dict[str, str], defined: dict[str, int], fallback=None):
    """resolve(symbol) -> the original address a relocation's symbol stands for, or None.

    In order: a function the object itself defines (`defined`: symbol -> the address its diff
    used, so a call into the same object checks against that function), the symbol's demangled
    name (`full_names`, else the symbol itself) with its parameter list stripped when it is
    fn_<addr>/g_unk_<addr>/unk_<addr> in any namespace (TARGET_RE), then fallback(name) on that
    stripped name (check.py: a name unique in data/functions/).
    """
    def resolve(sym: str) -> int | None:
        if sym in defined:
            return defined[sym]
        name = strip_params(full_names.get(sym, sym))
        m = TARGET_RE.match(name)
        if m:
            return int(m.group(1), 16)
        return fallback(name) if fallback else None
    return resolve


def object_resolver(functions: list[ObjFunction], results: list[FunctionResult], fallback=None):
    """target_resolver for one object: its relocation symbols demangled in a single llvm-cxxfilt
    call, its diffed functions as the defined set. Raises OSError without llvm-cxxfilt."""
    symbols = sorted({r.symbol for f in functions for r in f.relocs.values() if not r.section})
    return target_resolver(demangle(symbols, strip=False), {res.symbol: res.address for res in results}, fallback)


def diff_functions(functions: list[ObjFunction], original: Original, resolve=None,
                   strict: bool = False) -> list[FunctionResult]:
    """Diff every function that maps to an original address (see diff_object).

    With strict=True, each function whose bytes match is also checked for its relocation targets
    (check_relocs through object_resolver): the mismatches go in FunctionResult.reloc_mismatches
    and make `matching` False. Section-relative and string relocations, conditional branches and
    symbols that do not resolve stay unchecked, never a failure.
    """
    results = []
    for fn in functions:
        m = NAME_RE.search(fn.symbol)
        addr = int(m.group(1), 16) if m else (resolve(fn.symbol) if resolve else None)
        if addr is None:
            continue
        if addr not in original.sizes:
            raise SystemExit(f"{fn.symbol}: {addr:#x} is not a known function start")
        orig = original.function(addr)
        res = FunctionResult(fn.symbol, addr, len(orig), len(fn.code), relocs=fn.relocs)
        for i in range(min(len(orig), len(fn.code)) // 4):
            a, = struct.unpack_from("<I", orig, 4 * i)
            b, = struct.unpack_from("<I", fn.code, 4 * i)
            r = fn.relocs.get(4 * i)
            mask = MASKS.get(r.type, 0xFFFFFFFF) if r else 0xFFFFFFFF
            if (a & mask) != (b & mask):
                res.diff_insns.append(i)
        results.append(res)
    if strict:
        try:
            targets = object_resolver(functions, results)
        except OSError as e:
            raise SystemExit(f"the strict relocation check needs llvm-cxxfilt ({cxxfilt()}): {e}")
        for res in results:
            if res.bytes_match:
                res.reloc_mismatches = check_relocs(res, original, targets).mismatches
    return results


def diff_object(obj: Path, original: Original, resolve=None, strict: bool = False) -> list[FunctionResult]:
    """Diff every function in obj that maps to an original address.

    A symbol maps through fn_<addr> in its name, or through resolve(symbol) -> address|None.
    strict=True also rejects a byte match whose relocations name the wrong target (diff_functions).
    """
    return diff_functions(read_object(obj), original, resolve, strict)


def default_resolve(symbol: str) -> int | None:
    """fn_<addr> / g_unk_<addr> / unk_<addr> symbols (plain C names) -> address, without llvm-cxxfilt;
    object_resolver and check.py's resolver also demangle."""
    m = TARGET_RE.match(symbol)
    return int(m.group(1), 16) if m else None


def check_relocs(res: FunctionResult, original: Original, resolve=default_resolve) -> RelocReport:
    """Compare every relocation target in a *matching* function with what the original encodes.

    resolve(symbol) -> the original address of that symbol, or None. The instruction at each
    relocated offset is the same kind on both sides once the bytes match, so the original's
    fields can be decoded by our relocation's type.
    """
    rep = RelocReport()
    orig = original.function(res.address)
    items = sorted(res.relocs.items())

    def insn(off: int) -> int:
        return struct.unpack_from("<I", orig, off)[0]

    # Pair each page relocation with the first following lo12 relocation on the same
    # symbol+addend (up to the next page load of that target); the pair checks one address.
    # Grouped by target first, so each scan covers one target's few uses, not the whole function.
    by_target: dict[tuple[str, int, bool], list[tuple[int, ObjReloc]]] = {}
    for off, r in items:
        by_target.setdefault((r.symbol, r.addend, r.section), []).append((off, r))
    partner: dict[int, int] = {}
    paired: set[int] = set()
    for uses in by_target.values():
        for i, (off, r) in enumerate(uses):
            if r.type in ADRP_TYPES or r.type == GOT_PAGE:
                want = set(LO12_SCALE) if r.type in ADRP_TYPES else {GOT_LO12}
                for off2, r2 in uses[i + 1:]:
                    if r2.type == r.type:
                        break
                    if r2.type in want and off2 not in paired:
                        partner[off] = off2
                        paired.update((off, off2))
                        break

    def record(kind: str, r: ObjReloc, off: int, expected: int | None, actual: int | None,
               name: str | None = None, ok: bool | None = None) -> None:
        if ok is None:
            ok = actual == expected
        if ok:
            rep.checked[kind] += 1
        else:
            rep.mismatches.append(RelocMismatch(off, kind, r.symbol, r.addend, expected, actual, name))

    for off, r in items:
        if off in paired and off not in partner:
            continue  # the lo12 half: handled with its page relocation
        pc = res.address + off
        if r.section:
            rep.unchecked["string" if r.symbol.startswith(".rodata.str") else "section"] += 1
            continue
        if r.type in (CONDBR19, TSTBR14):
            rep.unchecked["condbr19" if r.type == CONDBR19 else "tstbr14"] += 1
            continue
        if r.type not in MASKS:
            rep.unchecked["other"] += 1
            continue
        target = resolve(r.symbol)
        expected = None if target is None else target + r.addend

        if r.type in (CALL26, JUMP26):
            kind = "call26" if r.type == CALL26 else "jump26"
            actual = branch_target(insn(off), pc)
            name = original.plt_import(actual)
            if expected is not None:
                record(kind, r, off, expected, actual, name)
            elif name is not None:
                record(kind + "-plt", r, off, None, actual, name, ok=name == r.symbol)
            else:
                rep.unchecked["unresolved"] += 1
        elif r.type == GOT_PAGE:
            if off not in partner:
                rep.unchecked["got-unpaired"] += 1
                continue
            slot = adrp_target(insn(off), pc) + lo12_field(insn(partner[off]), GOT_LO12)
            actual, name = original.got_slot(slot)
            if expected is not None:
                record("got", r, off, expected, actual, name)
            elif name is not None:
                record("got-import", r, off, None, actual, name, ok=name == r.symbol)
            elif actual is not None:
                rep.unchecked["unresolved"] += 1
            else:
                rep.unchecked["got-unknown"] += 1
        elif r.type == GOT_LO12:
            rep.unchecked["got-unpaired"] += 1
        elif expected is None:
            rep.unchecked["unresolved"] += 1
        elif r.type in ADRP_TYPES:
            page = adrp_target(insn(off), pc)
            if off in partner:
                r2 = res.relocs[partner[off]]
                record("adrp+lo12", r, off, expected, page + lo12_field(insn(partner[off]), r2.type))
            else:
                record("adrp", r, off, expected & ~0xFFF, page)
        else:  # an lo12 without its page load (the page was hoisted for another addend)
            record("lo12", r, off, expected & 0xFFF, lo12_field(insn(off), r.type))
    return rep


def format_mismatch(m: RelocMismatch, name: str | None = None) -> str:
    """`sym+addend -> original 0x... (kind) +offset`, with the import's name where the original has one."""
    sym = name or m.symbol
    if m.addend:
        sym += f"{m.addend:+#x}"
    if m.actual is not None:
        actual = f"{m.actual:#x}" + (f" ({m.actual_name})" if m.actual_name else "")
    else:
        actual = f"{m.actual_name} (import)" if m.actual_name else "nothing known"
    if m.kind == "lo12":
        actual = f"low bits {m.actual:#x}"
    elif m.kind == "adrp":
        actual = f"page {m.actual:#x}"
    return f"{sym} -> original {actual}  {m.offset:+#x}  {m.kind}"


def reloc_rejects(results: list[FunctionResult], limit: int = 10) -> list[str]:
    """Lines for a generator's log about the byte-identical functions strict mode rejected for a
    wrong relocation target: a count, up to `limit` of them with their first mismatch
    (format_mismatch), then the addresses of the rest. Empty when nothing was rejected."""
    bad = sorted((r for r in results if r.reloc_mismatches), key=lambda r: r.address)
    if not bad:
        return []
    lines = [f"{len(bad)} byte-identical candidate(s) rejected for a wrong relocation target:"]
    for r in bad[:limit]:
        more = f"  (+{len(r.reloc_mismatches) - 1} more)" if len(r.reloc_mismatches) > 1 else ""
        lines.append(f"  reloc  {r.address:#x}  {format_mismatch(r.reloc_mismatches[0])}{more}")
    if len(bad) > limit:
        lines.append(f"  ... and {len(bad) - limit} more: " + " ".join(f"{r.address:x}" for r in bad[limit:]))
    return lines


def print_side_by_side(res: FunctionResult, original: Original, code: bytes) -> None:
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    orig = original.function(res.address)

    def lines(b: bytes, base: int) -> list[str]:
        return [f"{i.mnemonic} {i.op_str}" for i in cs.disasm(b, base)]

    left, right = lines(orig, res.address), lines(code, res.address)
    for i in range(max(len(left), len(right))):
        mark = "  " if i not in res.diff_insns and i < min(len(left), len(right)) else "!!"
        lt = left[i] if i < len(left) else ""
        rt = right[i] if i < len(right) else ""
        print(f"  {mark} {i:3} {lt:40} {rt}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("objects", nargs="+", type=Path)
    ap.add_argument("-v", "--verbose", action="store_true", help="print side-by-side disassembly")
    ap.add_argument("--no-reloc", action="store_true", help="skip the relocation target check")
    args = ap.parse_args()
    # check.py's resolution (the data/functions name map on top of the demangled names) when its
    # tools are present; otherwise this module's own demangling, and without llvm-cxxfilt plain
    # fn_/g_unk_ symbol names, all a C-named object needs.
    try:
        import check
        by_name = check.csv_by_name(check.load_csv()[0])
        demangle(["_Z1fv"])
    except Exception as e:  # an import error, or llvm-cxxfilt missing
        check = None
        print(f"note: {e}; resolving fn_/g_unk_ symbol names only", file=sys.stderr)
    original = Original(args.version)
    total = matched = 0
    report = RelocReport()
    for obj in args.objects:
        functions = read_object(obj)
        codes = {f.symbol: f.code for f in functions}
        if check is not None:
            diffs, names, full_names, targets = check.diff_with_targets(functions, original, by_name)
        else:
            diffs, names, full_names = diff_functions(functions, original), {}, {}
            try:
                targets = object_resolver(functions, diffs)
            except OSError:
                targets = default_resolve
        for r in diffs:
            total += 1
            matched += r.matching
            status = "MATCH" if r.matching else f"{r.score:5.1%}"
            print(f"{status:>6}  {names.get(r.symbol, r.symbol)}  orig {r.orig_size} B, ours {r.our_size} B, "
                  f"{len(r.diff_insns)} differing insns")
            if r.matching and not args.no_reloc:
                rep = check_relocs(r, original, targets)
                report.merge(rep)
                for m in rep.mismatches:
                    sym = full_names.get(m.symbol, m.symbol)
                    print(f"  RELOC  {format_mismatch(m, strip_params(sym))}")
            if args.verbose and not r.matching:
                print_side_by_side(r, original, codes[r.symbol])
    print(f"{matched}/{total} functions match")
    if not args.no_reloc:
        print(f"relocations: {sum(report.checked.values())} checked, {len(report.mismatches)} mismatched, "
              f"{sum(report.unchecked.values())} unchecked"
              + (" (" + ", ".join(f"{k} {v}" for k, v in sorted(report.unchecked.items())) + ")"
                 if report.unchecked else ""))
    return 0 if matched == total and not report.mismatches else 1


if __name__ == "__main__":
    sys.exit(main())
