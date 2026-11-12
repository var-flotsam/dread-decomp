#!/usr/bin/env python3
"""drafts/: file a function that does not match yet, regrade what is there, move a draft into src/.

`drafts/` holds hand-written functions that neither match nor are verified equivalent (drafts/README.md);
`drafts/INDEX.csv` has a row for each. Nothing there is built or counted. This is the way in and the way out:

  add      file a source as the draft of one function: it is copied to drafts/<unit folder>/fn_<addr>.cpp
           (the folder its function would have under src/, tools/tu_map.py), graded, and given a row.
           Where a non-match of an agent run goes, and a file of src/ that has to come back (--remove).
  grade    compile the drafts again and say what separates each from the original now. A row is a record
           of the day it was graded: after a header changed, or after tools/permute.py --write, this is how
           it is brought up to date (--write).
  promote  move a draft to src/ when it has earned it (CLAUDE.md, "Completion"): every function its file
           defines matches, or differs in registers or order only and passes tools/equiv_test.py. The copy
           gets its includes rewritten for the new folder and `// NON_MATCHING(minor)` above a function
           that does not match, and is graded again where it now is. A declaration of a symbol that a header
           under include/ declares is replaced by that header where the file then compiles to the same code.
           Then its declarations are checked against the tree (tools/decl_check.py's file_problems): a draft
           that declares a shared symbol its own way stays a draft, with the reason in its row's note. Without
           --write nothing is kept.

After `promote --write`: tools/equiv_test.py --write on the moved files that do not match (the verdict is what
makes them count), then tools/decl_plan.py and tools/hub_migrate.py for declarations they copied, then
tools/check.py --update as after any change of src/. A file one of those rejects goes back with `add --remove`.

Usage:
  python tools/drafts.py --version 2.1.0 add FILE.cpp ADDR --from TAG [--note TEXT] [--replace] [--remove]
  python tools/drafts.py --version 2.1.0 grade [FILE.cpp ...] [--from TAG] [--write] [--jobs N]
  python tools/drafts.py --version 2.1.0 promote FILE.cpp [...] [--write]
Needs the dump; promote needs what tools/equiv_test.py needs for a draft that does not match.
"""

from __future__ import annotations

import argparse
import csv
import json
import os
import re
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
INDEX = ROOT / "drafts" / "INDEX.csv"
OUT = ROOT / "build" / "drafts"
FIELDS = ["address", "size", "file", "difference", "similarity", "differing_instructions", "from", "note"]
KINDS = {"registers": "registers or order", "operations": "operations", "conditions": "conditions",
         "control": "control flow", "calls": "calls"}
REGISTERS = KINDS["registers"]
MINOR_NOTE = ("// NON_MATCHING(minor): the same instructions in another order or with other registers; passes "
              "tools/equiv_test.py.")
INCLUDE = re.compile(r'^(\s*#\s*include\s+")([^"]+)(")', re.M)
_state: dict = {}


# ---- text ---------------------------------------------------------------------------------------------

def rehome(text: str, old_dir: Path, new_dir: Path) -> str:
    """The text with every include that is relative to its file's folder (it names a file there) written for
    the file's new folder. Includes found through include/ are left as they are."""
    def fix(m: re.Match) -> str:
        target = old_dir / m.group(2)
        if not target.is_file():
            return m.group(0)
        return m.group(1) + Path(os.path.relpath(target.resolve(), new_dir.resolve())).as_posix() + m.group(3)
    return INCLUDE.sub(fix, text)


def definition_line(lines: list[str], name: str) -> int | None:
    """The line a function's definition starts on (not a declaration, not a call): `name(` on a line that
    starts at the margin and has no `;` before a `{` follows."""
    for i, line in enumerate(lines):
        if re.match(rf"^[^\s/#][^;]*\b{re.escape(name)}\s*\(", line):
            for later in lines[i:i + 8]:
                if ";" in later.split("{")[0] and "{" not in later:
                    break
                if "{" in later:
                    return i
    return None


def annotate(text: str, names: list[str]) -> tuple[str, list[str]]:
    """The text with the tier-2 note above the definition of each named function that has no NON_MATCHING
    note yet; and the names whose definition was not found."""
    lines = text.split("\n")
    missing = []
    for name in names:
        i = definition_line(lines, name)
        if i is None:
            missing.append(name)
            continue
        k = i
        while k > 0 and lines[k - 1].lstrip().startswith("//"):
            k -= 1
        if not any("NON_MATCHING(" in line for line in lines[k:i]):
            lines.insert(i, MINOR_NOTE)
    return "\n".join(lines), missing


def read_index() -> list[dict]:
    with INDEX.open(newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def write_index(rows: list[dict]) -> None:
    rows = sorted(rows, key=lambda r: int(r["address"], 16))
    with INDEX.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, FIELDS, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)


def short(text: str, n: int = 150) -> str:
    text = " ".join(text.split())
    return text if len(text) <= n else text[:n - 3] + "..."


# ---- grading ------------------------------------------------------------------------------------------

def init(version: str) -> None:
    if "orig" in _state:
        return
    sys.path.insert(0, str(ROOT / "tools"))
    import asmdiff
    import buildcfg
    import check
    _state.update(asmdiff=asmdiff, buildcfg=buildcfg, check=check, orig=asmdiff.Original(version))
    rows, _ = check.load_csv()
    _state["by_name"] = check.csv_by_name(rows)
    _state["sizes"] = {int(r["address"], 16): r["size"] for r in rows.values()}


def functions(src: Path) -> list[dict] | str:
    """Every function the file defines, as it compiles now: [{address, name, difference, similarity,
    differing}], difference being `match`, one of KINDS' values or `wrong reference` (the bytes match and a
    relocation names another target than the original's). A string, the first error line, when it does not
    build."""
    asmdiff, check = _state["asmdiff"], _state["check"]
    obj = OUT / "obj" / (re.sub(r"\W+", "_", src.resolve().relative_to(ROOT).as_posix()) + ".o")
    obj.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(_state["buildcfg"].cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        errors = [line for line in r.stderr.splitlines() if "error" in line]
        return short((errors or r.stderr.splitlines() or ["no output"])[0])
    funcs = asmdiff.read_object(obj)
    codes = {f.symbol: f.code for f in funcs}
    results, names, _, targets = check.diff_with_targets(funcs, _state["orig"], _state["by_name"])
    out = []
    for res in results:
        if res.bytes_match:
            bad = asmdiff.check_relocs(res, _state["orig"], targets).mismatches
            difference = "wrong reference" if bad else "match"
        else:
            kind = asmdiff.classify(_state["orig"].function(res.address), codes[res.symbol]).split(" ")[0]
            difference = KINDS.get(kind, kind)
        out.append({"address": res.address, "name": names.get(res.symbol, res.symbol).split("::")[-1],
                    "difference": difference, "similarity": f"{res.score * 100:.1f}",
                    "differing": str(len(res.diff_insns))})
    return out


def grade(src: Path, address: int) -> dict:
    """The three graded columns of a row for the function at address, as src compiles now."""
    found = functions(src)
    if isinstance(found, str):
        return {"difference": "build fails", "similarity": "", "differing_instructions": "", "error": found}
    f = next((f for f in found if f["address"] == address), None)
    if f is None:
        return {"difference": "build fails", "similarity": "", "differing_instructions": "",
                "error": f"the file does not define the function at {address:#x}"}
    return {"difference": f["difference"], "similarity": f["similarity"], "differing_instructions": f["differing"]}


# ---- the three commands -------------------------------------------------------------------------------

def unit_folder(address: int) -> str:
    import tu_map
    if "units" not in _state:
        _state["units"] = tu_map.load_map()
        _state["starts"] = tu_map.unit_starts(_state["units"])
    u = tu_map.find_unit(_state["units"], address, _state["starts"])
    if u is None:
        raise SystemExit(f"drafts: {address:#x} is in no unit of data/tu_map.csv")
    return tu_map.source_folder(u)


def add(src: Path, address: int, tag: str, note: str = "", replace: bool = False, remove: bool = False,
        rows: list[dict] | None = None) -> dict:
    """File src as the draft of the function at address; returns its row. With rows, the caller writes the
    index (a batch); without, it is written here."""
    own = rows is None
    rows = read_index() if own else rows
    dst = ROOT / "drafts" / unit_folder(address) / f"fn_{address:x}.cpp"
    rel = dst.relative_to(ROOT).as_posix()
    old = next((r for r in rows if int(r["address"], 16) == address), None)
    if (old or dst.exists()) and not replace:
        raise SystemExit(f"drafts: {address:#x} has a draft already ({old['file'] if old else rel}); --replace to "
                         "put this one in its place")
    size = _state["sizes"].get(address)
    if size is None:
        raise SystemExit(f"drafts: {address:#x} is not a function of data/functions")
    text = src.read_text(encoding="utf-8").replace("\r\n", "\n")
    dst.parent.mkdir(parents=True, exist_ok=True)
    dst.write_text(rehome(text, src.parent, dst.parent), encoding="utf-8", newline="\n")
    if old:
        rows.remove(old)
        if old["file"] != rel and (ROOT / old["file"]).is_file():
            (ROOT / old["file"]).unlink()
    if remove and src.resolve() != dst.resolve():
        src.unlink()
    g = grade(dst, address)
    g.pop("error", None)
    row = {"address": f"0x{address:x}", "size": str(size), "file": rel, **g, "from": tag,
           "note": " ".join(note.replace(",", ";").split())}
    rows.append(row)
    if own:
        write_index(rows)
    return row


def cmd_add(args) -> int:
    row = add(args.file.resolve(), int(args.address, 16), args.tag, args.note, args.replace, args.remove)
    print(",".join(row[f] for f in FIELDS))
    return 0


def selected(rows: list[dict], files: list[Path], tag: str | None) -> list[dict]:
    want = {f.resolve().relative_to(ROOT).as_posix() for f in files}
    unknown = want - {r["file"] for r in rows}
    if unknown:
        raise SystemExit("drafts: no row of drafts/INDEX.csv for " + ", ".join(sorted(unknown)))
    return [r for r in rows if (not want or r["file"] in want) and (tag is None or r["from"] == tag)]


def cmd_grade(args) -> int:
    rows = read_index()
    chosen = selected(rows, args.files, args.tag)

    def one(r):
        p = ROOT / r["file"]
        if not p.is_file():
            return {"difference": "build fails", "similarity": "", "differing_instructions": "", "error": "no file"}
        return grade(p, int(r["address"], 16))
    with ThreadPoolExecutor(args.jobs) as pool:
        graded = list(pool.map(one, chosen))
    changed = 0
    for r, g in zip(chosen, graded):
        error = g.pop("error", None)
        was = (r["difference"], r["similarity"], r["differing_instructions"])
        now = (g["difference"], g["similarity"], g["differing_instructions"])
        if was != now:
            changed += 1
            print(f"{r['address']}  {was[0]} {was[1]}% ({was[2]})  ->  {now[0]} {now[1]}% ({now[2]})  {r['file']}"
                  + (f"  [{error}]" if error else ""))
            r.update(g)
    counts: dict[str, int] = {}
    for r in chosen:
        counts[r["difference"]] = counts.get(r["difference"], 0) + 1
    print(f"drafts: {len(chosen)} graded, {changed} differ from their row"
          + ("" if not changed else " (written)" if args.write else " (--write to record)") + "; now "
          + ", ".join(f"{n} {k}" for k, n in sorted(counts.items(), key=lambda kv: -kv[1])))
    if args.write and changed:
        write_index(rows)
    return 0


def verdicts(version: str, files: list[Path]) -> dict[int, tuple[str, str]]:
    """address -> (verdict, detail) from tools/equiv_test.py for the non-matching functions of these files."""
    out = OUT / "equiv.json"
    out.unlink(missing_ok=True)
    r = subprocess.run([sys.executable, str(ROOT / "tools" / "equiv_test.py"), "--version", version,
                        *[str(f) for f in files], "--json", str(out)], cwd=ROOT, capture_output=True, text=True)
    if not out.is_file():
        raise SystemExit("drafts: tools/equiv_test.py gave no verdicts:\n" + (r.stdout + r.stderr)[-600:])
    return {int(o["address"], 16): (o["verdict"], o.get("detail", "")) for o in json.loads(out.read_text("utf-8"))
            if o.get("address")}


def compiled(found: list[dict] | str):
    """What a file's functions are, for telling whether an edit changed the code."""
    return found if isinstance(found, str) else [(x["address"], x["difference"], x["differing"]) for x in found]


def take_header(lines: list[str], line: int, name: str, header: str) -> list[str] | None:
    """The file's lines without its own declaration of `name` (on 1-based `line`) and with the header that
    declares it included; None when the declaration is not one statement on one line, which is hand work."""
    import decl_check
    stmt = lines[line - 1].split("//")[0].rstrip() if 0 < line <= len(lines) else ""
    if (name not in stmt or not stmt.endswith(";") or len(decl_check.SYMBOL.findall(stmt)) != 1
            or not re.match(r"[A-Za-z_]", stmt) or stmt.startswith(name)):  # at the margin, a type before the name
        return None
    out = lines[:line - 1] + lines[line:]
    include = f'#include "{header}"'
    if include not in out:
        at = max((i for i, x in enumerate(out) if x.startswith("#include")), default=None)
        if at is None:  # no include yet: under the file's opening comment
            at = next((i for i, x in enumerate(out) if not x.startswith("//")), 0) - 1
        out.insert(at + 1, include)
    return out


def take_headers(path: Path, rel: str) -> list[str]:
    """For each declaration in the file of a symbol that a header under include/ declares: include the header
    and drop the declaration, one at a time, keeping an edit only when every function of the file compiles as
    before. Returns the symbols now taken from headers; the others are left for file_problems to name."""
    import decl_check
    taken: list[str] = []
    tried: set[str] = set()
    while True:
        todo = [c for c in decl_check.header_copies({rel}) if c[2] not in tried]
        if not todo:
            return taken
        _, line, name, header = todo[0]
        tried.add(name)
        old = path.read_text(encoding="utf-8")
        new = take_header(old.split("\n"), line, name, header)
        if new is None:
            continue
        before = compiled(functions(path))
        path.write_text("\n".join(new), encoding="utf-8", newline="\n")
        if compiled(functions(path)) == before:
            taken.append(name)
        else:
            path.write_text(old, encoding="utf-8", newline="\n")


def cmd_promote(args) -> int:
    import decl_check
    rows = read_index()
    chosen = selected(rows, args.files, None)
    if not chosen:
        raise SystemExit("drafts: name the drafts to move")
    # 1. what each file's functions are now
    state = {}
    for r in chosen:
        found = functions(ROOT / r["file"])
        state[r["file"]] = found
    need = [ROOT / f for f, found in state.items() if not isinstance(found, str)
            and all(x["difference"] in ("match", REGISTERS) for x in found)
            and any(x["difference"] == REGISTERS for x in found)]
    judged = verdicts(args.version, need) if need else {}
    moved, held, not_yet = [], [], []
    for r in chosen:
        found = state[r["file"]]
        if isinstance(found, str):
            not_yet.append((r, f"does not build: {found}"))
            continue
        other = [x for x in found if x["difference"] not in ("match", REGISTERS)]
        if other:
            not_yet.append((r, "; ".join(f"{x['name']}: {x['difference']}" for x in other)))
            continue
        unproven = [x for x in found if x["difference"] == REGISTERS and judged.get(x["address"], ("",))[0] != "pass"]
        if unproven:
            not_yet.append((r, "; ".join(f"{x['name']}: registers or order, tools/equiv_test.py says "
                                         f"{judged.get(x['address'], ('nothing',))[0]}" for x in unproven)))
            continue
        # 2. the copy in src/
        src = ROOT / r["file"]
        dst = ROOT / "src" / Path(r["file"]).relative_to("drafts")
        rel = dst.relative_to(ROOT).as_posix()
        if dst.exists():
            held.append((r, f"{rel} exists"))
            continue
        text = rehome(src.read_text(encoding="utf-8").replace("\r\n", "\n"), src.parent, dst.parent)
        own = [m.group(2) for m in INCLUDE.finditer(text) if "/drafts/" in m.group(2)]
        if own:
            held.append((r, f"includes {own[0].split('/drafts/')[1]} of drafts/: that header has to move first"))
            continue
        text, missing = annotate(text, [x["name"] for x in found if x["difference"] == REGISTERS])
        dst.parent.mkdir(parents=True, exist_ok=True)
        dst.write_text(text, encoding="utf-8", newline="\n")
        why, taken = None, []
        again = functions(dst)
        if missing:
            why = "no definition line found for " + ", ".join(missing)
        elif isinstance(again, str):
            why = f"does not build in src/: {again}"
        elif compiled(again) != compiled(found):
            why = "compiles to other code in src/"
        else:
            taken = take_headers(dst, rel)
            problems = decl_check.file_problems({rel})
            if problems:
                why = f"{len(problems)} declaration problem{'s' if len(problems) > 1 else ''}: " + short(problems[0], 260)
        if why:
            dst.unlink()
            held.append((r, why))
        elif args.write:
            src.unlink()
            rows.remove(r)
            moved.append((r, rel, taken))
        else:
            dst.unlink()
            moved.append((r, rel, taken))
        for d in (dst.parent, src.parent):
            if d.is_dir() and not any(d.iterdir()):
                d.rmdir()
    for r, rel, taken in moved:
        print(f"{'moved' if args.write else 'would move'}  {r['address']}  {rel}"
              + (f"  ({len(taken)} declarations taken from headers)" if taken else ""))
    for r, why in held:
        print(f"held   {r['address']}  {r['file']}\n       {why}")
        if args.write:
            head = "matches" if r["difference"] == "match" else "passes tools/equiv_test.py"
            r["note"] = f"{head}; held: " + short(why.replace(",", ";"), 200)
    for r, why in not_yet:
        print(f"not yet  {r['address']}  {r['file']}\n       {why}")
    print(f"drafts: {len(moved)} {'moved' if args.write else 'can move'} to src/, {len(held)} held on their "
          f"declarations, {len(not_yet)} not ready")
    if args.write:
        write_index(rows)
    return 0


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", required=True)
    sub = ap.add_subparsers(dest="command", required=True)
    a = sub.add_parser("add", help="file a source as the draft of one function")
    a.add_argument("file", type=Path)
    a.add_argument("address", help="the function's address (hex)")
    a.add_argument("--from", dest="tag", required=True, help="the run or piece of work it comes from")
    a.add_argument("--note", default="", help="what holds it back, in a few words")
    a.add_argument("--replace", action="store_true", help="in place of the draft the function has")
    a.add_argument("--remove", action="store_true", help="delete FILE once it is filed")
    g = sub.add_parser("grade", help="what separates each draft from the original now")
    g.add_argument("files", nargs="*", type=Path)
    g.add_argument("--from", dest="tag", help="only the rows of this run")
    g.add_argument("--write", action="store_true", help="record the grades in drafts/INDEX.csv")
    g.add_argument("--jobs", type=int, default=8)
    p = sub.add_parser("promote", help="move drafts that match or are verified equivalent to src/")
    p.add_argument("files", nargs="+", type=Path)
    p.add_argument("--write", action="store_true", help="keep the moves and write the reasons of the held ones")
    args = ap.parse_args()
    os.chdir(ROOT)
    init(args.version)
    return {"add": cmd_add, "grade": cmd_grade, "promote": cmd_promote}[args.command](args)


if __name__ == "__main__":
    sys.exit(main())
