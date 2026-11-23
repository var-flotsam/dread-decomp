#!/usr/bin/env python3
"""Daily progress from main's history: matched, clean in form, untouched and drafts, one row per day.

Nothing is stored or committed: each row is read with `git show` from the last first-parent commit of
that day on --ref (the committer's own date), so the record costs main no commit. A first row, `before` (or
`start` where history begins), is the day before, for the first day's change. Per day:

  matched      `O` bytes and their share of code (L excluded, as tools/progress.py counts)
  clean        matching and clean in form (tools/progress.py --quality, the project's headline since
               2026-11-23; before then the line said "byte-matching and readable", read the same way), read
               from the quality line under docs/HANDOFF.md's table as of that commit; the "as of" column
               says when that line was last regenerated, since only a run with check.py's cache writes it.
               The CSV keeps the column names finished_pct and finished_functions for its readers
  untouched    `U` bytes
  drafts       rows of drafts/INDEX.csv

Each day also shows the change from the day before. With --now, a last row "now" is added from the
working tree; when build/check/<version>/ is present its clean-in-form figure is computed fresh
(tools/progress.py's quality_report and names_report), otherwise it is HANDOFF's.

Usage: python tools/daily.py [--ref origin/main] [--days 14] [--now] [--csv FILE]
"""

from __future__ import annotations

import argparse
import csv
import io
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
QUALITY = re.compile(r"(?:matching and clean in form|byte-matching and readable) \*\*([0-9.]+)%\*\* "
                     r"\(([0-9,]+) functions[;)]")
FIELDS = ["day", "commit", "matched_bytes", "matched_pct", "matched_functions", "finished_pct",
          "finished_functions", "finished_as_of", "untouched_bytes", "drafts"]


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, check=True,
                          encoding="utf-8").stdout


def show(ref: str, path: str) -> str:
    try:
        return git("show", f"{ref}:{path}")
    except subprocess.CalledProcessError:
        return ""


def status_totals(rows) -> dict:
    t: dict[str, list[int]] = {}
    for r in rows:
        if r["status"] == "L":
            continue
        v = t.setdefault(r["status"], [0, 0])
        v[0] += 1
        v[1] += int(r["size"])
    total = sum(b for _, b in t.values())
    o = t.get("O", [0, 0])
    return {"matched_bytes": o[1], "matched_functions": o[0], "matched_pct": 100 * o[1] / total if total else 0.0,
            "untouched_bytes": t.get("U", [0, 0])[1], "total_bytes": total}


def finished_from_handoff(text: str) -> tuple[float | None, int | None]:
    m = QUALITY.search(text)
    return (float(m.group(1)), int(m.group(2).replace(",", ""))) if m else (None, None)


def drafts_count(text: str) -> int:
    return max(0, sum(1 for ln in text.splitlines() if ln.strip()) - 1)


def row_at(ref: str, day: str) -> dict:
    rows = []
    for name in git("ls-tree", "--name-only", ref, "data/functions/").split():
        if name.endswith(".csv"):
            rows.extend(csv.DictReader(io.StringIO(show(ref, name))))
    pct, n = finished_from_handoff(show(ref, "docs/HANDOFF.md"))
    as_of = git("log", "-1", "--format=%cI", ref, "--", "docs/HANDOFF.md").strip()[:16].replace("T", " ")
    return {"day": day, "commit": git("rev-parse", "--short=8", ref).strip(), **status_totals(rows),
            "finished_pct": pct, "finished_functions": n, "finished_as_of": as_of,
            "drafts": drafts_count(show(ref, "drafts/INDEX.csv"))}


def history(ref: str, days: int) -> list[dict]:
    last: dict[str, str] = {}  # day -> last first-parent commit that day (log is newest first)
    for line in git("log", "--first-parent", "--format=%H %cs", ref).splitlines():
        h, d = line.split()
        last.setdefault(d, h)
    keep = sorted(last)[-days:]
    out = [row_at(last[d], d) for d in keep]
    # The day before the first row, for its change; history's own start when it begins that day.
    before = [h for h, d in (ln.split() for ln in git("log", "--first-parent", "--format=%H %cs", ref).splitlines())
              if d < keep[0]] if keep else []
    if before:
        out.insert(0, row_at(before[0], "before"))
    elif keep:
        first = git("rev-list", "--first-parent", "--max-parents=0", ref).split()[0]
        if first != last[keep[0]]:
            out.insert(0, row_at(first, "start"))
    return out


def now_row(version: str) -> dict:
    from dread import functions
    rows = functions.rows()
    r = {"day": "now", "commit": git("rev-parse", "--short=8", "HEAD").strip(), **status_totals(rows),
         "drafts": drafts_count((ROOT / "drafts" / "INDEX.csv").read_text(encoding="utf-8"))}
    if (ROOT / "build" / "check" / version).is_dir():
        import progress
        progress.VERIFIED = progress.verified_addresses()
        progress.FUNCTIONAL = progress.functional_addresses()
        counted = [x for x in rows if x["status"] != "L"]
        qa = progress.quality_by_address(version, counted)
        h = progress.headline(progress.quality_report(version, counted, qa=qa),
                              progress.names_report(version, counted, qa))
        r.update(finished_pct=100 * h["bytes"] / r["total_bytes"],
                 finished_functions=h["functions"], finished_as_of="computed now")
    else:
        pct, n = finished_from_handoff((ROOT / "docs" / "HANDOFF.md").read_text(encoding="utf-8"))
        r.update(finished_pct=pct, finished_functions=n, finished_as_of="HANDOFF.md")
    return r


def delta(cur, prev, fmt) -> str:
    return "" if cur is None or prev is None else f" ({fmt(cur - prev)})"


def table(rows: list[dict]) -> str:
    out = ["| Day | Matched | Clean in form | Untouched | Drafts |", "|---|---|---|---|---|"]
    p: dict = {}
    for r in rows:
        fin = "n/a" if r["finished_pct"] is None else f"{r['finished_pct']:.2f}%"
        out.append(f"| {r['day']} "
                   f"| {r['matched_pct']:.2f}%{delta(r['matched_bytes'], p.get('matched_bytes'), lambda v: f'{v / 1e3:+,.0f} KB')} "
                   f"| {fin}{delta(r['finished_pct'], p.get('finished_pct'), lambda v: f'{v:+.2f} pt')} "
                   f"| {r['untouched_bytes'] / 1e6:.2f} MB "
                   f"| {r['drafts']:,}{delta(r['drafts'], p.get('drafts'), lambda v: f'{v:+,}')} |")
        p = r
    return "\n".join(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default="origin/main", help="branch whose first-parent history is read (default origin/main)")
    ap.add_argument("--days", type=int, default=14, help="days to show (default 14)")
    ap.add_argument("--now", action="store_true", help="add a row for the working tree")
    ap.add_argument("--version", default="2.1.0", help="version of check.py's cache for --now (default 2.1.0)")
    ap.add_argument("--csv", type=Path, help="also write the rows as CSV")
    args = ap.parse_args()
    rows = history(args.ref, args.days)
    if args.now:
        rows.append(now_row(args.version))
    print(table(rows))
    hist = [r for r in rows if r["day"] != "now"]
    if hist and rows[-1]["finished_as_of"] != "computed now":
        print(f"\nClean in form is read from docs/HANDOFF.md, last regenerated {hist[-1]['finished_as_of']}; "
              "a day whose HANDOFF was not regenerated repeats the previous figure.")
    if args.csv:
        with args.csv.open("w", newline="", encoding="utf-8") as fh:
            w = csv.DictWriter(fh, fieldnames=FIELDS, extrasaction="ignore", lineterminator="\n")
            w.writeheader()
            w.writerows(rows)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
