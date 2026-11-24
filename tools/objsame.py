#!/usr/bin/env python3
"""Does a source still compile to the same object? Snapshot and compare objects, with no game data.

The acceptance test for edits that must not change code (renaming a type or function, moving a
declaration, tidying text): compile each hand-written source with the project compiler and flags
(tools/buildcfg.py), record per object its section bytes, relocations (offset, type, addend,
target) and defined symbols, then rebuild after the edit and compare.

  python tools/objsame.py save    --tag T [FILE...] [@LIST] [--all] [--base REF]
  python tools/objsame.py compare --tag T [--modulo-names] [-q] [the same selection; default: every file saved]
  python tools/objsame.py files   [selection]                 # print the resolved file set

Selection: repository paths (a header expands to its hand-written includers, transitively), @FILE
with one path per line, --all (every hand-written source under src/). --base REF: the files that
differ between REF and the working tree (untracked ones included), and the includers of any such
header; `save --base REF` compiles them as they are at REF, in a temporary worktree, so the
snapshot can be taken after the edit. Generated directories are never included.

Two modes for compare: exact (every name as compiled), and --modulo-names, where every symbol name
is cut to its qualified name without parameter types (`a::f(UnkX*) const` -> `a::f`) before
comparing symbols, relocation targets and section names: the test for renaming a type, or a
function's parameter types. `compare` prints UNCHANGED or CHANGED per file, with what differs, and
exits 1 on any change; a file that does not compile on either side is CHANGED. A change in which
only names differ (a function renamed) is CHANGED with the line "names only" first.

Every compile runs from the tree's root with repository paths made relative, so a file name the
code embeds (`__FILE__`) is the same in a REF worktree as in the checkout. Objects and records go
to build/objsame/<tag>/ (base/ for the snapshot, now/ for the last compare), apart from
tools/check.py's cache. Standard library only, plus the project tools.
"""

from __future__ import annotations

import argparse
import hashlib
import json
import os
import shutil
import struct
import subprocess
import sys
import tempfile
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import baseline  # noqa: E402
import buildcfg  # noqa: E402
import decl_check  # noqa: E402
import hostenv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "build" / "objsame"
SUBMODULES = ("lib/musl", "lib/nnsdk")

# ELF constants (64-bit, little-endian: the project target).
SHT_SYMTAB, SHT_RELA, SHT_NOBITS = 2, 4, 8
SHF_ALLOC = 0x2
STT_SECTION, STT_FILE = 3, 4
SHN_UNDEF, SHN_LORESERVE = 0, 0xff00
SPECIAL_SHN = {0xfff1: "*ABS*", 0xfff2: "*COM*"}


# --- compiling ---------------------------------------------------------------------------------

def relative_command(cmd: list[str], tree: Path) -> list[str]:
    """A command with every path under ROOT or `tree` made relative, to run from `tree`'s root."""
    out = []
    for arg in cmd:
        for root in (tree, ROOT):
            if arg == str(root) or arg.startswith(str(root) + os.sep):
                arg = os.path.relpath(arg, root)
                break
        out.append(arg)
    return out


def compile_source(path: str, obj: Path, tree: Path = ROOT) -> str | None:
    """Compile the repository path `path` as it is in `tree` into `obj`. Returns None, or the error."""
    src = tree / path
    if not src.is_file():
        return "no such file"
    obj.parent.mkdir(parents=True, exist_ok=True)
    obj.unlink(missing_ok=True)
    try:
        cmd = relative_command(buildcfg.cxx_command(src, obj), tree)  # reads the tree's `// build-flags:`
    except ValueError as e:
        return str(e)
    cmd[cmd.index("-o") + 1] = str(obj.resolve())
    r = subprocess.run(cmd, cwd=tree, capture_output=True, text=True)
    if r.returncode != 0:
        lines = [ln for ln in r.stderr.splitlines() if "error" in ln] or r.stderr.splitlines() or ["failed"]
        return lines[0].strip()
    return None


# --- reading objects ---------------------------------------------------------------------------

def read_elf(data: bytes) -> dict:
    """Sections (allocated ones), relocations into them and defined symbols of an ELF64 object."""
    if data[:4] != b"\x7fELF" or data[4] != 2 or data[5] != 1:
        raise ValueError("not a 64-bit little-endian ELF object")
    shoff, = struct.unpack_from("<Q", data, 0x28)
    shentsize, shnum, shstrndx = struct.unpack_from("<HHH", data, 0x3a)
    headers = [struct.unpack_from("<IIQQQQIIQQ", data, shoff + i * shentsize) for i in range(shnum)]

    def strtab(index: int, offset: int) -> str:
        start = headers[index][4] + offset
        return data[start:data.index(b"\0", start)].decode("utf-8", "replace")

    names = [strtab(shstrndx, h[0]) for h in headers]
    symbols: list[tuple[str, int, int, int, int, int]] = []  # name, info, other, shndx, value, size
    for h in headers:
        if h[1] == SHT_SYMTAB:
            for off in range(h[4], h[4] + h[5], h[9] or 24):
                st_name, info, other, shndx, value, size = struct.unpack_from("<IBBHQQ", data, off)
                symbols.append((strtab(h[6], st_name), info, other, shndx, value, size))

    def section_key(index: int) -> str:
        return SPECIAL_SHN.get(index, names[index] if index < len(names) else f"#{index}")

    sections = []
    for i, h in enumerate(headers):
        if h[2] & SHF_ALLOC:
            body = b"" if h[1] == SHT_NOBITS else data[h[4]:h[4] + h[5]]
            sections.append({"name": names[i], "type": h[1], "flags": h[2], "align": h[8], "size": h[5],
                             "sha256": hashlib.sha256(body).hexdigest(), "relocs": []})
    by_index = {i: s for i, s in zip((i for i, h in enumerate(headers) if h[2] & SHF_ALLOC), sections)}
    for h in headers:
        if h[1] == SHT_RELA and h[7] in by_index:
            for off in range(h[4], h[4] + h[5], h[9] or 24):
                r_offset, r_info, r_addend = struct.unpack_from("<QQq", data, off)
                name, info, _, shndx, _, _ = symbols[r_info >> 32]
                target = f"section {section_key(shndx)}" if info & 0xf == STT_SECTION else name
                by_index[h[7]]["relocs"].append([r_offset, r_info & 0xffffffff, r_addend, target])
    defined = []
    for name, info, other, shndx, value, size in symbols:
        if not name or info & 0xf in (STT_SECTION, STT_FILE) or shndx == SHN_UNDEF:
            continue
        if SHN_LORESERVE <= shndx and shndx not in SPECIAL_SHN:
            continue
        defined.append([name, section_key(shndx), value, size, info & 0xf, info >> 4, other & 3])
    return {"sections": sections, "symbols": sorted(defined)}


# --- cutting names -----------------------------------------------------------------------------

QUALIFIER_TAIL = (" const", " volatile", " &&", " &")


def cut(demangled: str) -> str:
    """A demangled name without parameter lists: 'a::f(int) const' -> 'a::f',
    'f(UnkX*)::s' -> 'f::s'. A list directly after a name or a template's '>' is cut, with the
    qualifiers after it; '(anonymous namespace)' and parentheses inside template arguments
    (a function type such as `void (*)(int)`) are kept."""
    out: list[str] = []
    i, n, angle = 0, len(demangled), 0
    while i < n:
        c = demangled[i]
        if c == "<":
            angle += 1
        elif c == ">":
            angle = max(0, angle - 1)
        if (c == "(" and angle == 0 and out and (out[-1].isalnum() or out[-1] in "_>~]")
                and not demangled.startswith("(anonymous namespace)", i)):
            depth = 0
            while i < n:
                depth += {"(": 1, ")": -1}.get(demangled[i], 0)
                i += 1
                if depth == 0:
                    break
            changed = True
            while changed:
                changed = False
                for q in QUALIFIER_TAIL:
                    if demangled.startswith(q, i):
                        i += len(q)
                        changed = True
            continue
        out.append(c)
        i += 1
    return "".join(out)


def mangled_part(name: str) -> tuple[str, str] | None:
    """(prefix, mangled) when a symbol or section name holds an Itanium name: '_ZN1a1fEv', or
    '.text._ZN1a1fEv' (one section per function)."""
    at = name.find("_Z")
    if at == 0:
        return "", name
    if at > 0 and name[at - 1] == ".":
        return name[:at], name[at:]
    return None


def demangle(names: set[str]) -> dict[str, str]:
    """Mangled -> demangled, through one llvm-cxxfilt run for the whole set."""
    names = sorted(n for n in names if n)
    if not names:
        return {}
    out = subprocess.run([hostenv.cxxfilt()], input="\n".join(names), capture_output=True, text=True,
                         check=True).stdout.splitlines()
    return dict(zip(names, out))


def record_names(rec: dict) -> set[str]:
    found: set[str] = set()
    for s in rec.get("sections", []):
        found.add(s["name"])
        for r in s["relocs"]:
            found.add(r[3][len("section "):] if r[3].startswith("section ") else r[3])
    for sym in rec.get("symbols", []):
        found.update((sym[0], sym[1]))
    return {m[1] for m in map(mangled_part, found) if m}


def renamer(records: list[dict]):
    """name -> its cut form, for every Itanium name the records hold."""
    full = demangle(set().union(*(record_names(r) for r in records)) if records else set())

    def rename(name: str) -> str:
        if name.startswith("section "):
            return "section " + rename(name[len("section "):])
        m = mangled_part(name)
        if not m or m[1] not in full:
            return name
        return m[0] + cut(full[m[1]])
    return rename


def normalised(rec: dict, rename=None) -> dict:
    """The comparable form of a record: names cut when `rename` is given."""
    if "error" in rec:
        return rec
    rename = rename or (lambda n: n)
    sections = [{**s, "name": rename(s["name"]),
                 "relocs": sorted([r[0], r[1], r[2], rename(r[3])] for r in s["relocs"])}
                for s in rec["sections"]]
    symbols = sorted([rename(s[0]), rename(s[1]), *s[2:]] for s in rec["symbols"])
    return {"sections": sections, "symbols": symbols}


def skeleton(rec: dict) -> tuple:
    """A record with every name left out: what stays the same when only names change."""
    return (tuple((s["type"], s["flags"], s["align"], s["size"], s["sha256"],
                   tuple(sorted((r[0], r[1], r[2]) for r in s["relocs"]))) for s in rec["sections"]),
            tuple(sorted(tuple(s[2:]) for s in rec["symbols"])))


def differences(before: dict, after: dict, limit: int = 6) -> list[str]:
    """What differs between two normalised records, one line each (at most `limit`)."""
    if "error" in before or "error" in after:
        out = []
        if "error" in before:
            out.append(f"before: does not compile ({before['error']})")
        if "error" in after:
            out.append(f"after: does not compile ({after['error']})")
        return out
    out: list[str] = []
    a, b = before["sections"], after["sections"]
    if [s["name"] for s in a] != [s["name"] for s in b]:
        gone = sorted({s["name"] for s in a} - {s["name"] for s in b})
        new = sorted({s["name"] for s in b} - {s["name"] for s in a})
        out.append("sections differ" + (f"; only before: {', '.join(gone[:3])}" if gone else "")
                   + (f"; only after: {', '.join(new[:3])}" if new else "")
                   + ("" if gone or new else " in order"))
    else:
        for x, y in zip(a, b):
            if x["sha256"] != y["sha256"] or x["size"] != y["size"]:
                out.append(f"bytes of {x['name']} ({x['size']} -> {y['size']} bytes)")
            elif (x["type"], x["flags"], x["align"]) != (y["type"], y["flags"], y["align"]):
                out.append(f"attributes of {x['name']}")
            if x["relocs"] != y["relocs"]:
                diff = sorted(set(map(tuple, x["relocs"])) ^ set(map(tuple, y["relocs"])))
                first = diff[0] if diff else None
                out.append(f"relocations in {x['name']}"
                           + (f" (first at +0x{first[0]:x}: {first[3]})" if first else " (order)"))
    sa, sb = set(map(tuple, before["symbols"])), set(map(tuple, after["symbols"]))
    for label, items in (("only before", sa - sb), ("only after", sb - sa)):
        if items:
            names = sorted({s[0] for s in items})
            out.append(f"symbols {label}: {', '.join(names[:3])}" + (f" and {len(names) - 3} more" if len(names) > 3 else ""))
    if not out and before != after:
        out.append("objects differ")
    if out and skeleton(before) == skeleton(after):
        out.insert(0, "names only: the same bytes, relocations and symbols apart from their names")
    return out[:limit] + ([f"... and {len(out) - limit} more"] if len(out) > limit else [])


# --- snapshots ---------------------------------------------------------------------------------

def tag_dir(tag: str) -> Path:
    if not tag or "/" in tag or "\\" in tag or tag.startswith("."):
        raise SystemExit(f"objsame: bad tag {tag!r}")
    return BASE / tag


def build_record(path: str, out: Path, tree: Path = ROOT) -> dict:
    obj = out / "obj" / (path + ".o")
    err = compile_source(path, obj, tree)
    if err is not None:
        rec = {"error": err}
    else:
        try:
            rec = read_elf(obj.read_bytes())
        except (OSError, ValueError, struct.error) as e:
            rec = {"error": f"unreadable object: {e}"}
    target = out / "rec" / (path + ".json")
    target.parent.mkdir(parents=True, exist_ok=True)
    target.write_text(json.dumps(rec), encoding="utf-8")
    return rec


def build_all(paths: list[str], out: Path, jobs: int, tree: Path = ROOT) -> dict[str, dict]:
    with ThreadPoolExecutor(max(1, jobs)) as pool:
        return dict(zip(paths, pool.map(lambda p: build_record(p, out, tree), paths)))


def git(*args: str, cwd: Path = ROOT) -> str:
    return subprocess.run(["git", *args], cwd=cwd, capture_output=True, text=True, check=True).stdout


def changed_since(ref: str) -> list[str]:
    """Hand-written sources to rebuild for the edits since `ref`: changed ones and includers of changed headers."""
    paths = set(git("diff", "--name-only", "--no-renames", ref, "--").split("\n"))
    paths |= set(git("ls-files", "--others", "--exclude-standard").split("\n"))
    return sorted(baseline.expand({p for p in paths if p and decl_check.hand_written(p)}))


class Worktree:
    """A temporary detached worktree at `ref`, with the submodules borrowed from the checkout."""

    def __init__(self, ref: str):
        self.ref = ref
        self.path = Path(tempfile.mkdtemp(prefix="objsame-"))

    def __enter__(self) -> Path:
        git("worktree", "add", "--detach", "--quiet", str(self.path), self.ref)
        for sub in SUBMODULES:
            dest = self.path / sub
            if dest.is_dir() and not any(dest.iterdir()):
                dest.rmdir()
            if not dest.exists():
                dest.symlink_to(ROOT / sub, target_is_directory=True)
        return self.path

    def __exit__(self, *exc) -> None:
        subprocess.run(["git", "worktree", "remove", "--force", str(self.path)], cwd=ROOT, capture_output=True)
        shutil.rmtree(self.path, ignore_errors=True)
        subprocess.run(["git", "worktree", "prune"], cwd=ROOT, capture_output=True)


def save(tag: str, paths: list[str], jobs: int, ref: str | None = None) -> dict[str, dict]:
    out = tag_dir(tag) / "base"
    shutil.rmtree(out, ignore_errors=True)
    out.mkdir(parents=True)
    if ref:
        with Worktree(ref) as tree:
            records = build_all(paths, out, jobs, tree)
    else:
        records = build_all(paths, out, jobs)
    (out / "files.txt").write_text("".join(p + "\n" for p in paths), encoding="utf-8")
    (out / "meta.json").write_text(json.dumps({"ref": ref or "working tree", "compiler": buildcfg.COMPILER}),
                                   encoding="utf-8")
    return records


def saved(tag: str) -> list[str]:
    f = tag_dir(tag) / "base" / "files.txt"
    if not f.is_file():
        raise SystemExit(f"objsame: no snapshot {tag!r} (run `objsame.py save --tag {tag}` first)")
    return [ln for ln in f.read_text(encoding="utf-8").splitlines() if ln]


def compare(tag: str, paths: list[str], jobs: int, modulo_names: bool) -> dict[str, list[str]]:
    """path -> differences ([] when unchanged)."""
    base = tag_dir(tag) / "base"
    before = {}
    for p in paths:
        f = base / "rec" / (p + ".json")
        before[p] = json.loads(f.read_text(encoding="utf-8")) if f.is_file() else {"error": "not in the snapshot"}
    now = tag_dir(tag) / "now"
    shutil.rmtree(now, ignore_errors=True)
    after = build_all(paths, now, jobs)
    rename = renamer(list(before.values()) + list(after.values())) if modulo_names else None
    return {p: differences(normalised(before[p], rename), normalised(after[p], rename)) for p in paths}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", choices=["save", "compare", "files"])
    ap.add_argument("files", nargs="*", help="paths, or @LIST files")
    ap.add_argument("--tag", help="snapshot name under build/objsame/ (save, compare)")
    ap.add_argument("--all", action="store_true", help="every hand-written source under src/")
    ap.add_argument("--base", metavar="REF", help="the files changed since REF; save compiles them as at REF")
    ap.add_argument("--modulo-names", action="store_true",
                    help="compare: cut symbol names to the qualified name without parameter types")
    ap.add_argument("--jobs", "-j", type=int, default=os.cpu_count() or 4)
    ap.add_argument("-q", "--quiet", action="store_true", help="compare: print changed files only")
    args = ap.parse_args()

    if args.command != "files" and not args.tag:
        ap.error("--tag is required")
    paths = baseline.select(args.files, [], [], everything=args.all) if args.files or args.all else []
    if args.base:
        paths = sorted(set(paths) | set(changed_since(args.base)))
    if args.command == "files":
        print("\n".join(paths))
        return 0
    if args.command == "save":
        if not paths:
            ap.error("nothing selected: name files, --all or --base REF")
        records = save(args.tag, paths, args.jobs, args.base)
        failed = [p for p, r in records.items() if "error" in r]
        for p in failed:
            print(f"FAILED    {p}  {records[p]['error']}")
        print(f"objsame {args.tag}: saved {len(paths)} files from {args.base or 'the working tree'}"
              + (f", {len(failed)} do not compile" if failed else ""))
        return 0
    paths = paths or saved(args.tag)
    result = compare(args.tag, paths, args.jobs, args.modulo_names)
    changed = [p for p, d in result.items() if d]
    for p in paths:
        if result[p]:
            print(f"CHANGED   {p}")
            for line in result[p]:
                print(f"    {line}")
        elif not args.quiet:
            print(f"UNCHANGED {p}")
    mode = "modulo names" if args.modulo_names else "exact"
    print(f"objsame {args.tag} ({mode}): {len(paths)} files, {len(changed)} changed")
    return 1 if changed else 0


if __name__ == "__main__":
    sys.exit(main())
