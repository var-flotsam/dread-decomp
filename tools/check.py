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

Relocation targets: a function whose bytes match is O only if every relocation it
carries names the original's target too (asmdiff.check_relocs). A relocation's symbol
resolves to an original address when it is defined in the same object (through the
mapping above), when its demangled name is `fn_<addr>` or `g_unk_<addr>` (the older
`unk_<addr>` too; in any namespace: callee::, folded::, noret::), or when that name is unique in
data/functions/; the address plus the addend must equal what the original encodes
(BL/B offset, ADRP+lo12 address, or the GOT slot's RELATIVE relocation). Imports are
compared by name through the original's PLT stub or GOT slot. Section-relative
references (switch tables, string literals), conditional branches and symbols that do
not resolve are counted as unchecked, never as a mismatch. Each mismatch prints one
line, `RELOC  <addr>  <symbol> -> original <target>  +<offset>  <kind>  <source>`, and
the function is demoted from O to W (m/M when annotated), so a RELOC line on a
function that was O is also a REGRESSION. Generated sources are held to the same rule:
every generator verifies its candidates with asmdiff.diff_object(strict=True), so a RELOC
line in a generated file is a generator regression (fix the generator and regenerate, never
the output). REPORT_ONLY below can exempt top-level src/ directories temporarily; it is
empty. `--no-reloc` skips the check.

Usage:
  python tools/check.py --version 2.1.0            # report; exit 1 if an O function regressed or a function is defined in two files
  python tools/check.py --version 2.1.0 --update   # also write O/m/M/W into data/functions/
  python tools/check.py --version 2.1.0 --no-reloc # bytes only, as before the relocation check
  python tools/check.py --compile-only             # compile every source, no dump needed (hosted CI)
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
from dread import functions  # noqa: E402
import buildcfg  # noqa: E402
import hostenv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CXXFILT = hostenv.cxxfilt()
COMPILER = buildcfg.COMPILER  # project toolchain and flags: tools/buildcfg.py

ANNOTATION = re.compile(r"//\s*NON_MATCHING\((minor|major)\)")
DEFINITION = re.compile(r"^[A-Za-z_][\w:<>,\*& ]*?\b([A-Za-z_][\w:]*)\s*\(")
# Top-level src/ directories whose relocation mismatches are reported but do not demote. Empty
# since 2026-10-24: the generators verify relocation targets themselves and the Phase 3 samples
# were corrected. Name a directory here only as a temporary measure while a generator is fixed.
REPORT_ONLY: tuple[str, ...] = ()


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
    files = functions.by_file()
    rows = {r["address"]: r for frows in files.values() for r in frows}
    return rows, files


# Demangling (one llvm-cxxfilt call per batch) and parameter stripping live in asmdiff, which the
# generators use through asmdiff.diff_object(strict=True); these are the same functions.
demangle = asmdiff.demangle
strip_params = asmdiff.strip_params


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


def target_resolver(by_name: dict[str, list[int]], full_names: dict[str, str], defined: dict[str, int]):
    """Map a relocation's symbol to the original address it stands for, or None.

    asmdiff.target_resolver (in order: a function this object defines, the address its diff used;
    a demangled `fn_<addr>`/`g_unk_<addr>` name, asmdiff.TARGET_RE) with a name unique in
    data/functions/ as the last resort.
    """
    def unique(name: str) -> int | None:
        hits = by_name.get(name, [])
        return hits[0] if len(hits) == 1 else None
    return asmdiff.target_resolver(full_names, defined, unique)


def sources() -> list[Path]:
    return sorted((ROOT / "src").rglob("*.cpp"))


def csv_by_name(rows: dict[str, dict]) -> dict[str, list[int]]:
    """CSV name -> addresses carrying it (a name is a resolver key only when it has one)."""
    by_name: dict[str, list[int]] = {}
    for r in rows.values():
        by_name.setdefault(r["name"], []).append(int(r["address"], 16))
    return by_name


def diff_with_targets(functions: list[asmdiff.ObjFunction], original: asmdiff.Original,
                      by_name: dict[str, list[int]]):
    """Diff an object's functions and build the resolver for their relocation targets.

    Returns (diffs, names, full_names, targets): names maps each function symbol to its
    qualified demangled name, full_names every symbol (functions and relocation targets) to
    its full demangled form, targets(symbol) a relocation's symbol to an original address or None.
    """
    symbols = {f.symbol for f in functions}
    symbols.update(r.symbol for f in functions for r in f.relocs.values() if not r.section)
    full_names = demangle(sorted(symbols), strip=False)
    names = {f.symbol: full_names.get(f.symbol, f.symbol).split("(")[0] for f in functions}
    diffs = asmdiff.diff_functions(functions, original, resolver(by_name, names))
    targets = target_resolver(by_name, full_names, {res.symbol: res.address for res in diffs})
    return diffs, names, full_names, targets


def reloc_line(addr: str, m: asmdiff.RelocMismatch, full_names: dict[str, str], src: Path) -> str:
    sym = strip_params(full_names.get(m.symbol, m.symbol))
    return f"RELOC  {addr}  {asmdiff.format_mismatch(m, sym)}  {src.relative_to(ROOT).as_posix()}"


def compile_only(version: str, jobs: int) -> int:
    """Build every source and report failures; nothing is read from orig/ or written to data/."""
    from concurrent.futures import ThreadPoolExecutor
    srcs = sources()
    with ThreadPoolExecutor(jobs) as pool:
        objects = list(pool.map(lambda s: build(version, s), srcs))
    failed = sum(1 for o in objects if o is None)
    print(f"{len(srcs)} sources compiled, {failed} failed")
    return 1 if failed else 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="dump version to diff against (required unless --compile-only)")
    ap.add_argument("--update", action="store_true", help="write statuses into data/functions/")
    ap.add_argument("-j", "--jobs", type=int, default=8, help="parallel compiles")
    ap.add_argument("--no-reloc", action="store_true", help="skip the relocation target check")
    ap.add_argument("--compile-only", action="store_true",
                    help="only compile every source with the project toolchain; needs no dump (hosted CI)")
    args = ap.parse_args()
    if args.compile_only:
        return compile_only(args.version or "compile-only", args.jobs)
    if not args.version:
        ap.error("--version is required unless --compile-only")

    rows, files = load_csv()
    by_name = csv_by_name(rows)
    original = asmdiff.Original(args.version)

    results = {}  # address -> (status, symbol, score)
    defined_in: dict[str, set[Path]] = {}  # address -> source files that define it
    reloc_lines: dict[str, list[str]] = {}  # address -> RELOC lines, from every file defining it
    report_only_addrs: set[str] = set()  # addresses whose mismatches are in REPORT_ONLY directories
    demoted: set[str] = set()  # addresses a mismatch demoted
    report = asmdiff.RelocReport()
    srcs = sources()
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(args.jobs) as pool:
        objects = list(pool.map(lambda s: build(args.version, s), srcs))
    failed = any(o is None for o in objects)
    for src, obj in zip(srcs, objects):
        if obj is None:
            continue
        diffs, names, full_names, targets = diff_with_targets(asmdiff.read_object(obj), original, by_name)
        notes = annotations(src)
        report_only = src.relative_to(ROOT / "src").parts[0] in REPORT_ONLY
        for res in diffs:
            short = names.get(res.symbol, res.symbol).split("::")[-1]
            status = "O" if res.matching else notes.get(short, "W")
            addr = f"{res.address:#x}"
            if res.matching and not args.no_reloc:
                rep = asmdiff.check_relocs(res, original, targets)
                report.merge(rep)
                if rep.mismatches:
                    if report_only:
                        report_only_addrs.add(addr)
                    else:
                        status = notes.get(short, "W")
                        demoted.add(addr)
                    reloc_lines.setdefault(addr, []).extend(reloc_line(addr, m, full_names, src)
                                                            for m in rep.mismatches)
            results[addr] = (status, names.get(res.symbol, res.symbol), res.score)
            defined_in.setdefault(addr, set()).add(src)

    regressions = 0
    for addr, (status, name, score) in sorted(results.items()):
        old = rows.get(addr, {}).get("status")
        flag = ""
        if old == "O" and status != "O":
            flag = "  REGRESSION (was O)"
            regressions += 1
        for line in reloc_lines.get(addr, []):
            print(line)
        print(f"{status}  {addr}  {score:6.1%}  {name}{flag}")
    counts = {s: sum(1 for v in results.values() if v[0] == s) for s in "OmMW"}
    print(f"{len(results)} functions in src/: " + ", ".join(f"{k} {v}" for k, v in counts.items()))
    if not args.no_reloc:
        def kinds(c):
            return ", ".join(f"{k} {v}" for k, v in sorted(c.items(), key=lambda kv: -kv[1]))
        print(f"relocations: {sum(report.checked.values())} checked ({kinds(report.checked)}), "
              f"{len(report.mismatches)} mismatched in {len(reloc_lines)} functions"
              f" ({len(report_only_addrs - demoted)} in generated or sample files, reported only), "
              f"{sum(report.unchecked.values())} unchecked ({kinds(report.unchecked)})")
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
                elif r["address"] not in results and r["status"] in ("O", "m", "M", "W") and not failed:
                    r["status"] = "U"  # nothing in src/ defines it any more
                    changed = True
            if changed:
                functions.write(name, frows)
        print("statuses written to data/functions/")
    return 1 if failed or regressions or duplicates else 0


if __name__ == "__main__":
    sys.exit(main())
