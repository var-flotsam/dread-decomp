#!/usr/bin/env python3
"""Fail a change that turns a matched function (status O) back into anything else.

tools/check.py --update derives each function's status from the pull request's own src/, so a branch
committed from a stale tree passes it: its sources and its data/functions/ agree with each other, and
both have lost what other pull requests matched (#482 reverted #477 that way, 2026-11-15). This compares
data/functions/ with the base branch instead. Every function that is O there must still be O here.

On a pull request, GitHub checks out the merge of the branch into its base, so the working tree is what
main would become and an out-of-date branch is not blamed for matches it simply has not merged yet.

The rare intended demotion (a function moved back to drafts/, say) is allowed by a row in
data/demotions.csv (address,name,reason) that the same change adds, so it shows in the diff for review.
A row already in the base allows nothing: each demotion needs its own row.

Usage: python tools/lost_matches.py --base origin/main               # working tree vs the base (CI)
       python tools/lost_matches.py --base REF --head REF            # two commits, e.g. a past merge
"""

from __future__ import annotations

import argparse
import csv
import io
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))

from dread import functions  # noqa: E402

DEMOTIONS = "data/demotions.csv"


def _git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, check=True, encoding="utf-8").stdout


def _show(ref: str, path: str) -> str | None:
    try:
        return _git("show", f"{ref}:{path}")
    except subprocess.CalledProcessError:
        return None


def statuses_at(ref: str | None) -> dict[int, tuple[str, str]]:
    """Address -> (name, status) from data/functions/ at `ref`, or in the working tree when None."""
    rows: list[dict] = []
    if ref is None:
        rows = functions.rows()
    else:
        for name in _git("ls-tree", "--name-only", ref, "data/functions/").split():
            if name.endswith(".csv"):
                rows.extend(csv.DictReader(io.StringIO(_git("show", f"{ref}:{name}"))))
    return {int(r["address"], 16): (r["name"], r["status"]) for r in rows}


def demotions_at(ref: str | None) -> set[int]:
    """Addresses listed in data/demotions.csv at `ref` (working tree when None); empty if it is absent."""
    if ref is None:
        p = ROOT / DEMOTIONS
        text = p.read_text(encoding="utf-8") if p.exists() else None
    else:
        text = _show(ref, DEMOTIONS)
    if not text:
        return set()
    return {int(r["address"], 16) for r in csv.DictReader(io.StringIO(text)) if r.get("address", "").strip()}


def lost(base: dict[int, tuple[str, str]], head: dict[int, tuple[str, str]]) -> list[tuple[int, str, str]]:
    """(address, name, new status) for each function O in `base` and not O in `head`; "missing" if gone."""
    out = []
    for addr, (name, status) in sorted(base.items()):
        if status != "O":
            continue
        now = head.get(addr)
        if now is None:
            out.append((addr, name, "missing"))
        elif now[1] != "O":
            out.append((addr, name, now[1]))
    return out


def judge(base: dict, head: dict, base_rows: set[int], head_rows: set[int]) -> tuple[list, list]:
    """(allowed, refused) losses: a loss is allowed only by a data/demotions.csv row this change adds."""
    added = head_rows - base_rows
    found = lost(base, head)
    return [f for f in found if f[0] in added], [f for f in found if f[0] not in added]


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--base", required=True, help="the base branch or commit, e.g. origin/main")
    ap.add_argument("--head", help="a commit to check instead of the working tree")
    args = ap.parse_args(argv)

    ok, bad = judge(statuses_at(args.base), statuses_at(args.head), demotions_at(args.base), demotions_at(args.head))
    for addr, name, now in ok:
        print(f"allowed: 0x{addr:x} {name} O -> {now} (listed in {DEMOTIONS} by this change)")
    for addr, name, now in bad:
        print(f"LOST MATCH: 0x{addr:x} {name} O -> {now}")
    if bad:
        print(f"\n{len(bad)} function(s) matched on {args.base} are no longer matched. If the branch was "
              f"committed from a stale tree, merge {args.base} and restore them. If a demotion is intended, "
              f"add a row for it to {DEMOTIONS} (address,name,reason) in this change.")
        return 1
    print(f"no matched function lost against {args.base} ({len(ok)} allowed demotion(s))")
    return 0


if __name__ == "__main__":
    sys.exit(main())
