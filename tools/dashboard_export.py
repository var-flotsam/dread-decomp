#!/usr/bin/env python3
"""Tables for the private progress dashboard, written to build/dashboard/ (counts only, no code).

The dashboard follows the README's card: matched and finished first, then where the untouched code is,
then the port. One CSV per dataset:

  daily.csv        day,matched_pct,finished_pct,matched_functions,untouched_mb,drafts: one row per day on
                   --ref (tools/daily.py's record, read with `git show`, so the tree is untouched). With
                   --now, today's row is the working tree's, its finished share computed fresh when
                   check.py's cache (build/check/<version>/) is present.
  subsystems.csv   subsystem,functions,code_mb,matched_pct,untouched_mb: data/functions/main_*.csv summed
                   per subsystem ('?' folded in, library rows left out), most untouched first.
  port.csv         order,area,state: the port rows of docs/HANDOFF.md's "Where things stand".

subsystems and port come from the working tree, so run it after merging main (and `tools/check.py
--update`). A shallow clone has only the days it fetched. Refreshing the dashboard is then an upload of
these three files to it (ask Claude to do it).

Usage: python tools/dashboard_export.py [--ref origin/main] [--days 60] [--now] [--version 2.1.0]
       [--out build/dashboard]
"""

from __future__ import annotations

import argparse
import csv
import re
import sys
from collections import defaultdict
from datetime import date
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import daily  # noqa: E402
from dread import functions, handoff  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent


def write_csv(path: Path, header: list[str], rows) -> None:
    with path.open("w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh, lineterminator="\n")
        w.writerow(header)
        w.writerows(rows)


def daily_rows(rows: list[dict]) -> list[list]:
    """One row per day; the before/start row and days with no game code yet are left out."""
    out = []
    for r in rows:
        if r["day"] in ("before", "start") or not r["total_bytes"]:
            continue
        fin = r["finished_pct"]
        out.append([r["day"], round(r["matched_pct"], 2), "" if fin is None else round(fin, 2),
                    r["matched_functions"], round(r["untouched_bytes"] / 1e6, 3), r["drafts"]])
    return out


def subsystems(rows: list[dict]) -> list[list]:
    agg: dict[str, list[int]] = defaultdict(lambda: [0, 0, 0, 0])  # functions, bytes, O bytes, U bytes
    for r in rows:
        if r["status"] == "L":
            continue
        a, size = agg[r["subsystem"].rstrip("?") or "unknown"], int(r["size"])
        a[0] += 1
        a[1] += size
        a[2] += size if r["status"] == "O" else 0
        a[3] += size if r["status"] == "U" else 0
    return [[s, f, round(b / 1e6, 3), round(100 * o / b, 2) if b else 0.0, round(u / 1e6, 3)]
            for s, (f, b, o, u) in sorted(agg.items(), key=lambda kv: (-kv[1][3], kv[0]))]


def port(md: str) -> list[list]:
    return [[a["order"], re.sub(r" \([^)]*/[^)]*\)", "", a["area"]), a["state"]]
            for a in handoff.areas(md) if a["goal"] == "port"]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default="origin/main", help="branch whose first-parent history is read (default origin/main)")
    ap.add_argument("--days", type=int, default=60, help="days of history to keep (default 60)")
    ap.add_argument("--now", action="store_true", help="make today's row the working tree's")
    ap.add_argument("--version", default="2.1.0", help="version of check.py's cache for --now (default 2.1.0)")
    ap.add_argument("--out", type=Path, default=ROOT / "build" / "dashboard")
    args = ap.parse_args()

    hist = daily.history(args.ref, args.days)
    if args.now:
        now = daily.now_row(args.version)
        today = date.today().isoformat()
        hist = [r for r in hist if r["day"] != today] + [{**now, "day": today}]
    args.out.mkdir(parents=True, exist_ok=True)
    write_csv(args.out / "daily.csv", ["day", "matched_pct", "finished_pct", "matched_functions", "untouched_mb",
                                       "drafts"], daily_rows(hist))
    write_csv(args.out / "subsystems.csv", ["subsystem", "functions", "code_mb", "matched_pct", "untouched_mb"],
              subsystems(functions.rows()))
    write_csv(args.out / "port.csv", ["order", "area", "state"], port(handoff.text()))
    print(f"wrote daily.csv, subsystems.csv and port.csv to {args.out}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
