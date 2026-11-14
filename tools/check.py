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
data/functions/ (which carries the names data/names.csv gives, tools/rename.py). A function
that was O and that no source defines any more is a REGRESSION too: its definition is gone,
or has a name that maps to nothing.

Relocation targets: a function whose bytes match is O only if every relocation it
carries names the original's target too (asmdiff.check_relocs). A relocation's symbol
resolves to an original address when it is defined in the same object (through the
mapping above), when its demangled name is `fn_<addr>` or `g_unk_<addr>` (the older
`unk_<addr>` too; in any namespace: callee::, folded::, noret::), when it is a constructor or
destructor whose declaration names its address (Clang 9 ignores an asm label on one;
tools/dread/asm_labels.py reads the declaration), or when that name is unique in data/functions/;
the address plus the addend must equal what the original encodes
(BL/B offset, ADRP+lo12 address, or the GOT slot's RELATIVE relocation). Imports are
compared by name through the original's PLT stub or GOT slot. The content of a string
literal and of a constant-pool entry (a floating-point or vector constant, a switch's
lookup table) must equal the original's at the address its instructions encode. Other
section-relative references (jump tables, wide strings), conditional branches and symbols
that do not resolve are counted as unchecked, never as a mismatch, except a call to a constructor
or destructor that the object does not define and whose address nothing names (kind `special`).
Each mismatch prints one
line, `RELOC  <addr>  <symbol> -> original <target>  +<offset>  <kind>  <source>`, and
the function is demoted from O to W (m/M when annotated), so a RELOC line on a
function that was O is also a REGRESSION. Generated sources are held to the same rule:
every generator verifies its candidates with asmdiff.diff_object(strict=True), so a RELOC
line in a generated file is a generator regression (fix the generator and regenerate, never
the output). REPORT_ONLY below can exempt top-level src/ directories temporarily; it is
empty. `--no-reloc` skips the check.

Caches (build/check/<version>/, next to each object): a source is recompiled only when its
compile command or the content of any file in its dependency list (clang -MD: the source and
every header it includes, system headers too) or of the object itself differs from the last
build (`<obj>.inputs.json`). An object's diff is reused only when the object's bytes and every
input of the diff (this file, asmdiff.py, the original module and its function sizes, the names
in data/functions/, --no-reloc) are the same as when it was computed (`<obj>.diff.json`).
Statuses are always derived afresh from the diff and the source's annotations. `--no-cache`
rebuilds and rediffs everything (the nightly run does), refreshing both caches.

Verified equivalence (tier 2 of CLAUDE.md's completion): data/equivalence.csv holds the verdicts of
tools/equiv_test.py, each with the digest of the code it judged. Every `pass` row must still describe
the tree: its function is defined in src/ as NON_MATCHING(minor) and its code has that digest. A row
that does not prints `EQUIV  <addr>  <why>` and the check fails: run tools/equiv_test.py --write on the
file again, or take the row out (a function that now matches needs none).

Wrong addresses in functions that do not match (tools/ref_check.py): the relocation check above needs
matching bytes, so a non-matching function is compared as a set: what it calls and refers to against what
the original function calls and refers to. A global named after a GOT slot, and a call or a global the
original never has in a function whose difference is registers-only, print `REFS  <addr>  <what>` and the
check fails. So does such a call or global in any function marked NON_MATCHING(minor) unless its note names it
(the address or the import's name): `minor` claims the same behaviour, and the difference has to be accounted
for in words. Other differences of the sets are for `python tools/ref_check.py` to list.

Usage:
  python tools/check.py --version 2.1.0            # report; exit 1 if an O function regressed or a function is defined in two files
  python tools/check.py --version 2.1.0 --update   # also write O/m/M/W into data/functions/
  python tools/check.py --version 2.1.0 --no-reloc # bytes only, as before the relocation check
  python tools/check.py --version 2.1.0 --no-cache # rebuild and rediff every source
  python tools/check.py --compile-only             # compile every source, no dump needed (hosted CI)
"""

from __future__ import annotations

import argparse
import dataclasses
import hashlib
import json
import os
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
from dread import asm_labels, functions  # noqa: E402
import buildcfg  # noqa: E402
import equiv_test  # noqa: E402
import hostenv  # noqa: E402
import ref_check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
CXXFILT = hostenv.cxxfilt()
COMPILER = buildcfg.COMPILER  # project toolchain and flags: tools/buildcfg.py

ANNOTATION = re.compile(r"//\s*NON_MATCHING\((minor|major)\)")
DEFINITION = re.compile(r"^[A-Za-z_][\w:<>,\*& ]*?\b([A-Za-z_][\w:]*)\s*\(")
# Top-level src/ directories whose relocation mismatches are reported but do not demote. Empty
# since 2026-10-24: the generators verify relocation targets themselves and the Phase 3 samples
# were corrected. Name a directory here only as a temporary measure while a generator is fixed.
REPORT_ONLY: tuple[str, ...] = ()
# Bump when the layout of <obj>.inputs.json or <obj>.diff.json changes.
CACHE_VERSION = 1
DIFF_CHUNK = 256  # objects diffed per llvm-cxxfilt call


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


def minor_notes(src: Path) -> dict[str, str]:
    """Map function name (last component) -> the text of its NON_MATCHING(minor) note: the comment lines
    from the annotation down to the definition."""
    out: dict[str, str] = {}
    pending: list[str] | None = None
    for line in src.read_text(encoding="utf-8").splitlines():
        a = ANNOTATION.search(line)
        if a:
            pending = [line] if a.group(1) == "minor" else None
            continue
        if pending is not None and line.lstrip().startswith("//"):
            pending.append(line)
            continue
        d = DEFINITION.match(line)
        if d and pending is not None and not line.rstrip().endswith(";"):
            out[d.group(1).split("::")[-1]] = "\n".join(pending)
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


def digest(path: Path) -> str | None:
    """sha256 of a file's content, None when it cannot be read."""
    try:
        return hashlib.sha256(path.read_bytes()).hexdigest()
    except OSError:
        return None


_input_digests: dict[str, str | None] = {}


def input_digest(path: str) -> str | None:
    """digest() of a compile input, computed once per run: sources and headers do not change
    while check.py runs (objects do, and use digest())."""
    if path not in _input_digests:
        _input_digests[path] = digest(Path(path))
    return _input_digests[path]


DEP_TOKEN = re.compile(r"(?:\\[ #]|\S)+")


def depfile_inputs(text: str) -> list[str]:
    """The prerequisites of a clang -MD depfile (one Make rule; backslash-newline continuations,
    `\\ ` and `\\#` escapes, `$$`), as absolute paths. Windows paths keep their backslashes."""
    body = text.replace("\\\r\n", " ").replace("\\\n", " ")
    _, sep, deps = body.partition(": ")
    if not sep:
        return []
    return [os.path.abspath(re.sub(r"\\([ #])", r"\1", t).replace("$$", "$")) for t in DEP_TOKEN.findall(deps)]


def inputs_path(obj: Path) -> Path:
    return obj.with_name(obj.name + ".inputs.json")


def up_to_date(obj: Path, cmd: list[str]) -> bool:
    """True when obj was built by cmd from inputs that are all unchanged (<obj>.inputs.json)."""
    try:
        m = json.loads(inputs_path(obj).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return False
    if m.get("cache") != CACHE_VERSION or m.get("cmd") != cmd or not m.get("inputs"):
        return False
    if m.get("object") is None or digest(obj) != m["object"]:
        return False
    return all(d is not None and input_digest(p) == d for p, d in m["inputs"].items())


def build(version: str, src: Path, cache: bool = True) -> Path | None:
    """Compile one source with the project toolchain. Returns the object, or None on failure.

    With cache, an object whose command and inputs are unchanged is not rebuilt (see the docstring).
    """
    obj = object_path(version, src)
    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = buildcfg.cxx_command(src, obj)
    if cache and up_to_date(obj, cmd):
        return obj
    dep = obj.with_suffix(".d")
    inputs_path(obj).unlink(missing_ok=True)
    # -MD -MF only writes the dependency list; the object is the same as without them.
    r = subprocess.run([*cmd, "-MD", "-MF", str(dep)], capture_output=True, text=True)
    if r.returncode != 0:
        print(f"BUILD FAILED {src.relative_to(ROOT)}\n{r.stderr}")
        return None
    try:
        inputs = {p: input_digest(p) for p in depfile_inputs(dep.read_text(encoding="utf-8"))}
        dep.unlink()
    except OSError:
        inputs = {}
    if inputs and all(d is not None for d in inputs.values()):
        write_json(inputs_path(obj), {"cache": CACHE_VERSION, "cmd": cmd, "object": digest(obj), "inputs": inputs})
    return obj


def write_json(path: Path, data) -> None:
    """Write a cache file atomically, so an interrupted run never leaves a half-written one."""
    tmp = path.with_name(path.name + f".{os.getpid()}.tmp")
    tmp.write_text(json.dumps(data), encoding="utf-8")
    os.replace(tmp, path)


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


_labels: asm_labels.Labels | None = None


def labels() -> asm_labels.Labels:
    """The special members' addresses the hand-written declarations name (tools/dread/asm_labels.py),
    read once per process."""
    global _labels
    if _labels is None:
        _labels = asm_labels.Labels(ROOT)
    return _labels


def with_labels(resolve, full_names: dict[str, str], source: str | None):
    """resolve, then for a constructor or destructor the address its declaration names: Clang 9 ignores
    an asm label on one, so the object has only the mangled name."""
    def resolve2(sym: str) -> int | None:
        hit = resolve(sym)
        if hit is None and (asm_labels.is_special(sym) or asm_labels.is_vtable(sym)):
            hit, _ = labels().lookup(sym, full_names.get(sym, sym), source)
        return hit
    return resolve2


def diff_with_targets(functions: list[asmdiff.ObjFunction], original: asmdiff.Original,
                      by_name: dict[str, list[int]], full_names: dict[str, str] | None = None,
                      source: str | None = None):
    """Diff an object's functions and build the resolver for their relocation targets.

    Returns (diffs, names, full_names, targets): names maps each function symbol to its
    qualified demangled name, full_names every symbol (functions and relocation targets) to
    its full demangled form, targets(symbol) a relocation's symbol to an original address or None.
    full_names may be given already demangled (check.py demangles many objects in one call).
    `source` (the object's source, a repository path) is where a constructor's or destructor's
    declaration is looked for first, before the headers (tools/dread/asm_labels.py).
    """
    if full_names is None:
        full_names = demangle(object_symbols(functions), strip=False)
    names = {f.symbol: full_names.get(f.symbol, f.symbol).split("(")[0] for f in functions}
    # A constructor or destructor defined under its mangled name maps through its declaration's label, unless
    # the object already maps that address (an fn_<addr> alias of it, or its other variant, C1/C2 or D1/D2).
    claimed = {int(m.group(1), 16) for f in functions if (m := asmdiff.NAME_RE.search(f.symbol))}
    by_label = with_labels(resolver(by_name, names), full_names, source)

    def define(sym: str) -> int | None:
        hit = by_label(sym)
        if hit is not None and asm_labels.is_special(sym):
            if hit in claimed:
                return None
            claimed.add(hit)
        return hit
    diffs = asmdiff.diff_functions(functions, original, define)
    targets = with_labels(target_resolver(by_name, full_names, {res.symbol: res.address for res in diffs}),
                          full_names, source)
    return diffs, names, full_names, targets


def special_mismatches(res: asmdiff.FunctionResult, original: asmdiff.Original, targets, full_names: dict[str, str],
                       source: str | None, defined: set[str] = frozenset()) -> list[asmdiff.RelocMismatch]:
    """A matching function's calls to a constructor or destructor that the object does not define and
    whose address nothing names (no declaration in reach says it, and it is not an import): their targets
    would go unchecked, so each is a mismatch of kind `special`. (One the object defines inline is checked
    when it maps to an address, like any other function of the object.)"""
    out = []
    orig = original.function(res.address)
    for off, r in sorted(res.relocs.items()):
        if r.section or r.type not in (asmdiff.CALL26, asmdiff.JUMP26) or not asm_labels.is_special(r.symbol):
            continue
        if r.symbol in defined:
            continue
        if targets(r.symbol) is not None:
            continue
        actual = asmdiff.branch_target(int.from_bytes(orig[off:off + 4], "little"), res.address + off)
        if original.plt_import(actual) is not None:
            continue
        _, why = labels().lookup(r.symbol, full_names.get(r.symbol, r.symbol), source)
        name = asmdiff.strip_params(full_names.get(r.symbol, r.symbol))
        out.append(asmdiff.RelocMismatch(off, "special", r.symbol, r.addend, None, actual,
                                         detail=f"{name}: {why}"))
    return out


def object_symbols(functions: list[asmdiff.ObjFunction]) -> list[str]:
    """The symbols diff_with_targets demangles: every function and named relocation target."""
    symbols = {f.symbol for f in functions}
    symbols.update(r.symbol for f in functions for r in f.relocs.values() if not r.section)
    return sorted(symbols)


def reloc_line(addr: str, m: asmdiff.RelocMismatch, full_names: dict[str, str], src: Path) -> str:
    sym = strip_params(full_names.get(m.symbol, m.symbol))
    return f"RELOC  {addr}  {asmdiff.format_mismatch(m, sym)}  {src.relative_to(ROOT).as_posix()}"


def diff_entries(src: Path, functions: list[asmdiff.ObjFunction], full_names: dict[str, str],
                 original: asmdiff.Original, by_name: dict[str, list[int]], no_reloc: bool) -> list[dict]:
    """One object's diff as plain data (what <obj>.diff.json caches): per function its symbol,
    qualified name, address, score, whether it matches, and for a match its relocation report
    and RELOC lines. Statuses are not part of it (main derives them with the annotations)."""
    source = src.resolve().relative_to(ROOT).as_posix() if src.resolve().is_relative_to(ROOT) else None
    diffs, names, full_names, targets = diff_with_targets(functions, original, by_name, full_names, source)
    defined = {f.symbol for f in functions}
    out = []
    for res in diffs:
        e = {"symbol": res.symbol, "name": names.get(res.symbol, res.symbol), "address": res.address,
             "matching": res.matching, "score": res.score, "checked": {}, "unchecked": {},
             "mismatches": [], "reloc_lines": []}
        if res.matching and not no_reloc:
            rep = asmdiff.check_relocs(res, original, targets)
            special = special_mismatches(res, original, targets, full_names, source, defined)
            if special:
                rep.unchecked["unresolved"] -= len(special)
                rep.mismatches.extend(special)
            e["checked"], e["unchecked"] = dict(rep.checked), dict(rep.unchecked)
            e["mismatches"] = [dataclasses.asdict(m) for m in rep.mismatches]
            e["reloc_lines"] = [reloc_line(f"{res.address:#x}", m, full_names, src) for m in rep.mismatches]
        out.append(e)
    return out


def diff_inputs(version: str, rows: dict[str, dict], no_reloc: bool) -> str:
    """Digest of everything an object's diff depends on besides the object itself."""
    names = hashlib.sha256("\n".join(f"{a},{r['name']}" for a, r in sorted(rows.items())).encode()).hexdigest()
    tools = Path(__file__).resolve().parent
    parts = [CACHE_VERSION, digest(Path(__file__).resolve()), digest(tools / "asmdiff.py"),
             digest(ROOT / "orig" / version / "exefs" / "main"),
             digest(ROOT / "build" / "recon" / version / "main_eh_functions.json"),
             names, asmdiff.cxxfilt(), no_reloc, digest(Path(asm_labels.__file__)), labels().digest()]
    return hashlib.sha256(json.dumps(parts).encode()).hexdigest()


def diff_path(obj: Path) -> Path:
    return obj.with_name(obj.name + ".diff.json")


def cached_diff(obj: Path, obj_digest: str | None, inputs: str) -> list[dict] | None:
    try:
        c = json.loads(diff_path(obj).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return None
    if obj_digest is None or c.get("object") != obj_digest or c.get("inputs") != inputs:
        return None
    return c.get("entries")


def diff_all(pairs: list[tuple[Path, Path]], original: asmdiff.Original, by_name: dict[str, list[int]],
             inputs: str, no_reloc: bool, cache: bool) -> tuple[dict[Path, list[dict]], int]:
    """diff_entries for every (source, object), reusing cached diffs; the rest are diffed in
    chunks with one llvm-cxxfilt call each. Returns (source -> entries, number reused)."""
    out: dict[Path, list[dict]] = {}
    pending = []
    for src, obj in pairs:
        d = digest(obj)
        hit = cached_diff(obj, d, inputs) if cache else None
        if hit is not None:
            out[src] = hit
        else:
            pending.append((src, obj, d))
    reused = len(out)
    for i in range(0, len(pending), DIFF_CHUNK):
        chunk = [(src, obj, d, asmdiff.read_object(obj)) for src, obj, d in pending[i:i + DIFF_CHUNK]]
        full_names = demangle(sorted({s for *_, fns in chunk for s in object_symbols(fns)}), strip=False)
        for src, obj, d, fns in chunk:
            out[src] = diff_entries(src, fns, full_names, original, by_name, no_reloc)
            if d is not None:
                write_json(diff_path(obj), {"object": d, "inputs": inputs, "entries": out[src]})
    return out, reused


def compile_all(version: str, srcs: list[Path], jobs: int, cache: bool) -> list[Path | None]:
    from concurrent.futures import ThreadPoolExecutor
    with ThreadPoolExecutor(jobs) as pool:
        return list(pool.map(lambda s: build(version, s, cache), srcs))


def compile_only(version: str, jobs: int, cache: bool = True) -> int:
    """Build every source and report failures; nothing is read from orig/ or written to data/."""
    srcs = sources()
    objects = compile_all(version, srcs, jobs, cache)
    failed = sum(1 for o in objects if o is None)
    print(f"{len(srcs)} sources compiled, {failed} failed")
    return 1 if failed else 0


def wrong_references(original: asmdiff.Original, by_name: dict[str, list[int]],
                     where: dict[str, list[tuple[Path, str]]], addresses: set[str],
                     skip: set[str] = frozenset(), notes: dict[str, str] | None = None) -> list[tuple[str, str, str]]:
    """[(address, function, finding)] for the failing findings of tools/ref_check.py in the non-matching
    functions at `addresses` (`skip`: functions that match in bytes and were demoted for a RELOC line, which
    says the same thing already). `notes` maps the address of a NON_MATCHING(minor) function to its note:
    there a callee or global the original does not have fails unless the note names it."""
    notes = notes or {}
    objects = sorted({obj for a in addresses - skip for obj, _ in where.get(a, [])})
    if not objects:
        return []
    rodata, got = ref_check.ranges(original)
    out = []
    for obj in objects:
        for row in ref_check.object_findings(obj, original, by_name, rodata, got):
            if row["address"] in addresses:
                out += [(row["address"], row["function"], f["text"])
                        for f in ref_check.failing(row, notes.get(row["address"]))]
    return sorted(set(out))


def stale_verdicts(table: dict[int, dict], statuses: dict[str, str], where: dict[str, list[tuple[Path, str]]],
                   digest_of=None) -> list[tuple[str, str]]:
    """[(address, why)] for the `pass` rows of data/equivalence.csv that no longer describe the tree.
    `statuses` maps an address to its status in src/, `where` to the (object, symbol) pairs defining it;
    `digest_of(object, symbol)` returns equiv_test.digest of that function (the objects are read by default)."""
    objects: dict[Path, dict[str, str]] = {}

    def read(obj: Path, symbol: str) -> str | None:
        if obj not in objects:
            objects[obj] = {f.symbol: equiv_test.digest(f.code, f.relocs) for f in asmdiff.read_object(obj)}
        return objects[obj].get(symbol)

    digest_of = digest_of or read
    out = []
    for a in sorted(table):
        row = table[a]
        if row["verdict"] != "pass":
            continue
        addr = f"{a:#x}"
        status = statuses.get(addr)
        if status != "m":
            out.append((addr, "no function of src/ is defined at it" if status is None else
                        f"its status is {status}, not m: the verdict belongs to other code"))
            continue
        if row["rules"] != str(equiv_test.RULES):
            out.append((addr, f"judged under rules {row['rules']}, the test is at {equiv_test.RULES}"))
            continue
        digests = [digest_of(obj, symbol) for obj, symbol in where.get(addr, [])]
        if not any(d is not None and equiv_test.current(row, d) for d in digests):
            out.append((addr, "its code, or what it refers to, changed since it was judged"))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="dump version to diff against (required unless --compile-only)")
    ap.add_argument("--update", action="store_true", help="write statuses into data/functions/")
    ap.add_argument("-j", "--jobs", type=int, default=8, help="parallel compiles")
    ap.add_argument("--no-reloc", action="store_true", help="skip the relocation target check")
    ap.add_argument("--compile-only", action="store_true",
                    help="only compile every source with the project toolchain; needs no dump (hosted CI)")
    ap.add_argument("--no-cache", action="store_true",
                    help="rebuild and rediff every source, ignoring (and refreshing) the caches")
    args = ap.parse_args()
    cache = not args.no_cache
    if args.compile_only:
        return compile_only(args.version or "compile-only", args.jobs, cache)
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
    objects = compile_all(args.version, srcs, args.jobs, cache)
    failed = any(o is None for o in objects)
    pairs = [(src, obj) for src, obj in zip(srcs, objects) if obj is not None]
    entries, reused = diff_all(pairs, original, by_name, diff_inputs(args.version, rows, args.no_reloc),
                               args.no_reloc, cache)
    where: dict[str, list[tuple[Path, str]]] = {}  # address -> (object, symbol) of every definition
    note_of: dict[str, str] = {}  # address of a NON_MATCHING(minor) function -> its note
    for src, obj in pairs:
        notes = annotations(src)
        texts = minor_notes(src) if "m" in notes.values() else {}
        report_only = src.relative_to(ROOT / "src").parts[0] in REPORT_ONLY
        for e in entries[src]:
            short = e["name"].split("::")[-1]
            status = "O" if e["matching"] else notes.get(short, "W")
            addr = f"{e['address']:#x}"
            if e["matching"] and not args.no_reloc:
                report.merge(asmdiff.RelocReport(Counter(e["checked"]), Counter(e["unchecked"]),
                                                 [asmdiff.RelocMismatch(**m) for m in e["mismatches"]]))
                if e["mismatches"]:
                    if report_only:
                        report_only_addrs.add(addr)
                    else:
                        status = notes.get(short, "W")
                        demoted.add(addr)
                    reloc_lines.setdefault(addr, []).extend(e["reloc_lines"])
            results[addr] = (status, e["name"], e["score"])
            if status == "m" and short in texts:
                note_of[addr] = texts[short]
            defined_in.setdefault(addr, set()).add(src)
            where.setdefault(addr, []).append((obj, e["symbol"]))
    print(f"cache: {len(pairs) - reused} of {len(pairs)} objects diffed, {reused} reused")

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
    # An O function no source defines any more is lost too, not only one that stopped matching: most often a
    # name the sources give it that data/functions/ does not carry (a rename outside tools/rename.py), so its
    # definition maps to no address. Not counted when a compile failed (its functions are missing then).
    if not failed:
        for addr, row in sorted(rows.items()):
            if row.get("status") == "O" and addr not in results:
                print(f"U  {addr}  {'':>6}  {row['name']}  REGRESSION (was O, no source defines it: a name "
                      "check.py cannot resolve? tools/rename.py)")
                regressions += 1
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

    refs = wrong_references(original, by_name, where, {a for a, v in results.items() if v[0] != "O"}, demoted,
                            note_of)
    for addr, name, text in refs:
        print(f"REFS  {addr}  {name}: {text}")
    if refs:
        print(f"{len(refs)} wrong addresses in non-matching functions (`python tools/ref_check.py --version "
              f"{args.version} <file>` shows each function's whole comparison)")

    stale = stale_verdicts(equiv_test.read_table(), {a: v[0] for a, v in results.items()}, where)
    for addr, why in stale:
        print(f"EQUIV  {addr}  {why}")
    if stale:
        print(f"{len(stale)} rows of data/equivalence.csv do not describe the tree: judge those functions again "
              "(tools/equiv_test.py --write <file>) or take the rows out")

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
    return 1 if failed or regressions or duplicates or stale or refs else 0


if __name__ == "__main__":
    sys.exit(main())
