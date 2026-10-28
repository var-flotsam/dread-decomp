#!/usr/bin/env python3
"""Call graph of main, shared by tools/progress.py (--slice) and tools/hubs.py.

Joins the function list data/functions/main_*.csv (address,size,name,name_source,subsystem,
status) with the Ghidra export build/ghidra_export/<version>/main_functions.jsonl (one object
per function: `entry`, `calls` = direct callee addresses, `thunk_of`) into callees and callers
maps keyed by int address. A thunk (a bare `b` to another function) counts as a call of its
target. Only addresses in the function list are kept (so the junk thunk targets Ghidra invents
for import placeholders drop out).

`collapsed()` is the same graph with thunks bypassed: a call to a thunk is a call to the
function it finally branches to, and the thunk's callers become that function's callers. Most
thunks are already `O` (a 4-byte `b` matches whatever its target's status is), so without the
collapse a function reached only through thunks looks caller-poor and a function calling it
through an `O` thunk looks finished. tools/hubs.py uses the collapsed view; reachability is the
same in both.

Direct calls only: virtual calls (through vtables) and calls through function pointers are not
in the export, so a reachable set is a lower bound and a caller count an undercount. Every tool
built on this module prints CAVEAT once.

Usage: python tools/callgraph.py --version 2.1.0 [--from nnMain|0x71000018f0]
  Without --from: node, edge and thunk counts. With --from: the size of the set reachable from
  that function (a name from the CSV, or a hex address).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread import functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
NNMAIN = 0x71000018F0
CAVEAT = ("Direct calls only: virtual calls and function pointers are missing from the graph, "
          "so reachable sets are lower bounds and caller counts undercounts.")
UNMATCHED = frozenset("UWM")  # statuses that still need work
DONE = frozenset("OmL")  # statuses a caller can rely on
# Ghidra names imports `<EXTERNAL>::name`: the 16-byte PLT stubs and the 1-byte placeholders they
# branch to. They are status U in the CSV but hold no game code (the SDK provides them).
IMPORT_PREFIX = "<EXTERNAL>::"
_HEX = re.compile(r"^(?:0x)?[0-9a-f]+$", re.I)


def is_import(row: dict) -> bool:
    """An import stub or placeholder rather than a function to decompile."""
    return row["name"].startswith(IMPORT_PREFIX)


def export_path(version: str) -> Path:
    return ROOT / "build" / "ghidra_export" / version / "main_functions.jsonl"


def load_rows() -> dict[int, dict]:
    """data/functions/main_*.csv rows keyed by int address; `size` is converted to int."""
    rows: dict[int, dict] = {}
    for r in functions.rows():
        r["size"] = int(r["size"])
        rows[int(r["address"], 16)] = r
    return rows


class CallGraph:
    """Direct-call graph over the functions in data/functions/.

    `rows` (int address -> CSV row) replaces load_rows(); it may hold raw csv.DictReader rows
    (progress.py passes its own), so `size` is converted to int here, in a copy of any row that
    needs it: `funcs[a]["size"]` is always an int and the caller's dicts are left alone."""

    def __init__(self, version: str, rows: dict[int, dict] | None = None):
        self.version = version
        if rows is None:
            self.funcs = load_rows()
        else:
            self.funcs = {a: r if isinstance(r["size"], int) else {**r, "size": int(r["size"])}
                          for a, r in rows.items()}
        path = export_path(version)
        if not path.is_file():
            raise SystemExit(
                f"{path.relative_to(ROOT).as_posix()} is missing: run "
                f"`.venv/Scripts/python tools/ghidra/analyze.py --version {version}` to produce it "
                f"(Ghidra import, analysis and export, about 8 minutes)")
        callees: dict[int, set[int]] = {a: set() for a in self.funcs}
        callers: dict[int, set[int]] = {a: set() for a in self.funcs}
        self.thunk_of: dict[int, int] = {}  # thunk -> the listed function it branches to
        with path.open(encoding="utf-8") as fh:
            for line in fh:
                o = json.loads(line)
                a = int(o["entry"], 16)
                if a not in callees:
                    continue
                targets = [int(c, 16) for c in o["calls"]]
                if o.get("thunk_of"):
                    t = int(o["thunk_of"], 16)
                    targets.append(t)
                    if t in callees:
                        self.thunk_of[a] = t
                for t in targets:
                    if t in callees:
                        callees[a].add(t)
                        callers[t].add(a)
        self.callees = {a: sorted(s) for a, s in callees.items()}
        self.callers = {a: sorted(s) for a, s in callers.items()}
        self.edges = sum(len(s) for s in self.callees.values())
        self._by_name: dict[str, list[int]] | None = None
        self._collapsed: tuple[dict[int, list[int]], dict[int, list[int]]] | None = None

    def target(self, a: int) -> int:
        """The function `a` finally branches to through thunks (`a` itself if it is not one)."""
        seen = set()
        while a in self.thunk_of and a not in seen:
            seen.add(a)
            a = self.thunk_of[a]
        return a

    def collapsed(self) -> tuple[dict[int, list[int]], dict[int, list[int]]]:
        """(callees, callers) with thunks bypassed. Every callee is replaced by its final target
        and a thunk is not recorded as a caller, so its callers are its target's callers and its
        own callee list is just that target. Computed once."""
        if self._collapsed is None:
            callees: dict[int, set[int]] = {a: set() for a in self.funcs}
            callers: dict[int, set[int]] = {a: set() for a in self.funcs}
            for a, cs in self.callees.items():
                for c in cs:
                    t = self.target(c)
                    callees[a].add(t)
                    if a not in self.thunk_of:
                        callers[t].add(a)
            self._collapsed = ({a: sorted(s) for a, s in callees.items()},
                               {a: sorted(s) for a, s in callers.items()})
        return self._collapsed

    def reachable(self, start: int) -> set[int]:
        """`start` and everything it reaches by direct calls (thunks followed)."""
        if start not in self.funcs:
            raise KeyError(f"0x{start:x} is not a function start in data/functions/")
        seen = {start}
        stack = [start]
        while stack:
            for t in self.callees[stack.pop()]:
                if t not in seen:
                    seen.add(t)
                    stack.append(t)
        return seen

    def resolve(self, spec: str) -> int:
        """A CSV name or a hex address (with or without 0x) -> int address."""
        if self._by_name is None:
            self._by_name = {}
            for a, r in self.funcs.items():
                self._by_name.setdefault(r["name"], []).append(a)
        hits = self._by_name.get(spec)
        if hits and len(hits) == 1:
            return hits[0]
        if hits:
            raise SystemExit(f"{spec!r} names {len(hits)} functions: "
                             + ", ".join(f"0x{a:x}" for a in hits[:8]) + ("..." if len(hits) > 8 else "")
                             + "; give an address instead")
        if _HEX.match(spec):
            a = int(spec, 16)
            if a in self.funcs:
                return a
            raise SystemExit(f"{spec!r} is not a function name in data/functions/, "
                             f"and 0x{a:x} is not a function start")
        raise SystemExit(f"{spec!r} is neither a function name in data/functions/ nor a hex address")

    def name(self, addr: int) -> str:
        return self.funcs[addr]["name"]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--from", dest="start", metavar="NAME|ADDR", help="report the set reachable from here")
    args = ap.parse_args()
    g = CallGraph(args.version)
    root = g.resolve(args.start) if args.start else None  # a bad --from fails before any output
    print(f"main {args.version}: {len(g.funcs)} functions, {g.edges} direct call edges "
          f"({sum(1 for s in g.callers.values() if s)} functions have a caller); "
          f"{len(g.thunk_of)} thunks to listed functions")
    if root is not None:
        reach = g.reachable(root)
        game = [g.funcs[a] for a in reach if g.funcs[a]["status"] != "L"]
        print(f"reachable from {g.name(root)} (0x{root:x}): {len(reach)} functions, "
              f"{len(game)} game (non-L), {sum(r['size'] for r in game):,} game bytes")
    print(CAVEAT)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
