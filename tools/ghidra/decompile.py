#!/usr/bin/env python3
"""Print Ghidra's decompilation of one or more functions, for reading only.

    *** Never paste this output into src/, include/, docs/ or any other tracked file. ***
    Source in this repo is our own reconstruction (CLAUDE.md, "Hard rules"). This tool
    prints to the terminal only and writes nothing.

Functions are given by address (hex, with or without 0x) or by name from data/functions/.
Reads the analysed project in build/ghidra/<version>/ (tools/ghidra/analyze.py) without
changing it. Only one process can hold the project, so close other Ghidra tools first.

Usage:
  python tools/ghidra/decompile.py --version 2.1.0 7100227448
  python tools/ghidra/decompile.py --version 2.1.0 CActor::refl_GetClass fn_71000003d4
"""

from __future__ import annotations

import argparse
import csv
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

ROOT = common.ROOT


def resolve(names: list[str]) -> list[int]:
    by_name: dict[str, list[int]] = {}
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                by_name.setdefault(r["name"], []).append(int(r["address"], 16))
    out = []
    for n in names:
        try:
            out.append(int(n, 16))
            continue
        except ValueError:
            pass
        hits = by_name.get(n, [])
        if len(hits) != 1:
            raise SystemExit(f"{n}: {'not found' if not hits else f'{len(hits)} functions have this name'}")
        out.append(hits[0])
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("functions", nargs="+", help="addresses (hex) or names from data/functions/")
    ap.add_argument("--timeout", type=int, default=60, help="seconds per function")
    args = ap.parse_args()
    addresses = resolve(args.functions)
    if not (common.project_dir(args.version) / f"{common.PROJECT_NAME}.gpr").exists():
        raise SystemExit(f"no Ghidra project in {common.project_dir(args.version)}: run tools/ghidra/analyze.py")

    common.start("8g")
    import pyghidra
    from ghidra.app.decompiler import DecompInterface

    try:
        project = common.open_project(args.version)
    except Exception as e:  # LockException and friends arrive as Java exceptions
        raise SystemExit(f"cannot open the Ghidra project (is another Ghidra process using it?): {e}")
    try:
        with pyghidra.program_context(project, "/main.elf") as program:
            di = DecompInterface()
            di.openProgram(program)
            space = program.getAddressFactory().getDefaultAddressSpace()
            for a in addresses:
                fn = program.getFunctionManager().getFunctionAt(space.getAddress(a))
                print(f"// ---- {a:#x} (Ghidra decompilation: for reading only, never commit) ----")
                if fn is None:
                    print(f"// no function starts at {a:#x}")
                    continue
                res = di.decompileFunction(fn, args.timeout, pyghidra.task_monitor())
                if not res.decompileCompleted():
                    print(f"// decompilation failed: {res.getErrorMessage()}")
                    continue
                print(res.getDecompiledFunction().getC())
            di.dispose()
    finally:
        project.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
