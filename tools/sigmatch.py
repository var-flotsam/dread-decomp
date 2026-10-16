#!/usr/bin/env python3
"""Name library functions in the binary by matching compiled upstream code.

Compiles a vendored library (e.g. lib/lua) with the project toolchain, then looks for each
compiled function among main's functions by size and a position-independent signature:
the instruction words with call/branch targets (B/BL), ADRP pages and the lo12 offsets that
use an ADRP result masked out. The same masking is applied to both sides, so it doesn't
depend on relocation info. Only unique matches are accepted, and matches under 32 bytes only
inside the address range of the larger ones. Identical bodies the linker
folded, or tiny functions with many look-alikes, stay unnamed.

Writes data/library_symbols.csv (address, size, name, library), which
tools/gen_functions_csv.py uses to name these functions (name_source `library`, status L).

Usage: python tools/sigmatch.py --version 2.1.0 [--library lua]
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import struct
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
MIN_TRUSTED = 32
LIBRARIES = {
    "lua": {"sources": sorted((ROOT / "lib" / "lua").glob("*.c"))},
}


def signature(code: bytes) -> str:
    """Hash of the code with address-dependent fields masked."""
    adrp_regs: set[int] = set()
    words = []
    for i in range(len(code) // 4):
        w = struct.unpack_from("<I", code, 4 * i)[0]
        rd = w & 0x1F
        rn = (w >> 5) & 0x1F
        if w & 0x7C000000 == 0x14000000:          # B / BL
            w &= 0xFC000000
        elif w & 0x9F000000 == 0x90000000:        # ADRP
            w &= 0x9F00001F
            adrp_regs.add(rd)
            words.append(w)
            continue
        elif w & 0x9F000000 == 0x10000000:        # ADR
            w &= 0x9F00001F
        elif w & 0x7F800000 == 0x11000000 and rn in adrp_regs:   # ADD (imm) from an ADRP base
            w &= ~(0xFFF << 10) & 0xFFFFFFFF
        elif w & 0x3B000000 == 0x39000000 and rn in adrp_regs:   # LDR/STR (unsigned imm) from an ADRP base
            w &= ~(0xFFF << 10) & 0xFFFFFFFF
        # Any other write to rd ends its life as an ADRP page (stores don't write Rt).
        is_store = w & 0x3B000000 == 0x39000000 and not (w >> 22) & 1
        if not is_store:
            adrp_regs.discard(rd)
        words.append(w)
    return hashlib.sha1(struct.pack(f"<{len(words)}I", *words)).hexdigest()


def binary_index(original: asmdiff.Original) -> dict[tuple[int, str], list[int]]:
    index: dict[tuple[int, str], list[int]] = {}
    for addr, size in original.sizes.items():
        index.setdefault((size, signature(original.function(addr))), []).append(addr)
    return index


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--library", default="lua", choices=list(LIBRARIES))
    args = ap.parse_args()

    original = asmdiff.Original(args.version)
    index = binary_index(original)
    objdir = ROOT / "build" / "sigmatch" / args.version / args.library
    objdir.mkdir(parents=True, exist_ok=True)

    found: dict[int, tuple[int, str]] = {}
    ambiguous = missing = total = 0
    for src in LIBRARIES[args.library]["sources"]:
        obj = objdir / (src.stem + ".o")
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0:
            raise SystemExit(f"{src}: build failed\n{r.stderr[-1500:]}")
        funcs = asmdiff.object_functions(obj)
        names = check.demangle([n for n, _, _ in funcs])
        for name, code, _ in funcs:
            total += 1
            hits = index.get((len(code), signature(code)), [])
            if len(hits) == 1:
                found.setdefault(hits[0], (len(code), names.get(name, name)))
            elif hits:
                ambiguous += 1
            else:
                missing += 1

    # Tiny bodies (a load and a return) also occur in unrelated code. Trust matches of at least
    # MIN_TRUSTED bytes; accept smaller ones only inside the address range the trusted ones span.
    trusted = [a for a, (s, _) in found.items() if s >= MIN_TRUSTED]
    lo, hi = min(trusted), max(trusted)
    rejected = {a: v for a, v in found.items() if v[0] < MIN_TRUSTED and not lo <= a <= hi}
    for a in rejected:
        del found[a]

    out = ROOT / "data" / "library_symbols.csv"
    rows = []
    if out.is_file():
        with out.open(newline="", encoding="utf-8") as f:
            rows = [r for r in csv.DictReader(f) if r["library"] != args.library]
    rows += [{"address": f"{a:#x}", "size": s, "name": n, "library": args.library}
             for a, (s, n) in sorted(found.items())]
    rows.sort(key=lambda r: int(r["address"], 16))
    with out.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=["address", "size", "name", "library"], lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    print(f"{args.library}: {total} compiled functions; {len(found)} matched uniquely "
          f"(region {lo:#x}-{hi:#x}; {len(rejected)} small matches outside it rejected), "
          f"{ambiguous} ambiguous, {missing} not found -> {out.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
