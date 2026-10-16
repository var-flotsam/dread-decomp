#!/usr/bin/env python3
"""Whole-binary decompilation progress, from the status column of data/functions/.

Library functions (status L: nn::atk, SDK, runtime, Lua standard library) are excluded
from the totals, like BotW's "library function" status. Reports function counts and
code bytes by status, overall and per subsystem (inferred '?' subsystems are folded in).

objdiff's report (tools/objdiff_gen.py, then `objdiff-cli report generate`) covers only
the units in src/; this report covers the whole of main.

Usage: python tools/progress.py [--json build/progress.json] [--top 15]
"""

from __future__ import annotations

import argparse
import csv
import json
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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--json", type=Path, help="also write the report as JSON")
    ap.add_argument("--top", type=int, default=15)
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
    print(f"matching (O): {overall['by_status']['O']['functions']} functions, "
          f"{overall['matching_bytes_pct']:.4f}% of code; usable (O+m): {overall['usable_bytes_pct']:.4f}%")
    print()
    print("| Status | Functions | Bytes |")
    print("|---|---|---|")
    for s in STATUSES:
        v = overall["by_status"][s]
        print(f"| `{s}` {LABELS[s]} | {v['functions']:,} | {v['bytes']:,} |")
    print()
    print("| Subsystem | Functions | Bytes | O | m+M+W |")
    print("|---|---|---|---|---|")
    for name, t in sorted(subs.items(), key=lambda kv: -kv[1]["bytes"])[:args.top]:
        b = t["by_status"]
        print(f"| {name} | {t['functions']:,} | {t['bytes']:,} | {b['O']['functions']} | "
              f"{b['m']['functions'] + b['M']['functions'] + b['W']['functions']} |")

    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps({"overall": overall, "library_functions": len(library),
                                         "subsystems": subs}, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
