#!/usr/bin/env python3
"""Move hand-written sources out of the address-named directories into directories by unit.

`src/game_<addr>/` and `src/game_s<N>/` hold hand-written code by address slice, a layout frozen
since 2026-10-24 (CLAUDE.md, "Files by unit"). This moves each `fn_<addr>.cpp` whose translation
unit `data/tu_map.csv` knows with high confidence and a name to a directory named for that unit:
the unit's first class hint, split on `::`, under `src/` when it starts with `base`, `engine` or
`game` and under `src/game/` otherwise (`CPlaythrough` -> `src/game/CPlaythrough/`,
`navmesh::SNavMeshPath` -> `src/game/navmesh/SNavMeshPath/`). Files whose unit is unknown,
low or medium confidence, or unnamed stay where they are. File names do not change.

The headers in those directories (shared local layouts, several of them re-declaring symbols
`include/` declares) stay where they are until they are migrated to `include/` by hand
(`tools/decl_check.py` treats only `include/` as canonical); every `#include` in a moved file that
reached a neighbour by a relative path is rewritten for the new location. Text references to a
moved file's old path elsewhere (comments, docs but the journal) are updated too. Nothing is compiled here: build before and after with
`tools/check.py --compile-only` and compare with `tools/objcompare.py`.

Usage: python tools/unit_move.py            # the plan: counts and a sample of moves
       python tools/unit_move.py --apply    # move with `git mv` and rewrite the includes
"""

from __future__ import annotations

import argparse
import os
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import tu_map  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
SRC = ROOT / "src"
ADDRESS_DIRS = re.compile(r"^game_(?:s\d+|lo|hi|[0-9a-f]{6,10})$")
FILE = re.compile(r"^fn_([0-9a-f]{10})\.cpp$")
INCLUDE = re.compile(r'^(\s*#\s*include\s*")([^"]+)(")', re.M)
TOP_LEVEL = {"base", "engine", "game"}
NAMED_SOURCES = ("reflection", "init_call", "names", "library")
# Files that stay put whatever their unit says, with the reason.
KEEP = {
    # Keeps a u32-returning local declaration of fn_710009d50c on purpose (load-bearing for its m);
    # tools/decl_check.py --base would count it again under a new path.
    "src/game_s2/fn_71003f1b00.cpp": "deliberate second declaration",
}


def address_dirs() -> list[Path]:
    return sorted(d for d in SRC.iterdir() if d.is_dir() and ADDRESS_DIRS.match(d.name))


def unit_dir(hint: str) -> Path:
    parts = [re.sub(r"[^A-Za-z0-9_]+", "_", p).strip("_") for p in hint.split("::")]
    parts = [p for p in parts if p]
    return SRC.joinpath(*(parts if parts[0] in TOP_LEVEL else ["game", *parts]))


def plan() -> tuple[dict[Path, Path], Counter]:
    rows = tu_map.load_map()
    starts = tu_map.unit_starts(rows)
    sources: dict[Path, Path] = {}
    why: Counter = Counter()
    for d in address_dirs():
        for cpp in sorted(d.rglob("*.cpp")):
            m = FILE.match(cpp.name)
            u = tu_map.find_unit(rows, int(m.group(1), 16), starts) if m else None
            hints = u["class_hint"].split(";") if u and u["class_hint"] else []
            rel = cpp.relative_to(ROOT).as_posix()
            if rel in KEEP:
                why[f"stays: {KEEP[rel]}"] += 1
            elif not m:
                why["stays: not fn_<addr>.cpp"] += 1
            elif u is None:
                why["stays: no unit"] += 1
            elif u["confidence"] != "high":
                why[f"stays: {u['confidence']} confidence"] += 1
            elif not hints or u["hint_source"] not in NAMED_SOURCES:
                why["stays: unit has no name"] += 1
            else:
                sources[cpp] = unit_dir(hints[0]) / cpp.name
                why["moves"] += 1
    clash = [q for q in sources.values() if q.exists()]
    if clash:
        raise SystemExit("already exists: " + ", ".join(q.relative_to(ROOT).as_posix() for q in clash))
    return sources, why


def rewrite_includes(text: str, old: Path, new: Path, moved: dict[Path, Path]) -> str:
    """Re-point quoted includes of a file that lived at `old` and now lives at `new`."""
    def fix(m: re.Match) -> str:
        target = (old.parent / m.group(2)).resolve()
        if not target.is_file():
            return m.group(0)  # found through -I include/ (or a system path): unchanged
        dest = moved.get(target, target)
        if dest == target and new.parent == old.parent:
            return m.group(0)
        return m.group(1) + Path(os.path.relpath(dest, new.parent)).as_posix() + m.group(3)
    return INCLUDE.sub(fix, text)


def apply(sources: dict[Path, Path]) -> None:
    moved = {p.resolve(): q for p, q in sources.items()}
    # Every file that can include a moved one: all of src/ and include/.
    files = [p for p in sorted(SRC.rglob("*")) + sorted((ROOT / "include").rglob("*"))
             if p.suffix in (".cpp", ".h", ".c", ".inc") and p.is_file()]
    new_text: dict[Path, str] = {}
    for p in files:
        dest = moved.get(p.resolve(), p)
        text = p.read_text(encoding="utf-8")
        t = rewrite_includes(text, p, dest, moved)
        if t != text or dest != p:
            new_text[p] = t
    for p, q in sources.items():
        q.parent.mkdir(parents=True, exist_ok=True)
        subprocess.run(["git", "mv", str(p.relative_to(ROOT)), str(q.relative_to(ROOT))], cwd=ROOT, check=True)
    for p, t in new_text.items():
        dest = moved.get(p.resolve(), p)
        dest.write_text(t, encoding="utf-8", newline="\n")
    # Old paths named in text elsewhere (comments, docs; the journal is history).
    renames = {p.relative_to(ROOT).as_posix(): q.relative_to(ROOT).as_posix() for p, q in sources.items()}
    pattern = re.compile("|".join(re.escape(a) for a in sorted(renames, key=len, reverse=True)))
    tracked = subprocess.run(["git", "ls-files"], cwd=ROOT, capture_output=True, text=True, check=True).stdout.split()
    for rel in tracked:
        f = ROOT / rel
        if rel == "docs/JOURNAL.md" or f.suffix not in (".md", ".py", ".h", ".cpp", ".txt") or not f.is_file():
            continue
        text = f.read_text(encoding="utf-8")
        t = pattern.sub(lambda m: renames[m.group(0)], text)
        if t != text:
            f.write_text(t, encoding="utf-8", newline="\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--apply", action="store_true", help="move the files and rewrite the includes")
    args = ap.parse_args()
    sources, why = plan()
    dirs = Counter(q.parent for q in sources.values())
    for k, v in sorted(why.items()):
        print(f"{v:5}  {k}")
    print(f"{len(sources)} sources into {len(dirs)} unit directories")
    for p, q in list(sources.items())[:5]:
        print(f"  {p.relative_to(ROOT).as_posix()} -> {q.relative_to(ROOT).as_posix()}")
    if args.apply:
        apply(sources)
        left = [d for d in address_dirs() if any(d.rglob("*"))]
        print(f"moved; address directories still holding files: {len(left)}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
