"""Mercury Engine reflection: map reflected type names to their class-getter
functions, parent getters, class-descriptor init helpers, and object sizes.

Each reflected type T has a getter: a guarded static initialiser that builds the
name string "T", calls the parent type's getter, and passes both to a per-type
helper. The helper constructs a base::reflection::C{Class,EnumType,...}
descriptor. For CClass, the descriptor's +0x18 field holds sizeof(T).

Inputs are the Ghidra export (functions, strings with referencing functions)
and the MEDS type database.
"""

from __future__ import annotations

import json
from dataclasses import dataclass
from pathlib import Path

import capstone

from image import BASE, Image

DESCRIPTOR_KINDS = {
    "base::reflection::CClass": "class",
    "base::reflection::CEnumType": "enum",
    "base::reflection::CCollectionType": "collection",
    "base::reflection::CPointerType": "pointer",
    "base::reflection::CFlagsetType": "flagset",
}
GUARD_ACQUIRE = "__cxa_guard_acquire"


@dataclass
class ReflectedType:
    name: str
    getter: int
    kind: str | None = None
    init_helper: int | None = None
    parent_getter: int | None = None
    parent: str | None = None
    size: int | None = None


def load_export(export_dir: Path, module: str = "main"):
    funcs = {int(f["entry"], 16): f for f in map(json.loads, open(export_dir / f"{module}_functions.jsonl"))}
    strings = [json.loads(line) for line in open(export_dir / f"{module}_strings.jsonl", encoding="utf-8")]
    return funcs, strings


_CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
_CS.detail = True
# Caller-saved registers are clobbered by calls.
_CALLER_SAVED = {f"{i}" for i in range(19)}


def _regnum(reg: int) -> str:
    """'x8' and 'w8' both map to '8', so 32-bit moves and 64-bit stores line up."""
    return _CS.reg_name(reg).lstrip("xw")


def class_size(img: Image, helper: int, size: int) -> int | None:
    """Find the constant stored to [this, #0x18] in a CClass init helper (this = x0 on entry)."""
    consts: dict[str, int] = {}
    this = {"0"}
    off = helper - BASE
    for ins in _CS.disasm(img.data[off:off + size], helper):
        ops = ins.operands
        m = ins.mnemonic
        if m in ("mov", "movz", "orr") and len(ops) == 2:
            dst = _regnum(ops[0].reg)
            if ops[1].type == capstone.arm64.ARM64_OP_IMM:
                consts[dst] = ops[1].imm & 0xFFFFFFFFFFFFFFFF
            elif ops[1].type == capstone.arm64.ARM64_OP_REG:
                src = _regnum(ops[1].reg)
                if src in this:
                    this.add(dst)
                consts.pop(dst, None)
        elif m == "movk" and len(ops) == 2 and _regnum(ops[0].reg) in consts:
            shift = ops[1].shift.value if ops[1].shift.type else 0
            consts[_regnum(ops[0].reg)] |= (ops[1].imm & 0xFFFF) << shift
        elif m in ("str", "stp", "stur") and ops[-1].type == capstone.arm64.ARM64_OP_MEM \
                and _regnum(ops[-1].mem.base) in this:
            disp = ops[-1].mem.disp
            regs = [_regnum(o.reg) for o in ops[:-1]]
            width = 8 if _CS.reg_name(ops[0].reg).startswith("x") else 4
            for i, r in enumerate(regs):
                if disp + i * width == 0x18:
                    return consts.get(r)
        elif m in ("bl", "blr"):
            for r in _CALLER_SAVED:
                consts.pop(r, None)
            this &= {r for r in this if r not in _CALLER_SAVED}
        elif m == "ret":
            break
    return None


def recover(img: Image, export_dir: Path, meds_types: dict) -> dict[str, ReflectedType]:
    funcs, strings = load_export(export_dir)
    by_value: dict[str, list[int]] = {}
    for s in strings:
        by_value.setdefault(s["value"], []).extend(int(f, 16) for f in s["funcs"])
    guard = {a for a, f in funcs.items() if f["name"].endswith(GUARD_ACQUIRE)}
    helper_kind = {}
    for desc, kind in DESCRIPTOR_KINDS.items():
        for a in by_value.get(desc, []):
            helper_kind[a] = kind

    types: dict[str, ReflectedType] = {}
    for name in meds_types:
        for a in by_value.get(name, []):
            calls = {int(c, 16) for c in funcs[a]["calls"]}
            if calls & guard:
                types[name] = ReflectedType(name, a)
                break
    getter_to_type = {t.getter: t.name for t in types.values()}

    for t in types.values():
        calls = [int(c, 16) for c in funcs[t.getter]["calls"]]
        for c in calls:
            if c in helper_kind:
                t.kind, t.init_helper = helper_kind[c], c
            elif c in getter_to_type and c != t.getter:
                t.parent_getter, t.parent = c, getter_to_type[c]
        if t.kind == "class" and t.init_helper is not None:
            t.size = class_size(img, t.init_helper, funcs[t.init_helper]["bytes"])
    return types
