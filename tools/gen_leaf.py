#!/usr/bin/env python3
"""Generate src/leaf/: small leaf functions lifted from their instructions.

Thousands of tiny functions (per-type copy, assign, compare and accessor helpers, constant
returns) have no calls and no relocations. This tool lifts each one into C++:

  1. Candidates are functions of at most MAX_SIZE bytes with no calls, no PC-relative
     addressing and no branches except forward branches to the final `ret`.
  2. A small lifter turns the straight-line code into statements over `char*` parameters:
     loads and stores at fixed offsets, immediates, add/sub, compare + cset, and forward
     branches to the final `ret` (as `if (!cond) { ... }`). Anything else is skipped.
  3. All candidates are compiled in one scratch translation unit; only byte-identical
     functions are written out.

Usage: python tools/gen_leaf.py --version 2.1.0
Outputs are deterministic. Regenerate, never edit them.
"""

from __future__ import annotations

import argparse
import csv
import re
import shutil
import subprocess
import sys
from pathlib import Path

import capstone
from capstone import arm64

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "leaf"
MAX_SIZE = 64
CHUNK = 1000  # functions per generated file
NAME_RE = re.compile(r"^[A-Za-z_][\w:<>,*& ]*\bfn_([0-9a-f]{10})\(")  # a declaration or definition

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
CS.detail = True

LOADS = {"ldr": None, "ldrb": "u8", "ldrh": "u16", "ldrsb": "s8", "ldrsh": "s16", "ldrsw": "s32",
         "ldur": None, "ldurb": "u8", "ldurh": "u16"}
STORES = {"str": None, "strb": "u8", "strh": "u16", "stur": None, "sturb": "u8", "sturh": "u16"}
# cset/b.cond condition -> (C operator, signed)
CONDS = {arm64.ARM64_CC_EQ: ("==", False), arm64.ARM64_CC_NE: ("!=", False),
         arm64.ARM64_CC_LO: ("<", False), arm64.ARM64_CC_HS: (">=", False),
         arm64.ARM64_CC_HI: (">", False), arm64.ARM64_CC_LS: ("<=", False),
         arm64.ARM64_CC_LT: ("<", True), arm64.ARM64_CC_GE: (">=", True),
         arm64.ARM64_CC_GT: (">", True), arm64.ARM64_CC_LE: ("<=", True)}
NEGATE = {"==": "!=", "!=": "==", "<": ">=", ">=": "<", ">": "<=", "<=": ">"}


class Unsupported(Exception):
    pass


def regname(r: int) -> str:
    return CS.reg_name(r)


def width_type(name: str) -> str:
    return "u64" if name[0] == "x" else "u32"


def lift(code: bytes, addr: int) -> str | None:
    """C++ definition of fn_<addr>, or None if the code has an unsupported shape."""
    ins = list(CS.disasm(code, addr))
    if not ins or ins[-1].mnemonic != "ret" or len(ins) * 4 != len(code):
        return None
    end = ins[-1].address
    if any(x.mnemonic == "ret" for x in ins[:-1]):
        return None
    vals: dict[int, tuple[str, str]] = {}  # x<n> -> (expression, C type)
    params: set[int] = set()
    body: list[str] = []
    depth = 0
    cmp: tuple | None = None
    ret: tuple[str, str] | None = None

    def num(r: str) -> int:
        if r in ("xzr", "wzr"):
            return -1
        if r[0] not in "xw" or not r[1:].isdigit():
            raise Unsupported(r)
        return int(r[1:])

    def read(r: str) -> tuple[str, str]:
        if r in ("xzr", "wzr"):
            return "0", width_type(r)
        n = num(r)
        if n not in vals:
            if n > 7:
                raise Unsupported(r)
            params.add(n)
            vals[n] = (f"p{n}", "char*")
        e, t = vals[n]
        if r[0] == "w" and t != "u32":
            return f"(u32)(u64)({e})", "u32"
        return e, t

    def write(r: str, e: str, t: str) -> None:
        nonlocal ret
        n = num(r)
        if n < 0:
            return
        # Keep the expression as a temporary so later reads see this value.
        var = f"r{len(body)}_{n}"
        body.append("    " * (depth + 1) + f"{t} {var} = {e};")
        vals[n] = (var, t)
        if n == 0:
            ret = (var, t)

    def address(op) -> str:
        if op.mem.index or op.shift.type:
            raise Unsupported("indexed")
        base, _ = read(regname(op.mem.base))
        return f"((char*)({base}) + {op.mem.disp})"

    def operand(op, like: str) -> tuple[str, str]:
        if op.type == arm64.ARM64_OP_IMM:
            if op.shift.type:
                raise Unsupported("shifted imm")
            return str(op.imm), width_type(like)
        if op.type == arm64.ARM64_OP_REG:
            if op.shift.type or op.ext:
                raise Unsupported("shifted reg")
            return read(regname(op.reg))
        raise Unsupported("operand")

    try:
        for x in ins[:-1]:
            m, o = x.mnemonic, x.operands
            if x.writeback:
                return None
            if m in LOADS and len(o) == 2:
                rt = regname(o[0].reg)
                t = LOADS[m] or width_type(rt)
                if m in ("ldr", "ldur") and rt[0] not in "xw":
                    return None
                dest_t = width_type(rt) if t[0] == "s" else t
                write(rt, f"({dest_t})*({t}*){address(o[1])}", dest_t if t[0] == "s" else width_type(rt))
            elif m in STORES and len(o) == 2:
                rt = regname(o[0].reg)
                if rt[0] not in "xw":
                    return None
                t = STORES[m] or width_type(rt)
                e, _ = read(rt)
                body.append("    " * (depth + 1) + f"*({t}*){address(o[1])} = ({t})(u64)({e});")
            elif m == "stp" and len(o) == 3:
                for k in range(2):
                    rt = regname(o[k].reg)
                    if rt[0] not in "xw":
                        return None
                    t = width_type(rt)
                    e, _ = read(rt)
                    a = o[2]
                    base, _ = read(regname(a.mem.base))
                    off = a.mem.disp + k * (8 if t == "u64" else 4)
                    body.append("    " * (depth + 1) + f"*({t}*)((char*)({base}) + {off}) = ({t})(u64)({e});")
            elif m in ("mov", "movz") and len(o) == 2:
                rd = regname(o[0].reg)
                if o[1].type == arm64.ARM64_OP_IMM:
                    mask = 0xFFFFFFFF if rd[0] == "w" else 0xFFFFFFFFFFFFFFFF
                    write(rd, f"{o[1].imm & mask:#x}", width_type(rd))
                else:
                    e, t = read(regname(o[1].reg))
                    write(rd, e if rd[0] == "x" else f"(u32)(u64)({e})", t if rd[0] == "x" else "u32")
            elif m in ("add", "sub") and len(o) == 3:
                rd = regname(o[0].reg)
                a, ta = read(regname(o[1].reg))
                b, _ = operand(o[2], rd)
                op = "+" if m == "add" else "-"
                if ta == "char*" and rd[0] == "x":
                    write(rd, f"({a}) {op} {b}", "char*")
                else:
                    t = width_type(rd)
                    write(rd, f"({t})(u64)({a}) {op} ({t})({b})", t)
            elif m == "cmp" and len(o) == 2:
                a, ta = read(regname(o[0].reg))
                b, _ = operand(o[1], regname(o[0].reg))
                cmp = (a, b, regname(o[0].reg)[0])
            elif m == "cset" and len(o) == 1 and cmp:
                if x.cc not in CONDS:
                    return None
                opr, signed = CONDS[x.cc]
                t = ("s64" if cmp[2] == "x" else "s32") if signed else ("u64" if cmp[2] == "x" else "u32")
                write(regname(o[0].reg), f"({t})(u64)({cmp[0]}) {opr} ({t})({cmp[1]})", "bool")
            elif m.startswith("b.") and o[0].imm == end and cmp:
                if x.cc not in CONDS:
                    return None
                opr, signed = CONDS[x.cc]
                t = ("s64" if cmp[2] == "x" else "s32") if signed else ("u64" if cmp[2] == "x" else "u32")
                body.append("    " * (depth + 1) + f"if (({t})(u64)({cmp[0]}) {NEGATE[opr]} ({t})({cmp[1]})) {{")
                depth += 1
            elif m in ("cbz", "cbnz") and o[1].imm == end:
                e, _ = read(regname(o[0].reg))
                t = width_type(regname(o[0].reg))
                body.append("    " * (depth + 1) + f"if (({t})(u64)({e}) {'!=' if m == 'cbz' else '=='} 0) {{")
                depth += 1
            else:
                return None
    except Unsupported:
        return None
    if depth and ret is not None:
        return None  # a value set under a condition: not modelled
    body += ["    " * (depth - k) + "}" for k in range(depth)]
    rtype = "void"
    if ret is not None:
        rtype = ret[1]
        body.append(f"    return {ret[0]};")
    args = ", ".join(f"char* p{n}" for n in range(max(params) + 1)) if params else ""
    return "\n".join([f"{rtype} fn_{addr:x}({args}) {{", *body, "}"])


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    # Functions defined elsewhere in src/ (by hand or by another generator) are left alone.
    skip = set()
    for f in (ROOT / "src").rglob("*.cpp"):
        if OUT not in f.parents:
            for line in f.read_text(encoding="utf-8").splitlines():
                m = NAME_RE.match(line)
                if m and ("{" in line or not line.rstrip().endswith(";")):
                    skip.add(int(m.group(1), 16))
    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
    defs = {}
    for a in rows:
        size = orig.sizes.get(a)
        if size is None or size > MAX_SIZE or a in skip:
            continue
        src = lift(orig.function(a), a)
        if src:
            defs[a] = src

    work = ROOT / "build" / "gen_leaf"
    work.mkdir(parents=True, exist_ok=True)
    src_path, obj = work / "candidates.cpp", work / "candidates.o"
    header = ['#include "types.h"', ""]
    src_path.write_text("\n".join(header + [defs[a] for a in sorted(defs)]) + "\n", encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src_path, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
    ok = sorted({res.address for res in asmdiff.diff_object(obj, orig) if res.matching} & set(defs))

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = ["// Generated by tools/gen_leaf.py from main; regenerate rather than edit.",
            "// Small leaf functions lifted from their instructions, byte-identical to the original."]
    for k in range(0, len(ok), CHUNK):
        chunk = ok[k:k + CHUNK]
        (OUT / f"leaf_{k // CHUNK:02d}.cpp").write_text(
            "\n".join(note + header + [defs[a] + "\n" for a in chunk]), encoding="utf-8", newline="\n")
    print(f"{len(defs)} lifted of {len(rows)} candidates; {len(ok)} byte-identical "
          f"({sum(orig.sizes[a] for a in ok)} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
