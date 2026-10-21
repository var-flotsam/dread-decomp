#!/usr/bin/env python3
"""Build every source in src/ with the project toolchain and check it against the original.

Function status codes follow zeldaret/botw (nx-decomp-tools); see docs/references.md:
  O  matching                 (byte-identical, relocated fields masked)
  m  non-matching (minor)     source annotated `// NON_MATCHING(minor): <reason>`
  M  non-matching (major)     source annotated `// NON_MATCHING(major): <reason>`
  W  work in progress         compiled, does not match, not annotated
  U  not decompiled           (set by tools/gen_functions_csv.py)
  L  library                  (set by tools/gen_functions_csv.py; --update never changes it)

An annotation applies to the next function definition after it (a line starting
at column 0 with `name(`). A compiled symbol maps to an original function by
`fn_<addr>` in its name, or by its demangled name when that name is unique in
data/functions/.

Usage:
  python tools/check.py --version 2.1.0            # report; exit 1 if an O function regressed or a function is defined in two files
  python tools/check.py --version 2.1.0 --update   # also write O/m/M/W into data/functions/
"""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import hostenv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CXXFILT = hostenv.cxxfilt()
COMPILER = buildcfg.COMPILER  # project toolchain and flags: tools/buildcfg.py

ANNOTATION = re.compile(r"//\s*NON_MATCHING\((minor|major)\)")
DEFINITION = re.compile(r"^[A-Za-z_][\w:<>,\*& ]*?\b([A-Za-z_][\w:]*)\s*\(")


def annotations(src: Path) -> dict[str, str]:
    """Map function name (last component of the qualified name) -> 'm' or 'M'."""
    out, pending = {}, None
    for line in src.read_text(encoding="utf-8").splitlines():
        a = ANNOTATION.search(line)
        if a:
            pending = "m" if a.group(1) == "minor" else "M"
            continue
        d = DEFINITION.match(line)
        if d and pending and not line.rstrip().endswith(";"):
            out[d.group(1).split("::")[-1]] = pending
            pending = None
    return out


def load_csv() -> tuple[dict[str, dict], dict[str, list[dict]]]:
    rows, files = {}, {}
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            files[f.name] = list(csv.DictReader(fh))
        for r in files[f.name]:
            rows[r["address"]] = r
    return rows, files


def demangle(names: list[str]) -> dict[str, str]:
    if not names:
        return {}
    out = subprocess.run([CXXFILT], input="\n".join(names), capture_output=True, text=True,
                         check=True).stdout.splitlines()
    return {n: o.split("(")[0] for n, o in zip(names, out)}


def object_path(version: str, src: Path) -> Path:
    return ROOT / "build" / "check" / version / src.relative_to(ROOT / "src").with_suffix(".o")


def build(version: str, src: Path) -> Path | None:
    """Compile one source with the project toolchain. Returns the object, or None on failure."""
    obj = object_path(version, src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        print(f"BUILD FAILED {src.relative_to(ROOT)}\n{r.stderr}")
        return None
    return obj


def resolver(by_name: dict[str, list[int]], names: dict[str, str]):
    """Map an object symbol to an original address through a unique demangled CSV name."""
    def resolve(sym: str) -> int | None:
        hits = by_name.get(names.get(sym, sym), [])
        return hits[0] if len(hits) == 1 else None
    return resolve


def sources() -> list[Path]:
    return sorted((ROOT / "src").rglob("*.cpp"))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--update", action="store_true", help="write statuses into data/functions/")
    ap.add_argument("-j", "--jobs", type=int, default=8, help="parallel compiles")
    args = ap.parse_args()

    rows, files = load_csv()
    by_name: dict[str, list[int]] = {}
    for r in rows.values():
        by_name.setdefault(r["name"], []).append(int(r["address"], 16))
    original = asmdiff.Original(args.version)

    results = {}  # address -> (status, symbol, score)
    defined_in: dict[str, set[Path]] = {}  # address -> source files that define it
    srcs = sources()
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(args.jobs) as pool:
        objects = list(pool.map(lambda s: build(args.version, s), srcs))
    failed = any(o is None for o in objects)
    for src, obj in zip(srcs, objects):
        if obj is None:
            continue
        names = demangle([n for n, _, _ in asmdiff.object_functions(obj)])
        resolve = resolver(by_name, names)
        notes = annotations(src)
        for res in asmdiff.diff_object(obj, original, resolve):
            short = names.get(res.symbol, res.symbol).split("::")[-1]
            status = "O" if res.matching else notes.get(short, "W")
            results[f"{res.address:#x}"] = (status, names.get(res.symbol, res.symbol), res.score)
            defined_in.setdefault(f"{res.address:#x}", set()).add(src)

    regressions = 0
    for addr, (status, name, score) in sorted(results.items()):
        old = rows.get(addr, {}).get("status")
        flag = ""
        if old == "O" and status != "O":
            flag = "  REGRESSION (was O)"
            regressions += 1
        print(f"{status}  {addr}  {score:6.1%}  {name}{flag}")
    counts = {s: sum(1 for v in results.values() if v[0] == s) for s in "OmMW"}
    print(f"{len(results)} functions in src/: " + ", ".join(f"{k} {v}" for k, v in counts.items()))
    # Parallel workstreams can each define the same function (e.g. a clone and a lifted copy).
    # The Phase 3 identification samples overlap other sources by design and are not counted.
    samples = [ROOT / "src" / d for d in ("compiler_id", "flag_id", "atk_id")]
    duplicates = {}
    for a, fs in defined_in.items():
        fs = {f for f in fs if not any(d in f.parents for d in samples)}
        if len(fs) > 1:
            duplicates[a] = fs
    for addr, fs in sorted(duplicates.items()):
        print(f"DUPLICATE  {addr}  " + "  ".join(str(f.relative_to(ROOT).as_posix()) for f in sorted(fs)))
    if duplicates:
        print(f"{len(duplicates)} functions defined in more than one file; rerun the generators (verify.py --regen)")

    if args.update:
        for name, frows in files.items():
            changed = False
            for r in frows:
                # Library functions keep L (excluded from progress) even when they match.
                if r["address"] in results and r["status"] != "L" and r["status"] != results[r["address"]][0]:
                    r["status"] = results[r["address"]][0]
                    changed = True
            if changed:
                with (ROOT / "data" / "functions" / name).open("w", newline="", encoding="utf-8") as fh:
                    w = csv.DictWriter(fh, fieldnames=list(frows[0].keys()), lineterminator="\n")
                    w.writeheader()
                    w.writerows(frows)
        print("statuses written to data/functions/")
    return 1 if failed or regressions or duplicates else 0


if __name__ == "__main__":
    sys.exit(main())
