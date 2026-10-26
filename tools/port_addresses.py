#!/usr/bin/env python3
"""Recover the original address of the symbols tools/port_link.py cannot place, from matched code.

port_link's residue keeps two kinds of names it has no address for: vtables of classes whose
virtual functions are not all defined (`vtable for CAttack`) and functions named by something
other than fn_<addr> (`UnkAttackRefCounted::Delete()`). The game's own code knows where each one
is: a function whose bytes match the original (tools/check.py's diff) loads the vtable or calls
the function, and the original instruction at that relocation encodes the address. This tool
compiles nothing new: it reads tools/check.py's AArch64 objects (build/check/<version>/, rebuilt
for any source without one), diffs each with asmdiff exactly as check.py does, and for every
byte-matching function asks the relocation check to compare each relocation against the symbol
it cannot resolve as if that symbol were at 0: the "mismatch" it reports is the original's
target. Call and branch targets, page-and-offset pairs and GOT slots count; a lone page or
offset half does not.

It places every symbol the relocation check cannot resolve (not only port_link's residue, so the
output does not depend on a link that already used it). Writes build/port/addresses.json: per
mangled symbol, the address, how many relocations agree and in how many functions, and the kind;
symbols whose uses disagree are listed apart, with the functions behind each address, and left
out: one name standing for several things is a naming problem in the sources (a placeholder class
name reused for different classes, a declaration naming the wrong function), to fix there.
tools/port_link.py reads it (--addresses, by default this file when present): a vtable becomes an
absolute symbol at the game's vtable (the data loader makes its slots x86-64 code), a function an
alias of the definition at that address or a trap with it. Only addresses are written, derived
from the user's dump; build/ is not tracked.

Usage:
  python tools/port_addresses.py --version 2.1.0      # -> build/port/addresses.json (about a minute)
  python tools/port_link.py --version 2.1.0           # then the link uses it
"""

from __future__ import annotations

import argparse
import json
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
FULL_KINDS = {"call26", "jump26", "adrp+lo12", "got"}  # kinds whose `actual` is a whole address


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", required=True)
    ap.add_argument("--out", default=str(ROOT / "build" / "port" / "addresses.json"))
    ap.add_argument("-j", "--jobs", type=int, default=8)
    args = ap.parse_args()

    rows, _ = check.load_csv()
    by_name = check.csv_by_name(rows)
    original = asmdiff.Original(args.version)
    srcs = check.sources()

    def obj_for(src: Path) -> Path | None:
        obj = check.object_path(args.version, src)
        if obj.is_file() and obj.stat().st_mtime >= src.stat().st_mtime:
            return obj
        return check.build(args.version, src)

    with ThreadPoolExecutor(args.jobs) as pool:
        objects = list(pool.map(obj_for, srcs))

    found: dict[str, Counter] = defaultdict(Counter)  # mangled -> address -> uses
    kinds: dict[str, Counter] = defaultdict(Counter)
    functions: dict[str, set[int]] = defaultdict(set)
    users: dict[tuple[str, int], set[int]] = defaultdict(set)  # (symbol, address) -> functions using it
    demangled_of: dict[str, str] = {}
    matched = 0
    for src, obj in zip(srcs, objects):
        if obj is None:
            continue
        diffs, _, full_names, targets = check.diff_with_targets(asmdiff.read_object(obj), original, by_name)
        unknown: set[str] = set()

        def probe(sym: str) -> int | None:
            t = targets(sym)
            if t is not None:
                return t
            unknown.add(sym)
            return 0  # placed at 0: the mismatch reports the original's target

        for res in diffs:
            if not res.bytes_match:
                continue
            matched += 1
            rep = asmdiff.check_relocs(res, original, probe)
            for m in rep.mismatches:
                if m.symbol not in unknown or m.kind not in FULL_KINDS or m.actual is None:
                    continue
                addr = m.actual - m.addend
                found[m.symbol][addr] += 1
                kinds[m.symbol][m.kind] += 1
                functions[m.symbol].add(res.address)
                users[(m.symbol, addr)].add(res.address)
                demangled_of[m.symbol] = full_names.get(m.symbol, m.symbol)

    resolved, conflicts = {}, {}
    for sym, addrs in sorted(found.items()):
        entry = {"demangled": demangled_of[sym], "uses": sum(addrs.values()), "functions": len(functions[sym]),
                 "kinds": dict(kinds[sym])}
        if len(addrs) == 1:
            (a,) = addrs
            resolved[sym] = {"address": f"0x{a:x}", **entry}
        else:
            conflicts[sym] = {"addresses": {f"0x{a:x}": {"uses": n, "functions": [f"0x{f:x}" for f in sorted(users[(sym, a)])]}
                                            for a, n in addrs.most_common()}, **entry}
    out = Path(args.out)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps({"version": args.version, "resolved": resolved, "conflicts": conflicts}, indent=1)
                   + "\n", encoding="utf-8")
    vt = sum(1 for e in resolved.values() if e["demangled"].startswith("vtable for "))
    print(f"{matched} byte-matching functions read; {len(resolved)} symbols placed ({vt} vtables,"
          f" {len(resolved) - vt} others), {len(conflicts)} with disagreeing uses")
    for sym, e in list(conflicts.items())[:10]:
        print(f"  conflict: {e['demangled']}: "
              + "; ".join(f"{a} ({v['uses']} uses, in {', '.join(v['functions'][:3])}{', ...' if len(v['functions']) > 3 else ''})"
                          for a, v in e["addresses"].items()))
    print(f"json: {out.relative_to(ROOT).as_posix() if out.is_relative_to(ROOT) else out}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
