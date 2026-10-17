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
are defined under their own fn_<addr> names; the rest are declared with their slot's type.
The x3/x4 functions (class attributes, member fields) are reconstructed by tools/refl_attrs.py
and kept only when a scratch build of them is byte-identical (build/reflection_attrs/).

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
import csv
import json
import re
import shutil
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
    init: int | None = None               # descriptor constructor (T::refl_InitClass)
    traits: dict[int, int] | None = None  # CClass offset -> function address it stores
    abstract: bool = False                # constructor sets flag bit 0 (orr #1 after the and #~3)


def shape(code: bytes) -> str:
    """Position-independent signature that also ignores `add xN, x19, #imm`: the descriptor's
    offset from its guard (x19), which depends on the TU's layout, not on the code shape."""
    import struct
    words = bytearray(code)
    for i in range(len(code) // 4):
        w = struct.unpack_from("<I", code, 4 * i)[0]
        if w & 0xFF800000 == 0x91000000 and (w >> 5) & 0x1F == 19:  # ADD Xd, X19, #imm
            struct.pack_into("<I", words, 4 * i, w & ~(0xFFF << 10) & 0xFFFFFFFF)
    return sigmatch.signature(bytes(words))


TRAIT_SLOTS = [0x58, 0x60, 0x68, 0x28, 0x30, 0x38, 0x40, 0x48, 0x50]

# Slot -> (CClass.h type alias, declaration format). {name} is fn_<addr>.
SLOT_TYPES = {
    0x28: ("UnkFn", "void {name}();"),
    0x30: ("UnkFn_0x30", "void {name}(void* dst, const void* src);"),
    0x38: ("UnkFn_0x30", "void {name}(void* dst, const void* src);"),
    0x40: ("UnkFn_0x40", "void {name}(void* obj);"),
    0x48: ("UnkFn_0x48", "void* {name}(void* dst, const void* src);"),
    0x50: ("UnkFn_0x48", "void* {name}(void* dst, const void* src);"),
    0x58: ("UnkFn_0x58", "bool {name}(const void* a, const void* b);"),
    0x60: ("UnkFn_0x60", "s32 {name}();"),
    0x68: ("UnkFn_0x68", "const base::reflection::CType& {name}(const void* obj);"),
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
    if g.local_parent and g.parent:
        return any(o.address == g.parent for o in block)
    return g.shape_ok and bool(g.parent)


def file_name(type_name: str, taken: set[str]) -> str:
    base = re.sub(r"[^A-Za-z0-9]+", "_", type_name.replace("::", "__")).strip("_")[:80] or "unnamed"
    name = base
    n = 2
    while name.lower() in taken:
        name = f"{base}_{n}"
        n += 1
    taken.add(name.lower())
    return name


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
    attr_total = len({a for b in blocks for g in b for a in (g.fn_0x70, g.fn_0xe0)
                      if a and len(orig.function(a)) > 4})

    _, local_ptrs, _, ptr_decls = gen_types.scan(orig)
    ptrs_by_pointee: dict[int, list] = {}
    for p in local_ptrs:
        ptrs_by_pointee.setdefault(p[4], []).append(p)

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    files: dict[str, tuple[list[str], list]] = {}  # name -> (lines, local pointer pairs)
    taken: set[str] = set()
    generated = skipped_shape = skipped_layout = helpers_defined = attrs_defined = 0
    report = []
    for block in blocks:
        n = len(block)
        expected = [block[0].guard + 8 * n + DESCRIPTOR_SIZE * i for i in range(n)]
        layout_ok = [g.descriptor for g in block] == expected
        shape_ok = all(getter_ok(g, block) and g.fn_0x70 and g.fn_0xe0 and g.init and g.traits
                       and all(k in g.traits for k in (0x58, 0x60, 0x68)) and g.address in sizes
                       for g in block)
        if not layout_ok or not shape_ok:
            if not layout_ok:
                skipped_layout += n
                report.append({"tu": [g.type for g in block], "reason": "other statics in the TU"})
            else:
                skipped_shape += n
                report.append({"tu": [g.type for g in block], "reason": "getter code differs from the common shape"})
            # The getters wait, but the TU's helpers and attribute/field functions don't depend on
            # its statics: generate those that are known to match.
            lines = [
                "// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
                f"// Part of one original translation unit ({n} type{'s' if n > 1 else ''}) whose getters aren't",
                "// generated yet (see build/reflection_tus.json): the per-type helpers with a known uniform",
                "// shape and the attribute/member-field functions that tools/refl_attrs.py reconstructs",
                "// byte-identically.",
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
                if defs:
                    lines += [f"// {g.type}"] + defs + [""]
                    body += 1
            if body:
                name = file_name(block[0].type, taken)
                files[name] = (lines, [])
            continue
        name = file_name(block[0].type, taken)
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
        for g in block:
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
            if g.parent:
                lines.append(f'DREAD_REFL_CLASS_GETTER({g.address:x}, {g.init:x}, "{g.type}", {g.parent:x}, '
                             f'{g.fn_0x70:x}, {g.fn_0xe0:x});')
            else:
                lines.append(f'DREAD_REFL_ROOT_CLASS_GETTER({g.address:x}, {g.init:x}, "{g.type}", '
                             f'{g.fn_0x70:x}, {g.fn_0xe0:x});')
            lines.append("")
        ptrs = [p for g in block for p in ptrs_by_pointee.get(g.address, [])]
        files[name] = (lines, ptrs)
        generated += n

    def text(lines: list[str], ptrs: list) -> str:
        if not ptrs:
            return chr(10).join(lines)
        body = [line for p in ptrs for line in gen_types.emit(*p[:4])]
        pointees = {f"fn_{p[4]:x}" for p in ptrs}
        decls = [d for d in gen_types.used_decls(body, ptr_decls)
                 if gen_types.NAME_RE.search(d.split("(")[0]).group(0) not in pointees]
        return chr(10).join(lines[:1] + ['#include "base/reflection/CPointerType.h"'] + lines[1:]
                            + decls + body) + chr(10)

    # Pointer types: drop those that would redeclare a name the file already uses, then keep
    # only the byte-identical ones.
    for name, (lines, ptrs) in files.items():
        names = set(gen_types.NAME_RE.findall(chr(10).join(lines)))
        files[name] = (lines, [p for p in ptrs if not any(
            gen_types.NAME_RE.search(d.split("(")[0]).group(0) in names - {f"fn_{p[4]:x}"}
            for d in gen_types.used_decls(gen_types.emit(*p[:4]), ptr_decls))])
    work = ROOT / "build" / "reflection_ptrs"
    work.mkdir(parents=True, exist_ok=True)

    def check_file(name: str) -> set[int]:
        src, obj = work / f"{name}.cpp", work / f"{name}.o"
        src.write_text(text(*files[name]), encoding="utf-8")
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        if r.returncode != 0:
            return set()
        return {res.address for res in asmdiff.diff_object(obj, orig) if res.matching}

    with_ptrs = [n for n, (_, ptrs) in files.items() if ptrs]
    with ThreadPoolExecutor() as pool:
        matched = dict(zip(with_ptrs, pool.map(check_file, with_ptrs)))
    ptrs_generated = 0
    for name, (lines, ptrs) in files.items():
        ok = matched.get(name, set())
        ptrs = [p for p in ptrs if p[0] in ok and p[2] in ok]
        ptrs_generated += len(ptrs)
        (OUT / f"{name}.cpp").write_text(text(lines, ptrs), encoding="utf-8", newline=chr(10))

    (ROOT / "build" / "reflection_tus.json").write_text(json.dumps(report, indent=1), encoding="utf-8")
    print(f"{len(rows)} class getters; {len(located)} located; {len(blocks)} TUs. Generated {generated} getters "
          f"and {helpers_defined} helpers ({attrs_defined} of {attr_total} non-empty attribute/field functions) in {len(list(OUT.glob('*.cpp')))} files; skipped {skipped_layout} (other statics) and "
          f"{skipped_shape} (different code shape); {ptrs_generated} of {len(local_ptrs)} pointer types with a "
          f"local pointee. Skipped TUs: build/reflection_tus.json")
    return 0


if __name__ == "__main__":
    sys.exit(main())
