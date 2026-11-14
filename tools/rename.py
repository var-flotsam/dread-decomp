#!/usr/bin/env python3
"""Give a function a readable name: the name map (data/names.csv), the function list and every hand-written use.

A name the binary does not carry is chosen by reading what the function does (name_source `inferred`, owner,
2026-11-14) and recorded with its reason in data/names.csv (tools/dread/names.py). This tool is the only way it
gets there: it writes the map's row, applies it to data/functions/ the way tools/gen_functions_csv.py does, and
replaces the old name in the code of every hand-written file (src/ and include/ outside the generated
directories, drafts/, port/). Comments and string literals are left alone: a NON_MATCHING note names a callee by
its address, and keeps doing so. Generated sources keep the placeholder (they are regenerated, and
tools/check.py and tools/port_link.py resolve both names to the one address).

A rename keeps the function where it is: the qualifier of the new name must be the scope the definition is
written in (namespaces, class, and a `Class::` on the definition's line). Making a free function a member, or
moving it to another class, is a change of signature: do it in the source first, with the class's layout batch.
Renaming a name the map already gave (a better reading, or a real name found later) works the same way.

Gates (CLAUDE.md, "Conventions"): a rename changes no code. After one, `python tools/check.py --version <ver>
--update` reports no REGRESSION (one is reported for an `O` function no source defines any more: a name check.py
cannot resolve) and `git diff data/functions/` shows only the renamed rows' name and name_source.
data/equivalence.csv's verdicts stay current (tools/equiv_test.py spells a mapped name as its placeholder in
its digest). `--check` (CI's policy job) verifies the map, that the function list carries it, and that no
hand-written code still uses a mapped function's old placeholder.

Usage:
  python tools/rename.py 0x7100219588 UnkRenderManager::ProjectToScreen --reason "projects a point through the view"
  python tools/rename.py 0x7100219588 ... --dry-run    # what would change, nothing written
  python tools/rename.py --check                       # the map is valid, applied and complete; exit 1 if not
  python tools/rename.py --list                        # the map
Standard library only, no game data.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decl_check  # noqa: E402
from dread import functions  # noqa: E402
from dread import names  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GENERATED_FILES = ("include/base/reflection/UnkClassGetters.h",)
TOOL_DIRS = ("tools/",)


def hand_written(path: str) -> bool:
    """A file whose code people write: the sources and headers outside the generators' outputs, drafts and the
    port's own code."""
    if not path.endswith(decl_check.SUFFIXES) or path.startswith(decl_check.GENERATED) or path in GENERATED_FILES:
        return False
    return path.startswith(("src/", "include/", "drafts/", "port/"))


def tracked_files() -> list[str]:
    out = subprocess.run(["git", "ls-files", "--cached", "--others", "--exclude-standard"], cwd=ROOT,
                         capture_output=True, text=True, check=True).stdout
    return sorted(p for p in set(out.splitlines()) if hand_written(p) and (ROOT / p).is_file())


def word(ident: str) -> re.Pattern:
    return re.compile(rf"(?<![\w$]){re.escape(ident)}(?![\w$])")


def code_spans(text: str):
    """(is_code, piece) for the text in order: comments and string or character literals are not code."""
    pos = 0
    for m in decl_check.LEXEMES.finditer(text):
        if m.start() > pos:
            yield True, text[pos:m.start()]
        yield False, m.group(0)
        pos = m.end()
    if pos < len(text):
        yield True, text[pos:]


def replace_in_code(text: str, old: str, new: str) -> tuple[str, int, int]:
    """(text with `old` replaced in its code, replacements, uses inside string literals)."""
    pat = word(old)
    out, count, literal = [], 0, 0
    for is_code, piece in code_spans(text):
        if is_code:
            piece, n = pat.subn(new, piece)
            count += n
        elif piece[0] in "\"'":
            literal += len(pat.findall(piece))
        out.append(piece)
    return "".join(out), count, literal


def uses_in_code(text: str, ident: str) -> int:
    pat = word(ident)
    return sum(len(pat.findall(p)) for is_code, p in code_spans(text) if is_code)


def definition_scopes(text: str, ident: str) -> list[str]:
    """The scopes (`ns::Class`, "" at global scope) of every definition of `ident` in the file: a line starting
    at column 0 with the name, optionally qualified, its parameter list, and then a body."""
    stripped = decl_check.strip(text)
    out = []
    head = re.compile(rf"^(?:[A-Za-z_][\w:<>,*& \t]*?[\s*&])?((?:[A-Za-z_]\w*::)*){re.escape(ident)}\s*\(", re.M)
    for m in head.finditer(stripped):
        depth, i = 1, m.end()
        while i < len(stripped) and depth:
            depth += {"(": 1, ")": -1}.get(stripped[i], 0)
            i += 1
        tail = re.match(r"[^;{}()]*", stripped[i:]).group(0)
        if i + len(tail) >= len(stripped) or stripped[i + len(tail)] != "{":
            continue
        written = m.group(1)[:-2] if m.group(1) else ""
        enclosing = [n for n in enclosing_scopes(stripped, m.start()) if n]
        out.append("::".join(enclosing + ([written] if written else [])))
    return out


def enclosing_scopes(stripped: str, offset: int) -> list[str]:
    """The namespace and class names open at `offset` of comment-stripped text (decl_check.classify)."""
    stack: list[tuple[str, str]] = []
    start = 0
    for m in re.finditer(r"[{};]", stripped[:offset]):
        c = m.group(0)
        if c == "{":
            stack.append(decl_check.classify(stripped[start:m.start()]))
        elif c == "}" and stack:
            stack.pop()
        start = m.end()
    return [n for k, n in stack if k in ("ns", "struct") and n != "(anonymous)"]


def rename(address: int, new: str, reason: str, dry_run: bool) -> int:
    addr = f"{address:#x}"
    rows = functions.by_file()
    every = [r for frows in rows.values() for r in frows]
    row = next((r for r in every if r["address"] == addr), None)
    if row is None:
        return fail(f"{addr} is not a function of data/functions/")
    if row["status"] == "L":
        return fail(f"{addr} is library code (status L): it keeps the library's name")
    if row["name_source"] in names.GENERATED_SOURCES:
        return fail(f"{addr} is named by the binary: {row['name']} ({row['name_source']})")
    why = names.name_problem(new)
    if why:
        return fail(f"{new}: {why}")
    taken = [r["address"] for r in every if r["name"] == new and r["address"] != addr]
    if taken:
        return fail(f"{new} already names {', '.join(taken)}: tools/check.py resolves a name only when it is unique")
    if not reason.strip():
        return fail("a name needs its reason (--reason)")

    old = row["name"]
    old_qual, old_ident = names.split(old)
    new_qual, new_ident = names.split(new)
    placeholder = names.placeholder(address)
    if old_ident != placeholder:
        others = [r["name"] for r in every if r["address"] != addr and names.split(r["name"])[1] == old_ident]
        if others:
            return fail(f"{old_ident} is also the name of {', '.join(others[:3])}: rewriting it would touch those "
                        "too. Rename this one by hand, then record it with --record-only")

    texts = {p: (ROOT / p).read_text(encoding="utf-8") for p in tracked_files()}
    scopes = sorted({s for p, t in texts.items() if word(old_ident).search(t) for s in definition_scopes(t, old_ident)})
    if old_ident != placeholder and old_qual not in scopes and scopes:
        scopes.append(old_qual)
    if len(scopes) > 1:
        return fail(f"{old_ident} is defined in more than one scope ({', '.join(s or '(global)' for s in scopes)})")
    if scopes and scopes[0] != new_qual:
        return fail(f"the definition is in scope {scopes[0] or '(global)'}, the new name in {new_qual or '(global)'}: "
                    "a rename keeps the scope (change the signature in the source first)")
    if not scopes:
        print(f"note: no hand-written definition of {old_ident}; the scope {new_qual or '(global)'} is taken as given")

    changed: dict[str, str] = {}
    literals = []
    total = 0
    for p, t in texts.items():
        out, n, lit = replace_in_code(t, old_ident, new_ident)
        if lit:
            literals.append(p)
        if n:
            changed[p] = out
            total += n
            print(f"{p}: {n}")
    if literals:
        return fail(f"{old_ident} is spelled in a string literal (an asm label?) in {', '.join(literals[:5])}: "
                    "rename by hand")
    tools = [p for p in subprocess.run(["git", "ls-files", *TOOL_DIRS], cwd=ROOT, capture_output=True, text=True,
                                       check=True).stdout.splitlines()
             if p.endswith(".py") and word(old_ident).search((ROOT / p).read_text(encoding="utf-8"))]
    for p in tools:
        print(f"warning: {p} mentions {old_ident}; if it looks the function up by that name, make it use the address")

    mapped = [r for r in names.read() if r["address"] != addr]
    mapped.append({"address": addr, "name": new, "name_source": "inferred", "reason": " ".join(reason.split())})
    print(f"{addr}: {old} -> {new}, {total} uses in {len(changed)} files")
    if dry_run:
        print("dry run: nothing written")
        return 0
    for p, t in changed.items():
        (ROOT / p).write_text(t, encoding="utf-8", newline="")
    names.write(mapped)
    for fname, frows in rows.items():
        if any(r["address"] == addr for r in frows):
            names.apply(frows, [m for m in mapped if m["address"] == addr])
            functions.write(fname, frows)
    print("next: python tools/check.py --version <ver> --update (no REGRESSION, and git diff data/functions/ shows "
          "only this row), python tools/decl_check.py --strict --base origin/main, python tools/rename.py --check")
    return 0


def check() -> list[str]:
    """Every problem with the map: its rows, the function list carrying them, and code still using placeholders."""
    mapped = names.read()
    problems = names.row_problems(mapped)
    every = functions.rows()
    by_addr = {r["address"]: r for r in every}
    mapped_addr = {r["address"] for r in mapped}
    counts: dict[str, int] = {}
    for r in every:
        counts[r["name"]] = counts.get(r["name"], 0) + 1
    for r in mapped:
        f = by_addr.get(r["address"])
        if f is None:
            problems.append(f"{r['address']} ({r['name']}): not a function of data/functions/")
        elif (f["name"], f["name_source"]) != (r["name"], r["name_source"]):
            problems.append(f"{r['address']}: data/functions/ has {f['name']} ({f['name_source']}), the map "
                            f"{r['name']} ({r['name_source']}); rename with tools/rename.py, never by hand")
        elif counts.get(r["name"], 0) > 1:
            problems.append(f"{r['address']} ({r['name']}): the name is not unique in data/functions/")
    for r in every:
        if r["name_source"] in names.SOURCES and r["address"] not in mapped_addr:
            problems.append(f"{r['address']} ({r['name']}): name_source {r['name_source']} without a row in "
                            "data/names.csv")
    if mapped:
        stale = {names.placeholder(int(r["address"], 16)): r for r in mapped}
        pat = re.compile(r"(?<![\w$])fn_[0-9a-f]{10}(?![\w$])")
        for p in tracked_files():
            t = (ROOT / p).read_text(encoding="utf-8")
            for is_code, piece in code_spans(t):
                if not is_code:
                    continue
                for ph in sorted({m.group(0) for m in pat.finditer(piece)} & stale.keys()):
                    problems.append(f"{p}: still uses {ph}, which the map names {stale[ph]['name']} "
                                    "(tools/rename.py rewrites every use)")
    return sorted(set(problems))


def fail(msg: str) -> int:
    print(f"rename: {msg}", file=sys.stderr)
    return 1


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("address", nargs="?", help="the function's address (0x7100219588)")
    ap.add_argument("name", nargs="?", help="the new qualified name, without parameters")
    ap.add_argument("--reason", default="", help="one line: what the name is based on (required)")
    ap.add_argument("--dry-run", action="store_true", help="print what would change; write nothing")
    ap.add_argument("--record-only", action="store_true",
                    help="write the map and function list only (the code was renamed by hand)")
    ap.add_argument("--check", action="store_true", help="verify the map, its application and its completeness")
    ap.add_argument("--list", action="store_true", help="print the map")
    args = ap.parse_args()
    if args.check:
        problems = check()
        for p in problems:
            print(f"NAMES  {p}")
        print(f"rename --check: {len(names.read())} names in data/names.csv, {len(problems)} problems")
        return 1 if problems else 0
    if args.list:
        for r in names.read():
            print(f"{r['address']}  {r['name']}  ({r['name_source']}: {r['reason']})")
        return 0
    if not (args.address and args.name):
        ap.error("an address and a name, or --check, or --list")
    try:
        address = int(args.address, 16)
    except ValueError:
        ap.error(f"{args.address}: not a hex address")
    if args.record_only:
        return record_only(address, args.name, args.reason, args.dry_run)
    return rename(address, args.name, args.reason, args.dry_run)


def record_only(address: int, new: str, reason: str, dry_run: bool) -> int:
    """The map's row and the function list's, for a rename whose code was changed by hand."""
    addr = f"{address:#x}"
    why = names.name_problem(new)
    if why:
        return fail(f"{new}: {why}")
    if not reason.strip():
        return fail("a name needs its reason (--reason)")
    rows = functions.by_file()
    every = [r for frows in rows.values() for r in frows]
    row = next((r for r in every if r["address"] == addr), None)
    if row is None or row["status"] == "L" or row["name_source"] in names.GENERATED_SOURCES:
        return fail(f"{addr}: not a function the map can name")
    if any(r["name"] == new and r["address"] != addr for r in every):
        return fail(f"{new} is taken")
    mapped = [r for r in names.read() if r["address"] != addr]
    mapped.append({"address": addr, "name": new, "name_source": "inferred", "reason": " ".join(reason.split())})
    print(f"{addr}: {row['name']} -> {new} (recorded only)")
    if dry_run:
        return 0
    names.write(mapped)
    for fname, frows in rows.items():
        if any(r["address"] == addr for r in frows):
            names.apply(frows, [m for m in mapped if m["address"] == addr])
            functions.write(fname, frows)
    return 0


if __name__ == "__main__":
    sys.exit(main())
