#!/usr/bin/env python3
"""Class declarations for reflected classes, from the reflection tables and two hand-kept tables: include/reflected/.

A reflected class registers its name, its parent, its size and each member's name, type and offset
(data/reflection_types.csv, data/reflection_fields.csv). This writes one header per selected class (and
per class of their base chains): the class with its real name, its base, its members by name and type at
their offsets, `pad_0x<offset>` arrays for the gaps nothing describes, and static_asserts on its size and
every member's offset. tools/gen_reflection.py then writes those classes' reflection functions as the
class's own static members (base/reflection/ReflectedClass.h, DREAD_REFLECTED_CLASS_MEMBERS), with sizeof(T)
for the size and offsetof(T, member) for each field.

The header is the class's one declaration, so what the reflection does not record is kept in two tracked
tables that this merges in (a header is never edited by hand):

  data/class_members.csv  class,offset,type,name,init,source,note
      A member the reflection does not describe, at its offset: `type` is a spelling (f32, bool,
      base::math::CVector3D, ...) or a reflected type name, `name` its real name or a placeholder
      (`field_0x50`) when none is known (the rules for names are CLAUDE.md's), `note` a comment written
      after it. A row at a reflected member's offset, with its name, adds only `init`: the member's
      default initializer, as the class's constructor sets it. `source` says where the knowledge comes
      from (the matched function that uses it, RTTI, a string).
  data/class_methods.csv  class,kind,declaration,address,definition,source
      kind `vtable`: the class's virtual table is the original's object at `address` (written as
      `// vtable g_unk_<address>` on the class, which tools/dread/asm_labels.py reads).
      kind `ctor`, `dtor`, `static` or `plain`: `declaration` is written into the class body. A
      constructor's or destructor's `address` names the original's function (`// fn_<address>` after
      it, which tools/dread/asm_labels.py reads). Any other method takes no address: name the function
      through tools/rename.py and declare the method plainly (an asm label would hide the name). `definition` `default` defines it after the class as
      `inline T::T() = default;`: a constructor the original inlines, user-provided, so `T()` does not
      zero the object before running it. A destructor row replaces a root's generated destructor.
      kind `virtual <slot>`: a virtual function the class adds, in that slot of the table (a root's first
      is 3, after the getter and the two destructor slots; the rows of a class and its bases count up).

An overlap or a misaligned member, a member past the class's data, a type the generator cannot spell,
a malformed row (a missing column, an offset or address that is not hex), a second row for one member
or a second virtual table is an error, reported with the file and line.

Padding: a gap that the next member's alignment explains (and a leaf class's tail padding) is left to
the compiler, so the implicit copy of a class copies its members, not its padding. A base class's trailing
gap ends where the first member of a derived class starts when that is before the end of the base (the
derived class reuses the base's tail padding, as the Itanium C++ ABI lays out a derived class whose base is
not a POD).

A root class's virtual table is declared as the reflection sources call it: the class getter in slot 0
and the destructor after it (base/reflection/ReflectedClass.h, ClassOf and Destroy). The getter has no
known name and keeps its slot placeholder, vf_0x0.

Which classes: the rule is every reflected class the generator can express (its whole base chain lays out,
every member type is spelled, it is not nested in another reflected class) and whose reflection source
converts (src/reflection/ writes its block in the declared form, or tools/gen_reflection.py's pass would).
data/class_scope.csv (`class`; `*` for every class) limits the output while the family is brought over;
the classes the two tables name are in scope too. A class in scope that fails the rule is an error.

Usage:
  python tools/gen_classes.py            # write include/reflected/ (and drop headers no longer selected)
  python tools/gen_classes.py --check    # exit 1 if include/reflected/ is not what this would write
  python tools/gen_classes.py --rule     # how many classes the rule selects with no scope (writes nothing)
The output is generated: regenerate, never hand-edit.
"""

from __future__ import annotations

import argparse
import csv
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

# Tracked files and the standard library only: CI's policy job runs --check without game data or capstone.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread.declared_class import CLASS_BLOCK, declared_class_block  # noqa: E402

ROOT =Path(__file__).resolve().parent.parent
OUT = ROOT / "include" / "reflected"
TYPES = ROOT / "data" / "reflection_types.csv"
FIELDS = ROOT / "data" / "reflection_fields.csv"
MEMBERS = ROOT / "data" / "class_members.csv"
METHODS = ROOT / "data" / "class_methods.csv"
SCOPE = ROOT / "data" / "class_scope.csv"
REFLECTION = ROOT / "src" / "reflection"

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
# The same types by their spelling (the extra members' `type`), and the other integer spellings.
SPELLINGS = {v[0]: v for v in MEMBER_TYPES.values()} | {
    "u8": ("u8", 1, 1, None), "s8": ("s8", 1, 1, None), "u16": ("u16", 2, 2, None), "s16": ("s16", 2, 2, None),
    "u64": ("u64", 8, 8, None), "s64": ("s64", 8, 8, None), "f64": ("f64", 8, 8, None),
}
KINDS = ("vtable", "ctor", "dtor", "static", "plain")


class LayoutError(Exception):
    pass


@dataclass
class Extra:
    """The hand-kept knowledge of data/class_members.csv and data/class_methods.csv, by class."""
    members: dict[str, list[dict]] = field(default_factory=dict)
    methods: dict[str, list[dict]] = field(default_factory=dict)

    def classes(self) -> set[str]:
        return set(self.members) | set(self.methods)


NO_EXTRA = Extra()


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


def read_rows(path: Path) -> list[dict]:
    if not path.exists():
        return []
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


MEMBER_COLUMNS = ("class", "offset", "type", "name", "init", "source", "note")
METHOD_COLUMNS = ("class", "kind", "declaration", "address", "definition", "source")


def table_rows(rows: list[dict], columns: tuple[str, ...], where: str) -> list[dict]:
    """`rows` checked for shape: every column present (a short row has None), none extra. A bad row is a
    LayoutError naming `where` (the file) and its line."""
    out = []
    for line, r in enumerate(rows, start=2):  # line 1 is the header
        missing = [c for c in columns if r.get(c) is None]
        if missing or None in r or set(r) - set(columns):
            what = f"missing {', '.join(missing)}" if missing else "extra or unknown columns"
            raise LayoutError(f"{where}:{line}: {what} (columns: {','.join(columns)})")
        if not r["class"]:
            raise LayoutError(f"{where}:{line}: no class")
        out.append({**r, "_where": f"{where}:{line}"})
    return out


def extra_from_rows(member_rows: list[dict], method_rows: list[dict],
                    members_at: str = "data/class_members.csv", methods_at: str = "data/class_methods.csv") -> Extra:
    """The two tables, checked: offsets and addresses are hex, a member or a virtual table appears once."""
    out = Extra()
    seen: dict[tuple[str, int], str] = {}
    for r in table_rows(member_rows, MEMBER_COLUMNS, members_at):
        if not re.fullmatch(r"0x[0-9a-f]+", r["offset"]):
            raise LayoutError(f"{r['_where']}: offset {r['offset']!r} is not lowercase hex (0x..)")
        if not r["name"] or not r["type"]:
            raise LayoutError(f"{r['_where']}: a member needs a type and a name")
        key = (r["class"], int(r["offset"], 16))
        if key in seen:
            raise LayoutError(f"{r['_where']}: {r['class']} at {r['offset']} already has a row ({seen[key]})")
        seen[key] = r["_where"]
        out.members.setdefault(r["class"], []).append(r)
    vtables: dict[str, str] = {}
    for r in table_rows(method_rows, METHOD_COLUMNS, methods_at):
        if r["address"] and not re.fullmatch(r"0x71[0-9a-f]{8}", r["address"]):
            raise LayoutError(f"{r['_where']}: address {r['address']!r} is not 0x71xxxxxxxx")
        if r["kind"] == "vtable":
            if r["class"] in vtables:
                raise LayoutError(f"{r['_where']}: {r['class']} already has a vtable row ({vtables[r['class']]})")
            if not r["address"]:
                raise LayoutError(f"{r['_where']}: a vtable row needs its address")
            vtables[r["class"]] = r["_where"]
        out.methods.setdefault(r["class"], []).append(r)
    return out


def extra_tables() -> Extra:
    return extra_from_rows(read_rows(MEMBERS), read_rows(METHODS))


def member_type(typ: str) -> tuple[str, int, int, str | None] | None:
    return MEMBER_TYPES.get(typ) or SPELLINGS.get(typ)


@dataclass
class Member:
    offset: int
    name: str
    type: str
    reflected: bool
    init: str = ""
    note: str = ""


def members(name: str, fields: dict, extra: Extra) -> list[Member]:
    """The reflected members of `name` and its extra ones (data/class_members.csv), by offset."""
    out = [Member(off, n, t, True) for off, n, t in fields.get(name, [])]
    for row in extra.members.get(name, []):
        off = int(row["offset"], 16)
        same = [m for m in out if m.reflected and m.offset == off]
        if same:
            if same[0].name != row["name"]:
                raise LayoutError(f"{name}::{row['name']} at {off:#x}: overlaps the reflected {same[0].name}")
            if not row["init"]:
                raise LayoutError(f"{name}::{row['name']}: a reflected member's row gives only its init")
            if same[0].init:
                raise LayoutError(f"{name}::{row['name']}: a second init row")
            same[0].init, same[0].note = row["init"], row["note"]
            continue
        out.append(Member(off, row["name"], row["type"], False, row["init"], row["note"]))
    return sorted(out, key=lambda m: m.offset)


def chain(name: str, types: dict[str, dict]) -> list[str]:
    """`name` and its bases, the class first."""
    out = []
    while name:
        out.append(name)
        name = types[name]["parent"]
    return out


def include_of(name: str) -> str:
    """The header of class `name`, as an include path (base::core::CBaseObject -> reflected/base/core/CBaseObject.h)."""
    return "reflected/" + "/".join(name.split("::")) + ".h"


def data_size(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA) -> int:
    """Where a derived class may place its first member: the class's size, or earlier when a subclass's
    first member starts inside the base's tail padding."""
    size = int(types[name]["size"], 16)
    starts = [ms[0].offset for k, r in types.items() if r["parent"] == name for ms in [members(k, fields, extra)] if ms]
    return min([size, *[s for s in starts if s < size]])


def virtual_slots(name: str, types: dict[str, dict], extra: Extra) -> int:
    """How many virtual table slots class `name` has: its root's three and every `virtual` row on the way."""
    rows = [r for b in chain(name, types) for r in extra.methods.get(b, []) if r["kind"].startswith("virtual")]
    return 3 + len(rows)


def align_up(x: int, a: int) -> int:
    return (x + a - 1) // a * a


def class_lines(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA) -> tuple[list[str], set[str]]:
    """The declaration of `name` and the headers its members need."""
    row = types[name]
    parent = row["parent"]
    *outer, short = name.split("::")
    size = int(row["size"], 16)
    end = data_size(name, types, fields, extra)
    at = data_size(parent, types, fields, extra) if parent else 8  # a root starts after its virtual table pointer
    leaf = not any(r["parent"] == name for r in types.values())
    align = 8  # the virtual table pointer's
    includes: set[str] = set()
    body: list[str] = []
    own = members(name, fields, extra)
    for m in own:
        t = member_type(m.type)
        if t is None:
            raise LayoutError(f"{name}::{m.name}: type {m.type} is not declared")
        spelled, msize, malign, header = t
        if m.offset < at or m.offset % malign:
            raise LayoutError(f"{name}::{m.name} at {m.offset:#x}: overlaps or misaligned (next free {at:#x})")
        if m.offset > align_up(at, malign):
            body.append(f"    u8 pad_{at:#x}[{m.offset - at:#x}];")
        init = f" = {m.init}" if m.init else ""
        note = f": {m.note}" if m.note else ""
        body.append(f"    {spelled} {m.name}{init};  // {m.offset:#x}{note}")
        if header:
            includes.add(header)
        at = m.offset + msize
        align = max(align, malign)
    if at > end:
        raise LayoutError(f"{name}: members end at {at:#x}, past {end:#x}")
    if end > at and not (leaf and end == size == align_up(at, align)):
        body.append(f"    u8 pad_{at:#x}[{end - at:#x}];")

    vtable = ""
    methods: list[str] = []
    after: list[str] = []
    has_dtor = False
    slot = virtual_slots(parent, types, extra) if parent else 3  # a root: the getter, then two destructor slots
    for r in extra.methods.get(name, []):
        kind, decl, addr = r["kind"], r["declaration"], r["address"]
        if kind.split()[0] == "virtual":
            if kind != f"virtual {slot}" or not decl.startswith("virtual "):
                raise LayoutError(f"{name}: {decl!r} is a new virtual function in slot {slot}, not {kind!r}")
            slot += 1
            kind = "plain"
        elif kind not in KINDS:
            raise LayoutError(f"{name}: method kind {kind!r} is not one of {', '.join(KINDS)} or `virtual <slot>`")
        if addr and not re.fullmatch(r"0x71[0-9a-f]{8}", addr):
            raise LayoutError(f"{name}: address {addr!r} is not 0x71xxxxxxxx")
        if kind == "vtable":
            if vtable:
                raise LayoutError(f"{r.get('_where', name)}: {name} already has a vtable row")
            vtable = f"  // vtable g_unk_{addr[2:]}"
            continue
        if kind == "ctor" and not decl.startswith(f"{short}("):
            raise LayoutError(f"{name}: constructor {decl!r} does not start with {short}(")
        if kind == "dtor":
            if not re.match(rf"(virtual )?~{short}\(\)", decl):
                raise LayoutError(f"{name}: destructor {decl!r} is not ~{short}()")
            has_dtor = True
        if addr and kind not in ("ctor", "dtor"):
            # an asm label would keep the symbol fn_<addr>: the function list would never learn the name
            raise LayoutError(f"{r.get('_where', name)}: {decl!r} has an address; name the function through "
                              f"tools/rename.py and declare the method plainly, with no address")
        methods.append(f"    {decl};  // fn_{addr[2:]}" if addr else f"    {decl};")
        if r["definition"] == "default":
            if kind not in ("ctor", "dtor"):
                raise LayoutError(f"{name}: only a constructor or destructor is defined as default")
            after.append(f"inline {short}::{decl.removeprefix('virtual ')} = default;")
        elif r["definition"]:
            raise LayoutError(f"{name}: definition {r['definition']!r} is not `default` or empty")

    base = f" : public {parent}" if parent else ""
    lines = [f"namespace {'::'.join(outer)} {{"] if outer else []
    lines += [f"class {short}{base} {{{vtable}", "public:", "    DREAD_REFLECTED_CLASS_MEMBERS;"]
    if not parent:
        lines += ["    // The class getter (no known name) and the destructor, as the reflection functions call them.",
                  "    virtual const base::reflection::CClass* vf_0x0() const;"]
        if not has_dtor:
            lines.append(f"    virtual ~{short}();")
    lines += [*methods, *body, "};", *after]
    if outer:
        lines.append(f"}}  // namespace {'::'.join(outer)}")
    lines.append(f'static_assert(sizeof({name}) == {size:#x}, "{name} size");')
    if own:
        lines += OFFSETOF_ALLOWED
        for m in own:
            lines.append(f'static_assert(offsetof({name}, {m.name}) == {m.offset:#x}, "{name}::{m.name}");')
        lines.append("#pragma GCC diagnostic pop")
    return lines, includes


# offsetof on a class with a virtual table (no standard layout) is conditionally supported: Clang and GCC give
# the Itanium layout's offset, as the reflection records it. Allowed only around the lines that use it.
OFFSETOF_ALLOWED = ["#pragma GCC diagnostic push", '#pragma GCC diagnostic ignored "-Winvalid-offsetof"']


def header(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA) -> str:
    lines, includes = class_lines(name, types, fields, extra)
    parent = types[name]["parent"]
    incs = ["#include <cstddef>", ""]
    if parent:
        incs.append(f'#include "{include_of(parent)}"')
    incs += [f'#include "{h}"' for h in sorted(includes | {"base/reflection/ReflectedClass.h"})]
    incs.append('#include "types.h"')
    head = ["#pragma once", "",
            "// Generated by tools/gen_classes.py from data/reflection_types.csv, data/reflection_fields.csv,",
            "// data/class_members.csv and data/class_methods.csv; regenerate rather than edit. The class's members",
            "// at their offsets; the pad_ arrays are members nothing describes yet."]
    return "\n".join([*head, *incs, "", *lines]) + "\n"


# --- Which classes ---------------------------------------------------------------------------------------

DECLARED_FORM = re.compile(r"^DREAD_REFLECTED_DECLARED_(?:ROOT_)?CLASS\(([\w:]+)[,)]", re.M)


def reflection_blocks() -> tuple[set[str], dict[str, str]]:
    """(classes src/reflection/ already writes in the declared form, {class: body} of the namespace-form blocks)."""
    done: set[str] = set()
    blocks: dict[str, str] = {}
    for f in sorted(REFLECTION.glob("*.cpp")):
        text = f.read_text(encoding="utf-8")
        done.update(DECLARED_FORM.findall(text))
        for m in CLASS_BLOCK.finditer(text):
            blocks[m.group(1)] = m.group(2)
    return done, blocks


def problems(name: str, types: dict[str, dict], fields: dict, extra: Extra,
             refl: tuple[set[str], dict[str, str]]) -> list[str]:
    """Why the rule does not select `name` (empty: it does)."""
    done, blocks = refl
    sizes = {t: int(r["size"], 16) for t, r in types.items() if r["size"]}
    offsets = {(c, n): off for c, fs in fields.items() for off, n, _ in fs}
    out = []
    for b in chain(name, types):
        outer = b.split("::")
        if any("::".join(outer[:i]) in types for i in range(1, len(outer))):
            out.append(f"{b}: nested in another reflected class")
            continue
        try:
            class_lines(b, types, fields, extra)
        except LayoutError as e:
            out.append(str(e))
            continue
        if b not in done and (b not in blocks or declared_class_block(b, blocks[b], sizes, offsets) is None):
            out.append(f"{b}: its reflection source does not convert to the declared form")
    return out


def scope() -> set[str] | None:
    """data/class_scope.csv's classes; None for every class (`*`)."""
    names = {r["class"] for r in read_rows(SCOPE)}
    return None if "*" in names else names


def selected(types: dict[str, dict], fields: dict, extra: Extra, limit: set[str] | None,
             refl: tuple[set[str], dict[str, str]]) -> tuple[list[str], list[str]]:
    """(the classes the rule selects within `limit` (None: every class), the errors of those `limit` names)."""
    want = sorted(types) if limit is None else sorted(limit | extra.classes())
    out, errors = [], []
    for c in want:
        if c not in types:
            errors.append(f"{c}: not a reflected class")
            continue
        why = problems(c, types, fields, extra, refl)
        if not why:
            out.append(c)
        elif limit is not None:
            errors += why
    return out, errors


def declared(types: dict[str, dict], classes: list[str]) -> list[str]:
    """Every class with a header: `classes` and their bases, bases first."""
    out: list[str] = []
    for c in classes:
        for b in reversed(chain(c, types)):
            if b not in out:
                out.append(b)
    return out


def declared_classes() -> list[str]:
    """The classes this writes a header for (tools/gen_class_getters.py leaves their getters to the class)."""
    types, fields = tables()
    try:
        extra = extra_tables()
    except LayoutError as e:
        raise SystemExit(f"gen_classes: {e}")
    classes, errors = selected(types, fields, extra, scope(), reflection_blocks())
    if errors:
        raise SystemExit("gen_classes: " + "; ".join(errors))
    return declared(types, classes)


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
    ap.add_argument("--rule", action="store_true", help="count the classes the rule selects with no scope")
    ap.add_argument("--version", help="accepted for tools/verify.py; the headers depend only on data/ and src/reflection/")
    args = ap.parse_args()
    types, fields = tables()
    try:
        extra = extra_tables()
    except LayoutError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    refl = reflection_blocks()
    if args.rule:
        classes, _ = selected(types, fields, extra, None, refl)
        print(f"{len(classes)} of {len(types)} reflected classes pass the rule ({len(declared(types, classes))} headers)")
        return 0
    classes, errors = selected(types, fields, extra, scope(), refl)
    for e in errors:
        print(f"error: {e}", file=sys.stderr)
    if errors:
        return 1
    want = {OUT / (include_of(c)[len("reflected/"):]): header(c, types, fields, extra) for c in declared(types, classes)}
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
