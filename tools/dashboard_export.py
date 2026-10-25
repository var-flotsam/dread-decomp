#!/usr/bin/env python3
"""Status tables for the project dashboard, written to build/dashboard/ (counts only, no code).

Four CSV files, each one dataset of the dashboard:

  function_status.csv   subsystem,status,in_boot,functions,bytes: data/functions/main_*.csv summed
                        per subsystem ('?' folded in, like tools/progress.py) and status, split by
                        whether the boot slice (tools/callgraph.py, reachable from nnMain by direct
                        calls) reaches the function. Library (L) rows are kept; the page leaves
                        them out. Ghidra's import stubs are dropped.
  progress_history.csv  day,commit,status,functions,bytes: the same counts, L and import stubs
                        left out, at the last first-parent commit of each day on --ref that
                        changed data/functions/ (read with `git show`, so the tree is untouched).
  phases.csv            order,phase,state,detail: the "Where things stand" table of docs/HANDOFF.md.
  workstreams.csv       stream,area,branches: the "Workstreams" table of docs/HANDOFF.md.

The snapshot (function_status, phases, workstreams) comes from the working tree, so run it after
merging main and `tools/check.py --update`. The boot slice needs the Ghidra export
(build/ghidra_export/<version>/). build/dashboard/manifest.json records the commit and time.
Refreshing the dashboard is then an upload of these files to it (ask Claude to do it).

Usage: python tools/dashboard_export.py [--version 2.1.0] [--ref HEAD] [--days 30] [--out build/dashboard]
"""

from __future__ import annotations

import argparse
import csv
import io
import json
import re
import subprocess
from collections import defaultdict
from datetime import datetime, timezone
from pathlib import Path

import callgraph

ROOT = Path(__file__).resolve().parent.parent


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, check=True,
                          encoding="utf-8").stdout


def rows_at(ref: str) -> list[dict]:
    """data/functions/main_*.csv rows as committed at `ref`."""
    rows: list[dict] = []
    for name in git("ls-tree", "--name-only", ref, "data/functions/").split():
        if name.endswith(".csv"):
            rows.extend(csv.DictReader(io.StringIO(git("show", f"{ref}:{name}"))))
    return rows


def write_csv(path: Path, header: list[str], rows) -> None:
    with path.open("w", newline="", encoding="utf-8") as fh:
        w = csv.writer(fh)
        w.writerow(header)
        w.writerows(rows)


def function_status(rows: list[dict], boot: set[int]) -> list[list]:
    agg: dict[tuple, list[int]] = defaultdict(lambda: [0, 0])
    for r in rows:
        if callgraph.is_import(r):
            continue
        k = (r["subsystem"].rstrip("?") or "(none)", r["status"],
             "yes" if int(r["address"], 16) in boot else "no")
        agg[k][0] += 1
        agg[k][1] += int(r["size"])
    return [[*k, n, b] for k, (n, b) in sorted(agg.items())]


def history(ref: str, days: int) -> list[list]:
    last: dict[str, str] = {}  # day -> last commit that day (log is newest first)
    for line in git("log", "--first-parent", "--format=%H %cs", ref, "--", "data/functions").splitlines():
        h, d = line.split()
        last.setdefault(d, h)
    out = []
    for d in sorted(last)[-days:]:
        t: dict[str, list[int]] = defaultdict(lambda: [0, 0])
        for r in rows_at(last[d]):
            if r["status"] == "L" or callgraph.is_import(r):
                continue
            t[r["status"]][0] += 1
            t[r["status"]][1] += int(r["size"])
        out += [[d, last[d][:8], s, n, b] for s, (n, b) in sorted(t.items())]
    return out


def plain(s: str) -> str:
    return re.sub(r"\*\*|`", "", s).strip()


def handoff_table(md: str, heading: str) -> list[list[str]]:
    """Body rows of the first markdown table after `heading`, cells as written (markdown kept)."""
    block: list[str] = []
    for line in md.split(heading, 1)[1].splitlines()[1:]:
        if line.startswith("|"):
            block.append(line)
        elif block:
            break
    if len(block) < 3:
        raise SystemExit(f"docs/HANDOFF.md: no table under {heading!r}")
    return [[c.strip() for c in line.strip("|").split("|")] for line in block[2:]]


def phases(md: str) -> list[list]:
    out = []
    for i, (phase, state) in enumerate(handoff_table(md, "## Where things stand"), 1):
        # the state is the leading bold run: "**Done.** Dump imported ...", "**M0 done** (2026-10-24 ...): ..."
        m = re.match(r"\*\*(.+?)\*\*\s*(.*)", state)
        st, detail = (m.group(1).rstrip("."), m.group(2)) if m else (state, "")
        out.append([i, plain(phase), plain(st), plain(detail)])
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", default="2.1.0", help="dump version whose Ghidra export the boot slice uses")
    ap.add_argument("--ref", default="HEAD", help="branch whose first-parent history is read (default HEAD)")
    ap.add_argument("--days", type=int, default=30, help="days of history to keep (default 30)")
    ap.add_argument("--out", type=Path, default=ROOT / "build" / "dashboard")
    args = ap.parse_args()

    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows.extend(csv.DictReader(fh))
    graph = callgraph.CallGraph(args.version, rows={int(r["address"], 16): r for r in rows})
    boot = graph.reachable(callgraph.NNMAIN)
    md = (ROOT / "docs" / "HANDOFF.md").read_text(encoding="utf-8")

    args.out.mkdir(parents=True, exist_ok=True)
    write_csv(args.out / "function_status.csv", ["subsystem", "status", "in_boot", "functions", "bytes"],
              function_status(rows, boot))
    write_csv(args.out / "progress_history.csv", ["day", "commit", "status", "functions", "bytes"],
              history(args.ref, args.days))
    write_csv(args.out / "phases.csv", ["order", "phase", "state", "detail"], phases(md))
    write_csv(args.out / "workstreams.csv", ["stream", "area", "branches"],
              [[plain(c[0]), plain(c[1]), plain(c[3])] for c in handoff_table(md, "## Workstreams")])

    head, at = git("log", "-1", "--format=%h %cI", "HEAD").split()
    dirty = bool(git("status", "--porcelain", "--", "data/functions", "docs/HANDOFF.md").strip())
    manifest = {"commit": head, "committed_at": at, "uncommitted_changes": dirty,
                "exported_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
                "history_ref": args.ref, "functions": len(rows), "boot_slice": len(boot)}
    (args.out / "manifest.json").write_text(json.dumps(manifest, indent=1), encoding="utf-8")
    print(f"{args.out.relative_to(ROOT).as_posix() if args.out.is_relative_to(ROOT) else args.out}: "
          f"{len(rows)} functions, boot slice {len(boot)}, at {head}{' (+ uncommitted)' if dirty else ''}")
    print(callgraph.CAVEAT)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
