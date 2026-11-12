#!/usr/bin/env python3
"""The week's sample of newly decompiled hand-written functions, for a person or an independent session to read.

The gates prove bytes, relocation targets and declarations. They cannot say whether a function reads as
source: whether its comment is true, its names say something, its structs are layouts and not offsets, and
nothing in it is a decompiler's text. Only reading does, and nobody reads every merge (the independent
review of 2026-11-11 asked for this: 20 random merged functions a week, read, with the findings recorded in
the journal). This picks the sample so that the reader does not: every hand-written function whose status
became O, m or M since a date, a seeded random 20 of them, each with where it is defined and the fake-match
forms tools/fakematch.py sees in it.

  python tools/sample_review.py                                   # the last 7 days of origin/main, 20 functions
  python tools/sample_review.py --since 2026-11-05 --count 20 --seed 2026-W46

Writes build/review/sample_<seed>.md, a worksheet: one entry per function and the questions to answer for
each. It records nothing itself: the reader's findings go at the top of docs/JOURNAL.md, with the seed, so
the sample can be drawn again. Needs tools/check.py's cache (build/check/<version>) to know which source
defines a function; no dump.
"""

from __future__ import annotations

import argparse
import csv
import datetime
import io
import json
import random
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from dread import functions  # noqa: E402
from progress import GENERATED_DIRS  # noqa: E402

DONE = ("O", "m", "M")
QUESTIONS = [
    "Does the opening comment say what the function does, and is it true?",
    "Are the names placeholders (`fn_<addr>`, `field_0x..`) or sourced (RTTI, reflection, strings)? Any invented real-looking name?",
    "Do structs read as layouts (named fields at offsets), not as casts and arithmetic?",
    "Is anything there only to steer the compiler (barrier, `volatile`, empty block, inlining attribute, union, untyped store)?",
    "Does any line read like decompiler output (`param_1`, `uVar3`, `DAT_`, `FUN_`, casts on every operand)?",
    "Does it declare a callee or global its own way, where a header or another file has it?",
]


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, check=True).stdout


def statuses_at(commit: str) -> dict[int, str]:
    """address -> status of the function list as it was at a commit."""
    out = {}
    for name in git("ls-tree", "--name-only", commit, "data/functions/").split():
        if name.endswith(".csv"):
            for row in csv.DictReader(io.StringIO(git("show", f"{commit}:{name}"))):
                out[int(row["address"], 16)] = row["status"]
    return out


def newly_done(before: dict[int, str], now: list[dict]) -> list[dict]:
    """The rows that are O, m or M now and were not at the earlier commit (U, or not in the list)."""
    return [r for r in now if r["status"] in DONE and before.get(int(r["address"], 16), "U") not in DONE]


def defining_sources(version: str) -> dict[int, str]:
    """address -> the hand-written source that defines it, from check.py's diff cache; a function only a
    generated file defines is not in it."""
    cache = ROOT / "build" / "check" / version
    if not cache.is_dir():
        raise SystemExit(f"{cache.relative_to(ROOT)}: no check cache; run tools/check.py --version {version} first")
    out: dict[int, str] = {}
    for p in sorted(cache.rglob("*.o.diff.json")):
        rel = p.relative_to(cache)
        if rel.parts[0] in GENERATED_DIRS:
            continue
        try:
            entries = json.loads(p.read_text(encoding="utf-8"))["entries"]
        except (OSError, ValueError, KeyError, TypeError):
            continue
        src = "src/" + rel.as_posix()[:-len(".o.diff.json")] + ".cpp"
        for e in entries:
            if isinstance(e.get("address"), int):
                out.setdefault(e["address"], src)
    return out


def pick(candidates: list[dict], count: int, seed: str) -> list[dict]:
    """A seeded sample, in address order: the same seed and the same candidates give the same functions."""
    ordered = sorted(candidates, key=lambda r: int(r["address"], 16))
    chosen = random.Random(seed).sample(ordered, min(count, len(ordered)))
    return sorted(chosen, key=lambda r: int(r["address"], 16))


def definition_line(text: str, name: str) -> int | None:
    short = name.split("::")[-1].split("(")[0]
    m = re.search(rf"^[^\s/#][^;\n]*\b{re.escape(short)}\s*\(", text, re.M)
    return text.count("\n", 0, m.start()) + 1 if m else None


def worksheet(sample: list[dict], sources: dict[int, str], seed: str, since: str, pool: int) -> str:
    import fakematch
    lines = [f"# Sampled read, seed {seed}", "",
             f"{len(sample)} of the {pool} hand-written functions that became O, m or M since {since}. For each, "
             "read the function in its file and answer:", ""]
    lines += [f"{i}. {q}" for i, q in enumerate(QUESTIONS, 1)]
    lines += ["", "Record the findings at the top of docs/JOURNAL.md with this seed; a finding that holds for a "
              "whole run or recipe matters more than one function's.", ""]
    for r in sample:
        address = int(r["address"], 16)
        src = sources[address]
        path = ROOT / src
        text = path.read_text(encoding="utf-8", errors="replace") if path.is_file() else ""
        line = definition_line(text, r["name"])
        short = r["name"].split("::")[-1]
        forms = Counter(h.kind for h in fakematch.scan_text(src, text)
                        if h.function and h.function.split("::")[-1] == short) if text else Counter()
        lines += [f"## {r['name']}  ({r['address']}, {r['size']} bytes, {r['status']})",
                  f"- {src}" + (f":{line}" if line else "  (definition line not found: read the file)"),
                  "- fake-match forms in it: " + (", ".join(f"{k} x{n}" for k, n in sorted(forms.items())) or "none"),
                  "- findings:", ""]
    return "\n".join(lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", default="2.1.0")
    ap.add_argument("--since", help="a date (YYYY-MM-DD); default: 7 days ago")
    ap.add_argument("--ref", default="origin/main", help="the branch whose history dates the sample")
    ap.add_argument("--count", type=int, default=20)
    ap.add_argument("--seed", help="default: the ISO week, e.g. 2026-W46")
    args = ap.parse_args()
    today = datetime.date.today()
    since = args.since or (today - datetime.timedelta(days=7)).isoformat()
    year, week, _ = today.isocalendar()
    seed = args.seed or f"{year}-W{week:02d}"
    base = git("rev-list", "-1", f"--before={since}T00:00:00", args.ref).strip()
    if not base:
        raise SystemExit(f"sample_review: {args.ref} has no commit before {since}")
    sources = defining_sources(args.version)
    new = [r for r in newly_done(statuses_at(base), functions.rows()) if int(r["address"], 16) in sources]
    sample = pick(new, args.count, seed)
    out = ROOT / "build" / "review" / f"sample_{seed}.md"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(worksheet(sample, sources, seed, since, len(new)), encoding="utf-8", newline="\n")
    by_status = Counter(r["status"] for r in new)
    print(f"since {since} ({base[:10]}): {len(new)} hand-written functions newly decompiled "
          f"({', '.join(f'{n} {s}' for s, n in sorted(by_status.items()))}); {len(sample)} sampled, seed {seed}")
    for r in sample:
        print(f"  {r['address']}  {r['size']:>5}  {r['status']}  {sources[int(r['address'], 16)]}")
    print(f"worksheet: {out.relative_to(ROOT).as_posix()}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
