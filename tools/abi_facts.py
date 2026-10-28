#!/usr/bin/env python3
"""Calling-convention facts of main's functions, read from their instructions.

The byte-identical lifter (tools/gen_lift.py) does not need to know how many arguments a callee
takes: a register the caller leaves set reaches the callee in the original and in the lift alike,
since both are AArch64. A semantic lift compiled for x86-64 (tools/port_lift.py) passes only what
its C call names, so it needs the callee's real argument registers; its oracle needs them too, to
know which registers of a call to compare. This module computes, per function:

  args      argument registers the function may read before writing them, on some path from its
            entry: x0-x7 (keys 0-7), x8 (8, the indirect result register) and v0-v7 (100-107).
            A direct call reads its callee's `args` (recursively, memoized); an indirect call
            (blr/br) reads its target register, x0, the argument registers written earlier in
            the same block (the lifter's own rule for indirect calls) and `fwd`; every call
            clobbers x0-x18 and v0-v7/v16-v31.
  fwd       argument registers the function's direct callers set for it (written in the block
            before the call and not read again before it; vector copies through q/v registers
            do not count): what an indirect call, whose callee nothing names, may be passed on.
            A wrapper `this->vf(0.0f)` reached as fn(this, 0.0f) reads s0 that way. Only for
            functions with an indirect call (empty otherwise).
  stack     whether it reads its caller's frame (a load from [sp, #k] at or above the entry sp):
            arguments beyond the eight registers; stack_slots counts the 8-byte slots up to the
            highest one read.
  ret_used  whether some caller reads x0 (or s0/d0) after calling it, before writing it; a tail
            call passes the question on to the caller's callers. None when no direct caller is
            known (a virtual function, a function pointer): the oracle then compares the result.

Facts are None for functions that cannot be decoded or are larger than MAX_SIZE, and for imports
(their thunks are a `br x17`); C library imports with a known signature have fixed facts (CLIB).
Standard library plus capstone; the Ghidra export (tools/callgraph.py) gives the callers.

Usage: python tools/abi_facts.py --version 2.1.0 ADDR [ADDR...]   (prints the facts of each)
"""

from __future__ import annotations

import argparse
import os
import pickle
import sys
import time
from dataclasses import dataclass
from pathlib import Path

import capstone
from capstone import arm64

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import callgraph  # noqa: E402

MAX_SIZE = 65536
# result_shape's and result_main's analysis (their cache is kept apart: a change does not cost the table)
SHAPE_VERSION = 3
CACHE_VERSION = 10  # bump when the analysis changes (AbiFacts.load ignores other versions)
MAX_DEPTH = 48  # callee recursion; deeper chains count as unknown
INT_ARGS = frozenset(range(9))  # x0-x7 and x8
FLOAT_ARGS = frozenset(range(100, 108))
ARGS = INT_ARGS | FLOAT_ARGS
CLOBBER = frozenset(range(19)) | frozenset(range(100, 108)) | frozenset(range(116, 132)) | {"nzcv"}
RESULTS = frozenset({0, 1, 100, 101, 102, 103})  # what a call may return in: x0:x1, an HFA in s0-s3
# C library imports by name: (integer arguments, float arguments, returns a value).
CLIB = {"memcpy": (3, 0), "memmove": (3, 0), "memset": (3, 0), "memcmp": (3, 0), "strlen": (1, 0),
        "strcmp": (2, 0), "strncmp": (3, 0), "strcpy": (2, 0), "strncpy": (3, 0), "strchr": (2, 0),
        "strrchr": (2, 0), "strstr": (2, 0), "strcat": (2, 0), "free": (1, 0), "malloc": (1, 0),
        "calloc": (2, 0), "realloc": (2, 0), "abort": (0, 0), "sinf": (0, 1), "cosf": (0, 1),
        "tanf": (0, 1), "atan2f": (0, 2), "sqrtf": (0, 1), "powf": (0, 2), "expf": (0, 1),
        "logf": (0, 1), "fmodf": (0, 2), "floorf": (0, 1), "ceilf": (0, 1), "acosf": (0, 1),
        "asinf": (0, 1), "atanf": (0, 1), "vsnprintf": (4, 0), "snprintf": (3, 0), "sprintf": (2, 0),
        "__cxa_guard_acquire": (1, 0), "__cxa_guard_release": (1, 0), "__cxa_guard_abort": (1, 0),
        "__cxa_atexit": (3, 0), "__cxa_pure_virtual": (0, 0)}

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
CS.detail = True
BRANCHES = ("b", "cbz", "cbnz", "tbz", "tbnz")
# Corrections to capstone's register access (Insn.of).
FLAGS_ONLY = frozenset({"cmp", "cmn", "tst"})
# Vector-register forms that move one float (Clang 9 copies an FPR as `mov v0.16b, v8.16b`): not
# a vector copy (Insn.vec), so they count as a caller setting a float argument.
SCALAR_COPIES = frozenset({"mov", "fmov", "dup", "umov", "ins"})
NO_DEST_READ = frozenset({"lsl", "lsr", "asr", "ror", "ubfx", "sbfx", "ubfiz", "sbfiz", "sxtb", "sxth", "sxtw",
                          "uxtb", "uxth", "fmov", "tbl"})


@dataclass(frozen=True)
class Facts:
    args: frozenset   # register keys (see the module docstring)
    stack: bool
    wide: frozenset = frozenset()  # integer argument registers read as x<n> (else only as w<n>)
    fwd: frozenset = frozenset()   # argument registers the callers set, for an indirect call
    stack_slots: int = 0           # 8-byte slots of the caller's frame read (stack arguments)

    @property
    def ints(self) -> int:
        """Integer argument registers x0..x(n-1) a caller must set: up to the highest one read."""
        return max([k + 1 for k in self.args if k < 8], default=0)

    @property
    def floats(self) -> int:
        return max([k - 99 for k in self.args if 100 <= k < 108], default=0)

    @property
    def fwd_counts(self) -> tuple[int, int]:
        """(integer, float) argument registers an indirect call of this function is passed, by
        the rule above: x0 always, then up to the highest of `fwd`."""
        return (max([k + 1 for k in self.fwd if k < 8], default=1),
                max([k - 99 for k in self.fwd if 100 <= k < 108], default=0))

    @property
    def sret(self) -> bool:
        return 8 in self.args


def replace(tmp: Path, path: Path) -> bool:
    """os.replace for a cache file other processes may be reading: on Windows a reader holding it
    open makes the replace fail for a moment, so retry, and give up on the save (never the run)."""
    for _ in range(10):
        try:
            os.replace(tmp, path)
            return True
        except PermissionError:
            time.sleep(0.5)
    tmp.unlink(missing_ok=True)
    print(f"{path} is in use; not saved", file=sys.stderr)
    return False


_KEYS: dict[int, object] = {}


def reg_key(cs_reg: int):
    """Register key: n for x<n>/w<n>, 100+n for v/q/d/s/h/b<n>, 'sp', 'nzcv', else None."""
    if cs_reg not in _KEYS:
        _KEYS[cs_reg] = _reg_key(cs_reg)
    return _KEYS[cs_reg]


def _reg_key(cs_reg: int):
    name = CS.reg_name(cs_reg)
    name = {"fp": "x29", "lr": "x30"}.get(name, name)
    if name in ("sp", "wsp"):
        return "sp"
    if name == "nzcv":
        return "nzcv"
    if name in ("xzr", "wzr"):
        return None
    if name[0] in "xw" and name[1:].isdigit():
        return int(name[1:])
    if name[0] in "vqdshb" and name[1:].isdigit():
        return 100 + int(name[1:])
    return None


@dataclass(frozen=True)
class Insn:
    """What liveness needs of one instruction."""
    address: int
    mnemonic: str
    imms: tuple
    uses: frozenset
    defs: frozenset
    reg0: object         # key of the first register operand (the br/blr target)
    mem: tuple | None    # (base key, displacement, writeback) of the memory operand
    reg_ops: tuple       # keys of the register operands, in order
    xreads: frozenset    # integer registers read in their 64-bit form
    vec: bool            # a q register, or a v register outside SCALAR_COPIES: a vector copy, not a float argument

    @staticmethod
    def of(x) -> "Insn":
        r, w = x.regs_access()
        ops = [op for op in x.operands if op.type == arm64.ARM64_OP_REG]
        # capstone 5.0.7's access lists are wrong for some aliases (a census over main): a compare
        # reads its first register and writes only NZCV, where capstone writes the register and
        # does not read it; a shift, extend or bitfield move by immediate, and fmov of an
        # immediate, do not read their destination. (movk, bfi/bfxil and lane inserts do.)
        if x.mnemonic in FLAGS_ONLY:
            r = list(r) + [op.reg for op in ops]
            w = [q for q in w if CS.reg_name(q) == "nzcv"]
        elif x.mnemonic in NO_DEST_READ and ops and "[" not in x.op_str.split(",", 1)[0]:  # (not a lane write)
            sources = {op.reg for op in ops[1:]}
            r = [q for q in r if q != ops[0].reg or q in sources]
        regs = tuple(reg_key(op.reg) for op in ops)
        mem = [op for op in x.operands if op.type == arm64.ARM64_OP_MEM]
        # (immediates with their lsl shift: `sub sp, sp, #1, lsl #12` moves sp by 4 KiB)
        return Insn(x.address, x.mnemonic, tuple(op.imm << (op.shift.value if op.shift.type == arm64.ARM64_SFT_LSL else 0)
                                                 for op in x.operands if op.type == arm64.ARM64_OP_IMM),
                    frozenset(k for k in map(reg_key, r) if k is not None),
                    frozenset(k for k in map(reg_key, w) if k is not None),
                    regs[0] if regs else None,
                    (reg_key(mem[0].mem.base), mem[0].mem.disp, bool(x.writeback)) if mem else None, regs,
                    frozenset(reg_key(q) for q in r if CS.reg_name(q)[0] == "x" and isinstance(reg_key(q), int)),
                    any(CS.reg_name(op.reg)[0] == "q" for op in ops) or (
                        x.mnemonic not in SCALAR_COPIES and any(CS.reg_name(op.reg)[0] == "v" for op in ops)))


class AbiFacts:
    def __init__(self, orig: asmdiff.Original, rows: dict[int, dict] | None = None, graph=None):
        self.orig = orig
        self.rows = rows if rows is not None else callgraph.load_rows()
        self.graph = graph  # a callgraph.CallGraph, for ret_used (loaded on first use)
        self.version = None
        self._facts: dict[int, Facts | None] = {}
        self._busy: set[int] = set()
        self._decoded: dict[int, list | None] = {}
        self._ret: dict[int, bool | None] = {}
        self._shape: dict[int, tuple | None] = {}  # result_shape
        self._main: dict[int, frozenset] = {}  # result_main
        self._defines: dict[tuple[int, int], bool | None] = {}  # defines
        self._busy_def: set[tuple[int, int]] = set()
        self._fwd: dict[int, frozenset] = {}
        self._wide = False  # liveness over 64-bit reads only (Facts.wide)

    def load(self, path: Path) -> None:
        """The complete table an earlier run computed (complete()); a partial one is ignored."""
        if path.is_file():
            with path.open("rb") as fh:
                saved = pickle.load(fh)
            if saved.get("version") == CACHE_VERSION and saved.get("complete"):
                self._facts.update(saved["facts"])
                self._ret.update(saved["ret"])
                if saved.get("shape_version") == SHAPE_VERSION:
                    self._shape.update(saved.get("shape", {}))
                    self._main.update(saved.get("main", {}))
                self.is_complete = True

    def complete(self, path: Path) -> None:
        """Facts of every function, computed in address order from nothing, unless `path` holds
        that table already. A call cycle or a chain deeper than MAX_DEPTH leaves the function
        that closes it without facts at that point (facts()), so lazily computed facts would
        depend on the order they are asked for; this one order makes them a function of the
        binary, and tools/gen_lift.py's output with them (about two minutes, then cached)."""
        self.load(path)
        if getattr(self, "is_complete", False):
            return
        self._facts.clear()
        self._fwd.clear()
        for a in sorted(self.orig.sizes):
            self.facts(a)
        self._decoded.clear()  # (every function's instructions: gigabytes, decoded again on demand)
        self.is_complete = True
        self.save(path)

    def save(self, path: Path) -> None:
        """Write the table (only a complete one is ever loaded again). Several tools share the
        file: it is replaced in one step, and a reader that holds it open on Windows only costs
        the save, never the run."""
        path.parent.mkdir(parents=True, exist_ok=True)
        tmp = path.with_name(f"{path.name}.{os.getpid()}.tmp")
        with tmp.open("wb") as fh:
            pickle.dump({"version": CACHE_VERSION, "complete": getattr(self, "is_complete", False),
                         "facts": self._facts, "ret": self._ret, "shape": self._shape, "main": self._main,
                         "shape_version": SHAPE_VERSION}, fh)
        replace(tmp, path)

    # -- decoding -------------------------------------------------------------------------------

    def decode(self, addr: int) -> list | None:
        if addr in self._decoded:
            return self._decoded[addr]
        size = self.orig.sizes.get(addr)
        out = None
        if size is not None and size <= MAX_SIZE:
            code = self.orig.function(addr)
            ins = list(CS.disasm(code, addr))
            if len(ins) * 4 == len(code):
                out = [Insn.of(x) for x in ins]  # capstone's details are slow to query: once
        self._decoded[addr] = out
        return out

    def import_name(self, addr: int) -> str | None:
        row = self.rows.get(addr)
        if row and row["name"].startswith(callgraph.IMPORT_PREFIX):
            return row["name"][len(callgraph.IMPORT_PREFIX):]
        return self.orig.plt_import(addr)

    # -- facts ----------------------------------------------------------------------------------

    def facts(self, addr: int, depth: int = 0) -> Facts | None:
        if addr in self._facts:
            return self._facts[addr]
        name = self.import_name(addr)
        if name is not None:
            c = CLIB.get(name)
            f = Facts(frozenset(range(c[0])) | frozenset(range(100, 100 + c[1])), False,
                      frozenset(range(c[0]))) if c else None
            self._facts[addr] = f
            return f
        if addr in self._busy or depth > MAX_DEPTH:
            return None  # a cycle or a deep chain: unknown here, not memoized
        ins = self.decode(addr)
        if ins is None:
            self._facts[addr] = None
            return None
        self._busy.add(addr)
        outer = self._wide
        try:
            self._wide = False
            live_in, _ = self.liveness(addr, ins, depth)
            args = frozenset(k for k in live_in[0] if k in ARGS)
            # The same backward pass over 64-bit reads only (x<n>, a callee's wide arguments):
            # an argument the function reads whole somewhere before redefining it.
            self._wide = True
            live_wide, _ = self.liveness(addr, ins, depth)
            slots = self.stack_slots(ins)
            f = Facts(args, slots > 0, frozenset(k for k in args if k < 9 and k in live_wide[0]),
                      self.fwd_of(addr, ins), slots)
        finally:
            self._wide = outer
            self._busy.discard(addr)
        self._facts[addr] = f
        return f

    @staticmethod
    def block_starts(addr: int, ins: list) -> set[int]:
        """Block starts: the entry, branch targets, and the instruction after a branch or call."""
        end = addr + 4 * len(ins)
        starts = {addr}
        for x in ins:
            if x.mnemonic in BRANCHES or x.mnemonic.startswith("b."):
                starts |= {t for t in x.imms if addr <= t < end}
            if x.mnemonic in BRANCHES or x.mnemonic.startswith("b.") or x.mnemonic in ("bl", "blr", "br", "ret"):
                starts.add(x.address + 4)
        return starts

    @staticmethod
    def jump_table(ins: list, i: int) -> bool:
        """Whether the br at ins[i] dispatches a switch (Clang's `adr` shortly before), not a call."""
        return any(y.mnemonic == "adr" for y in ins[max(i - 10, 0):i])

    @staticmethod
    def cases(ins: list, i: int, starts: set[int], at: dict[int, int]) -> list[int]:
        """The successors of the switch br at ins[i]: every block after it (its cases and what
        follows them; more than the table holds, which only adds registers that may be read)."""
        return [at[t] for t in sorted(starts) if t > ins[i].address and t in at]

    def fwd_of(self, addr: int, ins: list) -> frozenset:
        """Facts.fwd: the callers' evidence, for a function with an indirect call."""
        if addr not in self._fwd:
            calls = any(x.mnemonic == "blr" or (x.mnemonic == "br" and not self.jump_table(ins, i))
                        for i, x in enumerate(ins))
            self._fwd[addr] = self.evidence(addr) if calls else frozenset()
        return self._fwd[addr]

    def evidence(self, addr: int) -> frozenset:
        """Argument registers (x0-x7, v0-v7) that addr's direct callers write in the block before
        calling it and do not read again before the call; a q/v register written by a vector
        copy does not count. Computed from the callers' instructions alone (no facts), so it does
        not depend on the order facts are asked for."""
        if self.graph is None:
            self.graph = callgraph.CallGraph(self.version, self.rows)
        ev: set = set()
        for c in sorted(set(self.graph.callers.get(addr, []))):
            ins = self.decode(c)
            if ins is None:
                continue
            starts = self.block_starts(c, ins)
            for i, x in enumerate(ins):
                if not x.imms or x.imms[-1] != addr or not (
                        x.mnemonic in ("bl", "b") or x.mnemonic in BRANCHES or x.mnemonic.startswith("b.")):
                    continue
                if x.address in starts and i and ins[i - 1].mnemonic in ("b", "ret", "br"):
                    continue  # a block nothing falls into: no write before the call in it
                later: set = set()
                for y in reversed(ins[:i]):
                    if y.mnemonic in ("bl", "blr"):
                        break
                    if not y.vec:
                        ev |= (y.defs & ARGS) - later - {8}
                    later |= y.uses
                    if y.address in starts:
                        break
        return frozenset(ev)

    def call_uses(self, target: int | None, block: list, k: int, depth: int, reg=None) -> set:
        """Registers a call at block[k] reads: the callee's facts, or the indirect-call rule."""
        if target is not None:
            f = self.facts(target, depth + 1)
            if f is not None:
                return set(f.wide if self._wide else f.args)
        uses = {0}
        if reg is not None:
            uses.add(reg)
        for x in reversed(block[:k]):
            if x.mnemonic in ("bl", "blr"):
                break
            uses |= x.defs & ARGS
        return uses

    def liveness(self, addr: int, ins: list, depth: int, ret_uses: frozenset = frozenset()):
        """Backward liveness over the function's instructions. Returns (live-in per instruction,
        index of each address). `ret_uses` are the registers a `ret` reads (its result, for
        ret_used's question; empty for argument facts)."""
        at = {x.address: i for i, x in enumerate(ins)}
        end = addr + 4 * len(ins)
        n = len(ins)
        uses: list[set] = [set() for _ in range(n)]
        defs: list[set] = [set() for _ in range(n)]
        succ: list[list[int]] = [[] for _ in range(n)]
        fwd = self.fwd_of(addr, ins)  # what an indirect call is passed on (Facts.fwd)
        # Blocks for the indirect-call rule: a block starts at a branch target or after a branch.
        starts = self.block_starts(addr, ins)
        block_of: list[tuple[int, int]] = []
        bstart = 0
        for i, x in enumerate(ins):
            if x.address in starts:
                bstart = i
            block_of.append((bstart, i))
        for i, x in enumerate(ins):
            m = x.mnemonic
            u, d = set(x.xreads if self._wide else x.uses), set(x.defs)
            fall = [i + 1] if i + 1 < n else []
            imm = x.imms
            block = ins[block_of[i][0]:i + 1]
            if m == "ret":
                u, d, fall = set(ret_uses) | {30}, set(), []
            elif m == "bl":
                u = self.call_uses(imm[0], block, len(block) - 1, depth)
                d = set(CLOBBER) | {30}
                if i == n - 1:
                    fall = []  # code ending in a call: the callee does not return
            elif m == "blr":
                reg = x.reg0
                u = self.call_uses(None, block, len(block) - 1, depth, reg) | fwd
                d = set(CLOBBER) | {30}
            elif m == "br" and self.jump_table(ins, i):
                u, d, fall = {x.reg0}, set(), self.cases(ins, i, starts, at)
            elif m == "br":
                reg = x.reg0
                u, d, fall = self.call_uses(None, block, len(block) - 1, depth, reg) | set(ret_uses) | fwd, set(), []
            elif m == "b":
                t = imm[0]
                if addr <= t < end:
                    fall = [at[t]] if t in at else []
                    u, d = set(), set()
                else:  # tail call: the callee's arguments, and its result is ours
                    u, d, fall = self.call_uses(t, block, len(block) - 1, depth) | set(ret_uses), set(), []
            elif m.startswith("b.") or m in ("cbz", "cbnz", "tbz", "tbnz"):
                t = imm[-1]
                if addr <= t < end:
                    if t in at:
                        fall = fall + [at[t]]
                else:  # a conditional tail call
                    u |= self.call_uses(t, block, len(block) - 1, depth) | set(ret_uses)
            if m == "movk":
                u |= d  # keeps the other halves
            uses[i], defs[i], succ[i] = u, d, fall
        live_in: list[set] = [set() for _ in range(n)]
        changed = True
        while changed:
            changed = False
            for i in range(n - 1, -1, -1):
                out = set()
                for s in succ[i]:
                    out |= live_in[s]
                new = uses[i] | (out - defs[i])
                if new != live_in[i]:
                    live_in[i] = new
                    changed = True
        return live_in, at

    def held(self, addr: int) -> dict[int, tuple[int | None, frozenset]]:
        """At each call leaving the function: (its direct target or None, the argument registers
        that may hold something there), by a forward pass: on some path from the entry they still
        hold the entry's value or the function wrote them since its last call (a call leaves its
        possible results, RESULTS). A callee reads garbage in the others, in the original too.
        A switch's br leads to every block after it."""
        ins = self.decode(addr)
        if ins is None:
            return {}
        at = {x.address: i for i, x in enumerate(ins)}
        end = addr + 4 * len(ins)
        regs = frozenset(range(8)) | FLOAT_ARGS
        n = len(ins)
        starts = self.block_starts(addr, ins)
        succ: list[list[int]] = []
        calls: dict[int, int | None] = {}
        for i, x in enumerate(ins):
            m, t = x.mnemonic, x.imms[-1] if x.imms else None
            fall = [i + 1] if i + 1 < n else []
            if m == "br" and self.jump_table(ins, i):
                fall = self.cases(ins, i, starts, at)
            elif m == "ret" or m == "br":
                if m == "br":
                    calls[i] = None
                fall = []
            elif m in ("bl", "blr"):
                calls[i] = t if m == "bl" else None
                if m == "bl" and i == n - 1:
                    fall = []
            elif m == "b":
                if addr <= t < end:
                    fall = [at[t]] if t in at else []
                else:
                    calls[i], fall = t, []
            elif (m.startswith("b.") or m in BRANCHES) and t is not None:
                if addr <= t < end:
                    fall = fall + ([at[t]] if t in at else [])
                else:
                    calls[i] = t
            succ.append(fall)
        live_in: list[frozenset | None] = [None] * n
        live_in[0] = regs
        work = [0]
        while work:
            i = work.pop()
            x, s = ins[i], live_in[i]
            if x.mnemonic in ("bl", "blr"):
                out = (s - CLOBBER) | RESULTS
            else:
                out = s | (x.defs & regs)
            for j in succ[i]:
                new = out if live_in[j] is None else live_in[j] | out
                if new != live_in[j]:
                    live_in[j] = new
                    work.append(j)
        return {ins[i].address: (t, live_in[i]) for i, t in calls.items() if live_in[i] is not None}

    @staticmethod
    def stack_slots(ins: list) -> int:
        """Loads from the caller's frame: [sp, #k] (or [x29, #k]) at or above the entry sp, with sp
        and fp tracked through the prologue (gen_lift.is_frame's instructions). Returns the 8-byte
        slots up to the highest one read (a pair load reads two), 0 for none."""
        delta, fp, top = 0, None, -1
        for x in ins:
            m, r, mem = x.mnemonic, x.reg_ops, x.mem
            if m == "sub" and r == ("sp", "sp") and x.imms:
                delta -= x.imms[0]
                continue
            if m == "add" and r == (29, "sp") and x.imms:
                fp = delta + x.imms[0]
                continue
            if m == "mov" and r == (29, "sp"):
                fp = delta
                continue
            if mem and m.startswith("st") and mem[2] and mem[0] == "sp":
                delta += mem[1]
                continue
            if mem and m.startswith("ld"):
                at = delta if mem[0] == "sp" else fp if mem[0] == 29 else None
                if at is not None and not mem[2] and at + mem[1] >= 0:
                    top = max(top, at + mem[1] + (8 if m.startswith("ldp") else 0))
        return top // 8 + 1 if top >= 0 else 0

    # -- result use -----------------------------------------------------------------------------

    def ret_used(self, addr: int, depth: int = 0) -> bool | None:
        """Whether a caller reads the result (module docstring); None without known callers."""
        if addr in self._ret:
            return self._ret[addr]
        if self.graph is None:
            self.graph = callgraph.CallGraph(self.version, self.rows)
        callers = self.graph.callers.get(addr, [])
        if not callers or depth > 8:
            return None
        self._ret[addr] = False  # breaks cycles; refined below
        used: bool | None = False
        for c in sorted(set(callers)):
            u = self.uses_result_of(c, addr, depth)
            if u is None:
                used = None if used is False else used
            elif u:
                used = True
                break
        self._ret[addr] = used
        return used

    def uses_result_of(self, caller: int, callee: int, depth: int) -> bool | None:
        ins = self.decode(caller)
        if ins is None:
            return None
        r = self.ret_used(caller, depth + 1)
        ret_uses = frozenset({0, 100}) if r is None or r else frozenset()
        live_in, _ = self.liveness(caller, ins, 0, ret_uses)
        n = len(ins)
        for i, x in enumerate(ins):
            imm = x.imms
            if not imm or imm[-1] != callee:
                continue
            if x.mnemonic == "bl":
                after = live_in[i + 1] if i + 1 < n else set()
                if after & {0, 100}:
                    return True
            elif x.mnemonic == "b" or x.mnemonic.startswith("b.") or x.mnemonic in ("cbz", "cbnz", "tbz", "tbnz"):
                if r is None or r:
                    return r
        return False

    def result_shape(self, addr: int, depth: int = 0) -> tuple | None:
        """A result in more than x0 or s0/d0, from what callers read right after calling: ("x", 2)
        for a 16-byte result in x0:x1, ("s" or "d", n) for a float aggregate in its first n float
        registers (a Vector3 in s0-s2); None for one register or none. A forward scan of each
        caller's straight-line code after `bl addr` for x1 or s1-s3 read before written (a
        caller-saved register read after a call is a result: compiled code never keeps a value
        there across one), and through a caller that returns it (`bl addr` then ret, or `b addr`),
        that caller's own shape."""
        if addr in self._shape:
            return self._shape[addr]
        if self.graph is None:
            self.graph = callgraph.CallGraph(self.version, self.rows)
        callers = self.graph.callers.get(addr, [])
        if not callers or depth > 8:
            return None
        self._shape[addr] = None  # breaks cycles
        regs: set[int] = set()
        letters: set[str] = set()
        targets: dict[int, set[int]] = {}
        for c in sorted(set(callers)):
            ins = self.decode(c)
            if ins is None:
                continue
            targets[c] = {x.imms[-1] for x in ins if x.imms and x.mnemonic not in ("bl", "blr")
                          and x.mnemonic.startswith(("b", "cb", "tb"))}
            for i, x in enumerate(ins):
                if not x.imms or x.imms[-1] != addr or x.mnemonic not in ("bl", "b"):
                    continue
                if x.mnemonic == "b":  # a tail call: the caller's result is this one's
                    own = self.result_shape(c, depth + 1)
                    regs |= set(shape_regs(own))
                    letters |= {own[0]} if own and own[0] != "x" else set()
                    continue
                written: set[int] = set()
                for y in ins[i + 1:]:
                    if y.address in targets[c]:
                        break  # (a join: another path may have written the register)
                    hit = (y.uses & RESULT_EXTRA) - written
                    if hit:
                        regs |= hit
                        if hit - {1}:
                            letters.add(float_letter(self.orig, y.address))
                    if y.mnemonic == "ret":
                        own = self.result_shape(c, depth + 1)
                        regs |= set(shape_regs(own)) - written
                        letters |= {own[0]} if own and own[0] != "x" else set()
                        break
                    if y.mnemonic in ("bl", "blr", "b", "br"):
                        break  # (another call or a jump: the straight line ends; it goes on past a test)
                    written |= y.defs
        if regs - {1} and self.defines(addr, 100) is False:
            regs -= RESULT_EXTRA - {1}  # (no float result: what callers read is something else)
        if regs & {1} and regs - {1}:
            shape = None  # (an integer and a float register: not one AAPCS64 aggregate)
        elif regs & {1}:
            shape = ("x", 2)
        elif regs:
            shape = ("d" if "d" in letters else "s", max(regs) - 100 + 1)
        else:
            shape = None
        self._shape[addr] = shape
        return shape


    def result_main(self, addr: int, depth: int = 0) -> frozenset:
        """Which of x0 (0) and s0/d0 (100) callers read after calling: the same forward scan as
        result_shape, counting x0 passed on to the next call (a function returning `this`) and a
        caller that returns the result. Empty when no caller says (then the last written wins)."""
        if addr in self._main:
            return self._main[addr]
        if self.graph is None:
            self.graph = callgraph.CallGraph(self.version, self.rows)
        callers = self.graph.callers.get(addr, [])
        if not callers or depth > 8:
            return frozenset()
        self._main[addr] = frozenset()  # breaks cycles
        regs: set[int] = set()
        for c in sorted(set(callers)):
            ins = self.decode(c)
            if ins is None:
                continue
            targets = {x.imms[-1] for x in ins if x.imms and x.mnemonic not in ("bl", "blr")
                       and x.mnemonic.startswith(("b", "cb", "tb"))}
            for i, x in enumerate(ins):
                if not x.imms or x.imms[-1] != addr or x.mnemonic not in ("bl", "b"):
                    continue
                if x.mnemonic == "b":
                    regs |= self.result_main(c, depth + 1)
                    continue
                written: set[int] = set()
                for y in ins[i + 1:]:
                    if y.address in targets:
                        break
                    if y.mnemonic in ("bl", "b") and y.imms:
                        f = self.facts(y.imms[-1])  # (the result passed on as the next call's argument)
                        regs |= (set(f.args) & {0, 100} if f is not None else set()) - written
                        if y.mnemonic == "b":
                            regs |= self.result_main(c, depth + 1) - written
                        break
                    regs |= (y.uses & {0, 100}) - written
                    if y.mnemonic == "ret":
                        regs |= self.result_main(c, depth + 1) - written
                        break
                    if y.mnemonic in ("blr", "br"):
                        break
                    written |= y.defs
        if 100 in regs and self.defines(addr, 100) is False:
            regs.discard(100)  # (a caller's read of s0 is its own argument, or another call's leftover)
        self._main[addr] = frozenset(regs)
        return self._main[addr]

    def defines(self, addr: int, reg: int, depth: int = 0) -> bool | None:
        """Whether the function can return a value in `reg` (100: s0/d0): True when, at some
        return, the register was last written by its own instructions or by a call to a callee
        that defines it; False when at every return it still holds its entry value (an argument
        passed through) or what a call to a callee that never defines it left (AAPCS64 keeps no
        float register across a call); None when unknown (an indirect call, a cycle, a callee
        that cannot be decoded). The callee-side check result_main and result_shape apply to
        their callers' evidence: a caller that reads s0 after a void setter is reading its own
        argument's register, not a result."""
        key = (addr, reg)
        if key in self._defines:
            return self._defines[key]
        name = self.import_name(addr)
        if name is not None:
            c = CLIB.get(name)
            v = None if c is None else (reg == 100 and c[1] > 0 and name.endswith("f"))
            self._defines[key] = v
            return v
        if key in self._busy_def or depth > MAX_DEPTH:
            return None
        ins = self.decode(addr)
        if ins is None:
            self._defines[key] = None
            return None
        self._busy_def.add(key)
        try:
            v = self._defines_scan(addr, ins, reg, depth)
        finally:
            self._busy_def.discard(key)
        self._defines[key] = v
        return v

    def _defines_scan(self, addr: int, ins: list, reg: int, depth: int) -> bool | None:
        # Forward reaching definitions of `reg`: "entry", "own", or ("call", target or None).
        at = {x.address: i for i, x in enumerate(ins)}
        end = addr + 4 * len(ins)
        starts = self.block_starts(addr, ins)
        n = len(ins)
        reach: list[set | None] = [None] * n
        reach[0] = {"entry"}
        work, ends = [0], []
        while work:
            i = work.pop()
            x, cur = ins[i], set(reach[i])
            m = x.mnemonic
            succ = [i + 1] if i + 1 < n else []
            if m == "ret":
                ends.append(cur)
                continue
            if m == "bl":
                cur = {("call", x.imms[0] if x.imms else None)}
                if i == n - 1:
                    succ = []
            elif m == "blr":
                cur = {("call", None)}
            elif m == "br":
                if self.jump_table(ins, i):
                    succ = self.cases(ins, i, starts, at)
                else:  # a tail call through a register
                    ends.append({("call", None)})
                    continue
            elif m == "b":
                t = x.imms[0] if x.imms else None
                if t is not None and addr <= t < end:
                    succ = [at[t]] if t in at else []
                else:  # a tail call: the callee's result is ours
                    ends.append({("call", t)})
                    continue
            elif m.startswith("b.") or m in ("cbz", "cbnz", "tbz", "tbnz"):
                t = x.imms[-1] if x.imms else None
                if t is not None and addr <= t < end:
                    succ = succ + ([at[t]] if t in at else [])
                else:  # a conditional tail call
                    ends.append({("call", t)})
            elif reg in x.defs:
                cur = {"own"}
            for j in succ:
                if reach[j] is None or not cur <= reach[j]:
                    reach[j] = (reach[j] or set()) | cur
                    work.append(j)
        unknown = False
        for d in set().union(*ends) if ends else set():
            if d == "own":
                return True
            if d == "entry":
                continue
            t = d[1]
            v = None if t is None else self.defines(t, reg, depth + 1)
            if v:
                return True
            if v is None:
                unknown = True
        return None if unknown else False


RESULT_EXTRA = frozenset({1, 101, 102, 103})  # result registers beyond x0 and s0/d0 (result_shape)


def shape_regs(shape: tuple | None) -> tuple:
    """The registers beyond x0 and s0/d0 a result_shape occupies."""
    if not shape:
        return ()
    return (1,) if shape[0] == "x" else tuple(100 + i for i in range(1, shape[1]))


def float_letter(orig: asmdiff.Original, address: int) -> str:
    """'d' when the instruction at `address` reads a double register, else 's'."""
    off = address - asmdiff.BASE
    x = next(CS.disasm(orig.img.data[off:off + 4], address), None)
    return "d" if x is not None and any(CS.reg_name(op.reg)[0] == "d" for op in x.operands
                                        if op.type == arm64.ARM64_OP_REG) else "s"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("addresses", nargs="+")
    args = ap.parse_args()
    af = AbiFacts(asmdiff.Original(args.version))
    af.version = args.version
    for a in (int(s, 16) for s in args.addresses):
        f = af.facts(a)
        if f is None:
            print(f"{a:x}: unknown")
            continue
        print(f"{a:x}: {f.ints} integer, {f.floats} float arguments{', x8 (indirect result)' if f.sret else ''}"
              f"{f', {f.stack_slots} stack argument slots' if f.stack else ''}; result used: {af.ret_used(a)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
