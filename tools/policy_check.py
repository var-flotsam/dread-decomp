#!/usr/bin/env python3
"""Check tracked files against the content rules in CONTRIBUTING.md.

Rejects (as tools/hooks/pre-commit does for staged files, plus content checks):
  - anything under orig/ or build/ (local-only);
  - console binaries, containers and key files by extension;
  - files over 1 MB;
  - text that looks like an assembly listing (several consecutive address + AArch64
    instruction lines) or a key file (`name = <32+ hex digits>` lines);
  - personal data: a home directory path with a user name in it, or an email address other
    than a GitHub noreply address.

Usage:
  python tools/policy_check.py                 # every tracked file
  python tools/policy_check.py --base <ref>    # files changed since <ref> (CI on pull requests)
Exit status 1 if anything is rejected. Standard library only, so CI needs no game data.
"""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MAX_BYTES = 1024 * 1024
LOCAL_DIRS = ("orig/", "build/")
FORBIDDEN_EXT = {".nso", ".nro", ".nca", ".nsp", ".xci", ".nsz", ".xcz", ".keys", ".elf", ".bin",
                 ".pak", ".bmsad", ".bmscc", ".bcmdl", ".bctex", ".gpr"}
INSN = r"(?:ldr|ldp|str|stp|adrp|add|sub|mov|bl|blr|br|b|cbz|cbnz|ret|cmp|tbz|tbnz|fmov|ldarb)"
LISTING = re.compile(rf"(?:^[ \t]*(?:0x)?71[0-9a-f]{{8}}:?[ \t]+{INSN}\b[^\n]*\n){{4,}}", re.M | re.I)
KEYS = re.compile(r"(?:^[ \t]*[a-z_0-9]+[ \t]*=[ \t]*[0-9a-f]{32,}[ \t]*\n){2,}", re.M | re.I)
# A personal home directory: C:\Users\<name>\..., /c/Users/<name>/..., /home/<name>/..., /Users/<name>/...
# (environment-variable forms like %USERPROFILE% and $HOME, and <placeholders>, are fine).
HOME_PATH = re.compile(r"(?:\b[A-Za-z]:[\\/]+Users[\\/]+|/[a-z]/Users/|(?<![\w.])/home/|(?<![\w.])/Users/)"
                       r"(?!Public\b|Default\b|<|%|\$|\{)[\w.-]+[\\/]", re.I)
# A personal email address (GitHub noreply addresses and placeholder domains are fine).
EMAIL = re.compile(r"\b[\w.+-]+@(?!users\.noreply\.github\.com\b|anthropic\.com\b|example\.(?:com|org)\b)"
                   r"[\w-]+(?:\.[\w-]+)*\.[a-z]{2,}\b", re.I)

# Files that legitimately contain addresses (licenses and vendored code credit their authors).
EMAIL_ALLOWED_PREFIXES = ("lib/",)
EMAIL_ALLOWED: set[str] = {"LICENSE"}


def tracked(base: str | None) -> list[str]:
    if base:
        out = subprocess.run(["git", "diff", "--name-only", "--diff-filter=ACMR", f"{base}...HEAD"],
                             cwd=ROOT, capture_output=True, text=True, check=True).stdout
    else:
        out = subprocess.run(["git", "ls-files"], cwd=ROOT, capture_output=True, text=True, check=True).stdout
    return [line for line in out.splitlines() if line]


def problems(path: str) -> list[str]:
    out = []
    if path.startswith(LOCAL_DIRS):
        out.append("orig/ and build/ are local-only")
    if Path(path).suffix.lower() in FORBIDDEN_EXT:
        out.append("console binary, container or key file type")
    f = ROOT / path
    if not f.is_file():
        return out
    size = f.stat().st_size
    if size > MAX_BYTES:
        out.append(f"{size} bytes > 1 MB limit")
        return out
    data = f.read_bytes()
    if b"\0" in data[:8192]:
        return out  # binary file of an allowed type (an image, say): no text checks
    text = data.decode("utf-8", "replace")
    if LISTING.search(text):
        out.append("looks like an assembly listing")
    if KEYS.search(text):
        out.append("looks like a key file")
    m = HOME_PATH.search(text)
    if m:
        out.append(f"contains a personal home path ({m.group(0)!r}); use %USERPROFILE%, $HOME or a placeholder")
    m = EMAIL.search(text)
    if m and path not in EMAIL_ALLOWED and not path.startswith(EMAIL_ALLOWED_PREFIXES):
        out.append(f"contains an email address ({m.group(0)!r})")
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--base", help="check only files changed since this ref")
    args = ap.parse_args()
    bad = 0
    files = tracked(args.base)
    for path in files:
        for p in problems(path):
            print(f"policy: {path}: {p}")
            bad += 1
    print(f"policy: {len(files)} files checked, {bad} problems")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
