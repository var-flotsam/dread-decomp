#!/usr/bin/env python3
"""Raw P-code of AArch64 instruction words, from Ghidra's SLEIGH specification.

tools/port_lift.py's oracle (tools/pcode_emu.py) executes both the original instructions and the
compiled semantic lift as P-code, so the meaning of every instruction comes from Ghidra's
processor specification and not from the lifter under test (docs/PORT_PLAN.md, oracle (b)). This
script runs in its own process because tools/ghidra/ must not be importable as `ghidra` next to
Ghidra's Java packages (tools/ is never on sys.path here).

Input: a text file of `<address hex> <word hex>` lines. Output: a pickle of
  {"ops": {(address, word): [(opcode, out, inputs), ...]},
   "userops": {index: name}, "registers": {name: (offset, size)}, "opcodes": {name: number}}
where `out` is None or (space, offset, size), `inputs` a tuple of the same, and space is
0 const, 1 unique, 2 register, 3 ram (any other space raises). An undecodable word maps to None.
No project is opened (an in-memory language parse), so it runs next to other Ghidra tools.
The output is game-derived (instruction semantics of the original's code): build/ only.

Usage: python tools/ghidra/pcode.py <words.txt> <out.pickle>
"""

from __future__ import annotations

import pickle
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import common  # noqa: E402

REGISTERS = [f"x{i}" for i in range(31)] + [f"q{i}" for i in range(32)] + \
    ["sp", "pc", "NG", "ZR", "CY", "OV", "tpidr_el0", "fpcr", "fpsr"]


def main() -> int:
    src, dst = Path(sys.argv[1]), Path(sys.argv[2])
    words = []
    for line in src.read_text(encoding="utf-8").split("\n"):
        if line.strip():
            a, w = line.split()
            words.append((int(a, 16), int(w, 16)))
    common.start("2g")
    from ghidra.app.util import PseudoInstruction
    from ghidra.program.model.lang import LanguageID, ProcessorContextImpl
    from ghidra.program.model.mem import ByteMemBufferImpl
    from ghidra.program.model.pcode import PcodeOp
    from ghidra.program.util import DefaultLanguageService

    lang = DefaultLanguageService.getLanguageService().getLanguage(LanguageID("AARCH64:LE:64:v8A"))
    space = lang.getAddressFactory().getDefaultAddressSpace()
    ctx = ProcessorContextImpl(lang)
    spaces: dict[str, int] = {"const": 0, "unique": 1, "register": 2, "ram": 3}

    def vn(v):
        if v is None:
            return None
        name = v.getAddress().getAddressSpace().getName()
        if name not in spaces:
            raise ValueError(f"space {name}")
        return spaces[name], int(v.getOffset()), int(v.getSize())

    out: dict = {}
    for a, w in words:
        addr = space.getAddress(a)
        buf = ByteMemBufferImpl(addr, w.to_bytes(4, "little"), False)
        try:
            proto = lang.parse(buf, ctx, False)
            ins = PseudoInstruction(addr, proto, buf, ctx)
            ops = [(int(op.getOpcode()), vn(op.getOutput()), tuple(vn(x) for x in op.getInputs()))
                   for op in ins.getPcode()]
        except Exception:  # an undecodable word or an unexpected space
            ops = None
        out[(a, w)] = ops
    regs = {}
    for name in REGISTERS:
        r = lang.getRegister(name)
        if r is not None:
            regs[name] = (int(r.getOffset()), int(r.getMinimumByteSize()))
    userops = {i: str(lang.getUserDefinedOpName(i)) for i in range(lang.getNumberOfUserDefinedOpNames())}
    opcodes = {str(PcodeOp.getMnemonic(i)): i for i in range(PcodeOp.PCODE_MAX)}
    with dst.open("wb") as fh:
        pickle.dump({"ops": out, "userops": userops, "registers": regs, "opcodes": opcodes}, fh)
    return 0


if __name__ == "__main__":
    sys.exit(main())
