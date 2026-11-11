#!/usr/bin/env python3
"""The reflected members of every class, as a table: data/reflection_fields.csv.

A reflected class registers each of its members with a name, a type and an offset
(tools/refl_attrs.py reconstructs those functions; they are in src/reflection/). This reads them back
out of the generated sources into one table, so a layout can be looked up instead of derived again in
every file that touches the class: 6,087 members of 1,135 classes on 2.1.0.

data/reflection_fields.csv columns: class,offset,name,type
  type: the member's reflected type by name, when its type expression is a getter that
  data/reflection_types.csv or src/reflection_types/ names, or (with --version) a type object one of
  those getters returns; else the expression as the source has it (`&g_unk_<addr>`: the type object of
  an enum, a flag set or a collection, which the tracked data does not name yet).

Usage:
  python tools/gen_fields.py [--version 2.1.0]   # write the table (--version also names type objects, from the dump)
  python tools/gen_fields.py --class CActor      # the class's layout: its members and its bases', by offset
  python tools/gen_fields.py --member vBones     # every class with a member of that name
The table is generated: regenerate, never hand-edit. Without --version only tracked files are read.
"""

from __future__ import annotations

import argparse
import csv
import re
import struct
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "data" / "reflection_fields.csv"
TYPES = ROOT / "data" / "reflection_types.csv"
FIELD = re.compile(r'fn_7100096524\(cls, CStrId\("([^"]+)", true\), ([^,]+), (0x[0-9a-f]+|\d+), ')
FUNCTION = re.compile(r"^void fn_(71[0-9a-f]{8})\((?:base::reflection::)?CClass\* cls\) \{\n(.*?)^\}", re.M | re.S)
GETTER = re.compile(r"DREAD_REFL_(?:ROOT_)?CLASS_GETTER(?:_INLINE_NAME)?\(([^)]*)\)")
NAMED = re.compile(r'(?:UnkReflTypeGetter|UnkReflPrimitiveTraits)<UnkRefl_fn_(71[0-9a-f]{8})> \{\s*static constexpr const char\* name = "([^"]*)";')


def members(text: str) -> list[tuple[str, int, str, str]]:
    """(class, offset, member, type expression) for the classes one generated reflection source registers."""
    bodies = {m.group(1): FIELD.findall(m.group(2)) for m in FUNCTION.finditer(text)}
    out = []
    for m in GETTER.finditer(text):
        args = [a.strip() for a in m.group(1).split(",")]
        name = next((a.strip('"') for a in args if a.startswith('"')), None)
        if name is None:
            continue
        out += [(name, int(off, 0), member, expr.strip()) for member, expr, off in bodies.get(args[-1], [])]
    return out


def getter_names(type_rows: list[dict], type_sources: list[str]) -> dict[int, str]:
    """{type getter address: type name} from data/reflection_types.csv and the generated type sources."""
    names = {int(r["getter"], 16): r["type"] for r in type_rows if r.get("getter")}
    for text in type_sources:
        for addr, name in NAMED.findall(text):
            names.setdefault(int(addr, 16), name)
    return names


def type_name(expr: str, getters: dict[int, str], objects: dict[int, str]) -> str:
    m = re.fullmatch(r"fn_(71[0-9a-f]{8})\(\)", expr)
    if m:
        return getters.get(int(m.group(1), 16), expr)
    m = re.fullmatch(r"&g_unk_(71[0-9a-f]{8})", expr)
    if m:
        return objects.get(int(m.group(1), 16), expr)
    return expr


def returned_objects(version: str, getters: dict[int, str]) -> dict[int, str]:
    """{type object address: type name}: the address each named getter returns (adrp + add into x0, or
    into a register moved to x0), read from the user's dump."""
    import asmdiff

    original = asmdiff.Original(version)
    out: dict[int, str] = {}
    for addr, name in getters.items():
        size = original.sizes.get(addr)
        if not size:
            continue
        page: dict[int, int] = {}
        full: dict[int, int] = {}
        for i, w in enumerate(struct.unpack_from(f"<{size // 4}I", original.data_at(addr, size))):
            rd, rn = w & 31, (w >> 5) & 31
            if w & 0x9F000000 == 0x90000000:
                page[rd] = asmdiff.adrp_target(w, addr + 4 * i)
                full.pop(rd, None)
            elif w & 0xFFC00000 == 0x91000000 and rn in page:
                full[rd] = page[rn] + ((w >> 10) & 0xFFF)
            elif w & 0xFFE0FFE0 == 0xAA0003E0 and ((w >> 16) & 31) in full:
                full[rd] = full[(w >> 16) & 31]
            elif w == 0xD65F03C0 and 0 in full:
                out.setdefault(full[0], name)
    return out


def layout(cls: str, rows: list[dict], types: dict[str, dict]) -> list[tuple[int, str, str, str]]:
    """(offset, member, type, declaring class) for a class and its bases, by offset."""
    out = []
    seen = set()
    while cls and cls not in seen:
        seen.add(cls)
        out += [(int(r["offset"], 16), r["name"], r["type"], cls) for r in rows if r["class"] == cls]
        cls = types.get(cls, {}).get("parent", "")
    return sorted(out)


def load() -> list[dict]:
    if not OUT.is_file():
        raise SystemExit(f"{OUT.relative_to(ROOT).as_posix()} is missing: run tools/gen_fields.py first")
    with OUT.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="also name type objects from orig/<version>")
    ap.add_argument("--class", dest="cls", help="print a class's layout")
    ap.add_argument("--member", help="print every class with a member of this name")
    args = ap.parse_args()
    with TYPES.open(newline="", encoding="utf-8") as f:
        type_rows = list(csv.DictReader(f))
    if args.cls or args.member:
        rows = load()
        if args.member:
            for r in rows:
                if r["name"] == args.member:
                    print(f"{r['class']}  {r['offset']}  {r['type']}")
            return 0
        types = {r["type"]: r for r in type_rows}
        names = [n for n in types if n == args.cls or n.endswith("::" + args.cls)]
        if not names:
            print(f"{args.cls}: not a reflected class in data/reflection_types.csv")
            return 1
        for name in names:
            t = types[name]
            print(f"{name}: {t['size'] or '?'} bytes, base {t['parent'] or 'none'}, getter {t['getter']}")
            for off, member, typ, owner in layout(name, rows, types):
                print(f"  {off:#06x}  {member:<32} {typ}" + (f"   ({owner})" if owner != name else ""))
        return 0

    sys.path.insert(0, str(Path(__file__).resolve().parent))
    getters = getter_names(type_rows, [p.read_text(encoding="utf-8")
                                       for p in sorted((ROOT / "src" / "reflection_types").glob("*.cpp"))])
    objects = returned_objects(args.version, getters) if args.version else {}
    found = []
    for p in sorted((ROOT / "src" / "reflection").glob("*.cpp")):
        found += members(p.read_text(encoding="utf-8"))
    found.sort(key=lambda r: (r[0], r[1], r[2]))
    with OUT.open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(["class", "offset", "name", "type"])
        for cls, off, member, expr in found:
            w.writerow([cls, f"{off:#x}", member, type_name(expr, getters, objects)])
    named = sum(1 for _c, _o, _m, e in found if type_name(e, getters, objects) != e)
    print(f"{len(found)} members of {len({r[0] for r in found})} classes written to "
          f"{OUT.relative_to(ROOT).as_posix()}; {named} with a named type")
    return 0


if __name__ == "__main__":
    sys.exit(main())
