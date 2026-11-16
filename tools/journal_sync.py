#!/usr/bin/env python3
"""Add the descriptions of merged pull requests to docs/JOURNAL.md.

A pull request's description is its journal entry: the "What this changes" section of the template.
Branches never edit docs/JOURNAL.md, so it cannot conflict between open pull requests (owner,
2026-11-16). The session that merges a round runs this with the docs/HANDOFF.md refresh, in the same
commit: it reads the merges on main's first-parent line after the marker in the journal, fetches each
pull request's description from GitHub, puts the entries at the top of the "Session log" (newest
first) and moves the marker to the last merge it read.

A merge whose pull request edited docs/JOURNAL.md itself (one opened before 2026-11-16) is skipped: its
entry is already there. The findings index is still kept by hand: the merging session adds a line for
each reusable finding the new entries record.

GitHub token: $GH_TOKEN or $GITHUB_TOKEN, else `gh auth token`, else none (the repository is public;
GitHub allows 60 unauthenticated requests an hour).

Usage: python tools/journal_sync.py                    # add the entries, move the marker
       python tools/journal_sync.py --dry-run          # print the entries, change nothing
       python tools/journal_sync.py --ref origin/main  # read merges up to REF (default origin/main)
"""

from __future__ import annotations

import argparse
import json
import os
import re
import subprocess
import sys
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
JOURNAL = ROOT / "docs" / "JOURNAL.md"
REPO = "var-flotsam/dread-decomp"
MARKER = re.compile(r"<!-- journal_sync: through ([0-9a-f]{40}) -->")
MERGE_SUBJECT = re.compile(r"^Merge pull request #(\d+) ")
SECTION = "what this changes"


def git(*args: str) -> str:
    return subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, check=True).stdout


def merges_after(last: str, ref: str) -> list[tuple[str, int, str]]:
    """(merge commit, pull request number, date) of each pull request merged on ref's first-parent line
    after `last`, oldest first."""
    out = []
    for line in git("log", "--first-parent", "--reverse", "--format=%H%x09%cs%x09%s", f"{last}..{ref}").splitlines():
        sha, date, subject = line.split("\t", 2)
        m = MERGE_SUBJECT.match(subject)
        if m:
            out.append((sha, int(m.group(1)), date))
    return out


def touched_journal(merge: str) -> bool:
    """Whether the merged pull request changed docs/JOURNAL.md (it then wrote its own entry)."""
    return "docs/JOURNAL.md" in git("diff", "--name-only", f"{merge}^1", merge).split()


def token() -> str | None:
    for name in ("GH_TOKEN", "GITHUB_TOKEN"):
        if os.environ.get(name):
            return os.environ[name]
    try:
        t = subprocess.run(["gh", "auth", "token"], capture_output=True, text=True, timeout=20).stdout.strip()
        return t or None
    except (OSError, subprocess.SubprocessError):
        return None


def fetch_pull(number: int, auth: str | None) -> dict:
    req = urllib.request.Request(f"https://api.github.com/repos/{REPO}/pulls/{number}",
                                 headers={"Accept": "application/vnd.github+json", "User-Agent": "journal_sync"})
    if auth:
        req.add_header("Authorization", f"Bearer {auth}")
    with urllib.request.urlopen(req, timeout=30) as r:
        return json.load(r)


def description(body: str | None) -> str:
    """The "What this changes" section of a pull request body, without HTML comments; the whole body
    (comments, the attribution line and a trailing checklist taken out) when it has no such section."""
    text = re.sub(r"<!--.*?-->", "", (body or "").replace("\r\n", "\n"), flags=re.S)
    lines = text.split("\n")
    start = next((i for i, l in enumerate(lines) if l.strip().lstrip("#").strip().lower() == SECTION
                  and l.lstrip().startswith("#")), None)
    if start is not None:
        end = next((i for i in range(start + 1, len(lines)) if re.match(r"\s*#{1,2}\s", lines[i])), len(lines))
        lines = lines[start + 1:end]
    else:
        lines = [l for l in lines if not l.startswith("_Requested by ")]
        cut = next((i for i, l in enumerate(lines) if re.match(r"\s*#{1,6}\s*checklist\s*$", l, re.I)), len(lines))
        lines = lines[:cut]
    # Footers: the generated-with line and the session link.
    lines = [l for l in lines if not re.match(r"\s*(🤖 Generated with|https://claude\.ai/code/session_)", l)]
    return "\n".join(lines).strip()


def entry(number: int, title: str, date: str, body: str | None) -> str:
    text = description(body)
    head = f"**{title.strip().rstrip('.')} (#{number}, {date}).**"
    if not text:
        return head
    if "\n" in text:  # paragraphs or a list: under the heading line
        return f"{head}\n\n{text}"
    return f"{head} {text}"


def insert(journal: str, entries: list[str], through: str) -> str:
    """The journal with `entries` (newest first) at the top of the session log and the marker at `through`."""
    marker = f"<!-- journal_sync: through {through} -->"
    if not MARKER.search(journal):
        raise SystemExit("journal_sync: no '<!-- journal_sync: through <sha> -->' marker in docs/JOURNAL.md")
    journal = MARKER.sub(marker, journal, count=1)
    anchor = marker + "\n\n"
    return journal.replace(anchor, anchor + "".join(e + "\n\n" for e in entries), 1)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--ref", default="origin/main", help="read merges up to this commit (default origin/main)")
    ap.add_argument("--dry-run", action="store_true", help="print the entries; change nothing")
    args = ap.parse_args()

    journal = JOURNAL.read_text(encoding="utf-8")
    m = MARKER.search(journal)
    if not m:
        raise SystemExit("journal_sync: no '<!-- journal_sync: through <sha> -->' marker in docs/JOURNAL.md")
    merges = merges_after(m.group(1), args.ref)
    if not merges:
        print("journal_sync: no pull request merged since the marker")
        return 0
    auth = token()
    entries = []
    for sha, number, date in merges:
        if touched_journal(sha):
            print(f"#{number}: skipped (it edited docs/JOURNAL.md itself)")
            continue
        pr = fetch_pull(number, auth)
        entries.append(entry(number, pr.get("title", ""), date, pr.get("body")))
        print(f"#{number}: {pr.get('title', '')}")
    entries.reverse()  # newest first
    if args.dry_run:
        print("\n\n".join(entries))
        return 0
    JOURNAL.write_text(insert(journal, entries, merges[-1][0]), encoding="utf-8")
    print(f"journal_sync: {len(entries)} entries added; marker at {merges[-1][0][:12]}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
