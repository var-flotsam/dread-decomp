#!/usr/bin/env python3
"""Plan the move of shared function declarations into one header per translation unit.

A function that several hand-written files call was declared in each of them (9,309 copies beyond the
first on 2026-11-11). This finds the ones whose move is mechanical, names a header for each by the unit
the function belongs to (tools/tu_map.py: `include/game/CFoo.h` for a unit with a class name,
`include/game/UnkTU<index>.h` otherwise), and writes the plan tools/hub_migrate.py --plan takes.
hub_migrate does the edits and keeps a file only when its compiled output is unchanged.

A function is planned when it is declared in more than one file with one signature, at global scope,
in no header under include/ yet, and its parameter types are builtin or plain structs the declaring
file forward-declares or defines (those are forward-declared in the header). Left out, and counted:
globals (they have no unit), signatures that conflict, declarations inside a namespace, signatures
with namespaced, template or function-pointer types or an asm label, a unit whose header exists
already, and the symbols listed in --exclude (the ones an earlier run could not move).

With --write the headers are created (a comment, `types.h`, the forward declarations; hub_migrate
appends the declarations) and the plan is written in chunks of whole headers, one hub_migrate run each.
Run it on a clean tree: a header it created counts as existing the next time.

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
import decl_check  # noqa: E402
import tu_map  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "build" / "decl_plans"
BUILTIN = frozenset("""void bool char int unsigned signed short long float double const volatile u8 u16 u32 u64
s8 s16 s32 s64 f32 f64 extern C""".split())
TOP_LEVEL = {"base", "engine", "game"}
NAMED_SOURCES = ("reflection", "init_call", "names", "library")
IDENT = re.compile(r"[A-Za-z_]\w*")


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


def plan(report: decl_check.Report, files: dict[str, str], units: list[dict]) -> tuple[list[Planned], Counter]:
    """(what can move, why the rest of the multiply-declared symbols cannot)."""
    find = unit_finder(units)
    out: list[Planned] = []
    why: Counter = Counter()
    for sym in report.multi:
        if not sym.startswith("fn_"):
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
        unit = find(int(sym[3:], 16))
        if unit is None:
            why["in no unit"] += 1
            continue
        out.append(Planned(sym, f"include/{unit_dir(unit)}.h", " ".join(text[ds[0].start:ds[0].end].split()),
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
    lines = ["#pragma once", "",
             f"// {name}: functions of this translation unit that other files call (tools/tu_map.py unit {unit['index']},",
             f"// {unit['start']}-{unit['end']}). One declaration per function: include this instead of declaring one.",
             "// Names and layouts are placeholders.", '#include "types.h"', ""]
    return "\n".join(lines + [f"{k} {t};" for t, k in sorted(types.items())]) + "\n"


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
    planned, why = plan(report, files, tu_map.load_map())
    print(f"{len(report.multi)} symbols declared in more than one file; {len(planned)} can move "
          f"({sum(p.copies for p in planned)} local copies)")
    for k, v in why.most_common():
        print(f"  {v:5}  left out: {k}")
    existing = sorted({p.header for p in planned if (ROOT / p.header).exists()})
    if existing:
        print(f"  {len(existing):5}  headers exist already; their {sum(1 for p in planned if p.header in existing)} "
              "symbols are left out")
    planned = [p for p in planned if p.header not in existing]
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
        (ROOT / h).parent.mkdir(parents=True, exist_ok=True)
        (ROOT / h).write_bytes(header_text(h, [p for p in planned if p.header == h]).encode("utf-8"))
    parts = chunks(planned, args.chunk)
    for k, part in enumerate(parts, 1):
        text = "".join(f"{p.symbol}\t{p.header}\t{p.declaration}\n" for p in part)
        (OUT / f"chunk_{k}.tsv").write_bytes(text.encode("utf-8"))
    print(f"{len(headers)} headers created; {len(parts)} plan files in {OUT.relative_to(ROOT).as_posix()}/: "
          + ", ".join(str(len(p)) for p in parts) + " symbols")
    said = copy_comments(report, files, planned)
    (OUT / "comments.tsv").write_bytes("".join("\t".join(map(str, row)) + "\n" for row in said).encode("utf-8"))
    print(f"{len({row[0] for row in said})} of the symbols have a comment on a copy, listed in "
          f"{OUT.relative_to(ROOT).as_posix()}/comments.tsv: the migration drops or strands these, so put one in the header")
    return 0


if __name__ == "__main__":
    sys.exit(main())
