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


# Type names as the game spells them ("base::global::CRntVector<CFoo>const*"): parsed so that a source can
# declare what they name and so that data/functions/ can spell them as llvm-cxxfilt does.
TYPE_TOKEN_RE = re.compile(r"\s*([A-Za-z_]\w*(?:::[A-Za-z_]\w*)*|[<>,*&])")
BUILTIN_TYPES = {"unsigned": "unsigned int", "char": "char", "int": "int", "void": "void", "bool": "bool",
                 "float": "float", "double": "double", "short": "short", "long": "long",
                 "unsigned int": "unsigned int", "unsigned char": "unsigned char", "unsigned short": "unsigned short",
                 "unsigned long": "unsigned long", "long long": "long long", "signed char": "signed char"}


def parse_type(text: str):
    """(name, template arguments or None, modifiers) for a type name made of qualified names, template
    arguments that are types, `const` and `*`; None for anything else (a non-type template argument, a
    member of a template, a name in std::)."""
    toks = TYPE_TOKEN_RE.findall(text)
    if "".join(toks) != re.sub(r"\s+", "", text):
        return None

    def one(i: int):
        const = toks[i] == "const"
        i += const
        if (i >= len(toks) or not re.fullmatch(r"[A-Za-z_][\w:]*", toks[i])
                or toks[i] in ("const", "true", "false")):
            raise ValueError
        name, args = toks[i], None
        i += 1
        # Two-word builtins ("unsigned int", "long long", ...), as canonical spellings have them.
        while name in ("unsigned", "signed", "long", "short") and i < len(toks) and toks[i] in ("int", "char", "short", "long"):
            name, i = f"{name} {toks[i]}", i + 1
        if i < len(toks) and toks[i] == "<":
            args, i = [], i + 1
            while True:
                arg, i = one(i)
                args.append(arg)
                if toks[i] == ",":
                    i += 1
                elif toks[i] == ">":
                    i += 1
                    break
                else:
                    raise ValueError
        mods = ["const"] if const else []
        while i < len(toks) and toks[i] in ("const", "*"):
            mods.append(toks[i])
            i += 1
        return (name, args, mods), i

    try:
        node, end = one(0)
    except (ValueError, IndexError):
        return None
    if end != len(toks) or any(n.startswith("std::") for n in type_names(node)[0] | set(type_names(node)[1])):
        return None
    return node


def type_names(node) -> tuple[set[str], dict[str, int]]:
    """The class names and the templates (with their argument counts) a parsed type refers to."""
    classes: set[str] = set()
    templates: dict[str, int] = {}

    def walk(n):
        name, args, _ = n
        if name in BUILTIN_TYPES:
            return
        if args is None:
            classes.add(name)
        else:
            templates[name] = len(args)
            for a in args:
                walk(a)
    walk(node)
    return classes, templates


def render_type(node) -> str:
    """A parsed type as llvm-cxxfilt prints it: `const` after what it qualifies, `unsigned int`."""
    name, args, mods = node
    out = BUILTIN_TYPES.get(name, name)
    if args is not None:
        out += "<" + ", ".join(render_type(a) for a in args) + ">"
    return out + "".join(" const" if m == "const" else m for m in mods)


def canonical_type(text: str) -> str:
    """`text` as llvm-cxxfilt prints it, or `text` itself when it does not parse."""
    node = parse_type(text)
    return render_type(node) if node else text


def declare_in(scope: str, decl: str) -> str:
    return f"namespace {scope} {{ {decl} }}" if scope else decl


def type_declarations(node, enums: frozenset[str] = frozenset()) -> list[str]:
    """Declarations of the classes and templates a parsed type names, `enums` declared as the enums they
    are (enum_declaration); an enclosing class is declared as a namespace, which mangles the same."""
    classes, templates = type_names(node)
    out = []
    for c in sorted(classes):
        scope, _, leaf = c.rpartition("::")
        out.append(enum_declaration(c) if c in enums else declare_in(scope, f"class {leaf};"))
    for t, n in sorted(templates.items()):
        scope, _, leaf = t.rpartition("::")
        out.append(declare_in(scope, f"template <{', '.join(['class'] * n)}> class {leaf};"))
    return out


def pointer_slots(orig: asmdiff.Original, init: int) -> dict[int, int] | None:
    """{slot offset: function} of a pointer constructor's CType slots, when every one holds a function."""
    _, st, _ = emulate(orig, init)
    out = {}
    for off in VALUE_OPS:
        v = st.get(off)
        if not v or v[0] != "fn":
            return None
        out[off] = v[1]
    return out


# An element iterator's descriptor (base/reflection/CollectionTypeMapper.h), named for its collection.
ITERATOR_RE = re.compile(r"base::reflection::CollectionTypeMapper<(.+)>::(TElementIterator|TElementConstIterator)")


def pointer_value_type(name: str):
    """(the name as llvm-cxxfilt prints it, the parsed type whose names it declares) of a pointer type
    whose value operations can come from ValueOps.h: a plain pointer (`T*`, `T const*`) or an element
    iterator of a collection whose name parses; None for anything else."""
    m = ITERATOR_RE.fullmatch(re.sub(r"\s+", "", name))
    if m:
        node = parse_type(m.group(1))
        if node and not node[2]:
            return f"base::reflection::CollectionTypeMapper<{render_type(node)}>::{m.group(2)}", node
        return None
    node = parse_type(name)
    if node and node[2] and node[2][-1] == "*":
        return render_type(node), node
    return None


def _scopes_of(full: str) -> set[str]:
    parts = full.split("::")
    return {"::".join(parts[:k]) for k in range(1, len(parts))}


def declaration_clashes(nodes: list, enums: frozenset[str] = frozenset()) -> set[str]:
    """The names whose declarations would collide if `nodes` (parsed types) and `enums` shared a
    translation unit: a class some name also uses as a scope, and a template named with two argument
    counts or also as a class."""
    names = [type_names(n) for n in nodes]
    classes = set().union(*(c for c, _ in names)) | set(enums)
    types = classes | set().union(*(set(t) for _, t in names))
    clash = types & set().union(*(_scopes_of(f) for f in types))
    counts: dict[str, set[int]] = {}
    for _, templates in names:
        for t, n in templates.items():
            counts.setdefault(t, set()).add(n)
    return clash | {t for t, ns in counts.items() if len(ns) > 1 or t in classes}


def named_pointers(orig: asmdiff.Original, pairs: list, enums: frozenset[str] = frozenset()) -> dict[int, str]:
    """{getter: name} of the pointer types (pointer_value_type) whose value operations can come from
    ValueOps.h: a name that parses, every CType slot a function, and no name whose declaration would
    collide with another's (declaration_clashes). The plain pointers are taken first, all together;
    each element iterator then joins only if it adds no collision."""
    cands = {}
    for g, name, init, _ in pairs:
        vt = pointer_value_type(name)
        if vt and pointer_slots(orig, init):
            cands[g] = vt
    refs = {g: (lambda c, t: c | set(t))(*type_names(n)) for g, (_, n) in cands.items()}
    plain = {g for g, (canon, _) in cands.items() if not ITERATOR_RE.fullmatch(canon)}
    clash = declaration_clashes([cands[g][1] for g in plain], enums)
    out: dict[int, str] = {}
    for g in plain:
        canon = cands[g][0]
        if not any(f in clash or _scopes_of(f) & clash for f in refs[g]) and canon not in out.values():
            out[g] = canon
    taken = [cands[g][1] for g in out]
    base = declaration_clashes(taken, enums)
    for g, (canon, node) in cands.items():
        if g in plain or canon in out.values():
            continue
        if declaration_clashes(taken + [node], enums) <= base:
            out[g] = canon
            taken.append(node)
    return out


def emit_pointer_ops(getter: int, name: str, init: int, body: list[str], canon: str,
                     enums: frozenset[str] = frozenset()) -> list[str]:
    """A pointer type's description with its CType slots from ValueOps<T> (ValueOps.h), T spelled
    `canon`; the pointer-specific slots (get, set, increment) and the getter (whose string is `name`)
    stay as they are."""
    slot_members = {f" {m} = " for m in SLOTS.values() if m not in ("get", "set", "increment")}
    rest = [line for line in body if not any(m in line for m in slot_members)]
    out = emit(getter, name, init, rest)
    name = canon
    head = out.index("template <>")
    desc = f"struct base::reflection::PointerTypeDescription<UnkRefl_fn_{init:x}> {{"
    out[out.index(desc)] = (f"struct base::reflection::PointerTypeDescription<UnkRefl_fn_{init:x}> "
                            f": base::reflection::ValueOpsDescription<{name}> {{")
    ops = ["template <>",
           f"const CType& base::reflection::ValueOps<{name}>::TypeOf(const void*) "
           f"{{ return *base::reflection::refl_GetPointerType<UnkRefl_fn_{getter:x}>(); }}",
           f"template struct base::reflection::ValueOps<{name}>;"]
    return type_declarations(pointer_value_type(canon)[1], enums) + out[:head] + ops + out[head:]


def pointer_ops_names(orig: asmdiff.Original, init: int, name: str) -> dict[int, str]:
    """data/functions/ names of a pointer type's value operations (pointer_value_type)."""
    return {a: f"base::reflection::ValueOps<{name}>::{VALUE_OPS[off]}"
            for off, a in (pointer_slots(orig, init) or {}).items()}


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
    """data/functions/ names of the enums, flag sets, pointer and plain types written in their named form in
    src/reflection_types/ (tools/gen_functions_csv.py)."""
    written = {m.group(1) for f in OUT.glob("*_types_*.cpp")
               for m in ENUM_OPS_RE.finditer(f.read_text(encoding="utf-8"))}
    pairs, _, enums, _ = scan(orig)
    out = {}
    named = enum_names(orig, version, enums)
    for init, (name, _) in named.items():
        if name in written:
            out.update(enum_ops_names(orig, init, name))
    flags = enum_names(orig, version, scan_flagsets(orig, set()))
    for init, (name, _) in flags.items():
        if name in written:
            out.update(enum_ops_names(orig, init, name))
    inits = {g: i for g, _, i, _ in pairs}
    known = frozenset(name for name, _ in list(named.values()) + list(flags.values()))
    for g, canon in named_pointers(orig, pairs, known).items():
        if canon in written:
            out.update(pointer_ops_names(orig, inits[g], canon))
    prims, _ = scan_primitives(orig, set())
    for g, body in prims:
        value_type = primitive_value_type(body)
        if value_type and value_type in written:
            out.update(primitive_ops_names(body, value_type))
    mapped = {m.group(1) for f in OUT.glob("collection_types_*.cpp")
              for m in MAPPER_DESC_RE.finditer(f.read_text(encoding="utf-8"))}
    named_ptrs = named_pointers(orig, pairs, known)
    colls, _ = scan_collections(orig, set())
    by_hand = defined_by_hand()
    for _, body in colls:
        v = mapped_collection(body, named_ptrs, frozenset(name for name, _ in named.values()))
        if v and v[0] in mapped:
            out.update(collection_ops_names(orig, body, *v, mapper_members(orig, body, by_hand, v[0])))
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


# A vector's collection descriptor written from CollectionTypeMapper<C> (base/reflection/CollectionTypeMapper.h):
# description member -> the mapper's function. The other slots keep their addresses until written.
MAPPER_SLOTS = {"construct": "Construct", "copy_construct": "CopyConstruct", "move_construct": "MoveConstruct",
                "destroy": "Destroy", "copy_assign": "CopyAssign", "move_assign": "MoveAssign", "hash": "Hash",
                "count": "Count", "clear": "Clear", "add_copy": "AddCopy", "add_move": "AddMove", "at": "At",
                "find": "Find", "begin": "Begin", "end": "End", "equals": "Equals", "erase": "Erase",
                "set_copy": "SetCopy", "set_move": "SetMove", "const_begin": "ConstBegin", "const_end": "ConstEnd"}
# The CRntVector member each calls out of line, and its declaration (E: the element type).
# Class elements whose header gives CollectionTypeMapper what it needs: the include a generated file adds.
CLASS_ELEMENT_HEADERS = {"base::core::AssetName": "base/core/AssetName.h"}

MAPPER_HELPERS = {"add_copy": ("InsertCopy", "void {C}::InsertCopy({E} const&, u32)"),
                  "add_move": ("InsertMove", "void {C}::InsertMove({E}&&, u32)"),
                  "set_copy": ("GrowAt", "{E}& {C}::GrowAt(u32)"),
                  "set_move": ("GrowAt", "{E}& {C}::GrowAt(u32)")}
VECTOR_RE = re.compile(r"base::global::CRntVector<(.+)>")
SMALL_DICT_RE = re.compile(r"base::global::CRntSmallDictionary<(.+)>")
MAPPER_DESC_RE = re.compile(r": base::reflection::(?:Collection|SmallDictionary)MapperDescription<(.+)> \{$", re.M)
# The slots CollectionTypeMapper writes for a small dictionary so far (SmallDictionaryMapperDescription).
SMALL_DICT_SLOTS = ("construct", "move_construct", "hash", "count", "at", "begin", "end")
# Value classes a small dictionary's mapper needs whole (At and End take their size), and their headers.
DICT_VALUE_HEADERS = {"base::global::CStrId": "base/global/CStrId.h",
                      "base::global::CFilePathStrId": "base/global/CFilePathStrId.h"}


def collection_slots(body: list[str]) -> dict[str, int]:
    """{description member: function} of a collection's address-form traits."""
    return {m: int(a, 16) for m, a in re.findall(r" (\w+) = &fn_([0-9a-f]+);", "\n".join(body))}


def mapped_vector(body: list[str], named_ptrs: dict[int, str], enums: frozenset[str]) -> tuple[str, str] | None:
    """(C, element) of a vector collection whose slots CollectionTypeMapper<C> writes today: kind 1 and 0x28
    bytes, C named by its element iterator's named form (named_pointers), every mapper slot a function, and an
    element that is a pointer, an enum, a plain number or a class of CLASS_ELEMENT_HEADERS."""
    text = "\n".join(body)
    if "size = 0x28;" not in text or "kind = 0x1;" not in text:
        return None
    it = element_iterator(body, named_ptrs)
    v = VECTOR_RE.fullmatch(it.group(1)) if it else None
    if v is None or not set(MAPPER_SLOTS) <= set(collection_slots(body)):
        return None
    element = v.group(1)
    if (element.endswith("*") or element in enums or element in PRIMITIVE_VALUE_TYPES.values()
            or element in CLASS_ELEMENT_HEADERS):
        return it.group(1), element
    return None


def element_iterator(body: list[str], named_ptrs: dict[int, str]) -> re.Match | None:
    """ITERATOR_RE's match of a collection's element iterator named form, or None."""
    m = re.search(r"static const CType\* iterator\(\) \{ return fn_([0-9a-f]+)\(\); \}", "\n".join(body))
    canon = named_ptrs.get(int(m.group(1), 16)) if m else None
    it = ITERATOR_RE.fullmatch(canon) if canon else None
    return it if it and it.group(2) == "TElementIterator" else None


def split_template_args(args: str) -> list[str]:
    """A template argument list split at its top-level commas."""
    out, depth, cur = [], 0, ""
    for ch in args:
        depth += ch == "<"
        depth -= ch == ">"
        if ch == "," and depth == 0:
            out.append(cur.strip())
            cur = ""
        else:
            cur += ch
    return out + [cur.strip()]


def mapped_small_dictionary(body: list[str], named_ptrs: dict[int, str],
                            enums: frozenset[str]) -> tuple[str, str] | None:
    """(C, value) of a small dictionary whose SMALL_DICT_SLOTS CollectionTypeMapper<C> writes: kind 2 and 0x28
    bytes, C named by its element iterator's named form, and a value that is a pointer, an enum, a plain number
    or a class of DICT_VALUE_HEADERS."""
    text = "\n".join(body)
    if "size = 0x28;" not in text or "kind = 0x2;" not in text:
        return None
    it = element_iterator(body, named_ptrs)
    d = SMALL_DICT_RE.fullmatch(it.group(1)) if it else None
    if d is None or not set(SMALL_DICT_SLOTS) <= set(collection_slots(body)):
        return None
    args = split_template_args(d.group(1))
    if len(args) != 2:
        return None
    value = args[1]
    if (value.endswith("*") or value in enums or value in PRIMITIVE_VALUE_TYPES.values()
            or value in DICT_VALUE_HEADERS):
        return it.group(1), value
    return None


def mapped_collection(body: list[str], named_ptrs: dict[int, str], enums: frozenset[str]):
    """mapped_vector's or mapped_small_dictionary's (C, element or value)."""
    return mapped_vector(body, named_ptrs, enums) or mapped_small_dictionary(body, named_ptrs, enums)


def mapper_slots(collection: str) -> tuple[str, ...]:
    """The description members CollectionTypeMapper<collection> can write."""
    return SMALL_DICT_SLOTS if SMALL_DICT_RE.fullmatch(collection) else tuple(MAPPER_SLOTS)


def call_target(orig: asmdiff.Original, addr: int) -> int | None:
    """The one function a slot calls (a `bl`, or the branch it ends with); None for none or several."""
    insns = disasm(orig, addr)
    calls = [x.operands[0].imm for x in insns
             if x.mnemonic == "bl" and x.operands[0].type == capstone.arm64.ARM64_OP_IMM]
    if insns and insns[-1].mnemonic == "b" and insns[-1].operands[0].type == capstone.arm64.ARM64_OP_IMM:
        calls.append(insns[-1].operands[0].imm)
    return calls[0] if len(calls) == 1 else None


def mapper_helpers(orig: asmdiff.Original, body: list[str], members) -> dict[str, int]:
    """{member: the CRntVector member it calls} for the members of `members` that call one."""
    slots = collection_slots(body)
    return {d: t for d in members if d in MAPPER_HELPERS and (t := call_target(orig, slots[d])) is not None}


def mapper_members(orig: asmdiff.Original, body: list[str], by_hand: set[int],
                   collection: str = "") -> frozenset[str]:
    """The description members the mapper writes for a collection: every one of mapper_slots whose slot, and
    the CRntVector member it calls, no hand-written source defines (those keep their addresses)."""
    slots = collection_slots(body)
    free = {d for d in mapper_slots(collection) if slots[d] not in by_hand}
    helpers = mapper_helpers(orig, body, free)
    return frozenset(d for d in free if helpers.get(d) not in by_hand)


def collection_ops_names(orig: asmdiff.Original, body: list[str], vector: str, element: str,
                         members) -> dict[int, str]:
    """data/functions/ names of a vector collection's mapper functions (`members`) and of the CRntVector
    members they call."""
    slots = collection_slots(body)
    out = {slots[d]: f"base::reflection::CollectionTypeMapper<{vector}>::{MAPPER_SLOTS[d]}" for d in members}
    for d, target in mapper_helpers(orig, body, members).items():
        out[target] = f"base::global::CRntVector<{element}>::{MAPPER_HELPERS[d][0]}"
    return out


def emit_collection_ops(init: int, body: list[str], vector: str, element: str, members,
                        enums: frozenset[str] = frozenset()) -> list[str]:
    """A vector collection's description with the mapper's functions (CollectionMapperDescription<C>) for
    `members`, the others by address; the CRntVector members they call are declared instantiated elsewhere,
    so that only the slots are emitted."""
    rest = [line for line in body if not any(f" {d} = " in line for d in members)]
    out = emit_collection(init, rest)
    desc = f"struct base::reflection::CollectionTypeDescription<UnkRefl_fn_{init:x}> {{"
    small_dict = SMALL_DICT_RE.fullmatch(vector) is not None
    base = "SmallDictionaryMapperDescription" if small_dict else "CollectionMapperDescription"
    out[out.index(desc)] = (f"struct base::reflection::CollectionTypeDescription<UnkRefl_fn_{init:x}> "
                            f": base::reflection::{base}<{vector}> {{")
    node = pointer_value_type(f"base::reflection::CollectionTypeMapper<{vector}>::TElementIterator")[1]
    helpers = sorted({"extern template " + MAPPER_HELPERS[d][1].format(C=vector, E=element) + ";"
                      for d in members if d in MAPPER_HELPERS})
    headers = {**DICT_VALUE_HEADERS} if small_dict else CLASS_ELEMENT_HEADERS
    include = (['#include "base/global/CRntSmallDictionary.h"'] if small_dict else []) + (
        [f'#include "{headers[element]}"'] if element in headers else [])
    return type_declarations(node, enums) + include + helpers + out


def defined_by_hand() -> set[int]:
    """Functions defined in src/ outside the generators' outputs (tools/gen_lift.py's scan): a slot one
    of them defines keeps its address."""
    import gen_lift
    return gen_lift.defined_outside(gen_lift.OUT, also=gen_lift.TAGGED_DIRS)


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


def flagset_definition(name: str) -> str:
    """A flag set's value type: its 32 bits, zero by default and compared whole. Only the alias name
    survives in the game (its CStrId), so the value is a struct of that name."""
    scope, _, leaf = name.rpartition("::")
    return declare_in(scope, f"struct {leaf} {{ u32 bits = 0; bool operator==(const {leaf}& other) const "
                             f"{{ return bits == other.bits; }} }};")


def emit_flagset_ops(init: int, name: str, obj: int, body: list[str]) -> list[str]:
    """A flag set's description and value operations, named by the flag set (ValueOps.h)."""
    rest = [line for line in body if " UnkFn " not in line]
    return [flagset_definition(name),
            "template <>",
            f"const CType& base::reflection::ValueOps<{name}>::TypeOf(const void*) {{ return g_unk_{obj:x}; }}",
            f"template struct base::reflection::ValueOps<{name}>;",
            "template <>",
            f"struct base::reflection::FlagsetTypeDescription<{name}> "
            f": base::reflection::ValueOpsDescription<{name}> {{",
            *rest,
            "};",
            f"template base::reflection::CFlagsetType::CFlagsetType(const base::global::CStrId&, {name}*);"]


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
        member = PRIMITIVE_SLOTS[off]
        if v == ("imm", 0):
            lines.append(f"    static constexpr UnkFn {member} = nullptr;")
        elif v and v[0] == "fn":
            decls.add(f"void fn_{v[1]:x}();")
            lines.append(f"    static constexpr UnkFn {member} = &fn_{v[1]:x};")
        else:
            return None
    return lines


# A plain type's CType slots by role (UnkReflPrimitiveType.h): the value operations and CType+0x70.
PRIMITIVE_SLOTS = {**{off: SLOTS[off] for off in VALUE_OPS}, 0x70: "add_attributes"}

# The plain types, by the name the game gives them, whose value operations come from ValueOps.h:
# the C++ arithmetic types.
PRIMITIVE_VALUE_TYPES = {name: name for name in (
    "bool", "char", "signed char", "unsigned char", "short", "unsigned short", "int", "long", "long long",
    "unsigned long", "unsigned long long", "float", "double")}
PRIMITIVE_VALUE_TYPES["unsigned"] = "unsigned int"


def primitive_value_type(body: list[str]) -> str | None:
    """The C++ type a plain type's traits describe, when its value operations can come from
    ValueOps.h (PRIMITIVE_VALUE_TYPES) and every slot is a function."""
    m = re.search(r'static constexpr const char\* name = "([^"]*)";', "\n".join(body))
    if not m or m.group(1) not in PRIMITIVE_VALUE_TYPES:
        return None
    if any(f" {SLOTS[off]} = nullptr;" in line for line in body for off in VALUE_OPS):
        return None
    return PRIMITIVE_VALUE_TYPES[m.group(1)]


def emit_primitive(getter: int, body: list[str], value_type: str | None = None) -> list[str]:
    """A plain type's traits and getter; with `value_type`, its CType slots come from
    ValueOps<value_type> (ValueOps.h)."""
    head = f"struct base::reflection::UnkReflPrimitiveTraits<UnkRefl_fn_{getter:x}> {{"
    ops = []
    if value_type:
        slot_members = {f" {SLOTS[off]} = " for off in VALUE_OPS}
        body = [line for line in body if not any(m in line for m in slot_members)]
        head = (f"struct base::reflection::UnkReflPrimitiveTraits<UnkRefl_fn_{getter:x}> "
                f": base::reflection::ValueOpsDescription<{value_type}> {{")
        ops = ["template <>",
               f"const CType& base::reflection::ValueOps<{value_type}>::TypeOf(const void*) "
               f"{{ return base::reflection::refl_GetPrimitiveType<UnkRefl_fn_{getter:x}>(); }}",
               f"template struct base::reflection::ValueOps<{value_type}>;"]
    return [f"struct UnkRefl_fn_{getter:x};",
            *ops,
            "template <>",
            head,
            *body,
            "};",
            f"template const base::reflection::CType& "
            f"base::reflection::refl_GetPrimitiveType<UnkRefl_fn_{getter:x}>();"]


def primitive_ops_names(body: list[str], value_type: str) -> dict[int, str]:
    """data/functions/ names of a plain type's value operations, read from its address-form traits."""
    out = {}
    for off in VALUE_OPS:
        m = re.search(rf" {SLOTS[off]} = &fn_([0-9a-f]+);", "\n".join(body))
        if m:
            out[int(m.group(1), 16)] = f"base::reflection::ValueOps<{value_type}>::{VALUE_OPS[off]}"
    return out


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


PRELUDE = ['#include "base/global/CRntVector.h"',
           '#include "base/reflection/CCollectionType.h"',
           '#include "base/reflection/CEnumType.h"',
           '#include "base/reflection/CFlagsetType.h"',
           '#include "base/reflection/CollectionTypeMapper.h"',
           '#include "base/reflection/CPointerType.h"',
           '#include "base/reflection/UnkReflPrimitiveType.h"',
           '#include "base/reflection/ValueOps.h"',
           "using base::reflection::CType;",
           "using base::reflection::UnkFn;"]
NAME_RE = re.compile(r"\b(?:fn|g_unk)_[0-9a-f]{10}\b")


# The declarations of the types a named form uses (type_declarations, enum_declaration), gathered at
# the top of each file.
TYPE_DECL_RE = re.compile(r"^(?:namespace [\w:]+ \{ )?(?:template <[^>]*> )?(?:class|enum class) \w+(?: : u32)?;(?: \})?$")


def write_chunks(prefix: str, blocks: list[list[str]], decls: set[str], header: list[str]) -> None:
    for k in range(0, len(blocks), CHUNK):
        lines = [line for b in blocks[k:k + CHUNK] for line in b]
        body = [line for line in lines if not TYPE_DECL_RE.match(line)]
        used = used_decls(body, decls) + sorted({line for line in lines if TYPE_DECL_RE.match(line)})
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
    # A pointer type's name may mention an enum (a dictionary of them): declared as the enum it is.
    enum_names_known = frozenset(name for name, _ in named.values())
    counts: dict[str, int] = {}
    for name, _ in named.values():
        counts[name] = counts.get(name, 0) + 1
    named = {i: v for i, v in named.items() if counts[v[0]] == 1}
    for _, o in named.values():
        decls.add(f"extern base::reflection::CType g_unk_{o:x};")

    named_flags = enum_names(orig, args.version, flagsets)  # the same slots, named the same way
    named_flags = {i: v for i, v in named_flags.items()
                   if v[0] not in enum_names_known and list(named_flags.values()).count(v) == 1}
    for _, o in named_flags.values():
        decls.add(f"extern base::reflection::CType g_unk_{o:x};")
    named_ptrs = named_pointers(orig, pairs, enum_names_known | {name for name, _ in named_flags.values()})
    named_prims = {g: v for g, b in prims if (v := primitive_value_type(b))}
    prim_bodies = dict(prims)
    known_types = enum_names_known | {name for name, _ in named_flags.values()}
    by_hand = defined_by_hand()
    # (C, element, the members the mapper writes); a slot a hand-written source defines keeps its address
    named_colls = {i: (*v, m) for i, b in colls if (v := mapped_collection(b, named_ptrs, enum_names_known))
                   and (m := mapper_members(orig, b, by_hand, v[0]))}
    coll_bodies = dict(colls)

    def collection_block(init: int, body: list[str]) -> list[str]:
        if init in named_colls:
            return emit_collection_ops(init, body, *named_colls[init], known_types)
        return emit_collection(init, body)

    def flagset_block(init: int, body: list[str]) -> list[str]:
        if init in named_flags:
            return emit_flagset_ops(init, *named_flags[init], body)
        return emit_flagset(init, body)

    def pointer_block(getter: int, name: str, init: int, body: list[str]) -> list[str]:
        if getter in named_ptrs:
            return emit_pointer_ops(getter, name, init, body, named_ptrs[getter], enum_names_known)
        return emit(getter, name, init, body)

    def enum_block(init: int, body: list[str]) -> list[str]:
        if init in named:
            name, o = named[init]
            return [enum_declaration(name)] + emit_enum_ops(init, name, o, body)
        return emit_enum(init, body)

    work = ROOT / "build" / "gen_types"
    work.mkdir(parents=True, exist_ok=True)
    src, obj = work / "candidates.cpp", work / "candidates.o"
    for attempt in range(2):
        text = (PRELUDE + sorted(decls) + [line for p in pairs for line in pointer_block(*p)]
                + [line for i, b in enums for line in enum_block(i, b)]
                + [line for i, b in colls for line in collection_block(i, b)]
                + [line for g, b in prims for line in emit_primitive(g, b, named_prims.get(g))]
                + [line for p in inline_ptrs for line in emit_inline_pointer(*p)]
                + [line for i, b in flagsets for line in flagset_block(i, b)])
        src.write_text("\n".join(text) + "\n", encoding="utf-8")
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0:
            raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
        # The named forms' symbols carry no address: they map through the names data/functions/
        # will give them.
        expected = {n: a for i, (name, _) in named.items() for a, n in enum_ops_names(orig, i, name).items()}
        expected.update({n: a for i, (name, _) in named_flags.items() for a, n in enum_ops_names(orig, i, name).items()})
        inits = {g: i for g, _, i, _ in pairs}
        expected.update({n: a for g, canon in named_ptrs.items()
                         for a, n in pointer_ops_names(orig, inits[g], canon).items()})
        expected.update({n: a for g, v in named_prims.items() for a, n in primitive_ops_names(prim_bodies[g], v).items()})
        coll_names = {i: collection_ops_names(orig, coll_bodies[i], *v) for i, v in named_colls.items()}
        expected.update({n: a for names in coll_names.values() for a, n in names.items()})
        expected.update({alias: a for n, a in list(expected.items()) for alias in asmdiff.name_aliases(n)})
        symbols = [f.symbol for f in asmdiff.read_object(obj)]
        demangled = asmdiff.demangle(symbols)
        # strict: a byte match whose relocations name the wrong callee, global or GOT slot is rejected
        results = asmdiff.diff_object(obj, orig, resolve=lambda sym: expected.get(demangled.get(sym, sym)),
                                      strict=True)
        ok = {res.address for res in results if res.matching}
        failed = {i for i, (name, _) in named.items() if not set(enum_ops_names(orig, i, name)) <= ok}
        failed_ptrs = {g for g, canon in named_ptrs.items()
                       if not set(pointer_ops_names(orig, inits[g], canon)) | {g, inits[g]} <= ok}
        failed_flags = {i for i, (name, _) in named_flags.items() if not set(enum_ops_names(orig, i, name)) <= ok}
        failed_prims = {g for g, v in named_prims.items()
                        if not set(primitive_ops_names(prim_bodies[g], v)) | {g} <= ok}
        # the helpers are not emitted: the slots that call them match only if the call names them
        # {collection: the mapper members that do not match}; those keep their addresses, and a collection
        # whose constructor or every member fails is kept by address whole
        failed_colls = {i: frozenset(d for d in v[2] if collection_slots(coll_bodies[i])[d] not in ok)
                        for i, v in named_colls.items()}
        failed_colls = {i: bad for i, bad in failed_colls.items() if bad or i not in ok}
        if not failed and not failed_ptrs and not failed_flags and not failed_prims and not failed_colls:
            break
        print(f"named forms that do not match, kept by address: enums {' '.join(f'{i:x}' for i in sorted(failed))}; "
              f"pointer types {' '.join(f'{g:x}' for g in sorted(failed_ptrs))}")
        named = {i: v for i, v in named.items() if i not in failed}
        named_ptrs = {g: v for g, v in named_ptrs.items() if g not in failed_ptrs}
        named_flags = {i: v for i, v in named_flags.items() if i not in failed_flags}
        named_prims = {g: v for g, v in named_prims.items() if g not in failed_prims}
        named_colls = {i: (*v[:2], v[2] - failed_colls.get(i, frozenset())) for i, v in named_colls.items()}
        named_colls = {i: v for i, v in named_colls.items() if v[2] and i in ok}
        if failed_colls:
            print("vector collection slots kept by address: " + "; ".join(
                f"{i:x} {' '.join(sorted(bad)) or '(constructor)'}" for i, bad in sorted(failed_colls.items())))
        if failed_prims:
            print(f"plain types kept by address: {' '.join(f'{g:x}' for g in sorted(failed_prims))}")
        if failed_flags:
            print(f"flag sets kept by address: {' '.join(f'{i:x}' for i in sorted(failed_flags))}")
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
    write_chunks("pointer_types", [pointer_block(*p) for p in good] + [emit_inline_pointer(*p) for p in good_inline], decls,
                 [note, "// Getters (refl_GetType<T>) and descriptor constructors (refl_InitType<T>) of pointer",
                  "// types, byte-identical to the original."])
    write_chunks("enum_types", [enum_block(*e) for e in good_enums], decls,
                 [note, "// Descriptor constructors (refl_InitType<E>) of enums, byte-identical to the original."])
    write_chunks("collection_types", [collection_block(*c) for c in good_colls], decls,
                 [note, "// Descriptor constructors of collection types (globals built by static initialisers),",
                  "// byte-identical to the original."])
    write_chunks("flagset_types", [flagset_block(*f) for f in good_flagsets], decls,
                 [note, "// Descriptor constructors of flag sets, byte-identical to the original."])
    write_chunks("primitive_types", [emit_primitive(g, b, named_prims.get(g)) for g, b in good_prims], decls,
                 [note, "// Getters (refl_GetType<T>) of plain CType descriptors, byte-identical to the original."])
    print(f"pointer types: {len(pairs)} getter/constructor candidates, {len(good)} pairs byte-identical "
          f"({sum(p[0] in named_ptrs for p in good)} with named value operations); "
          f"enums: {len(enums)} constructor candidates, {len(good_enums)} byte-identical "
          f"({sum(e[0] in named for e in good_enums)} with named value operations); "
          f"collections: {len(colls)} constructor candidates, {len(good_colls)} byte-identical "
          f"({sum(c[0] in named_colls for c in good_colls)} with the mapper's functions); "
          f"plain types: {len(prims)} getter candidates, {len(good_prims)} byte-identical "
          f"({sum(p[0] in named_prims for p in good_prims)} with named value operations); "
          f"pointer types with an inlined constructor: {len(inline_ptrs)} getter candidates, "
          f"{len(good_inline)} byte-identical; "
          f"flag sets: {len(flagsets)} constructor candidates, {len(good_flagsets)} byte-identical "
          f"({sum(f[0] in named_flags for f in good_flagsets)} with named value operations)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
