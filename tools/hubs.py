#!/usr/bin/env python3
"""Hub functions: which `U` functions to match next, from the direct-call graph.

Ranks the not-decompiled (`U`) functions in data/functions/ by their distinct unmatched callers
(callers with status U, W or M): matching a hub lets every one of those callers call a real
function. The graph comes from tools/callgraph.py (the Ghidra export, direct calls only), used
in its collapsed form: a call through a thunk counts as a call of the function the thunk
finally branches to, so a function reached only through `O` thunks keeps its callers, and a
function that calls a `U` function through an `O` thunk is not "ready". A function's calls to
itself are ignored. Virtual callers are not in the graph and the output says so. Import stubs
(`<EXTERNAL>::` names: PLT entries and import placeholders, status U in the CSV but provided by
the SDK) are not candidates and count as matched callees; the title says how many were
skipped. Listings are plain aligned text, or with --json one object: `title`, `functions`
(the rows shown) and `caveat`, plus `function`, `callers_total` and `unmatched_total` in
--callers mode.

  default        hubs, columns: address, size, callers, unmatched callers, name, subsystem
  --ready        instead the `U` functions whose callees (thunks followed) are all O, m, L or
                 import stubs, largest first, with a callees column; leaf functions qualify
  --callers F    instead the callers of one function (name or hex address) with their
                 statuses, unmatched first; a thunk is resolved to its target first
  --boot         only the boot slice (reachable from nnMain): candidates, and the callers that
                 are counted or listed, must lie in the slice
  --min-size/--max-size   keep only listed functions of that many bytes (the candidates, or
                 with --callers the callers)
  --top N        rows to print in every mode (default 40; 0 prints them all)

Usage: python tools/hubs.py --version 2.1.0 [--top 40] [--boot] [--min-size B] [--max-size B] [--json]
       python tools/hubs.py --version 2.1.0 --ready [--boot] [--top 40]
       python tools/hubs.py --version 2.1.0 --callers 0x7100123456 [--boot] [--top 0]
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import callgraph  # noqa: E402

HUB_COLUMNS = [("address", "address", "<"), ("size", "size", ">"), ("callers", "callers", ">"),
               ("unmatched", "unmatched_callers", ">"), ("name", "name", "<"), ("subsystem", "subsystem", "<")]
READY_COLUMNS = [("address", "address", "<"), ("size", "size", ">"), ("callees", "callees", ">"),
                 ("callers", "callers", ">"), ("unmatched", "unmatched_callers", ">"), ("name", "name", "<"),
                 ("subsystem", "subsystem", "<")]
CALLER_COLUMNS = [("address", "address", "<"), ("size", "size", ">"), ("status", "status", "<"),
                  ("name", "name", "<"), ("subsystem", "subsystem", "<")]
MAX_NAME_PAD = 70  # longer names push the subsystem column right rather than padding every row


def done(row: dict) -> bool:
    """Matched, usable or library, or an import stub: nothing left to decompile here."""
    return row["status"] in callgraph.DONE or callgraph.is_import(row)


def record(g: callgraph.CallGraph, a: int, within: set[int] | None) -> dict:
    """One function's row: counts over its callers (within the slice, if any) and callees, both
    with thunks collapsed to their targets and self-calls dropped."""
    callees_of, callers_of = g.collapsed()
    r = g.funcs[a]
    callers = [c for c in callers_of[a] if c != a and (within is None or c in within)]
    callees = [c for c in callees_of[a] if c != a]
    return {"address": f"0x{a:x}", "size": r["size"], "status": r["status"],
            "callers": len(callers),
            "unmatched_callers": sum(1 for c in callers if not done(g.funcs[c])),
            "callees": len(callees),
            "ready": all(done(g.funcs[c]) for c in callees),
            "name": r["name"], "subsystem": r["subsystem"]}


def size_ok(size: int, min_size: int | None, max_size: int | None) -> bool:
    return (min_size is None or size >= min_size) and (max_size is None or size <= max_size)


def print_table(rows: list[dict], columns: list[tuple[str, str, str]]) -> None:
    widths = {}
    for head, key, _ in columns:
        w = max([len(head)] + [len(str(r[key])) for r in rows])
        widths[key] = min(w, MAX_NAME_PAD) if key == "name" else w
    line = []
    for head, key, align in columns:
        line.append(f"{head:{align}{widths[key]}}")
    print("  ".join(line).rstrip())
    for r in rows:
        cells = [f"{str(r[key]):{align}{widths[key]}}" for _, key, align in columns]
        print("  ".join(cells).rstrip())


def emit(title: str, rows: list[dict], columns: list[tuple[str, str, str]], as_json: bool,
         extra: dict | None = None) -> None:
    """Print the title, the table and the caveat, or the same as one JSON object."""
    if as_json:
        out = {"title": title, **(extra or {}), "functions": rows, "caveat": callgraph.CAVEAT}
        print(json.dumps(out, indent=1))
        return
    print(title)
    if rows:
        print_table(rows, columns)
    print(callgraph.CAVEAT)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--top", type=int, default=40, help="rows to print (default 40; 0 = all)")
    ap.add_argument("--boot", action="store_true", help="only functions reachable from nnMain")
    ap.add_argument("--min-size", type=int, metavar="B", help="skip functions smaller than B bytes")
    ap.add_argument("--max-size", type=int, metavar="B", help="skip functions larger than B bytes")
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--ready", action="store_true",
                      help="list U functions whose callees are all O, m, L or import stubs")
    mode.add_argument("--callers", metavar="NAME|ADDR", help="list the callers of this function with their statuses")
    ap.add_argument("--json", action="store_true", help="print JSON instead of text")
    args = ap.parse_args()
    if args.top < 0:
        ap.error("--top must be 0 or more")
    top = args.top or None

    g = callgraph.CallGraph(args.version)
    within = g.reachable(callgraph.NNMAIN) if args.boot else None
    size_note = "".join(f", {k} {v} bytes" for k, v in (("min", args.min_size), ("max", args.max_size))
                        if v is not None)

    if args.callers:
        asked = g.resolve(args.callers)
        root = g.target(asked)
        f = g.funcs[root]
        rows = [record(g, c, within) for c in g.collapsed()[1][root]
                if c != root and (within is None or c in within) and size_ok(g.funcs[c]["size"], args.min_size, args.max_size)]
        rows.sort(key=lambda r: (done(g.funcs[int(r["address"], 16)]), r["address"]))
        unmatched = sum(1 for r in rows if not done(g.funcs[int(r["address"], 16)]))
        shown = rows[:top]
        scope = ""
        if args.boot:
            scope = " in the boot slice (reachable from nnMain)"
            if root not in within:
                scope += ", which does not contain it"
        via = f" (thunk {g.name(asked)} 0x{asked:x} resolved to its target)" if asked != root else ""
        title = (f"callers of {f['name']} (0x{root:x}; {f['size']} bytes, status {f['status']}, {f['subsystem']})"
                 f"{via}{scope}{size_note}: {len(rows)}, {unmatched} unmatched; first {len(shown)}")
        emit(title, shown, CALLER_COLUMNS, args.json,
             {"function": record(g, root, within), "callers_total": len(rows), "unmatched_total": unmatched})
        return 0

    rows = []
    imports = 0
    for a, r in g.funcs.items():
        if r["status"] != "U" or (within is not None and a not in within):
            continue
        if callgraph.is_import(r):
            imports += 1
            continue
        if size_ok(r["size"], args.min_size, args.max_size):
            rows.append(record(g, a, within))
    scope = " in the boot slice (reachable from nnMain; callers counted within it)" if args.boot else ""

    if args.ready:
        rows = [r for r in rows if r["ready"]]
        rows.sort(key=lambda r: (-r["size"], r["address"]))
        shown = rows[:top]
        title = (f"ready U functions{scope}{size_note}: {len(rows)} ({sum(r['size'] for r in rows):,} bytes; "
                 f"{sum(1 for r in rows if r['callees'] == 0)} leaves) whose callees are all O, m, L or import "
                 f"stubs, {imports} import stubs skipped; largest {len(shown)}")
        emit(title, shown, READY_COLUMNS, args.json)
    else:
        rows.sort(key=lambda r: (-r["unmatched_callers"], -r["callers"], -r["size"], r["address"]))
        shown = rows[:top]
        title = (f"hubs{scope}{size_note}: {len(rows)} U functions ranked by distinct unmatched callers "
                 f"(status U, W or M), {imports} import stubs skipped; top {len(shown)}")
        emit(title, shown, HUB_COLUMNS, args.json)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
