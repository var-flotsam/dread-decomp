#!/usr/bin/env python3
"""Mechanical canonical-header migration of fn_/g_unk_ symbols, verified by tools/baseline.py.

Most of a hub migration (CONTRIBUTING.md, "Migrating a hub to its canonical header") is mechanism:
delete a local prototype, add an #include, rebuild, compare. This does that part for every
hand-written file that declares a symbol locally, and leaves to a person only the files whose
output changes.

For each symbol the canonical header is the one header under include/ that declares it; --header
names it when there is none or several, and --declaration adds the prototype to that header first
(appended; move it into place by hand afterwards). Every other hand-written .cpp or .h that declares
the symbol has the declaration statement removed (the whole statement, a same-line trailing comment
included) and the header included, in sorted position among the file's quoted includes, unless it
already includes it. Before editing, tools/baseline.py records trymatch output for every file to be
touched and for the hand-written includers of every header to be touched; after editing each is
rebuilt and compared. A file whose output changed, or that no longer compiles, is restored from
the baseline and listed with its first differing line (usually the local declaration's types
differ from the header's in a way the call needs, or the file used the symbol's placeholder type
elsewhere), keeping its own declaration next to the header's. If it still differs then, the headers
it includes go back too (a header is all or nothing), and when that is the canonical header's new
declaration, every file migrated onto it goes back with it (printed as UNDONE). Those files are the
next batch's hand work; everything else is done.

  python tools/hub_migrate.py --version 2.1.0 SYMBOL [SYMBOL...] [--header include/x.h [--declaration "..."]]
  python tools/hub_migrate.py --version 2.1.0 --header-redeclared        # every symbol include/ declares once
                                                                        # that another file re-declares
  python tools/hub_migrate.py --version 2.1.0 SYMBOL --dry-run             # show the edits, change nothing
Exit 0 when every file migrated; 1 when some were restored; 2 when a symbol has no usable header.
Nothing is committed; `git diff --stat` shows the result. Standard library only, plus the project tools.
"""

from __future__ import annotations

import argparse
import os
import re
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import baseline  # noqa: E402
import decl_check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
INCLUDE_LINE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*(["<])([^">]+)[">][^\n]*$', re.M)
QUOTED_INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]*"([^"]+)"[^\n]*$', re.M)
PRAGMA_ONCE = re.compile(r"^[ \t]*#[ \t]*pragma[ \t]+once[^\n]*$", re.M)


def read(path: str) -> tuple[str, bool]:
    raw = (ROOT / path).read_text(encoding="utf-8")
    return raw.replace("\r\n", "\n"), "\r\n" in raw


def write(path: str, text: str, crlf: bool) -> None:
    if crlf:
        text = text.replace("\n", "\r\n")
    (ROOT / path).write_text(text, encoding="utf-8", newline="")


def include_spelling(header: str) -> str:
    """How a source includes a header: relative to include/ or to src/ (the project's -I roots)."""
    for root in ("include/", "src/"):
        if header.startswith(root):
            return header[len(root):]
    return header


def remove_statement(text: str, start: int, end: int) -> str:
    """Delete text[start:end] together with the rest of its line (a trailing // comment) and, when
    the statement began its line, the line itself; a blank line left between two blank lines goes."""
    line_start = text.rfind("\n", 0, start) + 1
    line_end = text.find("\n", end)
    line_end = len(text) if line_end < 0 else line_end
    before, tail = text[line_start:start], text[end:line_end]
    if before.strip() or (tail.strip() and not tail.strip().startswith("//")):
        # the statement shares its line with other code: cut the statement (and a comment after it) only
        cut_end = line_end if tail.strip().startswith("//") else end
        return text[:start] + text[cut_end:]
    cut_end = line_end + 1 if line_end < len(text) else line_end
    out = text[:line_start] + text[cut_end:]
    prev_blank = line_start >= 2 and out[line_start - 2:line_start] == "\n\n"
    next_blank = out[line_start:line_start + 1] == "\n"
    if prev_blank and next_blank:  # a blank line on both sides of the removed line: keep one
        out = out[:line_start] + out[line_start + 1:]
    if out.endswith("\n\n"):
        out = out.rstrip("\n") + "\n"
    return out


def has_include(text: str, header: str) -> bool:
    name = Path(header).name
    return any(m.group(1).rsplit("/", 1)[-1] == name for m in QUOTED_INCLUDE.finditer(text))


def add_include(text: str, spelling: str) -> str:
    """Insert `#include "spelling"` into the first block of quoted includes in sorted order; else
    after the last include; else after `#pragma once` or the leading comment block."""
    line = f'#include "{spelling}"'
    quoted = list(QUOTED_INCLUDE.finditer(text))
    if quoted:
        # the first contiguous block of quoted includes
        block = [quoted[0]]
        for m in quoted[1:]:
            between = text[block[-1].end():m.start()]
            if between.strip() == "":
                block.append(m)
            else:
                break
        for m in block:
            if m.group(1) > spelling:
                return text[:m.start()] + line + "\n" + text[m.start():]
        last = block[-1]
        return text[:last.end()] + "\n" + line + text[last.end():]
    anys = list(INCLUDE_LINE.finditer(text))
    if anys:
        last = anys[-1]
        return text[:last.end()] + "\n\n" + line + text[last.end():]
    pragma = PRAGMA_ONCE.search(text)
    if pragma:
        return text[:pragma.end()] + "\n\n" + line + text[pragma.end():]
    # after the leading comment block
    pos = 0
    lines = text.split("\n")
    i = 0
    while i < len(lines) and (lines[i].startswith("//") or lines[i].strip() == "" and i == 0):
        pos += len(lines[i]) + 1
        i += 1
    return text[:pos] + ("\n" if pos and not text[:pos].endswith("\n\n") else "") + line + "\n\n" + text[pos:].lstrip("\n")


def canonical_header(report: decl_check.Report, symbol: str, header: str | None) -> tuple[str | None, str]:
    """(header path, reason) for a symbol; header None when none is usable."""
    headers = [p for p in report.paths.get(symbol, []) if p.startswith("include/")]
    if header:
        return header, "given"
    if not headers:
        return None, "no header under include/ declares it (give --header and --declaration)"
    if len(headers) > 1:
        return None, "several headers declare it: " + " ".join(headers) + " (give --header)"
    return headers[0], "the one header that declares it"


def migrate(symbols: list[str], version: str, header: str | None, declaration: str | None,
            dry_run: bool, jobs: int) -> int:
    report = decl_check.Report(decl_check.tree_files(None))
    worst = 0
    for symbol in symbols:
        hdr, why = canonical_header(report, symbol, header)
        if hdr is None:
            print(f"SKIP {symbol}: {why}")
            worst = max(worst, 2)
            continue
        if hdr not in report.paths.get(symbol, []) and not declaration:
            print(f"SKIP {symbol}: {hdr} does not declare it; give --declaration to add the prototype")
            worst = max(worst, 2)
            continue
        targets = [p for p in report.paths.get(symbol, []) if p != hdr]
        if not targets and hdr in report.paths.get(symbol, []):
            print(f"DONE {symbol}: already declared only in {hdr}")
            continue
        print(f"{symbol}: canonical header {hdr} ({why}); {len(targets)} file(s) to migrate")
        edits: dict[str, tuple[str, bool]] = {}
        touched_headers = [p for p in targets if p.endswith((".h", ".hpp"))]
        if hdr not in report.paths.get(symbol, []):
            text, crlf = read(hdr)
            edits[hdr] = (text.rstrip("\n") + "\n" + declaration.strip() + "\n", crlf)
            touched_headers.append(hdr)
        spelling = include_spelling(hdr)
        for path in targets:
            text, crlf = read(path)
            decls = sorted((d for d in decl_check.scan(text, path) if d.name == symbol),
                           key=lambda d: d.start, reverse=True)
            for d in decls:
                text = remove_statement(text, d.start, d.end)
            if not has_include(text, hdr):
                text = add_include(text, spelling)
            edits[path] = (text, crlf)
            if dry_run:
                print(f"  {path}: remove {len(decls)} declaration(s) at line(s) "
                      + ", ".join(str(d.line) for d in sorted(decls, key=lambda d: d.line))
                      + ("" if has_include(read(path)[0], hdr) else f'; add #include "{spelling}"'))
        if dry_run:
            continue
        baseline._INCLUDE_CACHE = None  # an earlier symbol of this run may have added includes
        verify = sorted(set(baseline.expand(set(targets) | set(touched_headers))))
        tag = f"hub_migrate-{symbol}"
        print(f"  baseline {tag}: {len(verify)} file(s) to rebuild")
        outputs = baseline.save(tag, verify, version, jobs, copies=touched_headers)
        # A file that does not compile before the edit cannot be verified: leave it alone, and leave
        # alone any header one of its includers is (the header edit would be unverifiable too).
        broken = sorted(p for p in verify if baseline.broken(outputs[p]))
        bad_headers = {h for h in touched_headers if any(b in baseline.expand({h}) for b in broken)}
        for p in broken:
            print(f"  BROKEN  {p}  does not compile before the edit: {baseline.summary_line(outputs[p])[:160]}")
        for h in sorted(bad_headers):
            print(f"  SKIP    {h}  an includer does not compile; fix that first")
        for p in broken + sorted(bad_headers):
            edits.pop(p, None)
        targets = [p for p in targets if p not in broken and p not in bad_headers]
        touched_headers = [h for h in touched_headers if h not in bad_headers]
        verify = [p for p in verify if p not in broken]
        if not edits:
            print(f"  {symbol}: nothing migrated")
            worst = max(worst, 1)
            continue
        undo: set[str] = set()
        still: list[str] = []
        moved: list[str] = []
        try:  # every compare and restore inside: an interrupted run leaves the tree as it was
            for path, (text, crlf) in edits.items():
                write(path, text, crlf)
            result = baseline.compare(tag, verify, version, jobs)
            changed = [p for p, (st, _) in result.items() if st != "UNCHANGED"]
            if changed:
                # First the changed files alone go back: with their own declaration next to the
                # header's (the same signature, or an overload) they usually build as before.
                undo = set(changed)
                baseline.restore(tag, sorted(undo))
                again = baseline.compare(tag, sorted(changed), version, jobs)
                still = [p for p, (st, _) in again.items() if st != "UNCHANGED"]
            if still:
                # Then a header with an includer that still differs: a header is all or nothing.
                # Undoing the canonical header's new declaration takes back every file migrated
                # onto it; undoing another header changes what all its includers see (the files
                # restored alone too), so all are compared again and the symbol is given up if
                # any moved.
                bad = {h for h in touched_headers if any(c in baseline.expand({h}) for c in still)}
                undo |= bad
                if hdr in bad:
                    undo |= set(edits)
                baseline.restore(tag, sorted(undo))
                if hdr not in bad:
                    final = baseline.compare(tag, list(verify), version, jobs)
                    moved = sorted(p for p, (st, _) in final.items() if st != "UNCHANGED")
                    if moved:
                        undo |= set(edits)
                        baseline.restore(tag, sorted(undo))
        except BaseException:
            baseline.restore(tag, list(edits))
            raise
        kept = [p for p in targets if p not in undo]
        if changed:
            for p in sorted(changed):
                st, diff = result[p]
                lines = diff.splitlines()[2:]
                first = (next((ln for ln in lines if ln.startswith("+") and "error" in ln), None)
                         or next((ln for ln in lines if ln.startswith("+")), None)
                         or next((ln for ln in lines if ln.startswith("-")), ""))
                d = baseline.BASE / tag / "diff" / (p + ".txt")
                d.parent.mkdir(parents=True, exist_ok=True)
                d.write_text(diff, encoding="utf-8")
                if p in still:
                    print(f"  LEFT  {p}  {st}: {first.strip()[:200]}")
                else:
                    print(f"  LEFT  {p}  keeps its own declaration next to the header's (migrated: {first.strip()[:160]})")
            for p in moved:
                print(f"  MOVED  {p}  changed once a header went back: the symbol is given up")
            for p in sorted((set(targets) & undo) - set(changed)):
                print(f"  UNDONE  {p}  the header edit it relies on went back")
            worst = max(worst, 1)
        for p in kept:
            print(f"  migrated  {p}")
        left = [d for d in report.decls.get(symbol, []) if d.path in set(targets) - set(kept)]
        print(f"  {symbol}: {len(kept)} migrated, {len(targets) - len(kept)} left"
              + (f" ({len(left)} declaration(s) remain)" if left else ""))
    return worst


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("symbols", nargs="*", help="fn_<addr> / g_unk_<addr>")
    ap.add_argument("--version", default="2.1.0")
    ap.add_argument("--header", help="the canonical header (required when none or several declare the symbol)")
    ap.add_argument("--declaration", help="prototype to append to --header when it does not declare the symbol yet")
    ap.add_argument("--header-redeclared", action="store_true",
                    help="migrate every symbol that exactly one header under include/ declares and a file outside it re-declares")
    ap.add_argument("--dry-run", action="store_true", help="print the planned edits and change nothing")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()

    symbols = list(args.symbols)
    if args.header_redeclared:
        report = decl_check.Report(decl_check.tree_files(None))
        for s in report.header_redeclared:
            if len([p for p in report.paths[s] if p.startswith("include/")]) == 1:
                symbols.append(s)
        print(f"{len(symbols)} symbol(s) declared in exactly one header under include/ and re-declared elsewhere")
    if not symbols:
        ap.error("give symbols or --header-redeclared")
    if (args.header or args.declaration) and len(symbols) != 1:
        ap.error("--header/--declaration apply to one symbol")
    return migrate(symbols, args.version, args.header, args.declaration, args.dry_run, args.jobs)


if __name__ == "__main__":
    sys.exit(main())
