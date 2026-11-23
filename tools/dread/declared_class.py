"""A reflection source's class block in the declared-class form, as text only (no binary, no capstone).

tools/gen_reflection.py writes a reflected class's functions in a namespace block; when tools/gen_classes.py
declares the class, the block becomes the class's own static members (gen_reflection.with_declared_classes).
tools/gen_classes.py uses declared_class_block to decide whether a class's source converts, and must run
from tracked files alone (CI's policy job), so this module imports nothing beyond the standard library.
"""
from __future__ import annotations

import re

# A class block of the namespace form, with its getter's macro.
CLASS_BLOCK = re.compile(r"^namespace ([\w:]+) \{\n(.*?)^\}  // namespace \1\n"
                         r"DREAD_REFLECTED_(CLASS|ROOT_CLASS)\(\1(?:, ([\w:]+))?\);$", re.M | re.S)
REFL_DECLARATION = re.compile(r"^[^ ].*? refl_\w+\([^)]*\);$")
REFL_DEFINITION = re.compile(r"^([^ ].*?) (refl_\w+\(.*)$")
FIELD_OFFSET = re.compile(r'^(    DREAD_REFLECTED_(?:FIELD|CLASS_FIELD|FIELD_OF)\((?:"[^"]*"|[^,]+), "(\w+)", )(0x[0-9a-f]+|\d+)\b')


def declared_class_block(name: str, body: str, sizes: dict[str, int], offsets: dict[tuple[str, str], int]) -> str | None:
    """The namespace block of class `name` as its declared class's member definitions (tools/gen_classes.py):
    sizeof and offsetof for the numbers the header asserts, the class type for the virtual calls; None
    when a line has no such form."""
    if "DestructorFirst" in body:
        return None  # the header declares the getter-first virtual table
    short = name.split("::")[-1]
    out = []
    for line in body.split(chr(10))[:-1]:
        if REFL_DECLARATION.match(line):
            continue  # declared by the class
        if line == "struct refl_Description : ClassDescription {":
            out.append(f"struct {name}::refl_Description : ClassDescription {{")
            continue
        m = re.fullmatch(r"    static constexpr u64 size = (0x[0-9a-f]+);", line)
        if m:
            if int(m.group(1), 16) != sizes.get(name):
                return None
            out.append(f"    static constexpr u64 size = sizeof({short});")
            continue
        m = FIELD_OFFSET.match(line)
        if m:
            if offsets.get((name, m.group(2))) != int(m.group(3), 0):
                return None
            out.append(f"{m.group(1)}offsetof({short}, {m.group(2)}){line[m.end():]}")
            continue
        m = REFL_DEFINITION.match(line)
        if m:
            # in a member's body the class is named without its namespace
            line = f"{m.group(1)} {name}::{m.group(2)}"
            line = line.replace("{ return ClassOf(obj); }", f"{{ return *static_cast<const {short}*>(obj)->vf_0x0(); }}")
            line = line.replace("{ Destroy(obj); }", f"{{ static_cast<{short}*>(obj)->~{short}(); }}")
        elif not (line.startswith(" ") or line in ("}", "};")):
            return None
        out.append(line)
    return chr(10).join(out) + chr(10)
