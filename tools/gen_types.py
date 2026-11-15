#!/usr/bin/env python3
"""Generate src/reflection_types/: descriptors of non-class types.

Pointer types (CPointerType, include/base/reflection/CPointerType.h): getters
(refl_GetType<T>) share one code shape, a guarded static descriptor built from the type name
by the type's descriptor constructor, with its destructor registered at exit.
Enums (CEnumType, include/base/reflection/CEnumType.h): only the descriptor constructors;
the descriptors themselves are globals built by each TU's static initialiser.

  1. Candidates are the functions whose instruction sequence (mnemonics only) equals that of a
     verified reference: the getter REF_GETTER and the enum constructor REF_ENUM (pointer
     constructors are taken from the getters). Reference shapes are read from the binary at
     run time; nothing derived from code is stored here.
  2. Each constructor's stores are emulated to recover the kind, size, the type-specific
     function pointers and the kind's own fields; the getter gives the type name and its
     constructor.
  3. All candidates are compiled in one scratch translation unit; only byte-identical
     functions are written out (for pointer types, only pairs where both match).

Usage: python tools/gen_types.py --version 2.1.0
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

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import refl_attrs  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "reflection_types"
BASE = 0x7100000000
REF_GETTER = 0x71000081E8          # refl_GetType<char const*>
KIND_HASH = 0x7100001570
CSTRID_CTOR = 0x71000003D4
ALLOCATOR_GETTER = 0x7100083990
POINTER_KIND = "base::reflection::CPointerType"
ENUM_KIND = "base::reflection::CEnumType"
REF_ENUM = 0x71000145E8         # a CEnumType constructor
CHUNK = 200  # getter/constructor pairs per generated file

# CType/CPointerType slots filled by the constructor: offset -> description member (the roles
# ReflectedClass.h's ClassDescription names, then CPointerType's own).
SLOTS = {0x28: "construct", 0x30: "copy_construct", 0x38: "move_construct", 0x40: "destroy",
         0x48: "copy_assign", 0x50: "move_assign", 0x58: "equals", 0x60: "hash", 0x68: "type_of",
         0xF0: "get", 0xF8: "set", 0x100: "increment"}
# CCollectionType's accessors from +0x108 (CCollectionType.h).
COLLECTION_SLOTS = ["count", "clear", "add_copy", "add_move", "erase", "at", "find", "set_copy",
                    "set_move", "begin", "end", "const_begin", "const_end"]

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
CS.detail = True


def disasm(orig: asmdiff.Original, addr: int) -> list:
    return list(CS.disasm(orig.function(addr), addr))


def shape(orig: asmdiff.Original, addr: int) -> list[str]:
    return [x.mnemonic for x in disasm(orig, addr)]


def reg(x, op) -> str:
    """Register name, with w<n> folded into x<n> (a 32-bit write zero-extends)."""
    name = CS.reg_name(op.reg)
    return "x" + name[1:] if name[0] == "w" else name


def emulate(orig: asmdiff.Original, addr: int, base: str = "x19") -> tuple[str | None, dict, int | None]:
    """A descriptor constructor's kind string, its stores to the object and its tail call.

    Stores are keyed by offset from the object (`base`, including pre-index writebacks):
    off -> ('fn'|'data'|'call'|'imm'|'arg', value).
    """
    page: dict[str, int] = {}
    val: dict[str, tuple] = {"xzr": ("imm", 0), "x1": ("arg", 1), "x2": ("arg", 2), "x3": ("arg", 3)}
    stores: dict[int, tuple] = {}
    kind = tail = None
    delta = 0  # x19 - object, after pre-index writebacks
    for x in disasm(orig, addr):
        m, o = x.mnemonic, x.operands
        if m == "adrp":
            page[reg(x, o[0])] = o[1].imm
            val.pop(reg(x, o[0]), None)
            continue
        if m == "add" and len(o) == 3 and o[2].type == capstone.arm64.ARM64_OP_IMM and reg(x, o[1]) in page:
            a = page[reg(x, o[1])] + o[2].imm
            s = orig.img.cstring(a - BASE)
            val[reg(x, o[0])] = ("str", s) if s is not None else ("data", a)
            continue
        if m == "ldr" and o[1].type == capstone.arm64.ARM64_OP_MEM and CS.reg_name(o[1].mem.base) in page:
            target = orig.img.pointer(page[CS.reg_name(o[1].mem.base)] + o[1].mem.disp - BASE)
            if target is not None:
                t = BASE + target
                # Loaded through the GOT: marked, since a TU's own statics are addressed directly.
                val[reg(x, o[0])] = ("fn", t) if t in orig.sizes else ("data", t, "got")
            continue
        if m in ("mov", "movz") and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_IMM:
            val[reg(x, o[0])] = ("imm", o[1].imm & (2**64 - 1))
            continue
        if m == "mov" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_REG and reg(x, o[0]) != base:
            src = val.get(reg(x, o[1]))
            if src:
                val[reg(x, o[0])] = src
            else:
                val.pop(reg(x, o[0]), None)
            continue
        if m == "bl":
            if o[0].imm == KIND_HASH:
                kind = val.get("x0", ("", None))[1]
            val["x0"] = ("call", o[0].imm)
            for r in [f"x{i}" for i in range(1, 19)]:
                val.pop(r, None)
            continue
        if m == "b" and o[0].imm not in range(addr, addr + orig.sizes[addr]):
            tail = o[0].imm
            continue
        if m in ("str", "stp", "strb") and CS.reg_name(o[-1].mem.base) == base:
            regs = [reg(x, op) for op in o[:-1]]
            raw = CS.reg_name(o[0].reg)
            size = 8 if raw[0] in "xd" else 4
            for k, r in enumerate(regs):
                stores[delta + o[-1].mem.disp + k * size] = val.get(r, ("?", r))
            if x.writeback:
                delta += o[-1].mem.disp
            continue
        for r in [CS.reg_name(r) for r in x.regs_access()[1]]:
            r = "x" + r[1:] if r[0] == "w" else r
            val.pop(r, None)
            page.pop(r, None)
    return kind, stores, tail


def getter_facts(orig: asmdiff.Original, addr: int) -> tuple[str, int] | None:
    """(type name, constructor address) from a getter with the reference shape."""
    name = ctor = None
    page: dict[str, int] = {}
    strings: dict[str, str] = {}
    seen_name = False
    for x in disasm(orig, addr):
        m, o = x.mnemonic, x.operands
        if m == "adrp":
            page[reg(x, o[0])] = o[1].imm
        elif m == "add" and len(o) == 3 and reg(x, o[1]) in page and o[2].type == capstone.arm64.ARM64_OP_IMM:
            s = orig.img.cstring(page[reg(x, o[1])] + o[2].imm - BASE)
            if s is not None:
                strings[reg(x, o[0])] = s
        elif m == "bl":
            if o[0].imm == CSTRID_CTOR and not seen_name:
                name, seen_name = strings.get("x1"), True
            elif seen_name and ctor is None:
                ctor = o[0].imm
    return (name, ctor) if name and ctor else None


def local_pointee(orig: asmdiff.Original, init: int) -> int | None:
    """The pointee getter of a constructor that calls it but stores a constant address.

    When the pointee's getter is defined in the same TU, Clang replaces the call's result by
    the address of the getter's static descriptor; the call stays for its side effect."""
    kind, st, _ = emulate(orig, init)
    p = st.get(0xE8)
    if kind != POINTER_KIND or not p or p[0] != "data":
        return None
    calls = [x.operands[0].imm for x in disasm(orig, init) if x.mnemonic == "bl"]
    calls = [c for c in calls if c not in (KIND_HASH, ALLOCATOR_GETTER)]
    return calls[-1] if calls else None


def traits(orig: asmdiff.Original, init: int, decls: set[str], local: int | None = None,
           base: str = "x19") -> list[str] | None:
    """Traits lines; with `local`, the pointee is that getter, defined in the same TU. `base` is
    the object's register (another one when the constructor is inlined into its getter)."""
    kind, st, _ = emulate(orig, init, base)
    if kind != POINTER_KIND:
        return None
    size = st.get(0x18)
    if not size or size[0] != "imm":
        return None
    flag = st.get(0xE0)
    if not flag or flag[0] != "imm" or flag[1] not in (0, 1):
        return None
    lines = [f"    static constexpr u64 size = {size[1]:#x};",
             f"    static constexpr bool owned = {'true' if flag[1] else 'false'};"]
    for off, member in SLOTS.items():
        v = st.get(off)
        if v == ("imm", 0):
            lines.append(f"    static constexpr UnkFn {member} = nullptr;")
        elif v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            lines.append(f"    static constexpr UnkFn {member} = &fn_{v[1]:x};")
        else:
            return None
    p = st.get(0xE8)
    if local is not None:
        lines.append(f"    static const CType* pointee() {{ return fn_{local:x}(); }}")
    elif p and p[0] == "call":
        decls.add(f"const base::reflection::CType* fn_{p[1]:x}();")
        lines.append(f"    static const CType* pointee() {{ return fn_{p[1]:x}(); }}")
    elif p and p[0] == "data":
        decls.add(f"extern base::reflection::CType g_unk_{p[1]:x};")
        lines.append(f"    static const CType* pointee() {{ return &g_unk_{p[1]:x}; }}")
    else:
        return None
    return lines


def emit(getter: int, name: str, init: int, body: list[str]) -> list[str]:
    return [f"struct UnkRefl_fn_{init:x};",
            f"struct UnkRefl_fn_{getter:x};",
            "template <>",
            f"struct base::reflection::PointerTypeDescription<UnkRefl_fn_{init:x}> {{",
            *body,
            "};",
            "template <>",
            f"struct base::reflection::UnkReflTypeGetter<UnkRefl_fn_{getter:x}> {{",
            f"    static constexpr const char* name = {refl_attrs.c_string(name)};",
            f"    using Init = UnkRefl_fn_{init:x};",
            "};",
            f"template base::reflection::CPointerType::CPointerType(const base::global::CStrId&, "
            f"UnkRefl_fn_{init:x}*, void*);",
            f"template const base::reflection::CType* "
            f"base::reflection::refl_GetPointerType<UnkRefl_fn_{getter:x}>();"]


def enum_traits(orig: asmdiff.Original, init: int, decls: set[str]) -> list[str] | None:
    kind, st, tail = emulate(orig, init)
    if kind != ENUM_KIND or tail is None or st.get(0xE8) != ("arg", 2):
        return None
    size, under = st.get(0x18), st.get(0xE0)
    if not size or size[0] != "imm" or not under or under[0] != "call":
        return None
    lines = [f"    static constexpr u64 size = {size[1]:#x};"]
    for off in (0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68):
        v = st.get(off)
        if v == ("imm", 0):
            lines.append(f"    static constexpr UnkFn {SLOTS[off]} = nullptr;")
        elif v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            lines.append(f"    static constexpr UnkFn {SLOTS[off]} = &fn_{v[1]:x};")
        else:
            return None
    decls.add(f"const base::reflection::CType* fn_{under[1]:x}();")
    decls.add(f"void fn_{tail:x}(void* values, u32 count);")
    lines += [f"    static const CType* underlying() {{ return fn_{under[1]:x}(); }}",
              f"    static void reserve(void* values, u32 count) {{ fn_{tail:x}(values, count); }}"]
    return lines


def emit_enum(init: int, body: list[str]) -> list[str]:
    return [f"struct UnkRefl_fn_{init:x};",
            "template <>",
            f"struct base::reflection::EnumTypeDescription<UnkRefl_fn_{init:x}> {{",
            *body,
            "};",
            f"template base::reflection::CEnumType::CEnumType(const base::global::CStrId&, const void*, "
            f"UnkRefl_fn_{init:x}*);"]


# An enum's value operations (include/base/reflection/ValueOps.h): slot offset -> ValueOps member.
VALUE_OPS = {0x28: "Construct", 0x30: "CopyConstruct", 0x38: "MoveConstruct", 0x40: "Destroy",
             0x48: "CopyAssign", 0x50: "MoveAssign", 0x58: "Equals", 0x60: "Hash", 0x68: "TypeOf"}
# A qualified name a declaration can spell: identifiers joined by `::`.
QUALIFIED_RE = re.compile(r"[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*")


def enum_slots(orig: asmdiff.Original, init: int) -> dict[int, int] | None:
    """{slot offset: function} of an enum constructor, when it stores a function in every CType slot."""
    _, st, _ = emulate(orig, init)
    out = {}
    for off in VALUE_OPS:
        v = st.get(off)
        if not v or v[0] != "fn":
            return None
        out[off] = v[1]
    return out


def returned_object(orig: asmdiff.Original, addr: int) -> int | None:
    """The global a function returns from its GOT slot (`adrp; ldr x0, [slot]; ret`), or None."""
    ins = disasm(orig, addr)
    if [x.mnemonic for x in ins] != ["adrp", "ldr", "ret"]:
        return None
    page, mem = ins[0].operands[1].imm, ins[1].operands[1].mem
    target = orig.img.pointer(page + mem.disp - BASE)
    return BASE + target if target is not None else None


def enum_names(orig: asmdiff.Original, version: str, enums: list) -> dict[int, tuple[str, int]]:
    """{constructor: (enum name, its descriptor)} for the enums whose static initialiser names them
    (tools/gen_fields.py's built_objects) under a name a declaration can spell."""
    import gen_fields
    objects = {}
    for init, _ in enums:
        slots = enum_slots(orig, init)
        obj = returned_object(orig, slots[0x68]) if slots else None
        if obj is not None:
            objects[init] = obj
    names = gen_fields.built_objects(version, set(objects.values()))
    return {init: (names[obj], obj) for init, obj in objects.items()
            if obj in names and QUALIFIED_RE.fullmatch(names[obj])}


def enum_declaration(name: str) -> str:
    """`namespace A::B { enum class E : u32; }`: an enclosing class is declared as a namespace,
    which mangles the same."""
    scope, _, leaf = name.rpartition("::")
    decl = f"enum class {leaf} : u32;"
    return f"namespace {scope} {{ {decl} }}" if scope else decl


def emit_enum_ops(init: int, name: str, obj: int, body: list[str]) -> list[str]:
    """An enum's description and value operations, named by the enum (ValueOps.h). `body` is
    enum_traits' lines; the slot lines give way to ValueOpsDescription."""
    rest = [line for line in body if " UnkFn " not in line]
    return ["template <>",
            f"const CType& base::reflection::ValueOps<{name}>::TypeOf(const void*) {{ return g_unk_{obj:x}; }}",
            f"template struct base::reflection::ValueOps<{name}>;",
            "template <>",
            f"struct base::reflection::EnumTypeDescription<{name}> "
            f": base::reflection::ValueOpsDescription<{name}> {{",
            *rest,
            "};",
            f"template base::reflection::CEnumType::CEnumType(const base::global::CStrId&, const void*, {name}*);"]


def enum_ops_names(orig: asmdiff.Original, init: int, name: str) -> dict[int, str]:
    """data/functions/ names of an enum's constructor and value operations, as its symbols demangle
    (asmdiff.name_aliases maps refl_InitType<E> to the constructor)."""
    out = {a: f"base::reflection::ValueOps<{name}>::{VALUE_OPS[off]}"
           for off, a in (enum_slots(orig, init) or {}).items()}
    out[init] = f"base::reflection::refl_InitType<{name}>"
    return out


ENUM_OPS_RE = re.compile(r"^template struct base::reflection::ValueOps<(.+)>;$", re.M)


def reflection_names(orig: asmdiff.Original, version: str) -> dict[int, str]:
    """data/functions/ names of the enums written in their named form in src/reflection_types/
    (tools/gen_functions_csv.py)."""
    written = {m.group(1) for f in OUT.glob("enum_types_*.cpp")
               for m in ENUM_OPS_RE.finditer(f.read_text(encoding="utf-8"))}
    _, _, enums, _ = scan(orig)
    out = {}
    for init, (name, _) in enum_names(orig, version, enums).items():
        if name in written:
            out.update(enum_ops_names(orig, init, name))
    return out


COLLECTION_KIND = "base::reflection::CCollectionType"


def local_element(orig: asmdiff.Original, init: int) -> int | None:
    """The element class getter of a collection constructor that calls it but stores a constant
    address (the getter is defined in the same TU, as for local_pointee)."""
    kind, st, _ = emulate(orig, init)
    e = st.get(0xF0)
    if kind != COLLECTION_KIND or not e or e[0] != "data" or len(e) > 2:
        return None
    calls = [x.operands[0].imm for x in disasm(orig, init) if x.mnemonic == "bl"]
    calls = [c for c in calls if c not in (KIND_HASH, ALLOCATOR_GETTER)]
    return calls[1] if len(calls) > 1 else None


def collection_traits(orig: asmdiff.Original, init: int, decls: set[str],
                      local: int | None = None) -> list[str] | None:
    """Traits of a CCollectionType constructor whose key, element and iterator types come from
    getter calls; with `local`, the element is that class getter, defined in the same TU."""
    kind, st, _ = emulate(orig, init)
    if kind != COLLECTION_KIND:
        return None
    size, flag = st.get(0x18), st.get(0xE0)
    if not size or size[0] != "imm" or not flag or flag[0] != "imm":
        return None
    lines = [f"    static constexpr u64 size = {size[1]:#x};",
             f"    static constexpr u32 kind = {flag[1]:#x};"]

    def fn(off: int) -> str | None:
        v = st.get(off)
        if v == ("imm", 0):
            return "nullptr"
        if v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            return f"&fn_{v[1]:x}"
        return None

    for off in (0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68):
        f = fn(off)
        if f is None:
            return None
        lines.append(f"    static constexpr UnkFn {SLOTS[off]} = {f};")
    for off, member in ((0xE8, "key"), (0xF0, "element"), (0xF8, "iterator"), (0x100, "const_iterator")):
        v = st.get(off)
        if off == 0xF0 and local is not None:
            lines.append(f"    static const CType* element() {{ return fn_{local:x}(); }}")
            continue
        if v and v[0] == "data" and len(v) > 2:  # a global descriptor, through the GOT
            decls.add(f"extern base::reflection::CType g_unk_{v[1]:x};")
            lines.append(f"    static const CType* {member}() {{ return &g_unk_{v[1]:x}; }}")
            continue
        if not v or v[0] != "call":
            return None
        decls.add(f"const base::reflection::CType* fn_{v[1]:x}();")
        lines.append(f"    static const CType* {member}() {{ return fn_{v[1]:x}(); }}")
    for off, member in zip(range(0x108, 0x170, 8), COLLECTION_SLOTS):
        f = fn(off)
        if f is None:
            return None
        lines.append(f"    static constexpr UnkFn {member} = {f};")
    return lines


def emit_collection(init: int, body: list[str]) -> list[str]:
    return [f"struct UnkRefl_fn_{init:x};",
            "template <>",
            f"struct base::reflection::CollectionTypeDescription<UnkRefl_fn_{init:x}> {{",
            *body,
            "};",
            f"template base::reflection::CCollectionType::CCollectionType(const base::global::CStrId&, "
            f"UnkRefl_fn_{init:x}*, void*);"]


_KIND_CALLERS: dict[int, list[int]] = {}


def kind_hash_callers(orig: asmdiff.Original) -> list[int]:
    """`U`/`O` functions that call the kind hash within their first 16 instructions (cached)."""
    if id(orig) not in _KIND_CALLERS:
        rows = []
        for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
            with f.open(newline="", encoding="utf-8") as fh:
                rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
        _KIND_CALLERS[id(orig)] = [a for a in rows if a in orig.sizes and any(
            x.mnemonic == "bl" and x.operands[0].imm == KIND_HASH for x in disasm(orig, a)[:16])]
    return _KIND_CALLERS[id(orig)]


FLAGSET_KIND = "base::reflection::CFlagsetType"


def flagset_traits(orig: asmdiff.Original, init: int, decls: set[str]) -> list[str] | None:
    kind, st, tail = emulate(orig, init)
    if kind != FLAGSET_KIND or tail is not None:
        return None
    size, under, second = st.get(0x18), st.get(0xE0), st.get(0xE8)
    if not size or size[0] != "imm" or not under or under[0] != "call":
        return None
    if not second or second[0] != "data" or len(second) < 3:
        return None
    lines = [f"    static constexpr u64 size = {size[1]:#x};"]
    for off in (0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68):
        v = st.get(off)
        if v == ("imm", 0):
            lines.append(f"    static constexpr UnkFn {SLOTS[off]} = nullptr;")
        elif v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            lines.append(f"    static constexpr UnkFn {SLOTS[off]} = &fn_{v[1]:x};")
        else:
            return None
    decls.add(f"const base::reflection::CType* fn_{under[1]:x}();")
    decls.add(f"extern base::reflection::CType g_unk_{second[1]:x};")
    lines += [f"    static const CType* underlying() {{ return fn_{under[1]:x}(); }}",
              f"    static const CType* field_0xe8() {{ return &g_unk_{second[1]:x}; }}"]
    return lines


def emit_flagset(init: int, body: list[str]) -> list[str]:
    return [f"struct UnkRefl_fn_{init:x};",
            "template <>",
            f"struct base::reflection::FlagsetTypeDescription<UnkRefl_fn_{init:x}> {{",
            *body,
            "};",
            f"template base::reflection::CFlagsetType::CFlagsetType(const base::global::CStrId&, "
            f"UnkRefl_fn_{init:x}*);"]


def scan_flagsets(orig: asmdiff.Original, decls: set[str]) -> list:
    """CFlagsetType constructors: (init, traits lines)."""
    out = []
    for a in kind_hash_callers(orig):
        body = flagset_traits(orig, a, decls)
        if body:
            out.append((a, body))
    return out


def scan_collections(orig: asmdiff.Original, decls: set[str]) -> tuple[list, list]:
    """CCollectionType constructors (the descriptors are globals built by static initialisers):
    (init, traits lines), and those with a local element class as (init, traits lines, element
    getter), which tools/gen_reflection.py generates in that class's file. Candidates are the
    functions that call the kind hash."""
    out, local = [], []
    for a in kind_hash_callers(orig):
        element = local_element(orig, a)
        body = collection_traits(orig, a, decls, element)
        if body and element is not None:
            local.append((a, body, element))
        elif body:
            out.append((a, body))
    return out, local


def inline_getter(orig: asmdiff.Original, addr: int) -> tuple[str, str, int, str] | None:
    """(object register, type name, kind length, kind string) of a guarded getter whose descriptor
    constructor is inlined: a guard, the name's string id, the kind hash, the object's stores,
    the name's release, __cxa_atexit and the guard release."""
    insns = disasm(orig, addr)
    if not any(x.mnemonic == "ldarb" for x in insns[:12]):
        return None
    # The object: the base register of most stores.
    bases: dict[str, int] = {}
    for x in insns:
        if x.mnemonic in ("str", "stp", "strb") and x.operands[-1].type == capstone.arm64.ARM64_OP_MEM:
            b = CS.reg_name(x.operands[-1].mem.base)
            bases[b] = bases.get(b, 0) + 1
    if not bases:
        return None
    base = max(bases, key=bases.get)
    if base == "sp":
        return None
    page: dict[str, int] = {}
    val: dict[str, tuple] = {}
    name = length = kind = None
    calls = []
    for x in insns:
        m, o = x.mnemonic, x.operands
        if m == "adrp":
            page[reg(x, o[0])] = o[1].imm
        elif m == "add" and len(o) == 3 and o[2].type == capstone.arm64.ARM64_OP_IMM and reg(x, o[1]) in page:
            val[reg(x, o[0])] = ("str", orig.img.cstring(page[reg(x, o[1])] + o[2].imm - BASE))
        elif m in ("mov", "movz") and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_IMM:
            val[reg(x, o[0])] = ("imm", o[1].imm)
        elif m == "mov" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_REG:
            val[reg(x, o[0])] = val.get(reg(x, o[1]), ("?",))
        elif m == "bl":
            t = o[0].imm
            calls.append(t)
            if t == CSTRID_CTOR and name is None:
                name = val.get("x1", ("?", None))[1]
            elif t == KIND_HASH and length is None:
                kind = val.get("x0", ("?", None))[1]
                if not isinstance(kind, str) or val.get("x1", ("?",))[0] != "imm":
                    return None
                length = val["x1"][1]
    if not name or not kind or length != len(kind.encode("utf-8")):
        return None
    if calls[:3] != [0x7101238DA0, CSTRID_CTOR, KIND_HASH]:
        return None
    if [c for c in calls[3:] if c != ALLOCATOR_GETTER] != [0x71000803A4, 0x7100000250, 0x7101238DB0]:
        return None
    return base, name, length, kind


def primitive_traits(orig: asmdiff.Original, addr: int, decls: set[str]) -> list[str] | None:
    """Traits of a plain CType getter (refl_GetPrimitiveType): a guarded static whose descriptor
    is built inline, named by the string that is also hashed as its kind."""
    g = inline_getter(orig, addr)
    if not g or g[1] != g[3]:
        return None
    base, name, length, _ = g
    _, st, _ = emulate(orig, addr, base)
    size = st.get(0x18)
    if not size or size[0] != "imm" or st.get(0x78) != ("imm", 0):
        return None
    lines = [f"    static constexpr const char* name = {refl_attrs.c_string(name)};",
             f"    static constexpr u64 length = {length};",
             f"    static constexpr u64 size = {size[1]:#x};"]
    for off in (0x28, 0x30, 0x38, 0x40, 0x48, 0x50, 0x58, 0x60, 0x68, 0x70):
        v = st.get(off)
        if v == ("imm", 0):
            lines.append(f"    static constexpr UnkFn fn_{off:#x} = nullptr;")
        elif v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            lines.append(f"    static constexpr UnkFn fn_{off:#x} = &fn_{v[1]:x};")
        else:
            return None
    return lines


def emit_primitive(getter: int, body: list[str]) -> list[str]:
    return [f"struct UnkRefl_fn_{getter:x};",
            "template <>",
            f"struct base::reflection::UnkReflPrimitiveTraits<UnkRefl_fn_{getter:x}> {{",
            *body,
            "};",
            f"template const base::reflection::CType* "
            f"base::reflection::refl_GetPrimitiveType<UnkRefl_fn_{getter:x}>();"]


def emit_inline_pointer(getter: int, name: str, body: list[str]) -> list[str]:
    """A pointer type getter with its constructor inlined: the constructor isn't instantiated
    explicitly, and its tag carries no address (nothing out of line to match)."""
    return [f"struct UnkReflInline_{getter:x};",
            f"struct UnkRefl_fn_{getter:x};",
            "template <>",
            f"struct base::reflection::PointerTypeDescription<UnkReflInline_{getter:x}> {{",
            *body,
            "};",
            "template <>",
            f"struct base::reflection::UnkReflTypeGetter<UnkRefl_fn_{getter:x}> {{",
            f"    static constexpr const char* name = {refl_attrs.c_string(name)};",
            f"    using Init = UnkReflInline_{getter:x};",
            "};",
            f"template const base::reflection::CType* "
            f"base::reflection::refl_GetPointerType<UnkRefl_fn_{getter:x}>();"]


def scan_primitives(orig: asmdiff.Original, decls: set[str]) -> tuple[list, list]:
    """Getters with an inlined descriptor constructor: plain CType getters as (getter, traits
    lines) and pointer type getters as (getter, name, traits lines). Candidates call the
    string-id constructor and the kind hash."""
    out, ptrs = [], []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                a = int(r["address"], 16)
                if r["status"] not in ("U", "O") or a not in orig.sizes or orig.sizes[a] > 1024:
                    continue
                g = inline_getter(orig, a)
                if not g:
                    continue
                if g[1] == g[3]:
                    body = primitive_traits(orig, a, decls)
                    if body:
                        out.append((a, body))
                elif g[3] == POINTER_KIND:
                    body = traits(orig, a, decls, base=g[0])
                    if body:
                        ptrs.append((a, g[1], body))
    return out, ptrs


PRELUDE = ['#include "base/reflection/CCollectionType.h"',
           '#include "base/reflection/CEnumType.h"',
           '#include "base/reflection/CFlagsetType.h"',
           '#include "base/reflection/CPointerType.h"',
           '#include "base/reflection/UnkReflPrimitiveType.h"',
           '#include "base/reflection/ValueOps.h"',
           "using base::reflection::CType;",
           "using base::reflection::UnkFn;"]
NAME_RE = re.compile(r"\b(?:fn|g_unk)_[0-9a-f]{10}\b")


def write_chunks(prefix: str, blocks: list[list[str]], decls: set[str], header: list[str]) -> None:
    for k in range(0, len(blocks), CHUNK):
        body = [line for b in blocks[k:k + CHUNK] for line in b]
        used = used_decls(body, decls)
        (OUT / f"{prefix}_{k // CHUNK:02d}.cpp").write_text("\n".join(header + PRELUDE + used + body) + "\n",
                                                           encoding="utf-8")


def scan(orig: asmdiff.Original) -> tuple[list, list, list, set[str]]:
    """Candidates: (pointer pairs, pointer pairs with a local pointee, enums, declarations).

    A pair is (getter, name, init, traits lines); a local pair adds its pointee getter, which
    tools/gen_reflection.py generates in the same file."""
    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
    getter_shape = shape(orig, REF_GETTER)
    enum_shape = shape(orig, REF_ENUM)
    n = len(getter_shape) * 4

    pairs = []  # (getter, name, init, traits lines)
    local = []  # (getter, name, init, traits lines, pointee getter)
    enums = []  # (init, traits lines)
    decls: set[str] = set()
    for a in rows:
        size = orig.sizes.get(a)
        if size == len(enum_shape) * 4 and shape(orig, a) == enum_shape:
            body = enum_traits(orig, a, decls)
            if body:
                enums.append((a, body))
            continue
        if size != n or shape(orig, a) != getter_shape:
            continue
        facts = getter_facts(orig, a)
        if not facts:
            continue
        name, init = facts
        if init not in orig.sizes:
            continue
        pointee = local_pointee(orig, init)
        body = traits(orig, init, decls, pointee)
        if body and pointee is not None:
            local.append((a, name, init, body, pointee))
        elif body:
            pairs.append((a, name, init, body))
    # One constructor may serve several getters; emit it once (first getter wins).
    seen: set[int] = set()
    pairs = [p for p in pairs if not (p[2] in seen or seen.add(p[2]))]
    local = [p for p in local if not (p[2] in seen or seen.add(p[2]))]
    return pairs, local, enums, decls


def used_decls(lines: list[str], decls: set[str]) -> list[str]:
    """The declarations of decls that lines refer to."""
    by_name = {NAME_RE.search(d.split("(")[0]).group(0): d for d in decls}
    names = {n for line in lines for n in NAME_RE.findall(line)}
    return sorted(d for n, d in by_name.items() if n in names)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    pairs, _, enums, decls = scan(orig)
    colls, _ = scan_collections(orig, decls)
    prims, inline_ptrs = scan_primitives(orig, decls)
    flagsets = scan_flagsets(orig, decls)
    # Enums named by their static initialisers get their value operations from ValueOps.h; a name
    # two descriptors share, or a named form that does not match, keeps the address form.
    named = enum_names(orig, args.version, enums)
    counts: dict[str, int] = {}
    for name, _ in named.values():
        counts[name] = counts.get(name, 0) + 1
    named = {i: v for i, v in named.items() if counts[v[0]] == 1}
    for _, o in named.values():
        decls.add(f"extern base::reflection::CType g_unk_{o:x};")

    def enum_block(init: int, body: list[str]) -> list[str]:
        if init in named:
            name, o = named[init]
            return [enum_declaration(name)] + emit_enum_ops(init, name, o, body)
        return emit_enum(init, body)

    work = ROOT / "build" / "gen_types"
    work.mkdir(parents=True, exist_ok=True)
    src, obj = work / "candidates.cpp", work / "candidates.o"
    for attempt in range(2):
        text = (PRELUDE + sorted(decls) + [line for g, nm, i, b in pairs for line in emit(g, nm, i, b)]
                + [line for i, b in enums for line in enum_block(i, b)]
                + [line for i, b in colls for line in emit_collection(i, b)]
                + [line for g, b in prims for line in emit_primitive(g, b)]
                + [line for p in inline_ptrs for line in emit_inline_pointer(*p)]
                + [line for i, b in flagsets for line in emit_flagset(i, b)])
        src.write_text("\n".join(text) + "\n", encoding="utf-8")
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0:
            raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
        # The named forms' symbols carry no address: they map through the names data/functions/
        # will give them.
        expected = {n: a for i, (name, _) in named.items() for a, n in enum_ops_names(orig, i, name).items()}
        expected.update({alias: a for n, a in list(expected.items()) for alias in asmdiff.name_aliases(n)})
        symbols = [f.symbol for f in asmdiff.read_object(obj)]
        demangled = asmdiff.demangle(symbols)
        # strict: a byte match whose relocations name the wrong callee, global or GOT slot is rejected
        results = asmdiff.diff_object(obj, orig, resolve=lambda sym: expected.get(demangled.get(sym, sym)),
                                      strict=True)
        ok = {res.address for res in results if res.matching}
        failed = {i for i, (name, _) in named.items() if not set(enum_ops_names(orig, i, name)) <= ok}
        if not failed:
            break
        print(f"named enum forms that do not match, kept by address: {' '.join(f'{i:x}' for i in sorted(failed))}")
        named = {i: v for i, v in named.items() if i not in failed}
    for line in asmdiff.reloc_rejects(results):
        print(line)
    good = [p for p in pairs if p[0] in ok and p[2] in ok]
    good_enums = [e for e in enums if e[0] in ok]
    good_colls = [c for c in colls if c[0] in ok]
    good_prims = [p for p in prims if p[0] in ok]
    good_inline = [p for p in inline_ptrs if p[0] in ok]
    good_flagsets = [f for f in flagsets if f[0] in ok]

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = "// Generated by tools/gen_types.py from main; regenerate rather than edit."
    write_chunks("pointer_types", [emit(*p) for p in good] + [emit_inline_pointer(*p) for p in good_inline], decls,
                 [note, "// Getters (refl_GetType<T>) and descriptor constructors (refl_InitType<T>) of pointer",
                  "// types, byte-identical to the original."])
    write_chunks("enum_types", [enum_block(*e) for e in good_enums], decls,
                 [note, "// Descriptor constructors (refl_InitType<E>) of enums, byte-identical to the original."])
    write_chunks("collection_types", [emit_collection(*c) for c in good_colls], decls,
                 [note, "// Descriptor constructors of collection types (globals built by static initialisers),",
                  "// byte-identical to the original."])
    write_chunks("flagset_types", [emit_flagset(*f) for f in good_flagsets], decls,
                 [note, "// Descriptor constructors of flag sets, byte-identical to the original."])
    write_chunks("primitive_types", [emit_primitive(*p) for p in good_prims], decls,
                 [note, "// Getters (refl_GetType<T>) of plain CType descriptors, byte-identical to the original."])
    print(f"pointer types: {len(pairs)} getter/constructor candidates, {len(good)} pairs byte-identical; "
          f"enums: {len(enums)} constructor candidates, {len(good_enums)} byte-identical "
          f"({sum(e[0] in named for e in good_enums)} with named value operations); "
          f"collections: {len(colls)} constructor candidates, {len(good_colls)} byte-identical; "
          f"plain types: {len(prims)} getter candidates, {len(good_prims)} byte-identical; "
          f"pointer types with an inlined constructor: {len(inline_ptrs)} getter candidates, "
          f"{len(good_inline)} byte-identical; "
          f"flag sets: {len(flagsets)} constructor candidates, {len(good_flagsets)} byte-identical")
    return 0


if __name__ == "__main__":
    sys.exit(main())
