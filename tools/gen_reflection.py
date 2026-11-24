#!/usr/bin/env python3
"""Generate src/reflection/: one source per original translation unit of class getters.

Every reflected class T has a getter (T::refl_GetClass) and a descriptor constructor
(T::refl_InitClass, an instantiation of CClass's constructor template), both expressed in
include/base/reflection/CClass.h. This tool reads each getter in main (the parent getter it
calls, the type-specific attribute and member-field functions it passes in x3/x4, the constructor it calls)
and each constructor (sizeof(T) and the type-specific functions it stores), emits a traits
specialisation per type, and groups types back into their original translation units.

Helpers: the per-type functions a descriptor points to (CType+0x28..+0x68) and the two
functions the getter passes in (x3/x4). Those with a known uniform code shape (HELPER_SHAPES)
are defined; the rest are declared with their slot's type. The x3/x4 functions (class
attributes, member fields) are reconstructed by tools/refl_attrs.py and kept only when a scratch
build of them is byte-identical (build/reflection_attrs/).

Form: a class whose name is a C++ path is written as base/reflection/ReflectedClass.h describes:
its functions in a namespace of its name under role names (T::refl_Equals, T::refl_AddFields, ...;
ROLES), a description struct naming the ones its descriptor points to, and DREAD_REFLECTED_CLASS
for the getter and descriptor constructor. tools/gen_functions_csv.py gives data/functions/ the same
names (reflection_names), which is how their symbols map to addresses. base::reflection's own classes
(whose names the headers' classes have) keep the older form: fn_<addr> helpers, traits specialised for
an UnkRefl_fn_<addr> tag and DREAD_REFL_CLASS_GETTER (base/reflection/CClass.h).

TU grouping: within a TU, Clang lays out all guard variables first, then the descriptors
(0x138 bytes each), in definition order. A run of getter guards 8 bytes apart, whose
descriptors follow immediately and in order, is one TU. Getters are generated only for "clean" TUs:
every getter uses the common code shape and the TU holds nothing but getter statics. Other
TUs need extra statics that aren't known yet, and are reported; their files hold only the
helpers and attribute/field functions, which don't depend on the TU's statics.

Pointer types whose pointee is one of a TU's classes (tools/gen_types.py, local pairs) are
generated in that TU: their constructor uses the class descriptor's constant address in place
of the getter call's result. Each such file is compiled and only byte-identical pointer
getter/constructor pairs are kept.

Usage: python tools/gen_reflection.py --version 2.1.0
Generated files are outputs: regenerate, don't edit.
"""

from __future__ import annotations

import argparse
import collections
import csv
import json
import re
import shutil
import struct
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from dataclasses import dataclass
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import gen_types  # noqa: E402
import refl_attrs  # noqa: E402
import sigmatch  # noqa: E402
from image import BASE  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "reflection"
TYPES_HEADER = OUT / "ReflectedTypes.h"
DESCRIPTOR_SIZE = 0x138
REFERENCE_GETTER = 0x7100227448  # CActor::refl_GetClass, the common code shape
ROOT_GETTER = 0x7100023464       # base::core::CBaseObject::refl_GetClass: no parent
# CActivationPerceptionRuleSet::refl_GetClass: the parent's getter is in the same TU, so Clang
# uses the parent descriptor's (constant) address instead of the call's result.
LOCAL_PARENT_GETTER = 0x71002D0420


@dataclass
class Getter:
    type: str
    address: int
    guard: int | None = None
    descriptor: int | None = None
    parent: int | None = None
    fn_0x70: int | None = None             # x3: adds class attributes (CType+0x70)
    fn_0xe0: int | None = None             # x4: registers member fields (CClass+0xe0)
    shape_ok: bool = False
    root: bool = False                    # root class: the common shape with a null parent
    local_parent: bool = False            # the shape of a getter whose parent is in the same TU
    inline_name: bool = False             # the name's constructor is inlined (with or without a parent)
    init: int | None = None               # descriptor constructor (T::refl_InitClass)
    traits: dict[int, int] | None = None  # CClass offset -> function address it stores
    abstract: bool = False                # constructor sets flag bit 0 (orr #1 after the and #~3)


def shape(code: bytes) -> str:
    """Position-independent signature that also ignores `add xN, x19, #imm`: the descriptor's
    offset from its guard (x19), which depends on the TU's layout, not on the code shape. The
    same for `add xN, x2, #imm`: with the parent in the same TU, the descriptor's offset from
    the parent's."""
    import struct
    words = bytearray(code)
    for i in range(len(code) // 4):
        w = struct.unpack_from("<I", code, 4 * i)[0]
        if w & 0xFF800000 == 0x91000000 and (w >> 5) & 0x1F in (19, 2):  # ADD Xd, X19|X2, #imm
            struct.pack_into("<I", words, 4 * i, w & ~(0xFFF << 10) & 0xFFFFFFFF)
    return sigmatch.signature(bytes(words))


TRAIT_SLOTS = [0x58, 0x60, 0x68, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50]

# Slot -> (CClass.h type alias, declaration format). {name} is fn_<addr>.
SLOT_TYPES = {
    0x28: ("SlotFn", "void {name}();"),
    0x30: ("CopyConstructFn", "void {name}(void* dst, const void* src);"),
    0x38: ("CopyConstructFn", "void {name}(void* dst, const void* src);"),
    0x40: ("DestroyFn", "void {name}(void* obj);"),
    0x48: ("AssignFn", "void* {name}(void* dst, const void* src);"),
    0x50: ("AssignFn", "void* {name}(void* dst, const void* src);"),
    0x58: ("EqualsFn", "bool {name}(const void* a, const void* b);"),
    0x60: ("HashFn", "s32 {name}();"),
    0x68: ("TypeOfFn", "const base::reflection::CType& {name}(const void* obj);"),
}
OBJ = "base::reflection::UnkReflObject"
OBJ_B = "base::reflection::UnkReflObjectB"
# (slot, instructions) -> definition. Slots 0x70 and 0xe0 are the getter's x3/x4 functions
# (attributes and member fields). Instruction text is relocation-free for all of these.
HELPER_SHAPES = {
    (0x58, "cmp x0, x1; cset w0, eq; ret"):
        "bool {name}(const void* a, const void* b) {{ return a == b; }}",
    (0x58, "mov w0, wzr; ret"):
        "bool {name}(const void*, const void*) {{ return false; }}",
    (0x60, "mov w0, #-1; ret"):
        "s32 {name}() {{ return -1; }}",
    # Non-tail call: the virtual returns a pointer, the helper a reference.
    (0x68, "stp x29, x30, [sp, #-0x10]!; mov x29, sp; ldr x8, [x0]; ldr x8, [x8]; blr x8; "
           "ldp x29, x30, [sp], #0x10; ret"):
        f"const base::reflection::CType& {{name}}(const void* obj) {{{{ return *static_cast<const {OBJ}*>(obj)->vf_0x0(); }}}}",
    (0x68, "stp x29, x30, [sp, #-0x10]!; mov x29, sp; ldr x8, [x0]; ldr x8, [x8, #0x18]; blr x8; "
           "ldp x29, x30, [sp], #0x10; ret"):
        f"const base::reflection::CType& {{name}}(const void* obj) {{{{ return *static_cast<const {OBJ_B}*>(obj)->vf_0x18(); }}}}",
    (0x40, "ldr x8, [x0]; ldr x1, [x8, #8]; br x1"):
        f"void {{name}}(void* obj) {{{{ static_cast<{OBJ}*>(obj)->~UnkReflObject(); }}}}",
    (0x40, "ldr x8, [x0]; ldr x1, [x8]; br x1"):
        f"void {{name}}(void* obj) {{{{ static_cast<{OBJ_B}*>(obj)->~UnkReflObjectB(); }}}}",
    (0x30, "ret"): "void {name}(void*, const void*) {{}}",
    (0x38, "ret"): "void {name}(void*, const void*) {{}}",
    (0x48, "mov x0, xzr; ret"): "void* {name}(void*, const void*) {{ return nullptr; }}",
    (0x50, "mov x0, xzr; ret"): "void* {name}(void*, const void*) {{ return nullptr; }}",
    (0x70, "ret"): "void {name}(base::reflection::CClass*) {{}}",
    (0xe0, "ret"): "void {name}(base::reflection::CClass*) {{}}",
}


# The readable form (base/reflection/ReflectedClass.h): each class's functions live in a namespace with
# the class's name, under role names (T::refl_<role>, the names data/functions/ gives them through
# reflection_names below), and a description struct names the ones the descriptor points to.
ROLES = {0x58: ("Equals", "equals"), 0x60: ("Hash", "hash"), 0x68: ("TypeOf", "type_of"),
         0x28: ("Construct", "construct"), 0x30: ("CopyConstruct", "copy_construct"),
         0x38: ("MoveConstruct", "move_construct"), 0x40: ("Destroy", "destroy"),
         0x48: ("CopyAssign", "copy_assign"), 0x50: ("MoveAssign", "move_assign"),
         0x70: ("AddAttributes", None), 0xe0: ("AddFields", None)}
ROLE_DECLS = {
    0x28: "void {name}(void* obj);",
    0x30: "void {name}(void* dst, const void* src);",
    0x38: "void {name}(void* dst, const void* src);",
    0x40: "void {name}(void* obj);",
    0x48: "void* {name}(void* dst, const void* src);",
    0x50: "void* {name}(void* dst, const void* src);",
    0x58: "bool {name}(const void* a, const void* b);",
    0x60: "s32 {name}();",
    0x68: "const base::reflection::CType& {name}(const void* obj);",
    0x70: "void {name}(CClass* cls);",
    0xe0: "void {name}(CClass* cls);",
}
# HELPER_SHAPES in the readable form (the same code).
ROLE_SHAPES = {
    "bool {name}(const void* a, const void* b) {{ return a == b; }}":
        "bool {name}(const void* a, const void* b) {{ return a == b; }}",
    "bool {name}(const void*, const void*) {{ return false; }}":
        "bool {name}(const void*, const void*) {{ return false; }}",
    "s32 {name}() {{ return -1; }}": "s32 {name}() {{ return -1; }}",
    f"const base::reflection::CType& {{name}}(const void* obj) {{{{ return *static_cast<const {OBJ}*>(obj)->vf_0x0(); }}}}":
        "const base::reflection::CType& {name}(const void* obj) {{ return ClassOf(obj); }}",
    f"const base::reflection::CType& {{name}}(const void* obj) {{{{ return *static_cast<const {OBJ_B}*>(obj)->vf_0x18(); }}}}":
        "const base::reflection::CType& {name}(const void* obj) {{ return ClassOfDestructorFirst(obj); }}",
    f"void {{name}}(void* obj) {{{{ static_cast<{OBJ}*>(obj)->~UnkReflObject(); }}}}":
        "void {name}(void* obj) {{ Destroy(obj); }}",
    f"void {{name}}(void* obj) {{{{ static_cast<{OBJ_B}*>(obj)->~UnkReflObjectB(); }}}}":
        "void {name}(void* obj) {{ DestroyDestructorFirst(obj); }}",
    "void {name}(void*, const void*) {{}}": "void {name}(void*, const void*) {{}}",
    "void* {name}(void*, const void*) {{ return nullptr; }}": "void* {name}(void*, const void*) {{ return nullptr; }}",
    "void {name}(base::reflection::CClass*) {{}}": "void {name}(CClass*) {{}}",
}
READABLE_HEADER = [
    "// each class's functions are in a namespace of its name (base/reflection/ReflectedClass.h): its",
    "// getter (T::refl_GetClass) and descriptor constructor (T::refl_InitClass), the functions the",
    "// descriptor points to (those of a known uniform shape defined, the others declared) and the",
    "// attribute/member-field functions, defined when tools/refl_attrs.py reconstructs them byte-identically.",
]
READABLE_PRELUDE = [
    "using base::reflection::CClass;",
    "using base::reflection::ClassDescription;",
    "using base::reflection::ClassOf;",
    "using base::reflection::ClassOfDestructorFirst;",
    "using base::reflection::Destroy;",
    "using base::reflection::DestroyDestructorFirst;",
]
TYPE_PATH_RE = re.compile(r"^[A-Za-z_]\w*(::[A-Za-z_]\w*)*$")


def readable_type(name: str) -> bool:
    """A class whose functions can live in a namespace of its name: the name is a C++ path, and no class
    the reflection headers define has it (base::reflection's own classes keep the older form)."""
    return bool(TYPE_PATH_RE.match(name)) and not name.startswith("base::reflection::")


def role_name(slot: int) -> str:
    return f"refl_{ROLES[slot][0]}"


def role_helper(orig: asmdiff.Original, slot: int, addr: int, name: str | None = None) -> str | None:
    """A helper with a known shape, in the readable form under its role name (or `name`), or None."""
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    text = "; ".join(f"{x.mnemonic} {x.op_str}".strip() for x in cs.disasm(orig.function(addr), addr))
    shape_src = HELPER_SHAPES.get((slot, text))
    return ROLE_SHAPES[shape_src].format(name=name or role_name(slot)) if shape_src else None


def reflection_names(g: "Getter") -> dict[int, str]:
    """Address -> data/functions/ name of a readable class's getter helpers (T::refl_<role>): the
    attribute and member-field functions and the functions its descriptor constructor stores."""
    if not readable_type(g.type) or not g.init or g.traits is None or not g.fn_0x70 or not g.fn_0xe0:
        return {}
    out = {a: f"{g.type}::{role_name(slot)}" for slot, a in g.traits.items()}
    out[g.fn_0x70] = f"{g.type}::{role_name(0x70)}"
    out[g.fn_0xe0] = f"{g.type}::{role_name(0xe0)}"
    return out


# Declarations ReflectedFields.h already brings: the typed attribute keys and their enum types. (The other keys'
# address-named declarations stay until attribute_keys_by_name writes their calls by name.)
KEY_DECL_RE = re.compile(r"^(?:const base::reflection::UnkReflAttrKey& fn_|extern base::reflection::CType g_unk_)([0-9a-f]+)(?:\(\))?;$")
OBJECT_DECL_RE = re.compile(r"^extern base::reflection::CType g_unk_([0-9a-f]+);$")
GETTER_DECL_RE = re.compile(r"^const base::reflection::C(?:Type|Class)\* fn_([0-9a-f]+)\(\);$")
FIELD_MACRO_RE = re.compile(r"\bDREAD_REFLECTED_(?:FIELD|CLASS_FIELD|FIELD_OF|PROPERTY|READ_ONLY_PROPERTY)\(")  # ReflectedFields.h
DECLARED_KEYS = set(refl_attrs.TYPED_KEYS) | set(refl_attrs.TYPED_KEYS.values())


def readable_class_lines(orig: asmdiff.Original, g: "Getter", size: int, attr_sources: dict[int, tuple[list[str], str]],
                         macro: str, parent: str | None, declared: set[str],
                         class_types: dict[int, str] | None = None,
                         type_objects: dict[int, str] | None = None) -> tuple[list[str], int, int]:
    """One class in the readable form: the declarations its attribute and member-field functions need,
    its namespace (helpers, those functions, the description) and the getter's macro. Returns the
    lines, the helpers defined and the attribute/field functions defined. `declared` holds the file's
    declarations so far (each is written once per file). `class_types` maps the getters of the classes
    in this form to their names, for fields of those classes; `type_objects` names the type objects
    ReflectedTypes.h lists."""
    pre: list[str] = []
    inside: list[str] = []
    helpers_defined = attrs_defined = 0
    helpers = sorted([(a, slot) for slot, a in g.traits.items()] + [(g.fn_0x70, 0x70), (g.fn_0xe0, 0xe0)])
    for a, slot in helpers:
        name = role_name(slot)
        if a in attr_sources:
            extra, src = attr_sources[a]
            # One line per field (base/reflection/ReflectedFields.h); the named keys are declared there.
            src = "\n".join(refl_attrs.readable_fields(src.split("\n"), class_types, type_objects))
            extra = [d for d in extra if not (m := KEY_DECL_RE.match(d)) or int(m.group(1), 16) not in DECLARED_KEYS
                     or (d.startswith("extern") and re.search(rf"\bg_unk_{m.group(1)}\b", src))]
            extra = [d for d in extra if not (m := OBJECT_DECL_RE.match(d)) or int(m.group(1), 16) not in (type_objects or {})
                     or re.search(rf"\bg_unk_{m.group(1)}\b", src)]
            # A field of a class in this form calls T::refl_GetClass: declare it, in place of the
            # getter's address-named declaration once nothing else names that.
            for t in sorted(set(re.findall(r"DREAD_REFLECTED_CLASS_FIELD\(([\w:]+),", src))):
                extra.append(f"namespace {t} {{ const base::reflection::CClass* refl_GetClass(); }}")
            extra = [d for d in extra if not (m := GETTER_DECL_RE.match(d)) or int(m.group(1), 16) not in (class_types or {})
                     or re.search(rf"\bfn_{m.group(1)}\b", src)]
            pre += [d for d in extra if d not in declared]
            declared |= set(extra)
            inside.append(re.sub(rf"\bfn_{a:x}\(", f"{name}(", src, count=1))
            attrs_defined += 1
            helpers_defined += 1
            continue
        src = role_helper(orig, slot, a)
        if src:
            inside.append(src)
            helpers_defined += 1
        else:
            inside.append(ROLE_DECLS[slot].format(name=name))
    desc = ["struct refl_Description : ClassDescription {", f"    static constexpr u64 size = {size:#x};"]
    if g.abstract:
        desc.append("    static constexpr bool abstract = true;")
    for slot in TRAIT_SLOTS:
        if slot in g.traits:
            desc.append(f"    static constexpr auto {ROLES[slot][1]} = &{role_name(slot)};")
    desc.append("};")
    use = f"{macro}({g.type}, {parent});" if parent else f"{macro}({g.type});"
    return ([f"// {g.type}", *pre, f"namespace {g.type} {{", *inside, *desc, f"}}  // namespace {g.type}", use],
            helpers_defined, attrs_defined)


def helper_definition(orig: asmdiff.Original, slot: int, addr: int) -> str | None:
    """Source for a helper with a known shape, or None."""
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    text = "; ".join(f"{x.mnemonic} {x.op_str}".strip() for x in cs.disasm(orig.function(addr), addr))
    shape_src = HELPER_SHAPES.get((slot, text))
    return shape_src.format(name=f"fn_{addr:x}") if shape_src else None


def init_stores(orig: asmdiff.Original, addr: int) -> dict[int, int]:
    """CClass offset -> function address, for GOT-loaded values a descriptor constructor
    stores to [x19 + offset] (x19 = the descriptor)."""
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    regs: dict[int, int] = {}
    adrp: dict[int, int] = {}
    out: dict[int, int] = {}
    for x in cs.disasm(orig.function(addr), addr):
        o, m = x.operands, x.mnemonic
        if m == "adrp":
            adrp[o[0].reg] = o[1].imm
            regs.pop(o[0].reg, None)
        elif m == "ldr" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_MEM and o[1].mem.base in adrp:
            target = orig.img.pointer(adrp[o[1].mem.base] + o[1].mem.disp - BASE)
            if target is not None:
                regs[o[0].reg] = BASE + target
        elif m in ("str", "stp") and o[-1].type == capstone.arm64.ARM64_OP_MEM                 and cs.reg_name(o[-1].mem.base) == "x19":
            for i, r in enumerate(o[:-1]):
                if r.reg in regs:
                    out[o[-1].mem.disp + 8 * i] = regs[r.reg]
        elif m == "bl":
            regs.clear()
    return {k: v for k, v in out.items() if k in TRAIT_SLOTS}


def analyse(orig: asmdiff.Original, g: Getter, getters: set[int], ref_sig: str,
            inits: set[int] | None = None, root_sig: str | None = None,
            local_sig: str | None = None) -> None:
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    code = orig.function(g.address)
    g.shape_ok = shape(code) == ref_sig
    g.root = root_sig is not None and shape(code) == root_sig
    g.local_parent = local_sig is not None and shape(code) == local_sig
    adrp: dict[str, int] = {}
    for i, x in enumerate(cs.disasm(code, g.address)):
        ops = x.operands
        if x.mnemonic == "adrp":
            adrp[cs.reg_name(ops[0].reg)] = ops[1].imm
        elif x.mnemonic == "add" and len(ops) == 3 and cs.reg_name(ops[1].reg) in adrp \
                and ops[2].type == capstone.arm64.ARM64_OP_IMM:
            val = adrp[cs.reg_name(ops[1].reg)] + ops[2].imm
            if g.guard is None:
                g.guard = val
            elif cs.reg_name(ops[0].reg) == "x0" and g.descriptor is None and val != g.guard:
                g.descriptor = val
        elif x.mnemonic == "ldr" and len(ops) == 2 and ops[1].type == capstone.arm64.ARM64_OP_MEM:
            base = cs.reg_name(ops[1].mem.base)
            dst = cs.reg_name(ops[0].reg)
            if base in adrp and dst in ("x3", "x4"):
                slot = adrp[base] + ops[1].mem.disp - BASE
                target = orig.img.pointer(slot)
                if target is not None:
                    if dst == "x3":
                        g.fn_0x70 = BASE + target
                    else:
                        g.fn_0xe0 = BASE + target
        elif x.mnemonic == "bl":
            t = ops[0].imm
            if t in getters and t != g.address:
                g.parent = t
            elif t == gen_types.KIND_HASH:
                # The name's constructor is inlined (base::global::UnkStrIdInline): a few base
                # library getters. Their files are verified by compiling them.
                g.inline_name = True
            elif inits and t in inits:
                g.init = t
    if g.init is not None:
        g.traits = init_stores(orig, g.init)
        ins = [f"{x.mnemonic} {x.op_str}" for x in cs.disasm(orig.function(g.init), g.init)]
        j = next((i for i, s in enumerate(ins) if s.startswith("and w") and s.endswith("#0xfffffffc")), None)
        g.abstract = j is not None and any(s.startswith("orr w") and s.endswith("#1") for s in ins[j:j + 10])


def getter_ok(g: Getter, block: list[Getter]) -> bool:
    """The getter has a shape the macros reproduce, given the other types in its TU."""
    if g.root and not g.parent:
        return True
    if g.inline_name:
        return not g.local_parent
    if g.local_parent and g.parent:
        return any(o.address == g.parent for o in block)
    return g.shape_ok and bool(g.parent)


def layout_gap(block: list[Getter]) -> int | None:
    """Bytes of other statics between the TU's getter guards and its descriptors, when the
    descriptors are contiguous (None otherwise). LLVM's GlobalMerge packs a TU's internal
    statics into one block sorted by size, so statics sized between a guard (8) and a
    descriptor (0x138) land between them; the getters address descriptors from their guards."""
    n = len(block)
    gap = block[0].descriptor - (block[0].guard + 8 * n)
    expected = [block[0].guard + 8 * n + gap + DESCRIPTOR_SIZE * i for i in range(n)]
    return gap if gap >= 0 and gap % 8 == 0 and [g.descriptor for g in block] == expected else None


def padding_lines(block: list[Getter], gap: int) -> list[str]:
    """Internal statics standing in for the TU's unreconstructed ones: pieces of at most 0x130
    bytes (so they sort before the descriptors) and a 1-byte anchor (sorts first), used
    together so that GlobalMerge keeps them in the TU's block."""
    k = -(-gap // 0x130)
    units = [gap // 8 // k + (1 if i < gap // 8 % k else 0) for i in range(k)]
    names = [f"unk_statics_{i}" for i in range(k)]
    tag = f"{block[0].address:x}"
    return (["// Statics of this TU that aren't reconstructed: padding that puts the descriptors at",
             "// their original offsets in the TU's merged globals (see layout_gap in gen_reflection.py).",
             "namespace {"]
            + [f"alignas(8) char {nm}[{8 * u:#x}];" for nm, u in zip(names, units)]
            + ["char unk_statics_anchor;", "}  // namespace",
               f"void refl_UnkStaticsUse_{tag}(void** out) {{"]
            + [f"    out[{i}] = {nm};" for i, nm in enumerate(names)]
            + [f"    out[{k}] = &unk_statics_anchor;", "}", ""])


def registration_functions(orig: asmdiff.Original) -> list[int]:
    """Functions that call AddField, AddFunction or the class-attribute getter (refl_attrs)."""
    targets = {refl_attrs.GET_ATTRS, refl_attrs.ADD_FIELD, refl_attrs.ADD_FUNCTION}
    out = []
    for a, size in sorted(orig.sizes.items()):
        code = orig.function(a)
        for i in range(0, len(code) - 3, 4):
            w = struct.unpack_from("<I", code, i)[0]
            if w >> 26 == 0x25:  # BL
                imm = w & 0x3FFFFFF
                if a + i + 4 * (imm - (1 << 26) if imm & (1 << 25) else imm) in targets:
                    out.append(a)
                    break
    return out


REGISTRATION_CHUNK = 200  # functions per generated registration_NN.cpp / function_inits_NN.cpp
# Defined by hand (the smoke test's known match): src/base/reflection/CFunction.cpp.
HAND_WRITTEN = {0x7100A19530}
STRH_B2_X19 = 0x79000000 | (0xB2 // 2) << 10 | 19 << 5  # strh wN, [x19, #0xb2]


def initialiser_candidates(orig: asmdiff.Original) -> list[int]:
    """Functions that store a reflected function's u16 at +0xb2 through x19."""
    out = []
    for a, size in sorted(orig.sizes.items()):
        if a in HAND_WRITTEN:
            continue
        code = orig.function(a)
        if any(struct.unpack_from("<I", code, i)[0] & 0xFFFFFFE0 == STRH_B2_X19 for i in range(0, len(code) - 3, 4)):
            out.append(a)
    return out


CLASS_KIND = "base::reflection::CClass"


def is_abstract(orig: asmdiff.Original, init: int) -> bool:
    """The constructor sets flag bit 0 (orr #1 after the and #~3)."""
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    ins = [f"{x.mnemonic} {x.op_str}" for x in cs.disasm(orig.function(init), init)]
    j = next((i for i, s in enumerate(ins) if s.startswith("and w") and s.endswith("#0xfffffffc")), None)
    return j is not None and any(s.startswith("orr w") and s.endswith("#1") for s in ins[j:j + 10])


def class_constructors(orig: asmdiff.Original) -> dict[int, tuple[int, bool, dict[int, int]]]:
    """Every CClass descriptor constructor: init -> (size, abstract, traits)."""
    out = {}
    for a, size in sorted(orig.sizes.items()):
        code = orig.function(a)
        for i in range(0, len(code) - 3, 4):
            w = struct.unpack_from("<I", code, i)[0]
            if w >> 26 == 0x25:
                imm = w & 0x3FFFFFF
                if a + i + 4 * (imm - (1 << 26) if imm & (1 << 25) else imm) == gen_types.KIND_HASH:
                    break
        else:
            continue
        kind, stores, _ = gen_types.emulate(orig, a)
        sz = stores.get(0x18)
        traits = init_stores(orig, a)
        if kind == CLASS_KIND and sz and sz[0] == "imm" and all(k in traits for k in (0x58, 0x60, 0x68)):
            out[a] = (sz[1], is_abstract(orig, a), traits)
    return out


def constructor_objects(orig: asmdiff.Original, inits: set[int]) -> dict[int, int]:
    """{constructor: descriptor} for the descriptor constructors a function calls with a global's address in
    x0 (adrp + add, a GOT slot, or a register holding one): the descriptor the static initialiser builds.
    The first call, by address, decides."""
    candidates = []
    for a, size in sorted(orig.sizes.items()):
        code = orig.function(a)
        for i in range(0, len(code) - 3, 4):
            w = struct.unpack_from("<I", code, i)[0]
            if w >> 26 == 0x25 and asmdiff.branch_target(w, a + i) in inits:
                candidates.append(a)
                break
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    out: dict[int, int] = {}
    for a in candidates:
        regs: dict[str, int] = {}
        for x in cs.disasm(orig.function(a), a):
            o, m = x.operands, x.mnemonic
            dst = cs.reg_name(o[0].reg).replace("w", "x", 1) if o and o[0].type == capstone.arm64.ARM64_OP_REG else None
            if m == "bl":
                if o[0].imm in inits and "x0" in regs:
                    out.setdefault(o[0].imm, regs["x0"])
                regs = {r: v for r, v in regs.items() if r[1:].isdigit() and 19 <= int(r[1:]) <= 28}
                continue
            value = None
            if m == "adrp":
                value = o[1].imm
            elif m == "add" and len(o) == 3 and o[2].type == capstone.arm64.ARM64_OP_IMM \
                    and cs.reg_name(o[1].reg) in regs:
                value = regs[cs.reg_name(o[1].reg)] + o[2].imm
            elif m == "ldr" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_MEM \
                    and cs.reg_name(o[1].mem.base) in regs and dst and dst.startswith("x"):
                value = orig.got_slot(regs[cs.reg_name(o[1].mem.base)] + o[1].mem.disp)[0]
            elif m == "mov" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_REG:
                value = regs.get(cs.reg_name(o[1].reg))
            for r in x.regs_access()[1]:
                regs.pop(cs.reg_name(r).replace("w", "x", 1), None)
            if dst and value is not None:
                regs[dst] = value
    return out


def loose_class_names(orig: asmdiff.Original, version: str, attached: dict[int, str], taken: set[str]) -> dict[int, str]:
    """{constructor: class} for the descriptor constructors no class getter calls: the name the static
    initialiser gives the descriptor (tools/gen_fields.py's built_objects), found through the descriptor its
    TypeOf returns or the one it is called on. Only names the namespace form can spell (readable_type),
    that no other constructor and no class in data/reflection_types.csv has, of constructors whose
    functions no other constructor shares."""
    import gen_fields
    ctors = class_constructors(orig)
    users: dict[int, int] = {}
    for _, _, traits in ctors.values():
        for h in set(traits.values()):
            users[h] = users.get(h, 0) + 1
    loose = {a: traits for a, (_, _, traits) in ctors.items() if all(users[h] == 1 for h in traits.values())}
    called = constructor_objects(orig, set(loose))
    objects = {}
    for a, traits in loose.items():
        found = {o for o in (gen_types.returned_object(orig, traits[0x68]), called.get(a)) if o is not None}
        if len(found) == 1:
            objects[a] = found.pop()
    names = gen_fields.built_objects(version, set(objects.values()))
    named = {}
    for a, o in objects.items():
        if o not in names:
            continue
        if loose_readable(names[o]):
            named[a] = names[o]
        elif key_value_class(names[o]):
            named[a] = key_value_class(names[o])[0]
        elif value_template_class(names[o]):
            named[a] = value_template_class(names[o])[0]
    count: dict[str, int] = {}
    for n in named.values():
        count[n] = count.get(n, 0) + 1
    # a constructor reflection data already gives a class keeps it: only that class's name is taken here
    return {a: n for a, n in named.items() if count[n] == 1
            and (attached.get(a) == n if a in attached else n not in taken)}


def loose_names(orig: asmdiff.Original, version: str, attached: dict[int, str], taken: set[str]) -> dict[int, str]:
    """Address -> data/functions/ name of each named loose constructor (loose_class_names) and the functions
    it stores: T::refl_InitClass and T::refl_<role>, the names the namespace form writes them under."""
    ctors = class_constructors(orig)
    out = {}
    for a, name in loose_class_names(orig, version, attached, taken).items():
        out[a] = f"{name}::refl_InitClass"
        for slot, h in ctors[a][2].items():
            out[h] = f"{name}::{role_name(slot)}"
    return out


# The pair a collection's key-element iterator yields, as the static initialiser names its descriptor.
KEY_VALUE_RE = re.compile(r"base::reflection::CollectionTypeMapper<(.+)>::TKeyElement(Const)?Iterator::value_type")
# Its class (base/reflection/KeyElementValue.h), as data/functions/ names it.
KEY_VALUE_CLASS_RE = re.compile(r"base::reflection::(Const)?KeyElementValue<(.+)>")


# A member type of a template instance (`GUI::CDisplayObjectTrack<float>::SKey`), whose arguments name no template.
NESTED_TYPE_RE = re.compile(r"([A-Za-z_][\w:]*<[^<>]*>)::([A-Za-z_]\w*)")
# Members of template instances that are values, not types: an enum's count (`EnumClass<E>::Count`, a
# CArray's bound). A name with one isn't spelled: declaring it as a type would misstate it.
VALUE_MEMBER_RE = re.compile(r"\bEnumClass<")


def spell_type(text: str) -> tuple[str, list[str], list] | None:
    """(the type as llvm-cxxfilt prints it, the declarations a file needs to name it, the parsed types whose
    declarations must not collide) for a type tools/gen_types.py's parse_type reads, or one whose arguments
    include member types of template instances (declared as the template with that member); None otherwise."""
    if VALUE_MEMBER_RE.search(text):
        return None
    nested: dict[str, tuple[str, str]] = {}

    def placeholder(m: re.Match) -> str:
        key = f"UnkNestedType{len(nested)}"
        nested[key] = (m.group(1), m.group(2))
        return key
    node = gen_types.parse_type(NESTED_TYPE_RE.sub(placeholder, text))
    if not node or node[2]:
        return None
    nodes = [node]
    decls = [d for d in gen_types.type_declarations(node) if not re.fullmatch(r"class UnkNestedType\d+;", d)]
    rendered = gen_types.render_type(node)
    for key, (head, leaf) in nested.items():
        hn = gen_types.parse_type(head)
        if not hn or hn[2] or hn[1] is None:
            return None
        nodes.append(hn)
        scope, _, name = hn[0].rpartition("::")
        params = ", ".join(["class"] * len(hn[1]))
        declared = gen_types.declare_in(scope, f"template <{params}> class {name};")
        decls += [gen_types.declare_in(scope, f"template <{params}> class {name} {{ public: struct {leaf}; }};")
                  if d == declared else d for d in gen_types.type_declarations(hn)]
        rendered = re.sub(rf"\b{key}\b", f"{gen_types.render_type(hn)}::{leaf}", rendered)
    if gen_types.declaration_clashes(nodes):
        return None
    return rendered, decls, nodes


def key_value_class(name: str) -> tuple[str, tuple] | None:
    """(the class data/functions/ names it under, spell_type of the collection) of a key-element iterator's
    pair (KeyElementValue<C> or ConstKeyElementValue<C>), when spell_type spells C; None for any other name."""
    m = KEY_VALUE_RE.fullmatch(re.sub(r"(?<=\w)\s+(?=\w)", " ", re.sub(r"\s+(?!\w)|(?<!\w)\s+", "", name)))
    spelled = spell_type(m.group(1)) if m else None
    if not spelled:
        return None
    cls = f"base::reflection::{'Const' if m.group(2) else ''}KeyElementValue<{spelled[0]}>"
    return gen_types.demangled_spelling(cls), spelled


# Game class templates whose instances reflection describes, with the header that declares their value
# operations (DREAD_REFLECTED_VALUE_MEMBERS, base/reflection/ReflectedValue.h).
VALUE_TEMPLATES = {"CGameLink": "game/CGameLink.h"}


def value_template_class(name: str) -> tuple[str, tuple] | None:
    """(the class data/functions/ names it under, spell_type of it) of an instance of a VALUE_TEMPLATES
    template; None for any other name."""
    spelled = spell_type(re.sub(r"(?<=\w)\s+(?=\w)", " ", re.sub(r"\s+(?!\w)|(?<!\w)\s+", "", name)))
    if not spelled or spelled[2][0][0] not in VALUE_TEMPLATES or spelled[2][0][1] is None:
        return None
    return gen_types.demangled_spelling(spelled[0]), spelled


def template_value_lines(orig: asmdiff.Original, name: str, size: int, abstract: bool,
                         traits: dict[int, int]) -> tuple[list[str], list[str], list]:
    """A class template instance's description (`name` as data/functions/ gives it: a key-element pair or a
    VALUE_TEMPLATES instance) as specialisations of its members, with the declarations its file needs and the
    parsed types whose declarations must not collide."""
    source = gen_types.source_spelling(name)
    m = KEY_VALUE_CLASS_RE.fullmatch(source)
    if m:
        cls = f"{m.group(1) or ''}KeyElementValue<{m.group(2)}>"
        _, decls, nodes = spell_type(m.group(2))
        lines = [f"// The pair {m.group(2)}'s {'const ' if m.group(1) else ''}key-element iterator yields."]
    else:
        cls = source
        _, decls, nodes = spell_type(source)
        decls = [d for d in decls if not any(re.search(rf"\bclass {t};", d) for t in VALUE_TEMPLATES)]
        lines = [f"// {cls}"]
    for h, slot in sorted((h, slot) for slot, h in traits.items()):
        src = role_helper(orig, slot, h, f"{cls}::{role_name(slot)}")
        if src:
            lines += ["template <>", src]
    lines += ["template <>", f"struct {cls}::refl_Description : ClassDescription {{",
              f"    static constexpr u64 size = {size:#x};"]
    if abstract:
        lines.append("    static constexpr bool abstract = true;")
    lines += [f"    static constexpr auto {ROLES[slot][1]} = &{role_name(slot)};" for slot in TRAIT_SLOTS if slot in traits]
    return lines + ["};", f"DREAD_REFLECTED_DESCRIPTOR({cls});"], decls, nodes


def template_value_kind(name: str) -> str | None:
    """The files a data/functions/ class name's description goes to (TEMPLATE_VALUE_FILES), or None."""
    if KEY_VALUE_CLASS_RE.fullmatch(name):
        return "key_element_values"
    return "class_values" if value_template_class(gen_types.source_spelling(name)) else None


def reflection_header_classes() -> frozenset[str]:
    """The base::reflection classes the headers under include/base/reflection/ declare or define."""
    out = set()
    for h in (ROOT / "include" / "base" / "reflection").glob("*.h"):
        out |= set(re.findall(r"\b(?:class|struct)\s+([A-Za-z_]\w*)", h.read_text(encoding="utf-8")))
    return frozenset(out)


def member_form_header(name: str) -> str | None:
    """The header (relative to include/) of a base::reflection class whose definition there declares its
    reflection functions and `struct refl_Description;` as members, or None. Such a class's description is
    written as definitions of those members (loose_class_lines), with the same symbols as the namespace form."""
    m = re.fullmatch(r"base::reflection::([A-Za-z_]\w*)", name)
    if not m:
        return None
    for h in sorted((ROOT / "include" / "base" / "reflection").glob("*.h")):
        body = re.search(rf"^struct {m.group(1)} \{{\n(.*?)^\}};", h.read_text(encoding="utf-8"), re.M | re.S)
        if body and re.search(r"^\s*struct refl_Description;", body.group(1), re.M):
            return f"base/reflection/{h.name}"
    return None


def loose_readable(name: str) -> bool:
    """A loose constructor's class the namespace form can spell: readable_type's, or a base::reflection class
    the reflection headers neither declare nor define (a namespace of its name then collides with nothing),
    or one whose definition declares its reflection members (member_form_header)."""
    if readable_type(name):
        return True
    m = re.fullmatch(r"base::reflection::([A-Za-z_]\w*)", name)
    return bool(m) and (m.group(1) not in reflection_header_classes() or member_form_header(name) is not None)


KEY_VALUE_HEAD = ['#include "base/reflection/CClass.h"', '#include "base/reflection/KeyElementValue.h"',
                  '#include "base/reflection/ReflectedClass.h"', "", *READABLE_PRELUDE,
                  "using base::reflection::ConstKeyElementValue;", "using base::reflection::KeyElementValue;"]
# kind -> (the head of its files, their description)
TEMPLATE_VALUE_FILES = {
    "key_element_values": (KEY_VALUE_HEAD, "The classes of the pairs collections' key-element iterators yield "
                                           "(base/reflection/KeyElementValue.h),"),
    "class_values": (['#include "base/reflection/CClass.h"', '#include "base/reflection/ReflectedClass.h"',
                      *(f'#include "{h}"' for h in sorted(set(VALUE_TEMPLATES.values()))), "", *READABLE_PRELUDE],
                     "Reflected instances of game class templates, described as specialisations of their members,"),
}


def loose_class_lines(orig: asmdiff.Original, name: str, size: int, abstract: bool,
                      traits: dict[int, int]) -> list[str]:
    """A loose constructor's class in the namespace form: its functions under their roles, its description
    and DREAD_REFLECTED_DESCRIPTOR. A class whose header declares them as members (member_form_header) gets
    definitions of those members instead, and no namespace."""
    header = member_form_header(name)
    if header:
        defined = [src for h, slot in sorted((h, slot) for slot, h in traits.items())
                   if (src := role_helper(orig, slot, h, f"{name}::{role_name(slot)}"))]
        desc = [f"struct {name}::refl_Description : ClassDescription {{", f"    static constexpr u64 size = {size:#x};"]
        if abstract:
            desc.append("    static constexpr bool abstract = true;")
        desc += [f"    static constexpr auto {ROLES[slot][1]} = &{role_name(slot)};"
                 for slot in TRAIT_SLOTS if slot in traits]
        return [f"// {name} (its reflection functions are members, {header})", *defined, *desc, "};",
                f"DREAD_REFLECTED_DESCRIPTOR({name});"]
    inside = []
    for h, slot in sorted((h, slot) for slot, h in traits.items()):
        inside.append(role_helper(orig, slot, h) or ROLE_DECLS[slot].format(name=role_name(slot)))
    desc = ["struct refl_Description : ClassDescription {", f"    static constexpr u64 size = {size:#x};"]
    if abstract:
        desc.append("    static constexpr bool abstract = true;")
    desc += [f"    static constexpr auto {ROLES[slot][1]} = &{role_name(slot)};" for slot in TRAIT_SLOTS if slot in traits]
    return [f"// {name}", f"namespace {name} {{", *inside, *desc, "};", f"}}  // namespace {name}",
            f"DREAD_REFLECTED_DESCRIPTOR({name});"]


def member_includes(names: list[str]) -> list[str]:
    """#include lines for the headers of the member-form classes among `names` (member_form_header)."""
    headers = sorted({h for n in names if (h := member_form_header(n))})
    return [f'#include "{h}"' for h in headers]


def constructor_lines(init: int, size: int, abstract: bool, traits: dict[int, int]) -> list[str]:
    """Traits and the explicit instantiation of one descriptor constructor (helpers declared
    separately)."""
    lines = ["template <>",
             f"struct base::reflection::UnkReflClassTraits<UnkRefl_fn_{init:x}> {{",
             f"    static constexpr u64 size = {size:#x};",
             f"    static constexpr bool abstract = {'true' if abstract else 'false'};"]
    for slot in TRAIT_SLOTS:
        value = f"&fn_{traits[slot]:x}" if slot in traits else "nullptr"
        lines.append(f"    static constexpr {SLOT_TYPES[slot][0]} fn_{slot:#x} = {value};")
    return lines + ["};",
                    "template base::reflection::CClass::CClass(const base::global::CStrId&, "
                    "const base::reflection::CClass*, base::reflection::ClassSetupFn, "
                    f"base::reflection::ClassSetupFn, UnkRefl_fn_{init:x}*);"]


def write_chunks(prefix: str, header: list[str], defs: dict[int, tuple[list[str], str]]) -> None:
    addrs = sorted(defs)
    for k in range(0, len(addrs), REGISTRATION_CHUNK):
        chunk = addrs[k:k + REGISTRATION_CHUNK]
        decls = list(dict.fromkeys(d for a in chunk for d in defs[a][0]))
        lines = ["// Generated by tools/gen_reflection.py from main; regenerate rather than edit.", *header,
                 '#include "base/reflection/CClass.h"', *refl_attrs.PRELUDE, "", *decls, "",
                 *(defs[a][1] + chr(10) for a in chunk)]
        (OUT / f"{prefix}_{k // REGISTRATION_CHUNK:02d}.cpp").write_text(chr(10).join(lines), encoding="utf-8",
                                                                        newline=chr(10))


def file_name(type_name: str, taken: set[str]) -> str:
    base = re.sub(r"[^A-Za-z0-9]+", "_", type_name.replace("::", "__")).strip("_")[:80] or "unnamed"
    name = base
    n = 2
    while name.lower() in taken:
        name = f"{base}_{n}"
        n += 1
    taken.add(name.lower())
    return name


# A type-keyed getter's explicit instantiation in src/reflection_types/ (tools/gen_types.py).
KEYED_DEF_RE = re.compile(r"^template const base::reflection::CType[*&] base::reflection::(Pointer|Primitive)Type<(.+)>::Get\(\);$",
                          re.M)
ENUM_OPS_RE = re.compile(r"^template struct base::reflection::ValueOps<(.+)>;$", re.M)


def keyed_getters() -> tuple[dict[int, tuple[str, str]], frozenset[str]]:
    """({getter: (kind, T)} of the type-keyed getters src/reflection_types/ defines (kind Pointer or Primitive),
    found by the name data/functions/ gives them, refl_GetType<T>; the enums and flag sets those files name)."""
    texts = [f.read_text(encoding="utf-8") for f in (ROOT / "src" / "reflection_types").glob("*.cpp")]
    kinds = {gen_types.demangled_spelling(t): (k, t) for text in texts for k, t in KEYED_DEF_RE.findall(text)}
    enums = frozenset(e for f in (ROOT / "src" / "reflection_types").glob("*.cpp")
                      if f.name.startswith(("enum_types_", "flagset_types_"))
                      for e in ENUM_OPS_RE.findall(f.read_text(encoding="utf-8")))
    out = {}
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                m = re.fullmatch(r"base::reflection::refl_GetType<(.+)>", r["name"])
                if m and m.group(1) in kinds:
                    out[int(r["address"], 16)] = kinds[m.group(1)]
    return out, enums


def types_by_type(text: str, keyed: dict[int, tuple[str, str]], enums: frozenset[str] = frozenset()) -> str:
    """`text` with each call of a type-keyed getter (keyed_getters) written by its type: &PrimitiveType<T>::Get()
    or PointerType<T>::Get(). T's declarations and an explicit instantiation declaration go after the file's
    includes, so that the call stays a call, as in the original; the getter's own declaration goes."""
    used = {int(a, 16) for a in re.findall(r"\bfn_([0-9a-f]{10})\(\)", text)} & set(keyed)
    if not used:
        return text
    first: list[str] = []
    for a in sorted(used, key=lambda a: keyed[a][1]):
        kind, t = keyed[a]
        if kind == "Primitive":
            call = f"&base::reflection::PrimitiveType<{t}>::Get()"
            if t not in gen_types.PRIMITIVE_VALUE_TYPES.values():
                first.append(gen_types.class_declaration(t))
            first.append(f"extern template const base::reflection::CType& base::reflection::PrimitiveType<{t}>::Get();")
        else:
            parsed = gen_types.pointer_value_type(t, enums)
            if parsed is None:
                continue
            call = f"base::reflection::PointerType<{t}>::Get()"
            first += gen_types.type_declarations(parsed[1], enums)
            first.append(f"extern template const base::reflection::CType* base::reflection::PointerType<{t}>::Get();")
        decl = re.compile(rf"(?:const )?(?:base::reflection::)?CType\* fn_{a:x}\(\);")
        lines = [line if decl.fullmatch(line) else re.sub(rf"\bfn_{a:x}\(\)", call, line)
                 for line in text.split(chr(10))]
        if not any(re.search(rf"\bfn_{a:x}\b", line) for line in lines if not decl.fullmatch(line)):
            lines = [line for line in lines if not decl.fullmatch(line)]  # the getter is named only by type now
        text = chr(10).join(lines)
    lines = text.split(chr(10))
    head = 0
    for i, line in enumerate(lines):
        if line.startswith(("#include", "using ")):
            head = i + 1
        elif line and not line.startswith("//"):
            break
    includes = [f'#include "base/reflection/{h}"' for h, kind in (("PrimitiveType.h", "Primitive"),
                                                                   ("CPointerType.h", "Pointer"))
                if any(keyed[a][0] == kind for a in used)]
    first = [line for line in dict.fromkeys(includes + first) if line not in lines]
    return chr(10).join(lines[:head] + first + lines[head:])


# An enum's or flag set's TypeOf in src/reflection_types/ (tools/gen_types.py): the type object it returns.
TYPE_OBJECT_RE = re.compile(r"^const CType& base::reflection::ValueOps<(.+)>::TypeOf\(const void\*\) \{ return (g_unk_[0-9a-f]{10}); \}$",
                            re.M)


def enum_type_objects() -> dict[str, tuple[str, str]]:
    """{g_unk_<addr>: (kind, T)} of the type objects src/reflection_types/ names by their enum or flag set
    (kind enum or flagset): the objects base/reflection/TypeObject.h names by type."""
    out = {}
    for f in sorted((ROOT / "src" / "reflection_types").glob("*.cpp")):
        kind = "enum" if f.name.startswith("enum_types_") else "flagset" if f.name.startswith("flagset_types_") else None
        if kind:
            for t, g in TYPE_OBJECT_RE.findall(f.read_text(encoding="utf-8")):
                if gen_types.QUALIFIED_RE.fullmatch(t):
                    out[g] = (kind, t)
    out.update(BUILTIN_TYPE_OBJECTS)
    out.update(link_type_objects())
    return out


# A vector of links' collection descriptor (src/reflection_types/collection_types_*.cpp) and its element's type
# object: the type object of that CGameLink<T> (game/CGameLink.h).
LINK_COLLECTION_RE = re.compile(r"^struct base::reflection::CollectionTypeDescription<base::global::CRntVector<"
                                r"(CGameLink<\w+>)>> .*?^    static const CType\* element\(\) \{ return &(g_unk_[0-9a-f]{10}); \}$",
                                re.M | re.S)


# A collection's type object (src/reflection_types/collection_types_*.cpp): CollectionTypeMapper<T>::TypeOf returns it.
COLLECTION_TYPE_OBJECT_RE = re.compile(r"^template <> const base::reflection::CType& base::reflection::CollectionTypeMapper<(.+)>"
                                       r"::TypeOf\(const void\*\) \{ return (g_unk_[0-9a-f]{10}); \}$", re.M)


def collection_type_objects() -> dict[str, tuple[str, str]]:
    """{g_unk_<addr>: ("collection", T)} of the collections whose type objects src/reflection_types/ names."""
    out = {}
    for f in sorted((ROOT / "src" / "reflection_types").glob("collection_types_*.cpp")):
        for t, g in COLLECTION_TYPE_OBJECT_RE.findall(f.read_text(encoding="utf-8")):
            if gen_types.parse_type(t):
                out[g] = ("collection", t)
    return out


# A type object a generated file still declares by address.
UNNAMED_TYPE_OBJECT_RE = re.compile(r"^extern (?:base::reflection::)?CType (g_unk_[0-9a-f]{10});$", re.M)


def built_type_objects(version: str, texts: list[str], named: set[str]) -> dict[str, tuple[str, str]]:
    """{g_unk_<addr>: ("built", T)} of the type objects the generated texts still declare by address, named by
    the string their static initialiser builds them with (gen_fields.built_objects), where T parses as a type."""
    import gen_fields
    wanted = {int(g[len("g_unk_"):], 16) for text in texts for g in UNNAMED_TYPE_OBJECT_RE.findall(text)} - \
        {int(g[len("g_unk_"):], 16) for g in named}
    return {f"g_unk_{a:x}": ("built", t) for a, t in gen_fields.built_objects(version, wanted).items()
            if gen_types.parse_type(t)}


def link_type_objects() -> dict[str, tuple[str, str]]:
    """{g_unk_<addr>: ("link", "CGameLink<T>")} of the links whose type objects a vector of them names."""
    out = {}
    for f in sorted((ROOT / "src" / "reflection_types").glob("collection_types_*.cpp")):
        for block in f.read_text(encoding="utf-8").split(chr(10) + "template <>" + chr(10)):
            m = LINK_COLLECTION_RE.match(block)
            if m:
                out[m.group(2)] = ("link", m.group(1))
    return out


# Type objects of built-in types, which need no declaration of T: the void type (base/reflection/CType.h).
BUILTIN_TYPE_OBJECTS = {"g_unk_7101d5af48": ("builtin", "void")}


def type_objects_in_tiers(text: str, tiers: list[dict[str, tuple[str, str]]], built: dict[str, tuple[str, str]],
                          builds) -> str:
    """Names a file's type objects with the widest tier that builds. When the widest doesn't (one declaration of T
    collides with the file's own), the built objects are added one at a time over the narrower tier that does, so
    one bad name keeps only its own address."""
    base: dict[str, tuple[str, str]] = {}
    best = text
    for objs in tiers:
        new = type_objects_by_type(text, objs)
        if new != text and builds(new):
            if objs is tiers[0]:
                return new
            base, best = dict(objs), new
            break
    for g in sorted(k for k in built if k in text and k not in base):
        trial = type_objects_by_type(text, {**base, g: built[g]})
        if trial != best and builds(trial):
            base[g] = built[g]
            best = trial
    return best


def type_objects_by_type(text: str, objects: dict[str, tuple[str, str]], layouts: bool = False) -> str:
    """`text` with each type object of `objects` written by its type, `base::reflection::TypeObject<T>::object`
    (base/reflection/TypeObject.h): T's declaration and the specialisation's go after the file's includes, in
    place of the object's own `extern CType g_unk_<addr>;`. With `layouts`, an object the file declares at file
    scope with another type, `extern [const] Layout g_unk_<addr>;`, becomes `TypeObject<T, [const] Layout>`
    where that declaration was, so its uses keep their type; otherwise such an object stays."""
    lines = text.split(chr(10))
    # An object already named by type keeps only its label, asm("g_unk_<addr>").
    used = sorted(set(re.findall(r"(?<!asm\(\")\bg_unk_[0-9a-f]{10}\b", text)) & set(objects))
    first: list[str] = []
    in_place = False
    for g in used:
        decl = re.compile(rf"extern (?:base::reflection::)?CType {g};")
        others = [line for line in lines if re.search(rf"\b{g}\b", line) and "extern" in line and not decl.fullmatch(line)]
        kind, t = objects[g]
        scope, _, leaf = t.rpartition("::")
        declared = (gen_types.enum_declaration(t) if kind == "enum" else
                    gen_types.declare_in(scope, f"struct {leaf};") if kind == "flagset" else
                    f"template <class T> struct CGameLink;{chr(10)}class {t[len('CGameLink<'):-1]};" if kind == "link" else
                    chr(10).join(gen_types.type_declarations(gen_types.parse_type(t))) if kind in ("collection", "built") else "")
        if others:
            layout = re.compile(rf"extern ((?:const )?[A-Za-z_][\w:]*(?:<[\w:, ]*>)?) {g};(?:\s*//.*)?")
            match = [layout.fullmatch(line) for line in others]
            if not layouts or len(others) != 1 or not match[0]:
                continue
            spelled = match[0].group(1)
            if spelled in ("CType", "base::reflection::CType"):
                spelled_t = t
                head = "base::reflection::CType"
            else:
                spelled_t = f"{t}, {spelled}"
                head = spelled
            name = f"base::reflection::TypeObject<{spelled_t}>::object"
            at = lines.index(others[0])
            spec = [*declared.split(chr(10))] if declared else []
            lines[at:at + 1] = spec + [f'template <> {head} {name} asm("{g}");']
            lines = [re.sub(rf"\b{g}\b", name, line) if i < at or i > at + len(spec) else line
                     for i, line in enumerate(lines)]
            in_place = True
            continue
        if declared:
            first.extend(declared.split(chr(10)) if kind in ("link", "collection", "built") else [declared])
        first.append(f'template <> base::reflection::CType base::reflection::TypeObject<{t}>::object asm("{g}");')
        lines = [line if decl.fullmatch(line) else re.sub(rf"\b{g}\b", f"base::reflection::TypeObject<{t}>::object", line)
                 for line in lines if not decl.fullmatch(line)]
    if layouts:
        # A CType object read back as a CType is the object itself.
        lines = [re.sub(r"reinterpret_cast<const (?:base::reflection::)?CType&>\((base::reflection::TypeObject<[^<>]*>::object)\)",
                        r"\1", line) for line in lines]
    if not first:
        joined = chr(10).join(lines)
        return with_include(joined, '#include "base/reflection/TypeObject.h"') if in_place else joined
    head = 0
    for i, line in enumerate(lines):
        if line.startswith(("#include", "using ")):
            head = i + 1
        elif line and not line.startswith("//"):
            break
    first = [line for line in dict.fromkeys(['#include "base/reflection/TypeObject.h"'] + first) if line not in lines[:head]]
    return chr(10).join(lines[:head] + first + lines[head:])


def attribute_keys() -> dict[int, str]:
    """{key getter: name} of the attribute keys whose getters are named by the string they register,
    base::reflection::attr::<Name> (declared in base/reflection/ReflectedAttributes.h; data/names.csv): every key
    but the typed ones (typed_attribute_keys)."""
    return {a: name for a, name in refl_attrs.ATTRIBUTE_NAMES.items() if a not in refl_attrs.TYPED_KEYS}


def attribute_keys_by_name(text: str, keys: dict[int, str]) -> str:
    """`text` with each call of an attribute key's getter (attribute_keys) written by the getter's name,
    base::reflection::attr::<Name>(); its address-named declaration goes once nothing else names it by address."""
    used = {int(a, 16) for a in re.findall(r"\bfn_([0-9a-f]{10})\(\)", text)} & set(keys)
    if not used:
        return text
    lines = text.split(chr(10))
    for a in sorted(used):
        decl = re.compile(rf"(?:const )?(?:base::reflection::)?UnkReflAttrKey& fn_{a:x}\(\);(?:  //.*)?")
        lines = [line if decl.fullmatch(line) else re.sub(rf"\bfn_{a:x}\(\)", f"base::reflection::attr::{keys[a]}()", line)
                 for line in lines]
        if not any(re.search(rf"\bfn_{a:x}\b", line) for line in lines if not decl.fullmatch(line)):
            lines = [line for line in lines if not decl.fullmatch(line)]
    return with_include(chr(10).join(lines), '#include "base/reflection/ReflectedFields.h"')


# A reflected function's registration, in the form DREAD_REFLECTED_FUNCTION (ReflectedFields.h) writes.
FUNCTION_RE = re.compile(r'^(\s*)if \(CFunction\* fn = AddReflectedFunction\(cls, CStrId\(("(?:[^"\\]|\\.)*"), true\)\)\)',
                         re.M)


def functions_by_macro(text: str) -> str:
    """`text` with each reflected function's registration written DREAD_REFLECTED_FUNCTION("Name") (the same
    `if`, from base/reflection/ReflectedFields.h)."""
    new = FUNCTION_RE.sub(r"\1DREAD_REFLECTED_FUNCTION(\2)", text)
    return with_include(new, '#include "base/reflection/ReflectedFields.h"') if new != text else text


# A reflected function described in place (tools/refl_attrs.py): its name, return type, flags and invoker.
DESCRIPTION_RE = re.compile(r'^(\s*)DREAD_REFLECTED_FUNCTION\(("(?:[^"\\]|\\.)*")\) \{\n'
                            r'\s*fn->name = base::global::UnkStrId\(\2, true\);\n'
                            r'\s*fn->return_type = ?([^;\n]+);\n'
                            r'\s*fn->field_0xb2 = (\w+);\n'
                            r'\s*fn->field_0x40 = nullptr;\n'
                            r'\s*fn->invoker = reinterpret_cast<void\*>\(&(\w+)\);\n'
                            r'(\1\}\n)?', re.M)


def macro_argument(text: str) -> str:
    """`text` as one macro argument: in parentheses when it has a comma outside parentheses (a template's, which
    the preprocessor would split at)."""
    depth = 0
    for ch in text:
        depth += (ch == "(") - (ch == ")")
        if ch == "," and depth == 0:
            return f"({text})"
    return text


def functions_in_one_line(text: str) -> str:
    """`text` with each reflected function described in place written DREAD_REFLECTED_FUNCTION_OF (one line, when
    the description is all its registration does) or DREAD_REFLECTED_FUNCTION_DESCRIPTION at the head of its
    block (base/reflection/ReflectedFields.h); the macros expand to the same statements."""
    def one(m: re.Match) -> str:
        indent, name, ret, flags, invoker, close = m.groups()
        args = f"{name}, {macro_argument(ret.strip())}, {flags}, {invoker}"
        if close:
            return f"{indent}DREAD_REFLECTED_FUNCTION_OF({args});\n"
        return f"{indent}DREAD_REFLECTED_FUNCTION({name}) {{\n{indent}    DREAD_REFLECTED_FUNCTION_DESCRIPTION({args});\n"
    new = DESCRIPTION_RE.sub(one, text)
    new = INIT_DESCRIPTION_RE.sub(lambda m: f"{m.group(1)}DREAD_REFLECTED_FUNCTION_DESCRIPTION(name, "
                                             f"{macro_argument(m.group(2).strip())}, {m.group(3)}, {m.group(4)});\n", new)
    return with_include(new, '#include "base/reflection/ReflectedFields.h"') if new != text else text


# The same description in an out-of-line initialiser, `void fn_<addr>(CFunction* fn, const char* name)`.
INIT_DESCRIPTION_RE = re.compile(r'^(\s*)fn->name = base::global::UnkStrId\(name, true\);\n'
                                 r'\s*fn->return_type = ?([^;\n]+);\n'
                                 r'\s*fn->field_0xb2 = (\w+);\n'
                                 r'\s*fn->field_0x40 = nullptr;\n'
                                 r'\s*fn->invoker = reinterpret_cast<void\*>\(&(\w+)\);\n', re.M)


# An attribute key whose values are of its own enum type (base/reflection/ReflectedFields.h, TypedAttrKey).
TYPED_KEY_RE = re.compile(r"^inline TypedAttrKey (\w+)\(\) \{ return \{fn_([0-9a-f]{10})\(\), &(.+?)\}; \}$", re.M)


def typed_attribute_keys() -> dict[str, tuple[str, str]]:
    """{fn_<key getter>: (name, its type object as ReflectedFields.h spells it)} of the typed attribute keys."""
    text = (ROOT / "include" / "base" / "reflection" / "ReflectedFields.h").read_text(encoding="utf-8")
    return {f"fn_{a}": (name, obj) for name, a, obj in TYPED_KEY_RE.findall(text)}


# A reflected class's run-once guard (refl_AddFields' first statements) and its tunable object (registered in the
# global registry), both named in the class's namespace: refl_fields_added and refl_tunable.
GUARD_RE = re.compile(r"^    if \((g_unk_[0-9a-f]{10})\)\n        return;\n    \1 = true;$", re.M)
TUNABLE_USE_RE = re.compile(r"\(base::reflection::TunableRegistry\(\), &(g_unk_[0-9a-f]{10}), ")
NAMESPACE_RE = re.compile(r"^namespace ([\w:]+) \{$", re.M)


def namespace_at(lines: list[str], i: int) -> str | None:
    """The generated namespace block line `i` is in (`namespace X {` ... `}  // namespace`), or None."""
    scope = None
    for line in lines[:i]:
        m = NAMESPACE_RE.fullmatch(line)
        if m:
            scope = m.group(1)
        elif line.startswith("}  // namespace"):
            scope = None
    return scope


def class_statics_by_name(text: str) -> str:
    """`text` with each class's run-once guard and tunable object written by name, `refl_fields_added` and
    `refl_tunable`, declared in the class's namespace with the object's own label (asm("g_unk_<addr>")) in place of
    the file's `extern bool g_unk_<addr>;` / `extern UnkTunable g_unk_<addr>;`. An object named twice in one
    namespace, declared otherwise or used outside its namespace keeps its address."""
    lines = text.split(chr(10))
    found: dict[str, tuple[str, str, str]] = {}
    for regex, name, spelled in ((GUARD_RE, "refl_fields_added", "bool"), (TUNABLE_USE_RE, "refl_tunable", "UnkTunable")):
        for m in regex.finditer(text):
            scope = namespace_at(lines, text.count(chr(10), 0, m.start()))
            if scope:
                found.setdefault(m.group(1), (scope, name, spelled))
    taken = collections.Counter((scope, name) for scope, name, _ in found.values())
    for g, (scope, name, spelled) in found.items():
        decl = f"extern {spelled} {g};"
        if taken[(scope, name)] != 1 or lines.count(decl) != 1:
            continue
        uses = [i for i, line in enumerate(lines) if re.search(rf"\b{g}\b", line) and line != decl]
        if any(namespace_at(lines, i) != scope for i in uses):
            continue
        for i in uses:
            lines[i] = re.sub(rf"\b{g}\b", name, lines[i])
        lines[lines.index(decl)] = f'namespace {scope} {{ extern {spelled} {name} asm("{g}"); }}'
    return chr(10).join(lines)


def typed_keys_by_name(text: str, keys: dict[str, tuple[str, str]]) -> str:
    """`text` with each attribute added under a typed key with the key's own type, `Add<T>(fn_<key>(), &<type>,
    v)`, written `Add<T>(base::reflection::attr::<Name>(), v)` (the same key and type); the getter's declaration
    goes once nothing else names it."""
    for fn, (name, obj) in keys.items():
        old = f"({fn}(), &{obj}, "
        if old not in text:
            continue
        text = text.replace(old, f"(base::reflection::attr::{name}(), ")
        decl = f"const base::reflection::UnkReflAttrKey& {fn}();"
        lines = text.split(chr(10))
        if not any(re.search(rf"\b{fn}\b", line) for line in lines if line.strip() != decl):
            text = chr(10).join(line for line in lines if line.strip() != decl)
        text = with_include(text, '#include "base/reflection/ReflectedFields.h"')
    return text


def with_include(text: str, include: str) -> str:
    """`text` with `include` after its other includes, unless it has it."""
    lines = text.split(chr(10))
    if include not in lines:
        head = max((i + 1 for i, line in enumerate(lines[:40]) if line.startswith("#include")), default=0)
        lines.insert(head, include)
    return chr(10).join(lines)


def with_registry_include(text: str) -> str:
    """`text` with the tunable registry helper's header when it calls the helper (the final pass adds it)."""
    if "base::reflection::TunableRegistry()" in text:
        return with_include(text, '#include "base/reflection/UnkTunableRegistry.h"')
    return text


# Prelude names (refl_attrs.PRELUDE) only the CTunable* classes' field functions use: a file that names neither
# loses their `using` lines (the final pass).
TUNABLE_USINGS = ("UnkTunable", "UnkTunableRegistry")


def without_unused_usings(text: str) -> str:
    """`text` without the `using base::reflection::<name>;` lines of TUNABLE_USINGS it does not otherwise name."""
    lines = text.split(chr(10))
    for name in TUNABLE_USINGS:
        using = f"using base::reflection::{name};"
        if using in lines and not any(re.search(rf"\b{name}\b", line) for line in lines if line != using):
            lines.remove(using)
    return chr(10).join(lines)


# The declared-class form of a class block, as text (tools/dread/declared_class.py; tools/gen_classes.py
# uses it too, without this module's binary dependencies).
from dread.declared_class import CLASS_BLOCK, declared_class_block  # noqa: E402,F401

def with_declared_classes(text: str, includes: dict[str, str], sizes: dict[str, int],
                          offsets: dict[tuple[str, str], int]) -> str:
    """`text` with the blocks of the classes tools/gen_classes.py declares (`includes`: class -> header)
    written as the class's members, and the header included (the final pass)."""
    def one(m: re.Match) -> str:
        name, body, kind, parent = m.group(1), m.group(2), m.group(3), m.group(4)
        if name not in includes or (parent and parent not in includes):
            return m.group(0)
        members = declared_class_block(name, body, sizes, offsets)
        if members is None:
            return m.group(0)
        use = f"DREAD_REFLECTED_DECLARED_CLASS({name}, {parent});" if parent else f"DREAD_REFLECTED_DECLARED_ROOT_CLASS({name});"
        if "offsetof(" in members:
            # offsetof on a class with a virtual table: allowed for this class's definitions only
            # (base/reflection/ReflectedClass.h)
            return (f'#pragma GCC diagnostic push{chr(10)}#pragma GCC diagnostic ignored "-Winvalid-offsetof"{chr(10)}'
                    f"{members}{use}{chr(10)}#pragma GCC diagnostic pop")
        return members + use

    new = CLASS_BLOCK.sub(one, text)
    for name, include in includes.items():
        if new != text and re.search(rf"^DREAD_REFLECTED_DECLARED_(?:ROOT_)?CLASS\({re.escape(name)}[,)]", new, re.M):
            new = with_include(new, f'#include "{include}"')
    return new


# Prelude names only the namespace form's TypeOf and Destroy use: a file in the declared form that no longer names
# one loses its `using` line (the final pass).
DECLARED_FORM_USINGS = ("ClassOf", "ClassOfDestructorFirst", "Destroy", "DestroyDestructorFirst")


def without_unused_declared_usings(text: str) -> str:
    """`text` without the `using base::reflection::<name>;` lines of DECLARED_FORM_USINGS it does not otherwise
    name, when it writes a class in the declared form."""
    if not re.search(r"^DREAD_REFLECTED_DECLARED_", text, re.M):
        return text
    lines = text.split(chr(10))
    for name in DECLARED_FORM_USINGS:
        using = f"using base::reflection::{name};"
        if using in lines and not any(re.search(rf"\b{name}\b", line) for line in lines if line != using):
            lines.remove(using)
    return chr(10).join(lines)


def unconverted_classes(texts: list[str], includes: dict[str, str]) -> list[str]:
    """The declared classes (`includes`) whose getter no text writes in the declared form: their source kept
    the namespace form, which their header's class would clash with."""
    done = {m for text in texts for m in re.findall(r"^DREAD_REFLECTED_DECLARED_(?:ROOT_)?CLASS\(([\w:]+)", text, re.M)}
    return sorted(set(includes) - done)


def declared_classes() -> tuple[dict[str, str], dict[str, int], dict[tuple[str, str], int]]:
    """The arguments of with_declared_classes, from tools/gen_classes.py's headers and the class sizes."""
    import gen_classes

    types, _ = gen_classes.tables()
    return (gen_classes.declared_includes(), {t: int(r["size"], 16) for t, r in types.items() if r["size"]},
            gen_classes.offsets())


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()

    orig = asmdiff.Original(args.version)
    with (ROOT / "data" / "reflection_types.csv").open(newline="", encoding="utf-8") as f:
        rows = [r for r in csv.DictReader(f) if r["kind"] == "class"]
    getters = {int(r["getter"], 16) for r in rows}
    sizes = {int(r["getter"], 16): int(r["size"], 16) for r in rows if r["size"]}
    inits = set()
    for f in (ROOT / "data" / "functions").glob("main_*.csv"):
        with f.open(newline="", encoding="utf-8") as fh:
            inits |= {int(r["address"], 16) for r in csv.DictReader(fh) if r["name"].endswith("::refl_InitClass")}
    ref_sig = shape(orig.function(REFERENCE_GETTER))
    root_sig = shape(orig.function(ROOT_GETTER))
    local_sig = shape(orig.function(LOCAL_PARENT_GETTER))
    all_g = [Getter(r["type"], int(r["getter"], 16)) for r in rows]
    type_of = {g.address: g.type for g in all_g}
    by_address = {g.address: g for g in all_g}
    # the classes written in the namespace form, by getter (their fields of each other's type name them)
    class_types = {g.address: g.type for g in all_g
                   if readable_type(g.type) and (not g.parent or readable_type(type_of.get(g.parent, "")))}
    for g in all_g:
        analyse(orig, g, getters, ref_sig, inits, root_sig, local_sig)

    located = sorted((g for g in all_g if g.guard and g.descriptor), key=lambda g: g.guard)
    blocks: list[list[Getter]] = []
    for g in located:
        if blocks and g.guard - blocks[-1][-1].guard == 8:
            blocks[-1].append(g)
        else:
            blocks.append([g])

    candidates = {}
    for g in (g for b in blocks for g in b if g.fn_0x70 and g.fn_0xe0):
        for a in (g.fn_0x70, g.fn_0xe0):
            if a not in candidates and len(orig.function(a)) > 4:
                parsed = refl_attrs.parse(orig, a, getters)
                if parsed:
                    candidates[a] = parsed
    attr_ok = refl_attrs.verify(args.version, candidates, ROOT / "build" / "reflection_attrs")
    # Type objects static initialisers build, named by data/reflection_fields.csv for the fields they type
    # (ReflectedTypes.h); those another header already declares keep their address.
    with (ROOT / "data" / "reflection_fields.csv").open(newline="", encoding="utf-8") as f:
        field_types = {(r["class"], r["name"]): r["type"] for r in csv.DictReader(f)}
    declared_elsewhere = {int(a, 16) for h in (ROOT / "include").rglob("*.h")
                          for a in re.findall(r"\bg_unk_([0-9a-f]{10});", h.read_text(encoding="utf-8"))}
    type_objects = refl_attrs.type_object_names(
        {g.type: candidates[g.fn_0xe0][1] for g in all_g if g.address in class_types and g.fn_0xe0 in attr_ok},
        field_types, declared_elsewhere)
    # The same kind of functions outside the class getters (e.g. passed by static initialisers to
    # collection types): they don't depend on their TU either.
    attached = {a for b in blocks for g in b for a in (g.fn_0x70, g.fn_0xe0) if a}
    others = {}
    for a in registration_functions(orig):
        if a not in attached and a not in candidates:
            parsed = refl_attrs.parse(orig, a, getters)
            if parsed:
                others[a] = parsed
    others_ok = sorted(refl_attrs.verify(args.version, others, ROOT / "build" / "reflection_others") & set(others))
    # Out-of-line reflected-function initialisers (fn_X(fn, name)): also TU-independent.
    inits = {}
    for a in initialiser_candidates(orig):
        parsed = refl_attrs.parse_initialiser(orig, a, getters)
        if parsed:
            inits[a] = parsed
    inits_ok = refl_attrs.verify(args.version, inits, ROOT / "build" / "reflection_inits") & set(inits)
    # Descriptor constructors outside generated getter TUs depend only on their traits.
    # Padded TUs (layout_gap > 0) keep their constructors here too: their file falls back to the
    # skipped-TU form when the padding doesn't reproduce the layout.
    def block_ok(block: list[Getter]) -> bool:
        return layout_gap(block) == 0 and all(getter_ok(g, block) and g.fn_0x70 and g.fn_0xe0 and g.init and g.traits
                    and all(k in g.traits for k in (0x58, 0x60, 0x68)) and g.address in sizes for g in block)
    clean_inits = {g.init for b in blocks if block_ok(b) for g in b}
    ctors = {a: v for a, v in class_constructors(orig).items() if a not in clean_inits}
    helper_types: dict[int, str] = {}
    scratch = ["// Scratch: descriptor constructors outside generated getter TUs.", '#include "base/reflection/CClass.h"']
    for a, (size, abstract, traits) in ctors.items():
        if any(helper_types.setdefault(h, SLOT_TYPES[slot][0]) != SLOT_TYPES[slot][0] for slot, h in traits.items()):
            continue
        scratch += [f"DREAD_REFL_TAG({a:x});",
                    *(SLOT_TYPES[slot][1].format(name=f"fn_{h:x}") for slot, h in traits.items()),
                    *constructor_lines(a, size, abstract, traits)]
    work = ROOT / "build" / "reflection_ctors"
    work.mkdir(parents=True, exist_ok=True)
    (work / "candidates.cpp").write_text(chr(10).join(scratch) + chr(10), encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(work / "candidates.cpp", work / "candidates.o"), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"descriptor constructor candidates failed to compile:\n{r.stderr[:4000]}")
    # strict: a byte match whose relocations name the wrong callee, global or GOT slot is rejected
    ctor_results = asmdiff.diff_object(work / "candidates.o", orig, strict=True)
    ctor_ok = {res.address for res in ctor_results if res.matching} & set(ctors)
    for line in asmdiff.reloc_rejects([res for res in ctor_results if res.address in ctors]):
        print(line)
    attached_inits = {g.init for b in blocks for g in b if g.init}

    attr_total = len({a for b in blocks for g in b for a in (g.fn_0x70, g.fn_0xe0)
                      if a and len(orig.function(a)) > 4})

    # Types whose descriptor constructor uses a class of this TU (pointer types to it,
    # collections of it): (lines, addresses that must match, the class getter).
    _, local_ptrs, _, ptr_decls = gen_types.scan(orig)
    _, local_colls = gen_types.scan_collections(orig, ptr_decls)
    items_by_getter: dict[int, list] = {}
    for p in local_ptrs:
        items_by_getter.setdefault(p[4], []).append((gen_types.emit(*p[:4]), (p[0], p[2]), p[4]))
    for c in local_colls:
        items_by_getter.setdefault(c[2], []).append((gen_types.emit_collection(c[0], c[1]), (c[0],), c[2]))

    # Registration functions that call a class getter of a generated TU and use its descriptor's
    # address as a constant (Clang folds the call's result when the getter is in the same TU):
    # generated in that TU's file, as items checked like the local pointer types.
    descriptor_of = {g.address: g.descriptor for b in blocks for g in b}
    local_others: dict[int, list] = {}
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    for a in sorted(set(others) - set(others_ok)):
        page: dict[str, int] = {}
        consts, calls = set(), []
        for _, _, m, o in cs.disasm_lite(orig.function(a), a):
            ops = [x.strip() for x in o.split(",")]
            if m == "adrp":
                page[ops[0]] = int(ops[1][1:], 16)
            elif m == "add" and len(ops) == 3 and ops[1] in page and ops[2].startswith("#"):
                consts.add(page[ops[1]] + int(ops[2][1:], 16))
            elif m == "bl":
                calls.append(int(ops[0][1:], 16))
        for t in calls:
            if t in descriptor_of and descriptor_of[t] in consts:
                extra, src = others[a]
                local_others.setdefault(t, []).append((refl_attrs.PRELUDE + extra + [src], (a,), t, "other"))
                break

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    with TYPES_HEADER.open("w", encoding="utf-8", newline="\n") as f:
        f.write(refl_attrs.type_objects_header(type_objects))
    files: dict[str, tuple[list[str], list]] = {}  # name -> (lines, local pointer pairs)
    file_getters: dict[str, dict[int, str]] = {}  # name -> its readable classes' getters (address -> type)
    taken: set[str] = set()
    generated = skipped_shape = skipped_layout = helpers_defined = attrs_defined = ctors_generated = 0
    report = []
    # TUs verified by compiling them (padded, or with an inlined name): name -> (block, fallback lines)
    padded: dict[str, tuple[list[Getter], list[str] | None]] = {}

    def partial(block: list[Getter]) -> list[str] | None:
        """A skipped TU's file: the getters wait, but the TU's helpers and attribute/field
        functions don't depend on its statics, so those known to match are generated."""
        nonlocal helpers_defined, attrs_defined, ctors_generated
        n = len(block)
        lines = [
            "// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
            f"// Part of one original translation unit ({n} type{'s' if n > 1 else ''}) whose getters aren't",
            "// generated yet (see build/reflection_tus.json): the per-type helpers with a known uniform",
            "// shape, the attribute/member-field functions that tools/refl_attrs.py reconstructs",
            "// byte-identically and the descriptor constructors (T::refl_InitClass) that match.",
            '#include "base/reflection/CClass.h"',
        ]
        if any(a in attr_ok for g in block for a in (g.fn_0x70, g.fn_0xe0)):
            lines += refl_attrs.PRELUDE
        lines.append("")
        declared = set()
        decl_lines = set()
        body = 0
        for g in block:
            helpers = sorted([(a, slot) for slot, a in (g.traits or {}).items()]
                             + [(a, slot) for a, slot in ((g.fn_0x70, 0x70), (g.fn_0xe0, 0xe0)) if a],
                             key=lambda h: h[0])
            defs = []
            for a, slot in helpers:
                if a in declared:
                    continue
                declared.add(a)
                src = helper_definition(orig, slot, a)
                if a in attr_ok:
                    extra, src = candidates[a]
                    defs += [d for d in extra if d not in decl_lines]
                    decl_lines |= set(extra)
                    attrs_defined += 1
                elif src:
                    helpers_defined += 1
                if src:
                    defs.append(src)
                elif slot in SLOT_TYPES and g.init in ctor_ok:
                    defs.append(SLOT_TYPES[slot][1].format(name=f"fn_{a:x}"))
            if g.init in ctor_ok:
                defs += [f"DREAD_REFL_TAG({g.init:x});", *constructor_lines(g.init, *ctors[g.init])]
                ctors_generated += 1
            if defs:
                lines += [f"// {g.type}"] + defs + [""]
                body += 1
        return lines if body else None

    for block in blocks:
        n = len(block)
        gap = layout_gap(block)
        shape_ok = all(getter_ok(g, block) and g.fn_0x70 and g.fn_0xe0 and g.init and g.traits
                       and all(k in g.traits for k in (0x58, 0x60, 0x68)) and g.address in sizes
                       for g in block)
        if gap is None or not shape_ok:
            if gap is None:
                skipped_layout += n
                report.append({"tu": [g.type for g in block], "reason": "other statics in the TU"})
            else:
                skipped_shape += n
                report.append({"tu": [g.type for g in block], "reason": "getter code differs from the common shape"})
            lines = partial(block)
            if lines:
                files[file_name(block[0].type, taken)] = (lines, [])
            continue
        name = file_name(block[0].type, taken)
        if gap or any(g.inline_name for g in block):
            padded[name] = (block, None)
        lines = [
            "// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
            f"// Reflection descriptors for one original translation unit ({n} type{'s' if n > 1 else ''}):",
            "// for each type, its getter (T::refl_GetClass) and, through the traits below, its",
            "// descriptor constructor (T::refl_InitClass), plus the per-type helpers it points to",
            "// that have a known uniform shape and the attribute/member-field functions that",
            "// tools/refl_attrs.py reconstructs byte-identically.",
            '#include "base/reflection/CClass.h"',
        ]
        if any(a in attr_ok for g in block for a in (g.fn_0x70, g.fn_0xe0)):
            lines += refl_attrs.PRELUDE
        lines.append("")
        declared: set[int] = set()
        decl_lines: set[str] = set()
        readable = [g for g in block if readable_type(g.type) and (not g.parent or readable_type(type_of.get(g.parent, "")))]
        if readable:
            lines[2:6] = READABLE_HEADER
            lines[lines.index('#include "base/reflection/CClass.h"')] = '#include "base/reflection/ReflectedClass.h"'
            lines[-1:] = [u for u in READABLE_PRELUDE if u not in lines] + [""]
        for g in block:
            if g in readable:
                suffix = "_INLINE_NAME" if g.inline_name else ""  # ReflectedClass.h
                macro = f"DREAD_REFLECTED_CLASS{suffix}" if g.parent else f"DREAD_REFLECTED_ROOT_CLASS{suffix}"
                body, h, n_attrs = readable_class_lines(
                    orig, g, sizes[g.address], {a: candidates[a] for a in attr_ok}, macro,
                    type_of[g.parent] if g.parent else None, decl_lines, class_types, type_objects)
                lines += body + [""]
                fields_include = '#include "base/reflection/ReflectedFields.h"'
                if fields_include not in lines and any(FIELD_MACRO_RE.search(l) for l in body):
                    lines.insert(lines.index('#include "base/reflection/ReflectedClass.h"') + 1, fields_include)
                types_include = '#include "ReflectedTypes.h"'  # src/reflection/, for DREAD_REFLECTED_FIELD_OF
                if types_include not in lines and any("DREAD_REFLECTED_FIELD_OF(" in l for l in body):
                    lines.insert(lines.index(fields_include) + 1, types_include)
                helpers_defined += h
                attrs_defined += n_attrs
                continue
            lines.append(f"// {g.type}")
            lines.append(f"DREAD_REFL_TAG({g.init:x});")
            helpers = sorted([(a, slot) for slot, a in g.traits.items()]
                             + [(g.fn_0x70, 0x70), (g.fn_0xe0, 0xe0)], key=lambda h: h[0])
            for a, slot in helpers:
                if a in declared:
                    continue
                declared.add(a)
                src = helper_definition(orig, slot, a)
                if a in attr_ok:
                    extra, src = candidates[a]
                    lines += [d for d in extra if d not in decl_lines]
                    decl_lines |= set(extra)
                    attrs_defined += 1
                if src:
                    lines.append(src)
                    helpers_defined += 1
                elif slot in SLOT_TYPES:
                    lines.append(SLOT_TYPES[slot][1].format(name=f"fn_{a:x}"))
            lines.append("template <>")
            lines.append(f"struct base::reflection::UnkReflClassTraits<UnkRefl_fn_{g.init:x}> {{")
            lines.append(f"    static constexpr u64 size = {sizes[g.address]:#x};")
            lines.append(f"    static constexpr bool abstract = {'true' if g.abstract else 'false'};")
            for slot in TRAIT_SLOTS:
                value = f"&fn_{g.traits[slot]:x}" if slot in g.traits else "nullptr"
                lines.append(f"    static constexpr {SLOT_TYPES[slot][0]} fn_{slot:#x} = {value};")
            lines.append("};")
            suffix = "_INLINE_NAME" if g.inline_name else ""  # CClass.h
            if g.parent:
                lines.append(f'DREAD_REFL_CLASS_GETTER{suffix}({g.address:x}, {g.init:x}, "{g.type}", {g.parent:x}, '
                             f'{g.fn_0x70:x}, {g.fn_0xe0:x});')
            else:
                lines.append(f'DREAD_REFL_ROOT_CLASS_GETTER{suffix}({g.address:x}, {g.init:x}, "{g.type}", '
                             f'{g.fn_0x70:x}, {g.fn_0xe0:x});')
            lines.append("")
        if gap:
            lines += padding_lines(block, gap)
        ptrs = [p for g in block for p in items_by_getter.get(g.address, []) + local_others.get(g.address, [])]
        files[name] = (lines, ptrs)
        file_getters[name] = {g.address: g.type for g in readable}
        generated += n

    def text(lines: list[str], ptrs: list, getters: dict[int, str] | None = None) -> str:
        if not ptrs:
            return chr(10).join(lines)
        types = [p for p in ptrs if len(p) == 3]
        body = [line for p in types for line in p[0]]
        pointees = {f"fn_{p[2]:x}" for p in types}
        decls = [d for d in gen_types.used_decls(body, ptr_decls)
                 if gen_types.NAME_RE.search(d.split("(")[0]).group(0) not in pointees]
        tail = decls + body + [line for p in ptrs if len(p) > 3 for line in p[0]]
        # The file's readable classes: their getters are T::refl_GetClass, declared by the class's macro.
        for a, type_name in (getters or {}).items():
            tail = [re.sub(rf"\bfn_{a:x}\(\)", f"{type_name}::refl_GetClass()", line) for line in tail
                    if line != f"const base::reflection::CClass* fn_{a:x}();"]
        return chr(10).join(lines[:1] + ['#include "base/reflection/CCollectionType.h"',
                                         '#include "base/reflection/CPointerType.h"'] + lines[1:]
                            + tail) + chr(10)

    # Pointer types: drop those that would redeclare a name the file already uses (unless the
    # declaration is the same text as one in the file or a header), then keep only the
    # byte-identical ones.
    header_text = chr(10).join(h.read_text(encoding="utf-8") for h in (ROOT / "include").rglob("*.h"))
    for name, (lines, ptrs) in files.items():
        text_ = chr(10).join(lines)
        names = set(gen_types.NAME_RE.findall(text_))
        files[name] = (lines, [p for p in ptrs if len(p) > 3 or not any(
            gen_types.NAME_RE.search(d.split("(")[0]).group(0) in names - {f"fn_{p[2]:x}"}
            and d not in text_ and d not in header_text
            for d in gen_types.used_decls(p[0], ptr_decls))])
    work = ROOT / "build" / "reflection_ptrs"
    work.mkdir(parents=True, exist_ok=True)
    shutil.copyfile(TYPES_HEADER, work / TYPES_HEADER.name)  # the files there include it as their neighbour
    # Qualified name -> address of the readable classes' functions (the names data/functions/ gives them).
    readable_names: dict[str, int] = {}
    for getters_of_file in file_getters.values():
        for a, type_name in getters_of_file.items():
            g = by_address[a]
            names = {**reflection_names(g), a: f"{type_name}::refl_GetClass", g.init: f"{type_name}::refl_InitClass"}
            for addr, n in names.items():
                for key in [n, *asmdiff.name_aliases(n)]:
                    readable_names[key] = addr

    def checked_text(name: str) -> str:
        # The include the final pass adds to a file that calls the tunable registry's helper.
        return with_registry_include(text(*files[name], file_getters.get(name)))

    def check_file(name: str) -> set[int]:
        src, obj = work / f"{name}.cpp", work / f"{name}.o"
        src.write_text(checked_text(name), encoding="utf-8")
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0 and any(len(p) > 3 for p in files[name][1]):
            # Retry without the registration functions (their declarations may clash).
            files[name] = (files[name][0], [p for p in files[name][1] if len(p) == 3])
            src.write_text(checked_text(name), encoding="utf-8")
            r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0:
            return set()
        # bytes and relocation targets; the readable classes' functions map through their names
        functions = asmdiff.read_object(obj)
        demangled = asmdiff.demangle([f.symbol for f in functions], strip=False)

        def resolve(symbol: str) -> int | None:
            return readable_names.get(asmdiff.strip_params(demangled.get(symbol, symbol)))
        results = asmdiff.diff_functions(functions, orig, resolve, strict=True)
        for line in asmdiff.reloc_rejects(results, limit=3):
            print(f"{name}: {line}")
        return {res.address for res in results if res.matching}

    with_ptrs = [n for n, (_, ptrs) in files.items() if ptrs or n in padded]
    with ThreadPoolExecutor() as pool:
        matched = dict(zip(with_ptrs, pool.map(check_file, with_ptrs)))
    # Verified TUs whose getters or constructors don't all match fall back to the skipped-TU form.
    padded_ok = 0
    for name, (block, _) in padded.items():
        if all(g.address in matched[name] and g.init in matched[name] for g in block):
            padded_ok += len(block) if layout_gap(block) else 0
            continue
        generated -= len(block)
        skipped_layout += len(block)
        report.append({"tu": [g.type for g in block], "reason": "verified TU didn't match (padding or inlined name)"})
        lines = partial(block)
        if lines:
            files[name] = (lines, [])
            matched[name] = set()
        else:
            del files[name]
    ptrs_generated = others_local = 0
    for name, (lines, ptrs) in files.items():
        ok = matched.get(name, set())
        ptrs = [p for p in ptrs if all(a in ok for a in p[1])]
        ptrs_generated += sum(1 for p in ptrs if len(p) == 3)
        others_local += sum(1 for p in ptrs if len(p) > 3)
        (OUT / f"{name}.cpp").write_text(text(lines, ptrs, file_getters.get(name)), encoding="utf-8", newline=chr(10))

    loose = sorted(a for a in ctor_ok if a not in attached_inits)
    # Loose constructors whose class has a name (loose_class_names), in the namespace form, where
    # data/functions/ gives the constructor and its functions that class's names and the form compiles
    # to the same bytes; the others keep the tag form.
    fn_names: dict[int, str] = {}
    for f in (ROOT / "data" / "functions").glob("main_*.csv"):
        with f.open(newline="", encoding="utf-8") as fh:
            fn_names.update((int(r["address"], 16), r["name"]) for r in csv.DictReader(fh))
    class_of_init: dict[int, str] = {}
    for a in loose:
        n = fn_names.get(a, "")
        if n.endswith("::refl_InitClass"):
            t = n[:-len("::refl_InitClass")]
            if (loose_readable(t) or template_value_kind(t)) \
                    and all(fn_names.get(h) == f"{t}::{role_name(slot)}" for slot, h in ctors[a][2].items()):
                class_of_init[a] = t
    named_head = ['#include "base/reflection/CClass.h"', '#include "base/reflection/ReflectedClass.h"', "",
                  *READABLE_PRELUDE, ""]
    work = ROOT / "build" / "reflection_loose"
    work.mkdir(parents=True, exist_ok=True)

    def check_loose(a: int) -> bool:
        t = class_of_init[a]
        src, obj = work / f"{a:x}.cpp", work / f"{a:x}.o"
        if template_value_kind(t):
            body, decls, _ = template_value_lines(orig, t, *ctors[a])
            text = [*TEMPLATE_VALUE_FILES[template_value_kind(t)][0], *decls, "", *body]
        else:
            text = [*named_head[:2], *member_includes([t]), *named_head[2:], *loose_class_lines(orig, t, *ctors[a])]
        src.write_text(chr(10).join(text) + chr(10), encoding="utf-8")
        if subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True).returncode != 0:
            return False
        names = {f"{t}::{role_name(slot)}": h for slot, h in ctors[a][2].items()}
        names.update((k, a) for k in [f"{t}::refl_InitClass", *asmdiff.name_aliases(f"{t}::refl_InitClass")])
        functions = asmdiff.read_object(obj)
        demangled = asmdiff.demangle([f.symbol for f in functions], strip=False)
        results = asmdiff.diff_functions(functions, orig, lambda sym: names.get(asmdiff.strip_params(demangled.get(sym, sym))),
                                         strict=True)
        return any(r.address == a for r in results) and all(r.matching for r in results)

    with ThreadPoolExecutor() as pool:
        named_ok = {a for a, ok in zip(list(class_of_init), pool.map(check_loose, list(class_of_init))) if ok}
    # Template instances (key-element pairs, VALUE_TEMPLATES) go to files of their own by kind, each with the
    # declarations its types need; one whose declarations would collide with a file's starts the next.
    pairs = sorted(a for a in named_ok if template_value_kind(class_of_init[a]))
    by_kind: dict[str, list[list[tuple[list[str], list[str], list]]]] = {k: [] for k in TEMPLATE_VALUE_FILES}
    for a in pairs:
        item = template_value_lines(orig, class_of_init[a], *ctors[a])
        files_of_kind = by_kind[template_value_kind(class_of_init[a])]
        if not files_of_kind or len(files_of_kind[-1]) >= REGISTRATION_CHUNK \
                or gen_types.declaration_clashes([n for _, _, ns in files_of_kind[-1] for n in ns] + item[2]):
            files_of_kind.append([])
        files_of_kind[-1].append(item)
    for kind, groups in by_kind.items():
        for f in OUT.glob(f"{kind}_*.cpp"):
            f.unlink()
        head, about = TEMPLATE_VALUE_FILES[kind]
        for k, group in enumerate(groups):
            decls = sorted({d for _, ds, _ in group for d in ds})
            lines = ["// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
                     f"// {about}", "// byte-identical to the original.", *head, "", *decls, ""]
            for body, _, _ in group:
                lines += body + [""]
                ctors_generated += 1
            (OUT / f"{kind}_{k:02d}.cpp").write_text(chr(10).join(lines), encoding="utf-8", newline=chr(10))
    loose = [a for a in loose if a not in pairs]
    for k in range(0, len(loose), REGISTRATION_CHUNK):
        chunk = loose[k:k + REGISTRATION_CHUNK]
        lines = ["// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
                 "// Descriptor constructors (CClass) of types without a located class getter (most are",
                 "// built by static initialisers), with the per-type helpers of a known uniform shape;",
                 "// byte-identical to the original. A class whose static initialiser names it is in the",
                 "// namespace form (base/reflection/ReflectedClass.h), the others are keyed by constructor.",
                 *(named_head[:2] + member_includes([class_of_init[a] for a in chunk if a in named_ok]) + named_head[2:]
                   if any(a in named_ok for a in chunk) else ['#include "base/reflection/CClass.h"', ""])]
        declared: set[int] = set()
        for a in chunk:
            size, abstract, traits = ctors[a]
            if a in named_ok:
                lines += loose_class_lines(orig, class_of_init[a], size, abstract, traits) + [""]
                ctors_generated += 1
                continue
            lines.append(f"DREAD_REFL_TAG({a:x});")
            for slot, h in sorted(traits.items(), key=lambda t: t[1]):
                if h not in declared:
                    declared.add(h)
                    lines.append(helper_definition(orig, slot, h) or SLOT_TYPES[slot][1].format(name=f"fn_{h:x}"))
            lines += constructor_lines(a, size, abstract, traits) + [""]
            ctors_generated += 1
        (OUT / f"class_descriptors_{k // REGISTRATION_CHUNK:02d}.cpp").write_text(chr(10).join(lines), encoding="utf-8",
                                                                                newline=chr(10))
    print(f"loose descriptor constructors: {len(named_ok) - len(pairs)} of {len(loose)} in the namespace form, "
          f"{len(pairs)} template instances by type ({len(class_of_init)} named by data/functions/)")
    write_chunks("registration", ["// Attribute/member-field registration functions not reached from a class getter",
                                  "// (tools/refl_attrs.py), byte-identical to the original."],
                 {a: others[a] for a in others_ok})
    write_chunks("function_inits", ["// Out-of-line initialisers of reflected functions (tools/refl_attrs.py,",
                                    "// parse_initialiser), byte-identical to the original."],
                 {a: inits[a] for a in inits_ok})

    keyed = keyed_getters()
    keys = attribute_keys()
    by_type = 0
    for f in sorted(OUT.glob("*.cpp")):
        old = f.read_text(encoding="utf-8")
        new = functions_by_macro(attribute_keys_by_name(types_by_type(old, *keyed), keys))
        if new != old:
            by_type += 1
            f.write_text(new, encoding="utf-8", newline=chr(10))
    # Type objects by type: each file that changes is compiled first, and keeps its addresses if it doesn't build
    # (a declaration of T that collides with one of the file's own).
    # Collections' objects are tried first with the rest; a file their declarations don't build with keeps the rest.
    named = enum_type_objects()
    collections_named = {**collection_type_objects(), **named}
    built = built_type_objects(args.version, [f.read_text(encoding="utf-8") for f in OUT.glob("*.cpp")], set(collections_named))
    # Tried in turn: a file whose declarations of T collide with its own keeps the narrower set.
    tiers = [{**built, **collections_named}, collections_named, named]
    work = ROOT / "build" / "reflection_type_objects"
    work.mkdir(parents=True, exist_ok=True)

    def builds(f: Path, text: str) -> bool:
        trial = f.with_name(f"{f.stem}.type_objects.cpp")
        trial.write_text(with_registry_include(text), encoding="utf-8", newline=chr(10))
        try:
            return subprocess.run(buildcfg.cxx_command(trial, work / f"{f.stem}.o"), capture_output=True, text=True).returncode == 0
        finally:
            trial.unlink()

    def by_object_type(f: Path) -> bool:
        old = f.read_text(encoding="utf-8")
        new = type_objects_in_tiers(old, tiers, built, lambda text: builds(f, text))
        if new != old:
            f.write_text(new, encoding="utf-8", newline=chr(10))
        return new != old
    with ThreadPoolExecutor() as pool:
        objects_by_type = sum(pool.map(by_object_type, sorted(OUT.glob("*.cpp"))))
    # Reflected functions in one line, typed attribute keys by name: textual, so each file compiles as it did.
    typed_keys = typed_attribute_keys()
    # Classes tools/gen_classes.py declares: their blocks as the class's members.
    declared = declared_classes()
    for f in sorted(OUT.glob("*.cpp")):
        old = f.read_text(encoding="utf-8")
        new = without_unused_usings(with_registry_include(class_statics_by_name(functions_in_one_line(typed_keys_by_name(old, typed_keys)))))
        new = without_unused_declared_usings(with_declared_classes(new, *declared))
        if new != old:
            f.write_text(new, encoding="utf-8", newline=chr(10))
    missed = unconverted_classes([f.read_text(encoding="utf-8") for f in OUT.glob("*.cpp")], declared[0])
    if missed:
        print(f"error: {len(missed)} classes tools/gen_classes.py declares kept the namespace form in "
              f"src/reflection/ (a line the pass has no member form for, a destructor-first table, or an offset "
              f"or size the header does not assert): {', '.join(missed)}", file=sys.stderr)
        return 1
    (ROOT / "build" / "reflection_tus.json").write_text(json.dumps(report, indent=1), encoding="utf-8")
    print(f"{len(rows)} class getters; {len(located)} located; {len(blocks)} TUs. Generated {generated} getters "
          f"and {helpers_defined} helpers ({attrs_defined} of {attr_total} non-empty attribute/field functions) in {len(list(OUT.glob('*.cpp')))} files; {padded_ok} of the getters in padded TUs; skipped {skipped_layout} (other statics) and "
          f"{skipped_shape} (different code shape); {ptrs_generated} of {len(local_ptrs) + len(local_colls)} pointer/collection "
          f"types of a local class; {len(others_ok)} of {len(others)} other registration functions (and {others_local} in their getter's TU); "
          f"{len(inits_ok)} of {len(inits)} function initialisers; "
          f"{ctors_generated} of {len(ctors)} descriptor constructors outside generated getter TUs; "
          f"type getters, attribute keys and reflected functions written by name in {by_type} files, type objects by type in {objects_by_type}. Skipped TUs: build/reflection_tus.json")
    return 0


if __name__ == "__main__":
    sys.exit(main())
