#!/usr/bin/env python3
"""Shape census: group the functions nothing in src/ defines by size and mnemonic sequence.

The largest families of same-shaped `U` functions are the best candidates for a template in
tools/gen_clones.py (or a lifter fix). Prints the families of at least --min members, ranked by
total bytes: a representative address, the size, the member count and the total bytes.
--family ADDR shows, for that function's family, each instruction index whose operands vary
across the copies, with the number of distinct operand strings and their counts (the parts a
template has to parameterise). Output is for reading only; never commit it.

Usage: python tools/shape_census.py --version 2.1.0 [--min 5] [--max-size 2048] [--top 30]
       python tools/shape_census.py --version 2.1.0 --family 7100123456
"""

from __future__ import annotations

import argparse
import csv
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import gen_clones  # noqa: E402
import gen_lift  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent


def undefined_u(orig: asmdiff.Original) -> list[int]:
    """`U` functions that no source in src/ defines (generated clones included)."""
    skip = gen_clones.defined_in_src()
    for f in gen_clones.OUT.rglob("*.cpp"):
        skip |= {int(a, 16) for a in gen_lift.TAG_RE.findall(f.read_text(encoding="utf-8"))}
    out = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            out += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] == "U"]
    return [a for a in out if a not in skip and a in orig.sizes]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--min", type=int, default=5, help="smallest family to list (default 5)")
    ap.add_argument("--max-size", type=int, help="skip functions larger than this many bytes")
    ap.add_argument("--top", type=int, help="list only the N largest families by total bytes")
    ap.add_argument("--family", help="address of a function: show the operands that vary in its family")
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)
    addrs = undefined_u(orig)
    if args.max_size:
        addrs = [a for a in addrs if orig.sizes[a] <= args.max_size]

    if args.family:
        at = int(args.family, 16)
        if at not in orig.sizes:
            raise SystemExit(f"{at:x} is not a function start")
        ref = gen_clones.instructions(orig, at)
        key = (orig.sizes[at], tuple(m for m, _ in ref))
        copies = [ref] + [ins for a in addrs if a != at and orig.sizes[a] == key[0]
                          for ins in [gen_clones.instructions(orig, a)] if tuple(m for m, _ in ins) == key[1]]
        print(f"{at:x}: {len(copies)} functions of {key[0]} bytes ({len(ref)} instructions) share its mnemonics")
        for i, (m, _) in enumerate(ref):
            ops = Counter(ins[i][1] for ins in copies)
            if len(ops) > 1:
                shown = dict(ops.most_common(8))
                more = f" (+{len(ops) - 8} more)" if len(ops) > 8 else ""
                print(f"{i} {m} {len(ops)} {shown}{more}")
        return 0

    families: dict[tuple, list[int]] = {}
    for a in addrs:
        key = (orig.sizes[a], tuple(m for m, _ in gen_clones.instructions(orig, a)))
        families.setdefault(key, []).append(a)
    ranked = sorted(((k, v) for k, v in families.items() if len(v) >= args.min),
                    key=lambda kv: (-kv[0][0] * len(kv[1]), kv[1][0]))
    total = sum(k[0] * len(v) for k, v in ranked)
    print(f"{len(addrs)} undefined U functions; {len(ranked)} families of {args.min}+ hold "
          f"{sum(len(v) for _, v in ranked)} functions, {total} bytes")
    print(f"{'address':>10}  {'size':>5}  {'count':>5}  {'total':>7}")
    for (size, _), members in ranked[:args.top]:
        print(f"{members[0]:x}  {size:5d}  {len(members):5d}  {size * len(members):7d}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
