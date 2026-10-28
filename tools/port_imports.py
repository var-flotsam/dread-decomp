#!/usr/bin/env python3
"""Compare the main NSO's imports with the port runtime's definitions, by exact mangled name.

The dump's dynamic symbol table names every function and object `main` imports from the SDK, the
C library and the C++ ABI, with full parameter lists (`_ZN2nn3hid12GetNpadStateEPNS0_16NpadFullKeyStateERKj`).
The data loader (port/runtime/include/dread_port_loader.h) resolves the game's pointers to imports
by that name, and tools/port_link.py's image_tables.s lists the names the runtime can answer. A
runtime declaration whose name matches the import's exactly has the console's parameter types (the
mangling encodes them), so this is also the check that a new stub's signature is right: write the
declaration, rerun tools/port_link.py, then this.

Reports per namespace how many imports resolve and how many do not; --list NS prints the ones that
do not (demangled, with the mangled name), for writing the next stubs. Reads orig/<version>/exefs/main
at run time and prints names only; nothing is written.

--windows (a report, compile only): compiles port/runtime/src/ for x86_64-w64-mingw32 with llvm-mingw
(tools/port_sweep.py's Windows flags; objects in build/port/windows/runtime/) and tells which imports
the runtime answers on Linux that it would not answer by name on Windows. Under LLP64 `long` is 32
bits, so s64 and u64 are `long long`: a runtime function taking them mangles `x`/`y` where the
console's name has `l`/`m`, and the loader no longer finds it. Each such import is listed with the
name its definition mangles as there. Also listed: runtime imports whose Windows definition keeps the
console's name through a `long` it spells out, which resolve but at 32 bits, and runtime sources that
do not compile for Windows (first error). The C library names and the C++ ABI names the loader
resolves are the same on both and are not compared.

Usage:
  python tools/port_imports.py --version 2.1.0
  python tools/port_imports.py --version 2.1.0 --list nn::os --list nn::audio
  python tools/port_imports.py --version 2.1.0 --tables build/port/link_m1/image_tables.s
  python tools/port_imports.py --version 2.1.0 --windows [--list nn::os]
"""

from __future__ import annotations

import argparse
import re
import struct
import subprocess
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import nso2elf  # noqa: E402
import port_sweep  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
NESTED = re.compile(r"_ZN?K?(\d+)")
RUNTIME_SRC = ROOT / "port" / "runtime" / "src"
WINDOWS_TARGET = "x86_64-w64-mingw32"
RUNTIME_NAMESPACES = ("nn::", "nv::", "movie::")  # what image_tables.s answers from the runtime by mangled name
LONG = re.compile(r"\blong\b")
LONG_LONG = re.compile(r"\blong long\b|\blong double\b")


def imports(main: Path) -> list[str]:
    """The undefined symbols of the NSO's dynamic symbol table."""
    mod = nso2elf.read_module(main)
    img, dyn, dt = mod.image, mod.dyn, nso2elf.DT
    symtab, strtab = dyn[dt["SYMTAB"]], dyn[dt["STRTAB"]]
    out = []
    for i in range(1, mod.dynsym_size // 24):
        st_name, _, _, shndx, _, _ = struct.unpack_from("<IBBHQQ", img, symtab + 24 * i)
        if shndx == 0:
            out.append(img[strtab + st_name:img.index(b"\0", strtab + st_name)].decode())
    return out


def namespace(mangled: str, demangled: str) -> str:
    """nn::<namespace>, the other SDK namespaces (movie, nv), 'C' or 'C++'."""
    if not mangled.startswith("_Z"):
        return "C"
    if demangled.startswith("nn::"):
        return "::".join(demangled.split("::")[:2])
    top = demangled.split("::")[0].split("(")[0]
    if top in ("movie", "nv"):
        return top
    return "C++"


def lp64(demangled: str) -> str:
    """A demangled name with `long long` read as `long`: the LP64 spelling of an LLP64 definition."""
    return demangled.replace("long long", "long")


def windows_runtime(out_dir: Path) -> tuple[set[str], dict[str, str]]:
    """port/runtime/src/ compiled for Windows: the global names its objects define, and the sources
    that do not compile with their first error line."""
    cxx = port_sweep.mingw_cxx()
    nm = port_sweep.sibling_tool(cxx, "llvm-nm")
    if not nm:
        raise SystemExit(f"llvm-nm is needed next to {cxx} or on PATH")
    flags = [f"--target={WINDOWS_TARGET}", *(f for f in port_sweep.HOST_FLAGS if f != "-fPIC"), "-w",
             *port_sweep.stdlib_flags("libcxx9", WINDOWS_TARGET), *port_sweep.project_includes(),
             "-I", str(port_sweep.RUNTIME_INCLUDE)]
    out_dir.mkdir(parents=True, exist_ok=True)
    objs, failed = [], {}
    for src in sorted(RUNTIME_SRC.glob("*.cpp")):
        obj = out_dir / (src.stem + ".o")
        r = subprocess.run([cxx, *flags, str(src), "-o", str(obj)], capture_output=True, text=True,
                           encoding="utf-8", errors="replace", cwd=ROOT)
        if r.returncode == 0:
            objs.append(str(obj))
            continue
        obj.unlink(missing_ok=True)
        errors = [ln for ln in r.stderr.splitlines() if " error: " in ln or ln.startswith("error:")]
        failed[src.relative_to(ROOT).as_posix()] = (errors or r.stderr.splitlines() or ["?"])[0].strip()
    defined: set[str] = set()
    if objs:
        r = subprocess.run([nm, "--defined-only", "--extern-only", "--format=just-symbols", *objs],
                           capture_output=True, text=True, check=True)
        defined = {ln.strip() for ln in r.stdout.splitlines() if ln.strip() and not ln.endswith(":")}
    return defined, failed


def windows_report(names: list[str], resolvable: set[str], demangled: dict[str, str], lists: list[str]) -> None:
    """The runtime's imports on Windows against Linux (--windows)."""
    out_dir = ROOT / "build" / "port" / "windows" / "runtime"
    defined, failed = windows_runtime(out_dir)
    dem_win = asmdiff.demangle(sorted(defined), strip=False)
    by_name: dict[str, list[str]] = defaultdict(list)  # qualified name without parameters -> Windows definitions
    for sym, dem in dem_win.items():
        by_name[port_sweep.own_name(dem)].append(sym)

    runtime = [n for n in names if n in resolvable and demangled.get(n, n).startswith(RUNTIME_NAMESPACES)]
    lost: dict[str, list[str]] = defaultdict(list)
    narrow: dict[str, list[str]] = defaultdict(list)
    counts: Counter[tuple[str, str]] = Counter()
    for n in runtime:
        dem = demangled.get(n, n)
        ns = namespace(n, dem)
        if n in defined:
            counts[(ns, "same")] += 1
            if LONG.search(LONG_LONG.sub("", dem.partition("(")[2])):
                narrow[ns].append(f"{dem}  [{n}]")
            continue
        counts[(ns, "lost")] += 1
        there = sorted({dem_win[d] for d in by_name.get(port_sweep.own_name(dem), [])})  # C1/C2 read alike
        same = [t for t in there if lp64(t) == lp64(dem)]  # the definition it is, `long long` for `long`
        there = same or there
        lost[ns].append(f"{dem}  [{n}]\n      on Windows: " + ("; ".join(there) if there else "not defined"))
    print(f"\nwindows ({WINDOWS_TARGET}, {port_sweep.compiler_version(port_sweep.mingw_cxx())}):"
          f" runtime sources {len(list(RUNTIME_SRC.glob('*.cpp')))}, not compiling {len(failed)};"
          f" objects in {out_dir.relative_to(ROOT).as_posix()}")
    for src, err in sorted(failed.items()):
        print(f"  {src}: {err}")
    total_lost = sum(v for (_, k), v in counts.items() if k == "lost")
    print(f"runtime imports resolved on Linux: {len(runtime)}; by the same name on Windows:"
          f" {len(runtime) - total_lost}, lost: {total_lost}; resolved at 32 bits through a spelled-out"
          f" long: {sum(map(len, narrow.values()))}")
    for ns in sorted({ns for ns, _ in counts}):
        print(f"  {ns:14} same {counts[(ns, 'same')]:4}  lost {counts[(ns, 'lost')]:4}"
              f"  32-bit long {len(narrow.get(ns, [])):4}")
    for ns in lists:
        print(f"\n{ns} on Windows: {len(lost.get(ns, []))} lost, {len(narrow.get(ns, []))} at 32 bits")
        for line in sorted(lost.get(ns, [])):
            print(f"  lost   {line}")
        for line in sorted(narrow.get(ns, [])):
            print(f"  32-bit {line}")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", required=True)
    ap.add_argument("--tables", default=str(ROOT / "build" / "port" / "link" / "image_tables.s"),
                    help="tools/port_link.py's image_tables.s (default build/port/link/image_tables.s)")
    ap.add_argument("--list", action="append", default=[], metavar="NS",
                    help="print the unresolved imports of this namespace (nn::os, movie, C, C++); repeatable;"
                         " with --windows, the imports lost on Windows instead")
    ap.add_argument("--windows", action="store_true",
                    help="compile port/runtime/src/ for Windows (llvm-mingw) and report the imports it would"
                         " stop answering by name there (LLP64 mangling)")
    args = ap.parse_args()

    main_nso = ROOT / "orig" / args.version / "exefs" / "main"
    if not main_nso.is_file():
        raise SystemExit(f"{main_nso} not found: run tools/setup.py first")
    tables = Path(args.tables)
    if not tables.is_file():
        raise SystemExit(f"{tables} not found: run tools/port_link.py first")
    resolvable = set(re.findall(r'\.asciz\t"([^"]+)"', tables.read_text(encoding="utf-8")))
    names = imports(main_nso)
    try:
        demangled = asmdiff.demangle(names, strip=False)
    except (OSError, subprocess.SubprocessError) as e:
        raise SystemExit(f"llvm-cxxfilt is needed ({asmdiff.cxxfilt()}): {e}")

    counts: Counter[tuple[str, bool]] = Counter()
    missing: dict[str, list[str]] = defaultdict(list)
    for n in names:
        dem = demangled.get(n, n)
        ns = namespace(n, dem)
        ok = n in resolvable
        counts[(ns, ok)] += 1
        if not ok:
            missing[ns].append(f"{dem}  [{n}]")
    print(f"imports of {main_nso.relative_to(ROOT).as_posix()}: {len(names)};"
          f" resolvable by name through {tables.as_posix()}: {sum(v for (_, ok), v in counts.items() if ok)}")
    for ns in sorted({ns for ns, _ in counts}):
        print(f"  {ns:14} resolved {counts[(ns, True)]:4}  unresolved {counts[(ns, False)]:4}")
    if args.windows:
        windows_report(names, resolvable, demangled, args.list)
        return 0
    for ns in args.list:
        print(f"\n{ns}: {len(missing.get(ns, []))} unresolved")
        for line in sorted(missing.get(ns, [])):
            print(f"  {line}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
