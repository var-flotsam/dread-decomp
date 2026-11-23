#!/usr/bin/env python3
"""Class declarations for reflected classes, from the reflection tables: include/reflected/.

A reflected class registers its name, its parent, its size and each member's name, type and offset
(data/reflection_types.csv, data/reflection_fields.csv). This writes one header per class of CLASSES (and
per class of their base chains): the class with its real name, its base, its reflected members by name and
type at their offsets, `pad_0x<offset>` arrays for the gaps (members the reflection does not describe), and
static_asserts on its size and every member's offset. tools/gen_reflection.py then writes those classes'
reflection functions as the class's own static members (base/reflection/ReflectedClass.h,
DREAD_REFLECTED_CLASS_MEMBERS), with sizeof(T) for the size and offsetof(T, member) for each field.

A base class's trailing gap ends where the first member of a derived class starts when that is before
the end of the base (the derived class reuses the base's tail padding, as the Itanium C++ ABI lays out a
derived class whose base is not a POD).

A root class's virtual table is declared as the reflection sources call it: the class getter in slot 0
and the destructor after it (base/reflection/ReflectedClass.h, ClassOf and Destroy). The getter has no
known name and keeps its slot placeholder, vf_0x0.

Usage:
  python tools/gen_classes.py            # write include/reflected/ (and drop headers no longer listed)
  python tools/gen_classes.py --check    # exit 1 if include/reflected/ is not what this would write
The output is generated: regenerate, never hand-edit.
"""

from __future__ import annotations

import argparse
import csv
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "include" / "reflected"
TYPES = ROOT / "data" / "reflection_types.csv"
FIELDS = ROOT / "data" / "reflection_fields.csv"

# The classes declared (the pilot): their bases are declared with them.
CLASSES = [
    # base::core::CBaseObject > CGameObject > CCameraFX > ...; six share CCameraFX_Shake.
    "CCameraFX",
    "CCameraFX_Shake",
    "CCameraFX_CrossShake",
    "CCameraFX_MotionShake",
    "CCameraFX_OscillationShake",
    "CCameraFX_RandShake",
    "CCameraFX_RotationShake",
    "CCameraFX_SteadyCamera",
    # a root class with its own virtual table
    "CCooldownXBossFireBallDef",
    # base::global::timeline::CEvent's subclasses
    "CCharClassAIAttackEvent",
    "CCharClassAlternativeActionInitializeEvent",
    "CCharClassAlternativeActionPlayEvent",
    "CCharClassBaseFadeInEvent",
    "CCharClassBindLightToConstantAlphaEvent",
    "CCharClassCallEmmyEvent",
    "CCharClassCameraFXPresetEvent",
    "CCharClassEnableScenarioItemByNameEvent",
    "CCharClassFXFadeEvent",
    "CCharClassDropItemCrazyEvent",
]

# Reflected member types a declaration can spell: (spelling, size, alignment, header).
MEMBER_TYPES = {
    "float": ("f32", 4, 4, None),
    "bool": ("bool", 1, 1, None),
    "int": ("s32", 4, 4, None),
    "unsigned": ("u32", 4, 4, None),
    "base::global::CStrId": ("base::global::CStrId", 8, 8, "base/global/CStrId.h"),
    "base::global::CRntString": ("base::global::CRntString", 0x30, 8, "base/global/CRntString.h"),
    "base::global::CFilePathStrId": ("base::global::CFilePathStrId", 0x10, 8, "base/global/CFilePathStrId.h"),
    "base::math::CVector2D": ("base::math::CVector2D", 8, 4, "base/math/CVector.h"),
    "base::math::CVector3D": ("base::math::CVector3D", 0xC, 4, "base/math/CVector.h"),
    "base::math::CVector4D": ("base::math::CVector4D", 0x10, 4, "base/math/CVector4D.h"),
}


def tables() -> tuple[dict[str, dict], dict[str, list[tuple[int, str, str]]]]:
    with TYPES.open(newline="", encoding="utf-8") as f:
        types = {r["type"]: r for r in csv.DictReader(f) if r["kind"] == "class"}
    fields: dict[str, list[tuple[int, str, str]]] = {}
    with FIELDS.open(newline="", encoding="utf-8") as f:
        for r in csv.DictReader(f):
            if r["offset"] != "0xffffffff":  # a property: read and written through functions, no storage
                fields.setdefault(r["class"], []).append((int(r["offset"], 16), r["name"], r["type"]))
    for v in fields.values():
        v.sort()
    return types, fields


def chain(name: str, types: dict[str, dict]) -> list[str]:
    """`name` and its bases, the class first."""
    out = []
    while name:
        out.append(name)
        name = types[name]["parent"]
    return out


def declared(types: dict[str, dict]) -> list[str]:
    """Every class with a header: CLASSES and their bases, bases first."""
    out: list[str] = []
    for c in CLASSES:
        for b in reversed(chain(c, types)):
            if b not in out:
                out.append(b)
    return out


def include_of(name: str) -> str:
    """The header of class `name`, as an include path (base::core::CBaseObject -> reflected/base/core/CBaseObject.h)."""
    return "reflected/" + "/".join(name.split("::")) + ".h"


def data_size(name: str, types: dict[str, dict], fields: dict) -> int:
    """Where a derived class may place its first member: the class's size, or earlier when a reflected
    subclass's first member starts inside the base's tail padding."""
    size = int(types[name]["size"], 16)
    starts = [fields[k][0][0] for k, r in types.items() if r["parent"] == name and fields.get(k)]
    return min([size, *[s for s in starts if s < size]])


class LayoutError(Exception):
    pass


def class_lines(name: str, types: dict[str, dict], fields: dict) -> tuple[list[str], set[str]]:
    """The declaration of `name` and the headers its members need."""
    row = types[name]
    parent = row["parent"]
    *outer, short = name.split("::")
    size = int(row["size"], 16)
    end = data_size(name, types, fields)
    at = data_size(parent, types, fields) if parent else 8  # a root starts after its virtual table pointer
    includes: set[str] = set()
    body: list[str] = []
    for off, member, typ in fields.get(name, []):
        if typ not in MEMBER_TYPES:
            raise LayoutError(f"{name}::{member}: type {typ} is not declared")
        spelled, msize, align, header = MEMBER_TYPES[typ]
        if off < at or off % align:
            raise LayoutError(f"{name}::{member} at {off:#x}: overlaps or misaligned (next free {at:#x})")
        if off > at:
            body.append(f"    u8 pad_{at:#x}[{off - at:#x}];")
        body.append(f"    {spelled} {member};  // {off:#x}")
        if header:
            includes.add(header)
        at = off + msize
    if at > end:
        raise LayoutError(f"{name}: members end at {at:#x}, past {end:#x}")
    if end > at:
        body.append(f"    u8 pad_{at:#x}[{end - at:#x}];")
    base = f" : public {parent}" if parent else ""
    lines = [f"namespace {'::'.join(outer)} {{"] if outer else []
    lines += [f"class {short}{base} {{", "public:", "    DREAD_REFLECTED_CLASS_MEMBERS;"]
    if not parent:
        lines += ["    // The class getter (no known name) and the destructor, as the reflection functions call them.",
                  "    virtual const base::reflection::CClass* vf_0x0() const;",
                  f"    virtual ~{short}();"]
    lines += [*body, "};"]
    if outer:
        lines.append(f"}}  // namespace {'::'.join(outer)}")
    lines.append(f'static_assert(sizeof({name}) == {size:#x}, "{name} size");')
    if fields.get(name):
        lines += OFFSETOF_ALLOWED
        for off, member, _ in fields[name]:
            lines.append(f'static_assert(offsetof({name}, {member}) == {off:#x}, "{name}::{member}");')
        lines.append("#pragma GCC diagnostic pop")
    return lines, includes


# offsetof on a class with a virtual table (no standard layout) is conditionally supported: Clang and GCC give
# the Itanium layout's offset, as the reflection records it. Allowed only around the lines that use it.
OFFSETOF_ALLOWED = ["#pragma GCC diagnostic push", '#pragma GCC diagnostic ignored "-Winvalid-offsetof"']


def header(name: str, types: dict[str, dict], fields: dict) -> str:
    lines, includes = class_lines(name, types, fields)
    parent = types[name]["parent"]
    incs = ["#include <cstddef>", ""]
    if parent:
        incs.append(f'#include "{include_of(parent)}"')
    incs += [f'#include "{h}"' for h in sorted(includes | {"base/reflection/ReflectedClass.h"})]
    incs.append('#include "types.h"')
    head = ["#pragma once", "",
            "// Generated by tools/gen_classes.py from data/reflection_types.csv and data/reflection_fields.csv;",
            "// regenerate rather than edit. The class's reflected members at their offsets; the pad_ arrays are",
            "// members the reflection does not describe."]
    return "\n".join([*head, *incs, "", *lines]) + "\n"


def offsets() -> dict[tuple[str, str], int]:
    """{(class, member): offset} from the headers' static_asserts."""
    out = {}
    pat = re.compile(r'^static_assert\(offsetof\(([\w:]+), (\w+)\) == (0x[0-9a-f]+)', re.M)
    for f in OUT.rglob("*.h"):
        for cls, member, off in pat.findall(f.read_text(encoding="utf-8")):
            out[(cls, member)] = int(off, 16)
    return out


def declared_includes() -> dict[str, str]:
    """{class: its header's include path} for the headers on disk."""
    out = {}
    pat = re.compile(r"^static_assert\(sizeof\(([\w:]+)\) ==", re.M)
    for f in OUT.rglob("*.h"):
        m = pat.search(f.read_text(encoding="utf-8"))
        if m:
            out[m.group(1)] = include_of(m.group(1))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--version", help="accepted for tools/verify.py; the headers depend only on data/")
    args = ap.parse_args()
    types, fields = tables()
    want = {OUT / (include_of(c)[len("reflected/"):]): header(c, types, fields) for c in declared(types)}
    have = {f: f.read_text(encoding="utf-8") for f in OUT.rglob("*.h")} if OUT.exists() else {}
    stale = [f for f in have if f not in want] + [f for f, t in want.items() if have.get(f) != t]
    if args.check:
        for f in stale:
            print(f"stale: {f.relative_to(ROOT).as_posix()}")
        return 1 if stale else 0
    for f in have:
        if f not in want:
            f.unlink()
    for f, text in want.items():
        if have.get(f) != text:
            f.parent.mkdir(parents=True, exist_ok=True)
            f.write_text(text, encoding="utf-8", newline="\n")
    print(f"{len(want)} class headers in {OUT.relative_to(ROOT).as_posix()}/ ({len(stale)} written or removed)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
