#!/usr/bin/env python3
"""Generate src/lifted/: small functions lifted from their instructions.

Tens of thousands of small functions (per-type copy, assign and compare helpers, accessors,
constant returns, thin wrappers around a call or two) follow simple code patterns. This tool
lifts each one into C++ over `char*` parameters and keeps only the byte-identical results:

  1. Candidates are functions of at most MAX_SIZE bytes that aren't defined elsewhere in src/.
  2. A lifter turns the code into statements: loads and stores at fixed offsets, immediates,
     add/sub, compare + cset, forward branches to the epilogue (`if (!cond) { ... }`), calls
     and tail calls (direct or through a register), string literals and global addresses,
     and scalar float code (s/d registers: arithmetic, fcmp/fccmp, fcsel, conversions, bit
     casts; a call's result is a float when s0/d0 is read after it).
     Saves and restores of callee-saved registers are skipped. Anything else is rejected.
     Callees are declared in namespace `callee` (one overload per argument list), so their
     names never clash with the functions being defined; globals are `g_unk_<addr>` (reached
     through the GOT) or hidden `h::g_unk_<addr>` (addressed directly).
  3. The number of arguments a call passes and whether a value is returned are not visible
     in the code. Each function is lifted in several VARIANTS; every variant is compiled in one
     scratch translation unit, and the first byte-identical variant of each function is kept.
     Pass-through: a call also names the argument registers its callee reads (tools/abi_facts.py)
     that the function still holds, its own incoming arguments included, and an indirect call
     those its callers set for it (abi_facts' `fwd`). On AArch64 they reach the callee either
     way, so the bytes are the same; an x86-64 compile of the port passes only what a call names.
     Each variant is tried with pass-through first and, where that named more, as before.
  4. Functions that compile but don't match get a second round per ROUND2 rewrite of the
     statements: field stores in ascending offset order ("sort"), accesses through may_alias
     types so stores force reloads as in the original ("alias"), and load/store copies as
     memcpy ("copy").

Usage: python tools/gen_lift.py --version 2.1.0 [--plain]   (--plain: no pass-through)
Outputs are deterministic (the facts come from one complete table, abi_facts.AbiFacts.complete; its
cache is build/port/abi_facts.pickle). Regenerate, never edit them. There is one file per 64 KiB of
address space, and each function follows its own declarations, so

  python tools/gen_lift.py --prune

drops the lifted copies of functions another source now defines (a new clone, a hand-written
function, a merge) in seconds, with the result a full run would give for them.

To measure a lifter change in a minute instead of a full run, --report (implied by --only and
--sample) lifts, compiles and diffs a subset in build/gen_lift_report/ without touching
src/lifted/, and prints counts and bytes by outcome and size band: matched, lifted and
byte-identical but naming a wrong relocation target (callee, global or GOT slot; never emitted),
lifted but not byte-identical, or rejected with the reason from the attempt that got furthest
(the first unsupported mnemonic, or a category such as a register read before its definition).
  python tools/gen_lift.py --version 2.1.0 --sample 500 --seed 1 [--examples 3]
  python tools/gen_lift.py --version 2.1.0 --only 7100123456,7100123460   (or --only @addrs.txt)
"""

from __future__ import annotations

import argparse
import bisect
import csv
import math
import random
import re
import shutil
import struct
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
MAX_SIZE = 16384
RANGE_BITS = 16  # one generated file per 64 KiB of address space, so a file's contents depend on its range only
# Functions per scratch TU: with -ffunction-sections, more than ~65k sections overflow the ELF
# section index (SHN_XINDEX), which the object reader doesn't follow.
SCRATCH_CHUNK = 8000
NAME_RE = re.compile(r"^[A-Za-z_][\w:<>,*& ]*\bfn_([0-9a-f]{10})\(")  # a declaration or definition
TAGGED_DIRS = ("reflection", "reflection_types", "reflection_statics", "clones", "tables")  # generator outputs
TAG_RE = re.compile(r"(?:UnkRefl_fn_|UnkClone_fn_|UnkTable_fn_|DREAD_REFL_\w*GETTER\()([0-9a-f]{10})\b")  # tagged definitions
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
# Float operations ({s}: "f" for f32, "" for f64, as in the libm builtins' names).
FBINARY = {"fadd": "{a} + {b}", "fsub": "{a} - {b}", "fmul": "{a} * {b}", "fdiv": "{a} / {b}",
           "fnmul": "-({a} * {b})", "fabd": "__builtin_fabs{s}({a} - {b})",
           "fminnm": "__builtin_fmin{s}({a}, {b})", "fmaxnm": "__builtin_fmax{s}({a}, {b})",
           # NaN-propagating: an unordered select against a constant.
           "fmin": "!({a} >= {b}) ? {a} : {b}", "fmax": "!({a} <= {b}) ? {a} : {b}"}
FUNARY = {"fneg": "-({a})", "fabs": "__builtin_fabs{s}({a})", "fsqrt": "__builtin_sqrt{s}({a})",
          "frintm": "__builtin_floor{s}({a})", "frintp": "__builtin_ceil{s}({a})",
          "frintz": "__builtin_trunc{s}({a})", "frinta": "__builtin_round{s}({a})",
          "frintx": "__builtin_rint{s}({a})", "frinti": "__builtin_nearbyint{s}({a})"}
# Fused multiply-add forms (-ffp-contract=on fuses them within one expression).
FTERNARY = {"fmadd": "{c} + {a} * {b}", "fmsub": "{c} - {a} * {b}",
            "fnmadd": "-({a} * {b}) - {c}", "fnmsub": "{a} * {b} - {c}"}
# Float to integer conversions: rounding builtin applied before the cast.
FTOINT = {"fcvtzs": "", "fcvtzu": "", "fcvtms": "__builtin_floor{s}", "fcvtmu": "__builtin_floor{s}",
          "fcvtps": "__builtin_ceil{s}", "fcvtpu": "__builtin_ceil{s}", "fcvtas": "__builtin_round{s}",
          "fcvtau": "__builtin_round{s}"}


# Inlined helpers built on weak function-local statics, by the GOT slot of their guard: the
# statement standing for the guard check and its out-of-line initialisation, and the calls
# right after the check that belong to the helper. Their definitions are in HEADER (namespace
# lift_guard), written like those in include/ (UnkReflStatics.h, CStrId.h).
GUARD_ACQUIRE = 0x7101238da0  # __cxa_guard_acquire
DSO_HANDLE = 0x7101a19000  # __dso_handle (passed to __cxa_atexit): an address, not the string ""
GUARDS = {
    0x7101cba070: ("lift_guard::EnsureStringPool();", [0x710007fdb4]),
    0x7101cba0c0: ("lift_guard::EnsureFilePathPool();", [0x710007fdb4]),
    0x7101cbbb20: ("(void)lift_guard::GetSingleton();", []),
    0x7101cbbcd8: ("(void)lift_guard::TypeId<lift_guard::TypeIdTag>::id();", []),
    0x7101cbbcf8: ("(void)lift_guard::TypeId<lift_guard::TypeIdTag>::id();", []),
}


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
                   or (r[0] == "d" and r[1:].isdigit() and 8 <= int(r[1:]) <= 15) for r in regs)
    if m in ("add", "sub") and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM:
        return (regname(o[0].reg), regname(o[1].reg)) in (("x29", "sp"), ("sp", "sp"))
    return m == "mov" and len(o) == 2 and regname(o[0].reg) == "x29" and regname(o[1].reg) == "sp"


def fkey(r: str) -> int | None:
    """Float register number (s<n>/d<n>), or None."""
    return int(r[1:]) if r[0] in "sd" and r[1:].isdigit() else None


def is_int(t: str) -> bool:
    return t in ("u64", "u32", "u16", "u8", "bool", "char*")


def ftype(r: str) -> str | None:
    """C type of a float register name (s<n>: f32, d<n>: f64), or None."""
    return {"s": "f32", "d": "f64"}.get(r[0]) if fkey(r) is not None else None


def float_literal(bits: int, t: str) -> str | None:
    """A float constant with these bits, as an exact hex literal (None for inf/NaN)."""
    v = struct.unpack("<f", struct.pack("<I", bits & 0xFFFFFFFF))[0] if t == "f32" \
        else struct.unpack("<d", struct.pack("<Q", bits & 0xFFFFFFFFFFFFFFFF))[0]
    if not math.isfinite(v):
        return None
    return f"({v.hex()}{'f' if t == 'f32' else ''})"


ACCESS_RE = re.compile(r"\*\((u8|u16|u32|u64|s8|s16|s32|f32|f64)\*\)")
STORE_RE = re.compile(r"^(\s*)\*\((?:u8|u16|u32|u64|f32|f64)\*\)(.+?) = (.+);$")
ASSIGN_RE = re.compile(r"^\s*(?:[\w*]+ )?(\w+) = (.+);$")
IDENT_RE = re.compile(r"\b[A-Za-z_]\w*\b")


def store_slot(a: str) -> tuple[str, int] | None:
    """(base, offset) of a store address the lifter wrote, or None."""
    m = re.fullmatch(r"\(\(char\*\)\((\w+)\) \+ (-?\d+)\)|\((stk) \+ (0x[0-9a-f]+)\)", a)
    if m:
        return (m.group(1), int(m.group(2))) if m.group(1) else ("stk", int(m.group(4), 16))
    m = re.fullmatch(r"\((.+) \+ (\d+)\)", a)
    inner = m and store_slot(m.group(1))
    return (inner[0], inner[1] + int(m.group(2))) if inner else None


def sort_stores(body: list[str]) -> list[str]:
    """Stores through one base within straight-line code, in ascending offset order: the
    order fields are written in source, before the scheduler and store pairing moved them.
    A run of stores (with register-only lines between them) is flushed at its end; it ends
    at any other line, a store through another base, or a line that reassigns a variable a
    pending store reads."""
    out: list[str] = []
    run: list[tuple[int, str, set[str]]] = []  # (offset, line, identifiers it reads)
    key = None  # (indent, base) of the run

    def flush():
        out.extend(ln for _, ln, _ in sorted(run, key=lambda r: r[0]))
        run.clear()

    for ln in body:
        m = STORE_RE.match(ln)
        slot = m and store_slot(m.group(2))
        if slot:
            if not run or key != (m.group(1), slot[0]) or any(off == slot[1] for off, _, _ in run):
                flush()
                key = (m.group(1), slot[0])
            run.append((slot[1], ln, set(IDENT_RE.findall(m.group(2) + " " + m.group(3)))))
            continue
        a = ASSIGN_RE.match(ln)
        if run and a and not re.search(r"\*\(|::|\(\*\)\(", a.group(2)) and ln.startswith(key[0]) \
                and not ln[len(key[0])].isspace() and not any(a.group(1) in ids for _, _, ids in run):
            out.append(ln)  # register-only: the pending stores move past it
            continue
        flush()
        out.append(ln)
    flush()
    return out


SIZES = {"u8": 1, "u16": 2, "u32": 4, "u64": 8, "f32": 4, "f64": 8}
TEMP_RE = re.compile(r"^\s*char\* (a\d+) = (.+);$")
LOAD_RE = re.compile(r"^\s*(u8|u16|u32|u64|f32|f64) (\w+) = (?:\(\1\))?\*\(\1\*\)(.+);$")
VALUE_RE = re.compile(r"^(?:\((?:u8|u16|u32|u64)\)\(u64\)\((\w+)\)|(\w+))$")


def copy_runs(body: list[str]) -> list[str]:
    """A block of loads from one base followed by stores of those values, in the same order,
    to another base (both contiguous, nothing else touching memory in between) becomes
    __builtin_memcpy: struct copies compile to that load/store pattern. Lines are blanked,
    not removed, so recorded line numbers stay valid."""
    body = list(body)
    temps: dict[str, tuple[str, int]] = {}
    i = 0
    while i < len(body):
        loads: list[tuple[int, str, tuple[str, int], int]] = []  # (line, var, (base, off), size)
        j = i
        while j < len(body):
            t = TEMP_RE.match(body[j])
            if t and store_slot(t.group(2)):
                temps[t.group(1)] = store_slot(t.group(2))
                j += 1
                continue
            m = LOAD_RE.match(body[j])
            if not m:
                break
            a = m.group(3)
            tm = re.fullmatch(r"\((a\d+) \+ (\d+)\)", a)
            slot = (temps[tm.group(1)][0], temps[tm.group(1)][1] + int(tm.group(2))) \
                if tm and tm.group(1) in temps else store_slot(a)
            if not slot:
                break
            loads.append((j, m.group(2), slot, SIZES[m.group(1)]))
            j += 1
        stores: list[tuple[int, tuple[str, int], str, int]] = []
        while loads and j < len(body) and len(stores) < len(loads):
            m = STORE_RE.match(body[j])
            slot = m and store_slot(m.group(2))
            v = m and VALUE_RE.match(m.group(3))
            if not (slot and v):
                break
            t = re.match(r"^\s*\*\((\w+)\*\)", body[j]).group(1)
            stores.append((j, slot, v.group(1) or v.group(2), SIZES[t]))
            j += 1
        if loads and len(stores) == len(loads) and all(
                s[2] == l[1] and s[3] == l[3] for s, l in zip(stores, loads)):
            src, dst = loads[0][2], stores[0][1]
            n = sum(l[3] for l in loads)
            contiguous = all(l[2] == (src[0], src[1] + sum(x[3] for x in loads[:k])) for k, l in enumerate(loads)) \
                and all(s[1] == (dst[0], dst[1] + sum(x[3] for x in stores[:k])) for k, s in enumerate(stores))
            names = {l[1] for l in loads}
            used = any(re.search(rf"\b{v}\b", ln) for k, ln in enumerate(body)
                       if not loads[0][0] <= k < j for v in names)
            if contiguous and n > 8 and src[0] != dst[0] and not used:
                last = stores[-1][0]
                ind = body[last][:len(body[last]) - len(body[last].lstrip())]
                for k in [ln for ln, *_ in loads] + [ln for ln, *_ in stores]:
                    body[k] = ""
                body[last] = (f"{ind}__builtin_memcpy({base_ptr(dst[0])} + {dst[1]}, "
                              f"{base_ptr(src[0])} + {src[1]}, {n});")
                i = j
                continue
        i += 1
    return body


def base_ptr(b: str) -> str:
    return "stk" if b == "stk" else f"(char*)({b})"


def float_widths(ins, semantic: bool = False) -> dict[int, str]:
    """The width ("s" or "d") each float register is first read with in the function (first used,
    for one only written): an incoming argument or a register only passed on to a callee takes it
    (one variable per register and width in flat mode, and no "float width" clash with a later
    use in the structured lifter). Reads, since a write may come first at the other width
    (`fcvt d0, s0`)."""
    w: dict[int, str] = {}
    if semantic:
        ins = [x for x in ins if not is_frame(x)]  # (semantic mode: saves of d8-d15 are not uses)
    for x in ins:
        if x.mnemonic == "fmov" and any(op.type == arm64.ARM64_OP_FP for op in x.operands):
            continue  # (capstone counts fmov's immediate destination as read)
        for q in x.regs_access()[0]:
            name = CS.reg_name(q)
            if name[0] in "sd" and fkey(name) is not None:
                w.setdefault(100 + fkey(name), name[0])
    for x in ins:
        for op in x.operands:
            if op.type == arm64.ARM64_OP_REG and regname(op.reg)[0] in "sd" and fkey(regname(op.reg)) is not None:
                w.setdefault(100 + fkey(regname(op.reg)), regname(op.reg)[0])
    if semantic:
        # (semantic mode: a register only copied whole, `mov v9.16b, v1.16b`, takes the width its
        # copy is read with, and the other way round)
        pairs = [(100 + int(regname(x.operands[0].reg)[1:]), 100 + int(regname(x.operands[1].reg)[1:]))
                 for x in ins if x.mnemonic == "mov" and len(x.operands) == 2
                 and all(op.type == arm64.ARM64_OP_REG and regname(op.reg)[0] == "v" for op in x.operands)]
        for _ in range(4):
            for d, s in pairs:
                if s not in w and d in w:
                    w[s] = w[d]
                elif d not in w and s in w:
                    w[d] = w[s]
    return w


def frame_imm(op) -> int:
    """An add/sub immediate with its shift (`sub sp, sp, #1, lsl #12`); semantic mode's frame
    code uses it (byte mode keeps the unshifted value its matched lifts were made with)."""
    return op.imm << (op.shift.value if op.shift.type == arm64.ARM64_SFT_LSL else 0)


def nzcv_holds(nzcv: int, cc: int) -> bool | None:
    """Whether condition cc holds for the flags nzcv (as set by ccmp/fccmp when skipped)."""
    n, z, c, v = (nzcv >> 3) & 1, (nzcv >> 2) & 1, (nzcv >> 1) & 1, nzcv & 1
    return {arm64.ARM64_CC_EQ: z, arm64.ARM64_CC_NE: not z, arm64.ARM64_CC_HS: c,
            arm64.ARM64_CC_LO: not c, arm64.ARM64_CC_MI: n, arm64.ARM64_CC_PL: not n,
            arm64.ARM64_CC_VS: v, arm64.ARM64_CC_VC: not v, arm64.ARM64_CC_HI: c and not z,
            arm64.ARM64_CC_LS: not (c and not z), arm64.ARM64_CC_GE: n == v, arm64.ARM64_CC_LT: n != v,
            arm64.ARM64_CC_GT: not z and n == v, arm64.ARM64_CC_LE: not (not z and n == v)}.get(cc)


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
        self.retk: int | None = None               # the last written of x0 (0) and s0/d0 (100)
        self.fcall: tuple | None = None            # the last call, while its result may be a float
        self.x8w = False                           # x8 set to a local since the last call (semantic mode)

    def copy(self) -> "State":
        s = State()
        s.vals, s.pages, s.consts = dict(self.vals), dict(self.pages), dict(self.consts)
        s.poison, s.called, s.written, s.cmp = set(self.poison), self.called, set(self.written), self.cmp
        s.retk, s.fcall, s.x8w = self.retk, self.fcall, self.x8w
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
    opts: frozenset = frozenset()  # second-round rewrites of the body (ROUND2); "semantic": tools/port_lift.py
    # address -> abi_facts.Facts or None (callee argument registers): semantic mode, and byte mode's
    # pass-through (calls also name the callee's argument registers this function still holds).
    facts = None

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
        self.last_call: tuple[str | None, str] = (None, "")  # (callee or None, argument types)
        self.tails: dict[str, str] = {}  # direct tail callee -> argument types
        self.flagsrc: dict[int, tuple] = {}  # id(State.cmp) -> (cmp, "add" | "logic"): flags of adds/ands
        # Byte mode with facts: plain=True lifts as before pass-through existed; passed is set when a
        # call named a register only pass-through gave it (the plain lift's text then differs).
        self.plain, self.passed = False, False
        self.stack_params = 0  # semantic mode: 8-byte stack argument slots read (parameters st<i>)
        # Why lift() gave up (for --report), and how far it got: the instruction index, -1
        # before the first, the instruction count after the last.
        self.reason, self.pos = "", -1

    def fail(self, reason: str, pos: int | None = None) -> None:
        self.reason = reason
        if pos is not None:
            self.pos = pos

    def unsupported(self, e: Unsupported) -> None:
        r = str(e)
        if r[:1] in "xwsd" and r[1:].isdigit():
            r = "reg read before def" if not isinstance(self, FlatLifter) else "flat: reg read before linear def"
        elif r[:1] in "qvhb" and r[1:].isdigit():
            r = "vector/half/byte register"
        self.fail(f"unsupported: {r}")

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
        if n in s.pages and n not in s.poison and "semantic" in self.opts and r[0] == "x":
            return self.global_ref(s.pages[n], False), "char*"  # a bare page: its address
        if n in s.pages or n in s.poison:
            raise Unsupported("unknown value")
        if n == 100 and n not in s.vals and s.fcall:
            t = ftype(r)
            self.retype_call(t, f"r{s.fcall[0]}_100")
            s.vals[100] = (f"r{s.fcall[0]}_100", t)
            s.fcall = None
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

    def retype_call(self, t: str, dest: str) -> None:
        """s0/d0 is read after a call: the callee returns a float. Its line becomes
        `<t> <dest> = <call>;` (the call's x0 result must be unused)."""
        ln, var, expr, fn, types = self.s.fcall
        # (FlatLifter drops fcall when it reads X0.)
        if var != "X0" and any(re.search(rf"\b{var}\b", s) for s in self.body[ln + 1:]):
            raise Unsupported("call result used as both")
        if fn:
            ns = "fret" if t == "f32" else "dret"
            self.decls.add(f"namespace {ns} {{ {t} {fn}({types}); }}")
            expr = expr.replace("callee::", f"{ns}::", 1)
        else:
            expr = expr.replace("((char* (*)(", f"(({t} (*)(", 1)
        ind = self.body[ln][:len(self.body[ln]) - len(self.body[ln].lstrip())]
        self.body[ln] = ind + (f"{t} {dest} = {expr};" if dest[0] == "r" else f"{dest} = {expr};")
        if self.s.vals.get(0, ("",))[0] == var:
            del self.s.vals[0]
        self.s.retk = 100

    def write(self, r: str, e: str, t: str, const: int | None = None) -> None:
        n = self.key(r)
        if n < 0:
            return
        var = f"r{len(self.body)}_{n}"
        self.line(f"{t} {var} = {e};")
        s = self.s
        if n in (0, 100):
            s.retk = n
        if n == 100:
            s.fcall = None
        s.vals[n] = (var, t)
        s.pages.pop(n, None)
        s.poison.discard(n)
        if const is None:
            s.consts.pop(n, None)
        else:
            s.consts[n] = const
        if n <= 7 or 100 <= n <= 107:
            s.written.add(n)
        if n == 8:
            s.x8w = "stk" in e  # (an indirect callee may build its result there: Lifter.step)

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
        m.fcall = None  # lines are inserted below: recorded line numbers would move
        m.x8w = a.x8w or c.x8w
        m.retk = a.retk if a.retk == c.retk else None
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
            sem = "semantic" in self.opts
            if va is not None and vc is not None and va[1] != vc[1] and is_int(va[1]) and is_int(vc[1]) and sem:
                # (semantic mode: integers of two types join as the whole register, which is what a
                # 32-bit write leaves too: zero-extended)
                va, vc = (f"(u64)({va[0]})", "u64"), (f"(u64)({vc[0]})", "u64")
            if sem and (va is None) != (vc is None) and n not in m.poison \
                    and not (n == 100 and (a if va is None else c).fcall):  # (s0: a call's unread float result)
                # Semantic mode: one path has no value here (a call clobbered the register, or it was
                # never set), which is garbage in the original; the other path's value stands. Before
                # the if, it is in scope already; otherwise a phi that the other path leaves at zero.
                if b.kind == "if" and va is None:
                    m.vals[n] = vc
                    continue
                t = (va or vc)[1]
                var = f"m{self.phis}"
                self.phis += 1
                decls.append(f"{t} {var} = ({t})0;")
                if va is not None:
                    then_end.append(f"{var} = {va[0]};")
                if vc is not None and b.kind == "else":
                    else_end.append(f"{var} = {vc[0]};")
                m.vals[n] = (var, t)
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

    def snapshot(self, e: str, t: str) -> str:
        return e  # values are immutable here (FlatLifter overrides)

    def temp_ptr(self, e: str) -> str:
        var = f"a{len(self.body)}"
        self.line(f"char* {var} = {e};")
        return var

    def terminate(self, kind: str, payload) -> None:
        self.rets.append((len(self.body), kind, payload))
        self.line("@RET@")
        self.live = False

    # -- operands -----------------------------------------------------------------------------

    def global_ref(self, a: int, through_got: bool, align: int = 1) -> str:
        """Address of the object or function at a, as char*. A float access gives an
        alignment, so the page offset folds into the scaled load/store offset."""
        if align > 1 and not through_got and a % align == 0 and a not in self.orig.sizes:
            self.decls.add(f"namespace h{align} {{ extern HIDDEN char g_unk_{a:x}[] __attribute__((aligned({align}))); }}")
            return f"(char*)h{align}::g_unk_{a:x}"
        if a in self.orig.sizes:
            ns = "fp" if through_got else "hf"
            self.decls.add(f"namespace {ns} {{ {'' if through_got else 'HIDDEN '}void fn_{a:x}(); }}")
            return f"(char*)&{ns}::fn_{a:x}"
        if through_got:
            self.decls.add(f"extern char g_unk_{a:x}[];")
            return f"(char*)g_unk_{a:x}"
        self.decls.add(f"namespace h {{ extern HIDDEN char g_unk_{a:x}[]; }}")
        return f"(char*)h::g_unk_{a:x}"

    def arg_slot(self, x, op) -> int | None:
        """Semantic mode: the offset from the entry sp of a memory operand at or above it (the
        caller's frame, where the arguments beyond the eight registers are), else None."""
        if "semantic" not in self.opts or not self.frame or x.writeback or op.mem.index or op.shift.type \
                or regname(op.mem.base) not in ("sp", "x29"):
            return None
        base = self.frame["sp"] if regname(op.mem.base) == "sp" else self.frame["fp"]
        return base + op.mem.disp if base is not None and base + op.mem.disp >= 0 else None

    def stack_arg(self, off: int, size: int = 1) -> str:
        """The address `off` bytes above the entry sp: in stack argument slot st<off/8>, a
        parameter declared after p0-p7 (signature); `size` bytes from there are read or written."""
        self.stack_params = max(self.stack_params, (off + size - 1) // 8 + 1)
        return f"((char*)&st{off // 8} + {off % 8})"

    def address(self, x, op, align: int = 1) -> str:
        """The memory operand's address; applies pre/post-index writeback to the base."""
        base_reg = regname(op.mem.base)
        if op.mem.index:
            # Register offset: base + extend(index) << shift.
            if x.writeback or base_reg in ("sp", "x29") or self.key(base_reg) in self.s.pages:
                raise Unsupported("indexed")
            idx, _ = self.read(regname(op.mem.index))
            base, _ = self.read(base_reg)
            return f"((char*)({base}) + {self.extend(idx, op, 'u64')})"
        if op.shift.type:
            raise Unsupported("indexed")
        if base_reg in ("sp", "x29"):
            if x.writeback or not self.frame:
                raise Unsupported("stack writeback")
            base = self.frame["sp"] if base_reg == "sp" else self.frame["fp"]
            off = base + op.mem.disp
            if off >= 0 and "semantic" in self.opts:
                return self.stack_arg(off)
            return self.local(off)
        n = self.key(base_reg)
        if n in self.s.pages:
            if x.writeback:
                raise Unsupported("page writeback")
            a = self.s.pages[n] + op.mem.disp
            if self.orig.img.relocs.get(a - BASE) is not None:
                raise Unsupported("GOT slot as data")
            return self.global_ref(a, False, align)
        base, t = self.read(base_reg)
        if not x.writeback:
            return f"((char*)({base}) + {op.mem.disp})"
        post = [o for o in x.operands if o.type == arm64.ARM64_OP_IMM]
        step = post[-1].imm if post else op.mem.disp
        base = self.snapshot(base, "u64")  # the access uses the base before the writeback
        addr = f"((char*)({base}) + {0 if post else op.mem.disp})"
        self.write(base_reg, f"(char*)({base}) + {step}", "char*")
        return addr

    def operand(self, op, like: str) -> tuple[str, str]:
        if op.type == arm64.ARM64_OP_IMM:
            if op.shift.type and op.shift.type != arm64.ARM64_SFT_LSL:
                raise Unsupported("shifted imm")
            return str(op.imm << (op.shift.value if op.shift.type else 0)), width_type(like)
        if op.type == arm64.ARM64_OP_REG:
            v, t = self.read(regname(op.reg))
            if not op.shift.type and not op.ext:
                return v, t
            return self.extend(v, op, width_type(like)), width_type(like)
        raise Unsupported("operand")

    def extend(self, v: str, op, t: str) -> str:
        """A register operand's extension (uxtw, sxtb, ...) and shift, as an expression of type t."""
        ext = {arm64.ARM64_EXT_UXTB: "u8", arm64.ARM64_EXT_UXTH: "u16", arm64.ARM64_EXT_UXTW: "u32",
               arm64.ARM64_EXT_UXTX: "u64", arm64.ARM64_EXT_SXTB: "s8", arm64.ARM64_EXT_SXTH: "s16",
               arm64.ARM64_EXT_SXTW: "s32", arm64.ARM64_EXT_SXTX: "s64", 0: None}.get(op.ext, "?")
        if ext == "?":
            raise Unsupported("extension")
        e = f"({t})({ext})(u64)({v})" if ext else f"({t})(u64)({v})"
        if op.shift.type:
            k = op.shift.value
            if op.shift.type == arm64.ARM64_SFT_LSL:
                e = f"({t})({e} << {k})"
            elif op.shift.type == arm64.ARM64_SFT_LSR:
                e = f"({t})({e} >> {k})"
            elif op.shift.type == arm64.ARM64_SFT_ASR:
                st = "s64" if t == "u64" else "s32"
                e = f"({t})(({st})({e}) >> {k})"
            else:
                raise Unsupported("shift")
        return e

    @staticmethod
    def fcond(c: tuple, cc: int) -> str:
        """Condition cc after fcmp: each is ordered (false if either is NaN) or unordered."""
        a, b = f"({c[3]})({c[0]})", f"({c[3]})({c[1]})"
        expr = {arm64.ARM64_CC_EQ: f"{a} == {b}", arm64.ARM64_CC_NE: f"{a} != {b}",
                arm64.ARM64_CC_MI: f"{a} < {b}", arm64.ARM64_CC_LO: f"{a} < {b}",
                arm64.ARM64_CC_LS: f"{a} <= {b}", arm64.ARM64_CC_GT: f"{a} > {b}",
                arm64.ARM64_CC_GE: f"{a} >= {b}", arm64.ARM64_CC_LT: f"!({a} >= {b})",
                arm64.ARM64_CC_LE: f"!({a} > {b})", arm64.ARM64_CC_HI: f"!({a} <= {b})",
                arm64.ARM64_CC_PL: f"!({a} < {b})", arm64.ARM64_CC_HS: f"!({a} < {b})",
                arm64.ARM64_CC_VS: f"__builtin_isunordered({a}, {b})",
                arm64.ARM64_CC_VC: f"!__builtin_isunordered({a}, {b})"}.get(cc)
        if expr is None:
            raise Unsupported("float condition")
        return expr

    def cond(self, cc: int, negate: bool) -> str:
        c = self.s.cmp
        if c is None and "semantic" in self.opts and isinstance(self, FlatLifter):
            return self.flag_cond(cc, negate)  # flags set in another block
        if "semantic" in self.opts and c and c[2] in "xw":
            # Every condition, from the instruction that set the flags (int_cond).
            ent = self.flagsrc.get(id(c), (c, "sub"))
            if negate:
                cc = cc + 1 if cc % 2 else cc - 1
            if ent[1] == "add":
                return self.int_cond(ent[2], ent[3], c[2], cc, "add")
            return self.int_cond(c[0], "0" if ent[1] == "logic" else c[1], c[2], cc, ent[1])
        if not self.s.cmp or (cc not in CONDS and self.s.cmp[2] not in "fe"):
            raise Unsupported("condition")
        opr, signed = CONDS.get(cc, ("", False))
        c = self.s.cmp
        if c[2] in "fe":
            # "e": a chain (fccmp) whose condition is built per cc by c[0]. Negation inverts
            # the condition code (exact on flags; `a != b` lays out unlike `!(a == b)`).
            if negate:
                cc = cc + 1 if cc % 2 else cc - 1
            return self.fcond(c, cc) if c[2] == "f" else c[0](cc)
        t = ("s64" if c[2] == "x" else "s32") if signed else ("u64" if c[2] == "x" else "u32")
        return f"({t})(u64)({c[0]}) {NEGATE[opr] if negate else opr} ({t})({c[1]})"

    def has_value(self, n: int) -> bool:
        return n in self.s.vals and n not in self.s.poison

    def args(self, need: tuple = (0, 0)) -> list[tuple[str, str]]:
        w = self.s.written
        n = max([k for k in w if k < 100], default=-1) + 1
        n = max(n, self.extra)
        fl = max([k - 100 for k in w if k >= 100], default=-1) + 1
        # Up to the first float register with no value (a higher one was a temporary).
        fl = next((i for i in range(fl) if not self.has_value(100 + i)), fl)
        floats = []
        for i in range(fl):
            t = self.ftype_of(100 + i)
            floats.append(self.read(("s" if t == "f32" else "d") + str(i)))
        ints = [self.read(f"x{i}") for i in range(n)]
        # Also the registers the callee reads (callee_need). One the caller holds is passed on; one
        # with no value here (clobbered by a call) held garbage in the original: semantic mode
        # passes 0, byte mode stops there (a constant would cost an instruction).
        sem = "semantic" in self.opts
        for i in range(n, need[0]):
            try:
                ints.append(self.read(f"x{i}"))
            except Unsupported:
                if not sem:
                    break
                ints.append(("(u64)0", "u64"))  # (typed: a bare 0 is ambiguous between overloads)
            self.passed |= not sem
        for i in range(fl, need[1]):
            try:
                if self.has_value(100 + i) or (i == 0 and self.s.fcall):
                    # (s0 right after a call: that call's float result, passed on: read() retypes it)
                    t = self.ftype_of(100 + i)
                    floats.append(self.read(("s" if t == "f32" else "d") + str(i)))
                else:
                    # A register only passed on (an incoming argument): as f64, which carries all
                    # of its bits whatever width the callee reads, on AArch64 and x86-64 alike.
                    floats.append(self.read(f"{getattr(self, 'fwidth', {}).get(100 + i, 'd')}{i}"))
            except Unsupported:
                if not sem:
                    break
                floats.append(("(f64)0", "f64"))  # no value here: garbage in the original as well
            self.passed |= not sem
        # Semantic mode: arguments beyond the eight registers (callee_need's third count) are the
        # words this function stored at [sp + 8i] for the call; C passes them after the integers.
        stack = []
        for i in range(need[2] if len(need) > 2 else 0):
            at = self.cur_delta + 8 * i - self.frame["start"]
            if not 0 <= at <= self.frame["size"] - 8:
                raise Unsupported("stack arguments outside the frame")
            stack.append((f"*(u64*)(stk + {at:#x})", "u64"))
        return ints + stack + floats

    def ftype_of(self, n: int) -> str:
        """The C type of float register key n's current value (f32 when unknown)."""
        return self.s.vals.get(n, ("", "f32"))[1]

    def own_args(self) -> None:
        """Semantic mode: the function's own argument registers (abi_facts, x8 aside) are
        parameters from the entry on, so every path holds them until it writes them. A join then
        merges the incoming value with a write (a phi) instead of losing it: a call after
        `if (c) x1 = ...;` passes x1 on both paths, and nothing falls back to 0."""
        f = self.facts(self.addr) if self.facts is not None else None
        if f is None:
            return
        for k in sorted(f.args):
            if k < 8:
                self.read(f"x{k}")
            elif 100 <= k < 108:
                self.read(f"{self.fwidth.get(k, 'd')}{k - 100}")

    def callee_need(self, fn: str) -> tuple[int, int]:
        """The argument registers a direct callee reads (abi_facts), so the call names them all.
        Semantic mode does not support a callee that takes its result address in x8 or arguments
        on the stack. Byte mode (pass-through, unless plain) names those of them it holds. An
        indirect call (fn "") is passed x0 and what this function's callers set for it
        (abi_facts' `fwd`). On AArch64 they are there either way; an x86-64 compile passes only
        what the call names."""
        if self.facts is None:
            return 0, 0
        sem = "semantic" in self.opts
        if not sem and self.plain:
            return 0, 0
        if not fn:
            own = self.facts(self.addr)
            return own.fwd_counts if own is not None else (1, 0)
        f = self.facts(int(fn[3:], 16))
        if f is None:
            return 0, 0
        if sem and f.sret and not getattr(self, "sret_ok", False):
            raise Unsupported("call with an indirect result (x8)")
        if sem and f.stack:
            if not self.frame:
                raise Unsupported("call with stack arguments")
            return 8, f.floats, f.stack_slots  # (stack arguments follow all eight integers)
        return f.ints, f.floats

    def sret_callee(self, a: int) -> bool:
        """Whether the callee at a builds its result at x8 (abi_facts), in semantic mode and in
        byte mode's pass-through (where the call then names x8 too: see sret_call, sret_tail)."""
        if self.facts is None or ("semantic" not in self.opts and self.plain):
            return False
        f = self.facts(a)
        if f is None or not f.sret:
            return False
        self.passed |= "semantic" not in self.opts
        return True

    def sret_call(self, fn: str) -> None:
        """A call to a callee that builds its result at x8: C++17 elides the copy, so the result
        object is the memory x8 points to (x8 on AArch64, the hidden first argument on x86-64)."""
        x8, _ = self.read("x8")
        self.sret_ok = True
        try:
            args = self.args(self.callee_need(fn))
        finally:
            self.sret_ok = False
        types = ", ".join(t for _, t in args)
        self.decls.add(f"namespace sret {{ UnkResult {fn}({types}); }}")
        self.decls.add(LIFT_AT)
        self.line(f"new ((void*)({x8}), lift_at_t{{}}) UnkResult(sret::{fn}({', '.join(e for e, _ in args)}));")

    def sret_tail(self, fn: str) -> None:
        """A tail call (`b`) to a callee that builds its result at x8. With x8 untouched since the
        entry (this function's own result address, passed through) it is `return callee(...)`
        in a function returning UnkResult: C++17 elides the copy, so the callee builds the result
        where this function's caller wants it, and AArch64 keeps the tail call with x8 as it
        was. Otherwise (semantic mode) the result is built where x8 points and the function
        returns."""
        incoming = (8 not in self.types) if isinstance(self, FlatLifter) else \
            (8 not in self.s.vals or self.s.vals[8][0] == "(char*)&ret")
        if not incoming:
            if "semantic" not in self.opts:
                raise Unsupported("tail call with an indirect result (x8)")
            self.sret_call(fn)
            self.terminate("value", None)
            return
        self.read("x8")  # (this function returns through x8 too)
        self.sret_ok = True
        try:
            args = self.args(self.callee_need(fn))
        finally:
            self.sret_ok = False
        self.decls.add(f"namespace sret {{ UnkResult {fn}({', '.join(t for _, t in args)}); }}")
        self.terminate("sret_tail", f"sret::{fn}({', '.join(e for e, _ in args)})")

    def call(self, fn: str, ptr: str | None = None) -> str:
        """A call expression; declares the callee overload for these argument types."""
        args = self.args(self.callee_need(fn))
        types = ", ".join(t for _, t in args)
        values = ", ".join(e for e, _ in args)
        self.last_call = (fn if ptr is None else None, types)
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
        s.x8w = False

    # -- driver -------------------------------------------------------------------------------

    def find_guards(self, ins) -> None:
        """Inlined GUARDS helpers: self.guard maps the ldarb of each to its statement and the
        number of helper calls after the check; self.gskip holds the instructions it stands
        for (the tbz, those calls, and the out-of-line initialisation reached from the tbz)."""
        self.guard, self.gskip = {}, set()
        if "semantic" in self.opts:
            return  # semantic mode lifts the guard itself (ldarb, the out-of-line initialisation)
        at = {x.address: k for k, x in enumerate(ins)}
        pages, slots = {}, {}
        for k, x in enumerate(ins):
            m, o = x.mnemonic, x.operands
            if m == "adrp":
                pages[o[0].reg] = o[1].imm
            elif m == "ldr" and len(o) == 2 and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.base in pages:
                slots[o[0].reg] = pages[o[1].mem.base] + o[1].mem.disp
            elif m == "ldarb" and slots.get(o[1].mem.base) in GUARDS and k + 1 < len(ins) \
                    and x.address not in self.gskip:
                stmt, calls = GUARDS[slots[o[1].mem.base]]
                # The tbz, possibly after a few scheduled instructions that leave the flag alone.
                j = k + 1
                while j < min(k + 6, len(ins)) and ins[j].mnemonic != "tbz":
                    y = ins[j]
                    if y.mnemonic.startswith("b") or y.mnemonic in ("cbz", "cbnz", "tbnz", "ret") or \
                            (y.operands and y.operands[0].type == arm64.ARM64_OP_REG
                             and regname(y.operands[0].reg)[1:] == regname(o[0].reg)[1:]):
                        break
                    j += 1
                t = ins[j] if j < len(ins) else ins[k]
                if t.mnemonic != "tbz" or t.operands[0].reg != o[0].reg or t.operands[1].imm != 0:
                    continue
                after = ins[j + 1:j + 1 + len(calls)]
                if [a.operands[0].imm if a.mnemonic == "bl" else None for a in after] != calls:
                    continue
                resume = t.address + 4
                region, todo = set(), [t.operands[2].imm]
                while todo:
                    a = todo.pop()
                    while a != resume and a not in region:
                        if a not in at:
                            raise Unsupported("guard")
                        region.add(a)
                        y = ins[at[a]]
                        if y.mnemonic in ("ret", "br"):
                            break
                        tgt = [op.imm for op in y.operands if op.type == arm64.ARM64_OP_IMM]
                        if y.mnemonic == "b":
                            a = tgt[0]
                            continue
                        if y.mnemonic.startswith("b.") or y.mnemonic in ("cbz", "cbnz", "tbz", "tbnz"):
                            todo.append(tgt[-1])
                        a += 4
                if not any(ins[at[a]].mnemonic == "bl" and ins[at[a]].operands[0].imm == GUARD_ACQUIRE
                           for a in region):
                    continue
                # Nothing else may enter the initialisation.
                for y in ins:
                    if y.address in region or y.address == t.address:
                        continue
                    if any(op.type == arm64.ARM64_OP_IMM and op.imm in region for op in y.operands) and \
                            (y.mnemonic in ("b", "cbz", "cbnz", "tbz", "tbnz") or y.mnemonic.startswith("b.")):
                        raise Unsupported("guard")
                self.guard[x.address] = (stmt, len(calls))
                self.gskip |= region | {t.address} | {a.address for a in after}

    def guard_step(self, x) -> bool:
        """Emits a GUARDS helper at its ldarb; True if x was one or is part of one."""
        if x.address in self.gskip:
            return True
        if x.address not in self.guard:
            return False
        stmt, calls = self.guard[x.address]
        self.line(stmt)
        if calls:
            self.clobber()
        return True

    def lift(self) -> tuple[str, set[str]] | None:
        orig, addr = self.orig, self.addr
        code = orig.function(addr)
        ins = self.ins = list(CS.disasm(code, addr))
        if not ins or len(ins) * 4 != len(code):
            return self.fail("undecodable instruction")
        end = addr + len(code)
        last = ins[-1].address
        if not self.setup_frame(ins):
            return self.fail("stack frame")
        self.cur_delta = 0
        self.fwidth = float_widths(ins, "semantic" in self.opts)
        try:
            self.find_guards(ins)
            if "semantic" in self.opts:
                self.own_args()
            for self.pos, x in enumerate(ins):
                if x.address in self.gskip:
                    continue
                self.close_to(x.address)
                if self.live and self.guard_step(x):
                    continue
                if not self.live:
                    return self.fail("unreachable code")  # (or a jump-around we don't model)
                if self.pos:
                    self.frame_reset(ins[self.pos - 1])
                if self.frame_op(x):
                    continue
                m, o = x.mnemonic, x.operands
                if m == "ret":
                    v = None
                    # x0, unless s0/d0 was written after it.
                    for k in ((100, 0) if self.s.retk == 100 else (0, 100)):
                        if k in self.s.vals and k not in self.s.poison:
                            v = self.s.vals[k]
                            break
                    self.terminate("value", v)
                elif m == "b" and not addr <= o[0].imm < end and self.sret_callee(o[0].imm):
                    self.sret_tail(f"fn_{o[0].imm:x}")
                elif m == "b" and not addr <= o[0].imm < end:
                    self.terminate("tail", self.call(f"fn_{o[0].imm:x}"))
                    self.tails[f"fn_{o[0].imm:x}"] = self.last_call[1]
                elif m == "br":
                    ptr, _ = self.read(regname(o[0].reg))
                    self.s.written.discard(self.key(regname(o[0].reg)))  # the target isn't an argument
                    self.terminate("tail", self.call("", ptr))
                elif m == "b":
                    t = o[0].imm
                    if not (x.address < t <= last and self.blocks and self.blocks[-1].end == x.address + 4):
                        return self.fail("branch: not an else")
                    self.to_else(t)
                elif not self.step(x, last):
                    return self.fail(f"insn: {m}")
            if self.blocks or self.live:
                return self.fail("falls off the end", len(ins))
        except Unsupported as e:
            return self.unsupported(e)
        self.pos = len(ins)
        return self.finish()

    def finish(self) -> tuple[str, set[str]] | None:
        if "copy" in self.opts:
            self.body = copy_runs(self.body)
        if "sort" in self.opts:
            self.body = sort_stores(self.body)
        if "alias" in self.opts:
            self.body = [ACCESS_RE.sub(r"*(a_\1*)", ln) for ln in self.body]
        tails = any(k == "tail" for _, k, _ in self.rets)
        if self.sret:
            if self.returns != "value":
                return self.fail("variant")
            for ln, k, p in self.rets:
                ind = self.body[ln][:len(self.body[ln]) - len(self.body[ln].lstrip())]
                # A final call (e.g. an initialiser given &ret) stays a tail call: ret is
                # built in place, so nothing is left to do after it.
                self.body[ln] = ind + (f"{p};\n{ind}return ret;" if k == "tail" else f"{p};" if k == "noret"
                                       else f"return {p};" if k == "sret_tail" else "return ret;")
            return self.signature("UnkResult", ["    UnkResult ret;"] + self.body), self.decls
        if self.returns != "void":
            if any(k == "value" and p is None for _, k, p in self.rets):
                return self.fail("variant")
            types = [p[1] for _, k, p in self.rets if k == "value"]
            if not tails and not types:
                return self.fail("variant")  # only no-return calls: the "void" variant
            # Tail calls return what the other paths return when that is a float.
            rtype = types[0] if types and types[0][0] == "f" else "char*" if tails else types[0]
            if self.returns == "ref":
                if rtype != "char*" or tails:
                    return self.fail("variant")
                rtype = "char&"
            if any((t[0] == "f") != (rtype[0] == "f") for t in types) or \
                    (rtype[0] == "f" and any(t != rtype for t in types)):
                return self.fail("variant")
        else:
            rtype = "void"
        for ln, k, p in self.rets:
            ind = self.body[ln][:len(self.body[ln]) - len(self.body[ln].lstrip())]
            if k == "noret":
                s = f"{p};"
            elif k == "tail" and rtype[0] == "f":
                if p.startswith("callee::"):
                    fn = p[len("callee::"):p.index("(")]
                    ns = "fret" if rtype == "f32" else "dret"
                    self.decls.add(f"namespace {ns} {{ {rtype} {fn}({self.tails[fn]}); }}")
                    s = f"return {ns}::{p[len('callee::'):]};"
                else:
                    s = f"return {p.replace('((char* (*)(', f'(({rtype} (*)(', 1)};"
            elif k == "tail":
                s = f"{p};\n{ind}return;" if rtype == "void" else \
                    f"return {p};" if rtype == "char*" else f"return ({rtype})(u64)({p});"
            elif rtype == "void":
                s = "return;"
            elif rtype == "char&":
                if p[1] != "char*":
                    return self.fail("variant")
                s = f"return *{p[0]};"
            elif p[1] == rtype:
                s = f"return {p[0]};"
            else:
                s = f"return ({rtype})(u64)({p[0]});"
            self.body[ln] = ind + s
        return self.signature(rtype, self.body), self.decls

    def signature(self, rtype: str, body: list[str]) -> str:
        # A parameter first read as w<n> is 32-bit (u32), otherwise a pointer.
        stack = self.stack_params
        if "semantic" in self.opts and self.facts is not None and (f := self.facts(self.addr)) is not None:
            stack = max(stack, f.stack_slots)  # what every caller passes (callee_need)
        ints = [f"{self.params.get(n, 'char*')} p{n}"
                for n in range(max([k for k in self.params if k < 100] + [7 if stack else -1]) + 1)]
        ints += [f"u64 st{i}" for i in range(stack)]  # stack arguments, after all eight
        floats = [f"{self.params.get(100 + n, 'f32')} f{n}"
                  for n in range(max([k - 100 for k in self.params if k >= 100], default=-1) + 1)]
        if self.frame and self.frame["size"]:
            f = self.frame
            body = [f"    alignas({f['align']}) char stk[{f['size']:#x}];"] + body
        return "\n".join([f"{rtype} fn_{self.addr:x}({', '.join(ints + floats)}) {{", *filter(None, body), "}"])

    def frame_op(self, x) -> bool:
        """is_frame(x), and in semantic mode only when a register store or load of x19-x28 hits
        a slot the prologue saved: a spill of a callee-saved register into the locals
        (`stp x21, x20, [sp, #8]` ... `ldp x21, x20, [sp, #8]`) is code to lift, which the byte
        match never needed to know. sp is tracked through pushes, pops and adjustments; it is
        back at the body's value after each ret or unconditional branch (frame_reset)."""
        if not is_frame(x):
            return False
        if "semantic" not in self.opts:
            return True
        m, o = x.mnemonic, x.operands
        mem = [op for op in o if op.type == arm64.ARM64_OP_MEM]
        if not mem:
            if m in ("add", "sub") and regname(o[0].reg) == "sp":
                self.cur_delta += frame_imm(o[2]) if m == "add" else -frame_imm(o[2])
            return True
        if x.writeback:
            post = [op for op in o if op.type == arm64.ARM64_OP_IMM]
            self.cur_delta += post[-1].imm if post else mem[0].mem.disp
            return True
        at = self.cur_delta + mem[0].mem.disp
        n = len([op for op in o if op.type == arm64.ARM64_OP_REG])
        return all(at + 8 * i in self.save_slots for i in range(n))

    def frame_reset(self, x) -> None:
        # (An exit before a shrink-wrapped prologue leaves sp at the entry's value.)
        if x.mnemonic in ("ret", "b", "br"):
            self.cur_delta = self.body_delta if self.pos > self.frame_at else 0

    def setup_frame(self, ins) -> bool:
        """Stack locals: the prologue gives sp and fp (offsets from sp at entry) and the save
        slots; the locals become one char array over the gap between save slots that holds the
        lowest local ("gap"), or over just the bytes accessed ("tight")."""
        delta, fp, saves = 0, None, []
        k = 0
        if "semantic" in self.opts:
            # A shrink-wrapped prologue (an early check, then the frame): it starts at the first
            # frame instruction, when nothing before it touches sp or x29; frame_op tracks sp from
            # the entry, so the instructions before it run with the entry's sp.
            def touches(x) -> bool:
                return any((op.type == arm64.ARM64_OP_REG and regname(op.reg) in ("sp", "x29"))
                           or (op.type == arm64.ARM64_OP_MEM and regname(op.mem.base) in ("sp", "x29"))
                           for op in x.operands)
            p = next((j for j, x in enumerate(ins[:12]) if is_frame(x)), None)
            if p and not any(touches(x) for x in ins[:p]):
                k = p
        self.frame_at = k  # the prologue's first instruction (frame_reset)
        while k < len(ins) and is_frame(ins[k]):
            x = ins[k]
            m, o = x.mnemonic, x.operands
            if m == "sub" and regname(o[0].reg) == "sp":
                delta -= o[2].imm if "semantic" not in self.opts else frame_imm(o[2])
            elif m == "add" and regname(o[0].reg) == "x29":
                fp = delta + (o[2].imm if "semantic" not in self.opts else frame_imm(o[2]))
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
        self.save_slots = {s + 8 * i for s, e in saves for i in range((e - s) // 8)}
        self.body_delta = self.cur_delta = delta
        refs = []  # (offset from sp at entry, size)
        for x in ins[k:]:
            if self.frame_op(x):
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
                imm = (o[2].imm if "semantic" not in self.opts else frame_imm(o[2])) if len(o) == 3 else 0
                refs.append((base + (-imm if x.mnemonic == "sub" else imm), 1))
        if "semantic" in self.opts and any(r[0] >= 0 for r in refs):
            # The caller's frame (stack arguments) holds parameters, not locals (address()).
            refs = [r for r in refs if r[0] < 0]
            if not refs:
                self.frame = {"sp": delta, "fp": fp, "start": 0, "size": 0, "align": 8}
                return True
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

    def int_cond(self, a: str, b: str, w: str, cc: int, kind: str) -> str:
        """Condition cc on the flags of a - b ("sub"), a + b ("add") or of the result a of a
        logical operation ("logic": C and V clear), exactly as AArch64 sets them: carry from an
        unsigned compare of the result, overflow from the sum or difference in a wider type."""
        C = arm64
        t, st, wide = ("u64", "s64", "__int128") if w == "x" else ("u32", "s32", "s64")
        if kind == "logic":
            r = f"({st})(u64)({a})"
            return {C.ARM64_CC_EQ: f"{r} == 0", C.ARM64_CC_NE: f"{r} != 0", C.ARM64_CC_MI: f"{r} < 0",
                    C.ARM64_CC_PL: f"{r} >= 0", C.ARM64_CC_LT: f"{r} < 0", C.ARM64_CC_GE: f"{r} >= 0",
                    C.ARM64_CC_GT: f"{r} > 0", C.ARM64_CC_LE: f"{r} <= 0", C.ARM64_CC_HS: "0",
                    C.ARM64_CC_LO: "1", C.ARM64_CC_HI: "0", C.ARM64_CC_LS: "1", C.ARM64_CC_VS: "0",
                    C.ARM64_CC_VC: "1"}[cc]
        ua, ub = f"({t})(u64)({a})", f"({t})(u64)({b})"
        if kind == "add":
            res, true = f"({t})({ua} + {ub})", f"(({wide})({st}){ua} + ({wide})({st}){ub})"
            carry = f"({res} < {ua})"
            table = {C.ARM64_CC_HS: carry, C.ARM64_CC_LO: f"!{carry}", C.ARM64_CC_HI: f"({carry} && {res} != 0)",
                     C.ARM64_CC_LS: f"(!{carry} || {res} == 0)"}
        else:
            res, true = f"({t})({ua} - {ub})", f"(({wide})({st}){ua} - ({wide})({st}){ub})"
            table = {}
        table.update({C.ARM64_CC_EQ: f"{res} == 0", C.ARM64_CC_NE: f"{res} != 0",
                      C.ARM64_CC_MI: f"({st}){res} < 0", C.ARM64_CC_PL: f"({st}){res} >= 0",
                      C.ARM64_CC_VS: f"{true} != ({wide})({st}){res}", C.ARM64_CC_VC: f"{true} == ({wide})({st}){res}"})
        if kind == "add":
            table.update({C.ARM64_CC_LT: f"{true} < 0", C.ARM64_CC_GE: f"{true} >= 0",
                          C.ARM64_CC_GT: f"{true} > 0", C.ARM64_CC_LE: f"{true} <= 0"})
        if cc in table:
            return table[cc]
        if cc not in CONDS:
            raise Unsupported("condition")
        opr, signed = CONDS[cc]
        ct = st if signed else t
        return f"({ct})(u64)({a}) {opr} ({ct})({b})"

    def semantic_step(self, x, mem) -> bool:
        """Instructions only semantic mode lifts (their byte-identical form is unknown or the
        lifter's shortcut for them is not exact). True if x was one."""
        m, o = x.mnemonic, x.operands
        s = self.s
        regs = [regname(op.reg) for op in o if op.type == arm64.ARM64_OP_REG]
        acq = {"ldarb": "u8", "ldarh": "u16", "ldar": None, "ldaxrb": "u8", "ldaxrh": "u16", "ldaxr": None,
               "ldxrb": "u8", "ldxrh": "u16", "ldxr": None}
        rel = {"stlrb": "u8", "stlrh": "u16", "stlr": None}
        excl = {"stlxrb": "u8", "stlxrh": "u16", "stlxr": None, "stxrb": "u8", "stxrh": "u16", "stxr": None}
        if m in acq and mem and len(regs) == 1 and regs[0][0] in "xw":
            t = acq[m] or width_type(regs[0])
            a = self.temp_ptr(self.address(x, mem[0]))
            self.write(regs[0], f"({width_type(regs[0])})__atomic_load_n(({t}*)({a}), __ATOMIC_ACQUIRE)",
                       width_type(regs[0]))
            self.excl = (a, self.snapshot(self.read(regs[0])[0], width_type(regs[0])), t) if "x" in m[2:] else None
            return True
        if m in rel and mem and len(regs) == 1 and regs[0][0] in "xw":
            t = rel[m] or width_type(regs[0])
            v, _ = self.read(regs[0])
            self.line(f"__atomic_store_n(({t}*)({self.address(x, mem[0])}), ({t})(u64)({v}), __ATOMIC_RELEASE);")
            return True
        if m in excl and mem and len(regs) == 2 and regs[1][0] in "xw":
            # A store-exclusive succeeds (status 0) when memory still holds what the paired
            # load-exclusive read: a compare-and-swap with that value.
            ex = getattr(self, "excl", None)
            if ex is None:
                raise Unsupported("store-exclusive without its load")
            t = excl[m] or width_type(regs[1])
            a, old, _ = ex
            v, _ = self.read(regs[1])
            # (a statement expression: its local cannot be jumped over by flat mode's gotos)
            self.write(regs[0], f"({{ {t} lift_old = ({t})(u64)({old}); (u32)!__atomic_compare_exchange_n(({t}*)({a}), "
                                f"&lift_old, ({t})(u64)({v}), false, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST); }})", "u32")
            self.excl = None
            return True
        if m == "clrex":
            self.excl = None
            return True
        if m in ("ldr", "ldur") and mem and len(regs) == 1 and regs[0][0] in "hb" and regs[0][1:].isdigit():
            # `ldr h<n>|b<n>, [..]; ucvtf|scvtf s<n>, s<n>`: a u16 or u8 loaded into a float register
            # and converted there (zero-extended, so signed and unsigned agree)
            n, ins = regs[0][1:], self.ins
            nx = ins[self.pos + 1] if self.pos + 1 < len(ins) else None
            if nx is None or nx.mnemonic not in ("ucvtf", "scvtf") or len(nx.operands) != 2 \
                    or [regname(op.reg) for op in nx.operands if op.type == arm64.ARM64_OP_REG] != [f"s{n}", f"s{n}"] \
                    or any(op.type == arm64.ARM64_OP_IMM and op.imm == nx.address for y in ins for op in y.operands
                           if y.mnemonic in ("b", "cbz", "cbnz", "tbz", "tbnz") or y.mnemonic.startswith("b.")):
                return False
            t = "u16" if regs[0][0] == "h" else "u8"
            self.write(f"s{n}", f"(f32)*({t}*){self.address(x, mem[0])}", "f32")
            self.gskip.add(nx.address)
            return True
        if m in ("ldursb", "ldursh", "ldursw") and mem and regs and regs[0][0] in "xw":
            t = {"ldursb": "s8", "ldursh": "s16", "ldursw": "s32"}[m]
            dest = width_type(regs[0])
            self.write(regs[0], f"({dest})*({t}*){self.address(x, mem[0])}", dest)
            return True
        if m in ("ccmp", "ccmn") and len(o) == 3:
            # (flat mode: the values now, before a later write to their registers)
            prev = self.snapshot(self.cond(x.cc, False), "bool")
            a, ta = self.read(regname(o[0].reg))
            b, tb = self.operand(o[1], regname(o[0].reg))
            a, b = self.snapshot(a, "u64" if ta == "char*" else ta), self.snapshot(b, "u64" if tb == "char*" else tb)
            w, nzcv, kind = regname(o[0].reg)[0], o[2].imm, "sub" if m == "ccmp" else "add"
            self.e_refs = " ".join((prev, a, b))

            def chained(cc: int, prev=prev, a=a, b=b, w=w, nzcv=nzcv, kind=kind) -> str:
                holds = nzcv_holds(nzcv, cc)
                if holds is None:
                    raise Unsupported("condition")
                e = self.int_cond(a, b, w, cc, kind)
                return f"!({prev}) || ({e})" if holds else f"({prev}) && ({e})"
            s.cmp = (chained, None, "e")
            return True
        if m in ("cinc", "cinv", "cneg") and len(o) == 2:
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            alt = {"cinc": f"({t})(u64)({a}) + 1", "cinv": f"~({t})(u64)({a})", "cneg": f"-({t})(u64)({a})"}[m]
            self.write(rd, f"({self.cond(x.cc, False)}) ? ({t})({alt}) : ({t})(u64)({a})", t)
            return True
        if m == "csetm" and len(o) == 1:
            rd = regname(o[0].reg)
            t = width_type(rd)
            self.write(rd, f"({self.cond(x.cc, False)}) ? ({t})-1 : ({t})0", t)
            return True
        if m in ("bic", "bics", "orn", "eon") and len(o) == 3:
            rd = regname(o[0].reg)
            if rd[0] not in "xw" and rd not in ("xzr", "wzr"):
                return False
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.operand(o[2], rd)
            op = {"bic": "&", "bics": "&", "orn": "|", "eon": "^"}[m]
            res = f"({t})(u64)({a}) {op} ~({t})({b})"
            self.write(rd, res, t)
            if m == "bics":
                s.cmp = (self.read(rd)[0] if rd not in ("xzr", "wzr") else f"({res})", "0", rd[0])
                self.flagsrc[id(s.cmp)] = (s.cmp, "logic")
            return True
        if m in ("sxtb", "sxth", "sxtw", "uxtb", "uxth") and len(o) == 2:
            rd = regname(o[0].reg)
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            src = {"sxtb": "s8", "sxth": "s16", "sxtw": "s32", "uxtb": "u8", "uxth": "u16"}[m]
            self.write(rd, f"({t})({src})(u64)({a})", t)
            return True
        if m in ("umulh", "smulh") and len(o) == 3:
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.read(regname(o[2].reg))
            w = "unsigned __int128" if m == "umulh" else "__int128"
            st = "u64" if m == "umulh" else "s64"
            self.write(regname(o[0].reg), f"(u64)(((({w})({st})(u64)({a})) * ({w})({st})(u64)({b})) >> 64)", "u64")
            return True
        if m in ("ror", "rorv") and len(o) == 3:
            rd = regname(o[0].reg)
            t = width_type(rd)
            bits = 64 if t == "u64" else 32
            a, _ = self.read(regname(o[1].reg))
            k = f"{o[2].imm % bits}" if o[2].type == arm64.ARM64_OP_IMM else f"(({t})(u64)({self.read(regname(o[2].reg))[0]}) & {bits - 1})"
            self.write(rd, f"(({t})(u64)({a}) >> {k}) | (({t})(u64)({a}) << (({bits} - {k}) & {bits - 1}))", t)
            return True
        if m in ("bfi", "bfxil") and len(o) == 4:
            rd = regname(o[0].reg)
            t = width_type(rd)
            d, _ = self.read(rd)
            a, _ = self.read(regname(o[1].reg))
            lsb, width = o[2].imm, o[3].imm
            mask = (1 << width) - 1
            if m == "bfi":
                e = f"(({t})(u64)({d}) & ~(({t}){mask:#x} << {lsb})) | ((({t})(u64)({a}) & ({t}){mask:#x}) << {lsb})"
            else:
                e = f"(({t})(u64)({d}) & ~({t}){mask:#x}) | ((({t})(u64)({a}) >> {lsb}) & ({t}){mask:#x})"
            self.write(rd, e, t)
            return True
        if m == "negs" and len(o) == 2:
            rd = regname(o[0].reg)
            t = width_type(rd)
            b, _ = self.operand(o[1], rd)
            s.cmp = ("0", b, rd[0])
            self.write(rd, f"({t})0 - ({t})({b})", t)
            return True
        return False

    def step(self, x, last: int) -> bool:
        m, o = x.mnemonic, x.operands
        if self.frame and m in ("add", "sub", "mov") and len(o) >= 2 and o[1].type == arm64.ARM64_OP_REG \
                and regname(o[1].reg) in ("sp", "x29") and regname(o[0].reg) not in ("sp", "x29"):
            base = self.frame["sp"] if regname(o[1].reg) == "sp" else self.frame["fp"]
            imm = (o[2].imm if "semantic" not in self.opts else frame_imm(o[2])) if len(o) == 3 else 0
            off = base + (-imm if m == "sub" else imm)
            if off >= 0 and "semantic" in self.opts:  # a stack argument's address
                self.write(regname(o[0].reg), self.stack_arg(off), "char*")
                return True
            self.write(regname(o[0].reg), self.local(off), "char*")
            return True
        if any(op.type == arm64.ARM64_OP_REG and regname(op.reg) == "sp" for op in o) or \
                (not self.frame and any(op.type == arm64.ARM64_OP_MEM and regname(op.mem.base) in ("sp", "x29")
                                        for op in o)):
            return False
        mem = [op for op in o if op.type == arm64.ARM64_OP_MEM]
        if x.writeback and not (mem and m in tuple(LOADS) + tuple(STORES) + ("stp", "ldp")):
            return False
        if "semantic" in self.opts and self.semantic_step(x, mem):
            return True
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
            # Semantic mode keeps the game's own string (one address for every user, as in main).
            if text is not None and a not in self.orig.sizes and a != DSO_HANDLE and "semantic" not in self.opts:
                self.write(regname(o[0].reg), f"(char*){refl_attrs.c_string(text)}", "char*")
            else:
                self.write(regname(o[0].reg), self.global_ref(a, False), "char*")
        elif m == "ldr" and len(o) == 2 and o[1].type == arm64.ARM64_OP_MEM \
                and regname(o[1].mem.base) != "sp" and self.key(regname(o[1].mem.base)) in s.pages:
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
                self.write(rt, f"*({t}*){self.global_ref(slot, False, {'f32': 4, 'f64': 8}.get(t, 1))}", t)
        elif m in LOADS and mem:
            rt = regname(o[0].reg)
            if rt[0] in "sd" and m in ("ldr", "ldur"):
                t = "f32" if rt[0] == "s" else "f64"
                self.write(rt, f"*({t}*){self.address(x, mem[0], 4 if t == 'f32' else 8)}", t)
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
                self.line(f"*({t}*){self.address(x, mem[0], 4 if t == 'f32' else 8)} = {v};")
                return True
            if rt[0] not in "xw":
                return False
            t = STORES[m] or width_type(rt)
            v, _ = self.read(rt)
            self.line(f"*({t}*){self.address(x, mem[0])} = ({t})(u64)({v});")
        elif m == "stp" and mem:
            rts = [regname(o[k].reg) for k in range(2)]
            if any(r[0] not in "xwsd" for r in rts):
                return False
            vals = [self.read(r)[0] for r in rts]
            size = 8 if rts[0][0] in "xd" else 4
            arg = self.arg_slot(x, mem[0])
            a = self.address(x, mem[0], {"s": 4, "d": 8}.get(rts[0][0], 1)) if arg is None else None
            for k, (rt, v) in enumerate(zip(rts, vals)):
                t = ftype(rt) or width_type(rt)
                at = f"({a} + {k * size})" if arg is None else self.stack_arg(arg + k * size, size)
                self.line(f"*({t}*){at} = {v};" if rt[0] in "sd" else f"*({t}*){at} = ({t})(u64)({v});")
        elif m == "ldp" and mem and regname(o[0].reg)[0] in "xwsd":
            size = 8 if regname(o[0].reg)[0] in "xd" else 4
            arg = self.arg_slot(x, mem[0])
            if arg is None:
                tmp = self.temp_ptr(self.address(x, mem[0], {"s": 4, "d": 8}.get(regname(o[0].reg)[0], 1)))
            for k in range(2):
                rt = regname(o[k].reg)
                t = ftype(rt) or width_type(rt)
                at = f"({tmp} + {k * size})" if arg is None else self.stack_arg(arg + k * size, size)
                self.write(rt, f"*({t}*){at}", t)
        elif m == "mov" and len(o) == 2 and regname(o[0].reg)[0] == "v" and o[1].type == arm64.ARM64_OP_REG \
                and regname(o[1].reg)[0] == "v":
            # A whole-register float copy (mov vD.16b, vS.16b): keep the source's width.
            src = 100 + int(regname(o[1].reg)[1:])
            t = self.s.vals.get(src, ("", self.params.get(src, "f32")))[1] if src in self.s.vals \
                else getattr(self, "types", {}).get(src, "f32")
            letter = "s" if t == "f32" else "d"
            v, _ = self.read(f"{letter}{src - 100}")
            self.write(f"{letter}{regname(o[0].reg)[1:]}", v, t)
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
            src = regname(o[1].reg) if o[1].type == arm64.ARM64_OP_REG else None
            t = ftype(rd)
            if t is None:
                # fmov w, s / x, d: the float's bits.
                if rd[0] not in "xw" or src is None or ftype(src) != ("f32" if rd[0] == "w" else "f64"):
                    return False
                v, _ = self.read(src)
                self.write(rd, f"lift_bits({v})", width_type(rd))
            elif o[1].type == arm64.ARM64_OP_FP:
                self.write(rd, repr(float(o[1].fp)) + ("f" if t == "f32" else ""), t, float(o[1].fp))
            elif src in ("wzr", "xzr"):
                self.write(rd, "0.0f" if t == "f32" else "0.0", t, 0)
            elif src[0] == rd[0]:
                v, _ = self.read(src)
                self.write(rd, v, t)
            elif src[0] == ("w" if t == "f32" else "x"):
                # Bits from an integer register: a constant (mov/movk) becomes a literal.
                n = self.key(src)
                if n in s.consts:
                    lit = float_literal(s.consts[n], t)
                    if lit is None:
                        return False
                    self.write(rd, lit, t)
                else:
                    v, _ = self.read(src)
                    self.write(rd, f"lift_{t}(({width_type(src)})(u64)({v}))", t)
            else:
                return False
        elif m == "movi" and len(o) == 2 and ftype(regname(o[0].reg)) == "f64" and o[1].imm == 0:
            self.write(regname(o[0].reg), "0.0", "f64", 0)
        elif (m in FBINARY and len(o) == 3) or (m in FUNARY and len(o) == 2) or (m in FTERNARY and len(o) == 4):
            rd = regname(o[0].reg)
            t = ftype(rd)
            if t is None or any(op.type != arm64.ARM64_OP_REG or ftype(regname(op.reg)) != t for op in o):
                return False
            v = [self.read(regname(op.reg))[0] for op in o[1:]]
            fmt = FBINARY.get(m) or FUNARY.get(m) or FTERNARY[m]
            self.write(rd, fmt.format(**dict(zip("abc", v)), s="f" if t == "f32" else ""), t)
        elif m == "fcsel" and len(o) == 3:
            rd = regname(o[0].reg)
            t = ftype(rd)
            if t is None:
                return False
            a, b = self.read(regname(o[1].reg))[0], self.read(regname(o[2].reg))[0]
            ka, kb = self.key(regname(o[1].reg)), self.key(regname(o[2].reg))
            if s.consts.get(ka) == 1 and s.consts.get(kb) == 0 and ka != kb:
                # 1.0 or 0.0: a bool converted to float (the select ISel makes of it).
                self.write(rd, f"({t})({self.cond(x.cc, False)})", t)
            elif s.cmp and len(s.cmp) == 4 and s.cmp[2] in "xw" and x.cc in (arm64.ARM64_CC_NE, arm64.ARM64_CC_EQ):
                # A bool parameter (tst w<n>, #1): only `bool` keeps fcsel ne after tst.
                n = s.cmp[3]
                self.params[n] = "bool"
                self.write(rd, f"{'' if x.cc == arm64.ARM64_CC_NE else '!'}p{n} ? {a} : {b}", t)
            else:
                self.write(rd, f"({self.cond(x.cc, False)}) ? {a} : {b}", t)
        elif m in ("scvtf", "ucvtf") and len(o) == 2:
            rd, rn = regname(o[0].reg), regname(o[1].reg)
            t = ftype(rd)
            if t is None or rn[0] not in "xwsd":
                return False
            v, _ = self.read(rn)
            st = ("s" if m == "scvtf" else "u") + ("64" if rn[0] in "xd" else "32")
            # From a float register: an integer loaded straight into it (ldr s; ucvtf s, s).
            self.write(rd, f"({t})({st})lift_bits({v})" if rn[0] in "sd" else f"({t})({st})(u64)({v})", t)
        elif m in FTOINT and len(o) == 2:
            rd, rn = regname(o[0].reg), regname(o[1].reg)
            ft = ftype(rn)
            if ft is None or rd[0] not in "xw":
                return False
            v, _ = self.read(rn)
            if FTOINT[m]:
                v = f"{FTOINT[m].format(s='f' if ft == 'f32' else '')}({v})"
            it = ("s" if m[-1] == "s" else "u") + ("64" if rd[0] == "x" else "32")
            self.write(rd, f"({width_type(rd)})({it})({v})", width_type(rd))
        elif m == "fcvt" and len(o) == 2:
            rd, rn = regname(o[0].reg), regname(o[1].reg)
            if ftype(rd) is None or ftype(rn) is None:
                return False
            v, _ = self.read(rn)
            self.write(rd, f"({ftype(rd)})({v})", ftype(rd))
        elif m == "fccmp" and len(o) == 3:
            # Flags of the fcmp if the condition holds, else the immediate: one condition
            # chained with (&&) or (||) to the comparison.
            prev = self.snapshot(self.cond(x.cc, False), "bool")  # (semantic flat mode: see keep_compare)
            a, t = self.read(regname(o[0].reg))
            b, _ = self.read(regname(o[1].reg))
            a, b = self.snapshot(a, t), self.snapshot(b, t)
            fc, nzcv = (a, b, "f", t), o[2].imm
            self.e_refs = " ".join((prev, a, b))  # (semantic flat mode: see keep_compare)

            def chained(cc: int, prev=prev, fc=fc, nzcv=nzcv) -> str:
                holds = nzcv_holds(nzcv, cc)
                if holds is None:
                    raise Unsupported("condition")
                e = self.fcond(fc, cc)
                return f"!({prev}) || ({e})" if holds else f"({prev}) && ({e})"
            s.cmp = (chained, None, "e")
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
        elif m in ("subs", "adds", "ands") and len(o) == 3:
            # The operation, then flags: subs compares its operands; adds/ands compare the
            # result with zero (enough for eq/ne and the signed conditions).
            rd = regname(o[0].reg)
            if rd[0] not in "xw" and rd not in ("xzr", "wzr"):
                return False
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.operand(o[2], rd)
            op = {"subs": "-", "adds": "+", "ands": "&"}[m]
            res = f"({t})(u64)({a}) {op} ({t})({b})"
            if m == "subs":
                s.cmp = (a, b, rd[0])
                self.write(rd, res, t)
            else:
                # (semantic mode keeps the operands: an addition's carry and overflow need them)
                sa, sb = self.snapshot(a, "u64"), self.snapshot(b, "u64")
                self.write(rd, res, t)
                s.cmp = (self.read(rd)[0] if rd not in ("xzr", "wzr") else f"({res})", "0", rd[0])
                self.flagsrc[id(s.cmp)] = (s.cmp, "add" if m == "adds" else "logic", sa, sb)
        elif m in ("cmn", "tst") and len(o) == 2:
            a, _ = self.read(regname(o[0].reg))
            b, _ = self.operand(o[1], regname(o[0].reg))
            t = width_type(regname(o[0].reg))
            s.cmp = (f"({t})(({t})(u64)({a}) {'+' if m == 'cmn' else '&'} ({t})({b}))", "0", regname(o[0].reg)[0])
            self.flagsrc[id(s.cmp)] = (s.cmp, "add" if m == "cmn" else "logic",
                                       self.snapshot(a, "u64"), self.snapshot(b, "u64"))
            n = self.key(regname(o[0].reg))
            if m == "tst" and b == "1" and a == f"p{n}" and self.params.get(n) == "u32" \
                    and "semantic" not in self.opts:  # (a bool parameter only reads bit 0 in this compiler)
                s.cmp += (n,)  # bit 0 of a parameter: maybe a bool (see fcsel)
        elif m in ("csel", "csinc", "csneg", "csinv") and len(o) == 3:
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.read(regname(o[2].reg))
            b = {"csel": f"({t})(u64)({b})", "csinc": f"({t})(u64)({b}) + 1",
                 "csneg": f"-({t})(u64)({b})", "csinv": f"~({t})(u64)({b})"}[m]
            self.write(rd, f"({self.cond(x.cc, False)}) ? ({t})(u64)({a}) : ({t})({b})", t)
        elif m in ("mul", "udiv", "sdiv", "lslv", "lsrv", "asrv") or \
                (m in ("lsl", "lsr", "asr") and len(o) == 3 and o[2].type == arm64.ARM64_OP_REG):
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            t = width_type(rd)
            st = "s64" if t == "u64" else "s32"
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.read(regname(o[2].reg))
            m2 = m[:-1] if m in ("lslv", "lsrv", "asrv") else m
            sem = "semantic" in self.opts
            if m2 in ("sdiv", "asr"):
                op = "/" if m2 == "sdiv" else ">>"
                e = f"({t})(({st})(u64)({a}) {op} ({st})(u64)({b}))"
                if sem and m2 == "sdiv":  # AArch64: x / 0 is 0 and MIN / -1 is MIN (x86-64 traps)
                    e = f"(({st})(u64)({b}) == 0 ? ({t})0 : ({st})(u64)({b}) == -1 ? ({t})(0 - ({t})(u64)({a})) : {e})"
                elif sem:  # the shift amount is taken modulo the width
                    e = f"({t})(({st})(u64)({a}) >> (({t})(u64)({b}) & {8 * SIZES[t] - 1}))"
                self.write(rd, e, t)
            else:
                op = {"mul": "*", "udiv": "/", "lsl": "<<", "lsr": ">>"}[m2]
                e = f"({t})(u64)({a}) {op} ({t})(u64)({b})"
                if sem and m2 == "udiv":
                    e = f"(({t})(u64)({b}) == 0 ? ({t})0 : ({t})({e}))"
                elif sem and m2 in ("lsl", "lsr"):
                    e = f"({t})(u64)({a}) {op} (({t})(u64)({b}) & {8 * SIZES[t] - 1})"
                self.write(rd, e, t)
        elif m in ("madd", "msub") and len(o) == 4:
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            t = width_type(rd)
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.read(regname(o[2].reg))
            c, _ = self.read(regname(o[3].reg))
            self.write(rd, f"({t})(u64)({c}) {'+' if m == 'madd' else '-'} ({t})(u64)({a}) * ({t})(u64)({b})", t)
        elif m in ("umull", "smull", "umaddl", "smaddl") and len(o) >= 3:
            rd = regname(o[0].reg)
            w = "u32" if m[0] == "u" else "s32"
            a, _ = self.read(regname(o[1].reg))
            b, _ = self.read(regname(o[2].reg))
            prod = f"(u64)({w})(u64)({a}) * (u64)({w})(u64)({b})"
            if len(o) == 4:
                c, _ = self.read(regname(o[3].reg))
                prod = f"(u64)({c}) + {prod}"
            self.write(rd, prod, "u64")
        elif m in ("mvn", "neg") and len(o) == 2:
            rd = regname(o[0].reg)
            if rd[0] not in "xw":
                return False
            t = width_type(rd)
            b, _ = self.operand(o[1], rd)
            self.write(rd, f"{'~' if m == 'mvn' else '-'}({t})(u64)({b})", t)
        elif m in ("ubfx", "sbfx", "ubfiz", "sbfiz") and len(o) == 4:
            rd = regname(o[0].reg)
            t = width_type(rd)
            st = "s64" if t == "u64" else "s32"
            bits = 64 if t == "u64" else 32
            a, _ = self.read(regname(o[1].reg))
            lsb, width = o[2].imm, o[3].imm
            if m in ("ubfx", "ubfiz"):
                field = f"(({t})(u64)({a}) >> {lsb} & ({t}){(1 << width) - 1:#x})" if m == "ubfx" \
                    else f"(({t})(u64)({a}) & ({t}){(1 << width) - 1:#x})"
            else:
                src = f"({st})(({t})(u64)({a}) << {bits - width - (lsb if m == 'sbfx' else 0)})" if m == "sbfx" \
                    else f"({st})(({t})(u64)({a}) << {bits - width})"
                field = f"(({src}) >> {bits - width})"
            self.write(rd, f"({t})({field}){f' << {lsb}' if m.endswith('iz') else ''}", t)
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
        elif m == "bl" and x.address == last:
            # Code ends with a call, so the callee doesn't return (declared [[noreturn]]).
            fn = f"fn_{o[0].imm:x}"
            args = self.args(self.callee_need(fn))
            self.decls.add(f"namespace noret {{ [[noreturn]] void {fn}({', '.join(t for _, t in args)}); }}")
            self.terminate("noret", f"noret::{fn}({', '.join(e for e, _ in args)})")
        elif m == "bl" and self.sret_callee(o[0].imm):
            self.sret_call(f"fn_{o[0].imm:x}")
            self.clobber()
            self.s.written = set()
        elif m == "blr" and "semantic" in self.opts and self.s.x8w and regname(o[0].reg) != "x8":
            # An indirect callee given a local in x8 builds its result there (the original reads it
            # back): as sret_call, through the pointer.
            ptr, _ = self.read(regname(o[0].reg))
            self.s.written.discard(self.key(regname(o[0].reg)))
            x8, _ = self.read("x8")
            args = self.args(self.callee_need(""))
            types, values = ", ".join(t for _, t in args), ", ".join(e for e, _ in args)
            self.decls.add(LIFT_AT)
            self.line(f"new ((void*)({x8}), lift_at_t{{}}) UnkResult(((UnkResult (*)({types}))({ptr}))({values}));")
            self.clobber()
            self.s.written = set()
        elif m in ("bl", "blr"):
            if m == "bl":
                call = self.call(f"fn_{o[0].imm:x}")
            else:
                ptr, _ = self.read(regname(o[0].reg))
                self.s.written.discard(self.key(regname(o[0].reg)))  # the target isn't an argument
                call = self.call("", ptr)
            self.clobber()
            self.write("x0", call, "char*")
            self.s.written = set()
            # Until s0/d0 is read (then the callee returns a float; see retype_call).
            var = self.s.vals[0][0] if 0 in self.s.vals else "X0"
            self.s.fcall = (len(self.body) - 1, var, call) + self.last_call
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
        self.vtemps: list[str] = []  # semantic mode: snapshots of values a pending compare reads (their types)
        self.flag_vars = False  # semantic mode: NZCV kept in locals (store_flags)
        self.flags_bad = False  # ... except after a compare whose flags could not be written out
        self.clob: set[int] = set()  # semantic mode: caller-saved registers a call left, unwritten (clobber)

    def store_flags(self) -> None:
        """Semantic mode: NZCV of the compare just made, in locals, for a condition read in another
        block (flat mode forgets the pending compare at every label: `cmp; b L` ... `L: b.ls`)."""
        C = arm64
        try:
            exprs = [self.cond(cc, False) for cc in (C.ARM64_CC_MI, C.ARM64_CC_EQ, C.ARM64_CC_HS, C.ARM64_CC_VS)]
        except Unsupported:
            self.flags_bad = True  # (for good: the locals are run-time state, any path may reach a use)
            return
        self.flag_vars = True
        for name, e in zip("NZCV", exprs):
            self.line(f"f{name} = ({e});")

    def flag_cond(self, cc: int, negate: bool) -> str:
        """Condition cc from the locals store_flags wrote."""
        if not self.flag_vars or self.flags_bad:
            raise Unsupported("condition")
        if negate:
            cc = cc + 1 if cc % 2 else cc - 1
        C = arm64
        return {C.ARM64_CC_EQ: "fZ", C.ARM64_CC_NE: "!fZ", C.ARM64_CC_HS: "fC", C.ARM64_CC_LO: "!fC",
                C.ARM64_CC_MI: "fN", C.ARM64_CC_PL: "!fN", C.ARM64_CC_VS: "fV", C.ARM64_CC_VC: "!fV",
                C.ARM64_CC_HI: "(fC && !fZ)", C.ARM64_CC_LS: "(!fC || fZ)", C.ARM64_CC_GE: "(fN == fV)",
                C.ARM64_CC_LT: "(fN != fV)", C.ARM64_CC_GT: "(!fZ && fN == fV)",
                C.ARM64_CC_LE: "(fZ || fN != fV)"}[cc]

    def ftype_of(self, n: int) -> str:
        # Semantic mode: this lifter keeps types in self.types, not in the state (byte mode keeps
        # the old lookup, whose output is what matched).
        return self.types.get(n, "f32") if "semantic" in self.opts else super().ftype_of(n)

    def find_jump_tables(self, ins) -> None:
        """Semantic mode: Clang's compressed jump tables, `adr xL, base; ldrb|ldrh wE, [xT, xI{, lsl
        #1}]; add xL, xL, xE, lsl #2; br xL` (other instructions may sit in between), with `cmp wI,
        #N; b.hi` up to 16 instructions before (or the compare on J and `mov wI, wJ` after it) and
        xT last set by `adrp`+`add`. self.jtables maps the br to (index
        register, targets read from the image's table); gskip takes the adr, the load and the add,
        which the switch replaces."""
        img = self.orig.img
        at = {x.address: k for k, x in enumerate(ins)}
        end = self.addr + 4 * len(ins)
        writes: dict[str, list[int]] = {}  # x<n> -> indices of the instructions writing it
        for k, x in enumerate(ins):
            for q in x.regs_access()[1]:
                name = regname(q)
                writes.setdefault("x" + name[1:] if name[0] == "w" else name, []).append(k)
        for k, x in enumerate(ins):
            if x.mnemonic != "br":
                continue
            rr = regname(x.operands[0].reg)
            add = ld = adr = None
            for j in range(k - 1, max(k - 10, -1), -1):
                y = ins[j]
                ops = y.operands
                if add is None and y.mnemonic == "add" and len(ops) == 3 and regname(ops[0].reg) == rr \
                        and ops[2].type == arm64.ARM64_OP_REG and ops[2].shift.value == 2:
                    add = j
                elif add is not None and ld is None and y.mnemonic in ("ldrb", "ldrh") and ops[1].mem.index \
                        and regname(ops[0].reg)[1:] == regname(ins[add].operands[2].reg)[1:]:
                    ld = j
                elif add is not None and y.mnemonic == "adr" and regname(ops[0].reg) == regname(ins[add].operands[1].reg):
                    adr = j
            if None in (add, ld, adr):
                continue
            mem = ins[ld].operands[1].mem
            treg, ireg = regname(mem.base), "x" + regname(mem.index)[1:]
            size = 1 if ins[ld].mnemonic == "ldrb" else 2
            # the table: the last writes of T before the load are adrp T, #page then add T, T, #lo
            tw = [j for j in writes.get(treg, []) if j < ld][-2:]
            if len(tw) != 2 or ins[tw[0]].mnemonic != "adrp" or ins[tw[1]].mnemonic != "add" \
                    or ins[tw[1]].operands[2].type != arm64.ARM64_OP_IMM:
                continue
            table = ins[tw[0]].operands[1].imm + ins[tw[1]].operands[2].imm
            # cmp wI, #N; b.hi shortly before, I unwritten until the br; or the compare is on J and
            # `mov wI, wJ` copies it in between (J then unwritten from the compare to the copy)
            bound, cmp_at, copy, src = None, None, None, ireg
            for j in range(min(adr, ld) - 1, max(min(adr, ld) - 16, -1), -1):
                y = ins[j]
                if copy is None and y.mnemonic == "mov" and len(y.operands) == 2 \
                        and y.operands[1].type == arm64.ARM64_OP_REG and regname(y.operands[0].reg)[1:] == ireg[1:] \
                        and regname(y.operands[1].reg)[0] in "xw" and regname(y.operands[1].reg)[1:].isdigit():
                    copy, src = j, "x" + regname(y.operands[1].reg)[1:]
                    continue
                if y.mnemonic == "cmp" and y.operands[0].type == arm64.ARM64_OP_REG \
                        and regname(y.operands[0].reg)[1:] == src[1:] and y.operands[1].type == arm64.ARM64_OP_IMM \
                        and j + 1 < len(ins) and ins[j + 1].mnemonic == "b.hi":
                    bound, cmp_at = y.operands[1].imm, j
                    break
            if bound is None or any(cmp_at < j < k and j != copy for j in writes.get(ireg, [])) \
                    or (copy is not None and any(cmp_at < j < copy for j in writes.get(src, []))):
                continue
            base = ins[adr].operands[1].imm
            tgts = []
            for e in range(bound + 1):
                off = table - BASE + e * size
                v = img.data[off] if size == 1 else int.from_bytes(img.data[off:off + 2], "little")
                tgts.append(base + 4 * v)
            if not all(self.addr <= t < end and t in at for t in tgts):
                continue
            self.jtables[x.address] = (int(ireg[1:]), tgts)
            self.gskip |= {ins[adr].address, ins[ld].address, ins[add].address}

    def snapshot(self, e: str, t: str) -> str:
        """The value of `e` now, in a temporary (registers are mutable locals here). Semantic
        mode only; the byte-identical output stays as it was."""
        if "semantic" not in self.opts:
            return e
        self.vtemps.append(t)
        self.line(f"v{len(self.vtemps) - 1} = ({t})({e});")
        return f"v{len(self.vtemps) - 1}"

    def keep_compare(self, n: int) -> None:
        """Semantic mode: register n is about to be written while the pending compare reads it
        (`subs x10, x10, #56` then `b.ne`): the compare keeps the old value."""
        s = self.s
        if not s.cmp:
            return
        names = [f"X{n}"] if n < 100 else [f"S{n - 100}", f"D{n - 100}"]
        if s.cmp[2] == "e":
            if any(re.search(rf"\b{v}\b", getattr(self, "e_refs", "")) for v in names):
                raise Unsupported("flags operand overwritten")
            return
        a, b = str(s.cmp[0]), str(s.cmp[1])
        old = s.cmp
        for v in names:
            if re.search(rf"\b{v}\b", a + " " + b):
                t = "u64" if n < 100 else self.types.get(n, "f32")
                snap = self.snapshot(v, t)
                a, b = re.sub(rf"\b{v}\b", snap, a), re.sub(rf"\b{v}\b", snap, b)
        if (a, b) != (str(old[0]), str(old[1])):
            s.cmp = (a, b) + tuple(old[2:])
            if id(old) in self.flagsrc:
                self.flagsrc[id(s.cmp)] = (s.cmp,) + self.flagsrc[id(old)][1:]

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
        if n in self.clob:
            raise Unsupported("caller-saved register read after a call")
        if n in s.pages:
            if "semantic" in self.opts and r[0] == "x":
                return self.global_ref(s.pages[n], False), "char*"  # a bare page: its address
            raise Unsupported("bare page")
        if n == 100 and s.fcall:
            t = ftype(r)
            self.retype_call(t, self.storage(100, t))
            self.types[100] = t
            s.fcall = None
        elif n == 0:
            s.fcall = None  # the call's x0 result is used
        if n not in self.types:
            semantic_param = "semantic" in self.opts and (n <= 7 or 100 <= n <= 107)
            if (s.called or (n > 8 and not 100 <= n <= 107)) and not semantic_param:
                if "semantic" not in self.opts:
                    raise Unsupported(r)
                # Read before its first write in address order (a loop, or a block placed before
                # the one that sets it): every path that reaches here wrote it; zero-initialised.
                if n >= 100:
                    self.types[n] = ftype(r)
                    return self.storage(n, self.types[n]), self.types[n]
                self.types[n] = "u64"
                return (f"(u32)X{n}", "u32") if r[0] == "w" else (f"X{n}", "u64")
            # (semantic mode: an argument register unwritten in address order holds the incoming
            # argument on every path that has not written it: the machine's state at entry)
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
        if "semantic" in self.opts and t in ("u32", "u16", "u8", "bool"):
            # The type is the last write's in address order, not necessarily on this path: a
            # 64-bit read takes the whole register (a 32-bit write zero-extended it anyway).
            return f"X{n}", "u64"
        return (f"X{n}" if t == "u64" else f"(({t})X{n})"), t

    def write(self, r: str, e: str, t: str, const: int | None = None) -> None:
        n = self.key(r)
        if n < 0:
            return
        if "semantic" in self.opts:
            self.keep_compare(n)
        self.clob.discard(n)
        if n >= 100:
            self.line(f"{self.storage(n, t)} = {e};")
        else:
            self.line(f"X{n} = (u64)({e});")
        self.types[n] = t
        s = self.s
        if n in (0, 100):
            s.retk = n
        if n == 100:
            s.fcall = None
        s.pages.pop(n, None)
        if const is None:
            s.consts.pop(n, None)
        else:
            s.consts[n] = const
        if n <= 7 or 100 <= n <= 107:
            s.written.add(n)
        if n == 8:
            s.x8w = "stk" in e  # (an indirect callee may build its result there: Lifter.step)

    def has_value(self, n: int) -> bool:
        return n in self.types

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
        s.x8w = False
        if "semantic" in self.opts:
            # The caller-saved registers hold garbage now (or a result the call does not name,
            # x1 of a 16-byte one), while their locals keep what they held: read() refuses them
            # until written, in this straight line of code (a label forgets it: other paths).
            self.clob = set(range(1, 19)) | set(range(101, 108)) | set(range(116, 132))

    def lift(self) -> tuple[str, set[str]] | None:
        orig, addr = self.orig, self.addr
        code = orig.function(addr)
        ins = self.ins = list(CS.disasm(code, addr))
        if not ins or len(ins) * 4 != len(code):
            return self.fail("undecodable instruction")
        end = addr + len(code)
        last = ins[-1].address
        if not self.setup_frame(ins):
            return self.fail("stack frame")
        self.cur_delta = 0
        self.fwidth = float_widths(ins, "semantic" in self.opts)
        targets = set()
        for x in ins:
            for op in x.operands:
                if op.type == arm64.ARM64_OP_IMM and (x.mnemonic in ("b", "cbz", "cbnz", "tbz", "tbnz")
                                                      or x.mnemonic.startswith("b.")) \
                        and addr <= op.imm < end:
                    targets.add(op.imm)
        self.jtables: dict[int, tuple[int, list[int]]] = {}
        try:
            self.find_guards(ins)
            if "semantic" in self.opts:
                self.find_jump_tables(ins)
                for _, tgts in self.jtables.values():
                    targets |= set(tgts)
                self.own_args()
            for self.pos, x in enumerate(ins):
                if x.address in self.gskip:
                    continue
                if x.address in targets:
                    self.body.append(f"L_{x.address:x}:;")
                    self.s.cmp = None if x.address != addr else self.s.cmp
                    self.s.fcall = None
                    self.clob = set()
                if self.pos:
                    self.frame_reset(ins[self.pos - 1])
                if self.frame_op(x) or self.guard_step(x):
                    continue
                m, o = x.mnemonic, x.operands
                if m == "ret":
                    v = None
                    if 100 in self.types and (self.s.retk == 100 or 0 not in self.types):
                        v = self.read("s0" if self.types[100] == "f32" else "d0")
                    elif 0 in self.types:
                        v = self.read("x0")
                    self.terminate("value", v)
                elif m == "b" and not addr <= o[0].imm < end and self.sret_callee(o[0].imm):
                    self.sret_tail(f"fn_{o[0].imm:x}")
                elif m == "b" and not addr <= o[0].imm < end:
                    self.terminate("tail", self.call(f"fn_{o[0].imm:x}"))
                    self.tails[f"fn_{o[0].imm:x}"] = self.last_call[1]
                elif m == "br" and x.address in self.jtables:
                    idx, tgts = self.jtables[x.address]
                    v, _ = self.read(f"x{idx}")
                    cases = " ".join(f"case {k}: goto L_{t:x};" for k, t in enumerate(tgts))
                    self.line(f"switch ((u64)({v})) {{ {cases} default: __builtin_unreachable(); }}")
                elif m == "br":
                    ptr, _ = self.read(regname(o[0].reg))
                    self.s.written.discard(self.key(regname(o[0].reg)))  # the target isn't an argument
                    self.terminate("tail", self.call("", ptr))
                elif m == "b":
                    self.line(f"goto L_{o[0].imm:x};")
                else:
                    before = self.s.cmp
                    if not self.step(x, last):
                        return self.fail(f"insn: {m}")
                    if "semantic" in self.opts and self.s.cmp is not None and self.s.cmp is not before:
                        self.store_flags()
        except Unsupported as e:
            return self.unsupported(e)
        self.pos = len(ins)
        if ins[-1].mnemonic not in ("ret", "b", "br", "bl"):
            return self.fail("falls off the end")
        # Declarations go first: gotos must not jump over initialisations.
        pre = []
        for n in sorted(k for k in self.types if k < 100):
            init = f" = (u64)p{n}" if n in self.params else (" = (u64)&ret" if n == 8 and self.sret else
                                                             " = 0" if "semantic" in self.opts else "")
            pre.append(f"    u64 X{n}{init};")
        for n in sorted(k for k in self.types if k >= 100):
            t = self.types[n]
            init = f" = f{n - 100}" if n in self.params else ""
            if "semantic" in self.opts:
                # Both widths: a register used as s<n> in one place and d<n> in another has a
                # variable for each (the parameter initialises the one it arrives as).
                pt = self.params.get(n)
                for w in ("f32", "f64"):
                    pre.append(f"    {w} {self.storage(n, w)} = {'f%d' % (n - 100) if pt == w else '0'};")
                continue
            pre.append(f"    {t} {self.storage(n, t)}{init};")
        pre += [f"    char* t{i};" for i in range(self.temps)]
        pre += [f"    {t} v{i};" for i, t in enumerate(self.vtemps)]
        if self.flag_vars:
            pre.append("    bool fN = 0, fZ = 0, fC = 0, fV = 0;")
        self.body = pre + self.body
        self.rets = [(ln + len(pre), k, p) for ln, k, p in self.rets]
        return self.finish()


def defined_elsewhere() -> set[int]:
    """Functions defined in src/ outside src/lifted/ (by hand or by another generator)."""
    return defined_outside(OUT)


def defined_outside(out: Path) -> set[int]:
    """Functions defined in src/ outside `out`, by name or by tag."""
    skip = set()
    for f in (ROOT / "src").rglob("*.cpp"):
        if out not in f.parents:
            text = f.read_text(encoding="utf-8")
            lines = [ln.split("//", 1)[0] for ln in text.splitlines()]  # comments don't count
            for i, line in enumerate(lines):
                m = NAME_RE.match(line)
                if m:
                    # A definition reaches `{` before `;` (declarations may span several lines).
                    rest = " ".join(lines[i:i + 20])
                    brace, semi = rest.find("{"), rest.find(";")
                    if brace >= 0 and (semi < 0 or brace < semi):
                        skip.add(int(m.group(1), 16))
            # Template instantiations and macro-defined getters, named by their tags. Only the
            # generators define functions that way; hand-written code may just use a tag.
            if any(ROOT / "src" / d in f.parents for d in TAGGED_DIRS):
                skip |= {int(a, 16) for a in TAG_RE.findall(text)}
    return skip


def candidates(orig: asmdiff.Original) -> list[int]:
    # Functions defined elsewhere in src/ are left alone.
    skip = defined_elsewhere()
    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
    return [a for a in rows if a not in skip and orig.sizes.get(a, MAX_SIZE + 1) <= MAX_SIZE]


HEADER = ['#include "types.h"', '#define HIDDEN __attribute__((visibility("hidden")))',
          "// Bit casts between float and integer registers (fmov).",
          "inline f32 lift_f32(u32 v) { f32 f; __builtin_memcpy(&f, &v, 4); return f; }",
          "inline f64 lift_f64(u64 v) { f64 f; __builtin_memcpy(&f, &v, 8); return f; }",
          "inline u32 lift_bits(f32 f) { u32 v; __builtin_memcpy(&v, &f, 4); return v; }",
          "inline u64 lift_bits(f64 f) { u64 v; __builtin_memcpy(&v, &f, 8); return v; }",
          "// Inlined helpers on guarded statics (GUARDS).",
          "namespace lift_guard {",
          "u32 fn_7100080568(); u32 fn_7100080570(); void fn_710007fdb4();",
          "extern char* g_unk_7101d47108; extern char* g_unk_7101d47200;",
          "struct StringPoolHolder { u8 unk_0x0[0xe8]; StringPoolHolder(u32 a, char** pool); ~StringPoolHolder(); };",
          "inline void EnsureStringPool() { static StringPoolHolder s_holder(fn_7100080568(), &g_unk_7101d47108); fn_710007fdb4(); }",
          "inline void EnsureFilePathPool() { static StringPoolHolder s_holder(fn_7100080570(), &g_unk_7101d47200); fn_710007fdb4(); }",
          "struct Singleton { Singleton(); ~Singleton(); };",
          "inline Singleton& GetSingleton() { static Singleton s_singleton; return s_singleton; }",
          "struct ZeroWord { u32 value; ZeroWord() : value(0) {} };",
          "template <class T> struct TypeId {",
          "    static ZeroWord& base() { static ZeroWord s_base; return s_base; }",
          "    static u32& id() { static u32 s_id = (u8)~*reinterpret_cast<u8*>(&base()); return s_id; }",
          "};",
          "struct TypeIdTag;",
          "}",
          "// Returned in memory through x8 (built in place); the real size is unknown.",
          "struct UnkResult { char data[0x100]; };",
          "// Accesses that may alias any other (the \"alias\" rewrite): the original reloads values",
          "// a store of another type could have changed.",
          *[f"typedef {t} __attribute__((may_alias)) a_{t};"
            for t in ("u8", "u16", "u32", "u64", "s8", "s16", "s32", "f32", "f64")], ""]


# Placement new without <new>, for a callee's x8 result built where x8 points (Lifter.sret_call). A
# definition, so a generated file holds it once (write_out), not once per function using it.
LIFT_AT = ("struct lift_at_t {}; inline void* operator new(decltype(sizeof 0), void* p, lift_at_t) "
           "noexcept { return p; }")

_ORIG: asmdiff.Original | None = None  # for forked lifting workers


def _lift_part(job: tuple[list[int], bool, frozenset]):
    part, want_why, opts = job
    why: dict[int, tuple[int, str]] | None = {} if want_why else None
    return lift_serial(_ORIG, part, why, opts), why


def lift_all(orig: asmdiff.Original, cands: list[int],
             why: dict[int, tuple[int, str]] | None = None,
             opts: frozenset = frozenset()) -> list[dict[int, tuple[str, set[str]]]]:
    """lift_serial over slices of `cands` in parallel processes (each function is lifted on
    its own, so the result doesn't depend on the slicing)."""
    global _ORIG
    import multiprocessing
    import os
    n = os.cpu_count() or 1
    if n == 1 or len(cands) < 200 or "fork" not in multiprocessing.get_all_start_methods():
        return lift_serial(orig, cands, why, opts)
    _ORIG = orig
    parts = [cands[k::n * 4] for k in range(n * 4)]
    with multiprocessing.get_context("fork").Pool(n) as pool:
        results = pool.map(_lift_part, [(p, why is not None, opts) for p in parts])
    lifted: list[dict[int, tuple[str, set[str]]]] = []
    for part_lifted, part_why in results:
        for k, defs in enumerate(part_lifted):
            if k == len(lifted):
                lifted.append({})
            lifted[k].update(defs)
        if why is not None:
            why.update(part_why)
    return lifted


def lift_serial(orig: asmdiff.Original, cands: list[int],
                why: dict[int, tuple[int, str]] | None = None,
                opts: frozenset = frozenset()) -> list[dict[int, tuple[str, set[str]]]]:
    """Every lifting attempt, in variant order: one {address: (definition, declarations)} per
    attempt. With `why`, the failure that got furthest is kept for each address (earliest
    attempt on a tie) as (instruction index, reason). `opts` are body rewrites (ROUND2)."""
    lifted: list[dict[int, tuple[str, set[str]]]] = []
    # Byte mode with facts: each variant with pass-through first, then, for the functions where it
    # named more than before, the plain lift as it was (a separate attempt, tried after it).
    passing = Lifter.facts is not None and "semantic" not in opts

    def attempt(lifter: Lifter, a: int, defs: dict) -> None:
        lifter.opts = opts
        r = lifter.lift()
        if r:
            defs[a] = r
        elif why is not None and (a not in why or lifter.pos > why[a][0]):
            why[a] = (lifter.pos, lifter.reason)

    def variants(cls, todo: list[int], stack=None) -> None:
        for extra, returns in VARIANTS:
            defs, plain = {}, {}
            for a in todo:
                lifter = cls(orig, a, extra, returns, stack)
                attempt(lifter, a, defs)
                if lifter.passed:
                    p = cls(orig, a, extra, returns, stack)
                    p.plain = True
                    attempt(p, a, plain)
            lifted.append(defs)
            if passing:
                lifted.append(plain)

    variants(Lifter, cands)
    structured = set().union(*lifted)
    rest = [a for a in cands if a not in structured]
    variants(FlatLifter, rest)
    # Functions with stack locals: each layout of the locals array, structured then flat.
    rest = [a for a in rest if not any(a in d for d in lifted)]
    for stack in STACK_LAYOUTS:
        for cls in (Lifter, FlatLifter):
            variants(cls, rest, stack)
    return lifted


def build_all(orig: asmdiff.Original, lifted: list[dict[int, tuple[str, set[str]]]],
              work: Path, best: dict | None = None,
              reloc_bad: set[int] | None = None) -> tuple[dict[int, tuple[str, set[str]]], set[int]]:
    """Compile every attempt in scratch TUs under `work` and diff them. Returns the first
    matching definition of each address (variant order decides; byte-identical with every
    relocation naming the original's target, asmdiff strict mode) and the addresses that
    compiled in at least one attempt. With `best`, also keeps the closest attempt of each
    address there as (score, definition, declarations), the earliest on a tie. With `reloc_bad`,
    adds the addresses of attempts that were byte-identical but named a wrong target."""
    work.mkdir(parents=True, exist_ok=True)

    def build(job: tuple[int, int]) -> tuple[set[int], set[int], dict[int, float], set[int]]:
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
                # strict: one llvm-cxxfilt call per object; a byte match naming the wrong target is not a match
                res = [x for x in asmdiff.diff_object(obj, orig, strict=True) if x.address in defs]
                return ({x.address for x in res if x.matching}, set(defs), {x.address: x.score for x in res},
                        {x.address for x in res if x.reloc_mismatches})
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
        results = list(pool.map(build, jobs))
    chosen: dict[int, tuple[str, set[str]]] = {}
    for k in range(len(lifted)):  # variant order decides
        for (kk, _), (ok, _, scores, bad) in zip(jobs, results):
            if kk == k:
                for a in sorted(ok):
                    chosen.setdefault(a, lifted[k][a])
                if best is not None:
                    for a, sc in scores.items():
                        if a not in best or sc > best[a][0]:
                            best[a] = (sc, *lifted[k][a])
                if reloc_bad is not None:
                    reloc_bad.update(bad)
    return chosen, set().union(*[c for _, c, _, _ in results])


# Rewrites tried, in order, on functions that compile but don't match as lifted. Each is a
# separate round (all variants again), so a rewrite never costs a function that matched.
ROUND2 = [frozenset({"copy", "sort"}), frozenset({"copy", "alias"}), frozenset({"copy", "sort", "alias"})]


def lift_rounds(orig: asmdiff.Original, cands: list[int], work: Path,
                why: dict[int, tuple[int, str]] | None = None, reloc_bad: set[int] | None = None):
    """lift_all + build_all, then each ROUND2 rewrite on what compiled without matching.
    Returns (chosen, compiled, first-round attempts) as build_all and lift_all do; `reloc_bad`
    collects the byte-identical attempts build_all rejected for a wrong relocation target."""
    lifted = lift_all(orig, cands, why)
    chosen, compiled = build_all(orig, lifted, work, reloc_bad=reloc_bad)
    for k, opts in enumerate(ROUND2):
        rest = sorted(compiled - set(chosen))
        if not rest:
            break
        more, _ = build_all(orig, lift_all(orig, rest, opts=opts), work / f"round2_{k}", reloc_bad=reloc_bad)
        for a in sorted(more):
            chosen.setdefault(a, more[a])
    return chosen, compiled, lifted


BEST = ROOT / "build" / "lift_best"


def write_best(orig: asmdiff.Original, addrs: list[int]) -> None:
    """The closest lifted attempt of each address (first round and every ROUND2 rewrite), as a
    starting point for matching it by hand: build/lift_best/fn_<addr>.cpp, which
    tools/trymatch.py builds and diffs. Never commit these (they are machine lifts that don't
    match); a hand-matched version goes in src/ under its own name."""
    best: dict[int, tuple] = {}
    for k, opts in enumerate([frozenset()] + ROUND2):
        build_all(orig, lift_all(orig, addrs, opts=opts), ROOT / "build" / "gen_lift_best" / str(k), best)
    BEST.mkdir(parents=True, exist_ok=True)
    for a in addrs:
        if a not in best:
            print(f"{a:x}: no attempt compiled")
            continue
        sc, definition, decls = best[a]
        (BEST / f"fn_{a:x}.cpp").write_text("\n".join(
            [f"// Closest lifted attempt ({sc:.1%} of instructions matching); tools/gen_lift.py --best."]
            + HEADER + sorted(decls) + [definition, ""]), encoding="utf-8", newline="\n")
        print(f"{a:x}: {sc:6.1%}  {(BEST / f'fn_{a:x}.cpp').relative_to(ROOT).as_posix()}")


BANDS =[("<256", 0, 256), ("256-1K", 256, 1024), ("1-2K", 1024, 2048), (">2K", 2048, 1 << 62)]


def report(orig: asmdiff.Original, addrs: list[int], cands: set[int], examples: int) -> None:
    """Lift, compile and diff `addrs` in build/gen_lift_report/ (never src/) and print the
    outcome of each by size band: count/KB per row, rows by total bytes."""
    why: dict[int, tuple[int, str]] = {}
    reloc_bad: set[int] = set()
    todo = [a for a in addrs if a in cands]
    chosen, compiled, lifted = lift_rounds(orig, todo, ROOT / "build" / "gen_lift_report", why, reloc_bad)
    lifted_any = set().union(*lifted)
    outcome = {}
    for a in addrs:
        if a not in cands:
            outcome[a] = "rejected: size cap" if orig.sizes.get(a, 0) > MAX_SIZE else "rejected: not a candidate"
        elif a in chosen:
            outcome[a] = "matched"
        elif a in reloc_bad:
            outcome[a] = "lifted, wrong relocation target"
        elif a in compiled:
            outcome[a] = "lifted, not byte-identical"
        elif a in lifted_any:
            outcome[a] = "rejected: compile error"
        else:
            outcome[a] = f"rejected: {why[a][1]}"
    rows: dict[str, list[int]] = {}
    for a in sorted(addrs):
        rows.setdefault(outcome[a], []).append(a)

    def cells(group: list[int]) -> list[str]:
        out = []
        for _, lo, hi in BANDS + [("total", 0, 1 << 62)]:
            s = [orig.sizes.get(a, 0) for a in group if lo <= orig.sizes.get(a, 0) < hi]
            out.append(f"{len(s)}/{sum(s) / 1024:.0f}K" if s else "-")
        return out

    head = ["outcome"] + [b[0] for b in BANDS] + ["total"]
    first = ["matched", "lifted, wrong relocation target", "lifted, not byte-identical"]
    order = sorted(rows, key=lambda r: (first.index(r) if r in first else len(first),
                                        -sum(orig.sizes.get(a, 0) for a in rows[r]), r))
    table = [head] + [[r] + cells(rows[r]) for r in order] + [["all"] + cells(list(addrs))]
    w = [max(len(row[i]) for row in table) for i in range(len(head))]
    for row in table:
        print("  ".join(row[0].ljust(w[0]) if i == 0 else c.rjust(w[i]) for i, c in enumerate(row)))
    if examples:
        for r in order:
            print(f"{r}: {' '.join(f'{a:x}' for a in rows[r][:examples])}")


FACTS = ROOT / "build" / "port" / "abi_facts.pickle"  # tools/abi_facts.py's cache (depends on the binary only)


def load_facts(orig: asmdiff.Original, version: str):
    """Byte mode's pass-through: every callee's argument registers (tools/abi_facts.py), from the
    complete table (AbiFacts.complete: every function in address order), so the output depends
    on the binary only, not on what was asked first or on a forked worker's slice."""
    import abi_facts
    af = abi_facts.AbiFacts(orig)
    af.version = version
    af.complete(FACTS)
    Lifter.facts = af.facts
    return af


def parse_only(spec: str) -> list[int]:
    text = Path(spec[1:]).read_text(encoding="utf-8") if spec.startswith("@") else spec.replace(",", "\n")
    return sorted({int(t, 16) for t in text.split() if t.strip()})


NOTE = ["// Generated by tools/gen_lift.py from main; regenerate rather than edit.",
        "// Small functions lifted from their instructions, byte-identical to the original.",
        "// Each function follows its own declarations, so dropping one leaves the rest unchanged."]


def write_out(blocks: dict[int, str]) -> None:
    """src/lifted/: one file per address range; each block is a function's declarations then
    its definition."""
    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    files: dict[int, list[int]] = {}
    for a in sorted(blocks):
        files.setdefault((a - BASE) >> RANGE_BITS, []).append(a)
    for k, addrs in files.items():
        parts = ["\n".join(ln for ln in blocks[a].split("\n") if ln != LIFT_AT) for a in addrs]
        head = HEADER + ([LIFT_AT, ""] if any("lift_at_t{}" in p for p in parts) else [])
        (OUT / f"lifted_{k:03x}.cpp").write_text(
            "\n".join(NOTE + head + [p + "\n" for p in parts]), encoding="utf-8", newline="\n")


def read_out() -> dict[int, str]:
    """The blocks write_out wrote, by address."""
    blocks = {}
    for f in sorted(OUT.glob("*.cpp")):
        for block in f.read_text(encoding="utf-8").split("\n\n"):
            for line in block.splitlines():
                m = NAME_RE.match(line)
                if m and line.endswith("{"):
                    blocks[int(m.group(1), 16)] = block.strip("\n")
                    break
    return blocks


def prune() -> int:
    """Drop the lifted copies of functions that another source now defines (after a merge, or a
    new clone or hand-written function). Gives what a full run would, minus anything a full run
    would newly lift."""
    blocks = read_out()
    drop = defined_elsewhere() & set(blocks)
    write_out({a: b for a, b in blocks.items() if a not in drop})
    print(f"dropped {len(drop)} of {len(blocks)} lifted functions: " + " ".join(f"{a:x}" for a in sorted(drop)))
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="required except with --prune")
    ap.add_argument("--only", help="ADDR[,ADDR...] or @file (one address per line): measure just these")
    ap.add_argument("--sample", type=int, help="measure a random sample of N candidates")
    ap.add_argument("--seed", type=int, default=0, help="seed for --sample (default 0)")
    ap.add_argument("--report", action="store_true",
                    help="print outcomes by size band instead of writing src/lifted/ (implied by --only/--sample)")
    ap.add_argument("--new", action="store_true", help="with --report: skip functions src/lifted/ already defines")
    ap.add_argument("--examples", type=int, default=0, help="with --report: up to K addresses per outcome")
    ap.add_argument("--best", help="ADDR[,ADDR...] or @file: write the closest lifted attempt of each to "
                                   "build/lift_best/ (a starting point for hand matching)")
    ap.add_argument("--add", help="ADDR[,ADDR...] or @file: lift just these candidates into src/lifted/, keeping "
                                  "the rest (what a full run gives them, when nothing else changed)")
    ap.add_argument("--prune", action="store_true",
                    help="only drop functions now defined elsewhere in src/ from src/lifted/ (seconds; no lifting)")
    ap.add_argument("--plain", action="store_true",
                    help="lift without pass-through (calls name only the registers written before them)")
    args = ap.parse_args()
    if args.prune:
        return prune()
    if not args.version:
        ap.error("--version is required")
    orig = asmdiff.Original(args.version)
    if not args.plain:
        load_facts(orig, args.version)
    if args.best:
        write_best(orig, parse_only(args.best))
        return 0
    cands = candidates(orig)
    if args.add:
        todo = sorted(set(parse_only(args.add)) & set(cands))
        chosen, _, _ = lift_rounds(orig, todo, ROOT / "build" / "gen_lift_add")
        blocks = {a: b for a, b in read_out().items() if a not in todo}
        blocks.update({a: "\n".join(sorted(chosen[a][1]) + [chosen[a][0]]) for a in chosen})
        write_out(blocks)
        print(f"{len(todo)} candidates; {len(chosen)} byte-identical added: "
              + " ".join(f"{a:x}" for a in sorted(chosen)))
        return 0

    if args.report or args.only or args.sample:
        addrs = parse_only(args.only) if args.only else list(cands)
        if args.new:
            done = {int(m.group(1), 16) for f in OUT.glob("*.cpp")
                    for m in map(NAME_RE.match, f.read_text(encoding="utf-8").splitlines()) if m}
            addrs = [a for a in addrs if a not in done]
        if args.sample:
            pool = [a for a in addrs if a in set(cands)] if args.only else addrs
            addrs = sorted(random.Random(args.seed).sample(pool, min(args.sample, len(pool))))
        report(orig, addrs, set(cands), args.examples)
        return 0

    reloc_bad: set[int] = set()
    chosen, _, lifted = lift_rounds(orig, cands, ROOT / "build" / "gen_lift", reloc_bad=reloc_bad)
    write_out({a: "\n".join(sorted(chosen[a][1]) + [chosen[a][0]]) for a in chosen})
    ok = sorted(chosen)
    lifted_any = set().union(*lifted)
    reloc_bad -= set(chosen)
    print(f"{len(cands)} candidates, {len(lifted_any)} lifted; {len(ok)} byte-identical "
          f"({sum(orig.sizes[a] for a in ok)} bytes)"
          + (f"; {len(reloc_bad)} byte-identical but rejected for a wrong relocation target" if reloc_bad else ""))
    return 0


if __name__ == "__main__":
    sys.exit(main())
