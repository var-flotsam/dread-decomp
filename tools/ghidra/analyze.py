#!/usr/bin/env python3
"""Import orig/<version>/main.elf into a Ghidra project, auto-analyse it, then export.

Usage: python tools/ghidra/analyze.py --version 2.1.0 [--module main] [--reanalyze]

Re-running skips the import and analysis if the program already exists in the
project (unless --reanalyze is given), then runs the exporter again.
"""

from __future__ import annotations

import argparse
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

# Off by default in Ghidra and very slow on a binary this size; listed so the
# choice is explicit.
DISABLED_ANALYZERS = ("Decompiler Parameter ID", "Aggressive Instruction Finder")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--version", required=True)
    ap.add_argument("--module", default="main")
    ap.add_argument("--reanalyze", action="store_true")
    ap.add_argument("--max-heap", default="16g")
    args = ap.parse_args()

    elf = common.ROOT / "orig" / args.version / f"{args.module}.elf"
    if not elf.is_file():
        raise SystemExit(f"{elf} not found; run tools/nso2elf.py first")

    common.start(args.max_heap)
    import pyghidra
    from java.io import File  # type: ignore

    project = common.open_project(args.version, create=True)
    try:
        path = f"/{args.module}.elf"
        exists = project.getProjectData().getFile(path) is not None
        if not exists:
            print(f"Importing {elf} at {common.IMAGE_BASE:#x} ...", flush=True)
            with (pyghidra.program_loader()
                  .source(File(str(elf)))
                  .project(project)
                  .projectFolderPath("/")
                  .loaders("ElfLoader")
                  .language("AARCH64:LE:64:v8A")
                  .compiler("default")
                  .addLoaderArg("-loader-imagebase", f"{common.IMAGE_BASE:x}")
                  .load()) as results:
                results.save(pyghidra.task_monitor())
        with pyghidra.program_context(project, path) as program:
            base = program.getImageBase().getOffset()
            if base != common.IMAGE_BASE:
                raise SystemExit(f"image base is {base:#x}, expected {common.IMAGE_BASE:#x}")
            if not exists or args.reanalyze:
                opts = pyghidra.analysis_properties(program)
                with pyghidra.transaction(program, "options"):
                    for name in DISABLED_ANALYZERS:
                        if opts.contains(name):
                            opts.setBoolean(name, False)
                print("Analysing (this takes a while) ...", flush=True)
                t0 = time.time()
                log = pyghidra.analyze(program)
                print(f"Analysis finished in {(time.time() - t0) / 60:.1f} min", flush=True)
                (common.export_dir(args.version) / f"{args.module}_analysis_log.txt").write_text(
                    str(log), encoding="utf-8")
                program.save("auto-analysis", pyghidra.task_monitor())
            import export
            export.export_program(program, common.export_dir(args.version), args.module)
    finally:
        project.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
