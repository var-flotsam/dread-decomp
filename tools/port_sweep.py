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

Windows (compile only): --target x86_64-w64-mingw32 is the Windows target with the Itanium C++ ABI
(the game's vtables are used as they are, so not MSVC). It uses llvm-mingw's clang++
(tools/fetch_toolchains.py llvm-mingw-20261006) unless --cxx names another, and by default libc++
9.0.1's headers over MinGW-w64's C headers (--stdlib libcxx9, as the Linux build does over glibc);
--stdlib host is llvm-mingw's own libc++. Objects and the report go to build/port/windows/ unless
--objects and --json say otherwise. What it measures is LLP64 (`long` is 32-bit), a 16-bit
`wchar_t`, and the MinGW-w64 headers; the Win64 calling convention changes no source.

Usage:
  python tools/port_sweep.py --version 2.1.0                        # host clang++, x86_64-linux-gnu
  python tools/port_sweep.py --version 2.1.0 --jobs 16 --json build/port/sweep.json
  python tools/port_sweep.py --version 2.1.0 --only '^src/game_s7/'  # a subset while fixing one category
  python tools/port_sweep.py --version 2.1.0 --no-runtime            # without port/runtime/include
  python tools/port_sweep.py --version 2.1.0 --system-stdlib         # the host's own libc++/libc (Linux, macOS)
  python tools/port_sweep.py --version 2.1.0 --target x86_64-w64-mingw32   # Windows, compile only (llvm-mingw)
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
import fetch_toolchains  # noqa: E402
import hostenv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
RUNTIME_INCLUDE = ROOT / "port" / "runtime" / "include"
# Host code generation: the same language flags as the matching build (tools/buildcfg.CXX), a
# plain optimisation level, and none of the AArch64 flags. Without type-based alias analysis: the
# game code type-puns as the original did (words read as u64, storage cast to its layout), which
# the original's compiler tolerated at the bytes it produced but a newer one may not; port/CMakeLists.txt
# compiles dread_game the same way.
HOST_FLAGS = ["-std=c++1z", "-O2", "-fno-strict-aliasing", "-fno-rtti", "-fno-exceptions", "-fPIC", "-c"]
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


def is_mingw(target: str) -> bool:
    """A Windows target with the Itanium C++ ABI (x86_64-w64-mingw32, x86_64-w64-windows-gnu)."""
    return "mingw" in target or target.endswith("windows-gnu")


def mingw_cxx() -> str:
    """llvm-mingw's clang++ (tools/fetch_toolchains.py), else x86_64-w64-mingw32-clang++ on PATH."""
    cand = fetch_toolchains.llvm_mingw_root() / "bin" / hostenv.exe("clang++")
    if cand.is_file():
        return str(cand)
    found = shutil.which("x86_64-w64-mingw32-clang++")
    if found:
        return found
    raise SystemExit(f"{cand} not found: run tools/fetch_toolchains.py llvm-mingw-{fetch_toolchains.LLVM_MINGW_VERSION}"
                     " (or name a MinGW-w64 clang++ with --cxx)")


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


STDLIBS = ("project", "host", "libcxx9")


def stdlib_flags(mode: str, target: str = "") -> list[str]:
    """The standard library headers to compile against: "project", the matching build's libc++
    9.0.1 and musl (compile only: nothing links against musl); "host", the compiler's own; "libcxx9",
    libc++ 9.0.1's headers over the host's C library, for a build that links (the game's sources
    instantiate libc++ 9's internal std::__sort3 and its siblings, which later libc++ versions
    renamed, so a newer libc++'s headers do not compile them; the libc++ library itself keeps its
    ABI, so the host's links). libc++ 9.0.1 comes from tools/fetch_sources.py. On a MinGW target the
    C library is MinGW-w64's and libc++ 9 takes the Win32 thread API, as llvm-mingw's own libc++ is
    built."""
    if mode == "project":
        if is_mingw(target):
            raise SystemExit("--stdlib project is musl's headers, which do not describe Windows:"
                             " use --stdlib libcxx9 or host with a MinGW target")
        return buildcfg.system_includes()
    if mode == "libcxx9":
        include = buildcfg.libcxx_include()
        if not include.is_dir():
            raise SystemExit(f"{include} not found: run tools/fetch_sources.py libcxx-9.0.1 first")
        thread = "-D_LIBCPP_HAS_THREAD_API_WIN32" if is_mingw(target) else "-D_LIBCPP_HAS_THREAD_API_PTHREAD"
        return ["-nostdinc++", "-isystem", str(include), thread]
    return []


def stdlib_mode(args) -> str:
    """--stdlib, with --system-stdlib as the older spelling of --stdlib host; unset, project, or
    libcxx9 on a MinGW target."""
    if getattr(args, "system_stdlib", False):
        return "host"
    if args.stdlib:
        return args.stdlib
    return "libcxx9" if is_mingw(getattr(args, "target", None) or "") else "project"


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


# What compiles on Windows but means something else there (the MinGW target only): `long` and an
# `l`/`ul` literal are 32-bit under LLP64 (u64 and s64 from include/types.h are 64-bit everywhere),
# and wchar_t and the wide C functions are 16-bit (the game's wide text is 32-bit char32_t).
GENERATED = ("src/lifted/", "src/reflection/", "src/reflection_types/", "src/reflection_statics/",
             "src/clones/", "src/tables/")
LLP64_EXEMPT = ("include/types.h",)  # where u64 and s64 are defined from long
LLP64_CHECKS = (
    ("long", re.compile(r"\blong\b")),
    ("l literal", re.compile(r"(?<![.\w])(?:0[xX][0-9a-fA-F]+|\d+)(?:[uU][lL]|[lL][uU]?)\b")),
    ("wchar_t", re.compile(r"\bwchar_t\b|(?<!\w)L[\"']|\b(?:wcs\w+|sw(?:scanf|printf)|vsw\w+)\b")),
)
_LLP64_KEEP = re.compile(r"\blong\s+(?:long|double)\b")


def strip_code(text: str) -> list[str]:
    """The source's lines with comments and the contents of string and character literals blanked
    (a wide literal keeps its L prefix and quotes), so a scan sees only code."""
    out: list[str] = []
    in_block = False
    for line in text.splitlines():
        buf: list[str] = []
        i, n = 0, len(line)
        while i < n:
            if in_block:
                end = line.find("*/", i)
                if end < 0:
                    i = n
                    continue
                in_block, i = False, end + 2
                continue
            ch = line[i]
            if line.startswith("//", i):
                break
            if line.startswith("/*", i):
                in_block, i = True, i + 2
                continue
            if ch in "\"'":
                j = i + 1
                while j < n and line[j] != ch:
                    j += 2 if line[j] == "\\" else 1
                buf.append(ch + ch)
                i = j + 1
                continue
            buf.append(ch)
            i += 1
        out.append("".join(buf))
    return out


def llp64_scan() -> list[dict]:
    """Every place in src/ and include/ that LLP64_CHECKS flags, with its kind and whether the file
    is generated (a generator's to fix, never by hand)."""
    hits: list[dict] = []
    files = sorted([*(ROOT / "src").rglob("*.cpp"), *(ROOT / "src").rglob("*.h"), *(ROOT / "include").rglob("*.h")])
    for path in files:
        name = rel(path)
        if name in LLP64_EXEMPT:
            continue
        for no, line in enumerate(strip_code(path.read_text(encoding="utf-8", errors="replace")), 1):
            code = _LLP64_KEEP.sub("", line)
            for kind, pattern in LLP64_CHECKS:
                if pattern.search(code):
                    hits.append({"file": name, "line": no, "kind": kind, "generated": name.startswith(GENERATED),
                                 "text": line.strip()[:160]})
    return hits


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


def sibling_tool(cxx: str, name: str) -> str | None:
    """An LLVM tool next to the compiler, else on PATH."""
    exe = Path(cxx)
    cand = exe.parent / (name + exe.suffix)
    if cand.is_file():
        return str(cand)
    return shutil.which(name)


COFF_HEADER = re.compile(r"^(?P<path>.+):\s+file format coff")
COFF_SYM = re.compile(r"^\[\s*\d+\]\(sec\s+(?P<sec>-?\d+)\)\(fl 0x[0-9a-f]+\)\(ty\s+[0-9a-f]+\)"
                      r"\(scl\s+(?P<scl>\d+)\) \(nx (?P<nx>\d+)\) 0x[0-9a-f]+ (?P<name>.*)$")
COFF_AUX_SECTION = re.compile(r"^AUX scnlen .* comdat (?P<comdat>\d+)$")


def coff_comdat(objdump: str, paths: list[str]) -> dict[str, set[str]]:
    """Per COFF object, the external symbols defined in a COMDAT section: inline functions and their
    statics, template instantiations, MinGW's .refptr stubs. The linker keeps one copy of each, as
    an ELF linker does with weak definitions, but llvm-nm prints them as strong symbols."""
    out: dict[str, set[str]] = defaultdict(set)
    for i in range(0, len(paths), NM_BATCH):
        batch = paths[i:i + NM_BATCH]
        r = subprocess.run([objdump, "--syms", *batch], capture_output=True, text=True, encoding="utf-8",
                           errors="replace")
        current = None
        comdat_secs: set[str] = set()
        externals: list[tuple[str, str]] = []
        section_symbol, last_sec = False, ""

        def flush():
            if current is not None:
                out[current].update(name for sec, name in externals if sec in comdat_secs)

        for line in r.stdout.splitlines():
            m = COFF_HEADER.match(line)
            if m:
                flush()
                current, comdat_secs, externals, section_symbol = m.group("path"), set(), [], False
                continue
            m = COFF_SYM.match(line)
            if m:
                sec, scl, nx = m.group("sec"), m.group("scl"), m.group("nx")
                section_symbol = scl == "3" and nx == "1"  # a section's own symbol: its AUX record follows
                if scl == "2" and int(sec) > 0:
                    externals.append((sec, m.group("name")))
                if section_symbol:
                    last_sec = sec
                continue
            m = COFF_AUX_SECTION.match(line)
            if m and section_symbol:
                if m.group("comdat") != "0":
                    comdat_secs.add(last_sec)
                section_symbol = False
        flush()
    return out


def demangle(cxxfilt: str, names: set[str]) -> dict[str, str]:
    """Itanium-mangled names to their demangled form (llvm-cxxfilt reads one per line)."""
    order = sorted(names)
    if not order:
        return {}
    r = subprocess.run([cxxfilt], input="\n".join(order) + "\n", capture_output=True, text=True,
                       encoding="utf-8", errors="replace")
    lines = r.stdout.splitlines()
    if r.returncode != 0 or len(lines) != len(order):
        return {n: n for n in order}
    return dict(zip(order, lines))


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


def definitions(nm: str, objects: list[tuple[Path, Path]], coff: tuple[str, str] | None = None) -> dict:
    """Read the compiled objects back with llvm-nm: strong global symbols defined in more than
    one object, and fn_<addr> functions defined in more than one object under any symbol
    (the same one, a link error; different ones, two copies of one function). For COFF objects,
    coff is (llvm-objdump, llvm-cxxfilt): COMDAT definitions are left out (coff_comdat) and the
    names are demangled afterwards, since the COMDAT list is of mangled names."""
    by_obj = {str(obj): src for src, obj in objects}
    symbol_objs: dict[str, set[str]] = defaultdict(set)
    func_syms: dict[str, set[str]] = defaultdict(set)  # object -> T/t symbols, as nm printed them
    paths = sorted(by_obj)
    comdat = coff_comdat(coff[0], paths) if coff else {}
    for i in range(0, len(paths), NM_BATCH):
        batch = paths[i:i + NM_BATCH]
        r = subprocess.run([nm, "--defined-only", *([] if coff else ["--demangle"]), *batch],
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
            if sym in comdat.get(current, ()):
                continue
            if kind in ("T", "D", "B", "R"):
                symbol_objs[sym].add(current)
            if kind in ("T", "t") and (not coff or "fn_7" in sym):
                func_syms[current].add(sym)
    if coff:
        names = demangle(coff[1], {s for s, objs in symbol_objs.items() if len(objs) > 1}
                         | set().union(*func_syms.values()))
        symbol_objs = {names[s]: objs for s, objs in symbol_objs.items() if len(objs) > 1}
        func_syms = {o: {names[s] for s in syms} for o, syms in func_syms.items()}
    func_objs: dict[str, dict[str, set[str]]] = defaultdict(lambda: defaultdict(set))
    for obj, syms in func_syms.items():
        for sym in syms:
            last = own_name(sym).rsplit("::", 1)[-1].lstrip("~")
            m = FN_ADDR.search(last)
            if m:
                func_objs[m.group(1)][obj].add(sym)
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
                    help="Clang target triple (default: x86_64-linux-gnu on Windows, the compiler's own elsewhere;"
                         " x86_64-w64-mingw32 is Windows, compile only, with llvm-mingw)")
    ap.add_argument("--cxx", default=None, help="host clang++ (default: $CXX, the LLVM install, PATH)")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4, help="parallel compiles")
    ap.add_argument("--json", default=None,
                    help="JSON report path (default build/port/sweep.json; build/port/windows/sweep.json for MinGW)")
    ap.add_argument("--objects", default=None,
                    help="object directory (default build/port/sweep, build/port/windows/sweep for MinGW;"
                         " the Linux build in Docker uses its own)")
    ap.add_argument("--only", action="append", default=[], help="regex on the source path (repeatable)")
    ap.add_argument("--no-runtime", action="store_true", help="leave port/runtime/include off the include path")
    ap.add_argument("--stdlib", choices=STDLIBS, default=None,
                    help="standard library headers: project (libc++ 9.0.1 + musl, the default), host, or libcxx9"
                         " (libc++ 9.0.1 over the target's C library, for a build that links, and the default"
                         " for MinGW; stdlib_flags)")
    ap.add_argument("--system-stdlib", action="store_true", help="the same as --stdlib host")
    ap.add_argument("--error-limit", type=int, default=50, help="errors Clang reports per file (default 50)")
    ap.add_argument("--examples", type=int, default=1, help="examples printed per category (default 1)")
    args = ap.parse_args()

    mingw = is_mingw(args.target or "")
    cxx = args.cxx or (mingw_cxx() if mingw else host_cxx())
    args.target = args.target or default_target(cxx)
    out_base = ROOT / "build" / "port" / ("windows" if mingw else "")
    args.json = args.json or str(out_base / "sweep.json")
    args.objects = args.objects or str(out_base / "sweep")
    stdlib = stdlib_mode(args)
    # -fPIC is the ELF code model; a Windows target has none (Clang rejects it there).
    host = [f for f in HOST_FLAGS if not (mingw and f == "-fPIC")]
    flags = [f"--target={args.target}", *host, *DIAG_FLAGS, f"-ferror-limit={args.error_limit}"]
    flags += stdlib_flags(stdlib, args.target)
    flags += project_includes()
    runtime = None
    if not args.no_runtime and RUNTIME_INCLUDE.is_dir():
        runtime = RUNTIME_INCLUDE
        flags += ["-I", str(runtime)]
    srcs = sources(args.only)
    obj_dir = Path(args.objects)
    version = compiler_version(cxx)
    print(f"port sweep: {len(srcs)} sources, target {args.target}, {version}, stdlib {stdlib}, {args.jobs} jobs"
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
        coff = None
        if mingw:
            objdump, cxxfilt = sibling_tool(cxx, "llvm-objdump"), sibling_tool(cxx, "llvm-cxxfilt") or hostenv.cxxfilt()
            coff = (objdump, cxxfilt) if objdump else None
        defs = definitions(nm, [(src, obj_path(src, obj_dir)) for src, code, _ in results if code == 0], coff)
        print(f"definitions ({len(defs['symbol_collisions'])} symbols and {len(defs['function_copies'])} functions"
              f" defined in more than one of the {defs['objects']} objects; llvm-nm):")
        for c in defs["symbol_collisions"]:
            print(f"  symbol   {c['symbol']}  " + "  ".join(c["sources"]))
        for c in defs["function_copies"]:
            print(f"  function {c['function']}  "
                  + "  ".join(f"{f} [{'; '.join(syms)}]" for f, syms in c["definitions"].items()))
    else:
        print("definitions: not checked (no llvm-nm next to the compiler or on PATH)")

    llp64 = None
    if mingw:
        llp64 = llp64_scan()
        kinds = Counter(h["kind"] for h in llp64)
        hand = [h for h in llp64 if not h["generated"]]
        print(f"llp64 (compiles, but 32-bit long or 16-bit wchar_t on Windows): {len(llp64)} places in"
              f" {len({h['file'] for h in llp64})} files, {len(hand)} hand-written"
              + "".join(f", {k} {n}" for k, n in sorted(kinds.items())))
        for h in llp64:
            print(f"  {h['kind']:9s} {h['file']}:{h['line']}{'  (generated)' if h['generated'] else ''}  {h['text']}")

    report = {
        "version": args.version,
        "target": args.target,
        "compiler": version,
        "stdlib": stdlib,
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
        "llp64": llp64,
    }
    out = Path(args.json)
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(report, indent=1, ensure_ascii=False) + "\n", encoding="utf-8")
    print(f"json: {rel(out)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
