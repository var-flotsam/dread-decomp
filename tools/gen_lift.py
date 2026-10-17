#!/usr/bin/env python3
"""Generate src/lifted/: small functions lifted from their instructions.

Tens of thousands of small functions (per-type copy, assign and compare helpers, accessors,
constant returns, thin wrappers around a call or two) follow simple code patterns. This tool
lifts each one into C++ over `char*` parameters and keeps only the byte-identical results:

  1. Candidates are functions of at most MAX_SIZE bytes that aren't defined elsewhere in src/.
  2. A lifter turns the code into statements: loads and stores at fixed offsets, immediates,
     add/sub, compare + cset, forward branches to the epilogue (`if (!cond) { ... }`), calls
     and tail calls (direct or through a register), string literals and global addresses.
     Saves and restores of callee-saved registers are skipped. Anything else is rejected.
     Callees are declared in namespace `callee` (one overload per argument list), so their
     names never clash with the functions being defined; globals are `g_unk_<addr>` (reached
     through the GOT) or hidden `h::g_unk_<addr>` (addressed directly).
  3. The number of arguments a call passes and whether a value is returned are not visible
     in the code. Each function is lifted in a few VARIANTS; every variant is compiled in one
     scratch translation unit, and the first byte-identical variant of each function is kept.

Usage: python tools/gen_lift.py --version 2.1.0
Outputs are deterministic. Regenerate, never edit them.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import re
import shutil
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

import capstone
from capstone import arm64

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import refl_attrs  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "lifted"
BASE = 0x7100000000
MAX_SIZE = 128
CHUNK = 1000  # functions per generated file
NAME_RE = re.compile(r"^[A-Za-z_][\w:<>,*& ]*\bfn_([0-9a-f]{10})\(")  # a declaration or definition
# (extra call arguments beyond those written, return x0 if set): tried in this order.
VARIANTS = [(0, True), (0, False), (1, True), (1, False), (2, True), (2, False)]

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
    name = CS.reg_name(r)
    return {"fp": "x29", "lr": "x30"}.get(name, name)


def width_type(name: str) -> str:
    return "u64" if name[0] == "x" else "u32"


def is_frame(x) -> bool:
    """Prologue/epilogue: saves and restores of x19-x30, frame pointer and stack adjustments."""
    m, o = x.mnemonic, x.operands
    mem = [op for op in o if op.type == arm64.ARM64_OP_MEM]
    if m in ("stp", "str", "ldp", "ldr") and mem and regname(mem[0].mem.base) == "sp":
        regs = [regname(op.reg) for op in o if op.type == arm64.ARM64_OP_REG]
        return all(r in ("x29", "x30") or (r[0] == "x" and r[1:].isdigit() and 19 <= int(r[1:]) <= 28)
                   for r in regs)
    if m in ("add", "sub") and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM:
        return (regname(o[0].reg), regname(o[1].reg)) in (("x29", "sp"), ("sp", "sp"))
    return m == "mov" and len(o) == 2 and regname(o[0].reg) == "x29" and regname(o[1].reg) == "sp"


def fkey(r: str) -> int | None:
    """Float register number (s<n>/d<n>), or None."""
    return int(r[1:]) if r[0] in "sd" and r[1:].isdigit() else None


class Lifter:
    def __init__(self, orig: asmdiff.Original, addr: int, extra: int, returns: bool):
        self.orig, self.addr, self.extra, self.returns = orig, addr, extra, returns
        # Register -> (expression, C type). Keys: n for x<n>/w<n>, 100 + n for s<n>/d<n>.
        self.vals: dict[int, tuple[str, str]] = {}
        self.consts: dict[int, int] = {}
        self.params: dict[int, str] = {}  # key -> C type
        self.written: set[int] = set()  # argument registers written since the last call
        self.called = False
        self.body: list[str] = []
        self.decls: set[str] = set()
        self.closes: list[int] = []  # open `if` blocks: address where each ends
        self.snaps: list[tuple] = []
        self.poison: set[int] = set()
        self.cmp: tuple | None = None
        self.ret: tuple[str, str, int] | None = None  # (expression, type, depth when set)
        self.pages: dict[int, int] = {}

    @property
    def depth(self) -> int:
        return len(self.closes)

    def line(self, s: str) -> None:
        self.body.append("    " * (self.depth + 1) + s)

    @staticmethod
    def key(r: str) -> int:
        if r in ("xzr", "wzr"):
            return -1
        if r[0] in "xw" and r[1:].isdigit():
            return int(r[1:])
        f = fkey(r)
        if f is not None:
            return 100 + f
        raise Unsupported(r)

    def read(self, r: str) -> tuple[str, str]:
        if r in ("xzr", "wzr"):
            return "0", width_type(r)
        n = self.key(r)
        if n in self.pages or n in self.poison:
            raise Unsupported("unknown value")
        if n not in self.vals:
            if self.called or (n > 7 and not 100 <= n <= 107):
                raise Unsupported(r)
            t = "char*" if n < 100 else ("f32" if r[0] == "s" else "f64")
            self.params[n] = t
            self.vals[n] = (f"p{n}" if n < 100 else f"f{n - 100}", t)
        e, t = self.vals[n]
        if r[0] == "w" and t != "u32":
            return f"(u32)(u64)({e})", "u32"
        if r[0] in "sd" and t != ("f32" if r[0] == "s" else "f64"):
            raise Unsupported("float width")
        return e, t

    def write(self, r: str, e: str, t: str, const: int | None = None) -> None:
        n = self.key(r)
        if n < 0:
            return
        var = f"r{len(self.body)}_{n}"
        self.line(f"{t} {var} = {e};")
        self.vals[n] = (var, t)
        self.pages.pop(n, None)
        self.poison.discard(n)
        if const is None:
            self.consts.pop(n, None)
        else:
            self.consts[n] = const
        if n <= 7 or 100 <= n <= 107:
            self.written.add(n)
        if n in (0, 100):
            self.ret = (var, t, self.depth)

    def open_if(self, cond: str, target: int) -> None:
        if self.closes and target > self.closes[-1]:
            raise Unsupported("crossing branches")
        self.line(f"if ({cond}) {{")
        self.closes.append(target)
        self.snaps.append((dict(self.vals), dict(self.pages), set(self.poison), self.called))

    def close_to(self, address: int) -> None:
        while self.closes and self.closes[-1] <= address:
            self.closes.pop()
            vals, pages, poison, called = self.snaps.pop()
            self.body.append("    " * (self.depth + 1) + "}")
            # Registers changed in the block hold different values on the two paths.
            changed = {n for n in set(vals) | set(self.vals) if vals.get(n) != self.vals.get(n)}
            if self.called and not called:
                changed |= set(range(19)) | set(range(100, 132))
            changed |= {n for n in set(pages) | set(self.pages) if pages.get(n) != self.pages.get(n)}
            self.vals = {n: v for n, v in vals.items() if n not in changed}
            self.pages = {n: v for n, v in pages.items() if n not in changed}
            self.poison = poison | changed
            self.consts = {n: v for n, v in self.consts.items() if n not in changed}

    def global_ref(self, a: int, through_got: bool) -> str:
        """Address of the object or function at a, as char*."""
        if a in self.orig.sizes:
            ns = "fp" if through_got else "hf"
            self.decls.add(f"namespace {ns} {{ {'' if through_got else 'HIDDEN '}void fn_{a:x}(); }}")
            return f"(char*)&{ns}::fn_{a:x}"
        if through_got:
            self.decls.add(f"extern char g_unk_{a:x}[];")
            return f"(char*)g_unk_{a:x}"
        self.decls.add(f"namespace h {{ extern HIDDEN char g_unk_{a:x}[]; }}")
        return f"(char*)h::g_unk_{a:x}"

    def address(self, op) -> str:
        if op.mem.index or op.shift.type:
            raise Unsupported("indexed")
        n = self.key(regname(op.mem.base))
        if n in self.pages:
            a = self.pages[n] + op.mem.disp
            if self.orig.img.relocs.get(a - BASE) is not None:
                raise Unsupported("GOT slot as data")
            return self.global_ref(a, False)
        base, _ = self.read(regname(op.mem.base))
        return f"((char*)({base}) + {op.mem.disp})"

    def operand(self, op, like: str) -> tuple[str, str]:
        if op.type == arm64.ARM64_OP_IMM:
            if op.shift.type:
                raise Unsupported("shifted imm")
            return str(op.imm), width_type(like)
        if op.type == arm64.ARM64_OP_REG:
            if op.shift.type or op.ext:
                raise Unsupported("shifted reg")
            return self.read(regname(op.reg))
        raise Unsupported("operand")

    def cond(self, cc: int, negate: bool) -> str:
        if cc not in CONDS or not self.cmp:
            raise Unsupported("condition")
        opr, signed = CONDS[cc]
        c = self.cmp
        if c[2] == "f":
            expr = f"({c[3]})({c[0]}) {opr} ({c[3]})({c[1]})"
            return f"!({expr})" if negate else expr
        t = ("s64" if c[2] == "x" else "s32") if signed else ("u64" if c[2] == "x" else "u32")
        return f"({t})(u64)({c[0]}) {NEGATE[opr] if negate else opr} ({t})({c[1]})"

    def args(self) -> list[tuple[str, str]]:
        n = max([k for k in self.written if k < 100], default=-1) + 1
        n = max(n, self.extra)
        fl = max([k - 100 for k in self.written if k >= 100], default=-1) + 1
        floats = []
        for i in range(fl):
            t = self.vals.get(100 + i, ("", "f32"))[1]
            floats.append(self.read(("s" if t == "f32" else "d") + str(i)))
        return [self.read(f"x{i}") for i in range(n)] + floats

    def call(self, fn: str, ptr: str | None = None) -> str:
        """A call expression; declares the callee overload for these argument types."""
        args = self.args()
        types = ", ".join(t for _, t in args)
        values = ", ".join(e for e, _ in args)
        if ptr is not None:
            return f"((char* (*)({types}))({ptr}))({values})"
        self.decls.add(f"namespace callee {{ char* {fn}({types}); }}")
        return f"callee::{fn}({values})"

    def clobber(self) -> None:
        for n in list(self.vals):
            if n <= 18 or 100 <= n <= 107 or n >= 116:
                del self.vals[n]
        for n in list(self.pages):
            if n <= 18:
                del self.pages[n]
        self.consts = {n: v for n, v in self.consts.items() if n > 18}
        self.written = set()
        self.called = True
        self.cmp = None

    def lift(self) -> tuple[str, set[str]] | None:
        orig, addr = self.orig, self.addr
        code = orig.function(addr)
        ins = list(CS.disasm(code, addr))
        if not ins or len(ins) * 4 != len(code):
            return None
        last = ins[-1]
        tail = last.mnemonic in ("b", "br")
        if last.mnemonic not in ("ret", "b", "br") or any(x.mnemonic == "ret" for x in ins[:-1]):
            return None
        if last.mnemonic == "b" and addr <= last.operands[0].imm < addr + len(code):
            return None
        try:
            for x in ins[:-1]:
                self.close_to(x.address)
                if is_frame(x):
                    continue
                if not self.step(x, last.address):
                    return None
            self.close_to(last.address)
            if self.depth:
                return None
            if tail:
                if last.mnemonic == "b":
                    call = self.call(f"fn_{last.operands[0].imm:x}")
                else:
                    ptr, _ = self.read(regname(last.operands[0].reg))
                    call = self.call("", ptr)
                self.line(f"return {call};")
                rtype = "char*"
            else:
                rtype = "void"
                if self.ret is not None and self.returns:
                    if self.ret[2] or (self.ret[0], self.ret[1]) not in self.vals.values():
                        return None  # a value set under a condition: not modelled
                    rtype = self.ret[1]
                    self.body.append(f"    return {self.ret[0]};")
        except Unsupported:
            return None
        ints = [f"char* p{n}" for n in range(max([k for k in self.params if k < 100], default=-1) + 1)]
        floats = [f"{self.params.get(100 + n, 'f32')} f{n}"
                  for n in range(max([k - 100 for k in self.params if k >= 100], default=-1) + 1)]
        return "\n".join([f"{rtype} fn_{addr:x}({', '.join(ints + floats)}) {{", *self.body, "}"]), self.decls

    def step(self, x, last: int) -> bool:
        m, o = x.mnemonic, x.operands
        if x.writeback:
            return False
        if any(op.type == arm64.ARM64_OP_REG and regname(op.reg) == "sp" for op in o) or \
                any(op.type == arm64.ARM64_OP_MEM and regname(op.mem.base) in ("sp", "x29") for op in o):
            return False
        if m == "adrp":
            n = self.key(regname(o[0].reg))
            self.pages[n] = o[1].imm
            self.vals.pop(n, None)
            self.poison.discard(n)
        elif m == "add" and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM \
                and self.key(regname(o[1].reg)) in self.pages:
            a = self.pages[self.key(regname(o[1].reg))] + o[2].imm
            s = self.orig.img.cstring(a - BASE)
            if s is not None and a not in self.orig.sizes:
                self.write(regname(o[0].reg), f"(char*){refl_attrs.c_string(s)}", "char*")
            else:
                self.write(regname(o[0].reg), self.global_ref(a, False), "char*")
        elif m == "ldr" and len(o) == 2 and o[1].type == arm64.ARM64_OP_MEM \
                and self.key(regname(o[1].mem.base)) in self.pages:
            slot = self.pages[self.key(regname(o[1].mem.base))] + o[1].mem.disp
            r = self.orig.img.relocs.get(slot - BASE)
            rt = regname(o[0].reg)
            if r is not None:
                target = self.orig.img.pointer(slot - BASE)
                if target is None or rt[0] != "x":
                    return False
                self.write(rt, self.global_ref(BASE + target, True), "char*")
            else:
                t = {"x": "u64", "w": "u32", "s": "f32", "d": "f64"}.get(rt[0])
                if t is None:
                    return False
                self.write(rt, f"*({t}*){self.global_ref(slot, False)}", t)
        elif m in LOADS and len(o) == 2:
            rt = regname(o[0].reg)
            if rt[0] in "sd" and m in ("ldr", "ldur"):
                t = "f32" if rt[0] == "s" else "f64"
                self.write(rt, f"*({t}*){self.address(o[1])}", t)
                return True
            t = LOADS[m] or width_type(rt)
            if rt[0] not in "xw":
                return False
            dest_t = width_type(rt) if t[0] == "s" else (t if LOADS[m] else width_type(rt))
            self.write(rt, f"({dest_t})*({t}*){self.address(o[1])}", dest_t)
        elif m in STORES and len(o) == 2:
            rt = regname(o[0].reg)
            if rt[0] in "sd" and m in ("str", "stur"):
                v, t = self.read(rt)
                self.line(f"*({t}*){self.address(o[1])} = {v};")
                return True
            if rt[0] not in "xw":
                return False
            t = STORES[m] or width_type(rt)
            v, _ = self.read(rt)
            self.line(f"*({t}*){self.address(o[1])} = ({t})(u64)({v});")
        elif m == "stp" and len(o) == 3:
            for k in range(2):
                rt = regname(o[k].reg)
                if rt[0] not in "xw":
                    return False
                t = width_type(rt)
                v, _ = self.read(rt)
                base, _ = self.read(regname(o[2].mem.base))
                off = o[2].mem.disp + k * (8 if t == "u64" else 4)
                self.line(f"*({t}*)((char*)({base}) + {off}) = ({t})(u64)({v});")
        elif m == "ldp" and len(o) == 3 and regname(o[0].reg)[0] in "xw":
            base, _ = self.read(regname(o[2].mem.base))
            for k in range(2):
                rt = regname(o[k].reg)
                t = width_type(rt)
                off = o[2].mem.disp + k * (8 if t == "u64" else 4)
                self.write(rt, f"*({t}*)((char*)({base}) + {off})", t)
        elif m in ("mov", "movz") and len(o) == 2:
            rd = regname(o[0].reg)
            if o[1].type == arm64.ARM64_OP_IMM:
                mask = 0xFFFFFFFF if rd[0] == "w" else 0xFFFFFFFFFFFFFFFF
                v = o[1].imm & mask
                self.write(rd, f"{v:#x}", width_type(rd), v)
            else:
                if rd[0] not in "xw":
                    return False
                v, t = self.read(regname(o[1].reg))
                self.write(rd, v if rd[0] == "x" else f"(u32)(u64)({v})", t if rd[0] == "x" else "u32")
        elif m == "movk" and len(o) == 2:
            rd = regname(o[0].reg)
            n = self.key(rd)
            if n not in self.consts:
                return False
            shift = o[1].shift.value if o[1].shift.type else 0
            v = (self.consts[n] & ~(0xFFFF << shift)) | (o[1].imm << shift)
            self.write(rd, f"{v:#x}", width_type(rd), v)
        elif m == "fmov" and len(o) == 2:
            rd = regname(o[0].reg)
            if rd[0] not in "sd":
                return False
            t = "f32" if rd[0] == "s" else "f64"
            if o[1].type == arm64.ARM64_OP_FP:
                self.write(rd, repr(float(o[1].fp)) + ("f" if t == "f32" else ""), t)
            elif regname(o[1].reg) in ("wzr", "xzr"):
                self.write(rd, "0.0f" if t == "f32" else "0.0", t)
            elif regname(o[1].reg)[0] == rd[0]:
                v, _ = self.read(regname(o[1].reg))
                self.write(rd, v, t)
            else:
                return False
        elif m in ("add", "sub", "and", "orr", "eor") and len(o) == 3:
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            a, ta = self.read(regname(o[1].reg))
            b, tb = self.operand(o[2], rd)
            op = {"add": "+", "sub": "-", "and": "&", "orr": "|", "eor": "^"}[m]
            if ta == "char*" and tb != "char*" and rd[0] == "x" and m in ("add", "sub"):
                self.write(rd, f"({a}) {op} {b}", "char*")
            else:
                t = width_type(rd)
                self.write(rd, f"({t})(u64)({a}) {op} ({t})({b})", t)
        elif m in ("lsl", "lsr", "asr") and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM:
            rd = regname(o[0].reg)
            a, _ = self.read(regname(o[1].reg))
            t = width_type(rd)
            st = t if m != "asr" else ("s64" if t == "u64" else "s32")
            self.write(rd, f"({t})(({st})(u64)({a}) {'<<' if m == 'lsl' else '>>'} {o[2].imm})", t)
        elif m == "cmp" and len(o) == 2:
            a, _ = self.read(regname(o[0].reg))
            b, _ = self.operand(o[1], regname(o[0].reg))
            self.cmp = (a, b, regname(o[0].reg)[0])
        elif m == "fcmp" and len(o) == 2:
            a, t = self.read(regname(o[0].reg))
            b = "0.0" if o[1].type == arm64.ARM64_OP_FP else self.read(regname(o[1].reg))[0]
            self.cmp = (a, b, "f", t)
        elif m == "cset" and len(o) == 1:
            self.write(regname(o[0].reg), self.cond(x.cc, False), "bool")
        elif m.startswith("b.") and x.address < o[0].imm <= last:
            self.open_if(self.cond(x.cc, True), o[0].imm)
        elif m in ("cbz", "cbnz") and x.address < o[1].imm <= last:
            v, _ = self.read(regname(o[0].reg))
            t = width_type(regname(o[0].reg))
            self.open_if(f"({t})(u64)({v}) {'!=' if m == 'cbz' else '=='} 0", o[1].imm)
        elif m in ("tbz", "tbnz") and x.address < o[2].imm <= last:
            v, _ = self.read(regname(o[0].reg))
            self.open_if(f"((u64)({v}) >> {o[1].imm} & 1) {'!=' if m == 'tbz' else '=='} 0", o[2].imm)
        elif m == "bl":
            call = self.call(f"fn_{o[0].imm:x}")
            self.clobber()
            self.write("x0", call, "char*")
            self.written = set()
        elif m == "blr":
            ptr, _ = self.read(regname(o[0].reg))
            call = self.call("", ptr)
            self.clobber()
            self.write("x0", call, "char*")
            self.written = set()
        else:
            return False
        return True


def candidates(orig: asmdiff.Original) -> list[int]:
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
    return [a for a in rows if a not in skip and orig.sizes.get(a, MAX_SIZE + 1) <= MAX_SIZE]


HEADER = ['#include "types.h"', '#define HIDDEN __attribute__((visibility("hidden")))', ""]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)
    cands = candidates(orig)

    work = ROOT / "build" / "gen_lift"
    work.mkdir(parents=True, exist_ok=True)
    lifted: list[dict[int, tuple[str, set[str]]]] = []
    for extra, returns in VARIANTS:
        defs = {}
        for a in cands:
            r = Lifter(orig, a, extra, returns).lift()
            if r:
                defs[a] = r
        lifted.append(defs)

    def build(k: int) -> set[int]:
        defs = lifted[k]
        src, obj = work / f"variant_{k}.cpp", work / f"variant_{k}.o"
        for _ in range(5):
            decls = sorted({d for _, ds in defs.values() for d in ds})
            lines = HEADER + decls
            owner = {}  # line number of each definition's first line -> address
            for a in sorted(defs):
                owner[len(lines) + 1] = a
                lines += defs[a][0].split("\n")
            src.write_text("\n".join(lines) + "\n", encoding="utf-8")
            r = subprocess.run(buildcfg.cxx_command(src, obj, ["-ferror-limit=0"]), capture_output=True, text=True)
            if r.returncode == 0:
                return {res.address for res in asmdiff.diff_object(obj, orig) if res.matching} & set(defs)
            # Drop the functions with errors (a lifter gap) and try again.
            starts = sorted(owner)
            bad = set()
            for m in re.finditer(re.escape(src.name) + r":(\d+):\d+: error", r.stderr):
                i = bisect.bisect_right(starts, int(m.group(1))) - 1
                if i < 0:
                    raise SystemExit(f"variant {k}: error outside a definition:\n{r.stderr[:4000]}")
                bad.add(owner[starts[i]])
            if not bad:
                raise SystemExit(f"variant {k} failed to compile:\n{r.stderr[:4000]}")
            defs = {a: d for a, d in defs.items() if a not in bad}
        raise SystemExit(f"variant {k} still fails to compile")

    with ThreadPoolExecutor() as pool:
        matched = list(pool.map(build, range(len(VARIANTS))))
    chosen: dict[int, tuple[str, set[str]]] = {}
    for k, ok in enumerate(matched):
        for a in ok:
            chosen.setdefault(a, lifted[k][a])

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = ["// Generated by tools/gen_lift.py from main; regenerate rather than edit.",
            "// Small functions lifted from their instructions, byte-identical to the original."]
    ok = sorted(chosen)
    for k in range(0, len(ok), CHUNK):
        chunk = ok[k:k + CHUNK]
        decls = sorted({d for a in chunk for d in chosen[a][1]})
        (OUT / f"lifted_{k // CHUNK:02d}.cpp").write_text(
            "\n".join(note + HEADER + decls + [""] + [chosen[a][0] + "\n" for a in chunk]),
            encoding="utf-8", newline="\n")
    lifted_any = set().union(*lifted)
    print(f"{len(cands)} candidates, {len(lifted_any)} lifted; {len(ok)} byte-identical "
          f"({sum(orig.sizes[a] for a in ok)} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
