#!/usr/bin/env python3
"""Generate src/reflection/: one source per original translation unit of class getters.

Every reflected class T has a getter (T::refl_GetClass) and a descriptor constructor
(T::refl_InitClass, an instantiation of CClass's constructor template), both expressed in
include/base/reflection/CClass.h. This tool reads each getter in main (the parent getter it
calls, the type-specific create/destroy functions it passes in x3/x4, the constructor it calls)
and each constructor (sizeof(T) and the type-specific functions it stores), emits a traits
specialisation per type, and groups types back into their original translation units.

TU grouping: within a TU, Clang lays out all guard variables first, then the descriptors
(0x138 bytes each), in definition order. A run of getter guards 8 bytes apart, whose
descriptors follow immediately and in order, is one TU. Only "clean" TUs are generated:
every getter uses the common code shape and the TU holds nothing but getter statics. Other
TUs need extra statics that aren't known yet, and are reported instead.

Usage: python tools/gen_reflection.py --version 2.1.0
Generated files are outputs: regenerate, don't edit.
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import shutil
import sys
from dataclasses import dataclass
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
import asmdiff  # noqa: E402
import sigmatch  # noqa: E402
from image import BASE  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "reflection"
DESCRIPTOR_SIZE = 0x138
REFERENCE_GETTER = 0x7100227448  # CActor::refl_GetClass, the common code shape


@dataclass
class Getter:
    type: str
    address: int
    guard: int | None = None
    descriptor: int | None = None
    parent: int | None = None
    create: int | None = None
    destroy: int | None = None
    shape_ok: bool = False
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
            inits: set[int] | None = None) -> None:
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    code = orig.function(g.address)
    g.shape_ok = shape(code) == ref_sig
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
                        g.create = BASE + target
                    else:
                        g.destroy = BASE + target
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
    all_g = [Getter(r["type"], int(r["getter"], 16)) for r in rows]
    for g in all_g:
        analyse(orig, g, getters, ref_sig, inits)

    located = sorted((g for g in all_g if g.guard and g.descriptor), key=lambda g: g.guard)
    blocks: list[list[Getter]] = []
    for g in located:
        if blocks and g.guard - blocks[-1][-1].guard == 8:
            blocks[-1].append(g)
        else:
            blocks.append([g])

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    taken: set[str] = set()
    generated = skipped_shape = skipped_layout = 0
    report = []
    for block in blocks:
        n = len(block)
        expected = [block[0].guard + 8 * n + DESCRIPTOR_SIZE * i for i in range(n)]
        layout_ok = [g.descriptor for g in block] == expected
        shape_ok = all(g.shape_ok and g.parent and g.create and g.destroy and g.init and g.traits
                       and all(k in g.traits for k in (0x58, 0x60, 0x68)) and g.address in sizes
                       for g in block)
        if not layout_ok:
            skipped_layout += n
            report.append({"tu": [g.type for g in block], "reason": "other statics in the TU"})
            continue
        if not shape_ok:
            skipped_shape += n
            report.append({"tu": [g.type for g in block], "reason": "getter code differs from the common shape"})
            continue
        name = file_name(block[0].type, taken)
        lines = [
            "// Generated by tools/gen_reflection.py from main; regenerate rather than edit.",
            f"// Reflection descriptors for one original translation unit ({n} type{'s' if n > 1 else ''}):",
            "// for each type, its getter (T::refl_GetClass) and, through the traits below, its",
            "// descriptor constructor (T::refl_InitClass).",
            '#include "base/reflection/CClass.h"',
            "",
        ]
        declared: set[int] = set()
        for g in block:
            lines.append(f"// {g.type}")
            lines.append(f"DREAD_REFL_TAG({g.init:x});")
            fns = sorted(set(g.traits.values()) - declared)
            declared |= set(fns)
            lines += [f"void fn_{a:x}();" for a in fns]
            lines.append("template <>")
            lines.append(f"struct base::reflection::UnkReflClassTraits<UnkRefl_fn_{g.init:x}> {{")
            lines.append(f"    static constexpr u64 size = {sizes[g.address]:#x};")
            lines.append(f"    static constexpr bool abstract = {'true' if g.abstract else 'false'};")
            for slot in TRAIT_SLOTS:
                value = f"&fn_{g.traits[slot]:x}" if slot in g.traits else "nullptr"
                lines.append(f"    static constexpr UnkFn fn_{slot:#x} = {value};")
            lines.append("};")
            lines.append(f'DREAD_REFL_CLASS_GETTER({g.address:x}, {g.init:x}, "{g.type}", {g.parent:x}, '
                         f'{g.create:x}, {g.destroy:x});')
            lines.append("")
        (OUT / f"{name}.cpp").write_text(chr(10).join(lines), encoding="utf-8", newline=chr(10))
        generated += n

    (ROOT / "build" / "reflection_tus.json").write_text(json.dumps(report, indent=1), encoding="utf-8")
    print(f"{len(rows)} class getters; {len(located)} located; {len(blocks)} TUs. Generated {generated} getters "
          f"in {len(list(OUT.glob('*.cpp')))} files; skipped {skipped_layout} (other statics) and "
          f"{skipped_shape} (different code shape). Skipped TUs: build/reflection_tus.json")
    return 0


if __name__ == "__main__":
    sys.exit(main())
