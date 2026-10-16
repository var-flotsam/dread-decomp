#!/usr/bin/env python3
"""Binary reconnaissance that needs no Ghidra. Writes JSON to build/recon/<version>/.

  segments.json       NSO segments, MOD0, ELF sections, BSS
  dynsym.json         dynamic symbols (imports and exports)
  eh_functions.json   [start, size] for every .eh_frame FDE (exact function bounds)
  rtti.json           Itanium type_info objects, bases, and vtables
  lua_tables.json     luaL_Reg-style {name, function} tables
  build_strings.json  SDK/middleware/build identification strings
  reflection.json     MEDS type names found (or not) in the binary's strings

Usage: python tools/recon/run.py --version 2.1.0 [--module main]
All addresses in the output are absolute (image base 0x7100000000).
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import eh_frame  # noqa: E402
import lua_tables  # noqa: E402
import nso2elf  # noqa: E402
import rtti  # noqa: E402
from image import BASE, Image  # noqa: E402

ROOT = Path(__file__).resolve().parents[2]

BUILD_STRING_RE = re.compile(
    rb"SDK MW\+[ -~]{3,120}|NintendoSDK[ -~]{0,100}|nnSdk[ -~]{0,60}|[Cc]lang version[ -~]{0,80}"
    rb"|Lua 5\.[0-9][ -~]{0,40}|\$Lua[ -~]{0,80}|Copyright[ -~]{0,100}|\(C\) ?[0-9]{4}[ -~]{0,80}"
    rb"|Box2D[ -~]{0,30}|imgui[ -~]{0,30}|zlib[ -~]{0,30}|inflate [0-9][ -~]{0,60}"
    rb"|\.\.\\[ -~]{3,120}\.(?:h|cpp|hpp|c)\b")


def hexaddr(a: int) -> str:
    return f"{BASE + a:#x}"


def c_strings(img: Image, lo: int, hi: int) -> dict[str, int]:
    out: dict[str, int] = {}
    d = img.data
    i = lo
    while i < hi:
        j = d.find(b"\0", i, hi)
        if j < 0:
            break
        if j - i >= 2 and all(32 <= c < 127 for c in d[i:j]):
            out.setdefault(d[i:j].decode(), i)
        i = j + 1
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--module", default="main")
    args = ap.parse_args()

    out = ROOT / "build" / "recon" / args.version
    out.mkdir(parents=True, exist_ok=True)
    img = Image(ROOT / "orig" / args.version / "exefs" / args.module)
    m = args.module

    def dump(name: str, obj) -> None:
        (out / f"{m}_{name}.json").write_text(json.dumps(obj, indent=1), encoding="utf-8")

    sections = nso2elf.collect_sections(img.mod)
    dump("segments", {
        "segments": {s.name: {"start": hexaddr(s.mem_offset), "size": s.size, "file_size": s.file_size,
                              "compressed": s.compressed}
                     for s in (img.mod.text, img.mod.rodata, img.mod.data)},
        "bss": {"start": hexaddr(img.mod.data.mem_offset + img.mod.data.size),
                "end": hexaddr(img.mod.bss_end)},
        "mod0": hexaddr(img.mod.mod0),
        "sections": [{"name": s.name, "start": hexaddr(s.addr), "size": s.size} for s in sections],
    })
    print(f"segments: text {img.mod.text.size:#x}, rodata {img.mod.rodata.size:#x}, "
          f"data {img.mod.data.size:#x}, bss to {img.mod.bss_end:#x}")

    dump("dynsym", [{**s, "value": hexaddr(s["value"]) if s["shndx"] else None,
                     "kind": "export" if s["shndx"] else "import"}
                    for s in img.symbols if s["name"]])
    n_exp = sum(1 for s in img.symbols if s["name"] and s["shndx"])
    print(f"dynsym: {n_exp} exports, {sum(1 for s in img.symbols if s['name']) - n_exp} imports")

    fdes = eh_frame.fde_functions(img)
    dump("eh_functions", [[hexaddr(a), n] for a, n in fdes])
    print(f"eh_frame: {len(fdes)} functions covering {sum(n for _, n in fdes):,} of {img.mod.text.size:,} text bytes")

    tis = rtti.recover(img)
    dump("rtti", [{
        "typeinfo": hexaddr(t.addr), "kind": t.kind, "mangled": t.mangled, "name": t.name,
        "bases": [{**b, "typeinfo": hexaddr(b["typeinfo"]),
                   "name": tis[b["typeinfo"]].name if b["typeinfo"] in tis else None} for b in t.bases],
        "vtables": [{"addr": hexaddr(v["addr"]), "offset_to_top": v["offset_to_top"],
                     "slots": [hexaddr(s) if isinstance(s, int) else s for s in v["slots"]]}
                    for v in t.vtables],
    } for t in tis.values()])
    print(f"rtti: {len(tis)} type_infos, {sum(len(t.vtables) for t in tis.values())} vtables")

    tables = lua_tables.find_tables(img)
    dump("lua_tables", [{"addr": hexaddr(t["addr"]),
                         "entries": [{"name": e["name"], "func": hexaddr(e["func"])} for e in t["entries"]]}
                        for t in tables])
    print(f"lua: {len(tables)} tables, {sum(len(t['entries']) for t in tables)} bindings")

    seen: dict[str, str] = {}
    for mt in BUILD_STRING_RE.finditer(img.data):
        s = mt.group(0).decode("ascii", "replace")
        seen.setdefault(s, hexaddr(mt.start()))
    dump("build_strings", [{"address": a, "value": s} for s, a in sorted(seen.items(), key=lambda kv: kv[1])])
    print(f"build strings: {len(seen)}")

    if m == "main":
        from mercury_engine_data_structures import dread_data
        types = dread_data.get_raw_types()
        strings = c_strings(img, img.rodata[0], img.mod.data.mem_offset + img.mod.data.size)
        found = {t: hexaddr(strings[t]) for t in types if t in strings}
        props = dread_data.all_name_to_property_id()
        dump("reflection", {
            "meds_types": len(types), "found": found,
            "missing": sorted(t for t in types if t not in strings),
            "meds_property_names": len(props),
            "property_names_found": sum(1 for p in props if p in strings),
        })
        print(f"reflection: {len(found)}/{len(types)} MEDS type names present as strings")
    return 0


if __name__ == "__main__":
    sys.exit(main())
