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
    for x in CS.disasm(orig.function(addr), addr):
        m, o = x.mnemonic, x.operands
        if m == "ldarb" and CS.reg_name(o[1].mem.base) in got:
            out.setdefault("guard", got[CS.reg_name(o[1].mem.base)])
        if m == "adrp":
            page[CS.reg_name(o[0].reg)] = o[1].imm
        elif m == "add" and len(o) == 3 and CS.reg_name(o[1].reg) in page and o[2].type == arm64.ARM64_OP_IMM:
            s = orig.img.cstring(page[CS.reg_name(o[1].reg)] + o[2].imm - BASE)
            if s is not None:
                strings[CS.reg_name(o[0].reg)] = s
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0                 and CS.reg_name(o[1].mem.base) in got:
            out.setdefault("source", got[CS.reg_name(o[1].mem.base)])
            got.pop(CS.reg_name(o[0].reg), None)
        elif m == "ldr" and o[1].type == arm64.ARM64_OP_MEM and CS.reg_name(o[1].mem.base) in page:
            t = orig.img.pointer(page.pop(CS.reg_name(o[1].mem.base)) + o[1].mem.disp - BASE)
            if t is not None:
                got[CS.reg_name(o[0].reg)] = BASE + t
        elif m == "mov" and len(o) == 2 and o[1].type == arm64.ARM64_OP_IMM:
            imms[CS.reg_name(o[0].reg)] = o[1].imm
        elif m == "str" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0x10 \
                and CS.reg_name(o[0].reg) in imms:
            out["u32"] = imms[CS.reg_name(o[0].reg)] & 0xFFFFFFFF
        elif m == "str" and o[1].type == arm64.ARM64_OP_MEM and o[1].mem.disp == 0x18                 and CS.reg_name(o[0].reg) in imms:
            out["flags"] = imms[CS.reg_name(o[0].reg)] & 0xFFFFFFFF
        elif m == "bl":
            out["calls"].append(o[0].imm)
            if o[0].imm == CSTRID_CTOR and "name" not in out:
                if "x1" not in strings:
                    return None
                out["name"] = strings["x1"]
                after_ctor = True
            elif after_ctor and "type" not in out:
                out["type"] = o[0].imm
    return out


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
    if kind in ("key", "key_one"):
        if "type" not in f or "u32" not in f:
            return None
        decls.add(f"const base::reflection::CType* fn_{f['type']:x}();")
        return head + [f"    static const CType* type() {{ return fn_{f['type']:x}(); }}",
                       f"    static constexpr u32 field_0x10 = {f['u32']:#x};", "};",
                       f"template const base::reflection::UnkReflAttrKey& "
                       f"base::reflection::refl_GetAttrKey<{tag}>();"]
    return head + ["};", f"template const base::global::CStrId& base::reflection::refl_GetStaticStrId<{tag}>();"]


PRELUDE = ['#include "base/reflection/UnkReflStatics.h"', "using base::reflection::CClass;",
           "using base::reflection::CType;", "using base::reflection::UnkReflObject;",
           "using base::reflection::UnkReflRegistryItem;", "using base::reflection::UnkReflRegistryMap;"]


def is_instantiation(line: str) -> bool:
    return line.startswith("template ") and not line.startswith("template <>")


def layout(addrs: list[int], blocks: dict[int, list[str]], uses: dict[int, int]) -> list[str]:
    """Specialisations first, then the explicit instantiations, so that a registry map's
    instantiation sees its registry's specialisation (copied in when that one is elsewhere)."""
    order = addrs + sorted({uses[a] for a in addrs if a in uses} - set(addrs))
    specs = [ln for a in order for ln in blocks[a] if not is_instantiation(ln)]
    return specs + [ln for a in addrs for ln in blocks[a] if is_instantiation(ln)]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
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
    return 0


if __name__ == "__main__":
    sys.exit(main())
