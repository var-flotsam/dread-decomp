#!/usr/bin/env python3
"""Snapshot and compare tools/trymatch.py output for a set of sources, in parallel.

The evidence a canonical-header migration rests on (CONTRIBUTING.md, "Migrating a hub"): every
touched source, and every includer of a touched header, rebuilds to the same per-function result,
non-matching percentages and RELOC lines included. Agents did this by hand, one trymatch call at a
time, for hundreds of files per batch. This does it for a file set in one command, keeps a copy of
each source as it was, and puts back the ones whose output changed.

  python tools/baseline.py save    --tag T [FILE...] [@LIST] [--symbol SYM]... [--includers HDR]...
  python tools/baseline.py compare --tag T [the same selection; default: every file saved under T]
  python tools/baseline.py restore --tag T [FILE...]          # default: every file compare found changed
  python tools/baseline.py files   [selection]                 # print the resolved file set

Selection: repository paths (a header expands to its hand-written includers, transitively); @FILE
with one path per line; --symbol NAME, every hand-written file that declares NAME (tools/decl_check.py);
--includers HEADER, the hand-written includers of HEADER. Generated directories are never included.

Outputs under build/baseline/<tag>/: out/<path>.txt (trymatch stdout and stderr with the exit status)
and src/<path> (the source as saved). `compare` prints UNCHANGED, CHANGED or NEW per file with the
differing lines, and exits 1 if anything changed; a file that compiled before and fails now is
CHANGED too. Objects go to build/baseline/obj/<dir of the source>/, so parallel runs on different
sources never collide (tools/trymatch.py --obj-dir). Standard library only, plus the project tools.
"""

from __future__ import annotations

import argparse
import difflib
import os
import re
import shutil
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import decl_check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
BASE = ROOT / "build" / "baseline"
OBJ = BASE / "obj"


def rel(path: Path | str) -> str:
    p = Path(path)
    if not p.is_absolute():
        p = ROOT / p
    return p.resolve().relative_to(ROOT).as_posix()


def hand_written_files() -> list[str]:
    names = subprocess.run(["git", "ls-files", "--cached", "--others", "--exclude-standard", "--", "src", "include"],
                           cwd=ROOT, capture_output=True, text=True, check=True).stdout.split("\n")
    return sorted({n for n in names if n and decl_check.hand_written(n) and (ROOT / n).is_file()})


_INCLUDE_CACHE: dict[str, list[str]] | None = None


def include_map() -> dict[str, list[str]]:
    """header basename -> hand-written files whose #include names it (by its last path component)."""
    global _INCLUDE_CACHE
    if _INCLUDE_CACHE is None:
        pat = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"', re.M)
        out: dict[str, list[str]] = {}
        for f in hand_written_files():
            try:
                text = (ROOT / f).read_text(encoding="utf-8", errors="replace")
            except OSError:
                continue
            for m in pat.finditer(text):
                out.setdefault(m.group(1).rsplit("/", 1)[-1], []).append(f)
        _INCLUDE_CACHE = out
    return _INCLUDE_CACHE


def includers(header: str) -> set[str]:
    """Hand-written sources that include `header` directly or through other hand-written headers."""
    seen: set[str] = set()
    todo = [header]
    while todo:
        h = todo.pop()
        for f in include_map().get(Path(h).name, []):
            if f not in seen and f != h:
                seen.add(f)
                if f.endswith(decl_check.SUFFIXES[3:]):  # .h, .hpp
                    todo.append(f)
    return seen


def expand(paths: set[str]) -> set[str]:
    """Headers become their includers; the result holds compilable sources only."""
    out: set[str] = set()
    for p in paths:
        if p.endswith((".h", ".hpp")):
            out |= {f for f in includers(p) if not f.endswith((".h", ".hpp"))}
        else:
            out.add(p)
    return out


def select(files: list[str], symbols: list[str], headers: list[str], report: decl_check.Report | None = None) -> list[str]:
    paths: set[str] = set()
    for f in files:
        if f.startswith("@"):
            paths |= {ln.strip() for ln in Path(f[1:]).read_text(encoding="utf-8").splitlines() if ln.strip()}
        else:
            paths.add(rel(f))
    if symbols:
        report = report or decl_check.Report(decl_check.tree_files(None))
        for s in symbols:
            paths |= set(report.paths.get(s, []))
    for h in headers:
        paths |= includers(rel(h))
    paths = {p for p in paths if decl_check.hand_written(p)}
    return sorted(expand(paths))


DIAG_POS = re.compile(r"^(.*?):\d+:\d+: (warning|note): ", re.M)


def normalise(output: str) -> str:
    """A warning's or note's line and column are dropped: removing a declaration above a line
    moves every diagnostic below it without changing the code it is about."""
    return DIAG_POS.sub(r"\1: \2: ", output)


def trymatch(version: str, path: str) -> str:
    obj_dir = OBJ / Path(path).parent
    cmd = [sys.executable, str(ROOT / "tools" / "trymatch.py"), "--version", version, path, "--obj-dir", str(obj_dir)]
    r = subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    out = r.stdout.replace("\r\n", "\n")
    if r.stderr.strip():
        out += "\n[stderr]\n" + r.stderr.replace("\r\n", "\n")
    return normalise(out + f"\n[exit {r.returncode}]\n")


def run_all(version: str, paths: list[str], jobs: int) -> dict[str, str]:
    with ThreadPoolExecutor(max_workers=max(1, jobs)) as pool:
        return dict(zip(paths, pool.map(lambda p: trymatch(version, p), paths)))


def out_path(tag: str, path: str) -> Path:
    return BASE / tag / "out" / (path + ".txt")


def src_path(tag: str, path: str) -> Path:
    return BASE / tag / "src" / path


def save(tag: str, paths: list[str], version: str, jobs: int, copies: list[str] = ()) -> dict[str, str]:
    """Record trymatch output and a copy of each source under build/baseline/<tag>/; `copies` are
    files (headers) copied without being compiled, so that `restore` covers them too."""
    outputs = run_all(version, paths, jobs)
    for p, text in outputs.items():
        o = out_path(tag, p)
        o.parent.mkdir(parents=True, exist_ok=True)
        o.write_text(text, encoding="utf-8")
    for p in list(outputs) + list(copies):
        s = src_path(tag, p)
        s.parent.mkdir(parents=True, exist_ok=True)
        shutil.copyfile(ROOT / p, s)
    return outputs


def saved_files(tag: str) -> list[str]:
    """Every file the baseline holds a copy of (compiled sources and copied headers)."""
    root = BASE / tag / "src"
    if not root.is_dir():
        return []
    return sorted(f.relative_to(root).as_posix() for f in root.rglob("*") if f.is_file())


def broken(output: str) -> bool:
    """trymatch could not build the source (exit 2)."""
    return output.rstrip().endswith("[exit 2]")


def compare(tag: str, paths: list[str], version: str, jobs: int) -> dict[str, tuple[str, str]]:
    """path -> ('UNCHANGED' | 'CHANGED' | 'NEW', unified diff of the trymatch outputs)."""
    outputs = run_all(version, paths, jobs)
    result: dict[str, tuple[str, str]] = {}
    for p, now in outputs.items():
        o = out_path(tag, p)
        if not o.is_file():
            result[p] = ("NEW", now)
            continue
        before = normalise(o.read_text(encoding="utf-8"))
        if before == now:
            result[p] = ("UNCHANGED", "")
        else:
            diff = "".join(difflib.unified_diff(before.splitlines(True), now.splitlines(True),
                                                f"baseline/{p}", f"now/{p}", n=0))
            result[p] = ("CHANGED", diff)
    return result


def restore(tag: str, paths: list[str]) -> list[str]:
    """Copy the saved sources back; returns the paths restored."""
    done = []
    for p in paths:
        s = src_path(tag, p)
        if not s.is_file():
            continue
        data = s.read_bytes()
        for attempt in range(5):  # Windows: a scanner or an editor may hold the file for a moment
            try:
                (ROOT / p).write_bytes(data)
                break
            except OSError:
                if attempt == 4:
                    raise
                time.sleep(0.5)
        done.append(p)
    return done


def summary_line(text: str) -> str:
    """The `N/M functions match` line of a trymatch output, or its failure."""
    for line in reversed(text.splitlines()):
        if "functions match" in line or line.startswith("BUILD FAILED") or "error:" in line:
            return line.strip()
    return text.strip().splitlines()[-1] if text.strip() else "(no output)"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("command", choices=["save", "compare", "restore", "files"])
    ap.add_argument("files", nargs="*", help="paths, or @LIST files")
    ap.add_argument("--tag", help="baseline name under build/baseline/ (save, compare, restore)")
    ap.add_argument("--version", default="2.1.0")
    ap.add_argument("--symbol", action="append", default=[], help="every hand-written file declaring this symbol")
    ap.add_argument("--includers", action="append", default=[], help="hand-written includers of this header")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    ap.add_argument("-q", "--quiet", action="store_true", help="compare: print changed files only")
    args = ap.parse_args()

    if args.command != "files" and not args.tag:
        ap.error("--tag is required")
    paths = select(args.files, args.symbol, args.includers)
    if args.command == "files":
        print("\n".join(paths))
        return 0
    if args.command == "save":
        outputs = save(args.tag, paths, args.version, args.jobs)
        for p in paths:
            print(f"saved  {p}  {summary_line(outputs[p])}")
        print(f"baseline {args.tag}: {len(paths)} files under {rel(BASE / args.tag)}")
        return 0
    if args.command == "restore":
        targets = paths or [p for p in saved_files(args.tag)]
        if not paths:
            marks = BASE / args.tag / "changed.txt"
            if marks.is_file():
                targets = [ln.strip() for ln in marks.read_text(encoding="utf-8").splitlines() if ln.strip()]
        for p in restore(args.tag, targets):
            print(f"restored  {p}")
        return 0
    # compare
    if not paths:
        paths = [p for p in saved_files(args.tag) if not p.endswith((".h", ".hpp"))]
    result = compare(args.tag, paths, args.version, args.jobs)
    changed = [p for p, (st, _) in result.items() if st != "UNCHANGED"]
    for p in paths:
        st, diff = result[p]
        if st == "UNCHANGED" and args.quiet:
            continue
        print(f"{st:9s} {p}")
        if diff:
            for line in diff.splitlines()[2:]:
                print("    " + line)
    (BASE / args.tag).mkdir(parents=True, exist_ok=True)
    (BASE / args.tag / "changed.txt").write_text("".join(p + "\n" for p in changed), encoding="utf-8")
    print(f"compare {args.tag}: {len(paths)} files, {len(changed)} changed"
          + (f" (`baseline.py restore --tag {args.tag}` puts them back)" if changed else ""))
    return 1 if changed else 0


if __name__ == "__main__":
    sys.exit(main())
