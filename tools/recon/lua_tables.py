"""Find luaL_Reg-style {const char* name, lua_CFunction func} tables in a module's data."""

from __future__ import annotations

from image import R_AARCH64_RELATIVE, Image

MIN_ENTRIES = 3


def find_tables(img: Image) -> list[dict]:
    rel = {a for a, r in img.relocs.items() if r.type == R_AARCH64_RELATIVE}
    visited: set[int] = set()
    tables = []
    for a in sorted(rel):
        if a in visited:
            continue
        entries = []
        p = a
        while p in rel and p + 8 in rel:
            s, f = img.pointer(p), img.pointer(p + 8)
            name = img.cstring(s, 128) if s is not None else None
            # Single-letter names are packed format/type tables, not bindings.
            if not (name and len(name) > 1 and name.isidentifier() and img.in_text(f)):
                break
            entries.append({"name": name, "func": f})
            p += 16
        if len(entries) >= MIN_ENTRIES:
            visited.update(range(a, p, 8))
            tables.append({"addr": a, "entries": entries})
    return tables
