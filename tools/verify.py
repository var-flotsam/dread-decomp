#!/usr/bin/env python3
"""One-command gate: regenerate, check, report progress and the content rules, in a few lines.

Runs, in order:
  1. the generators named with --regen, in the order CLAUDE.md lists them
     (gen_class_getters, gen_classes, gen_reflection, gen_types, gen_statics, gen_clones, gen_tables,
     gen_lift; gen_class_getters writes include/base/reflection/UnkClassGetters.h from
     data/functions/ alone; gen_classes writes include/reflected/, which gen_reflection reads, and
     without --regen gen_classes `gen_classes.py --check` fails the gate when those headers are stale);
     without gen_lift, `gen_lift.py --prune` instead (seconds), which
     drops lifted copies of functions that other sources now define. gen_types brings the rest of
     its chain: gen_functions_csv (its named forms' names into data/functions/), gen_types again
     (the named forms are compared through those names) and gen_clones (the clones of slots that
     now have names drop out);
  2. tools/check.py --update (build src/, write O/m/M/W into data/functions/);
  3. tools/progress.py;
  4. the tools/policy_check.py rules, on tracked and untracked (not ignored) files;
  5. tools/decl_check.py in report mode: how many fn_/g_unk_ symbols the hand-written sources
     declare in more than one file or with conflicting signatures (informational; it never fails
     this gate, CI enforces no growth per pull request with `decl_check.py --base`);
  6. tools/name_check.py --strict: every name that is not an address points to one address of the
     original in all the matching functions that use it.

Prints PASS/FAIL per step, the O count before -> after, regressions (count and up to 5
addresses), game-code progress and, on failure, the next action. Exit status 0 only if every
step passes, no function that was O is lost and no function is defined in two files.

"Before" is the set of O functions from the last passing run, kept in
build/verify/<version>/baseline.txt; the first run builds it with tools/check.py (no --update)
before anything else. A function counts as a regression when it was O there and is not O now
(it no longer matches, or nothing in src/ defines it any more).

verify.py holds check.py's lock (build/check/<version>/check.lock, tools/dread/runlock.py) for its whole run, so
no other check.py or verify.py, from this checkout or one sharing build/, runs between its steps; the check.py it
starts runs under that lock. --wait is how long to wait for another run that holds it (minutes, default 30).

Usage: python tools/verify.py --version 2.1.0 [--regen gen_statics,gen_clones] [--wait 30]
"""

from __future__ import annotations

import argparse
import json
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import policy_check  # noqa: E402
from dread import runlock  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GENERATORS = ["gen_class_getters", "gen_classes", "gen_reflection", "gen_types", "gen_statics", "gen_clones", "gen_tables",
              "gen_lift"]
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
    ap.add_argument("--wait", type=float, default=30, metavar="MINUTES",
                    help="how long to wait for another run that holds build/check/<version>/ (default 30)")
    args = ap.parse_args()
    try:
        with runlock.RunLock(ROOT / "build" / "check" / args.version, "verify.py", wait=max(0.0, args.wait) * 60,
                             worktree=ROOT):
            return run_steps(args)
    except runlock.Busy as e:
        print(f"FAIL {e}")
        return runlock.EXIT_BUSY


def stopped(rc: int, lines: list[str]) -> str | None:
    """Why check.py ended without a result (an exception, or it gave up waiting for the lock), None when it ran
    to the end: its statuses are then the tree's, not a fragment of them to count as lost."""
    if rc != runlock.EXIT_BUSY and not any(ln.startswith("Traceback (most recent call last)") for ln in lines):
        return None
    return f"check.py stopped before its summary (exit {rc}): {short(lines[-1] if lines else '(no output)')}"


def run_steps(args: argparse.Namespace) -> int:
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
        rc, baseline, failed, summary, lines = check(v, update=False)
        why = stopped(rc, lines)
        if why:
            print(f"FAIL baseline: {why}")
            print(f"NEXT: run `python tools/check.py --version {v}` alone and read the end of its output")
            return 1
        if failed:
            print(f"FAIL baseline: {len(failed)} sources do not build before any regeneration: {', '.join(failed[:3])}")
            print("NEXT: fix the build first: python tools/check.py --version " + v + " | grep -A5 'BUILD FAILED'")
            return 1
        print(f"NOTE baseline: none saved; built one from the current tree (O {len(baseline)})")

    if "gen_types" in wanted:
        wanted.add("gen_clones")
    steps = []
    for g in GENERATORS:
        if g in wanted:
            steps.append((g, ["--version", v]))
            if g == "gen_types":
                steps += [("gen_functions_csv", ["--version", v]), ("gen_types", ["--version", v])]
    if "gen_classes" not in wanted:
        steps.insert(0, ("gen_classes", ["--check"]))
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
        # "REGRESSION (was O)", or "REGRESSION (was O, no source defines it: ...)" for a lost definition
        flagged = {ln.split()[1] for ln in lines if "REGRESSION (was O" in ln}
        why = stopped(rc, lines)
        lost = [] if why else sorted((baseline - current) | flagged)
        if why:
            ok = False
            print(f"FAIL check.py --update: {why}")
            nexts.append(f"run `python tools/check.py --version {v} --update` alone and read the end of its output")
        elif failed:
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
        elif not failed and not why:
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

    # Informational: declaration consistency of the hand-written sources. Never fails this gate;
    # CI enforces no growth per pull request with `tools/decl_check.py --base <ref>`.
    rc, lines, _ = run("tools/decl_check.py", "--json")
    try:
        d = json.loads("\n".join(lines))
        print(f"PASS decl_check (info): {d['declared']} fn_/g_unk_ symbols declared in {d['files']} hand-written "
              f"files, {d['multi_file']} in more than one file ({d['extra_files']} extra copies), {d['conflicting']} "
              f"with conflicting signatures ({d['extra_signatures']} extra), {d['header_redeclared']} declared in "
              "include/ and again in src/ (no growth enforced by `decl_check.py --base <ref>`)")
    except (ValueError, KeyError):
        print(f"NOTE decl_check: no counts (exit {rc}): {short(lines[-1] if lines else '')}")

    rc, lines, _ = run("tools/name_check.py", "--version", v, "--strict")
    summary = next((ln for ln in lines if ln.startswith("name_check:")), lines[-1] if lines else "")
    if rc == 0:
        print(f"PASS {short(summary)}")
    else:
        ok = False
        print(f"FAIL {short(summary)}")
        nexts.append("`python tools/name_check.py --version <ver>` lists each name and where its references point")

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
