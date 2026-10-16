#!/usr/bin/env python3
"""Generate objdiff target objects and objdiff.json for every source in src/.

objdiff (encounter/objdiff) diffs a "base" object (our compiled code) against a
"target" object (the original). The Switch has no split executable like a GameCube
DOL, so for each source file this script writes a target ELF relocatable that holds
the original bytes of exactly the functions that source defines, under the same
symbol names:

  - base:   build/check/<ver>/<path>.o            (tools/check.py builds these)
  - target: build/objdiff/<ver>/target/<path>.o   (written here)

Relocations in the target mirror the base object's, so objdiff compares symbols, not
raw immediates. Relocated instruction fields are cleared as in an unlinked object.
Call targets are verified: when a base call names an address (fn_/unk_/g_unk_<addr>, or
a unique name in data/functions/) and the original calls somewhere else, the target
gets a distinct `orig_<addr>` symbol so the mismatch is visible. Data references are
mirrored unverified.

Usage:
  python tools/objdiff_gen.py --version 2.1.0      # builds base objects, writes targets + objdiff.json
Then open the repo folder in objdiff (%LOCALAPPDATA%\\dread-decomp\\tools\\objdiff-<ver>.exe).
"""

from __future__ import annotations

import argparse
import json
import re
import struct
import sys
from dataclasses import dataclass, field
from pathlib import Path

from elftools.elf.elffile import ELFFile
from elftools.elf.relocation import RelocationSection

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import check  # noqa: E402
import fetch_toolchains  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
ADDR_RE = re.compile(r"(?:fn|unk|g_unk)_([0-9a-fA-F]{10})")
CALL_RELOCS = {282, 283}  # JUMP26, CALL26
STB = {"STB_LOCAL": 0, "STB_GLOBAL": 1, "STB_WEAK": 2}

# Progress category per top-level src/ folder.
CATEGORIES = {"atk_id": "nn_atk"}
CATEGORY_NAMES = {"game": "Game code", "nn_atk": "nn::atk (NintendoWare)"}


@dataclass
class Reloc:
    offset: int        # function-relative
    type: int
    symbol: str
    addend: int


@dataclass
class TargetFunc:
    name: str
    binding: int
    code: bytes
    relocs: list[Reloc] = field(default_factory=list)


def base_functions(obj: Path):
    """(name, binding, section index, value, size) for defined functions, plus relocations by section."""
    with obj.open("rb") as f:
        elf = ELFFile(f)
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        funcs = [(s.name, STB[s["st_info"]["bind"]], s["st_shndx"], s["st_value"], s["st_size"])
                 for s in syms if s["st_info"]["type"] == "STT_FUNC" and isinstance(s["st_shndx"], int)]
        relocs: dict[int, list[tuple[int, int, str, int]]] = {}
        for sec in elf.iter_sections():
            if not isinstance(sec, RelocationSection):
                continue
            for r in sec.iter_relocations():
                sym = syms[r["r_info_sym"]]
                name = sym.name
                addend = r["r_addend"]
                if sym["st_info"]["type"] == "STT_SECTION":
                    # Section-relative reference: name the function defined at that offset.
                    hit = next((fn for fn in funcs if fn[2] == sym["st_shndx"] and fn[3] == addend), None)
                    name = hit[0] if hit else elf.get_section(sym["st_shndx"]).name
                    addend = 0 if hit else addend
                relocs.setdefault(sec["sh_info"], []).append((r["r_offset"], r["r_info_type"], name, addend))
    return funcs, relocs


def expected_address(symbol: str, demangled: dict[str, str], by_name: dict[str, list[int]]) -> int | None:
    m = ADDR_RE.search(symbol)
    if m:
        return int(m.group(1), 16)
    hits = by_name.get(demangled.get(symbol, symbol), [])
    return hits[0] if len(hits) == 1 else None


def branch_target(word: int, pc: int) -> int:
    imm = word & 0x3FFFFFF
    if imm & 0x2000000:
        imm -= 0x4000000
    return pc + (imm << 2)


def build_target(funcs: list[TargetFunc]) -> bytes:
    """Minimal ELF64 AArch64 relocatable: .text.<fn> + .rela.text.<fn> per function."""
    strtab = bytearray(b"\0")
    shstrtab = bytearray(b"\0")

    def add(tab: bytearray, s: str) -> int:
        off = len(tab)
        tab += s.encode() + b"\0"
        return off

    sections = []  # (name_off, type, flags, data, link, info, align, entsize)
    text_index = {}
    for fn in funcs:
        text_index[fn.name] = len(sections) + 1
        sections.append([add(shstrtab, f".text.{fn.name}"), 1, 6, fn.code, 0, 0, 4, 0])

    # Symbols: null, local functions, then global functions and undefined references.
    local_fns = [f for f in funcs if f.binding == 0]
    global_fns = [f for f in funcs if f.binding != 0]
    undefined = sorted({r.symbol for f in funcs for r in f.relocs} - {f.name for f in funcs})
    sym_entries = [(0, 0, 0, 0, 0)]
    sym_index = {}
    for f in local_fns + global_fns:
        sym_index[f.name] = len(sym_entries)
        sym_entries.append((add(strtab, f.name), (f.binding << 4) | 2, text_index[f.name], 0, len(f.code)))
    first_global = 1 + len(local_fns)
    for name in undefined:
        sym_index[name] = len(sym_entries)
        sym_entries.append((add(strtab, name), (1 << 4) | 0, 0, 0, 0))

    symtab_idx = len(sections) + 1 + sum(1 for f in funcs if f.relocs)
    for fn in funcs:
        if not fn.relocs:
            continue
        data = b"".join(struct.pack("<QQq", r.offset, (sym_index[r.symbol] << 32) | r.type, r.addend)
                        for r in fn.relocs)
        sections.append([add(shstrtab, f".rela.text.{fn.name}"), 4, 0x40, data, symtab_idx,
                         text_index[fn.name], 8, 24])
    symtab = b"".join(struct.pack("<IBBHQQ", n, info, 0, shndx, value, size)
                      for n, info, shndx, value, size in sym_entries)
    strtab_idx = symtab_idx + 1
    sections.append([add(shstrtab, ".symtab"), 2, 0, symtab, strtab_idx, first_global, 8, 24])
    sections.append([add(shstrtab, ".strtab"), 3, 0, bytes(strtab), 0, 0, 1, 0])
    shstr_name = add(shstrtab, ".shstrtab")
    sections.append([shstr_name, 3, 0, bytes(shstrtab), 0, 0, 1, 0])

    out = bytearray(64)
    offsets = []
    for s in sections:
        out += b"\0" * (-len(out) % max(s[6], 1))
        offsets.append(len(out))
        out += s[3]
    out += b"\0" * (-len(out) % 8)
    shoff = len(out)
    out += b"\0" * 64
    for s, off in zip(sections, offsets):
        out += struct.pack("<IIQQQQIIQQ", s[0], s[1], s[2], 0, off, len(s[3]), s[4], s[5], s[6], s[7])
    struct.pack_into("<4sBBBBB7sHHIQQQIHHHHHH", out, 0, b"\x7fELF", 2, 1, 1, 0, 0, b"\0" * 7,
                     1, 183, 1, 0, 0, shoff, 0, 64, 0, 0, 64, len(sections) + 1, len(sections))
    return bytes(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()

    rows, _ = check.load_csv()
    by_name: dict[str, list[int]] = {}
    for r in rows.values():
        by_name.setdefault(r["name"], []).append(int(r["address"], 16))
    original = asmdiff.Original(args.version)
    units = []
    mismatched_calls = 0
    for src in check.sources():
        obj = check.build(args.version, src)
        if obj is None:
            return 1
        funcs, relocs = base_functions(obj)
        demangled = check.demangle([f[0] for f in funcs] + sorted({r[2] for rs in relocs.values() for r in rs}))
        targets = []
        all_matching = True
        for name, binding, shndx, value, size in funcs:
            addr = expected_address(name, demangled, by_name)
            if addr is None or addr not in original.sizes:
                continue
            code = bytearray(original.function(addr))
            out_relocs = []
            for off, rtype, sym, addend in relocs.get(shndx, []):
                if not value <= off < value + size:
                    continue
                rel = off - value
                if rel + 4 > len(code):
                    continue
                word = struct.unpack_from("<I", code, rel)[0]
                if rtype in CALL_RELOCS:
                    want = expected_address(sym, demangled, by_name)
                    actual = branch_target(word, addr + rel)
                    if want is not None and want != actual:
                        sym, addend = f"orig_{actual:x}", 0
                        mismatched_calls += 1
                struct.pack_into("<I", code, rel, word & asmdiff.MASKS.get(rtype, 0xFFFFFFFF))
                out_relocs.append(Reloc(rel, rtype, sym, addend))
            targets.append(TargetFunc(name, binding, bytes(code), out_relocs))
            status = rows.get(f"{addr:#x}", {}).get("status")
            all_matching &= status in ("O", "L")
        if not targets:
            continue
        rel_path = src.relative_to(ROOT / "src").with_suffix("")
        target = ROOT / "build" / "objdiff" / args.version / "target" / rel_path.with_suffix(".o")
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_bytes(build_target(targets))
        category = CATEGORIES.get(rel_path.parts[0], "game")
        units.append({
            "name": rel_path.as_posix(),
            "target_path": target.relative_to(ROOT).as_posix(),
            "base_path": obj.relative_to(ROOT).as_posix(),
            "metadata": {"source_path": src.relative_to(ROOT).as_posix(), "complete": all_matching,
                         "progress_categories": [category]},
        })

    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "min_version": "3.0.0",
        "build_base": False,
        "build_target": False,
        "watch_patterns": ["*.c", "*.cpp", "*.h", "*.hpp"],
        "units": units,
        "progress_categories": [{"id": k, "name": v} for k, v in CATEGORY_NAMES.items()],
    }
    (ROOT / "objdiff.json").write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8")
    print(f"{len(units)} units written to objdiff.json; {mismatched_calls} call targets differ from the original")
    print(f"objdiff GUI: {fetch_toolchains.tool_path('objdiff-' + fetch_toolchains.OBJDIFF_VERSION)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
