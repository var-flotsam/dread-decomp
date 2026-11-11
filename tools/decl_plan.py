#!/usr/bin/env python3
"""Plan the move of shared declarations into one header per translation unit.

A function or global that several hand-written files use was declared in each of them (9,309 copies
beyond the first on 2026-11-11). This finds the ones whose move is mechanical, names a header for each by
the unit the symbol belongs to (`include/game/CFoo.h` for a unit with a class name,
`include/game/UnkTU<index>.h` otherwise), and writes the plan tools/hub_migrate.py --plan takes.
hub_migrate does the edits and keeps a file only when its compiled output is unchanged. A function's
unit is the one its address is in (tools/tu_map.py); a global's is the one whose data span holds its
address (tools/data_map.py, read from data/data_map.csv).

A symbol is planned when it is declared in more than one file with one signature, at global scope, in
no header under include/ yet, and its types are builtin or plain structs the declaring file
forward-declares or defines (those are forward-declared in the header). A global's copies must also be
the same text once the macros their files define are written out (the signature leaves attributes out,
and a visibility attribute changes the code). Left
out, and counted: signatures that conflict, declarations inside a namespace, signatures with namespaced,
template or function-pointer types or an asm label, globals between two units' data spans, global arrays
of a struct type (an array needs the type's definition), a unit whose header exists and was not made by
this tool, and the symbols listed in --exclude (the ones an earlier run could not move).

With --write the headers are created (a comment, `types.h`, the forward declarations; hub_migrate
appends the declarations), a unit header that exists gets the forward declarations it lacks, and the
plan is written in chunks of whole headers, one hub_migrate run each.

hub_migrate removes a prototype together with the comment at the end of its line, and leaves a comment
that stood above it. --write therefore also lists what the copies say about each planned function
(build/decl_plans/comments.tsv: symbol, header, files, where, text): write one description beside the
declaration in the header, and remove or re-attach the comments left in the sources.

Usage:
  python tools/decl_plan.py                                   # what would be planned and what is left out
  python tools/decl_plan.py --write [--exclude FILE] [--units N] [--chunk 380]   # headers + build/decl_plans/chunk_<k>.tsv
  python tools/hub_migrate.py --version 2.1.0 --plan build/decl_plans/chunk_1.tsv
Standard library and the project tools; no game data.
"""

from __future__ import annotations

import argparse
import bisect
import re
import sys
from collections import Counter
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import data_map  # noqa: E402
import decl_check  # noqa: E402
import tu_map  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "build" / "decl_plans"
BUILTIN = frozenset("""void bool char int unsigned signed short long float double const volatile u8 u16 u32 u64
s8 s16 s32 s64 f32 f64 wchar_t char16_t char32_t extern C""".split())
TOP_LEVEL = {"base", "engine", "game"}
NAMED_SOURCES = ("reflection", "init_call", "names", "library")
IDENT = re.compile(r"[A-Za-z_]\w*")
ADDRESS = re.compile(r"71[0-9a-f]{8}")
UNIT_HEADER = "(tools/tu_map.py unit "        # in the comment of a header this tool made
FUNCTIONS_ONLY = "functions of this translation unit that other files call"
WITH_GLOBALS = "functions and globals of this translation unit that other files use"


@dataclass
class Planned:
    symbol: str
    header: str
    declaration: str             # the statement as one of the files wrote it
    copies: int                  # files that declare it today
    types: dict[str, str] = field(default_factory=dict)   # struct name -> struct | class | union
    unit: dict = field(default_factory=dict)


def unit_dir(unit: dict) -> str:
    """The folder a unit's files live in under src/ (and its header's path under include/)."""
    hints = unit["class_hint"].split(";") if unit["class_hint"] else []
    if not (hints and unit["hint_source"] in NAMED_SOURCES and unit["confidence"] == "high"):
        return f"game/UnkTU{unit['index']}"
    parts = [p for p in (re.sub(r"[^A-Za-z0-9_]+", "_", p).strip("_") for p in hints[0].split("::")) if p]
    return "/".join(parts if parts[0] in TOP_LEVEL else ["game", *parts])


def unit_finder(units: list[dict]):
    starts = [int(u["start"], 16) for u in units]

    def find(addr: int) -> dict | None:
        i = bisect.bisect_right(starts, addr) - 1
        return units[i] if i >= 0 and addr <= int(units[i]["end"], 16) else None
    return find


def spelled_out(statement: str, text: str) -> str:
    """A declaration on one line, with the object-like macros its file defines (HIDDEN) written out, so it
    means the same in a header."""
    def body(m: re.Match) -> str:
        d = re.search(rf"^[ \t]*#[ \t]*define[ \t]+{m.group(0)}[ \t]+(.+)$", text, re.M)
        return d.group(1).strip() if d else m.group(0)
    return " ".join(IDENT.sub(body, statement).split())


def plan(report: decl_check.Report, files: dict[str, str], units: list[dict],
         data_unit=None) -> tuple[list[Planned], Counter]:
    """(what can move, why the rest of the multiply-declared symbols cannot). `data_unit` gives the index of
    the unit a data address belongs to, or None (data_map.finder); without it no global is planned."""
    find = unit_finder(units)
    by_index = {str(u["index"]): u for u in units}
    out: list[Planned] = []
    why: Counter = Counter()
    for sym in report.multi:
        is_global = not sym.startswith("fn_")
        if is_global and data_unit is None:
            why["a global, not a function"] += 1
            continue
        if len(report.sigs[sym]) != 1:
            why["conflicting signatures"] += 1
            continue
        ds = report.decls[sym]
        if any(d.path.startswith("include/") for d in ds):
            why["already in a header under include/"] += 1
            continue
        if any(d.scope != ds[0].scope for d in ds) or "global" not in ds[0].scope:
            why["declared inside a namespace or struct"] += 1
            continue
        sig = report.sigs[sym][0]
        if "::" in sig or "<" in sig or "asm" in sig or "(*" in sig:
            why["namespaced, template, function-pointer or asm-label types"] += 1
            continue
        text = files[ds[0].path].replace("\r\n", "\n")
        types: dict[str, str] = {}
        for t in {t for t in IDENT.findall(sig) if t not in BUILTIN and t != sym}:
            m = re.search(rf"^\s*(struct|class|union)\s+{t}\b", text, re.M)
            if not m:
                types = None
                break
            types[t] = m.group(1)
        if types is None:
            why["a type that is not a plain struct declared in the file"] += 1
            continue
        if is_global:
            address = ADDRESS.search(sym)
            texts = {spelled_out(t[d.start:d.end], t) for d in ds for t in [files[d.path].replace("\r\n", "\n")]}
            if len(texts) != 1:
                why["a global whose copies differ in their attributes"] += 1
                continue
            if types and "[" in sig and "*" not in sig:
                why["a global array of a struct type"] += 1
                continue
            index = data_unit(int(address.group(0), 16)) if address else None
            unit = by_index.get(str(index)) if index is not None else None
            if unit is None:
                why["a global in no unit's data span"] += 1
                continue
        else:
            unit = find(int(sym[3:], 16))
            if unit is None:
                why["in no unit"] += 1
                continue
        out.append(Planned(sym, f"include/{unit_dir(unit)}.h", spelled_out(text[ds[0].start:ds[0].end], text),
                           len(report.paths[sym]), types, unit))
    out.sort(key=lambda p: (-p.copies, p.symbol))
    return out, why


def copy_comments(report: decl_check.Report, files: dict[str, str], planned: list[Planned]) -> list[tuple]:
    """What the copies of each planned declaration say about it: (symbol, header, files, where, text) per
    distinct text, `where` being `line` (the comment that ends the prototype's line) or `above` (the comment
    block over the prototype, or over the group of prototypes it stands in)."""
    out = []
    for p in planned:
        seen: Counter = Counter()
        for d in report.decls[p.symbol]:
            lines = files[d.path].replace("\r\n", "\n").split("\n")
            text = "\n".join(lines)
            rest = text[d.end:].split("\n", 1)[0].strip()
            if rest.startswith("//"):
                seen[("line", rest[2:].strip())] += 1
            above = text.count("\n", 0, d.start) - 1
            while above >= 0 and lines[above].strip() and not lines[above].lstrip().startswith("//"):
                above -= 1                        # the other prototypes of its group
            block = []
            while above >= 0 and lines[above].lstrip().startswith("//"):
                block.insert(0, lines[above].strip()[2:].strip())
                above -= 1
            if block:
                seen[("above", " ".join(block))] += 1
        out += [(p.symbol, p.header, n, where, text) for (where, text), n in seen.most_common()]
    return out


def header_text(header: str, mine: list[Planned]) -> str:
    unit = mine[0].unit
    types: dict[str, str] = {}
    for p in mine:
        types.update(p.types)
    name = header.rsplit("/", 1)[-1][:-2]
    what = FUNCTIONS_ONLY if all(p.symbol.startswith("fn_") for p in mine) else WITH_GLOBALS
    lines = ["#pragma once", "",
             f"// {name}: {what} {UNIT_HEADER}{unit['index']},",
             f"// {unit['start']}-{unit['end']}). One declaration per symbol: include this instead of declaring one.",
             "// Names and layouts are placeholders.", '#include "types.h"', ""]
    return "\n".join(lines + [f"{k} {t};" for t, k in sorted(types.items())]) + "\n"


def extended(text: str, mine: list[Planned]) -> str:
    """An existing unit header's text with the forward declarations `mine` needs and it lacks."""
    types: dict[str, str] = {}
    for p in mine:
        types.update(p.types)
    lines = text.replace("\r\n", "\n").split("\n")
    new = [f"{k} {t};" for t, k in sorted(types.items())
           if not re.search(rf"^\s*(struct|class|union)\s+{t}\b", text, re.M)]
    if any(not p.symbol.startswith("fn_") for p in mine):
        lines = [ln.replace(FUNCTIONS_ONLY, WITH_GLOBALS) for ln in lines]
    if new:
        at = lines.index('#include "types.h"') + 1
        if at < len(lines) and not lines[at].strip():
            at += 1
        else:
            new.insert(0, "")
        if at < len(lines) and lines[at].strip() and not re.match(r"(struct|class|union)\s+\w+;", lines[at]):
            new.append("")
        lines[at:at] = new
    return "\n".join(lines)


def chunks(planned: list[Planned], size: int) -> list[list[Planned]]:
    """The plan split by whole headers into runs of about `size` symbols."""
    out: list[list[Planned]] = [[]]
    for h in sorted({p.header for p in planned}):
        mine = [p for p in planned if p.header == h]
        if out[-1] and len(out[-1]) + len(mine) > size:
            out.append([])
        out[-1] += mine
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--write", action="store_true", help="create the headers and write the plan")
    ap.add_argument("--exclude", type=Path, help="a file of symbols to leave out (whitespace-separated)")
    ap.add_argument("--units", type=int, help="only the N units with the most local copies (a pilot)")
    ap.add_argument("--chunk", type=int, default=380, help="symbols per plan file (default %(default)s)")
    args = ap.parse_args()

    files = decl_check.tree_files(None)
    report = decl_check.Report(files)
    data_unit = data_map.finder(data_map.load()) if data_map.OUT.is_file() else None
    planned, why = plan(report, files, tu_map.load_map(), data_unit)
    print(f"{len(report.multi)} symbols declared in more than one file; {len(planned)} can move "
          f"({sum(p.copies for p in planned)} local copies)")
    for k, v in why.most_common():
        print(f"  {v:5}  left out: {k}")
    existing = {p.header for p in planned if (ROOT / p.header).exists()}
    foreign = sorted(h for h in existing if UNIT_HEADER not in (ROOT / h).read_text(encoding="utf-8")[:600])
    if foreign:
        print(f"  {len(foreign):5}  headers exist and are not unit headers of this tool; their "
              f"{sum(1 for p in planned if p.header in foreign)} symbols are left out")
    planned = [p for p in planned if p.header not in foreign]
    if args.exclude:
        skip = set(args.exclude.read_text(encoding="utf-8").split())
        planned = [p for p in planned if p.symbol not in skip]
        print(f"  {len(skip):5}  symbols excluded ({args.exclude})")
    if args.units:
        weight = Counter()
        for p in planned:
            weight[p.header] += p.copies
        keep = {h for h, _ in weight.most_common(args.units)}
        planned = [p for p in planned if p.header in keep]
    headers = sorted({p.header for p in planned})
    print(f"plan: {len(planned)} symbols, {sum(p.copies for p in planned)} local copies, {len(headers)} headers")
    if not args.write:
        return 0
    OUT.mkdir(parents=True, exist_ok=True)
    for old in OUT.glob("chunk_*.tsv"):
        old.unlink()
    for h in headers:
        mine = [p for p in planned if p.header == h]
        (ROOT / h).parent.mkdir(parents=True, exist_ok=True)
        text = extended((ROOT / h).read_text(encoding="utf-8"), mine) if h in existing else header_text(h, mine)
        (ROOT / h).write_bytes(text.encode("utf-8"))
    parts = chunks(planned, args.chunk)
    for k, part in enumerate(parts, 1):
        text = "".join(f"{p.symbol}\t{p.header}\t{p.declaration}\n" for p in part)
        (OUT / f"chunk_{k}.tsv").write_bytes(text.encode("utf-8"))
    print(f"{len(headers) - len(existing & set(headers))} headers created, {len(existing & set(headers))} extended; "
          f"{len(parts)} plan files in {OUT.relative_to(ROOT).as_posix()}/: "
          + ", ".join(str(len(p)) for p in parts) + " symbols")
    said = copy_comments(report, files, planned)
    (OUT / "comments.tsv").write_bytes("".join("\t".join(map(str, row)) + "\n" for row in said).encode("utf-8"))
    print(f"{len({row[0] for row in said})} of the symbols have a comment on a copy, listed in "
          f"{OUT.relative_to(ROOT).as_posix()}/comments.tsv: the migration drops or strands these, so put one in the header")
    return 0


if __name__ == "__main__":
    sys.exit(main())
