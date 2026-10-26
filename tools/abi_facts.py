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
            (blr/br) reads its target register, x0 and the argument registers written earlier in
            the same block (the lifter's own rule for indirect calls); every call clobbers x0-x18
            and v0-v7/v16-v31.
  stack     whether it reads its caller's frame (a load from [sp, #k] at or above the entry sp):
            arguments beyond the eight registers.
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
import pickle
import sys
from dataclasses import dataclass
from pathlib import Path

import capstone
from capstone import arm64

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import callgraph  # noqa: E402

MAX_SIZE = 65536
CACHE_VERSION = 6  # bump when the analysis changes (AbiFacts.load ignores other versions)
MAX_DEPTH = 48  # callee recursion; deeper chains count as unknown
INT_ARGS = frozenset(range(9))  # x0-x7 and x8
FLOAT_ARGS = frozenset(range(100, 108))
ARGS = INT_ARGS | FLOAT_ARGS
CLOBBER = frozenset(range(19)) | frozenset(range(100, 108)) | frozenset(range(116, 132)) | {"nzcv"}
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


@dataclass(frozen=True)
class Facts:
    args: frozenset   # register keys (see the module docstring)
    stack: bool
    wide: frozenset = frozenset()  # integer argument registers read as x<n> (else only as w<n>)

    @property
    def ints(self) -> int:
        """Integer argument registers x0..x(n-1) a caller must set: up to the highest one read."""
        return max([k + 1 for k in self.args if k < 8], default=0)

    @property
    def floats(self) -> int:
        return max([k - 99 for k in self.args if 100 <= k < 108], default=0)

    @property
    def sret(self) -> bool:
        return 8 in self.args


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

    @staticmethod
    def of(x) -> "Insn":
        r, w = x.regs_access()
        regs = tuple(reg_key(op.reg) for op in x.operands if op.type == arm64.ARM64_OP_REG)
        mem = [op for op in x.operands if op.type == arm64.ARM64_OP_MEM]
        return Insn(x.address, x.mnemonic, tuple(op.imm for op in x.operands if op.type == arm64.ARM64_OP_IMM),
                    frozenset(k for k in map(reg_key, r) if k is not None),
                    frozenset(k for k in map(reg_key, w) if k is not None),
                    regs[0] if regs else None,
                    (reg_key(mem[0].mem.base), mem[0].mem.disp, bool(x.writeback)) if mem else None, regs,
                    frozenset(reg_key(q) for q in r if CS.reg_name(q)[0] == "x" and isinstance(reg_key(q), int)))


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
        self._wide = False  # liveness over 64-bit reads only (Facts.wide)

    def load(self, path: Path) -> None:
        """Facts computed by an earlier run (they depend on the binary only)."""
        if path.is_file():
            with path.open("rb") as fh:
                saved = pickle.load(fh)
            if saved.get("version") == CACHE_VERSION:
                self._facts.update(saved["facts"])
                self._ret.update(saved["ret"])

    def save(self, path: Path) -> None:
        path.parent.mkdir(parents=True, exist_ok=True)
        with path.open("wb") as fh:
            pickle.dump({"version": CACHE_VERSION, "facts": self._facts, "ret": self._ret}, fh)

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
            f = Facts(args, self.reads_stack(ins), frozenset(k for k in args if k < 9 and k in live_wide[0]))
        finally:
            self._wide = outer
            self._busy.discard(addr)
        self._facts[addr] = f
        return f

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
        # Blocks for the indirect-call rule: a block starts at a branch target or after a branch.
        starts = {addr}
        for x in ins:
            if x.mnemonic in BRANCHES or x.mnemonic.startswith("b."):
                starts |= {t for t in x.imms if addr <= t < end}
            if x.mnemonic in BRANCHES or x.mnemonic.startswith("b.") or x.mnemonic in ("bl", "blr", "br", "ret"):
                starts.add(x.address + 4)
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
                u = self.call_uses(None, block, len(block) - 1, depth, reg)
                d = set(CLOBBER) | {30}
            elif m == "br":
                reg = x.reg0
                u, d, fall = self.call_uses(None, block, len(block) - 1, depth, reg) | set(ret_uses), set(), []
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

    @staticmethod
    def reads_stack(ins: list) -> bool:
        """A load from the caller's frame: [sp, #k] (or [x29, #k]) at or above the entry sp, with
        sp and fp tracked through the prologue (gen_lift.is_frame's instructions)."""
        delta, fp = 0, None
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
                    return True
        return False

    # -- result use -----------------------------------------------------------------------------

    def ret_used(self, addr: int, depth: int = 0) -> bool | None:
        """Whether a caller reads the result (module docstring); None without known callers."""
        if addr in self._ret:
            return self._ret[addr]
        if self.graph is None:
            self.graph = callgraph.CallGraph(self.version)
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
              f"{', stack arguments' if f.stack else ''}; result used: {af.ret_used(a)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
