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

Usage:
  python tools/port_imports.py --version 2.1.0
  python tools/port_imports.py --version 2.1.0 --list nn::os --list nn::audio
  python tools/port_imports.py --version 2.1.0 --tables build/port/link_m1/image_tables.s
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

ROOT = Path(__file__).resolve().parent.parent
NESTED = re.compile(r"_ZN?K?(\d+)")


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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", required=True)
    ap.add_argument("--tables", default=str(ROOT / "build" / "port" / "link" / "image_tables.s"),
                    help="tools/port_link.py's image_tables.s (default build/port/link/image_tables.s)")
    ap.add_argument("--list", action="append", default=[], metavar="NS",
                    help="print the unresolved imports of this namespace (nn::os, movie, C, C++); repeatable")
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
    for ns in args.list:
        print(f"\n{ns}: {len(missing.get(ns, []))} unresolved")
        for line in sorted(missing.get(ns, [])):
            print(f"  {line}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
