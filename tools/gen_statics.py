#!/usr/bin/env python3
"""Generate src/reflection_statics/: getters of guarded function-local statics.

Templates in the original (include/base/reflection/UnkReflStatics.h): the guard and the object
are weak, reached through the GOT, and the object's destructor is registered at exit.

  1. Candidates are the functions whose instruction sequence (mnemonics only) equals that of a
     verified reference in REFERENCES. Reference shapes are read from the binary at run time;
     nothing derived from code is stored here.
  2. Each candidate's constants are read from its code: the name passed to the string-id
     constructor, the value type getter it calls and the u32 it stores.
  3. All candidates are compiled in one scratch translation unit; only byte-identical
     functions are written out.
  4. Game code with its own guarded statics (a string id from a literal, a zeroed word) is
     lifted by tools/gen_lift.py's Lifter, taught those statics in this process (own_patch);
     only byte-identical functions are written out (own_*.cpp).

Usage: python tools/gen_statics.py --version 2.1.0
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
import refl_attrs  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "reflection_statics"
BASE = 0x7100000000
CSTRID_CTOR = 0x71000003D4
GUARD_ACQUIRE, GUARD_RELEASE, ATEXIT = 0x7101238DA0, 0x7101238DB0, 0x7100000250
SINGLETON_CTOR = 0x7100031894
CHUNK = 500  # functions per generated file
NAME_RE = re.compile(r"\b(?:fn|g_unk)_[0-9a-f]{10}\b")
# kind -> verified reference address
REFERENCES = {
    "key": 0x7100007E80,   # refl_GetAttrKey: "Description"
    # The same for keys whose u32 is 1: it shares a register with the string-id flag.
    "key_one": 0x710008FA30,  # refl_GetAttrKey (its family includes "EventGroup", fn_7100090d50)
    "strid": 0x7100302290,  # refl_GetStaticStrId: "ABILITY"
    "strid_copy": 0x71004F0F04,  # refl_GetStaticStrIdCopy
    "find": 0x7100286AD4,  # refl_FindTyped
    "register": 0x71004F1338,  # refl_Register
    "registry_map": 0x71004F0648,  # refl_RegistryMapInit (uses the registry of fn_71004f1338)
    # A static string id with internal linkage (a plain function; object and guard adjacent).
    "strid_local": 0x7100304140,
    "send_type_id": 0x71003D3CF8,  # refl_SendTypeId
    "singleton_part": 0x71003B27B4,  # refl_GetSingletonPart
    "find_hidden": 0x71004EEA08,  # refl_FindTypedHidden
    "key_flags": 0x710000D120,  # refl_GetAttrKeyFlags
    "key_flags_b": 0x710000BF80,
    "key_flags_c": 0x7100005070,
    "key_global": 0x710008FF80,  # refl_GetAttrKey with a global value type
}
KNOWN_CALLS = (CSTRID_CTOR, GUARD_ACQUIRE, GUARD_RELEASE, ATEXIT,
               0x71000803A4,  # the string pool's release
               0x7100083990)  # the default allocator

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
CS.detail = True


def shape(orig: asmdiff.Original, addr: int) -> list[str]:
    return [x.mnemonic for x in CS.disasm(orig.function(addr), addr)]


def facts(orig: asmdiff.Original, addr: int) -> dict | None:
    """The name string, the first call after the string-id constructor and the u32 stored at
    +0x10 of the object."""
    page: dict[str, int] = {}
    strings: dict[str, str] = {}
    imms: dict[str, int] = {}
    out: dict = {"calls": []}
    got: dict[str, int] = {}
    after_ctor = False
    prev: tuple[str, str] | None = None
    for x in CS.disasm(orig.function(addr), addr):
        m, o = x.mnemonic, x.operands
        cur = prev
        prev = (m, CS.reg_name(o[0].reg)) if o and o[0].type == arm64.ARM64_OP_REG else None
        if m == "ldarb" and CS.reg_name(o[1].mem.base) in got:
            out.setdefault("guard", got[CS.reg_name(o[1].mem.base)])
        if m == "adrp":
            page[CS.reg_name(o[0].reg)] = o[1].imm
        elif m == "add" and len(o) == 3 and CS.reg_name(o[1].reg) in page and o[2].type == arm64.ARM64_OP_IMM:
            target = page[CS.reg_name(o[1].reg)] + o[2].imm
            s = orig.img.cstring(target - BASE)
            if s is not None:
                strings[CS.reg_name(o[0].reg)] = s
            if CS.reg_name(o[0].reg) == "x1":
                out.setdefault("x1_adds", []).append(target)
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0                 and CS.reg_name(o[1].mem.base) in got:
            out.setdefault("source", got[CS.reg_name(o[1].mem.base)])
            got.pop(CS.reg_name(o[0].reg), None)
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and CS.reg_name(o[1].mem.base) in page:
            t = orig.img.pointer(page.pop(CS.reg_name(o[1].mem.base)) + o[1].mem.disp - BASE)
            if t is not None:
                got[CS.reg_name(o[0].reg)] = BASE + t
                if CS.reg_name(o[0].reg) == "x1":
                    out.setdefault("x1_got", BASE + t)
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and CS.reg_name(o[1].mem.base) == "x0":
            out.setdefault("x0_disp", o[1].mem.disp)
        elif m == "mov" and len(o) == 2 and o[1].type == arm64.ARM64_OP_IMM:
            imms[CS.reg_name(o[0].reg)] = o[1].imm
            out.setdefault("first_imm", o[1].imm)
        elif m == "str" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0x10 \
                and CS.reg_name(o[0].reg) in imms:
            out["u32"] = imms[CS.reg_name(o[0].reg)] & 0xFFFFFFFF
        elif m == "str" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 8 \
                and CS.reg_name(o[0].reg) in got and cur == ("ldr", CS.reg_name(o[0].reg)):
            out["type_global"] = got[CS.reg_name(o[0].reg)]
        elif m == "strh" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0x14 \
                and CS.reg_name(o[0].reg) in imms:
            out["u16"] = imms[CS.reg_name(o[0].reg)] & 0xFFFF
        elif m == "str" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0x18                 and CS.reg_name(o[0].reg) in imms:
            out["flags"] = imms[CS.reg_name(o[0].reg)] & 0xFFFFFFFF
        elif m == "bl":
            out["calls"].append(o[0].imm)
            if o[0].imm == 0x71000803A4:
                continue  # the string pool's release, after the key's type
            if o[0].imm == CSTRID_CTOR and "name" not in out:
                if "x1" not in strings:
                    return None
                out["name"] = strings["x1"]
                after_ctor = True
            elif after_ctor and "type" not in out:
                out["type"] = o[0].imm
    return out


def key_type(f: dict, decls: set[str]) -> str | None:
    """An attribute key's value type: a global descriptor stored directly, or a getter's result."""
    if "type_global" in f:
        decls.add(f"extern const base::reflection::CType g_unk_{f['type_global']:x};")
        return f"&g_unk_{f['type_global']:x}"
    if "type" in f:
        decls.add(f"const base::reflection::CType* fn_{f['type']:x}();")
        return f"fn_{f['type']:x}()"
    return None


def emit(kind: str, addr: int, f: dict, decls: set[str], registries: dict[int, int]) -> list[str] | None:
    """registries: guard address -> address of the refl_Register candidate using it."""
    tag = f"UnkRefl_fn_{addr:x}"
    head = [f"struct {tag};", "template <>", f"struct base::reflection::UnkReflStatic<{tag}> {{"]
    if kind == "strid_local":
        if "name" not in f:
            return None
        return [f"const base::global::CStrId& fn_{addr:x}() {{",
                f"    static base::global::CStrId s_id({refl_attrs.c_string(f['name'])}, true);",
                "    return s_id;", "}"]
    if kind == "send_type_id":
        if "x1_got" not in f or "x0_disp" not in f:
            return None
        decls.add(f"extern u8 g_unk_{f['x1_got']:x};")
        return head + [f"    static constexpr u32 offset = {f['x0_disp']:#x};",
                       f"    static const void* key() {{ return &g_unk_{f['x1_got']:x}; }}", "};",
                       f"template void base::reflection::refl_SendTypeId<{tag}>(void*);"]
    if kind == "find_hidden":
        calls = [c for c in f["calls"] if c not in KNOWN_CALLS]
        if len(calls) < 3 or len(f.get("x1_adds", [])) != 2 or "name" not in f:
            return None
        head.append(f"    static constexpr const char* name = {refl_attrs.c_string(f['name'])};")
        find, init, isa = calls[:3]
        cls = f["x1_adds"][0]
        decls |= {f"base::reflection::UnkReflObject* fn_{find:x}(void* owner, const base::global::CStrId& name);",
                  f"const base::reflection::CClass* fn_{init:x}();",
                  f"bool fn_{isa:x}(const base::reflection::CClass* cls, const base::reflection::CClass* base);",
                  f'extern base::reflection::CClass g_unk_{cls:x} __attribute__((visibility("hidden")));'}
        return head + [f"    static UnkReflObject* find(void* o, const base::global::CStrId& n) {{ return fn_{find:x}(o, n); }}",
                       f"    static void init() {{ fn_{init:x}(); }}",
                       f"    static constexpr const CClass& type = g_unk_{cls:x};",
                       f"    static bool is_a(const CClass* c, const CClass* b) {{ return fn_{isa:x}(c, b); }}", "};",
                       f"template base::reflection::UnkReflObject* base::reflection::refl_FindTypedHidden<{tag}>(void*);"]
    if kind == "singleton_part":
        if "first_imm" not in f or f["calls"][1:2] != [SINGLETON_CTOR]:
            return None
        return head + [f"    static constexpr u32 offset = {f['first_imm']:#x};", "};",
                       f"template void* base::reflection::refl_GetSingletonPart<{tag}>();"]
    if kind == "registry_map":
        calls = [c for c in f["calls"] if c not in KNOWN_CALLS]
        if len(calls) != 2 or "guard" not in f:
            return None
        construct, grow = calls
        decls |= {f"void fn_{grow:x}(base::reflection::UnkReflRegistryMap::Value** values, u32 n);",
                  f"void fn_{construct:x}(base::reflection::UnkReflRegistryMap::Value* value, "
                  f"base::reflection::UnkReflRegistryItem* item, const base::global::CStrId& name);"}
        reg = registries.get(f["guard"])
        if reg is not None:
            head = [f"struct UnkRefl_fn_{reg:x};"] + head + [f"    using Registry = UnkRefl_fn_{reg:x};"]
        elif "flags" in f:
            head += [f"    using Registry = {tag};", f"    static constexpr u32 flags = {f['flags']:#x};"]
        else:
            return None
        return head + ["    static void grow(UnkReflRegistryMap::Value** v, u32 n) "
                       f"{{ fn_{grow:x}(v, n); }}",
                       "    static void construct(UnkReflRegistryMap::Value* v, UnkReflRegistryItem* i, "
                       f"const base::global::CStrId& n) {{ fn_{construct:x}(v, i, n); }}", "};",
                       f"template bool base::reflection::refl_RegistryMapInit<{tag}>("
                       "base::reflection::UnkReflRegistryMap*, const base::global::CStrId&);"]
    if kind == "strid_copy":
        if "source" not in f:
            return None
        decls.add(f"extern base::global::CStrId g_unk_{f['source']:x};")
        return head + [f"    static const base::global::CStrId& source() {{ return g_unk_{f['source']:x}; }}", "};",
                       f"template const base::global::CStrId& base::reflection::refl_GetStaticStrIdCopy<{tag}>();"]
    if kind == "register":
        if "flags" not in f:
            return None
        return head + [f"    static constexpr u32 flags = {f['flags']:#x};", "};",
                       f"template void base::reflection::refl_Register<{tag}>(void*);"]
    if "name" not in f:
        return None
    head.append(f"    static constexpr const char* name = {refl_attrs.c_string(f['name'])};")
    if kind == "find":
        calls = [c for c in f["calls"] if c not in (CSTRID_CTOR, GUARD_ACQUIRE, GUARD_RELEASE, ATEXIT)]
        if len(calls) < 3:
            return None
        find, typ, isa = calls[:3]
        decls |= {f"base::reflection::UnkReflObject* fn_{find:x}(void* owner, const base::global::CStrId& name);",
                  f"const base::reflection::CClass* fn_{typ:x}();",
                  f"bool fn_{isa:x}(const base::reflection::CClass* cls, const base::reflection::CClass* base);"}
        return head + [f"    static UnkReflObject* find(void* o, const base::global::CStrId& n) {{ return fn_{find:x}(o, n); }}",
                       f"    static const CClass* type() {{ return fn_{typ:x}(); }}",
                       f"    static bool is_a(const CClass* c, const CClass* b) {{ return fn_{isa:x}(c, b); }}", "};",
                       f"template base::reflection::UnkReflObject* base::reflection::refl_FindTyped<{tag}>(void*);"]
    if kind.startswith("key_flags"):
        if "u32" not in f or "u16" not in f or not (ty := key_type(f, decls)):
            return None
        return head + [f"    static const CType* type() {{ return {ty}; }}",
                       f"    static constexpr u32 field_0x10 = {f['u32']:#x};",
                       f"    static constexpr u16 field_0x14 = {f['u16']:#x};", "};",
                       f"template const base::reflection::UnkReflAttrKey& "
                       f"base::reflection::refl_GetAttrKeyFlags<{tag}>();"]
    if kind in ("key", "key_one", "key_global"):
        if "u32" not in f or not (ty := key_type(f, decls)):
            return None
        return head + [f"    static const CType* type() {{ return {ty}; }}",
                       f"    static constexpr u32 field_0x10 = {f['u32']:#x};", "};",
                       f"template const base::reflection::UnkReflAttrKey& "
                       f"base::reflection::refl_GetAttrKey<{tag}>();"]
    return head + ["};", f"template const base::global::CStrId& base::reflection::refl_GetStaticStrId<{tag}>();"]


PRELUDE = ['#include "base/reflection/UnkReflStatics.h"', "using base::reflection::CClass;",
           "using base::reflection::CType;", "using base::reflection::UnkReflObject;",
           "using base::reflection::UnkReflRegistryItem;", "using base::reflection::UnkReflRegistryMap;"]

# Game code that sends a u32 under a hash (fn_7101176690, with the type id's statics inlined):
# a short chain of loads from `self` (optionally null-checked) gives the target, the hash is a
# constant or the second parameter, the value a constant. Parsed from the path up to the call;
# each candidate is compiled and kept only if byte-identical.
EMIT_U32 = 0x7101176690
TYPE_ID_GUARDS = (0x7101CBBCD8, 0x7101CBBCE0)  # GOT slots of the type id's guard and value


def parse_emit_u32(orig: asmdiff.Original, addr: int) -> dict | None:
    ins = list(CS.disasm(orig.function(addr), addr))
    bls = [x.operands[0].imm for x in ins if x.mnemonic == "bl"]
    if not bls or bls.count(EMIT_U32) != 1 or any(b not in (EMIT_U32, GUARD_ACQUIRE, GUARD_RELEASE) for b in bls):
        return None
    if any(x.mnemonic in ("blr", "br") for x in ins):
        return None
    regs: dict[str, object] = {"x0": ("self",), "x1": ("hash",)}
    loads: list[tuple] = []  # load expressions in order
    checks: set[tuple] = set()
    consts: dict[str, int] = {}
    value = None
    hash_const = None
    page: dict[str, int] = {}
    got: dict[str, int] = {}
    seen: set[int] = set()
    rets = [x.address for x in ins if x.mnemonic == "ret"]
    if len(rets) != 1:
        return None
    for i, x in enumerate(ins):
        m, o = x.mnemonic, x.operands
        r0 = CS.reg_name(o[0].reg) if o and o[0].type == arm64.ARM64_OP_REG else None
        if m == "bl":
            break
        if m in ("stp", "ldp") or (m in ("sub", "add") and r0 in ("sp", "x29", "fp", "x3", "x6")
                                        and CS.reg_name(o[1].reg) in ("sp", "x29", "fp")):
            continue
        if m == "str" and r0 in ("x19", "x20") and "sp" in x.op_str:
            continue
        if m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.index == 0:
            base = CS.reg_name(o[1].mem.base)
            if base in regs and isinstance(regs[base], tuple) and regs[base][0] in ("self", "load"):
                e = ("load", regs[base], o[1].mem.disp)
                regs[r0] = e
                loads.append(e)
                continue
            if base == "x9" and o[1].mem.disp == 0:
                regs[r0] = None
                continue
            if base in page:
                slot = page.pop(base) + o[1].mem.disp
                t = orig.img.pointer(slot - BASE)
                if t is None:
                    return None
                got[r0] = BASE + t
                seen.add(slot)
                regs[r0] = None
                continue
            if r0 == "w8" and base == "x9" and o[1].mem.disp == 0:
                continue
            return None
        if m == "mov" and len(o) == 2:
            if o[1].type == arm64.ARM64_OP_IMM:
                consts[r0] = o[1].imm
                regs[r0] = None
                continue
            src = CS.reg_name(o[1].reg)
            if src == "wzr":
                consts[r0] = 0
                continue
            if src in regs:
                regs[r0] = regs[src]
                continue
            return None
        if m == "movk":
            if r0 not in consts:
                return None
            sh = o[1].shift.value if o[1].shift.type else 0
            consts[r0] = (consts[r0] & ~(0xFFFF << sh)) | (o[1].imm << sh)
            continue
        if m in ("stur", "str") and o[1].type == arm64.ARM64_OP_MEM and CS.reg_name(o[1].mem.base) in ("x29", "fp", "sp"):
            if r0 == "wzr":
                value = 0 if value is None else value
            elif r0 == "w8" and ins[i - 1].mnemonic == "ldr" and CS.reg_name(ins[i - 1].operands[0].reg) == "w8" \
                    and CS.reg_name(ins[i - 1].operands[1].mem.base) == "x9":
                pass  # the type id
            elif r0 in consts and value is None:
                value = consts[r0]
            else:
                return None
            continue
        if m == "cbz" and r0 and r0.startswith("x") and isinstance(regs.get(r0), tuple):
            checks.add(regs[r0])
            continue
        if m == "adrp":
            page[r0] = o[1].imm
            regs.pop(r0, None)
            continue
        if m in ("ldarb", "tbz"):
            continue
        return None
    else:
        return None
    if "x4" not in got or "x5" not in got or not seen >= set(TYPE_ID_GUARDS):
        return None
    if "x1" in consts:
        hash_const = consts["x1"] & 0xFFFFFFFFFFFFFFFF
    elif regs.get("x1") != ("hash",):
        return None
    target = regs.get("x0")
    if not isinstance(target, tuple) or value is None or consts.get("x2") is None:
        return None
    k = next(i for i, x in enumerate(ins) if x.mnemonic == "bl")
    tail = ins[k + 1:next(i for i in range(k, len(ins)) if ins[i].mnemonic == "ret")]
    returns_bool = any(x.mnemonic == "and" and x.op_str == "w0, w0, #1" for x in tail)
    return {"target": target, "checks": checks, "hash": hash_const, "value": value & 0xFFFFFFFF,
            "kind": consts["x2"] & 0xFFFFFFFFFFFFFFFF, "bool": returns_bool, "a": got["x4"], "b": got["x5"]}


def emit_emit_u32(addr: int, f: dict) -> list[str]:
    fail = "false" if f["bool"] else ""
    lines: list[str] = []
    names: dict[tuple, str] = {("self",): "self"}

    def walk(e: tuple) -> str:
        if e in names:
            return names[e]
        base = walk(e[1])
        n = f"p{len(names)}"
        names[e] = n
        lines.append(f"    char* {n} = *reinterpret_cast<char**>({base} + {e[2]:#x});")
        if e in f["checks"]:
            lines.append(f"    if (!{n})")
            lines.append(f"        return {fail};".replace(" ;", ";"))
        return n

    target = walk(f["target"])
    params = "char* self" + (", u64 hash" if f["hash"] is None else "")
    h = "hash" if f["hash"] is None else f"{f['hash']:#x}"
    call = (f"base::reflection::refl_EmitU32({target}, {h}, {f['kind']:#x}, {f['value']:#x}, "
            f"&g_unk_{f['a']:x}, &g_unk_{f['b']:x});")
    ret = "bool" if f["bool"] else "void"
    return [f"{ret} fn_{addr:x}({params}) {{"] + lines + [f"    {'return ' if f['bool'] else ''}{call}", "}"]


def is_instantiation(line: str) -> bool:
    return line.startswith("template ") and not line.startswith("template <>")


def layout(addrs: list[int], blocks: dict[int, list[str]], uses: dict[int, int]) -> list[str]:
    """Specialisations first, then the explicit instantiations, so that a registry map's
    instantiation sees its registry's specialisation (copied in when that one is elsewhere)."""
    order = addrs + sorted({uses[a] for a in addrs if a in uses} - set(addrs))
    specs = [ln for a in order for ln in blocks[a] if not is_instantiation(ln)]
    return specs + [ln for a in addrs for ln in blocks[a] if is_instantiation(ln)]


# -- Functions with their own guarded statics, through tools/gen_lift.py --------------------
# The lifter inlines the GUARDS helpers; these are statics local to the function itself: a
# string id (or file-path id) built from a literal, or a word set to zero. The guard's ldarb,
# its tbz (with up to four unrelated instructions scheduled between) and the initialisation
# reached from the tbz become one statement declaring the static; its address is a local the
# rest of the body uses. Weak statics (through the GOT) belong to an inline function.

OWN_MAX_SIZE = 1024  # larger ones lift but none matched, and lifting them took most of the run
OWN_CTORS = {0x71000003D4: "StrId", 0x71000166C8: "FilePathId"}
OWN_HEADER = ["// Function-local statics (gen_statics.py).", "namespace lift_static {",
              "struct StrId { void* data; StrId(const char* s, bool f); ~StrId(); };",
              "struct FilePathId { void* data; FilePathId(const char* s, bool f); ~FilePathId(); };",
              "struct ZeroWord { u32 v; ZeroWord() : v(0) {} };", "}", ""]


def _own_region(gl, ins, at, t):
    """The instructions reached from the tbz's target until they rejoin after it."""
    resume = t.address + 4
    region, todo = set(), [t.operands[2].imm]
    while todo:
        a = todo.pop()
        while a != resume and a not in region:
            if a not in at:
                raise gl.Unsupported("guard")
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
    return region


def _own_init(gl, lifter, ins, at, before, region):
    """(type, object, literal or None) from a linear walk of `before` (the straight-line code up
    to the tbz) then the region, or None if the initialisation is something else."""
    pages, regs = {}, {}
    calls, ctor, zero = [], None, None
    for a in [y.address for y in before] + sorted(region):
        y = ins[at[a]]
        m, o = y.mnemonic, y.operands
        r0 = gl.regname(o[0].reg) if o and o[0].type == arm64.ARM64_OP_REG else None
        inside = a in region
        if not inside and m in ("bl", "blr"):
            for r in range(19):
                regs.pop(f"x{r}", None)
                pages.pop(f"x{r}", None)
            continue
        if m not in ("adrp", "add", "ldr", "mov", "str", "bl"):
            for r in y.regs_access()[1]:
                n = "x" + gl.regname(r)[1:]
                regs.pop(n, None)
                pages.pop(n, None)
            continue
        if not inside and m in ("str", "bl"):
            continue
        p0, g0 = dict(pages), dict(regs)
        if m != "adrp" and r0 and m != "str":
            regs.pop(r0, None)
            pages.pop(r0, None)
        if m == "adrp":
            pages[r0] = o[1].imm
        elif m == "add" and len(o) == 3 and o[2].type == arm64.ARM64_OP_IMM and gl.regname(o[1].reg) in p0:
            regs[r0] = ("int", p0[gl.regname(o[1].reg)] + o[2].imm)
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and gl.regname(o[1].mem.base) in p0:
            p = lifter.orig.img.pointer(p0[gl.regname(o[1].mem.base)] + o[1].mem.disp - BASE)
            if p is not None:
                regs[r0] = ("got", BASE + p)
        elif m == "mov" and len(o) == 2 and o[1].type == arm64.ARM64_OP_REG:
            if g0.get(gl.regname(o[1].reg)):
                regs[r0] = g0[gl.regname(o[1].reg)]
        elif m == "str" and o[0].type == arm64.ARM64_OP_REG and gl.regname(o[0].reg) == "wzr" and \
                o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0 and g0.get(gl.regname(o[1].mem.base)):
            zero = g0[gl.regname(o[1].mem.base)]
        elif m == "bl":
            calls.append(o[0].imm)
            if o[0].imm in OWN_CTORS:
                obj, lit = regs.get("x0"), regs.get("x1")
                if ctor or not obj or not lit or lit[0] != "int":
                    return None
                s = lifter.orig.img.cstring(lit[1] - BASE)
                if s is None:
                    return None
                ctor = (OWN_CTORS[o[0].imm], obj, s)
    if not ctor and zero and set(calls) == {GUARD_ACQUIRE, GUARD_RELEASE}:
        ctor = ("ZeroWord", zero, None)
    if not ctor or GUARD_ACQUIRE not in calls or set(calls) - {GUARD_ACQUIRE, ATEXIT, GUARD_RELEASE} - set(OWN_CTORS):
        return None
    return ctor


def own_patch(gl) -> None:
    """Teach gen_lift's Lifter (in this process only) the statics above."""
    find, gref = gl.Lifter.find_guards, gl.Lifter.global_ref

    def find_guards(self, ins):
        find(self, ins)
        self.statics = {}
        at = {x.address: k for k, x in enumerate(ins)}
        for k, x in enumerate(ins):
            if x.mnemonic != "ldarb" or x.address in self.guard or x.address in self.gskip:
                continue
            w, j = x.operands[0].reg, k + 1
            while j < len(ins) and j <= k + 4 and ins[j].mnemonic not in ("tbz", "tbnz") and \
                    ins[j].mnemonic[0] not in "bc" and w not in ins[j].regs_access()[1]:
                j += 1
            if j >= len(ins):
                continue
            t = ins[j]
            if t.mnemonic != "tbz" or t.operands[0].reg != w or t.operands[1].imm != 0:
                continue
            region = _own_region(gl, ins, at, t)
            init = _own_init(gl, self, ins, at, ins[:j], region)
            if not init:
                continue
            for y in ins:  # nothing else may enter the initialisation
                if y.address not in region and y.address != t.address and \
                        (y.mnemonic in ("b", "cbz", "cbnz", "tbz", "tbnz") or y.mnemonic.startswith("b.")) and \
                        any(op.type == arm64.ARM64_OP_IMM and op.imm in region for op in y.operands):
                    raise gl.Unsupported("guard")
            typ, (kind, obj), lit = init
            name = f"s_{obj:x}"
            args = f"({refl_attrs.c_string(lit)}, true)" if lit is not None else ""
            if kind == "got":  # weak: the static of an inline function
                self.decls.add(f"namespace lift_static {{ inline {typ}& {name}() "
                               f"{{ static {typ} s{args}; return s; }} }}")
                stmt = f"g{name} = (char*)&lift_static::{name}();"
            else:
                if obj in self.statics:
                    raise gl.Unsupported("guard: object twice")
                stmt = f"static lift_static::{typ} {name}{args}; g{name} = (char*)&{name};"
            if obj not in self.statics:
                self.line(f"char* g{name};")  # at the top: gotos may jump past the statement
            self.guard[x.address] = (stmt, 0)
            self.gskip |= region | {t.address}
            self.statics[obj] = f"g{name}"

    def global_ref(self, a, through_got, align=1):
        if a in getattr(self, "statics", {}):
            return f"(char*)({self.statics[a]})"
        return gref(self, a, through_got, align)

    gl.Lifter.find_guards, gl.Lifter.global_ref = find_guards, global_ref
    gl.HEADER = gl.HEADER + OWN_HEADER


def own_candidates(gl, orig: asmdiff.Original, addrs: set[int]) -> list[int]:
    """Those of `addrs` (up to OWN_MAX_SIZE bytes) with one of these statics."""
    out = []
    for a in sorted(addrs):
        n = orig.sizes.get(a, 0)
        if not n or n > OWN_MAX_SIZE:
            continue
        code = orig.function(a)
        for k in range(0, n, 4):
            w = int.from_bytes(code[k:k + 4], "little")
            if w >> 26 == 0x25:
                off = (w & 0x3FFFFFF) - ((w & 0x2000000) << 1)
                if a + k + off * 4 == GUARD_ACQUIRE:
                    lifter = gl.Lifter(orig, a, *gl.VARIANTS[0])
                    try:
                        lifter.find_guards(list(gl.CS.disasm(code, a)))
                    except gl.Unsupported:
                        break
                    if lifter.statics:
                        out.append(a)
                    break
    return out


def own_statics(orig: asmdiff.Original, addrs: set[int]) -> tuple[list[tuple[str, set[str]]], int, int]:
    """The byte-identical lifted (definition, declarations) of `addrs` that use one of these
    statics, in address order; and the candidate count and matched bytes."""
    import gen_lift as gl
    own_patch(gl)
    cands = own_candidates(gl, orig, addrs)
    chosen, _, _ = gl.lift_rounds(orig, cands, ROOT / "build" / "gen_statics" / "own")
    keep = sorted(a for a, (src, _) in chosen.items() if "lift_static::" in src)
    return [chosen[a] for a in keep], len(cands), sum(orig.sizes[a] for a in keep)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    rows: list[int] = []
    u_rows: list[int] = []  # parsed kinds only take functions nothing else in src/ defines
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                if r["status"] in ("U", "O"):
                    rows.append(int(r["address"], 16))
                if r["status"] == "U":
                    u_rows.append(int(r["address"], 16))
    shapes = {kind: shape(orig, ref) for kind, ref in REFERENCES.items()}
    sizes = {kind: len(s) * 4 for kind, s in shapes.items()}
    blocks: dict[int, list[str]] = {}
    uses: dict[int, int] = {}  # registry map -> the refl_Register whose specialisation it needs
    decls: set[str] = set()
    counts = {kind: 0 for kind in REFERENCES}
    found: list[tuple[str, int, dict]] = []
    for a in rows:
        for kind in REFERENCES:
            if orig.sizes.get(a) == sizes[kind] and shape(orig, a) == shapes[kind]:
                f = facts(orig, a)
                if f:
                    found.append((kind, a, f))
                break
    previous = set()
    if OUT.exists():
        for f in OUT.glob("*.cpp"):
            previous |= {int(a, 16) for a in re.findall(r"^[\w*]+ fn_([0-9a-f]{10})\(", f.read_text(encoding="utf-8"), re.M)}
    undefined = set(u_rows) | previous
    for a in sorted(undefined - {a for _, a, _ in found}):
        if a not in orig.sizes:
            continue
        f = parse_emit_u32(orig, a)
        if f:
            blocks[a] = emit_emit_u32(a, f)
            decls |= {f"extern u8 g_unk_{f['a']:x};", f"extern u8 g_unk_{f['b']:x};"}
            counts["emit_u32"] = counts.get("emit_u32", 0) + 1
    # Registries first: a registry map names the refl_Register that shares its guard.
    registries = {f["guard"]: a for kind, a, f in found if kind == "register" and "guard" in f}
    for kind, a, f in found:
        b = emit(kind, a, f, decls, registries)
        if b:
            blocks[a] = b
            counts[kind] += 1
            if kind == "registry_map" and f["guard"] in registries:
                uses[a] = registries[f["guard"]]

    work = ROOT / "build" / "gen_statics"
    work.mkdir(parents=True, exist_ok=True)
    src, obj = work / "candidates.cpp", work / "candidates.o"
    src.write_text("\n".join(PRELUDE + sorted(decls) + layout(sorted(blocks), blocks, uses)) + "\n",
                   encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
    ok = sorted({res.address for res in asmdiff.diff_object(obj, orig) if res.matching} & set(blocks))

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = ["// Generated by tools/gen_statics.py from main; regenerate rather than edit.",
            "// Functions around guarded function-local statics (attribute keys, string ids, typed",
            "// lookups, registries), byte-identical to the original."]
    for k in range(0, len(ok), CHUNK):
        body = layout(ok[k:k + CHUNK], blocks, uses)
        names = set(NAME_RE.findall(" ".join(body)))
        used = sorted(d for d in decls if NAME_RE.search(d).group(0) in names)
        (OUT / f"statics_{k // CHUNK:02d}.cpp").write_text("\n".join(note + PRELUDE + used + body) + "\n",
                                                          encoding="utf-8", newline="\n")
    print(f"candidates: {counts}; {len(ok)} of {len(blocks)} byte-identical "
          f"({sum(orig.sizes[a] for a in ok)} bytes)")

    own, n, size = own_statics(orig, undefined - set(ok))
    import gen_lift as gl
    note = ["// Generated by tools/gen_statics.py (through tools/gen_lift.py) from main; regenerate",
            "// rather than edit. Functions with their own guarded statics, byte-identical to the original."]
    for k in range(0, len(own), CHUNK):
        part = own[k:k + CHUNK]
        decls = sorted(set().union(*[d for _, d in part]))  # an inline static's getter is defined once
        (OUT / f"own_{k // CHUNK:02d}.cpp").write_text(
            "\n".join(note + gl.HEADER + decls + [""] + [src + "\n" for src, _ in part]), encoding="utf-8", newline="\n")
    print(f"own statics: {len(own)} of {n} byte-identical ({size} bytes)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
