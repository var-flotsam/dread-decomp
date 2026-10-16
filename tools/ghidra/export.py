#!/usr/bin/env python3
"""Export analysis results from the Ghidra project to build/ghidra_export/<version>/.

Writes, per module:
  <module>_blocks.json     memory blocks (name, start, size, permissions)
  <module>_functions.jsonl one line per function: entry, name, name source, body extent,
                           byte count, thunk target, called functions
  <module>_imports.json    external (imported) symbols
  <module>_exports.json    exported entry points
  <module>_strings.jsonl   defined strings (address, value, functions referencing them)

Usage: python tools/ghidra/export.py --version 2.1.0 [--module main]
(also called at the end of analyze.py)
"""

from __future__ import annotations

import argparse
import json
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402


def _addr(a) -> str:
    return f"{a.getOffset():#x}"


def export_program(program, out: Path, module: str) -> None:
    import pyghidra

    monitor = pyghidra.task_monitor()
    mem = program.getMemory()

    blocks = [{
        "name": str(b.getName()), "start": _addr(b.getStart()), "size": b.getSize(),
        "perms": ("r" if b.isRead() else "-") + ("w" if b.isWrite() else "-") + ("x" if b.isExecute() else "-"),
        "initialized": b.isInitialized(),
    } for b in mem.getBlocks()]
    (out / f"{module}_blocks.json").write_text(json.dumps(blocks, indent=1), encoding="utf-8")

    fm = program.getFunctionManager()
    n = 0
    with (out / f"{module}_functions.jsonl").open("w", encoding="utf-8") as f:
        for fn in fm.getFunctions(True):
            body = fn.getBody()
            sym = fn.getSymbol()
            thunk = fn.getThunkedFunction(False) if fn.isThunk() else None
            rec = {
                "entry": _addr(fn.getEntryPoint()),
                "name": str(fn.getName(True)),
                "source": str(sym.getSource()),
                "min": _addr(body.getMinAddress()),
                "max": _addr(body.getMaxAddress()),
                "bytes": body.getNumAddresses(),
                "external": fn.isExternal(),
                "thunk_of": _addr(thunk.getEntryPoint()) if thunk else None,
                "calls": sorted({_addr(c.getEntryPoint()) for c in fn.getCalledFunctions(monitor)}),
            }
            f.write(json.dumps(rec) + "\n")
            n += 1
    print(f"  {n} functions", flush=True)

    st = program.getSymbolTable()
    imports = [{"name": str(s.getName(True)), "address": _addr(s.getAddress()),
                "type": str(s.getSymbolType())}
               for s in st.getExternalSymbols()]
    (out / f"{module}_imports.json").write_text(json.dumps(imports, indent=1), encoding="utf-8")

    exports = []
    for a in st.getExternalEntryPointIterator():
        s = st.getPrimarySymbol(a)
        exports.append({"address": _addr(a), "name": str(s.getName(True)) if s else None})
    (out / f"{module}_exports.json").write_text(json.dumps(exports, indent=1), encoding="utf-8")

    from ghidra.program.util import DefinedStringIterator  # type: ignore
    refman = program.getReferenceManager()
    ns = 0
    with (out / f"{module}_strings.jsonl").open("w", encoding="utf-8") as f:
        for d in DefinedStringIterator.forProgram(program):
            try:
                value = str(d.getValue())
            except Exception:
                continue
            funcs = set()
            for ref in refman.getReferencesTo(d.getAddress()):
                fn = fm.getFunctionContaining(ref.getFromAddress())
                if fn is not None:
                    funcs.add(_addr(fn.getEntryPoint()))
            f.write(json.dumps({"address": _addr(d.getAddress()), "value": value,
                                "funcs": sorted(funcs)}) + "\n")
            ns += 1
    print(f"  {len(blocks)} blocks, {len(imports)} imports, {len(exports)} exports, {ns} strings", flush=True)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("--version", required=True)
    ap.add_argument("--module", default="main")
    args = ap.parse_args()
    common.start()
    import pyghidra
    project = common.open_project(args.version)
    try:
        with pyghidra.program_context(project, f"/{args.module}.elf") as program:
            export_program(program, common.export_dir(args.version), args.module)
    finally:
        project.close()
    return 0


if __name__ == "__main__":
    sys.exit(main())
