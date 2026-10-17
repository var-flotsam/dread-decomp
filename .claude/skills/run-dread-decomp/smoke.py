#!/usr/bin/env python3
"""Smoke driver for dread-decomp: checks the local setup and exercises the matching loop.

Run with the repo venv from the repo root:
  .venv/Scripts/python .claude/skills/run-dread-decomp/smoke.py all
Subcommands:
  env       prerequisites: venv, dump, ELF, Clang 9.0.1, libc++ headers, submodules, objdiff, Ghidra
  selftest  trymatch on a known-matching source (expects MATCH) and on a broken copy (expects a diff)
  check     tools/check.py: builds all of src/, fails on any REGRESSION line
  try FILE  tools/trymatch.py on one source (extra args after FILE go to trymatch)
  objdiff UNIT  objdiff-cli one-shot diff of one objdiff.json unit (needs tools/objdiff_gen.py first)
  progress  tools/progress.py summary
  all       env, selftest, check, progress
Exit status: 0 when every step passed. Prints no game code or data.
"""

from __future__ import annotations

import os
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
VERSION = os.environ.get("DREAD_VERSION", "2.1.0")
PY = ROOT / ".venv" / "Scripts" / "python.exe"
if not PY.is_file():
    PY = ROOT / ".venv" / "bin" / "python"
OUT = ROOT / "build" / "smoke"
# A hand-written source that matches byte for byte (one function).
KNOWN = ROOT / "src" / "base" / "reflection" / "UnkReflFunction.cpp"


def run(args: list[str], timeout: int = 900) -> subprocess.CompletedProcess:
    t = time.time()
    r = subprocess.run([str(PY), *args], cwd=ROOT, capture_output=True, text=True, timeout=timeout)
    print(f"  ({time.time() - t:.1f} s, exit {r.returncode})")
    return r


def env() -> bool:
    sys.path.insert(0, str(ROOT / "tools"))
    import buildcfg  # noqa: E402
    import fetch_sources  # noqa: E402
    import fetch_toolchains  # noqa: E402

    local = Path(os.environ.get("LOCALAPPDATA", ""))
    checks = [
        ("venv Python", PY.is_file(), "py -3.12 -m venv .venv (see docs/HANDOFF.md)"),
        (f"dump orig/{VERSION}/exefs/main", (ROOT / "orig" / VERSION / "exefs" / "main").is_file(),
         "tools/setup.py --version ... --exefs ... --romfs ..."),
        (f"orig/{VERSION}/main.elf", (ROOT / "orig" / VERSION / "main.elf").is_file(),
         f"tools/nso2elf.py orig/{VERSION}/exefs/main orig/{VERSION}/main.elf"),
        (f"Clang {buildcfg.COMPILER}", fetch_toolchains.clang_path(buildcfg.COMPILER).is_file(),
         "tools/fetch_toolchains.py"),
        ("libc++ 9.0.1 headers", buildcfg.libcxx_include().is_dir(), "tools/fetch_sources.py"),
        ("lib/nnsdk submodule", any((ROOT / "lib" / "nnsdk").glob("*")), "git submodule update --init"),
        ("lib/musl submodule", any((ROOT / "lib" / "musl").glob("*")), "git submodule update --init"),
        ("function list", any((ROOT / "data" / "functions").glob("main_*.csv")),
         "tools/gen_functions_csv.py --version ..."),
        ("objdiff CLI (optional)", any((local / "dread-decomp" / "tools").glob("objdiff-cli*")),
         "tools/fetch_toolchains.py"),
        ("Ghidra project (optional, decompile.py)", (ROOT / "build" / "ghidra" / VERSION / "dread.gpr").is_file(),
         "tools/ghidra/analyze.py --version ..."),
    ]
    ok = True
    for name, present, fix in checks:
        optional = "optional" in name
        print(f"  {'ok  ' if present else ('skip' if optional else 'MISS')}  {name}"
              + ("" if present else f"  -> {fix}"))
        ok &= present or optional
    del fetch_sources
    return ok


def selftest() -> bool:
    OUT.mkdir(parents=True, exist_ok=True)
    print(f"  trymatch {KNOWN.relative_to(ROOT)} (expect MATCH)")
    r = run(["tools/trymatch.py", "--version", VERSION, str(KNOWN)])
    good = r.returncode == 0 and "MATCH" in r.stdout
    print("  " + (r.stdout.strip().splitlines() or ["(no output)"])[-1])
    # The same source with one store changed must be reported as a mismatch (exit 1).
    broken = OUT / "selftest_broken.cpp"
    text = KNOWN.read_text(encoding="utf-8")
    broken.write_text(text.replace("fn->field_0xb2 = 0;", "fn->field_0xb2 = 1;"), encoding="utf-8")
    print("  trymatch broken copy (expect a mismatch, exit 1)")
    r2 = run(["tools/trymatch.py", "--version", VERSION, str(broken)])
    bad = r2.returncode == 1 and "0/1 functions match" in r2.stdout
    print("  " + (r2.stdout.strip().splitlines() or ["(no output)"])[-1])
    return good and bad


def check() -> bool:
    r = run(["tools/check.py", "--version", VERSION])
    lines = r.stdout.strip().splitlines()
    regressions = [l for l in lines if "REGRESSION" in l]
    for l in regressions[:20]:
        print("  " + l)
    print("  " + (lines[-1] if lines else r.stderr.strip()[-500:]))
    return r.returncode == 0 and not regressions


def try_one(args: list[str]) -> bool:
    r = run(["tools/trymatch.py", "--version", VERSION, *args])
    print(r.stdout, end="")
    print(r.stderr, end="", file=sys.stderr)
    return r.returncode == 0


def objdiff_unit(unit: str) -> bool:
    """objdiff-cli one-shot diff of one unit from objdiff.json: match % per function."""
    import json

    if not (ROOT / "objdiff.json").is_file():
        print("  objdiff.json missing -> tools/objdiff_gen.py --version " + VERSION)
        return False
    tools = Path(os.environ.get("LOCALAPPDATA", "")) / "dread-decomp" / "tools"
    cli = sorted(tools.glob("objdiff-cli*"))
    if not cli:
        print("  objdiff-cli missing -> tools/fetch_toolchains.py")
        return False
    OUT.mkdir(parents=True, exist_ok=True)
    out = OUT / "objdiff_unit.json"
    r = subprocess.run([str(cli[-1]), "--no-color", "diff", "-p", ".", "-u", unit, "-o", str(out)],
                       cwd=ROOT, capture_output=True, text=True)
    if r.returncode != 0:
        print(r.stderr.strip()[-1000:])
        return False
    left = json.loads(out.read_text(encoding="utf-8")).get("left") or {}  # left = original
    ok = True
    for sym in left.get("symbols") or []:
        pct = sym.get("match_percent")
        name = (sym.get("symbol") or {}).get("name") or sym.get("name")
        if pct is not None:
            print(f"  {pct:6.2f}%  {name}")
            ok &= pct == 100.0
    return ok


def progress() -> bool:
    r = run(["tools/progress.py"])
    print("\n".join("  " + l for l in r.stdout.splitlines()[:3]))
    return r.returncode == 0


def main() -> int:
    if len(sys.argv) < 2:
        print(__doc__)
        return 2
    cmd, rest = sys.argv[1], sys.argv[2:]
    steps = {"env": env, "selftest": selftest, "check": check, "progress": progress}
    if cmd == "try":
        if not rest:
            print("usage: smoke.py try FILE [trymatch args]")
            return 2
        return 0 if try_one(rest) else 1
    if cmd == "objdiff":
        if not rest:
            print("usage: smoke.py objdiff UNIT   (unit names are in objdiff.json, e.g. base/reflection/UnkReflFunction)")
            return 2
        return 0 if objdiff_unit(rest[0]) else 1
    names = ["env", "selftest", "check", "progress"] if cmd == "all" else [cmd]
    if any(n not in steps for n in names):
        print(__doc__)
        return 2
    failed = []
    for n in names:
        print(f"== {n}")
        if not steps[n]():
            failed.append(n)
            if n == "env":
                break
    print("== result: " + ("PASS" if not failed else "FAIL (" + ", ".join(failed) + ")"))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
