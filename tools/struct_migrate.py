#!/usr/bin/env python3
"""Move a type that several sources define with the same text into one header.

tools/struct_check.py counts the struct, class and union names that more than one file defines; where every
definition has the same text (`--kind same`), the move is mechanical, and this does it the way
tools/hub_migrate.py does a declaration's: the definition goes into the header once, each source loses its
own and includes the header, and a source keeps the edit only when every function in it compiles to the same
code as before. A source that does not (or that stops compiling: its own later types may need the removed one
earlier than the include gives it) is put back and listed.

  python tools/struct_migrate.py --version 2.1.0 NAME --header include/game/UnkX.h [--needs base/y.h ...]
                                 [--drop-comment REGEX]   # also a one-line comment above a copy that says this

NAME is as tools/struct_check.py prints it (with its namespace; not a template). The header is created when it
does not exist (`#pragma once`, a comment, the --needs includes, the definition) or gets the definition
appended when it does not have one. Only .cpp sources under src/ are edited: a definition in a header of src/
is listed, since all its includers would have to be compared. A name whose definitions differ is refused.
Needs the dump. Afterwards: tools/check.py and tools/struct_check.py, as after any change of src/.
"""

from __future__ import annotations

import argparse
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import decl_check  # noqa: E402
import struct_check  # noqa: E402


def span(text: str, name: str, comment: str | None = None) -> tuple[int, int] | None:
    """(first line, last line), 0-based, of the definition of the type `name` (its bare name) at the margin of
    this text; None when there is not exactly one. A one-line comment directly above it is part of it when it
    matches `comment` (a regular expression): a comment is only taken away on the word of whoever runs this,
    since the block above a type is as often about the file or about the declarations that follow."""
    lines = text.split("\n")
    starts = [i for i, line in enumerate(lines) if re.match(rf"^(?:struct|class|union) {re.escape(name)}\b[^;]*\{{", line)]
    if len(starts) != 1:
        return None
    i = j = starts[0]
    if "};" not in lines[i]:
        while j < len(lines) and not lines[j].startswith("};"):
            j += 1
        if j == len(lines):
            return None
    # the layout assertions directly under it are about it and go where it goes
    while j + 1 < len(lines) and lines[j + 1].startswith("static_assert") and re.search(rf"\b{re.escape(name)}\b", lines[j + 1]):
        j += 1
    if (comment and i > 0 and lines[i - 1].startswith("//") and re.search(comment, lines[i - 1])
            and (i < 2 or not lines[i - 2].startswith("//"))):
        i -= 1
    return i, j


def without(text: str, name: str, header: str, comment: str | None = None) -> str | None:
    """The source without its definition of `name` and with the header included (after the last include above
    the definition, or under the opening comment); None when the definition is not found as one block at the margin."""
    found = span(text, name, comment)
    if found is None:
        return None
    lines = text.split("\n")
    i, j = found
    out = lines[:i] + lines[j + 1:]
    if i < len(out) and i > 0 and not out[i].strip() and not out[i - 1].strip():
        del out[i]
    include = f'#include "{header}"'
    if include not in out:
        # the last include above where the definition was: a later one (a file can include something further
        # down) would give the type too late
        at = max((k for k, line in enumerate(out[:i]) if line.startswith("#include")), default=None)
        if at is None:
            at = next((k for k, line in enumerate(out) if not line.startswith("//")), 0) - 1
        out.insert(at + 1, include)
    return "\n".join(out)


def header_text(header: Path, block: str, needs: list[str], name: str) -> str:
    """The header with the definition in it: as it is when it defines the type already, with the block
    appended when it exists, new otherwise."""
    if header.is_file():
        text = header.read_text(encoding="utf-8").replace("\r\n", "\n")
        if span(text, name.split("::")[-1]) is not None:
            return text
        return text.rstrip("\n") + "\n\n" + block + "\n"
    includes = "".join(f'#include "{n}"\n' for n in [*needs, "types.h"])
    return ("#pragma once\n\n"
            f"// {name.split('::')[-1]}: one definition of a type that several sources had each written out with the same\n"
            "// text (tools/struct_migrate.py). Names and layouts are placeholders.\n" + includes + "\n" + block + "\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("name")
    ap.add_argument("--header", required=True, type=Path, help="under include/")
    ap.add_argument("--needs", nargs="*", default=[], help="headers the definition needs, relative to include/")
    ap.add_argument("--drop-comment", metavar="REGEX",
                    help="also remove a one-line comment directly above a definition when it matches this")
    args = ap.parse_args()
    header = (ROOT / args.header).resolve()
    rel_header = header.relative_to(ROOT / "include").as_posix()
    defs = struct_check.collect(decl_check.tree_files(None)).get(args.name, [])
    if not defs:
        raise SystemExit(f"struct_migrate: no definition of {args.name}")
    if len({d.text for d in defs}) != 1:
        raise SystemExit(f"struct_migrate: {args.name} has {len({d.text for d in defs})} different definitions: "
                         "settle them first (tools/struct_check.py --name)")
    if args.name.startswith("template ") or "::" in args.name:
        raise SystemExit("struct_migrate: templates and types inside a namespace are hand work")
    bare = args.name
    sources = sorted({d.path for d in defs if d.path.startswith("src/") and d.path.endswith(".cpp")})
    others = sorted({d.path for d in defs} - set(sources) - {header.relative_to(ROOT).as_posix()})
    first = (ROOT / sources[0]).read_text(encoding="utf-8").replace("\r\n", "\n")
    i, j = span(first, bare)
    block = "\n".join(first.split("\n")[i:j + 1])
    import drafts
    drafts.init(args.version)
    # every source as it compiles now, before the header changes: a source that includes the header already
    # (for another type moved there earlier) would not build with the type defined twice
    befores = {path: drafts.compiled(drafts.functions(ROOT / path)) for path in sources}
    header.parent.mkdir(parents=True, exist_ok=True)
    old_header = header.read_bytes() if header.is_file() else None
    header.write_text(header_text(header, block, args.needs, args.name), encoding="utf-8", newline="\n")
    moved, left = [], []
    raws: dict[str, bytes] = {}
    for path in sources:
        p = ROOT / path
        raw = raws[path] = p.read_bytes()
        newline = "\r\n" if b"\r\n" in raw else "\n"
        text = raw.decode("utf-8").replace("\r\n", "\n")
        new = without(text, bare, rel_header, args.drop_comment)
        before = befores[path]
        if new is None or isinstance(before, str):
            left.append((path, "its definition is not one block at the margin" if new is None else
                         f"did not build before: {before}"))
            continue
        p.write_bytes(new.replace("\n", newline).encode("utf-8"))
        after = drafts.compiled(drafts.functions(p))
        if after == before:
            moved.append(path)
        else:
            p.write_bytes(raw)
            left.append((path, after if isinstance(after, str) else "compiles to other code"))
    # a source that stays as it was and includes the header would now see the type twice: then the header
    # cannot have it, and nothing is moved
    broken = [path for path, _ in left if not isinstance(befores[path], str)
              and isinstance(drafts.functions(ROOT / path), str)]
    if broken:
        for path in moved:
            (ROOT / path).write_bytes(raws[path])
        if old_header is None:
            header.unlink()
        else:
            header.write_bytes(old_header)
        raise SystemExit(f"struct_migrate: nothing moved. {len(broken)} sources that cannot lose their definition "
                         f"include {rel_header} already and would not build with the type in it too ({broken[0]}): "
                         "settle those by hand first, or choose another header")
    for path in moved:
        print(f"migrated  {path}")
    for path, why in left:
        print(f"LEFT  {path}  {why[:150]}")
    for path in others:
        print(f"LEFT  {path}  not a .cpp under src/: its includers would all have to be compared")
    print(f"struct_migrate: {args.name} -> {header.relative_to(ROOT).as_posix()}: {len(moved)} sources migrated, "
          f"{len(left) + len(others)} left")
    return 0


if __name__ == "__main__":
    sys.exit(main())
