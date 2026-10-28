#!/usr/bin/env python3
"""Compare two trees of compiled objects function by function: code bytes and relocations.

Evidence that a refactor that should not change code (moving files, renaming includes, merging
headers) did not: build the tree before and after with `tools/check.py --compile-only` (objects
under build/check/<label>/), then compare. Functions are keyed by object file name and symbol, so a
file that moved to another directory is still compared with itself; a relocation is compared by
type, offset, target symbol and addend. Needs no game data.

Usage: python tools/objcompare.py build/units_before build/check/compile-only
"""

from __future__ import annotations

import argparse
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402


def functions(tree: Path) -> tuple[dict[tuple[str, str], tuple], list[str]]:
    out: dict[tuple[str, str], tuple] = {}
    clashes = []
    for obj in sorted(tree.rglob("*.o")):
        for f in asmdiff.read_object(obj):
            key = (obj.name, f.symbol)
            value = (f.code, tuple(sorted((o, r.type, r.symbol, r.addend, r.section) for o, r in f.relocs.items())))
            if key in out and out[key] != value:
                clashes.append(f"{obj.name} {f.symbol}")
            out[key] = value
    return out, clashes


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("before", type=Path)
    ap.add_argument("after", type=Path)
    args = ap.parse_args()
    a, ca = functions(args.before)
    b, cb = functions(args.after)
    for c in ca + cb:
        print(f"AMBIGUOUS  {c} (two objects of this name define it differently)")
    missing = sorted(set(a) - set(b))
    added = sorted(set(b) - set(a))
    changed = sorted(k for k in set(a) & set(b) if a[k] != b[k])
    for kind, keys in (("MISSING", missing), ("ADDED", added), ("CHANGED", changed)):
        for obj, sym in keys[:50]:
            print(f"{kind}  {obj}  {sym}")
    print(f"objcompare: {len(a)} functions before, {len(b)} after; {len(missing)} missing, {len(added)} added, "
          f"{len(changed)} changed")
    return 1 if missing or added or changed or ca or cb else 0


if __name__ == "__main__":
    raise SystemExit(main())
