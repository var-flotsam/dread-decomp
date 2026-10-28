#!/usr/bin/env python3
"""Status tables for the project dashboard, written to build/dashboard/ (counts only, no code).

One CSV file per dataset of the dashboard, which follows the project's two goals, the matching
decompilation and the PC port:

  function_status.csv   subsystem,status,in_boot,functions,bytes: data/functions/main_*.csv summed
                        per subsystem ('?' folded in, like tools/progress.py) and status, split by
                        whether the boot slice (tools/callgraph.py, reachable from nnMain by direct
                        calls) reaches the function. Library (L) rows are kept; the page leaves
                        them out. Ghidra's import stubs are dropped.
  progress_history.csv  day,commit,status,functions,bytes: the same counts, L and import stubs
                        left out, at the last first-parent commit of each day on --ref that
                        changed data/functions/ (read with `git show`, so the tree is untouched).
  areas.csv             order,goal,area,state,detail: the "Where things stand" table of docs/HANDOFF.md,
                        each row given a goal (decomp or port, from the area's name: tools/dread/handoff.py).
  workstreams.csv       stream,files,branches,next: the "Workstreams" table of docs/HANDOFF.md.
  lifts.csv             slice,outcome,kind,functions,bytes: tools/port_lift.py's outcomes summed, from
                        build/port/lifted/manifest.json (slice boot) and build/port/lifted_all/manifest.json
                        (slice all); kind is accepted, inconclusive, rejected or skipped. A missing
                        manifest leaves its slice out.
  boot_milestones.csv   order,key,label,reached,seconds: tools/boot_distance.py's milestones from
                        build/boot/boot_distance.json (the nightly run's output); left out when missing.
  boot_run.csv          furthest,reached,milestones,traps,trap_functions,trap_imports,trap_mismatched,
                        lifts_ran,lifts_linked,ended: the same run in one row.

The snapshot (function_status, areas, workstreams) comes from the working tree, so run it after
merging main and `tools/check.py --update`. The boot slice needs the Ghidra export
(build/ghidra_export/<version>/). build/dashboard/manifest.json records the commit and time, and each
optional file's source and time. Refreshing the dashboard is then an upload of these files to it (ask
Claude to do it).

Usage: python tools/dashboard_export.py [--version 2.1.0] [--ref HEAD] [--days 30] [--out build/dashboard]
       [--lifted build/port/lifted] [--lifted-all build/port/lifted_all] [--boot build/boot]
"""

from __future__ import annotations

import argparse
import csv
import io
import json
import subprocess
from collections import defaultdict
from datetime import datetime, timezone
from pathlib import Path

import callgraph
from dread import functions, handoff

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


def lift_kind(outcome: str) -> str:
    for kind in ("accepted", "inconclusive", "skipped"):
        if outcome.startswith(kind):
            return kind
    return "rejected"


def lifts(manifests: dict[str, dict]) -> list[list]:
    """Outcomes per slice from tools/port_lift.py's manifest.json files, summed."""
    out = []
    for sl, man in manifests.items():
        agg: dict[str, list[int]] = defaultdict(lambda: [0, 0])
        for f in man["functions"].values():
            agg[f["outcome"]][0] += 1
            agg[f["outcome"]][1] += int(f["size"])
        out += [[sl, o, lift_kind(o), n, b] for o, (n, b) in sorted(agg.items(), key=lambda kv: -kv[1][1])]
    return out


def boot(result: dict) -> tuple[list[list], list]:
    """tools/boot_distance.py's result: one row per milestone, and the run in one row."""
    ms = [[i, m["key"], m["label"], "yes" if m["reached"] else "no",
           "" if m["seconds"] is None else round(m["seconds"], 1)]
          for i, m in enumerate(result["milestones"], 1)]
    end = result.get("end") or {}
    ended = ("time-out" if end.get("timeout") else f"signal {end['signal']}" if end.get("signal")
             else f"exit {end['exit']}" if "exit" in end else "")
    if result.get("fault"):
        ended += f", {result['fault']['signal']}"
    tr, li = result["traps"], result["lifted"]
    run = [result.get("furthest_label") or "", sum(m["reached"] for m in result["milestones"]),
           len(result["milestones"]), tr["distinct"], tr["functions"], tr["imports"], tr["mismatched"],
           li.get("ran", ""), "" if li.get("linked") is None else li["linked"], ended]
    return ms, run


def read_json(path: Path) -> dict | None:
    return json.loads(path.read_text(encoding="utf-8")) if path.exists() else None


def mtime(path: Path) -> str:
    return datetime.fromtimestamp(path.stat().st_mtime, timezone.utc).isoformat(timespec="seconds")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", default="2.1.0", help="dump version whose Ghidra export the boot slice uses")
    ap.add_argument("--ref", default="HEAD", help="branch whose first-parent history is read (default HEAD)")
    ap.add_argument("--days", type=int, default=30, help="days of history to keep (default 30)")
    ap.add_argument("--lifted", type=Path, default=ROOT / "build" / "port" / "lifted",
                    help="tools/port_lift.py output for the boot slice (default build/port/lifted)")
    ap.add_argument("--lifted-all", type=Path, default=ROOT / "build" / "port" / "lifted_all",
                    help="tools/port_lift.py --slice all output (default build/port/lifted_all)")
    ap.add_argument("--boot", type=Path, default=ROOT / "build" / "boot",
                    help="tools/boot_distance.py output (default build/boot)")
    ap.add_argument("--out", type=Path, default=ROOT / "build" / "dashboard")
    args = ap.parse_args()

    rows = functions.rows()
    graph = callgraph.CallGraph(args.version, rows={int(r["address"], 16): r for r in rows})
    boot_set = graph.reachable(callgraph.NNMAIN)
    md = handoff.text()

    args.out.mkdir(parents=True, exist_ok=True)
    write_csv(args.out / "function_status.csv", ["subsystem", "status", "in_boot", "functions", "bytes"],
              function_status(rows, boot_set))
    write_csv(args.out / "progress_history.csv", ["day", "commit", "status", "functions", "bytes"],
              history(args.ref, args.days))
    cols = ["order", "goal", "area", "state", "detail"]
    write_csv(args.out / "areas.csv", cols, [[a[c] for c in cols] for a in handoff.areas(md)])
    cols = ["stream", "files", "branches", "next"]
    write_csv(args.out / "workstreams.csv", cols, [[w[c] for c in cols] for w in handoff.workstreams(md)])

    sources: dict[str, dict] = {}
    manifests = {}
    for sl, d in (("boot", args.lifted), ("all", args.lifted_all)):
        man = read_json(d / "manifest.json")
        if man is None:
            print(f"lifts: no {d / 'manifest.json'}; slice {sl} left out")
            continue
        manifests[sl] = man
        sources[f"lifts_{sl}"] = {"path": str(d / "manifest.json"), "modified": mtime(d / "manifest.json")}
    if manifests:
        write_csv(args.out / "lifts.csv", ["slice", "outcome", "kind", "functions", "bytes"], lifts(manifests))
    result = read_json(args.boot / "boot_distance.json")
    if result is None:
        print(f"boot: no {args.boot / 'boot_distance.json'}; boot_milestones.csv and boot_run.csv left out")
    else:
        ms, run = boot(result)
        write_csv(args.out / "boot_milestones.csv", ["order", "key", "label", "reached", "seconds"], ms)
        write_csv(args.out / "boot_run.csv", ["furthest", "reached", "milestones", "traps", "trap_functions",
                                              "trap_imports", "trap_mismatched", "lifts_ran", "lifts_linked",
                                              "ended"], [run])
        sources["boot"] = {"path": str(args.boot / "boot_distance.json"),
                           "modified": mtime(args.boot / "boot_distance.json")}

    head, at = git("log", "-1", "--format=%h %cI", "HEAD").split()
    dirty = bool(git("status", "--porcelain", "--", "data/functions", "docs/HANDOFF.md").strip())
    manifest = {"commit": head, "committed_at": at, "uncommitted_changes": dirty,
                "exported_at": datetime.now(timezone.utc).isoformat(timespec="seconds"),
                "history_ref": args.ref, "functions": len(rows), "boot_slice": len(boot_set), "sources": sources}
    (args.out / "manifest.json").write_text(json.dumps(manifest, indent=1), encoding="utf-8")
    print(f"{args.out.relative_to(ROOT).as_posix() if args.out.is_relative_to(ROOT) else args.out}: "
          f"{len(rows)} functions, boot slice {len(boot_set)}, at {head}{' (+ uncommitted)' if dirty else ''}")
    print(callgraph.CAVEAT)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
