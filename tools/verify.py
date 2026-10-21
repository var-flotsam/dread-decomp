#!/usr/bin/env python3
"""One-command gate: regenerate, check, report progress and the content rules, in a few lines.

Runs, in order:
  1. the generators named with --regen, in the order CLAUDE.md lists them
     (gen_reflection, gen_types, gen_statics, gen_clones, gen_tables, gen_lift); without
     gen_lift, `gen_lift.py --prune` instead (seconds), which drops lifted copies of functions
     that other sources now define;
  2. tools/check.py --update (build src/, write O/m/M/W into data/functions/);
  3. tools/progress.py;
  4. the tools/policy_check.py rules, on tracked and untracked (not ignored) files.

Prints PASS/FAIL per step, the O count before -> after, regressions (count and up to 5
addresses), game-code progress and, on failure, the next action. Exit status 0 only if every
step passes, no function that was O is lost and no function is defined in two files.

"Before" is the set of O functions from the last passing run, kept in
build/verify/<version>/baseline.txt; the first run builds it with tools/check.py (no --update)
before anything else. A function counts as a regression when it was O there and is not O now
(it no longer matches, or nothing in src/ defines it any more).

Usage: python tools/verify.py --version 2.1.0 [--regen gen_statics,gen_clones]
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import policy_check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GENERATORS = ["gen_reflection", "gen_types", "gen_statics", "gen_clones", "gen_tables", "gen_lift"]
STATUS_LINE = re.compile(r"^([OmMW])  (0x[0-9a-f]+)  ")
SUMMARY_LINE = re.compile(r"^\d+ functions in src/: (.*)$")
PROGRESS_LINE = re.compile(r"^matching \(O\): (\d+) functions, ([\d.]+)% of code")


def run(*args: str) -> tuple[int, list[str], float]:
    start = time.time()
    r = subprocess.run([sys.executable, *args], cwd=ROOT, capture_output=True, text=True,
                       encoding="utf-8", errors="replace")
    return r.returncode, (r.stdout + r.stderr).splitlines(), time.time() - start


def check(version: str, update: bool) -> tuple[int, set[str], list[str], str, list[str]]:
    """Run check.py: (exit code, O addresses, BUILD FAILED sources, summary, all lines)."""
    rc, lines, _ = run("tools/check.py", "--version", version, *(["--update"] if update else []))
    o = {m.group(2) for m in map(STATUS_LINE.match, lines) if m and m.group(1) == "O"}
    failed = [ln.split(" ", 2)[2] for ln in lines if ln.startswith("BUILD FAILED ")]
    summary = next((m.group(1) for m in map(SUMMARY_LINE.match, lines) if m), "")
    return rc, o, failed, summary, lines


def short(text: str, width: int = 100) -> str:
    return text if len(text) <= width else text[:width - 3] + "..."


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--regen", default="", help="comma-separated generators to rerun first: " + ", ".join(GENERATORS))
    args = ap.parse_args()
    v = args.version
    wanted = {g.strip().removesuffix(".py") for g in args.regen.split(",") if g.strip()}
    unknown = wanted - set(GENERATORS)
    if unknown:
        print(f"FAIL --regen: unknown {', '.join(sorted(unknown))} (choose from {', '.join(GENERATORS)})")
        return 2

    ok = True
    nexts: list[str] = []
    base_file = ROOT / "build" / "verify" / v / "baseline.txt"
    if base_file.is_file():
        baseline = set(base_file.read_text(encoding="utf-8").split())
    else:
        rc, baseline, failed, _, _ = check(v, update=False)
        if failed:
            print(f"FAIL baseline: {len(failed)} sources do not build before any regeneration: {', '.join(failed[:3])}")
            print("NEXT: fix the build first: python tools/check.py --version " + v + " | grep -A5 'BUILD FAILED'")
            return 1
        print(f"NOTE baseline: none saved; built one from the current tree (O {len(baseline)})")

    steps = [(g, ["--version", v]) for g in GENERATORS if g in wanted]
    if "gen_lift" not in wanted:
        steps.append(("gen_lift", ["--prune"]))
    for g, gargs in steps:
        rc, lines, secs = run(f"tools/{g}.py", *gargs)
        last = lines[-1] if lines else ""
        if rc == 0:
            print(f"PASS {g} ({secs:.0f} s): {short(last)}")
        else:
            ok = False
            print(f"FAIL {g} (exit {rc}): {short(last)}")
            nexts.append(f"run `python tools/{g}.py --version {v}` alone and read the end of its error")
            break  # later steps would run on a half-regenerated tree

    if ok:
        rc, current, failed, summary, lines = check(v, update=True)
        flagged = {ln.split()[1] for ln in lines if ln.endswith("REGRESSION (was O)")}
        lost = sorted((baseline - current) | flagged)
        if failed:
            ok = False
            print(f"FAIL check.py --update: {len(failed)} sources do not build: {', '.join(failed[:3])}")
            nexts.append(f"fix the build error: python tools/check.py --version {v} | grep -A5 'BUILD FAILED'")
        else:
            print(f"{'PASS' if not lost else 'FAIL'} check.py --update: O {len(baseline)} -> {len(current)} "
                  f"({len(current) - len(baseline):+d}); {summary}")
        if lost:
            ok = False
            print(f"FAIL regressions: {len(lost)} functions no longer O: {' '.join(lost[:5])}")
            nexts.append("undo the change that lost them (git diff; git checkout -- data/functions), or diff one with "
                         f"`python tools/trymatch.py --version {v} <file.cpp> -v`. If main removed them on purpose, "
                         f"delete {base_file.relative_to(ROOT).as_posix()} and rerun")
        elif not failed:
            print("PASS regressions: 0")
        dups = [ln.split()[1] for ln in lines if ln.startswith("DUPLICATE ")]
        if dups:
            ok = False
            print(f"FAIL duplicates: {len(dups)} functions defined in two files: {' '.join(dups[:5])}")
            nexts.append(f"see the DUPLICATE lines of `python tools/check.py --version {v}`; rerun the generator "
                         "that should drop its copy, or remove the hand-written one")

    rc, lines, _ = run("tools/progress.py")
    m = next((m for m in map(PROGRESS_LINE.match, lines) if m), None)
    if rc == 0 and m:
        print(f"PASS progress: {m.group(2)}% of game code matching (O {m.group(1)} in data/functions/)")
    else:
        ok = False
        print(f"FAIL progress.py (exit {rc})")
        nexts.append("run `python tools/progress.py` and read its error")

    files = sorted(set(policy_check.tracked(None)) | set(subprocess.run(
        ["git", "ls-files", "--others", "--exclude-standard"], cwd=ROOT, capture_output=True, text=True,
        check=True).stdout.splitlines()))
    bad = [(f, p) for f in files for p in policy_check.problems(f)]
    if bad:
        ok = False
        print(f"FAIL policy: {len(bad)} problems, first: {bad[0][0]}: {bad[0][1]}")
        nexts.append("fix or delete the files `python tools/policy_check.py` names (never commit them)")
    else:
        print(f"PASS policy: {len(files)} files, 0 problems")

    if ok:
        base_file.parent.mkdir(parents=True, exist_ok=True)
        base_file.write_text("\n".join(sorted(current)) + "\n", encoding="utf-8")
        print("RESULT: PASS")
        return 0
    print("RESULT: FAIL")
    print("NEXT: " + nexts[0] if nexts else "NEXT: read the FAIL lines above")
    return 1


if __name__ == "__main__":
    sys.exit(main())
