#!/usr/bin/env python3
"""Whole-binary decompilation progress, from the status column of data/functions/.

Library functions (status L: nn::atk, SDK, runtime, Lua standard library) are excluded
from the totals, like BotW's "library function" status. Reports function counts and
code bytes by status, overall and per subsystem (inferred '?' subsystems are folded in).

objdiff's report (tools/objdiff_gen.py, then `objdiff-cli report generate`) covers only
the units in src/; this report covers the whole of main.

A slice adds the same status table for one part of the call graph (tools/callgraph.py, from
the Ghidra export, so it needs build/ghidra_export/<version>/): `--slice boot` is everything
reachable from nnMain, `--slice-from NAME|ADDR` everything reachable from that function. The
graph has direct calls only, so a slice is a lower bound; the output says so. Without a slice
the output is unchanged (docs quote it).

Usage: python tools/progress.py [--json build/progress.json] [--top 15]
       python tools/progress.py --slice boot [--version 2.1.0] [--json build/progress.json]
       python tools/progress.py --slice-from CGameBlackboard::refl_GetClass
"""

from __future__ import annotations

import argparse
import csv
import json
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
STATUSES = ["O", "m", "M", "W", "U"]
LABELS = {"O": "matching", "m": "minor", "M": "major", "W": "wip", "U": "not decompiled"}


def load() -> list[dict]:
    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows.extend(csv.DictReader(fh))
    return rows


def tally(rows: list[dict]) -> dict:
    t = {s: {"functions": 0, "bytes": 0} for s in STATUSES}
    for r in rows:
        t[r["status"]]["functions"] += 1
        t[r["status"]]["bytes"] += int(r["size"])
    total_f = sum(v["functions"] for v in t.values())
    total_b = sum(v["bytes"] for v in t.values())
    return {"by_status": t, "functions": total_f, "bytes": total_b,
            "matching_bytes_pct": 100 * t["O"]["bytes"] / total_b if total_b else 0.0,
            "usable_bytes_pct": 100 * (t["O"]["bytes"] + t["m"]["bytes"]) / total_b if total_b else 0.0}


def print_status(overall: dict) -> None:
    """The matching/usable line and the status table (shared by the whole binary and a slice)."""
    print(f"matching (O): {overall['by_status']['O']['functions']} functions, "
          f"{overall['matching_bytes_pct']:.4f}% of code; usable (O+m): {overall['usable_bytes_pct']:.4f}%")
    print()
    print("| Status | Functions | Bytes |")
    print("|---|---|---|")
    for s in STATUSES:
        v = overall["by_status"][s]
        print(f"| `{s}` {LABELS[s]} | {v['functions']:,} | {v['bytes']:,} |")


def slice_report(version: str, rows: list[dict], boot: bool, start: str | None) -> dict:
    """Status tally of the functions reachable (by direct calls) from nnMain or from `start`."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import callgraph  # noqa: E402  (needs the Ghidra export; imported only for a slice)

    graph = callgraph.CallGraph(version, rows={int(r["address"], 16): r for r in rows})
    root = callgraph.NNMAIN if boot else graph.resolve(start)
    reach = graph.reachable(root)
    members = [graph.funcs[a] for a in sorted(reach)]
    game = [r for r in members if r["status"] != "L"]
    return {"kind": "boot" if boot else "from", "root": graph.name(root), "root_address": f"0x{root:x}",
            "functions": len(members), "library_functions": len(members) - len(game),
            "overall": tally(game), "caveat": callgraph.CAVEAT}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--json", type=Path, help="also write the report as JSON")
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--version", default="2.1.0", help="dump version whose Ghidra export a slice uses (default 2.1.0)")
    sl = ap.add_mutually_exclusive_group()
    sl.add_argument("--slice", choices=["boot"], help="also report the functions reachable from nnMain")
    sl.add_argument("--slice-from", metavar="NAME|ADDR", help="also report the functions reachable from this one")
    args = ap.parse_args()

    rows = load()
    library = [r for r in rows if r["status"] == "L"]
    game = [r for r in rows if r["status"] != "L"]
    overall = tally(game)
    by_sub = defaultdict(list)
    for r in game:
        by_sub[r["subsystem"].rstrip("?")].append(r)
    subs = {k: tally(v) for k, v in by_sub.items()}

    print(f"main: {len(rows)} functions; {len(library)} library (L, excluded); "
          f"{overall['functions']} counted, {overall['bytes']:,} bytes")
    print_status(overall)
    print()
    print("| Subsystem | Functions | Bytes | O | m+M+W |")
    print("|---|---|---|---|---|")
    for name, t in sorted(subs.items(), key=lambda kv: -kv[1]["bytes"])[:args.top]:
        b = t["by_status"]
        print(f"| {name} | {t['functions']:,} | {t['bytes']:,} | {b['O']['functions']} | "
              f"{b['m']['functions'] + b['M']['functions'] + b['W']['functions']} |")

    report = {"overall": overall, "library_functions": len(library), "subsystems": subs}
    if args.slice or args.slice_from:
        s = slice_report(args.version, rows, args.slice == "boot", args.slice_from)
        report["slice"] = s
        label = "boot slice" if s["kind"] == "boot" else "slice"
        print()
        print(f"{label}: reachable from {s['root']} ({s['root_address']}) by direct calls: "
              f"{s['functions']} functions; {s['library_functions']} library (L, excluded); "
              f"{s['overall']['functions']} counted, {s['overall']['bytes']:,} bytes")
        print_status(s["overall"])
        print()
        print(s["caveat"])

    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
