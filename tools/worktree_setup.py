#!/usr/bin/env python3
"""Give a git worktree the local, untracked inputs the tools need, from the checkout this runs in.

A new worktree has only the tracked files. The matching tools also need the dump (orig/), the
Python environment (.venv), the function bounds (build/recon/<version>) and the submodules; the
boot slice and tools/hubs.py need the Ghidra export; the P-code oracle needs its two caches. This
links or copies each one, initialises the submodules, and then proves the result by grading one
source that is known to match. Run it from the checkout that has those inputs (the main one).

  linked   .venv, orig, build/ghidra_export   read by the tools, never written from a worktree
  copied   build/recon/<version>              small, and tools/recon/run.py rewrites it
  copied   build/port/abi_facts_v*.pickle, build/port/pcode_cache.pickle   with --oracle: runs add to them

A directory link is a junction on Windows (no privilege needed; Git Bash's `ln -s` copies instead,
12.6 GB once) and a symbolic link elsewhere. Nothing that exists in the worktree is replaced or
removed. Remove a link with `rmdir` (Windows) or `rm` (elsewhere), never with a recursive delete.

Usage:
  python tools/worktree_setup.py ../dread-decomp-topic --new claude/topic   # git worktree add -b <branch> <path> origin/main, then set it up
  python tools/worktree_setup.py ../dread-decomp-topic                      # set up a worktree that exists
  python tools/worktree_setup.py ../dread-decomp-topic --oracle             # also the oracle's caches
  python tools/worktree_setup.py ../dread-decomp-topic --check              # only report; exit 1 if something is missing
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import hostenv  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
PROOF = "src/atk_id/nn_atk_Shutdown_pilot.cpp"  # a small source that matches (the kits' own check)


@dataclass
class Item:
    how: str        # "link" (a directory link) or "copy" (a file or a directory)
    path: str       # relative to both checkouts
    why: str
    required: bool = True


def items(source: Path, version: str, oracle: bool) -> list[Item]:
    out = [Item("link", ".venv", "the Python environment"),
           Item("link", "orig", "the dump"),
           Item("copy", f"build/recon/{version}", "function bounds (tools/recon/run.py)"),
           Item("link", "build/ghidra_export", "the Ghidra export: the boot slice, tools/hubs.py", required=False)]
    if oracle:
        port = source / "build" / "port"
        facts = sorted(port.glob("abi_facts_v*.pickle"), key=lambda p: int(p.stem.rsplit("v", 1)[1]))
        out.append(Item("copy", f"build/port/{facts[-1].name}" if facts else "build/port/abi_facts_v*.pickle",
                        "argument facts (tools/abi_facts.py)", required=False))
        out.append(Item("copy", "build/port/pcode_cache.pickle", "P-code of instruction words (tools/ghidra/pcode.py)",
                        required=False))
    return out


def link_dir(target: Path, link: Path) -> None:
    """A directory link at `link` to `target`: a junction on Windows, a symbolic link elsewhere."""
    link.parent.mkdir(parents=True, exist_ok=True)
    if hostenv.WINDOWS:
        r = subprocess.run(["cmd", "/c", "mklink", "/J", str(link), str(target.resolve())], capture_output=True, text=True)
        if r.returncode != 0:
            raise OSError(f"mklink /J {link}: {(r.stdout + r.stderr).strip()}")
    else:
        os.symlink(target.resolve(), link, target_is_directory=True)


def provide(item: Item, source: Path, dest: Path, dry: bool) -> str:
    """'present', 'linked', 'copied', 'missing' (dry run) or 'no source': what was done for one item."""
    src, dst = source / item.path, dest / item.path
    if dst.exists():
        return "present"
    if not src.exists():
        return "no source"
    if dry:
        return "missing"
    if item.how == "link":
        link_dir(src, dst)
        return "linked"
    dst.parent.mkdir(parents=True, exist_ok=True)
    if src.is_dir():
        shutil.copytree(src, dst)
    else:
        shutil.copy2(src, dst)
    return "copied"


def run(cmd: list[str], cwd: Path) -> subprocess.CompletedProcess:
    return subprocess.run(cmd, cwd=cwd, capture_output=True, text=True)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("worktree", type=Path)
    ap.add_argument("--new", metavar="BRANCH", help="create the worktree first, on a new branch from --base")
    ap.add_argument("--base", default="origin/main", help="what --new branches from (default %(default)s)")
    ap.add_argument("--version", default="2.1.0")
    ap.add_argument("--oracle", action="store_true", help="also copy the P-code oracle's caches (build/port)")
    ap.add_argument("--check", action="store_true", help="report only; exit 1 if a required input is missing")
    ap.add_argument("--no-proof", action="store_true", help="skip grading the known match")
    args = ap.parse_args()
    dest = args.worktree.resolve()
    if dest == ROOT:
        ap.error("the worktree is this checkout")

    if args.new and not args.check:
        r = run(["git", "worktree", "add", "-b", args.new, str(dest), args.base], ROOT)
        if r.returncode != 0:
            print(r.stderr.strip())
            return 1
    if not (dest / ".git").exists():
        print(f"{dest}: not a git worktree (create it with --new BRANCH, or `git worktree add`)")
        return 1

    bad = 0
    for item in items(ROOT, args.version, args.oracle):
        state = provide(item, ROOT, dest, args.check)
        failed = state in ("missing", "no source") and item.required
        bad += failed
        print(f"{state:9}  {item.path}  ({item.why})" + ("" if item.required else "  [optional]"))
    if args.check:
        return 1 if bad else 0
    if bad:
        print(f"{bad} required input(s) could not be provided from {ROOT}")
        return 1

    r = run(["git", "submodule", "update", "--init"], dest)
    print("submodules  " + ("ready" if r.returncode == 0 else f"FAILED: {r.stderr.strip()[-300:]}"))
    if r.returncode != 0:
        return 1
    if args.no_proof or not (dest / PROOF).is_file():
        return 0
    r = run([sys.executable, "tools/trymatch.py", "--version", args.version, PROOF], dest)
    last = (r.stdout.strip().splitlines() or [r.stderr.strip()[-300:]])[-1]
    print(f"proof       {PROOF}: {last}")
    return 0 if r.returncode == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
