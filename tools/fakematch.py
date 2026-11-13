#!/usr/bin/env python3
"""Fake-match patterns in the hand-written sources: byte-identical code that a programmer would not
have written, which usually points at a missing helper, a wrong type or two variables merged into one.

A match proves the bytes; readability is the goal (CLAUDE.md). This lists the source forms that most
often match only by accident of codegen, so a reviewer can look at them. It reports; it changes
nothing. Scanned: src/ without the generated directories (tools/progress.py GENERATED_DIRS).

Kinds:
  offset-cast       a cast to a byte pointer or integer plus a constant, `*(u32*)((char*)p + 0x10)` or
                    `reinterpret_cast<char*>(p) + 8`: a struct field written as arithmetic. Not counted
                    on a global (`g_...`): `(char*)g_unk_<vtable> + 0x10` is how the tree writes an
                    unmodelled vtable's address point
  volatile          `volatile` on a variable or pointer (not `asm volatile`): usually steers codegen
                    rather than marks hardware or shared memory
  empty-block       `if (...) {}`, `else {}`, `switch (...) {}`, `do {} while`: usually a macro or a
                    helper's shape (empty loops are left out: a spin or hang loop the original has)
  barrier           an empty asm barrier; allowed by CLAUDE.md with a comment directly above
  barrier-no-comment  an empty asm barrier with no comment above it or at the end of its line (breaks
                    the CLAUDE.md rule)
  untyped-store     `__builtin_memcpy`/`__builtin_memset`: legitimate for bit casts and block copies,
                    but used for a single field store it often hides a field's wrong type

Each hit is attributed to the function definition it sits in. Hits in a function named fn_<addr>
are also totalled in bytes, so the report gives "matched bytes in functions with a fake-match form".

Usage:
  python tools/fakematch.py                     # counts per kind; exit 0
  python tools/fakematch.py --list [--kind K]   # every hit: file:line, kind, function
  python tools/fakematch.py --base origin/main  # only files changed since <ref> (a branch's or a PR's)
  python tools/fakematch.py --strict            # exit 1 on any barrier-no-comment
  python tools/fakematch.py --unset [PATH...]   # every read of a variable that is not set, by the project
                                                # compiler's own warnings: a text with no behaviour of its own,
                                                # whose byte match proves nothing. Needs the compiler; a minute
  python tools/fakematch.py --unneeded-barriers [--write] [--list] [PATH...]
                                                # compile each file without each barrier: one that changes no
                                                # function's code is not needed (--write removes it; --list also
                                                # shows what each needed one prevents). Needs the dump; a minute
  python tools/fakematch.py PATH...             # only these files or directories
"""

from __future__ import annotations

import argparse
import csv
import re
import subprocess
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(ROOT / "tools"))
from progress import GENERATED_DIRS  # noqa: E402

KINDS = ("offset-cast", "volatile", "empty-block", "barrier", "barrier-no-comment", "untyped-store")
_BYTE_T = r"(?:const\s+)?(?:char|u8|s8|unsigned\s+char|signed\s+char|uintptr_t|uptr|intptr_t|size_t|u64|s64)"
PATTERNS = {
    "offset-cast": re.compile(
        r"\(\s*" + _BYTE_T + r"\s*\*?\s*\)\s*\(?\s*[A-Za-z_][\w.>\-\[\]]*\s*\)?\s*\+\s*(?:0x[0-9a-fA-F]+|\d+)\b"
        r"|reinterpret_cast\s*<\s*" + _BYTE_T + r"\s*\*?\s*>\s*\([^()]*\)\s*\+\s*(?:0x[0-9a-fA-F]+|\d+)\b"),
    "volatile": re.compile(r"(?<!asm )(?<!__asm__ )(?<!asm)\bvolatile\b(?!\s*\()"),
    "empty-block": re.compile(r"\b(?:if|switch)\s*\((?:[^(){}]|\([^(){}]*\))*\)\s*\{\s*\}"
                              r"|\belse\s*\{\s*\}|\bdo\s*\{\s*\}\s*while"),
    "barrier": re.compile(r"\b(?:asm|__asm__)\s*(?:volatile\s*|__volatile__\s*)?\(\s*\"\""),
    "untyped-store": re.compile(r"\b__builtin_mem(?:cpy|set)\s*\("),
}
FUNC_RE = re.compile(r"^(?!(?:struct|class|union|enum|namespace|typedef|using|template|extern\s+\"C\"\s*\{|static_assert"
                     r"|#|//|/\*|\}|return)\b)[^;{}]*?([~\w:]+)\s*\([^;]*$")
_GLOBAL_BASE = re.compile(r"[(>]\s*\(?\s*g_\w*")
_ATTR = re.compile(r"__attribute__\s*\(\((?:[^()]|\([^()]*\))*\)\)\s*")
TYPE_RE = re.compile(r"^(?:template\s*<[^>]*>\s*)?(?:struct|class|union)\s+(?:__attribute__\S*\s+)?(\w+)[^;]*$")
ADDR_RE = re.compile(r"fn_([0-9a-f]{10})\b")


@dataclass
class Hit:
    path: str
    line: int
    kind: str
    function: str | None


def strip_comments(text: str) -> str:
    """Comments and string contents blanked to spaces (line breaks kept), except the empty string
    `""` that an asm barrier needs."""
    out, i, n = [], 0, len(text)
    while i < n:
        c = text[i]
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            out.append(" " * (j - i))
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            out.append(re.sub(r"[^\n]", " ", text[i:j]))
            i = j
        elif c == '"' or c == "'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            out.append(c + " " * max(0, j - i - 2) + (c if j - i >= 2 else ""))
            i = j
        else:
            out.append(c)
            i += 1
    return "".join(out)


def function_starts(code: str) -> list[tuple[int, str]]:
    """(line number, name) of each line at column 0 that starts a function definition, or a struct
    whose members are defined inside it (named `Struct::(member)`)."""
    starts = []
    lines = code.split("\n")
    for k, line in enumerate(lines, 1):
        if not line or line[0].isspace():
            continue
        t = TYPE_RE.match(line)
        if t and "{" in "\n".join(lines[k - 1:k + 2]).split(";")[0]:
            starts.append((k, f"{t.group(1)}::(member)"))  # a member defined inside its struct
            continue
        m = FUNC_RE.match(_ATTR.sub("", line))
        if not m:
            continue
        rest = "\n".join(lines[k - 1:k + 3])  # a definition reaches `{` before any `;`
        brace, semi = rest.find("{"), rest.find(";")
        if brace >= 0 and (semi < 0 or brace < semi):
            starts.append((k, m.group(1)))
    return starts


def scan_text(path: str, text: str) -> list[Hit]:
    text = text.replace("\r\n", "\n")
    code = strip_comments(text)
    raw = text.split("\n")
    starts = function_starts(code)
    line_of = [0]
    for k, ch in enumerate(code):
        if ch == "\n":
            line_of.append(k + 1)

    def lineno(pos: int) -> int:
        lo, hi = 0, len(line_of) - 1
        while lo < hi:
            mid = (lo + hi + 1) // 2
            if line_of[mid] <= pos:
                lo = mid
            else:
                hi = mid - 1
        return lo + 1

    def owner(line: int) -> str | None:
        name = None
        for k, n in starts:
            if k > line:
                break
            name = n
        return name

    hits = []
    for kind, rx in PATTERNS.items():
        for m in rx.finditer(code):
            ln = lineno(m.start())
            if kind == "offset-cast" and _GLOBAL_BASE.search(m.group(0)):
                continue
            if kind == "barrier":
                above = next((raw[k].strip() for k in range(ln - 2, -1, -1) if raw[k].strip()), "")
                if not above.startswith("//") and "//" not in raw[ln - 1]:
                    hits.append(Hit(path, ln, "barrier-no-comment", owner(ln)))
            hits.append(Hit(path, ln, kind, owner(ln)))
    return sorted(hits, key=lambda h: (h.line, h.kind))


def hand_written(paths: list[Path]) -> list[Path]:
    out = []
    for p in paths:
        files = [p] if p.is_file() else sorted(p.rglob("*"))
        for f in files:
            if f.suffix not in (".cpp", ".h", ".hpp", ".c", ".inc"):
                continue
            rel = f.resolve().relative_to(ROOT)
            if rel.parts[0] == "src" and len(rel.parts) > 1 and rel.parts[1] in GENERATED_DIRS:
                continue
            out.append(f)
    return out


def changed_files(base: str) -> list[Path]:
    r = subprocess.run(["git", "diff", "--name-only", "--diff-filter=AM", f"{base}...HEAD"], cwd=ROOT,
                       capture_output=True, text=True, check=True)
    return [ROOT / p for p in r.stdout.split() if p.startswith("src/") and (ROOT / p).is_file()]


def sizes() -> dict[int, tuple[int, str]]:
    out = {}
    for f in sorted((ROOT / "data" / "functions").glob("*.csv")):
        with f.open(encoding="utf-8", newline="") as fh:
            for row in csv.DictReader(fh):
                out[int(row["address"], 16)] = (int(row["size"]), row["status"])
    return out


ALONE = re.compile(r'^\s*(?:asm|__asm__)\s*(?:volatile\s*|__volatile__\s*)?\(\s*""[^;]*\)\s*;\s*(//.*)?$')


def without_barrier(lines: list[str], line: int, commented: bool) -> list[str] | None:
    """The file's lines without the barrier on 1-based `line`, and without its comment when that is a block
    of `//` lines directly above it (a comment at the end of the barrier's own line goes with the line).
    None when the barrier does not stand alone on its line (inside a macro or beside other code)."""
    if not 0 < line <= len(lines) or not ALONE.match(lines[line - 1]):
        return None
    lo = line - 1
    if commented and "//" not in lines[line - 1]:
        while lo > 0 and lines[lo - 1].lstrip().startswith("//"):
            lo -= 1
    return lines[:lo] + lines[line:]


def unneeded_barriers(files: list[Path], version: str, write: bool, jobs: int = 8) -> list[dict]:
    """Try every barrier of these sources: take it out, compile, and compare each function of the file (its
    bytes and what its relocations name) with what it was. [{file, line, result, without}], result being
    `not needed` (nothing changed), `needed` (`without` says what the first changed function becomes) or
    `not tried` (not alone on its line; a header, whose includers would all have to be compared). With write,
    the barriers that are not needed stay out. Needs the dump."""
    from concurrent.futures import ThreadPoolExecutor
    import drafts
    drafts.init(version)
    rows: list[dict] = []
    todo = []
    for f in files:
        rel = f.relative_to(ROOT).as_posix()
        hits = scan_text(rel, f.read_text(encoding="utf-8", errors="replace"))
        bare = {h.line for h in hits if h.kind == "barrier-no-comment"}
        mine = sorted({h.line for h in hits if h.kind == "barrier"}, reverse=True)
        if mine and f.suffix != ".cpp":
            rows += [{"file": rel, "line": n, "result": "not tried", "without": "in a header"} for n in mine]
        elif mine:
            todo.append((f, rel, mine, bare))

    def one(item):
        f, rel, mine, bare = item
        raw = f.read_bytes()
        newline = "\r\n" if b"\r\n" in raw else "\n"
        lines = raw.decode("utf-8").replace("\r\n", "\n").split("\n")
        before = drafts.compiled(drafts.functions(f))
        if isinstance(before, str):
            return [{"file": rel, "line": n, "result": "not tried", "without": "the file does not build"} for n in mine]
        out = []
        for n in mine:  # bottom up: the lines above keep their numbers
            trial = without_barrier(lines, n, n not in bare)
            if trial is None:
                out.append({"file": rel, "line": n, "result": "not tried", "without": "not alone on its line"})
                continue
            f.write_bytes(newline.join(trial).encode("utf-8"))
            found = drafts.functions(f)
            after = drafts.compiled(found)
            if after == before:
                out.append({"file": rel, "line": n, "result": "not needed", "without": ""})
                lines = trial
            elif isinstance(found, str):
                out.append({"file": rel, "line": n, "result": "needed", "without": "does not build"})
            else:
                changed = next(x for x, a, b in zip(found, after, before) if a != b)
                out.append({"file": rel, "line": n, "result": "needed",
                            "without": f"{changed['name']}: {changed['difference']}, {changed['differing']} differing"})
        f.write_bytes(newline.join(lines).encode("utf-8") if write else raw)
        return out
    with ThreadPoolExecutor(jobs) as pool:
        for found in pool.map(one, todo):
            rows += found
    return sorted(rows, key=lambda r: (r["file"], r["line"]))


UNSET_FLAGS = ["-fsyntax-only", "-Wuninitialized", "-Wsometimes-uninitialized"]
UNSET_LINE = re.compile(r":(\d+):\d+: warning: (variable '.*' is (?:used )?uninitialized.*?) \[-W")


def unset_warnings(stderr: str) -> list[tuple[int, str]]:
    """(line, message) for each read of a variable that is not set, from the compiler's diagnostics."""
    return [(int(m.group(1)), m.group(2)) for m in map(UNSET_LINE.search, stderr.splitlines()) if m]


def unset_reads(files: list[Path], jobs: int = 8) -> list[tuple[str, int, str]]:
    """Every read of a variable before it is set that the project compiler sees in these sources (syntax only,
    its -Wuninitialized and -Wsometimes-uninitialized): [(file, line, message)]. Such a text has no behaviour of
    its own, so a byte match of it proves nothing about what the source says: a local left unset and passed to
    a callee to reproduce an argument register the original does not load, a result left unset on one path,
    or a use moved above its assignment. Needs the project compiler; no dump."""
    from concurrent.futures import ThreadPoolExecutor
    import buildcfg

    def one(f: Path):
        cmd = buildcfg.cxx_command(f, ROOT / "build" / "fakematch_unset.o")
        if "-o" in cmd:
            i = cmd.index("-o")
            del cmd[i:i + 2]
        cmd = [c for c in cmd if c != "-c"] + UNSET_FLAGS
        r = subprocess.run(cmd, capture_output=True, text=True)
        return [(f.relative_to(ROOT).as_posix(), n, text) for n, text in unset_warnings(r.stderr)]
    out: list[tuple[str, int, str]] = []
    with ThreadPoolExecutor(jobs) as pool:
        for found in pool.map(one, [f for f in files if f.suffix == ".cpp"]):
            out += found
    return sorted(out)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--unset", action="store_true",
                    help="list every read of a variable that is not set (the project compiler's own warnings)")
    ap.add_argument("--unneeded-barriers", action="store_true",
                    help="compile each file without each of its barriers and list the ones that change nothing")
    ap.add_argument("--write", action="store_true", help="with --unneeded-barriers: remove them")
    ap.add_argument("--version", default="2.1.0", help="with --unneeded-barriers: the dump to compare with")
    ap.add_argument("paths", nargs="*", type=Path)
    ap.add_argument("--base", help="only files added or changed since this ref")
    ap.add_argument("--list", action="store_true", help="every hit")
    ap.add_argument("--kind", choices=KINDS, help="with --list: only this kind")
    ap.add_argument("--strict", action="store_true", help="exit 1 on a barrier without its comment")
    args = ap.parse_args()

    files = changed_files(args.base) if args.base else [p.resolve() for p in args.paths] or [ROOT / "src"]
    files = hand_written(files)
    if args.unset:
        found = unset_reads(files)
        for path, line, text in found:
            print(f"{path}:{line}  {text}")
        print(f"{len(files)} files: {len(found)} reads of a variable that is not set, in "
              f"{len({p for p, _, _ in found})} files")
        return 0
    if args.unneeded_barriers:
        rows = unneeded_barriers(files, args.version, args.write)
        for r in rows:
            if r["result"] != "needed" or args.list:
                print(f"{r['file']}:{r['line']}  {r['result']:10}  {r['without']}")
        n = Counter(r["result"] for r in rows)
        print(f"{len(rows)} barriers: {n['not needed']} not needed" + (" (removed)" if args.write and n["not needed"] else "")
              + f", {n['needed']} needed, {n['not tried']} not tried")
        return 0
    hits = []
    for f in files:
        hits += scan_text(f.relative_to(ROOT).as_posix(), f.read_text(encoding="utf-8", errors="replace"))

    if args.list:
        for h in hits:
            if args.kind in (None, h.kind):
                print(f"{h.path}:{h.line}  {h.kind:18}  {h.function or '?'}")
        return 1 if args.strict and any(h.kind == "barrier-no-comment" for h in hits) else 0

    table = sizes()
    by_kind: dict[str, Counter] = defaultdict(Counter)
    funcs: dict[str, set] = defaultdict(set)
    flagged: set[int] = set()
    for h in hits:
        by_kind[h.kind]["hits"] += 1
        funcs[h.kind].add((h.path, h.function))
        m = ADDR_RE.search(h.function or "")
        if m and h.kind not in ("barrier", "untyped-store"):
            flagged.add(int(m.group(1), 16))
    print(f"{len(files)} hand-written files scanned")
    print("| Kind | Hits | Functions | Files |")
    print("|---|---|---|---|")
    for k in KINDS:
        if by_kind[k]["hits"]:
            print(f"| {k} | {by_kind[k]['hits']:,} | {len(funcs[k]):,} | {len({p for p, _ in funcs[k]}):,} |")
    matched = [a for a in flagged if table.get(a, (0, ""))[1] in ("O", "m")]
    print(f"matched functions (O, m) named fn_<addr> with a fake-match form (not counting comment-carrying "
          f"barriers or untyped stores): {len(matched):,}, {sum(table[a][0] for a in matched):,} bytes")
    return 1 if args.strict and by_kind["barrier-no-comment"]["hits"] else 0


if __name__ == "__main__":
    sys.exit(main())
