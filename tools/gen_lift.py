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
     in the code. Each function is lifted in several VARIANTS; every variant is compiled in one
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
MAX_SIZE = 512
CHUNK = 1000  # functions per generated file
# Functions per scratch TU: with -ffunction-sections, more than ~65k sections overflow the ELF
# section index (SHN_XINDEX), which the object reader doesn't follow.
SCRATCH_CHUNK = 8000
NAME_RE = re.compile(r"^[A-Za-z_][\w:<>,*& ]*\bfn_([0-9a-f]{10})\(")  # a declaration or definition
# (minimum call arguments, return mode): tried in this order. Return modes: "value" returns x0
# (or s0) as set, "void" returns nothing, "ref" returns a pointer as a reference (Clang 9 then
# doesn't turn a final call into a tail call, as in the original's `bl; ret`).
VARIANTS = [(n, mode) for n in range(3) for mode in ("value", "void", "ref")]
# The same, through FlatLifter, for functions no structured variant can lift.
# Layouts of the stack-locals array, (span, alignment), for functions with locals.
STACK_LAYOUTS = [("gap", 16), ("gap", 8), ("tight", 8)]

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


def is_int(t: str) -> bool:
    return t in ("u64", "u32", "u16", "u8", "bool", "char*")


class State:
    """Register knowledge on one path. Keys: n for x<n>/w<n>, 100 + n for s<n>/d<n>."""

    def __init__(self):
        self.vals: dict[int, tuple[str, str]] = {}  # key -> (expression, C type)
        self.pages: dict[int, int] = {}            # key -> adrp page
        self.consts: dict[int, int] = {}
        self.poison: set[int] = set()              # differs between merged paths
        self.called = False
        self.written: set[int] = set()             # argument registers written since the last call
        self.cmp: tuple | None = None

    def copy(self) -> "State":
        s = State()
        s.vals, s.pages, s.consts = dict(self.vals), dict(self.pages), dict(self.consts)
        s.poison, s.called, s.written, s.cmp = set(self.poison), self.called, set(self.written), self.cmp
        return s


class Block:
    """An open `if` (kind "if") or its `else` (kind "else")."""

    def __init__(self, end: int, before: State, if_line: int):
        self.end, self.before, self.if_line = end, before, if_line
        self.kind = "if"
        self.then: State | None = None  # state at the end of the then-part (kind "else")
        self.then_live = False
        self.else_line = -1


class Lifter:
    def __init__(self, orig: asmdiff.Original, addr: int, extra: int, returns: bool,
                 stack: tuple[str, int] | None = None):
        self.orig, self.addr, self.extra, self.returns = orig, addr, extra, returns
        self.stack = stack            # (layout, alignment) of the locals array; None: no locals
        self.frame: dict | None = None
        self.s = State()
        self.live = True
        self.params: dict[int, str] = {}  # key -> C type
        self.body: list[str] = []
        self.decls: set[str] = set()
        self.blocks: list[Block] = []
        self.rets: list[tuple[int, str, tuple | None]] = []  # (line, "value"|"tail", payload)
        self.phis = 0
        self.sret = False

    @property
    def depth(self) -> int:
        return len(self.blocks)

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
        s = self.s
        if n in s.pages or n in s.poison:
            raise Unsupported("unknown value")
        if n not in s.vals:
            if s.called or (n > 8 and not 100 <= n <= 107):
                raise Unsupported(r)
            t = ("u32" if r[0] == "w" else "char*") if n < 100 else ("f32" if r[0] == "s" else "f64")
            if n == 8:
                # The indirect result register: the function returns a struct built in place.
                self.sret = True
                v = ("(char*)&ret", t)
            else:
                self.params[n] = t
                v = (f"p{n}" if n < 100 else f"f{n - 100}", t)
            # A parameter holds on every path that hasn't made a call.
            for st in [s] + [b.before for b in self.blocks] + [b.then for b in self.blocks if b.then]:
                if not st.called and n not in st.vals and n not in st.poison:
                    st.vals[n] = v
        e, t = s.vals[n]
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
        s = self.s
        s.vals[n] = (var, t)
        s.pages.pop(n, None)
        s.poison.discard(n)
        if const is None:
            s.consts.pop(n, None)
        else:
            s.consts[n] = const
        if n <= 7 or 100 <= n <= 107:
            s.written.add(n)

    # -- control flow -------------------------------------------------------------------------

    def open_if(self, cond: str, target: int) -> None:
        if self.blocks and target > self.blocks[-1].end:
            raise Unsupported("crossing branches")
        self.line(f"if ({cond}) {{")
        self.blocks.append(Block(target, self.s.copy(), len(self.body) - 1))

    def to_else(self, target: int) -> None:
        """`b target` ending a then-part whose skip target is the next instruction."""
        b = self.blocks[-1]
        if b.kind != "if" or (len(self.blocks) > 1 and target > self.blocks[-2].end):
            raise Unsupported("else")
        b.kind, b.then, b.then_live = "else", self.s, self.live
        b.end = target
        self.body.append("    " * self.depth + "} else {")
        b.else_line = len(self.body) - 1
        self.s, self.live = b.before.copy(), True

    def close_to(self, address: int) -> None:
        while self.blocks and self.blocks[-1].end <= address:
            if self.blocks[-1].end < address:
                raise Unsupported("branch into an instruction")
            b = self.blocks.pop()
            if b.kind == "if":
                a, a_live, c, c_live = self.s, self.live, b.before, True
            else:
                a, a_live, c, c_live = b.then, b.then_live, self.s, self.live
            if a_live and c_live:
                merged = self.merge(b, a, c)
            else:
                merged = a if a_live else c
            self.body.append("    " * (self.depth + 1) + "}")
            self.s, self.live = merged, a_live or c_live

    def merge(self, b: Block, a: State, c: State) -> State:
        """Join two live paths: a (end of the then-part) and c (skip path or else-part)."""
        m = a.copy()
        m.called = a.called or c.called
        m.written = a.written | c.written
        m.cmp = None
        m.poison = a.poison | c.poison
        m.pages = {n: p for n, p in a.pages.items() if c.pages.get(n) == p}
        m.poison |= (set(a.pages) | set(c.pages)) - set(m.pages)
        m.consts = {n: v for n, v in a.consts.items() if c.consts.get(n) == v}
        m.vals = {}
        decls, then_end, else_end = [], [], []
        for n in sorted(set(a.vals) | set(c.vals)):
            va, vc = a.vals.get(n), c.vals.get(n)
            if va == vc:
                m.vals[n] = va
                continue
            if va is None or vc is None or va[1] != vc[1] or n in m.poison:
                m.poison.add(n)
                continue
            var, t = f"m{self.phis}", va[1]
            self.phis += 1
            if b.kind == "if":
                decls.append(f"{t} {var} = {vc[0]};")   # c is the state before the if
                then_end.append(f"{var} = {va[0]};")
            else:
                decls.append(f"{t} {var};")
                then_end.append(f"{var} = {va[0]};")
                else_end.append(f"{var} = {vc[0]};")
            m.vals[n] = (var, t)
        ind = "    " * (self.depth + 2)
        if b.kind == "if":
            self.body += [ind + s for s in then_end]
        else:
            self.body += [ind + s for s in else_end]
            self.body[b.else_line:b.else_line] = [ind + s for s in then_end]
        outer = "    " * (self.depth + 1)
        self.body[b.if_line:b.if_line] = [outer + s for s in decls]
        # Lines were inserted before later positions: shift recorded return lines.
        shift_if, shift_else = len(decls), len(then_end) if b.kind == "else" else 0
        self.rets = [(ln + (shift_if if ln >= b.if_line else 0) + (shift_else if b.kind == "else" and ln >= b.else_line else 0), k, p)
                     for ln, k, p in self.rets]
        return m

    def branch_ok(self, src: int, target: int, last: int) -> bool:
        return src < target <= last

    def temp_ptr(self, e: str) -> str:
        var = f"a{len(self.body)}"
        self.line(f"char* {var} = {e};")
        return var

    def terminate(self, kind: str, payload) -> None:
        self.rets.append((len(self.body), kind, payload))
        self.line("@RET@")
        self.live = False

    # -- operands -----------------------------------------------------------------------------

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

    def address(self, x, op) -> str:
        """The memory operand's address; applies pre/post-index writeback to the base."""
        if op.mem.index or op.shift.type:
            raise Unsupported("indexed")
        base_reg = regname(op.mem.base)
        if base_reg in ("sp", "x29"):
            if x.writeback or not self.frame:
                raise Unsupported("stack writeback")
            base = self.frame["sp"] if base_reg == "sp" else self.frame["fp"]
            return self.local(base + op.mem.disp)
        n = self.key(base_reg)
        if n in self.s.pages:
            if x.writeback:
                raise Unsupported("page writeback")
            a = self.s.pages[n] + op.mem.disp
            if self.orig.img.relocs.get(a - BASE) is not None:
                raise Unsupported("GOT slot as data")
            return self.global_ref(a, False)
        base, t = self.read(base_reg)
        if not x.writeback:
            return f"((char*)({base}) + {op.mem.disp})"
        post = [o for o in x.operands if o.type == arm64.ARM64_OP_IMM]
        step = post[-1].imm if post else op.mem.disp
        addr = f"((char*)({base}) + {0 if post else op.mem.disp})"
        self.write(base_reg, f"(char*)({base}) + {step}", "char*")
        return addr

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
        if cc not in CONDS or not self.s.cmp:
            raise Unsupported("condition")
        opr, signed = CONDS[cc]
        c = self.s.cmp
        if c[2] == "f":
            expr = f"({c[3]})({c[0]}) {opr} ({c[3]})({c[1]})"
            return f"!({expr})" if negate else expr
        t = ("s64" if c[2] == "x" else "s32") if signed else ("u64" if c[2] == "x" else "u32")
        return f"({t})(u64)({c[0]}) {NEGATE[opr] if negate else opr} ({t})({c[1]})"

    def args(self) -> list[tuple[str, str]]:
        w = self.s.written
        n = max([k for k in w if k < 100], default=-1) + 1
        n = max(n, self.extra)
        fl = max([k - 100 for k in w if k >= 100], default=-1) + 1
        floats = []
        for i in range(fl):
            t = self.s.vals.get(100 + i, ("", "f32"))[1]
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
        s = self.s
        for n in list(s.vals):
            if n <= 18 or 100 <= n <= 107 or n >= 116:
                del s.vals[n]
        s.pages = {n: p for n, p in s.pages.items() if n > 18}
        s.consts = {n: v for n, v in s.consts.items() if n > 18}
        s.poison = {n for n in s.poison if 18 < n < 100 or 108 <= n < 116}
        s.written = set()
        s.called = True
        s.cmp = None

    # -- driver -------------------------------------------------------------------------------

    def lift(self) -> tuple[str, set[str]] | None:
        orig, addr = self.orig, self.addr
        code = orig.function(addr)
        ins = list(CS.disasm(code, addr))
        if not ins or len(ins) * 4 != len(code):
            return None
        end = addr + len(code)
        last = ins[-1].address
        if not self.setup_frame(ins):
            return None
        try:
            for x in ins:
                self.close_to(x.address)
                if not self.live:
                    return None  # unreachable code (or a jump-around we don't model)
                if is_frame(x):
                    continue
                m, o = x.mnemonic, x.operands
                if m == "ret":
                    v = None
                    if 0 in self.s.vals and 0 not in self.s.poison:
                        v = self.s.vals[0]
                    elif 100 in self.s.vals and 100 not in self.s.poison:
                        v = self.s.vals[100]
                    self.terminate("value", v)
                elif m == "b" and not addr <= o[0].imm < end:
                    self.terminate("tail", self.call(f"fn_{o[0].imm:x}"))
                elif m == "br":
                    ptr, _ = self.read(regname(o[0].reg))
                    self.terminate("tail", self.call("", ptr))
                elif m == "b":
                    t = o[0].imm
                    if not (x.address < t <= last and self.blocks and self.blocks[-1].end == x.address + 4):
                        return None
                    self.to_else(t)
                elif not self.step(x, last):
                    return None
            if self.blocks or self.live:
                return None
        except Unsupported:
            return None
        return self.finish()

    def finish(self) -> tuple[str, set[str]] | None:
        tails = any(k == "tail" for _, k, _ in self.rets)
        if self.sret:
            if self.returns != "value":
                return None
            for ln, k, p in self.rets:
                ind = self.body[ln][:len(self.body[ln]) - len(self.body[ln].lstrip())]
                # A final call (e.g. an initialiser given &ret) stays a tail call: ret is
                # built in place, so nothing is left to do after it.
                self.body[ln] = ind + (f"{p};\n{ind}return ret;" if k == "tail" else "return ret;")
            return self.signature("UnkResult", ["    UnkResult ret;"] + self.body), self.decls
        if self.returns != "void":
            if any(k == "value" and p is None for _, k, p in self.rets):
                return None
            types = [p[1] for _, k, p in self.rets if k == "value"]
            rtype = "char*" if tails else types[0]
            if self.returns == "ref":
                if rtype != "char*" or tails:
                    return None
                rtype = "char&"
            if any((t[0] == "f") != (rtype[0] == "f") for t in types) or \
                    (rtype[0] == "f" and any(t != rtype for t in types)):
                return None
        else:
            rtype = "void"
        for ln, k, p in self.rets:
            ind = self.body[ln][:len(self.body[ln]) - len(self.body[ln].lstrip())]
            if k == "tail":
                s = f"{p};\n{ind}return;" if rtype == "void" else \
                    f"return {p};"
            elif rtype == "void":
                s = "return;"
            elif rtype == "char&":
                if p[1] != "char*":
                    return None
                s = f"return *{p[0]};"
            elif p[1] == rtype:
                s = f"return {p[0]};"
            else:
                s = f"return ({rtype})(u64)({p[0]});"
            self.body[ln] = ind + s
        return self.signature(rtype, self.body), self.decls

    def signature(self, rtype: str, body: list[str]) -> str:
        # A parameter first read as w<n> is 32-bit (u32), otherwise a pointer.
        ints = [f"{self.params.get(n, 'char*')} p{n}"
                for n in range(max([k for k in self.params if k < 100], default=-1) + 1)]
        floats = [f"{self.params.get(100 + n, 'f32')} f{n}"
                  for n in range(max([k - 100 for k in self.params if k >= 100], default=-1) + 1)]
        if self.frame:
            f = self.frame
            body = [f"    alignas({f['align']}) char stk[{f['size']:#x}];"] + body
        return "\n".join([f"{rtype} fn_{self.addr:x}({', '.join(ints + floats)}) {{", *body, "}"])

    def setup_frame(self, ins) -> bool:
        """Stack locals: the prologue gives sp and fp (offsets from sp at entry) and the save
        slots; the locals become one char array over the gap between save slots that holds the
        lowest local ("gap"), or over just the bytes accessed ("tight")."""
        delta, fp, saves = 0, None, []
        k = 0
        while k < len(ins) and is_frame(ins[k]):
            x = ins[k]
            m, o = x.mnemonic, x.operands
            if m == "sub" and regname(o[0].reg) == "sp":
                delta -= o[2].imm
            elif m == "add" and regname(o[0].reg) == "x29":
                fp = delta + o[2].imm
            elif m == "mov" and regname(o[0].reg) == "x29":
                fp = delta
            elif m in ("stp", "str"):
                mem = [op for op in o if op.type == arm64.ARM64_OP_MEM][0]
                if x.writeback:
                    delta += mem.mem.disp
                    at = delta
                else:
                    at = delta + mem.mem.disp
                n = len([op for op in o if op.type == arm64.ARM64_OP_REG])
                saves.append((at, at + 8 * n))
            k += 1
        refs = []  # (offset from sp at entry, size)
        for x in ins[k:]:
            if is_frame(x):
                continue
            o = x.operands
            for op in o:
                if op.type == arm64.ARM64_OP_MEM and regname(op.mem.base) in ("sp", "x29"):
                    base = delta if regname(op.mem.base) == "sp" else fp
                    if base is None or x.writeback:
                        return False
                    regs = [r for r in o if r.type == arm64.ARM64_OP_REG]
                    w = {"b": 1, "h": 2}.get(x.mnemonic[-1], 8 if regname(regs[0].reg)[0] in "xd" else 4)
                    refs.append((base + op.mem.disp, w * len(regs)))
            if x.mnemonic in ("add", "sub", "mov") and len(o) >= 2 and o[1].type == arm64.ARM64_OP_REG \
                    and regname(o[1].reg) in ("sp", "x29") and regname(o[0].reg) not in ("sp", "x29"):
                base = delta if regname(o[1].reg) == "sp" else fp
                if base is None or (len(o) == 3 and o[2].type != arm64.ARM64_OP_IMM):
                    return False
                imm = o[2].imm if len(o) == 3 else 0
                refs.append((base + (-imm if x.mnemonic == "sub" else imm), 1))
        if not refs:
            self.frame = None
            return True
        if self.stack is None:
            return False
        lo = min(r[0] for r in refs)
        hi = max(r[0] + r[1] for r in refs)
        layout, align = self.stack
        if layout == "gap":
            start = max([e for s, e in saves if e <= lo] + [delta])
            end = min([s for s, e in saves if s > lo] + [max(hi, 0)])
        else:
            start, end = lo, lo + ((hi - lo + 7) & ~7)
        if not (start <= lo and hi <= end):
            return False
        self.frame = {"sp": delta, "fp": fp, "start": start, "size": end - start, "align": align}
        return True

    def local(self, offset: int) -> str:
        """The address of the stack location `offset` bytes from sp at entry."""
        return f"(stk + {offset - self.frame['start']:#x})"

    def step(self, x, last: int) -> bool:
        m, o = x.mnemonic, x.operands
        if self.frame and m in ("add", "sub", "mov") and len(o) >= 2 and o[1].type == arm64.ARM64_OP_REG \
                and regname(o[1].reg) in ("sp", "x29") and regname(o[0].reg) not in ("sp", "x29"):
            base = self.frame["sp"] if regname(o[1].reg) == "sp" else self.frame["fp"]
            imm = o[2].imm if len(o) == 3 else 0
            self.write(regname(o[0].reg), self.local(base + (-imm if m == "sub" else imm)), "char*")
            return True
        if any(op.type == arm64.ARM64_OP_REG and regname(op.reg) == "sp" for op in o) or \
                (not self.frame and any(op.type == arm64.ARM64_OP_MEM and regname(op.mem.base) in ("sp", "x29")
                                        for op in o)):
            return False
        mem = [op for op in o if op.type == arm64.ARM64_OP_MEM]
        if x.writeback and not (mem and m in tuple(LOADS) + tuple(STORES) + ("stp", "ldp")):
            return False
        s = self.s
        if m == "adrp":
            n = self.key(regname(o[0].reg))
            s.pages[n] = o[1].imm
            s.vals.pop(n, None)
            s.poison.discard(n)
        elif m == "add" and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM \
                and self.key(regname(o[1].reg)) in s.pages:
            a = s.pages[self.key(regname(o[1].reg))] + o[2].imm
            text = self.orig.img.cstring(a - BASE)
            if text is not None and a not in self.orig.sizes:
                self.write(regname(o[0].reg), f"(char*){refl_attrs.c_string(text)}", "char*")
            else:
                self.write(regname(o[0].reg), self.global_ref(a, False), "char*")
        elif m == "ldr" and len(o) == 2 and o[1].type == arm64.ARM64_OP_MEM \
                and self.key(regname(o[1].mem.base)) in s.pages:
            slot = s.pages[self.key(regname(o[1].mem.base))] + o[1].mem.disp
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
        elif m in LOADS and mem:
            rt = regname(o[0].reg)
            if rt[0] in "sd" and m in ("ldr", "ldur"):
                t = "f32" if rt[0] == "s" else "f64"
                self.write(rt, f"*({t}*){self.address(x, mem[0])}", t)
                return True
            t = LOADS[m] or width_type(rt)
            if rt[0] not in "xw":
                return False
            dest_t = width_type(rt) if t[0] == "s" else (t if LOADS[m] else width_type(rt))
            a = self.address(x, mem[0])
            self.write(rt, f"({dest_t})*({t}*){a}", dest_t)
        elif m in STORES and mem:
            rt = regname(o[0].reg)
            if rt[0] in "sd" and m in ("str", "stur"):
                v, t = self.read(rt)
                self.line(f"*({t}*){self.address(x, mem[0])} = {v};")
                return True
            if rt[0] not in "xw":
                return False
            t = STORES[m] or width_type(rt)
            v, _ = self.read(rt)
            self.line(f"*({t}*){self.address(x, mem[0])} = ({t})(u64)({v});")
        elif m == "stp" and mem:
            rts = [regname(o[k].reg) for k in range(2)]
            if any(r[0] not in "xw" for r in rts):
                return False
            vals = [self.read(r)[0] for r in rts]
            a = self.address(x, mem[0])
            for k, (rt, v) in enumerate(zip(rts, vals)):
                t = width_type(rt)
                self.line(f"*({t}*)({a} + {k * (8 if t == 'u64' else 4)}) = ({t})(u64)({v});")
        elif m == "ldp" and mem and regname(o[0].reg)[0] in "xw":
            a = self.address(x, mem[0])
            tmp = self.temp_ptr(a)
            for k in range(2):
                rt = regname(o[k].reg)
                t = width_type(rt)
                self.write(rt, f"*({t}*)({tmp} + {k * (8 if t == 'u64' else 4)})", t)
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
            if n not in s.consts:
                return False
            shift = o[1].shift.value if o[1].shift.type else 0
            v = (s.consts[n] & ~(0xFFFF << shift)) | (o[1].imm << shift)
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
            s.cmp = (a, b, regname(o[0].reg)[0])
        elif m == "fcmp" and len(o) == 2:
            a, t = self.read(regname(o[0].reg))
            b = "0.0" if o[1].type == arm64.ARM64_OP_FP else self.read(regname(o[1].reg))[0]
            s.cmp = (a, b, "f", t)
        elif m == "cset" and len(o) == 1:
            self.write(regname(o[0].reg), self.cond(x.cc, False), "bool")
        elif m.startswith("b.") and self.branch_ok(x.address, o[0].imm, last):
            self.open_if(self.cond(x.cc, True), o[0].imm)
        elif m in ("cbz", "cbnz") and self.branch_ok(x.address, o[1].imm, last):
            v, _ = self.read(regname(o[0].reg))
            t = width_type(regname(o[0].reg))
            self.open_if(f"({t})(u64)({v}) {'!=' if m == 'cbz' else '=='} 0", o[1].imm)
        elif m in ("tbz", "tbnz") and self.branch_ok(x.address, o[2].imm, last):
            v, _ = self.read(regname(o[0].reg))
            self.open_if(f"((u64)({v}) >> {o[1].imm} & 1) {'!=' if m == 'tbz' else '=='} 0", o[2].imm)
        elif m == "bl":
            call = self.call(f"fn_{o[0].imm:x}")
            self.clobber()
            self.write("x0", call, "char*")
            self.s.written = set()
        elif m == "blr":
            ptr, _ = self.read(regname(o[0].reg))
            call = self.call("", ptr)
            self.clobber()
            self.write("x0", call, "char*")
            self.s.written = set()
        else:
            return False
        return True


class FlatLifter(Lifter):
    """For functions the structured lifter rejects (crossing branches, loops): every register
    is a mutable local (integers as u64), branches are gotos to labels. mem2reg rebuilds the
    same SSA form, so the code usually comes out the same."""

    def __init__(self, orig: asmdiff.Original, addr: int, extra: int, returns: str,
                 stack: tuple[str, int] | None = None):
        super().__init__(orig, addr, extra, returns, stack)
        self.types: dict[int, str] = {}  # register key -> C type of its current value (linear)
        self.temps = 0

    @property
    def depth(self) -> int:
        return 0

    def branch_ok(self, src: int, target: int, last: int) -> bool:
        return self.addr <= target <= last

    def storage(self, n: int, t: str) -> str:
        if n < 100:
            return f"X{n}"
        return f"{'S' if t == 'f32' else 'D'}{n - 100}"

    def read(self, r: str) -> tuple[str, str]:
        if r in ("xzr", "wzr"):
            return "0", width_type(r)
        n = self.key(r)
        s = self.s
        if n in s.pages:
            raise Unsupported("bare page")
        if n not in self.types:
            if s.called or (n > 8 and not 100 <= n <= 107):
                raise Unsupported(r)
            if n == 8:
                self.sret = True
                self.types[8] = "char*"
            else:
                t = ("u32" if r[0] == "w" else "char*") if n < 100 else ("f32" if r[0] == "s" else "f64")
                self.params[n] = t
                self.types[n] = t
        t = self.types[n]
        if n >= 100:
            if t != ("f32" if r[0] == "s" else "f64"):
                raise Unsupported("float width")
            return self.storage(n, t), t
        if r[0] == "w":
            return f"(u32)X{n}", "u32"
        return (f"X{n}" if t == "u64" else f"(({t})X{n})"), t

    def write(self, r: str, e: str, t: str, const: int | None = None) -> None:
        n = self.key(r)
        if n < 0:
            return
        if n >= 100:
            self.line(f"{self.storage(n, t)} = {e};")
        else:
            self.line(f"X{n} = (u64)({e});")
        self.types[n] = t
        s = self.s
        s.pages.pop(n, None)
        if const is None:
            s.consts.pop(n, None)
        else:
            s.consts[n] = const
        if n <= 7 or 100 <= n <= 107:
            s.written.add(n)

    def temp_ptr(self, e: str) -> str:
        var = f"t{self.temps}"
        self.temps += 1
        self.line(f"{var} = {e};")
        return var

    def open_if(self, cond: str, target: int) -> None:
        self.line(f"if (!({cond})) goto L_{target:x};")

    def clobber(self) -> None:
        s = self.s
        s.pages = {n: p for n, p in s.pages.items() if n > 18}
        s.consts = {n: v for n, v in s.consts.items() if n > 18}
        s.written = set()
        s.called = True
        s.cmp = None

    def lift(self) -> tuple[str, set[str]] | None:
        orig, addr = self.orig, self.addr
        code = orig.function(addr)
        ins = list(CS.disasm(code, addr))
        if not ins or len(ins) * 4 != len(code):
            return None
        end = addr + len(code)
        last = ins[-1].address
        if not self.setup_frame(ins):
            return None
        targets = set()
        for x in ins:
            for op in x.operands:
                if op.type == arm64.ARM64_OP_IMM and (x.mnemonic in ("b", "cbz", "cbnz", "tbz", "tbnz")
                                                      or x.mnemonic.startswith("b.")) \
                        and addr <= op.imm < end:
                    targets.add(op.imm)
        try:
            for x in ins:
                if x.address in targets:
                    self.body.append(f"L_{x.address:x}:;")
                    self.s.cmp = None if x.address != addr else self.s.cmp
                if is_frame(x):
                    continue
                m, o = x.mnemonic, x.operands
                if m == "ret":
                    v = None
                    if 0 in self.types:
                        v = self.read("x0")
                    elif 100 in self.types:
                        v = self.read("s0" if self.types[100] == "f32" else "d0")
                    self.terminate("value", v)
                elif m == "b" and not addr <= o[0].imm < end:
                    self.terminate("tail", self.call(f"fn_{o[0].imm:x}"))
                elif m == "br":
                    ptr, _ = self.read(regname(o[0].reg))
                    self.terminate("tail", self.call("", ptr))
                elif m == "b":
                    self.line(f"goto L_{o[0].imm:x};")
                elif not self.step(x, last):
                    return None
        except Unsupported:
            return None
        if ins[-1].mnemonic not in ("ret", "b", "br"):
            return None
        # Declarations go first: gotos must not jump over initialisations.
        pre = []
        for n in sorted(k for k in self.types if k < 100):
            init = f" = (u64)p{n}" if n in self.params else (" = (u64)&ret" if n == 8 and self.sret else "")
            pre.append(f"    u64 X{n}{init};")
        for n in sorted(k for k in self.types if k >= 100):
            t = self.types[n]
            init = f" = f{n - 100}" if n in self.params else ""
            pre.append(f"    {t} {self.storage(n, t)}{init};")
        pre += [f"    char* t{i};" for i in range(self.temps)]
        self.body = pre + self.body
        self.rets = [(ln + len(pre), k, p) for ln, k, p in self.rets]
        return self.finish()


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


HEADER = ['#include "types.h"', '#define HIDDEN __attribute__((visibility("hidden")))',
          "// Returned in memory through x8 (built in place); the real size is unknown.",
          "struct UnkResult { char data[0x100]; };", ""]


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
    structured = set().union(*lifted)
    rest = [a for a in cands if a not in structured]
    for extra, returns in VARIANTS:
        defs = {}
        for a in rest:
            r = FlatLifter(orig, a, extra, returns).lift()
            if r:
                defs[a] = r
        lifted.append(defs)
    # Functions with stack locals: each layout of the locals array, structured then flat.
    rest = [a for a in rest if not any(a in d for d in lifted)]
    for stack in STACK_LAYOUTS:
        for cls in (Lifter, FlatLifter):
            for extra, returns in VARIANTS:
                defs = {}
                for a in rest:
                    r = cls(orig, a, extra, returns, stack).lift()
                    if r:
                        defs[a] = r
                lifted.append(defs)

    def build(job: tuple[int, int]) -> set[int]:
        k, part = job
        addrs = sorted(lifted[k])[part * SCRATCH_CHUNK:(part + 1) * SCRATCH_CHUNK]
        defs = {a: lifted[k][a] for a in addrs}
        src, obj = work / f"variant_{k}_{part}.cpp", work / f"variant_{k}_{part}.o"
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

    jobs = [(k, part) for k in range(len(lifted))
            for part in range((len(lifted[k]) + SCRATCH_CHUNK - 1) // SCRATCH_CHUNK)]
    with ThreadPoolExecutor() as pool:
        matched = list(pool.map(build, jobs))
    chosen: dict[int, tuple[str, set[str]]] = {}
    for k in range(len(lifted)):  # variant order decides
        for (kk, _), ok in zip(jobs, matched):
            if kk == k:
                for a in sorted(ok):
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
