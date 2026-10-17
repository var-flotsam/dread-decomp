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
POINTER_KIND = "base::reflection::CPointerType"
ENUM_KIND = "base::reflection::CEnumType"
REF_ENUM = 0x71000145E8         # a CEnumType constructor
CHUNK = 200  # getter/constructor pairs per generated file

# CType/CPointerType slots filled by the constructor: offset -> traits member.
SLOTS = {0x28: "fn_0x28", 0x30: "fn_0x30", 0x38: "fn_0x38", 0x40: "fn_0x40", 0x48: "fn_0x48",
         0x50: "fn_0x50", 0x58: "fn_0x58", 0x60: "fn_0x60", 0x68: "fn_0x68", 0xF0: "fn_0xf0",
         0xF8: "fn_0xf8", 0x100: "fn_0x100"}

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


def emulate(orig: asmdiff.Original, addr: int) -> tuple[str | None, dict, int | None]:
    """A descriptor constructor's kind string, its stores to the object and its tail call.

    Stores are keyed by offset from the object (x19, including pre-index writebacks):
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
                val[reg(x, o[0])] = ("fn", t) if t in orig.sizes else ("data", t)
            continue
        if m in ("mov", "movz") and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_IMM:
            val[reg(x, o[0])] = ("imm", o[1].imm & (2**64 - 1))
            continue
        if m == "mov" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_REG and reg(x, o[0]) != "x19":
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
        if m in ("str", "stp") and CS.reg_name(o[-1].mem.base) == "x19":
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


def traits(orig: asmdiff.Original, init: int, decls: set[str]) -> list[str] | None:
    kind, st, _ = emulate(orig, init)
    if kind != POINTER_KIND:
        return None
    size = st.get(0x18)
    if not size or size[0] != "imm":
        return None
    lines = [f"    static constexpr u64 size = {size[1]:#x};"]
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
    if p and p[0] == "call":
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
            f"struct base::reflection::UnkReflPointerTraits<UnkRefl_fn_{init:x}> {{",
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
            f"struct base::reflection::UnkReflEnumTraits<UnkRefl_fn_{init:x}> {{",
            *body,
            "};",
            f"template base::reflection::CEnumType::CEnumType(const base::global::CStrId&, const void*, "
            f"UnkRefl_fn_{init:x}*);"]


PRELUDE = ['#include "base/reflection/CEnumType.h"',
           '#include "base/reflection/CPointerType.h"',
           "using base::reflection::CType;",
           "using base::reflection::UnkFn;"]
NAME_RE = re.compile(r"\b(?:fn|g_unk)_[0-9a-f]{10}\b")


def write_chunks(prefix: str, blocks: list[list[str]], decls: set[str], header: list[str]) -> None:
    by_name = {NAME_RE.search(d.split("(")[0]).group(0): d for d in decls}
    for k in range(0, len(blocks), CHUNK):
        body = [line for b in blocks[k:k + CHUNK] for line in b]
        names = {n for line in body for n in NAME_RE.findall(line)}
        used = sorted(d for n, d in by_name.items() if n in names)
        (OUT / f"{prefix}_{k // CHUNK:02d}.cpp").write_text("\n".join(header + PRELUDE + used + body) + "\n",
                                                           encoding="utf-8")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
    getter_shape = shape(orig, REF_GETTER)
    enum_shape = shape(orig, REF_ENUM)
    n = len(getter_shape) * 4

    pairs = []  # (getter, name, init, traits lines)
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
        body = traits(orig, init, decls)
        if body:
            pairs.append((a, name, init, body))
    # One constructor may serve several getters; emit it once (first getter wins).
    seen: set[int] = set()
    pairs = [p for p in pairs if not (p[2] in seen or seen.add(p[2]))]

    work = ROOT / "build" / "gen_types"
    work.mkdir(parents=True, exist_ok=True)
    src, obj = work / "candidates.cpp", work / "candidates.o"
    text = (PRELUDE + sorted(decls) + [line for g, nm, i, b in pairs for line in emit(g, nm, i, b)]
            + [line for i, b in enums for line in emit_enum(i, b)])
    src.write_text("\n".join(text) + "\n", encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
    ok = {res.address for res in asmdiff.diff_object(obj, orig) if res.matching}
    good = [p for p in pairs if p[0] in ok and p[2] in ok]
    good_enums = [e for e in enums if e[0] in ok]

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = "// Generated by tools/gen_types.py from main; regenerate rather than edit."
    write_chunks("pointer_types", [emit(*p) for p in good], decls,
                 [note, "// Getters (refl_GetType<T>) and descriptor constructors (refl_InitType<T>) of pointer",
                  "// types, byte-identical to the original."])
    write_chunks("enum_types", [emit_enum(*e) for e in good_enums], decls,
                 [note, "// Descriptor constructors (refl_InitType<E>) of enums, byte-identical to the original."])
    print(f"pointer types: {len(pairs)} getter/constructor candidates, {len(good)} pairs byte-identical; "
          f"enums: {len(enums)} constructor candidates, {len(good_enums)} byte-identical")
    return 0


if __name__ == "__main__":
    sys.exit(main())
