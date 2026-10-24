#!/usr/bin/env python3
"""Rank U functions in an address range by how close they are to an O function of the same size.

Hand-matching starting points (the dread-match skill): a U function whose normalised
instructions equal an O function's is usually that function's source under another name.
Normalising masks branch and call targets, page addresses and the immediates of add and
unsigned-offset loads/stores, so struct offsets and element sizes may still differ.

The refs column counts call and data (adrp + add/ldr) targets that differ between the two.
tools/trymatch.py and tools/check.py mask relocated fields, so a copy with refs > 0 scores O
even when it calls the wrong function or reads the wrong global: run --refs and declare the
copy's own callees and data where they differ.

Usage:
  python tools/nearcopy.py --version 2.1.0 LO HI [--min 36] [--top 200]
  python tools/nearcopy.py --version 2.1.0 --refs U O
Output is addresses and scores only; never commit it. Don't run it while check.py --update
rewrites data/functions/.
"""

from __future__ import annotations

import argparse
import csv
import struct
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GENERATED = ("src/lifted/", "src/reflection", "src/clones/", "src/tables/")


def norm(code: bytes) -> list[int]:
    out = []
    for (w,) in struct.iter_unpack("<I", code):
        if w & 0x9F000000 == 0x90000000: w &= 0x9F00001F          # adrp page
        elif w & 0x7C000000 == 0x14000000: w &= 0xFC000000         # b / bl target
        elif w & 0xFF000010 == 0x54000000: w &= 0xFF00001F         # b.cond
        elif w & 0x7E000000 in (0x34000000, 0x36000000): w &= 0xFF00001F  # cbz/cbnz/tbz/tbnz
        elif w & 0x7F800000 == 0x11000000: w &= 0xFFC003FF         # add #imm (lo12)
        elif w & 0x3B000000 == 0x39000000: w &= 0xFFC003FF         # ldr/str unsigned offset
        out.append(w)
    return out


def refs(addr: int, code: bytes) -> list[int]:
    """Absolute targets in order: calls/tail calls outside the function, adrp+add/ldr addresses."""
    out, page = [], {}
    for i, (w,) in enumerate(struct.iter_unpack("<I", code)):
        pc = addr + 4 * i
        if w & 0x7C000000 == 0x14000000:
            t = pc + (((w & 0x3FFFFFF) ^ 0x2000000) - 0x2000000) * 4
            if not addr <= t < addr + len(code): out.append(t)
        elif w & 0x9F000000 == 0x90000000:
            imm = ((w >> 29) & 3) | ((w >> 3) & 0x1FFFFC)
            page[w & 31] = (pc & ~0xFFF) + ((imm ^ 0x100000) - 0x100000) * 4096
        elif w & 0x7F800000 == 0x11000000 and (w >> 5) & 31 in page:
            out.append(page[(w >> 5) & 31] + ((w >> 10) & 0xFFF))
            page.pop(w & 31, None)  # the destination no longer holds a page
        elif w & 0x3B000000 == 0x39000000 and (w >> 5) & 31 in page:
            out.append(page[(w >> 5) & 31] + (((w >> 10) & 0xFFF) << (w >> 30)))
            if w & 0x400000:  # a load overwrites its target register
                page.pop(w & 31, None)
    return out


def defining_file(addr: int) -> str:
    """The src/ file that defines addr: hand-written files first, then generated ones."""
    out = subprocess.run(["git", "grep", "-n", f"{addr:x}", "--", "src/"], capture_output=True, text=True, cwd=ROOT).stdout
    found = []
    for line in out.splitlines():
        path, _, text = line.split(":", 2)
        t = text.split("//", 1)[0].strip()  # an address in a trailing comment is not a definition
        if f"{addr:x}" not in t:
            continue
        clone = t.startswith("template ") and f"UnkClone_fn_{addr:x}" in t
        if not clone and (t.endswith(";") or "callee" in t):
            continue
        found.append(path)
    found.sort(key=lambda p: p.startswith(GENERATED))
    return found[0] if found else "?"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("lo", nargs="?"); ap.add_argument("hi", nargs="?")
    ap.add_argument("--min", type=int, default=36); ap.add_argument("--top", type=int, default=200)
    ap.add_argument("--refs", nargs=2, metavar=("U", "O"))
    a = ap.parse_args()
    orig = asmdiff.Original(a.version)
    if a.refs:
        u, o = (int(x, 16) for x in a.refs)
        ru, ro = refs(u, orig.function(u)), refs(o, orig.function(o))
        for i, (x, y) in enumerate(zip(ru, ro)):
            print(f"{i:3} U 0x{x:x}  O 0x{y:x}" + ("" if x == y else "   <- differs"))
        if len(ru) != len(ro): print(f"ref counts differ: {len(ru)} vs {len(ro)}")
        return 0
    lo, hi = int(a.lo, 16), int(a.hi, 16)
    rows = [r for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")) for r in csv.DictReader(f.open(newline=""))]
    K = 4  # chunk length in instructions
    index, code = defaultdict(list), {}
    for r in rows:
        if r["status"] == "O" and int(r["size"]) >= a.min:
            ad = int(r["address"], 16)
            code[ad] = n = norm(orig.function(ad))
            for i in range(0, len(n), K):
                index[(len(n), i, tuple(n[i:i + K]))].append(ad)
    best = []
    for r in rows:
        ad, sz = int(r["address"], 16), int(r["size"])
        if r["status"] != "U" or not lo <= ad < hi or sz < a.min or sz > 2048:
            continue
        u = norm(orig.function(ad))
        hits = defaultdict(int)
        for i in range(0, len(u), K):
            for oa in index.get((len(u), i, tuple(u[i:i + K])), ()):
                hits[oa] += 1
        if not hits:
            continue
        cands = sorted(hits, key=hits.get, reverse=True)[:5]
        score, src = max((sum(x == y for x, y in zip(u, code[oa])) / len(u), oa) for oa in cands)
        best.append((score, sz, ad, src))
    best.sort(key=lambda t: (-t[0], -t[1]))
    full = sum(s == 1 for s, *_ in best)
    near = sum(0.9 <= s < 1 for s, *_ in best)
    print(f"{len(best)} candidates: {full} at 100%, {near} at 90-99%", file=sys.stderr)
    print("score  size  U  O  refs-differ  O-source")
    for score, sz, ad, src in best[:a.top]:
        ru, ro = refs(ad, orig.function(ad)), refs(src, orig.function(src))
        nd = sum(x != y for x, y in zip(ru, ro)) + abs(len(ru) - len(ro))
        print(f"{score:6.1%} {sz:5} U=0x{ad:x}  O=0x{src:x}  refs={nd}  {defining_file(src)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
