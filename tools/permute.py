#!/usr/bin/env python3
"""A cheap permuter: turn a registers-only near-match into a byte match by reordering its source.

A function whose only difference from the original is register choice or instruction order (tools/trymatch.py:
`KIND registers`) is often one statement or one operand away from matching. This tries that for free: it
changes one thing in the function's body, compiles with the project compiler, counts the instructions that
differ from the original, keeps a change that does not make it worse, and stops at a byte match whose
relocations name the original's targets. The changes it tries:

  swap      two adjacent statements
  move      a statement two or three lines up or down in its block
  split     `T x = e;` into `T x;` and `x = e;`, and the reverse (merge)
  commute   the operands of + * & | ^ == !=
  compare   `a < b` as `b > a` (and <=, >, >=)

A change need not keep the behaviour: one that does not cannot match, and a match is its own proof. What it
must keep is the source readable (CLAUDE.md: readability outranks a match), so a match goes through two more
passes before it is shown: every changed piece whose reversal still matches is put back (the search accepts
changes that do nothing, such as `2 == kind`), and a declaration it split from its initialiser is merged again
where the match survives. What is left is the original text with a line or three moved or two operands
swapped. Look at it before keeping it all the same.

It is not a general permuter and is not meant to grow into one. On 2026-11-12 it matched 27 of 248
registers-only functions (11%; docs/JOURNAL.md), all of them one to three lines from the original, in about an
hour of CPU and no model tokens. Use it before another model attempt on a near-match, not instead of one.

Usage:
  python tools/permute.py --version 2.1.0 FILE.cpp [...]      # every registers-only function of these files
  python tools/permute.py --version 2.1.0 FILE.cpp:ADDR       # one function
  python tools/permute.py --version 2.1.0 --minor             # every NON_MATCHING(minor) file under src/
  python tools/permute.py --version 2.1.0 --drafts            # the `registers or order` rows of drafts/INDEX.csv
  ... --iterations N   # tries per function (default 1500, about a tenth of a second each)
  ... --jobs N         # functions in parallel (default 8)
  ... --write          # put each match into its file: the body, without its NON_MATCHING note; a row of
                       # data/equivalence.csv for it goes too (a match needs no verdict)
Without --write the matching text of each function is left in build/permute/<addr>.cpp and nothing tracked
changes. Needs the dump. After --write, run tools/check.py as after any change of src/.
"""

from __future__ import annotations

import argparse
import collections
import csv
import difflib
import itertools
import json
import multiprocessing
import random
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

OUT = ROOT / "build" / "permute"
STMT = re.compile(r"^\s+[^\s/{}#][^{}]*;\s*(//.*)?$")
DECL_INIT = re.compile(r"^(\s+)((?:const\s+)?[A-Za-z_][\w:<>]*(?:\s*[\*&])*\s+\**&?)([A-Za-z_]\w*)\s*=\s*(.+);\s*$")
DECL_BARE = re.compile(r"^(\s+)([A-Za-z_][\w:<>]*(?:\s*\*)*\s+\**)([A-Za-z_]\w*);\s*$")
OPERAND = r"[A-Za-z_][\w]*(?:(?:->|\.)\w+|\[\w+\])*|\d+[uUlLfF.]*"
COMMUTE = re.compile(rf"({OPERAND}) (\+|\*|&|\||\^|==|!=) ({OPERAND})(?![\w(\[.])")
COMPARE = re.compile(rf"({OPERAND}) (<=|>=|<|>) ({OPERAND})(?![\w(\[.<>])")
FLIP = {"<": ">", ">": "<", "<=": ">=", ">=": "<="}
NOT_MOVED = ("return", "break", "continue", "goto", "case", "default", "else", "delete", "throw", "using", "typedef",
             "for", "if", "while", "do", "switch")
KINDS = ("swap", "swap", "move", "split", "merge", "commute", "commute", "compare")


# --- the text side (no compiler: tools/tests/test_permute.py) ----------------------------------------

def definition_line(lines: list[str], addr: str) -> int | None:
    """The index of the line that starts the definition of fn_<addr> (not a declaration or a call)."""
    pat = re.compile(rf"^[^\s/#][^;]*\bfn_{addr}\s*\(")
    for i, line in enumerate(lines):
        if not pat.match(line):
            continue
        for j in range(i, len(lines)):
            if "{" in lines[j]:
                return i
            if lines[j].rstrip().endswith(";"):
                break
    return None


def body_span(lines: list[str], addr: str) -> tuple[int, int] | None:
    """(first, last) line indices inside the braces of fn_<addr>'s definition, or None."""
    i = definition_line(lines, addr)
    if i is None:
        return None
    depth, opened = 0, False
    for j in range(i, len(lines)):
        for ch in lines[j]:
            if ch == "{":
                depth += 1
                opened = True
            elif ch == "}":
                depth -= 1
        if opened and depth == 0:
            return (i + 1, j - 1) if j - 1 >= i + 1 else None
    return None


def indent(line: str) -> int:
    return len(line) - len(line.lstrip())


def simple(line: str) -> bool:
    """A statement on one line that may be moved: not a control statement, a label or a brace."""
    return bool(STMT.match(line)) and not line.strip().startswith(NOT_MOVED)


def mutate(lines: list[str], lo: int, hi: int, rnd: random.Random, kind: str | None = None) -> list[str] | None:
    """A copy of `lines` with one change inside lines[lo..hi], or None when the picked change has no place."""
    out = list(lines)
    kind = kind or rnd.choice(KINDS)
    cand = [i for i in range(lo, hi + 1) if simple(out[i])]
    if not cand:
        return None
    if kind == "swap":
        pairs = [i for i in cand if i + 1 <= hi and simple(out[i + 1]) and indent(out[i]) == indent(out[i + 1])]
        if not pairs:
            return None
        i = rnd.choice(pairs)
        out[i], out[i + 1] = out[i + 1], out[i]
    elif kind == "move":
        i = rnd.choice(cand)
        j = i + rnd.choice((-3, -2, 2, 3))
        if not lo <= j <= hi or not all(simple(out[k]) and indent(out[k]) == indent(out[i])
                                        for k in range(min(i, j), max(i, j) + 1)):
            return None
        out.insert(j, out.pop(i))
    elif kind == "split":
        ds = [(i, m) for i in cand for m in [DECL_INIT.match(out[i])]
              if m and "const " not in out[i] and "&" not in m.group(2)]
        if not ds:
            return None
        i, m = rnd.choice(ds)
        out[i:i + 1] = [f"{m.group(1)}{m.group(2)}{m.group(3)};", f"{m.group(1)}{m.group(3)} = {m.group(4)};"]
    elif kind == "merge":
        ms = [(i, m) for i in cand for m in [DECL_BARE.match(out[i])]
              if m and i + 1 <= hi and re.match(rf"^\s+{m.group(3)} = .+;\s*$", out[i + 1])]
        if not ms:
            return None
        i, m = rnd.choice(ms)
        out[i:i + 2] = [f"{m.group(1)}{m.group(2)}{out[i + 1].strip()}"]
    else:
        pat = COMMUTE if kind == "commute" else COMPARE
        hits = [(i, m) for i in range(lo, hi + 1) for m in pat.finditer(out[i]) if "//" not in out[i][:m.start()]]
        if not hits:
            return None
        i, m = rnd.choice(hits)
        op = m.group(2) if kind == "commute" else FLIP[m.group(2)]
        out[i] = out[i][:m.start()] + f"{m.group(3)} {op} {m.group(1)}" + out[i][m.end():]
    return out


def search(lines: list[str], addr: str, cost, iterations: int, rnd: random.Random) -> dict:
    """Hill climbing over mutate(). `cost(lines)` returns (number, matched) or (None, False) for text that does
    not build. Returns {start, best, matched, tries, built, lines} (lines: the match, else the best seen)."""
    span = body_span(lines, addr)
    start, matched = cost(lines)
    row = {"start": start, "best": start, "matched": matched, "tries": 0, "built": 0, "lines": lines}
    if span is None or start is None or matched:
        row["note"] = "no body found" if span is None else "does not build" if start is None else "already matches"
        return row
    lo, hi = span
    cur, cur_cost = lines, start
    best, best_cost = lines, start
    for n in range(1, iterations + 1):
        row["tries"] = n
        new = mutate(cur, lo, hi, rnd)
        if new is None:
            continue
        c, ok = cost(new)
        if c is None:
            continue
        row["built"] += 1
        if c <= cur_cost:
            hi += len(new) - len(cur)
            cur, cur_cost = new, c
            if c < best_cost:
                best, best_cost = new, c
        if ok:
            row.update(matched=True, best=0, lines=new)
            return row
        if n % 60 == 0 and cur_cost > best_cost:
            cur, cur_cost = best, best_cost
            hi = body_span(cur, addr)[1]
    row.update(best=best_cost, lines=best)
    return row


def minimise(orig: list[str], match: list[str], matches) -> list[str]:
    """`match` with every changed piece put back to `orig`'s whose reversal still satisfies `matches(lines)`.

    A moved line is two pieces to a diff, gone from one place and new in another. Putting back only one of them
    writes the line twice or not at all, and the compiler may not mind (a store repeated is a dead store), so a
    trial is only tried when no line is in it more often, or less often, than in both `orig` and `match`."""
    have, want = collections.Counter(orig), collections.Counter(match)

    def whole(trial: list[str]) -> bool:
        n = collections.Counter(trial)
        return all(min(have[x], want[x]) <= n[x] <= max(have[x], want[x]) for x in set(n) | set(have) | set(want))
    cur = list(match)
    while True:
        ops = [o for o in difflib.SequenceMatcher(None, orig, cur, autojunk=False).get_opcodes() if o[0] != "equal"]
        def put_back(hunks) -> list[str]:
            out = list(cur)
            for _, i1, i2, j1, j2 in sorted(hunks, key=lambda o: -o[3]):
                out[j1:j2] = orig[i1:i2]
            return out
        # one piece at a time, then two together: a move is one piece gone and one piece new
        trials = [put_back([o]) for o in ops] + [put_back(pair) for pair in itertools.combinations(ops, 2)]
        # neighbouring changes come as one piece, so also line by line: a line the original does not have,
        # against the line of the original it was made from (the closest one that is now missing)
        missing = [line for line in dict.fromkeys(orig) if orig.count(line) > cur.count(line)]
        new = [j for j, line in enumerate(cur) if line not in orig]
        for j in new:
            close = difflib.get_close_matches(cur[j], missing, n=1, cutoff=0.6)
            if close:
                trials.append(cur[:j] + close + cur[j + 1:])
        # and last, without it: a line that does nothing any more (`x = e;` left behind `T x = e;`)
        trials += [cur[:j] + cur[j + 1:] for j in new]
        for trial in trials:
            if whole(trial) and matches(trial):
                cur = trial
                break
        else:
            return cur


def merge_declarations(lines: list[str], addr: str, matches) -> list[str]:
    """Every `T x;` of the body whose first later use is `x = e;` becomes `T x = e;` there, where the result
    still satisfies `matches(lines)`."""
    changed = True
    while changed:
        changed = False
        span = body_span(lines, addr)
        if span is None:
            break
        lo, hi = span
        for i in range(lo, hi + 1):
            m = DECL_BARE.match(lines[i])
            if not m:
                continue
            name = m.group(3)
            j = next((k for k in range(i + 1, hi + 1) if re.match(rf"^\s+{name} = .+;\s*$", lines[k])), None)
            if j is None or any(re.search(rf"\b{name}\b", lines[k]) for k in range(i + 1, j)):
                continue
            pad = lines[j][:indent(lines[j])]
            trial = lines[:i] + lines[i + 1:j] + [f"{pad}{m.group(2)}{lines[j].strip()}"] + lines[j + 1:]
            if matches(trial):
                lines, changed = trial, True
                break
    return lines


def put_back(real: list[str], match: list[str], addr: str) -> tuple[list[str], int]:
    """`real` (the file's lines) with fn_<addr>'s body replaced by the one in `match`, and the function's
    NON_MATCHING note removed: (lines, note lines removed)."""
    a, b = body_span(real, addr), body_span(match, addr)
    if a is None or b is None:
        raise ValueError(f"fn_{addr}: no body found")
    new = real[:a[0]] + match[b[0]:b[1] + 1] + real[a[1] + 1:]
    d = definition_line(new, addr)
    k = d - 1
    while k >= 0 and new[k].lstrip().startswith("//"):
        k -= 1
    note = next((i for i in range(k + 1, d) if "NON_MATCHING(" in new[i]), None)
    if note is None:
        return new, 0
    removed = d - note
    del new[note:d]
    return new, removed


# --- the compiler side --------------------------------------------------------------------------------

_state: dict = {}


def _init(version: str) -> None:
    import asmdiff
    import check
    _state["asmdiff"], _state["check"] = asmdiff, check
    _state["orig"] = asmdiff.Original(version)
    rows, _ = check.load_csv()
    _state["by_name"] = check.csv_by_name(rows)


def prepared(src: Path) -> list[str]:
    """The file's lines with includes relative to its own folder made absolute, so that a copy in the work
    folder builds."""
    text = src.read_text(encoding="utf-8").replace("\r\n", "\n")
    text = re.sub(r'#include "(\.\./[^"]+|[^"/]+\.h)"',
                  lambda m: f'#include "{(src.parent / m.group(1)).resolve().as_posix()}"'
                  if (src.parent / m.group(1)).exists() else m.group(0), text)
    return text.split("\n")


def unprepared(lines: list[str], src: Path) -> list[str]:
    """prepared()'s includes written as the file has them."""
    real = src.read_text(encoding="utf-8").replace("\r\n", "\n").split("\n")
    back = {p: r for p, r in zip(prepared(src), real) if p != r}
    return [back.get(line, line) for line in lines]


def compiled_cost(lines: list[str], work: Path, addr: str) -> tuple[int | None, bool]:
    """(differing instructions plus a penalty for another size, matched) for fn_<addr> of this text; matched
    needs the same bytes and no relocation that names another target. (None, False) when it does not build."""
    import buildcfg
    asmdiff, check = _state["asmdiff"], _state["check"]
    src, obj = work / "t.cpp", work / "t.o"
    src.write_text("\n".join(lines), encoding="utf-8", newline="\n")
    if subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True).returncode != 0:
        return None, False
    try:
        funcs = asmdiff.read_object(obj)
        results, _, _, targets = check.diff_with_targets(funcs, _state["orig"], _state["by_name"])
    except Exception:  # an object the differ cannot read: as a text that does not build
        return None, False
    for res in results:
        if res.address == int(addr, 16):
            if res.bytes_match:
                bad = asmdiff.check_relocs(res, _state["orig"], targets).mismatches
                return (0, True) if not bad else (1, False)
            return len(res.diff_insns) + abs(res.orig_size - res.our_size) // 2, False
    return None, False


def registers_only(src: Path) -> list[str]:
    """The addresses (hex, no 0x) of the functions of a file whose difference from the original is registers
    only; [] when the file does not build."""
    import buildcfg
    asmdiff, check = _state["asmdiff"], _state["check"]
    obj = OUT / "probe" / (src.stem + ".o")
    obj.parent.mkdir(parents=True, exist_ok=True)
    if subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True).returncode != 0:
        return []
    funcs = asmdiff.read_object(obj)
    codes = {f.symbol: f.code for f in funcs}
    results, _, _, _ = check.diff_with_targets(funcs, _state["orig"], _state["by_name"])
    return [f"{r.address:x}" for r in results if not r.bytes_match
            and asmdiff.classify(_state["orig"].function(r.address), codes[r.symbol]).startswith("registers")]


def run(job: tuple) -> dict:
    version, path, addr, iterations, seed = job
    if "orig" not in _state:
        _init(version)
    src = ROOT / path
    work = OUT / "work" / addr
    work.mkdir(parents=True, exist_ok=True)
    t0 = time.time()
    orig = prepared(src)

    def cost(lines):
        return compiled_cost(lines, work, addr)

    def matches(lines):
        return cost(lines)[1]
    row = search(orig, addr, cost, iterations, random.Random(seed))
    lines = row.pop("lines")
    if row["matched"] and not row.get("note"):
        lines = merge_declarations(minimise(orig, lines, matches), addr, matches)
        assert matches(lines), addr
        (OUT / f"{addr}.cpp").write_text("\n".join(unprepared(lines, src)), encoding="utf-8", newline="\n")
        row["changes"] = [d for d in difflib.unified_diff(orig, lines, lineterm="", n=0)
                          if d[:1] in "+-" and d[:3] not in ("+++", "---")]
    return {"file": path, "address": addr, "seconds": round(time.time() - t0, 1), **row}


def write_back(rows: list[dict]) -> None:
    done = []
    for r in rows:
        if not r["matched"] or r.get("note"):
            continue
        p = ROOT / r["file"]
        raw = p.read_bytes()
        crlf = b"\r\n" in raw
        real = raw.decode("utf-8").replace("\r\n", "\n").split("\n")
        match = (OUT / f"{r['address']}.cpp").read_text(encoding="utf-8").split("\n")
        new, removed = put_back(real, match, r["address"])
        p.write_bytes(("\r\n" if crlf else "\n").join(new).encode("utf-8"))
        done.append(r["address"])
        print(f"written  {r['address']}  {r['file']}  ({removed} note lines removed)")
    table = ROOT / "data" / "equivalence.csv"
    if done and table.is_file():
        lines = table.read_text(encoding="utf-8").splitlines()
        keep = [line for line in lines if not line.startswith(tuple(f"0x{a}," for a in done))]
        if len(keep) != len(lines):
            table.write_text("\n".join(keep) + "\n", encoding="utf-8", newline="\n")
            print(f"{len(lines) - len(keep)} rows of data/equivalence.csv removed (a match needs no verdict)")


def targets(args) -> list[tuple[str, str | None]]:
    """[(file relative to the repository, address or None for every registers-only function of the file)]."""
    out: list[tuple[str, str | None]] = []
    if args.drafts:
        with (ROOT / "drafts" / "INDEX.csv").open(newline="", encoding="utf-8") as fh:
            out += [(r["file"], r["address"][2:]) for r in csv.DictReader(fh) if r["difference"] == "registers or order"]
    if args.minor:
        r = subprocess.run(["git", "grep", "-l", "NON_MATCHING(minor)", "--", "src"], cwd=ROOT, capture_output=True,
                           text=True)
        out += [(p, None) for p in r.stdout.split() if p.endswith(".cpp")]
    for t in args.files:
        m = re.fullmatch(r"(.+\.cpp):(?:0x)?([0-9a-fA-F]{10})", t)
        path = Path(m.group(1) if m else t).resolve()
        out.append((path.relative_to(ROOT).as_posix(), m.group(2).lower() if m else None))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("files", nargs="*", help="FILE.cpp, or FILE.cpp:ADDR for one function")
    ap.add_argument("--minor", action="store_true", help="every NON_MATCHING(minor) file under src/")
    ap.add_argument("--drafts", action="store_true", help="the `registers or order` rows of drafts/INDEX.csv")
    ap.add_argument("--iterations", type=int, default=1500, help="tries per function (default 1500)")
    ap.add_argument("--jobs", type=int, default=8, help="functions in parallel (default 8)")
    ap.add_argument("--seed", type=int, default=1112)
    ap.add_argument("--write", action="store_true", help="put each match into its file")
    args = ap.parse_args()
    picked = targets(args)
    if not picked:
        ap.error("no files: name some, or --minor, or --drafts")
    _init(args.version)
    OUT.mkdir(parents=True, exist_ok=True)
    jobs = []
    for path, addr in picked:
        for a in ([addr] if addr else registers_only(ROOT / path)):
            jobs.append((args.version, path, a, args.iterations, args.seed + len(jobs)))
    print(f"{len(picked)} files, {len(jobs)} functions to try, {args.iterations} tries each", flush=True)
    t0 = time.time()
    rows = []
    with multiprocessing.Pool(args.jobs) as pool:
        for row in pool.imap_unordered(run, jobs):
            rows.append(row)
            state = "MATCH" if row["matched"] and not row.get("note") else row.get("note", "")
            print(f"{state:15} {row['address']}  {row['start']} -> {row['best']}  tries {row['tries']}  "
                  f"{row['seconds']} s  {row['file']}", flush=True)
            for line in row.get("changes", []):
                print(f"    {line[:150]}")
    (OUT / "results.json").write_text(json.dumps(rows, indent=1), encoding="utf-8")
    tried = [r for r in rows if not r.get("note")]
    won = [r for r in tried if r["matched"]]
    closer = [r for r in tried if not r["matched"] and r["best"] is not None and r["best"] < r["start"]]
    print(f"permute: {len(tried)} functions tried, {len(won)} matched, {len(closer)} closer but not matched, "
          f"{len(rows) - len(tried)} skipped; {time.time() - t0:.0f} s; matching text in build/permute/<addr>.cpp")
    if args.write:
        write_back(rows)
    return 0


if __name__ == "__main__":
    sys.exit(main())
