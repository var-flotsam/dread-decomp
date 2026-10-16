#!/usr/bin/env python3
"""Compare functions in a compiled AArch64 object file against the original binary.

Functions are matched by name: any symbol containing fn_<10 hex digits> is
compared against that address in the original module (size from .eh_frame).
Instruction fields that a relocation fills in (ADRP pages, lo12 offsets, BL/B
targets, conditional branch offsets) are masked on both sides.

Usage:
  python tools/asmdiff.py --version 2.1.0 build/obj/foo.o [-v]
As a library: diff_object(obj_path, original) -> list[FunctionResult]

Disassembly printed with -v is for discussion only. Never write it into
tracked files.
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
from image import BASE, Image  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
NAME_RE = re.compile(r"fn_([0-9a-fA-F]{10})")

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


@dataclass
class FunctionResult:
    symbol: str
    address: int
    orig_size: int
    our_size: int
    diff_insns: list[int] = field(default_factory=list)  # indices of differing instructions

    @property
    def matching(self) -> bool:
        return self.orig_size == self.our_size and not self.diff_insns

    @property
    def score(self) -> float:
        n = max(self.orig_size, self.our_size) // 4
        same = min(self.orig_size, self.our_size) // 4 - len(self.diff_insns)
        return same / n if n else 0.0


class Original:
    def __init__(self, version: str, module: str = "main"):
        self.img = Image(ROOT / "orig" / version / "exefs" / module)
        fdes = json.loads((ROOT / "build" / "recon" / version / f"{module}_eh_functions.json").read_text())
        self.sizes = {int(a, 16): n for a, n in fdes}

    def function(self, address: int) -> bytes:
        size = self.sizes[address]
        off = address - BASE
        return self.img.data[off:off + size]


def object_functions(path: Path) -> list[tuple[str, bytes, dict[int, int]]]:
    """(symbol, code bytes, {offset: reloc type}) for each defined function in an ELF object."""
    out = []
    with path.open("rb") as f:
        elf = ELFFile(f)
        relocs_by_section: dict[int, dict[int, int]] = {}
        for sec in elf.iter_sections():
            if isinstance(sec, RelocationSection):
                target = sec["sh_info"]
                for r in sec.iter_relocations():
                    relocs_by_section.setdefault(target, {})[r["r_offset"]] = r["r_info_type"]
        symtab = elf.get_section_by_name(".symtab")
        for sym in symtab.iter_symbols():
            if sym["st_info"]["type"] != "STT_FUNC" or sym["st_shndx"] in ("SHN_UNDEF", "SHN_ABS"):
                continue
            sec = elf.get_section(sym["st_shndx"])
            start, size = sym["st_value"], sym["st_size"]
            code = sec.data()[start:start + size]
            rel = {o - start: t for o, t in relocs_by_section.get(sym["st_shndx"], {}).items()
                   if start <= o < start + size}
            out.append((sym.name, code, rel))
    return out


def diff_object(obj: Path, original: Original) -> list[FunctionResult]:
    results = []
    for name, code, rel in object_functions(obj):
        m = NAME_RE.search(name)
        if not m:
            continue
        addr = int(m.group(1), 16)
        if addr not in original.sizes:
            raise SystemExit(f"{name}: {addr:#x} is not a known function start")
        orig = original.function(addr)
        res = FunctionResult(name, addr, len(orig), len(code))
        for i in range(min(len(orig), len(code)) // 4):
            a, = struct.unpack_from("<I", orig, 4 * i)
            b, = struct.unpack_from("<I", code, 4 * i)
            mask = MASKS.get(rel.get(4 * i), 0xFFFFFFFF)
            if (a & mask) != (b & mask):
                res.diff_insns.append(i)
        results.append(res)
    return results


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
    args = ap.parse_args()
    original = Original(args.version)
    total = matched = 0
    for obj in args.objects:
        codes = {n: c for n, c, _ in object_functions(obj)}
        for r in diff_object(obj, original):
            total += 1
            matched += r.matching
            status = "MATCH" if r.matching else f"{r.score:5.1%}"
            print(f"{status:>6}  {r.symbol}  orig {r.orig_size} B, ours {r.our_size} B, "
                  f"{len(r.diff_insns)} differing insns")
            if args.verbose and not r.matching:
                print_side_by_side(r, original, codes[r.symbol])
    print(f"{matched}/{total} functions match")
    return 0 if matched == total else 1


if __name__ == "__main__":
    sys.exit(main())
