#!/usr/bin/env python3
"""Class declarations for reflected classes, from the reflection tables and two hand-kept tables: include/reflected/.

A reflected class registers its name, its parent, its size and each member's name, type and offset
(data/reflection_types.csv, data/reflection_fields.csv). This writes one header per selected class (and
per class of their base chains): the class with its real name, its base, its members by name and type at
their offsets, `pad_0x<offset>` arrays for the gaps nothing describes, and static_asserts on its size and
every member's offset. tools/gen_reflection.py then writes those classes' reflection functions as the
class's own static members (base/reflection/ReflectedClass.h), with sizeof(T) for the size and
offsetof(T, member) for each field. The class declares the refl_ functions it has (named in data/functions/
or by a source) and its descriptor, `struct refl_Description`.

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
known name and keeps its slot placeholder, vf_0x0. Every other class declares its destructor out of line
(`~T() override;`, or a `dtor` row's declaration), defined nowhere here: it is the class's key function, so
no unit that constructs the class emits a weak copy of its virtual table or of the implicit destructors.

Which classes: the rule is every reflected class the generator can express (its whole base chain lays out
to its reflected size, every member type is spelled, it is not nested in another reflected class and
src/reflection/ names no type nested in it) and whose reflection source converts (src/reflection/ writes
its block in the declared form, or tools/gen_reflection.py's pass would), and then every unit compiles: a
source of src/reflection/ that would both see a declared class (through a class it converts, or its bases)
and open a namespace of that class's name (a block it keeps in the namespace form, its parent's getter, the
CTunable statics `refl_fields_added` and `refl_tunable`) leaves that class and its subclasses out.
data/class_scope.csv (`class,reason`) limits the output: `*` for every class, or the classes by name (the
classes the two tables name are in scope too); a row with a reason leaves that class (and its subclasses)
out, for a reason the rule cannot see. A class in scope that fails the rule is an error.

Usage:
  python tools/gen_classes.py            # write include/reflected/ (and drop headers no longer selected)
  python tools/gen_classes.py --check    # exit 1 if include/reflected/ is not what this would write
  python tools/gen_classes.py --rule     # how many classes the rule selects with no scope, and why not the rest
  python tools/gen_classes.py --views    # declared classes whose hand-written unit still defines Unk... types
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
FUNCTIONS = ROOT / "data" / "functions"
REFLECTION = ROOT / "src" / "reflection"

# Reflected member types a declaration can spell: (spelling, size, alignment, header).
MEMBER_TYPES = {
    "float": ("f32", 4, 4, None),
    "bool": ("bool", 1, 1, None),
    "int": ("s32", 4, 4, None),
    "unsigned": ("u32", 4, 4, None),
    "base::global::CStrId": ("base::global::CStrId", 8, 8, "base/global/CStrId.h"),
    "base::global::CRntString": ("base::global::CRntString", 0x30, 8, "base/global/CRntString.h"),
    # Not base::global::CFilePathStrId: a reflected member takes 0x10 bytes, its declaration (base/global/
    # CFilePathStrId.h) only 8, so a class with one would not have its reflected size.
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


_CHILDREN: list = [None, {}]  # [the types table it was built from (kept alive, so its id is not reused), {class: children}]


def children(types: dict[str, dict]) -> dict[str, list[str]]:
    """{class: the classes whose parent it is}, indexed once per types table rather than once per class."""
    if _CHILDREN[0] is not types:
        kids: dict[str, list[str]] = {}
        for k, r in types.items():
            kids.setdefault(r["parent"], []).append(k)
        _CHILDREN[:] = [types, kids]
    return _CHILDREN[1]


def data_size(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA) -> int:
    """Where a derived class may place its first member: the class's size, or earlier when a subclass's
    first member starts inside the base's tail padding."""
    size = int(types[name]["size"], 16)
    starts = [ms[0].offset for k in children(types).get(name, []) for ms in [members(k, fields, extra)] if ms]
    return min([size, *[s for s in starts if s < size]])


def virtual_slots(name: str, types: dict[str, dict], extra: Extra) -> int:
    """How many virtual table slots class `name` has: its root's three and every `virtual` row on the way."""
    rows = [r for b in chain(name, types) for r in extra.methods.get(b, []) if r["kind"].startswith("virtual")]
    return 3 + len(rows)


def align_up(x: int, a: int) -> int:
    return (x + a - 1) // a * a


# The static reflection functions a declared class may have, as its header declares them (the namespace form's
# names and types: a class and a namespace mangle the same way).
REFL_MEMBERS = [
    ("refl_GetClass", "static const base::reflection::CClass* refl_GetClass()"),
    ("refl_AddAttributes", "static void refl_AddAttributes(base::reflection::CClass* cls)"),
    ("refl_AddFields", "static void refl_AddFields(base::reflection::CClass* cls)"),
    ("refl_Equals", "static bool refl_Equals(const void* a, const void* b)"),
    ("refl_Hash", "static s32 refl_Hash()"),
    ("refl_TypeOf", "static const base::reflection::CType& refl_TypeOf(const void* obj)"),
    ("refl_Construct", "static void refl_Construct(void* obj)"),
    ("refl_CopyConstruct", "static void refl_CopyConstruct(void* dst, const void* src)"),
    ("refl_MoveConstruct", "static void refl_MoveConstruct(void* dst, const void* src)"),
    ("refl_Destroy", "static void refl_Destroy(void* obj)"),
    ("refl_CopyAssign", "static void* refl_CopyAssign(void* dst, const void* src)"),
    ("refl_MoveAssign", "static void* refl_MoveAssign(void* dst, const void* src)"),
]
REFL_NAME = re.compile(r"\brefl_(?!Description\b)\w+")


def class_lines(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA,
                refl: dict[str, set[str]] | None = None) -> tuple[list[str], set[str]]:
    """The declaration of `name` and the headers its members need; `refl` ({class: its refl_ functions}) limits
    the reflection functions declared to those the class has (None: all of them)."""
    row = types[name]
    parent = row["parent"]
    *outer, short = name.split("::")
    size = int(row["size"], 16)
    end = data_size(name, types, fields, extra)
    at = data_size(parent, types, fields, extra) if parent else 8  # a root starts after its virtual table pointer
    leaf = not children(types).get(name)
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
        at = end
    if align_up(at, align) != size:
        # e.g. a base whose subclasses start inside its reflected size, but with nothing to pad it out to it
        raise LayoutError(f"{name}: lays out to {align_up(at, align):#x} bytes, not its reflected {size:#x}")

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
    lines += [f"class {short}{base} {{{vtable}", "public:",
              "    // The reflection functions it has (src/reflection/), and its descriptor's traits.",
              *[f"    {decl};" for fn, decl in REFL_MEMBERS if refl is None or fn in refl.get(name, ())],
              "    struct refl_Description;"]
    if not parent:
        lines += ["    // The class getter (no known name) and the destructor, as the reflection functions call them.",
                  "    virtual const base::reflection::CClass* vf_0x0() const;"]
        if not has_dtor:
            lines.append(f"    virtual ~{short}();")
    elif not has_dtor:
        # Declared out of line (defined with the original's virtual table, not in a source here): the class's
        # key function, so no unit that constructs it emits a weak copy of its virtual table or destructor.
        lines.append(f"    ~{short}() override;")
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


def header(name: str, types: dict[str, dict], fields: dict, extra: Extra = NO_EXTRA,
           refl: dict[str, set[str]] | None = None) -> str:
    lines, includes = class_lines(name, types, fields, extra, refl)
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


QUALIFIED_NAME = re.compile(r"\b\w+(?:::\w+)+")
NESTED_PART = re.compile(r"(?!refl_)[A-Z]")  # a type's name, not a member function (refl_...) or a namespace


NAMESPACE_OPEN = re.compile(r"^namespace ([\w:]+) \{", re.M)


@dataclass
class Unit:
    """One src/reflection/ source, as the clash check needs it."""
    path: str
    done: set[str]                 # classes it writes in the declared form
    blocks: dict[str, str | None]  # {class: parent} of its namespace-form blocks (None: a root)
    opened: set[str]               # namespaces it opens outside those blocks (`namespace X { extern ... }`)
    slots: dict[str, set[str]] = field(default_factory=dict)  # {class: SLOT_FORMS its declared-form lines show}


def prefixes(name: str) -> set[str]:
    """`a::b::c` -> {a, a::b, a::b::c}: every namespace a `namespace a::b::c {` opens."""
    parts = name.split("::")
    return {"::".join(parts[:i]) for i in range(1, len(parts) + 1)}


def reflection_blocks() -> tuple[set[str], dict[str, str], set[str], list[Unit]]:
    """(classes src/reflection/ already writes in the declared form, {class: body} of the namespace-form blocks,
    the names src/reflection/ qualifies another type with: `X` of every `X::Y` that is not one of X's own
    refl_ members, and each source as a Unit). Read once per run."""
    done: set[str] = set()
    blocks: dict[str, str] = {}
    outer: set[str] = set()
    units: list[Unit] = []
    for f in sorted(REFLECTION.glob("*.cpp")):
        text = f.read_text(encoding="utf-8")
        unit = Unit(f.name, set(DECLARED_FORM.findall(text)), {}, set())
        done |= unit.done
        starts = set()
        for m in CLASS_BLOCK.finditer(text):
            blocks[m.group(1)] = m.group(2)
            unit.blocks[m.group(1)] = m.group(4)
            starts.add(m.start())
        for m in NAMESPACE_OPEN.finditer(text):
            if m.start() not in starts:
                unit.opened |= prefixes(m.group(1))
        for m in QUALIFIED_NAME.finditer(text):
            parts = m.group(0).split("::")
            outer.update("::".join(parts[:i]) for i in range(1, len(parts)) if NESTED_PART.match(parts[i]))
        for m in DECLARED_LINE.finditer(text):
            tags = slot_forms(m.group(0))
            if tags:
                unit.slots.setdefault(m.group(1), set()).update(tags)
        units.append(unit)
    return done, blocks, outer, units


TYPE_HEAD = re.compile(r"\b(?:struct|class|union)\s+(\w+)\b[^;{}()]*\{")


def hand_written_types(short_names: set[str]) -> set[str]:
    """The qualified names among reflected classes' (`short_names`: their last parts) that a hand-written
    source under src/ or include/ defines as a type (tools/struct_check.py's reading): a declared class of the
    same name would be its second, different definition."""
    import struct_check  # standard library only (with tools/decl_check.py)

    out: set[str] = set()
    for top, glob in (("src", "*.cpp"), ("include", "*.h")):
        for f in (ROOT / top).rglob(glob):
            rel = f.relative_to(ROOT).as_posix()
            if f.is_relative_to(OUT) or not struct_check.decl_check.hand_written(rel):
                continue
            text = f.read_text(encoding="utf-8", errors="replace")
            if any(m.group(1) in short_names for m in TYPE_HEAD.finditer(text)):
                out.update(d.name for d in struct_check.definitions(text, rel))
    return out


QUALIFIED_REFL = re.compile(r"\b((?:\w+::)*\w+)::(refl_(?!Description\b)\w+)\b")


def refl_functions() -> dict[str, set[str]]:
    """{class: the refl_ functions it has}: named so in data/functions/, or named by a source (a block of
    src/reflection/ in either form, or a hand-written `T::refl_...`)."""
    out: dict[str, set[str]] = {}
    for path in sorted(FUNCTIONS.glob("*.csv")):
        for r in read_rows(path):
            m = QUALIFIED_REFL.fullmatch(r["name"])
            if m:
                out.setdefault(m.group(1), set()).add(m.group(2))
    for top, glob in (("src", "*.cpp"), ("include", "*.h")):
        for f in (ROOT / top).rglob(glob):
            if f.is_relative_to(OUT):
                continue
            text = f.read_text(encoding="utf-8", errors="replace")
            if "refl_" not in text:
                continue
            for m in QUALIFIED_REFL.finditer(text):
                out.setdefault(m.group(1), set()).add(m.group(2))
            if f.parent == REFLECTION:
                for regex in (CLASS_BLOCK, DECLARED_DESCRIPTION):  # unqualified in a block or a descriptor
                    for m in regex.finditer(text):
                        out.setdefault(m.group(1), set()).update(REFL_NAME.findall(m.group(2)))
    return out


DECLARED_DESCRIPTION = re.compile(r"^struct ([\w:]+)::refl_Description : ClassDescription \{\n(.*?)^\};$", re.M | re.S)


def clashes(classes: set[str], types: dict[str, dict], units: list[Unit]) -> dict[str, str]:
    """{declared class: the source that would also open a namespace of its name}. A unit includes the header of
    every class it writes in the declared form (and so its bases'); a block it keeps in the namespace form opens
    its class's namespace and declares its parent's getter in the parent's (DREAD_REFLECTED_CLASS), and the
    lines outside the blocks open theirs (the CTunable* statics, `refl_fields_added` and `refl_tunable`)."""
    out: dict[str, str] = {}
    for u in units:
        converted = (u.done | set(u.blocks)) & classes
        included = {b for c in converted for b in chain(c, types)}
        opened = set(u.opened)
        for c, parent in u.blocks.items():
            if c not in classes:
                opened |= prefixes(c) | (prefixes(parent) if parent else set())
            else:
                opened |= prefixes(c) - {c}
        for x in sorted(included & opened):
            out.setdefault(x, u.path)
    return out


# How a reflection function shows its class's virtual table (base/reflection/ReflectedClass.h): the getter in slot 0
# and the destructor after it (ClassOf, Destroy, or the declared form), or the destructor first.
SLOT_FORMS = [
    (re.compile(r"refl_TypeOf\(const void\* obj\) \{ return (?:ClassOf\(obj\)|\*static_cast<const [\w:]+\*>\(obj\)->vf_0x0\(\)); \}"),
     "getter"),
    (re.compile(r"refl_Destroy\(void\* obj\) \{ (?:Destroy\(obj\);|static_cast<[\w:]+\*>\(obj\)->~\w+\(\);) \}"), "destructor"),
    (re.compile(r"\b(?:ClassOfDestructorFirst|DestroyDestructorFirst)\(obj\)"), "destructor first"),
]
DECLARED_LINE = re.compile(r"^.*?\b((?:\w+::)*\w+)::refl_(?:TypeOf|Destroy)\(.*$", re.M)


def slot_forms(text: str) -> set[str]:
    """The SLOT_FORMS tags `text` (a namespace-form block's body or a declared-form line) shows."""
    return {tag for regex, tag in SLOT_FORMS if regex.search(text)}


def slot_evidence(blocks: dict[str, str], units: list[Unit]) -> dict[str, set[str]]:
    """{class: what its src/reflection/ functions show of its virtual table} (SLOT_FORMS), from the
    reflection_blocks() it is given: its namespace-form block, or its declared-form lines. A function that is not
    in src/reflection/ (say, a lifted one) shows nothing, whatever it does."""
    out: dict[str, set[str]] = {}
    for cls, body in blocks.items():
        tags = slot_forms(body)
        if tags:
            out.setdefault(cls, set()).update(tags)
    for unit in units:
        for cls, tags in unit.slots.items():
            out.setdefault(cls, set()).update(tags)
    return out


def reflected_destroys() -> set[str]:
    """The classes data/functions/ gives a refl_Destroy."""
    out = set()
    for path in sorted(FUNCTIONS.glob("*.csv")):
        for r in read_rows(path):
            m = QUALIFIED_REFL.fullmatch(r["name"])
            if m and m.group(2) == "refl_Destroy":
                out.add(m.group(1))
    return out


def unpinned_roots(roots: set[str], types: dict[str, dict], evidence: dict[str, set[str]],
                   destroys: set[str]) -> dict[str, str]:
    """{root: why} for each root whose virtual table src/reflection/ does not pin to the one a header declares (the
    getter in slot 0, the destructor after it). The evidence of every reflected class under the root counts, as
    they share its table: some class must call the getter in slot 0; if any has a refl_Destroy, some class must
    call the destructor after it; none may put the destructor first."""
    out: dict[str, str] = {}
    for r in sorted(roots):
        tree, todo = [], [r]
        while todo:
            c = todo.pop()
            tree.append(c)
            todo += children(types).get(c, [])
        seen = set().union(*(evidence.get(c, set()) for c in tree))
        if "destructor first" in seen:
            out[r] = "a class under it puts the destructor first in the virtual table"
        elif "getter" not in seen:
            out[r] = "no source of src/reflection/ shows the class getter in slot 0 of its virtual table"
        elif "destructor" not in seen and any(c in destroys for c in tree):
            out[r] = "no source of src/reflection/ shows where its virtual table holds the destructor"
    return out


def problems(name: str, types: dict[str, dict], fields: dict, extra: Extra,
             refl: tuple[set[str], dict[str, str]], memo: dict[str, list[str]] | None = None) -> list[str]:
    """Why the rule does not select `name` (empty: it does). `memo` keeps each base's own answer across calls
    (one run asks about every class, and a base is in many chains)."""
    done, blocks, *rest = refl
    outer = rest[0] if rest else set()
    memo = {} if memo is None else memo
    if "" not in memo:  # the tables' sizes and offsets, once per memo
        memo[""] = [({t: int(r["size"], 16) for t, r in types.items() if r["size"]},
                     {(c, n): off for c, fs in fields.items() for off, n, _ in fs})]
    sizes, offsets = memo[""][0]
    out = []
    for b in chain(name, types):
        if b not in memo:
            memo[b] = own_problems(b, types, fields, extra, done, blocks, sizes, offsets, outer)
        out += memo[b]
    return out


def own_problems(b: str, types: dict[str, dict], fields: dict, extra: Extra, done: set[str], blocks: dict[str, str],
                 sizes: dict[str, int], offsets: dict[tuple[str, str], int], outer: set[str] = frozenset()) -> list[str]:
    """Why class `b` itself (not its bases) cannot be declared."""
    parts = b.split("::")
    if any("::".join(parts[:i]) in types for i in range(1, len(parts))):
        return [f"{b}: nested in another reflected class"]
    if b in outer:
        # src/reflection/ declares that type in a namespace of b's name, which a class b would clash with
        return [f"{b}: src/reflection/ names a type nested in it ({b}::...)"]
    if b in EXCLUDED:
        return [f"{b}: excluded by data/class_scope.csv: {EXCLUDED[b]}"]
    try:
        class_lines(b, types, fields, extra)
    except LayoutError as e:
        return [str(e)]
    if b not in done and (b not in blocks or declared_class_block(b, blocks[b], sizes, offsets) is None):
        return [f"{b}: its reflection source does not convert to the declared form"]
    return []


def scope() -> set[str] | None:
    """data/class_scope.csv's classes (the rows without a reason); None for every class (`*`)."""
    names = {r["class"] for r in read_rows(SCOPE) if not r.get("reason")}
    return None if "*" in names else names


def exclusions() -> dict[str, str]:
    """{class: reason} of data/class_scope.csv's rows with a reason: a class the rule would select that is not
    declared, and why (it and its subclasses keep the namespace form)."""
    return {r["class"]: r["reason"] for r in read_rows(SCOPE) if r.get("reason")}


EXCLUDED = exclusions()


def selected(types: dict[str, dict], fields: dict, extra: Extra, limit: set[str] | None,
             refl: tuple, why: dict[str, str] | None = None,
             hand_defined: set[str] | None = None, destroys: set[str] | None = None) -> tuple[list[str], list[str]]:
    """(the classes the rule selects within `limit` (None: every class), the errors of those `limit` names);
    `why` gets the first reason of each class left out; `hand_defined` is hand_written_types(); `destroys` is
    reflected_destroys() (None: read it). The virtual-table evidence is read from `refl` (reflection_blocks())."""
    want = sorted(types) if limit is None else sorted(limit | extra.classes())
    out, errors = [], []
    memo: dict[str, list[str]] = {}
    for c in want:
        if c not in types:
            errors.append(f"{c}: not a reflected class")
            continue
        problem = problems(c, types, fields, extra, refl, memo)
        if not problem:
            out.append(c)
        else:
            if why is not None:
                why[c] = problem[0]
            if limit is not None:
                errors += problem
    # A class a hand-written source also defines (by its own name) would have two definitions: left out, with
    # its subclasses.
    if hand_defined:
        kept = []
        for c in out:
            hit = [b for b in chain(c, types) if b in hand_defined]
            if not hit:
                kept.append(c)
                continue
            message = f"{hit[0]}: a hand-written source defines a type of its name"
            if why is not None:
                why[c] = message
            if limit is not None:
                errors.append(f"{c}: {message}")
        out = kept
    # A root's virtual table is declared as the getter in slot 0 and the destructor after it: a root whose
    # src/reflection/ functions do not show that (or show otherwise) is left out, with every class under it.
    units = refl[3] if len(refl) > 3 else []
    unpinned = unpinned_roots({c for c in declared(types, out) if not types[c]["parent"]}, types,
                              slot_evidence(refl[1], units), reflected_destroys() if destroys is None else destroys)
    if unpinned:
        kept = []
        for c in out:
            root = chain(c, types)[-1]
            if root not in unpinned:
                kept.append(c)
                continue
            message = f"{root}: {unpinned[root]}"
            if why is not None:
                why[c] = message
            if limit is not None:
                errors.append(f"{c}: {message}")
        out = kept
    # A unit that would both see a declared class and open a namespace of its name does not compile: that class
    # (and so every class based on it) is left out, until no unit clashes.
    while True:
        clash = clashes(set(declared(types, out)), types, units)
        if not clash:
            break
        kept = []
        for c in out:
            hit = [b for b in chain(c, types) if b in clash]
            if not hit:
                kept.append(c)
                continue
            message = f"{hit[0]}: src/reflection/{clash[hit[0]]} opens a namespace of its name"
            if why is not None:
                why[c] = message
            if limit is not None:
                errors.append(f"{c}: {message}")
        out = kept
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
    classes, errors = selected(types, fields, extra, scope(), reflection_blocks(), hand_defined=hand_written_types(
        {t.split("::")[-1] for t in types}))
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


# --rule's tally: the first reason a class is left out, by kind.
REASONS = [
    ("a member type has no declaration here", ("is not declared",)),
    ("its layout does not fit (overlap, misalignment, past the end, or the wrong size)",
     ("overlaps or misaligned", "past", "lays out to")),
    ("nested in another reflected class", ("nested in another reflected class",)),
    ("src/reflection/ names a type nested in it", ("names a type nested in it",)),
    ("a unit opens a namespace of its name (CTunable statics, a namespace-form subclass)", ("opens a namespace",)),
    ("a hand-written source defines a type of its name", ("a hand-written source defines",)),
    ("its reflection source does not convert", ("does not convert",)),
    ("its root's virtual table is not shown by src/reflection/", ("virtual table",)),
    ("excluded by data/class_scope.csv", ("excluded by data/class_scope.csv",)),
]
UNK_TYPE = re.compile(r"^(?:struct|class|union) (Unk\w*)\b[^;\n]*$", re.M)  # a definition, not a declaration


def hand_written_views(classes: list[str]) -> list[tuple[str, str]]:
    """(class, `file: Unk types`) for each declared class whose hand-written unit (a src/ or include/ path with a
    directory or file named for the class) defines an Unk... type: possibly a second view of the class, to be
    merged into data/class_members.csv and data/class_methods.csv. A heuristic report, not a gate."""
    short = {c.split("::")[-1]: c for c in classes}
    out = []
    for top in ("src", "include"):
        for p in sorted((ROOT / top).rglob("*")):
            rel = p.relative_to(ROOT).as_posix()
            if p.suffix not in (".h", ".cpp") or rel.startswith(("src/reflection", "src/clones/", "src/tables/",
                                                                     "src/lifted/", "include/reflected/")):
                continue
            names = {part for part in p.relative_to(ROOT / top).parts[:-1]} | {p.stem}
            hit = sorted(short[n] for n in names if n in short)
            if not hit:
                continue
            unk = sorted(set(UNK_TYPE.findall(p.read_text(encoding="utf-8", errors="replace"))))
            if unk:
                out += [(c, f"{rel}: {', '.join(unk)}") for c in hit]
    return sorted(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--check", action="store_true")
    ap.add_argument("--rule", action="store_true", help="count the classes the rule selects with no scope, and why the rest are not")
    ap.add_argument("--views", action="store_true",
                    help="list declared classes whose hand-written unit still defines an Unk... type (a second view, perhaps)")
    ap.add_argument("--version", help="accepted for tools/verify.py; the headers depend only on data/ and src/reflection/")
    args = ap.parse_args()
    types, fields = tables()
    try:
        extra = extra_tables()
    except LayoutError as e:
        print(f"error: {e}", file=sys.stderr)
        return 1
    refl = reflection_blocks()
    hand = hand_written_types({t.split("::")[-1] for t in types})
    if args.rule:
        why: dict[str, str] = {}
        classes, _ = selected(types, fields, extra, None, refl, why, hand)
        print(f"{len(classes)} of {len(types)} reflected classes pass the rule ({len(declared(types, classes))} headers)")
        tally: dict[str, int] = {}
        for reason in why.values():
            kind = next((k for k, pats in REASONS if any(p in reason for p in pats)), "other")
            tally[kind] = tally.get(kind, 0) + 1
        for kind, n in sorted(tally.items(), key=lambda kv: -kv[1]):
            print(f"  {n:5}  {kind}")
        return 0
    if args.views:
        for cls, where in hand_written_views(declared(types, selected(types, fields, extra, scope(), refl,
                                                                       hand_defined=hand)[0])):
            print(f"{cls}: {where}")
        return 0
    classes, errors = selected(types, fields, extra, scope(), refl, hand_defined=hand)
    for e in errors:
        print(f"error: {e}", file=sys.stderr)
    if errors:
        return 1
    refl_of = refl_functions()
    want = {OUT / (include_of(c)[len("reflected/"):]): header(c, types, fields, extra, refl_of)
            for c in declared(types, classes)}
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
