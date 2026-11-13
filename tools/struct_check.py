#!/usr/bin/env python3
"""One struct per object: the struct, class and union names the hand-written files define more than once.

tools/decl_check.py holds functions and globals to one declaration. Nothing did the same for types, and the
agent runs gave most functions a struct of their own for the object they touch: the same component under
fifteen names in twelve files, or under one name with two different bodies. This counts the second kind,
which a tool can see: a type name (with its namespace) that more than one file defines.

  same text      every file defines it with the same members: copies, which one header could hold
  other text     the definitions differ: each file's own partial view of one object, or two unrelated things
                 that happen to share a name
  with code      ... and a differing definition has a function body in it (a constructor, a destructor, a
                 method). Where the compiler does not inline such a function, the port's link has one
                 symbol with several bodies and keeps one of them. (On 2026-11-13 the x86-64 sweep's
                 objects had no such collision: everything of the kind was inlined. An unoptimised
                 build would not be so lucky.)

It reads text, not a compiler's view: comments and strings are blanked, a definition is `struct|class|union
NAME ... { ... };` at file or namespace scope, and two definitions are the same when their text is the same
once whitespace is collapsed. Types inside a function or another type are part of that body, not counted. A
template and its specialisations are told apart by their heads.

Usage:
  python tools/struct_check.py                      # the counts
  python tools/struct_check.py --list [--kind same|other|code]   # every name, most files first
  python tools/struct_check.py --name NAME          # each definition of one name, with its file and line
  python tools/struct_check.py --base origin/main   # exit 1 when a name gained a differing definition, or a
                                                    # new name has one (a ratchet; CI runs it)
  python tools/struct_check.py PATH...              # only names with a definition under these paths
"""

from __future__ import annotations

import argparse
import re
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
import decl_check  # noqa: E402

HEAD = re.compile(r"(?:template\s*<[^{};]*>\s*)?\b(struct|class|union)\s+(?:alignas\s*\([^)]*\)\s*|__attribute__\s*\(\([^)]*\)\)\s*)*"
                  r"((?:[A-Za-z_]\w*\s*::\s*)*[A-Za-z_]\w*(?:\s*<[^{};]*>)?)\s*(?:final\s*)?(?::(?!:)[^{};]*)?$")
NAMESPACE = re.compile(r"(?:inline\s+)?namespace(?:\s+([A-Za-z_][\w:]*))?\s*$")
EXTERN_C = re.compile(r'extern\s*"[^"]*"\s*$')
CODE = re.compile(r"\)\s*(?:const\s*)?(?:noexcept\s*)?(?:override\s*)?(?::[^{};]*)?\{")


@dataclass(frozen=True)
class Definition:
    name: str     # with its namespace path
    path: str
    line: int
    text: str     # head and body, whitespace collapsed
    code: bool    # a function body inside


def closing(text: str, start: int) -> int:
    """The index of the brace that closes the one at `start`."""
    depth = 0
    for i in range(start, len(text)):
        c = text[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
            if depth == 0:
                return i
    return len(text) - 1


def definitions(text: str, path: str) -> list[Definition]:
    """Every struct, class or union defined at file or namespace scope of this text."""
    code = decl_check.strip(text.replace("\r\n", "\n"))
    code = decl_check.PREPROCESSOR.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), code)
    out: list[Definition] = []
    scopes: list[tuple[int, str]] = []  # (index of the closing brace, namespace name or "")
    i = last = 0
    while i < len(code):
        c = code[i]
        if c in ";}":
            if c == "}" and scopes and scopes[-1][0] == i:
                scopes.pop()
            last = i + 1
        elif c == "{":
            head = " ".join(code[last:i].split())
            end = closing(code, i)
            ns = NAMESPACE.match(head)
            m = HEAD.match(head)
            if ns or EXTERN_C.match(head):
                scopes.append((end, ns.group(1) or "" if ns else ""))
                last = i + 1
            else:
                if m and "(" not in head.split(m.group(1), 1)[0]:
                    qual = "::".join([s for _, s in scopes if s] + [" ".join(m.group(2).split())])
                    body = " ".join(code[i:end + 1].split())
                    tmpl = "template " if head.startswith("template") else ""
                    out.append(Definition(tmpl + qual, path, code.count("\n", 0, last + len(code[last:i]) - len(code[last:i].lstrip())) + 1,
                                          head + " " + body, bool(CODE.search(body))))
                i = end
                last = i + 1
        i += 1
    return out


def collect(files: dict[str, str]) -> dict[str, list[Definition]]:
    by: dict[str, list[Definition]] = defaultdict(list)
    for path, text in files.items():
        if "{" in text:
            for d in definitions(text, path):
                by[d.name].append(d)
    return by


def kind(defs: list[Definition]) -> str | None:
    """None for a name one file defines; else `same`, `other`, or `code` (other text, with a function body)."""
    if len({d.path for d in defs}) < 2:
        return None
    texts = {d.text for d in defs}
    if len(texts) == 1:
        return "same"
    return "code" if any(d.code for d in defs) else "other"


def summary(by: dict[str, list[Definition]]) -> dict[str, int]:
    out = {"names": len(by), "several": 0, "copies": 0, "same": 0, "other": 0, "code": 0}
    for defs in by.values():
        k = kind(defs)
        if k:
            out["several"] += 1
            out["copies"] += len({d.path for d in defs}) - 1
            out[k] += 1
    return out


def worse(now: dict[str, list[Definition]], before: dict[str, list[Definition]]) -> list[str]:
    """Names that have more differing definitions now than before (a new name with two bodies included)."""
    out = []
    for name, defs in now.items():
        if kind(defs) in ("other", "code"):
            old = before.get(name, [])
            a, b = len({d.text for d in defs}), len({d.text for d in old}) if kind(old) in ("other", "code") else 1
            if a > b:
                out.append(f"{name}: {b} -> {a} different definitions ({', '.join(sorted({d.path for d in defs})[:4])})")
    return sorted(out)


def line_of(s: dict[str, int]) -> str:
    return (f"{s['names']} type names defined; {s['several']} in more than one file ({s['copies']} definitions beyond "
            f"the first): {s['same']} with the same text everywhere, {s['other'] + s['code']} with different texts, "
            f"{s['code']} of those with a function body in one")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("paths", nargs="*", type=Path)
    ap.add_argument("--list", action="store_true")
    ap.add_argument("--kind", choices=("same", "other", "code"))
    ap.add_argument("--name")
    ap.add_argument("--base", metavar="REF")
    args = ap.parse_args()
    by = collect(decl_check.tree_files(None))
    if args.paths:
        under = tuple(p.resolve().relative_to(ROOT).as_posix() for p in args.paths)
        by = {n: ds for n, ds in by.items() if any(d.path.startswith(under) for d in ds)}
    if args.name:
        for d in sorted(by.get(args.name, []), key=lambda d: (d.text, d.path)):
            print(f"{d.path}:{d.line}  {d.text[:150]}")
        print(f"{args.name}: {len(by.get(args.name, []))} definitions, {len({d.text for d in by.get(args.name, [])})} different")
        return 0
    if args.list:
        rows = [(len({d.path for d in ds}), len({d.text for d in ds}), kind(ds), n) for n, ds in by.items() if kind(ds)]
        for files, texts, k, n in sorted(rows, key=lambda r: (-r[0], r[3])):
            if args.kind in (None, k):
                print(f"{files:4} files  {texts:3} texts  {k:5}  {n}")
    print("struct_check: " + line_of(summary(by)))
    if args.base:
        before = collect(decl_check.tree_files(args.base))
        print(f"struct_check: {args.base}: " + line_of(summary(before)))
        found = worse(by, before)
        for f in found:
            print("WORSE  " + f)
        print(("FAIL" if found else "PASS") + f" struct_check: {len(found)} names with more different definitions than at {args.base}")
        return 1 if found else 0
    return 0


if __name__ == "__main__":
    sys.exit(main())
