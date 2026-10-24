#!/usr/bin/env python3
"""Compile every source under src/ for a PC target (compile only) and report what does not port.

The port (docs/PORT_PLAN.md, milestone M0) builds the matched and generated sources for x86-64
with a host Clang. This sweep compiles each src/**/*.cpp on its own with the project's include
setup (tools/buildcfg.py: the libc++ 9.0.1 and musl headers, include/) but the host's code
generation flags, never the AArch64 ones, and sorts the diagnostics into categories: an error
message with its file, line and quoted names stripped, so the same problem in a hundred generated
files is one category with a hundred files. lib/nnsdk is not on the include path: the port must
not depend on it, so every `<nn/...>` header that src/ includes is a declaration the port's own
runtime has to provide (port/runtime/include). Those files are listed separately, and with
port/runtime/include present (the default) its declarations are used; --no-runtime shows the raw list.

With llvm-nm (next to the compiler, or on PATH) the compiled objects are read back for definitions:
a strong symbol defined in more than one object (a link error) and a `fn_<addr>` function defined in
more than one object under different signatures (two copies of one function, which no linker
notices). tools/check.py reports duplicates too, but exempts the Phase 3 sample directories from
that rule; the port build's per-file exclusions (port/CMakeLists.txt) are drawn from this list.

Objects go to build/port/sweep/, the report to stdout and build/port/sweep.json. The exit status is
always 0: this is a report, not a check. Standard library only (it imports tools/buildcfg.py for the
include paths).

Usage:
  python tools/port_sweep.py --version 2.1.0                        # host clang++, x86_64-linux-gnu
  python tools/port_sweep.py --version 2.1.0 --jobs 16 --json build/port/sweep.json
  python tools/port_sweep.py --version 2.1.0 --only '^src/game_s7/'  # a subset while fixing one category
  python tools/port_sweep.py --version 2.1.0 --no-runtime            # without port/runtime/include
  python tools/port_sweep.py --version 2.1.0 --system-stdlib         # the host's own libc++/libc (Linux, macOS)
"""

from __future__ import annotations

import argparse
import json
import os
import re
import shutil
import subprocess
import sys
from collections import Counter, defaultdict
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import buildcfg  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
RUNTIME_INCLUDE = ROOT / "port" / "runtime" / "include"
# Host code generation: the same language flags as the matching build (tools/buildcfg.CXX), a
# plain optimisation level, and none of the AArch64 flags.
HOST_FLAGS = ["-std=c++1z", "-O2", "-fno-rtti", "-fno-exceptions", "-fPIC", "-c"]
# Diagnostics as one line each, errors only; warnings are the port build's business later.
DIAG_FLAGS = ["-w", "-fno-caret-diagnostics", "-fno-color-diagnostics", "-fno-diagnostics-show-option",
              "-fno-diagnostics-fixit-info", "-fno-show-column"]
_WIN_CLANGXX = r"C:\Program Files\LLVM\bin\clang++.exe"

DIAGNOSTIC = re.compile(r"^(?P<file>.+?):(?P<line>\d+):(?:\d+:)? (?P<kind>fatal error|error|warning|note): (?P<msg>.*)$")
SDK_HEADER = re.compile(r"'(nn/[^']+)' file not found")
HEADER = re.compile(r"'([^']+)' file not found")
STATIC_ASSERT = re.compile(r'static_assert failed(?: due to requirement \'.*?\')?(?: "(.*)")?')
TOO_MANY = "too many errors emitted"
# A function named after its address, in the last component of a demangled symbol's own name
# (fn_<addr>, or Unk_fn_<addr> for a function the sample sources write as a constructor).
FN_ADDR = re.compile(r"fn_(7[01][0-9a-f]{8})\b")
NM_BATCH = 150  # objects per llvm-nm call (Windows command lines are limited to 32 K characters)


def host_cxx() -> str:
    """The host C++ compiler: $CXX, the Windows LLVM install, then clang++ on PATH."""
    env = os.environ.get("CXX")
    if env:
        return env
    if sys.platform == "win32" and Path(_WIN_CLANGXX).is_file():
        return _WIN_CLANGXX
    return shutil.which("clang++") or "clang++"


def compiler_version(cxx: str) -> str:
    try:
        out = subprocess.run([cxx, "--version"], capture_output=True, text=True, check=True).stdout
        return out.splitlines()[0].strip() if out else cxx
    except (OSError, subprocess.CalledProcessError):
        return cxx


def default_target(cxx: str) -> str:
    """x86_64-linux-gnu on Windows (no Windows SDK is needed with the musl headers); the
    compiler's own target elsewhere, where the host's standard library is the one that links."""
    if sys.platform == "win32":
        return "x86_64-linux-gnu"
    try:
        out = subprocess.run([cxx, "-dumpmachine"], capture_output=True, text=True, check=True).stdout.strip()
        return out or "x86_64-linux-gnu"
    except (OSError, subprocess.CalledProcessError):
        return "x86_64-linux-gnu"


def project_includes() -> list[str]:
    """buildcfg.project_includes() without lib/nnsdk (the port must not depend on it) and without
    the Switch target macros."""
    out: list[str] = []
    args = buildcfg.project_includes()
    i = 0
    while i < len(args):
        a = args[i]
        if a == "-I":
            path = args[i + 1]
            if "nnsdk" not in Path(path).parts:
                out += [a, path]
            i += 2
            continue
        if a in ("-DSWITCH", "-DNNSDK"):
            i += 1
            continue
        out.append(a)
        i += 1
    return out


def sources(only: list[str]) -> list[Path]:
    srcs = sorted((ROOT / "src").rglob("*.cpp"))
    if only:
        pats = [re.compile(p) for p in only]
        srcs = [s for s in srcs if any(p.search(rel(s)) for p in pats)]
    return srcs


def rel(path: Path | str) -> str:
    p = Path(path)
    try:
        return p.resolve().relative_to(ROOT).as_posix()
    except ValueError:
        return p.as_posix()


def normalise(msg: str) -> str:
    """Group a Clang error message: quoted names and numbers stripped, except that a missing
    header keeps its name (which header is the point) and a static_assert keeps its text."""
    m = SDK_HEADER.search(msg)
    if m:
        return "sdk header not found"
    m = HEADER.search(msg)
    if m:
        return f"header not found '{m.group(1)}'"
    m = STATIC_ASSERT.search(msg)
    if m:
        return "static_assert failed" + (f' "{m.group(1)}"' if m.group(1) else "")
    out = re.sub(r"'[^']*'", "'…'", msg)
    out = re.sub(r'"[^"]*"', '"…"', out)
    out = re.sub(r"\b0x[0-9a-fA-F]+\b|\b\d+\b", "N", out)
    return out


def obj_path(src: Path, obj_dir: Path) -> Path:
    return obj_dir / src.relative_to(ROOT / "src").with_suffix(".o")


def compile_one(cxx: str, flags: list[str], src: Path, obj_dir: Path) -> tuple[Path, int, str]:
    obj = obj_path(src, obj_dir)
    obj.parent.mkdir(parents=True, exist_ok=True)
    cmd = [cxx, *flags, rel(src), "-o", str(obj)]
    try:
        r = subprocess.run(cmd, capture_output=True, text=True, encoding="utf-8", errors="replace", cwd=ROOT)
    except OSError as e:
        return src, 1, f"{rel(src)}:1: fatal error: cannot run the compiler: {e}"
    return src, r.returncode, r.stderr


def parse(stderr: str) -> tuple[list[tuple[str, int, str]], bool]:
    """(file, line, message) for each error, and whether Clang stopped early."""
    errors: list[tuple[str, int, str]] = []
    truncated = False
    for line in stderr.splitlines():
        m = DIAGNOSTIC.match(line.strip())
        if not m or m.group("kind") not in ("error", "fatal error"):
            continue
        if TOO_MANY in m.group("msg"):
            truncated = True
            continue
        errors.append((rel(m.group("file")), int(m.group("line")), m.group("msg").strip()))
    return errors, truncated


def find_nm(cxx: str) -> str | None:
    """llvm-nm next to the compiler, else on PATH; None when there is none."""
    exe = Path(cxx)
    for name in ("llvm-nm" + exe.suffix, "llvm-nm"):
        cand = exe.parent / name
        if cand.is_file():
            return str(cand)
    return shutil.which("llvm-nm")


def own_name(demangled: str) -> str:
    """A demangled symbol's qualified name without parameter list or template arguments, so a
    function address inside a template argument (`UnkOutOfLineElement<48u, &fn_710007ecd4>`) is
    not taken for the function's own name. `fn_71000e027c(base::UnkVector<...>&&)` gives
    `fn_71000e027c`; `Unk_fn_71005a2dd8::Unk_fn_71005a2dd8()` keeps both components."""
    out: list[str] = []
    depth = 0
    for ch in demangled.replace("(anonymous namespace)::", ""):
        if ch == "<":
            depth += 1
        elif ch == ">":
            depth = max(0, depth - 1)
        elif depth == 0:
            if ch == "(":
                break
            out.append(ch)
    return "".join(out).strip()


def definitions(nm: str, objects: list[tuple[Path, Path]]) -> dict:
    """Read the compiled objects back with llvm-nm: strong global symbols defined in more than
    one object, and fn_<addr> functions defined in more than one object under any symbol
    (the same one, a link error; different ones, two copies of one function)."""
    by_obj = {str(obj): src for src, obj in objects}
    symbol_objs: dict[str, set[str]] = defaultdict(set)
    func_objs: dict[str, dict[str, set[str]]] = defaultdict(lambda: defaultdict(set))
    paths = sorted(by_obj)
    for i in range(0, len(paths), NM_BATCH):
        batch = paths[i:i + NM_BATCH]
        r = subprocess.run([nm, "--defined-only", "--demangle", *batch],
                           capture_output=True, text=True, encoding="utf-8", errors="replace")
        current = batch[0] if len(batch) == 1 else None  # one input: no file header lines
        for line in r.stdout.splitlines():
            if line.endswith(":") and line[:-1] in by_obj:
                current = line[:-1]
                continue
            parts = line.split(None, 2)
            if len(parts) < 3 or current is None:
                continue
            _value, kind, sym = parts
            if kind in ("T", "D", "B", "R"):
                symbol_objs[sym].add(current)
            if kind in ("T", "t"):
                last = own_name(sym).rsplit("::", 1)[-1].lstrip("~")
                m = FN_ADDR.search(last)
                if m:
                    func_objs[m.group(1)][current].add(sym)
    collisions = [{"symbol": sym, "sources": sorted(rel(by_obj[o]) for o in objs)}
                  for sym, objs in sorted(symbol_objs.items()) if len(objs) > 1]
    copies = [{"function": f"fn_{addr}",
               "definitions": {rel(by_obj[o]): sorted(syms)
                               for o, syms in sorted(per.items(), key=lambda kv: rel(by_obj[kv[0]]))}}
              for addr, per in sorted(func_objs.items()) if len(per) > 1]
    return {"tool": nm, "objects": len(paths), "symbol_collisions": collisions, "function_copies": copies}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True, help="game version the sources were matched against (recorded)")
    ap.add_argument("--target", default=None,
                    help="Clang target triple (default: x86_64-linux-gnu on Windows, the compiler's own elsewhere)")
    ap.add_argument("--cxx", default=None, help="host clang++ (default: $CXX, the LLVM install, PATH)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4, help="parallel compiles")
    ap.add_argument("--json", default=str(ROOT / "build" / "port" / "sweep.json"), help="JSON report path")
    ap.add_argument("--only", action="append", default=[], help="regex on the source path (repeatable)")
    ap.add_argument("--no-runtime", action="store_true", help="leave port/runtime/include off the include path")
    ap.add_argument("--system-stdlib", action="store_true",
                    help="use the host's standard library headers instead of the project's libc++ 9.0.1 + musl")
    ap.add_argument("--error-limit", type=int, default=50, help="errors Clang reports per file (default 50)")
    ap.add_argument("--examples", type=int, default=1, help="examples printed per category (default 1)")
    args = ap.parse_args()

    cxx = args.cxx or host_cxx()
    args.target = args.target or default_target(cxx)
    flags = [f"--target={args.target}", *HOST_FLAGS, *DIAG_FLAGS, f"-ferror-limit={args.error_limit}"]
    if not args.system_stdlib:
        flags += buildcfg.system_includes()
    flags += project_includes()
    runtime = None
    if not args.no_runtime and RUNTIME_INCLUDE.is_dir():
        runtime = RUNTIME_INCLUDE
        flags += ["-I", str(runtime)]
    srcs = sources(args.only)
    obj_dir = ROOT / "build" / "port" / "sweep"
    version = compiler_version(cxx)
    print(f"port sweep: {len(srcs)} sources, target {args.target}, {version}, {args.jobs} jobs"
          + (f", runtime {rel(runtime)}" if runtime else ", no runtime include"), flush=True)

    with ThreadPoolExecutor(max_workers=max(1, args.jobs)) as pool:
        results = list(pool.map(lambda s: compile_one(cxx, flags, s, obj_dir), srcs))

    failed_files: dict[str, dict] = {}
    cat_count: Counter[str] = Counter()
    cat_files: dict[str, set[str]] = defaultdict(set)
    cat_examples: dict[str, list[dict]] = defaultdict(list)
    sdk_headers: Counter[str] = Counter()
    per_dir: dict[str, dict[str, int]] = defaultdict(lambda: {"total": 0, "failed": 0})
    for src, code, stderr in results:
        name = rel(src)
        top = "src/" + src.relative_to(ROOT / "src").parts[0]
        per_dir[top]["total"] += 1
        if code == 0:
            continue
        per_dir[top]["failed"] += 1
        errors, truncated = parse(stderr)
        if not errors:
            errors = [(name, 0, f"compiler exited with status {code} and no error line")]
        cats: Counter[str] = Counter()
        for file, line, msg in errors:
            cat = normalise(msg)
            cats[cat] += 1
            cat_count[cat] += 1
            cat_files[cat].add(name)
            if len(cat_examples[cat]) < args.examples:
                cat_examples[cat].append({"file": file, "line": line, "message": msg, "source": name})
            m = SDK_HEADER.search(msg)
            if m:
                sdk_headers[m.group(1)] += 1
        failed_files[name] = {"categories": dict(sorted(cats.items())), "truncated": truncated}

    sdk_only = sorted(f for f, info in failed_files.items() if set(info["categories"]) == {"sdk header not found"})
    compiled = len(srcs) - len(failed_files)
    order = sorted(cat_count, key=lambda c: (-cat_count[c], -len(cat_files[c]), c))

    print(f"files: {len(srcs)} compiled {compiled}, failed {len(failed_files)}")
    print(f"categories: {len(order)} (errors, files, normalised message; e.g. = one example)")
    for cat in order:
        print(f"  {cat_count[cat]:6d} {len(cat_files[cat]):5d}  {cat}")
        for ex in cat_examples[cat]:
            print(f"                e.g. {ex['file']}:{ex['line']}: {ex['message']}")
    print("per directory (failed/total):")
    for top in sorted(per_dir):
        d = per_dir[top]
        print(f"  {top:28s} {d['failed']:5d} / {d['total']}")
    if sdk_headers:
        print("sdk headers src/ includes (the port's own runtime must declare these):")
        for h, n in sorted(sdk_headers.items(), key=lambda kv: (-kv[1], kv[0])):
            print(f"  {n:5d}  <{h}>")
    print(f"files failing only on sdk headers: {len(sdk_only)}")
    for f in sdk_only:
        print(f"  {f}")
    truncated = sorted(f for f, info in failed_files.items() if info["truncated"])
    if truncated:
        print(f"files where Clang stopped at {args.error_limit} errors (counts are lower bounds): {len(truncated)}")

    nm = find_nm(cxx)
    defs = None
    if nm:
        defs = definitions(nm, [(src, obj_path(src, obj_dir)) for src, code, _ in results if code == 0])
        print(f"definitions ({len(defs['symbol_collisions'])} symbols and {len(defs['function_copies'])} functions"
              f" defined in more than one of the {defs['objects']} objects; llvm-nm):")
        for c in defs["symbol_collisions"]:
            print(f"  symbol   {c['symbol']}  " + "  ".join(c["sources"]))
        for c in defs["function_copies"]:
            print(f"  function {c['function']}  "
                  + "  ".join(f"{f} [{'; '.join(syms)}]" for f, syms in c["definitions"].items()))
    else:
        print("definitions: not checked (no llvm-nm next to the compiler or on PATH)")

    report = {
        "version": args.version,
        "target": args.target,
        "compiler": version,
        "flags": flags,
        "runtime_include": rel(runtime) if runtime else None,
        "files": {"total": len(srcs), "compiled": compiled, "failed": len(failed_files)},
        "categories": [{"category": c, "errors": cat_count[c], "files": len(cat_files[c]),
                        "examples": cat_examples[c]} for c in order],
        "per_directory": {k: per_dir[k] for k in sorted(per_dir)},
        "sdk_headers": dict(sorted(sdk_headers.items())),
        "sdk_only_files": sdk_only,
        "failed_files": {k: failed_files[k] for k in sorted(failed_files)},
        "definitions": defs,
    }
    out = Path(args.json)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"json: {rel(out)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
