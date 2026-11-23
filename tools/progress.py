#!/usr/bin/env python3
"""Whole-binary decompilation progress, from the status column of data/functions/.

Library functions (status L: nn::atk, SDK, runtime, Lua standard library) are excluded
from the totals, like BotW's "library function" status. Reports function counts and
code bytes by status, overall and per subsystem (inferred '?' subsystems are folded in).

objdiff's report (tools/objdiff_gen.py, then `objdiff-cli report generate`) covers only
the units in src/; this report covers the whole of main.

A slice adds the same status table for one part of the call graph (tools/callgraph.py, from
the Ghidra export, so it needs build/ghidra_export/<version>/): `--slice boot` is everything
reachable from nnMain, `--slice-from NAME|ADDR` everything reachable from that function. The
graph has direct calls only, so a slice is a lower bound; the output says so. Without a slice
the output is unchanged (docs quote it).

`--handoff` rewrites the status table between the `progress:begin` and `progress:end` markers of
docs/HANDOFF.md, so the page's numbers are generated, never typed. Without a slice it replaces only
the whole-binary row and keeps the boot row it finds there.

"Done" is the second figure of CLAUDE.md's completion: matching, or `m` with a passing verdict of
tools/equiv_test.py in data/equivalence.csv (tools/check.py fails a verdict that is not about the code in
the tree any more). It is printed beside "matching" and written under the handoff's table.

`--origin` splits the matched (`O`, `m`) functions and bytes by where their source lives: hand-typed
(`src/` outside the generated directories) or generated (reflection, clones, tables, byte-identical
lifts). Hand-typed, readable code is the project's completion metric (CLAUDE.md, "Purpose"); the
generated share verifies correctness but does not count toward it. It reads check.py's diff cache
(build/check/<version>/**/*.o.diff.json, the object that defines each function), so run check.py first.

`--quality` reports the qualities each function needs, apart (owner, 2026-11-13): the bytes match, and the
source is clean in form. Its headline (owner, 2026-11-23) is *matching and clean in form*: a function that
byte-matches, has no steering form and is hand-written (the row "byte-matching, hand-written, no steering form":
none of tools/fakematch.py's steering forms in the function), or comes from a generated family whose sampled read
in data/family_reads.csv passed ("generated, read and passed"). "Complete" keeps its strict meaning (matching,
readable, real names); the headline does not claim it. Identical behaviour with other bytes (verified equivalent)
has a row of its own: acceptable now, a match later. The same figures go under the handoff's table with --handoff
when check.py's cache is there, the headline first.

Names are a second figure beside the headline, not a cut to it (reporting only; no figure above changes). Of the
byte-matching, hand-written functions with no steering form: those whose names are *loose* (its own name, class and
members real; references to placeholder functions allowed) and *strict* (no placeholder anywhere), tested on each
function's own text, by file where the tool does not find its definition. *Named* is the loose ones whose own name
the function list records: from a source (`name_source` rtti, reflection, string, symbol, library) or inferred
(`inferred`, through data/names.csv), the two parts shown apart (owner, 2026-11-23: inferred names count as named,
as their own tier). A function whose source spells a real name while its row in data/functions/ has none (a
placeholder name, `name_source` unknown) is shown apart and not counted as named: a name counts once the list
records it with its source (tools/rename.py). Per generated directory, the matching functions whose file has no
placeholder ("clean, not yet read"), and those of them a family's passing sampled read covers ("read and passed").
A family's latest row decides, pass or fail; `scope` is a path glob relative to the repository or a list file of
globs and addresses. The names line goes under the handoff's table with --handoff. The name and placeholder scans
are a floor a tool can see, not a reading.

A function marked `// NON_MATCHING(functional)` with a current `behaves` verdict of tools/equiv_test.py
--any-kind (tier 3, owner 2026-11-19) has a row of its own in --quality: the same results on every input tried,
not proven, not counted as done.

`--fuzzy` (owner, 2026-11-19) adds partial credit: the game's bytes weighted by how close each function is,
1 for `O` and for verified equivalent, a draft by its similarity in drafts/INDEX.csv, a non-matching source
in src/ by its diff score in check.py's cache (the higher of the two), 0 for nothing written. It also counts
the drafts at 95% similarity or more. A figure of closeness, never of completion; it goes under the handoff's
table with --handoff when it is computed.

`--units` adds the translation units by state (matched, done, partial, untouched), from tools/tu_status.py,
which has the rest: one unit's functions, the units nearest to matched, data/tu_status.csv.

Usage: python tools/progress.py [--json build/progress.json] [--top 15]
       python tools/progress.py --slice boot [--version 2.1.0] [--json build/progress.json]
       python tools/progress.py --slice boot --handoff
       python tools/progress.py --slice-from CGameBlackboard::refl_GetClass
       python tools/progress.py --origin [--version 2.1.0]
       python tools/progress.py --quality [--version 2.1.0]
       python tools/progress.py --fuzzy [--version 2.1.0]
       python tools/progress.py --units
"""

from __future__ import annotations

import argparse
import datetime
import json
import re
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread import functions  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
STATUSES = ["O", "m", "M", "W", "U"]
LABELS = {"O": "matching", "m": "minor", "M": "major", "W": "wip", "U": "not decompiled"}


def load() -> list[dict]:
    return functions.rows()


VERIFIED: frozenset[int] = frozenset()  # addresses with a passing row in data/equivalence.csv (main sets it)
FUNCTIONAL: frozenset[int] = frozenset()  # addresses with a `behaves` row (main sets it)


def verified_addresses() -> frozenset[int]:
    """The functions tools/equiv_test.py verified equivalent: the `pass` rows of data/equivalence.csv under
    the test's current rules. tools/check.py keeps the table true to the tree (a stale row fails it)."""
    import equiv_test
    return frozenset(a for a, r in equiv_test.read_table().items()
                     if r["verdict"] == "pass" and r["rules"] == str(equiv_test.RULES))


def functional_addresses() -> frozenset[int]:
    """The functions tools/equiv_test.py --any-kind found to behave the same with another kind of difference
    (tier 3, `functional`): the `behaves` rows under the test's current rules; tools/check.py keeps them true."""
    import equiv_test
    return frozenset(a for a, r in equiv_test.read_table().items()
                     if r["verdict"] == "behaves" and r["rules"] == str(equiv_test.RULES))


def tally(rows: list[dict], verified: frozenset[int] | None = None) -> dict:
    verified = VERIFIED if verified is None else verified
    t = {s: {"functions": 0, "bytes": 0} for s in STATUSES}
    ver = {"functions": 0, "bytes": 0}
    for r in rows:
        t[r["status"]]["functions"] += 1
        t[r["status"]]["bytes"] += int(r["size"])
        if r["status"] == "m" and verified and int(r["address"], 16) in verified:
            ver["functions"] += 1
            ver["bytes"] += int(r["size"])
    total_f = sum(v["functions"] for v in t.values())
    total_b = sum(v["bytes"] for v in t.values())
    return {"by_status": t, "functions": total_f, "bytes": total_b, "verified": ver,
            "matching_bytes_pct": 100 * t["O"]["bytes"] / total_b if total_b else 0.0,
            "done_bytes_pct": 100 * (t["O"]["bytes"] + ver["bytes"]) / total_b if total_b else 0.0,
            "usable_bytes_pct": 100 * (t["O"]["bytes"] + t["m"]["bytes"]) / total_b if total_b else 0.0}


def print_status(overall: dict) -> None:
    """The matching/done/usable lines and the status table (shared by the whole binary and a slice)."""
    print(f"matching (O): {overall['by_status']['O']['functions']} functions, "
          f"{overall['matching_bytes_pct']:.4f}% of code; usable (O+m): {overall['usable_bytes_pct']:.4f}%")
    print(f"done (O, or m verified equivalent by tools/equiv_test.py): {overall['done_bytes_pct']:.4f}% of code; "
          f"{overall['verified']['functions']} of the {overall['by_status']['m']['functions']} m functions are "
          f"verified ({overall['verified']['bytes']:,} bytes)")
    print()
    print("| Status | Functions | Bytes |")
    print("|---|---|---|")
    for s in STATUSES:
        v = overall["by_status"][s]
        print(f"| `{s}` {LABELS[s]} | {v['functions']:,} | {v['bytes']:,} |")


def slice_report(version: str, rows: list[dict], boot: bool, start: str | None) -> dict:
    """Status tally of the functions reachable (by direct calls) from nnMain or from `start`."""
    sys.path.insert(0, str(Path(__file__).resolve().parent))
    import callgraph  # noqa: E402  (needs the Ghidra export; imported only for a slice)

    graph = callgraph.CallGraph(version, rows={int(r["address"], 16): r for r in rows})
    root = callgraph.NNMAIN if boot else graph.resolve(start)
    reach = graph.reachable(root)
    members = [graph.funcs[a] for a in sorted(reach)]
    game = [r for r in members if r["status"] != "L"]
    return {"kind": "boot" if boot else "from", "root": graph.name(root), "root_address": f"0x{root:x}",
            "functions": len(members), "library_functions": len(members) - len(game),
            "overall": tally(game), "caveat": callgraph.CAVEAT}


GENERATED_DIRS = {"reflection", "reflection_types", "reflection_statics", "clones", "tables", "lifted"}
ORIGINS = {"hand": "Hand-typed (`src/` outside the generated directories)",
           "generated": "Generated (reflection, clones, tables, byte-identical lifts)"}


def origin_report(version: str, rows: list[dict]) -> dict:
    """Matched (`O`, `m`) functions and bytes by the origin of their source, from check.py's diff
    cache (the object that defines each function). A function defined in both a generated and a
    hand-written file counts as hand-typed (the hand-written definition is the one the port keeps)."""
    cache = ROOT / "build" / "check" / version
    if not cache.is_dir():
        raise SystemExit(f"{cache}: no check cache; run tools/check.py --version {version} first")
    by_addr = {int(r["address"], 16): r for r in rows}
    origin: dict[int, str] = {}
    for p in cache.rglob("*.o.diff.json"):
        kind = "generated" if p.relative_to(cache).parts[0] in GENERATED_DIRS else "hand"
        try:
            entries = json.loads(p.read_text(encoding="utf-8"))["entries"]
        except (OSError, ValueError, KeyError, TypeError):
            continue
        for e in entries:
            a = e.get("address")
            if a in by_addr and (kind == "hand" or a not in origin):
                origin[a] = kind
    out = {k: {s: {"functions": 0, "bytes": 0} for s in ("O", "m")} for k in ORIGINS}
    for a, kind in origin.items():
        r = by_addr[a]
        if r["status"] in ("O", "m"):
            out[kind][r["status"]]["functions"] += 1
            out[kind][r["status"]]["bytes"] += int(r["size"])
    return out


def print_origin(o: dict, overall: dict) -> None:
    matched = overall["by_status"]["O"]["bytes"] + overall["by_status"]["m"]["bytes"]
    print("| Origin of the source | `O` functions | `O` bytes | `m` functions | `m` bytes | Share of matched bytes |")
    print("|---|---|---|---|---|---|")
    for kind, label in ORIGINS.items():
        v = o[kind]
        share = 100 * (v["O"]["bytes"] + v["m"]["bytes"]) / matched if matched else 0.0
        print(f"| {label} | {v['O']['functions']:,} | {v['O']['bytes'] / 1e6:.2f} MB | {v['m']['functions']:,} | "
              f"{v['m']['bytes'] / 1e6:.2f} MB | {share:.1f}% |")


STEERING = ("offset-cast", "volatile", "empty-block", "barrier", "barrier-no-comment")
# "complete" is the key of the hand-written part of the headline (matching and clean in form); the key keeps its
# old spelling for tools/tu_status.py and the JSON report, the label says what the row tests.
QUALITIES = {
    "complete": "Byte-matching, hand-written, no steering form",
    "match_steered": "Byte-matching, hand-written with a steering form",
    "match_generated": "Byte-matching, generated source",
    "equivalent": "Identical behaviour, bytes differ (verified equivalent)",
    "functional": "Same results on every input tried, bytes differ (functional, tier 3, not done)",
    "unproven": "Written, not proven (`m`, `M`, `W` without a passing verdict)",
    "missing": "Not decompiled (`U`)",
}


def quality(status: str, hand: bool, steered: bool, verified: bool, functional: bool = False) -> str:
    """Which row of the completeness table a function is in. Two qualities, both needed (owner, 2026-11-13):
    the bytes match, and the source is clean in form. Identical behaviour with other bytes is a match for later."""
    if status == "O":
        return "match_generated" if not hand else "match_steered" if steered else "complete"
    if status == "U":
        return "missing"
    if status == "m" and verified:
        return "equivalent"
    return "functional" if status == "M" and functional else "unproven"


_CACHE_OBJECTS: dict[str, list[tuple[Path, list[dict]]]] = {}


def cache_objects(version: str) -> list[tuple[Path, list[dict]]]:
    """(path relative to check.py's cache, entries) of each object in build/check/<version>/, sorted; read once
    per run (quality_by_address and names_report both walk it)."""
    if version not in _CACHE_OBJECTS:
        cache = ROOT / "build" / "check" / version
        if not cache.is_dir():
            raise SystemExit(f"{cache}: no check cache; run tools/check.py --version {version} first")
        objs = []
        for p in sorted(cache.rglob("*.o.diff.json")):
            try:
                objs.append((p.relative_to(cache), json.loads(p.read_text(encoding="utf-8"))["entries"]))
            except (OSError, ValueError, KeyError, TypeError):
                continue
        _CACHE_OBJECTS[version] = objs
    return _CACHE_OBJECTS[version]


def source_of(rel: Path) -> Path:
    """The source under src/ that check.py compiled into the cached object `rel`."""
    return ROOT / "src" / (rel.as_posix()[:-len(".o.diff.json")] + ".cpp")


def quality_by_address(version: str, rows: list[dict], verified: frozenset[int] | None = None) -> dict[int, tuple[str, bool]]:
    """Each function's row of the completeness table (a key of QUALITIES), and whether its source has a
    steering form. tools/tu_status.py --quality adds these up per translation unit; quality_report, below,
    for the whole binary, and its docstring says what clean in form means here."""
    import fakematch
    verified = VERIFIED if verified is None else verified
    by_addr = {int(r["address"], 16): r for r in rows}
    hand: set[int] = set()
    steered: set[int] = set()
    seen: set[int] = set()
    for rel, entries in cache_objects(version):
        if not isinstance(entries, list):
            continue
        mine = [e for e in entries if e.get("address") in by_addr]
        seen.update(e["address"] for e in mine)
        if rel.parts[0] in GENERATED_DIRS or not mine:
            continue
        hand.update(e["address"] for e in mine)
        src = source_of(rel)
        if not src.is_file():
            continue
        flagged = {h.function.split("::")[-1] for h in fakematch.scan_text(src.relative_to(ROOT).as_posix(),
                                                                         src.read_text(encoding="utf-8", errors="replace"))
                   if h.kind in STEERING and h.function}
        steered.update(e["address"] for e in mine if e.get("name", "").split("::")[-1] in flagged)
    out: dict[int, tuple[str, bool]] = {}
    for a, r in by_addr.items():
        k = quality(r["status"], a in hand, a in steered, a in verified, a in FUNCTIONAL)
        if k == "match_generated" and a not in seen:
            k = "unproven"  # an O the cache does not know: the cache is older than the function list
        out[a] = (k, a in steered)
    return out


def quality_report(version: str, rows: list[dict], verified: frozenset[int] | None = None,
                   qa: dict[int, tuple[str, bool]] | None = None) -> dict:
    """The game's functions by the qualities the project needs of each: byte-matching, and clean in form.

    Clean in form is measured, for hand-written code, by what a tool can see: the function's source is not in a
    generated directory and has none of the forms that are there to steer the compiler (tools/fakematch.py's
    offset casts, `volatile`, empty blocks and barriers, attributed to the function they sit in); generated code
    is clean in form once its family's sampled read passed (names_report). It cannot
    see a wrong comment, a meaningless name, a struct private to one file or a form inside a shared inline
    helper: reading does (tools/sample_review.py). Needs check.py's diff cache for where each function is
    defined."""
    by_addr = {int(r["address"], 16): r for r in rows}
    out = {k: {"functions": 0, "bytes": 0} for k in QUALITIES}
    out["equivalent_readable"] = {"functions": 0, "bytes": 0}
    for a, (k, steered) in (qa if qa is not None else quality_by_address(version, rows, verified)).items():
        for key in [k] + (["equivalent_readable"] if k == "equivalent" and not steered else []):
            out[key]["functions"] += 1
            out[key]["bytes"] += int(by_addr[a]["size"])
    return out


HEADLINE = "Matching and clean in form"


def headline(q: dict, n: dict | None) -> dict:
    """The headline (owner, 2026-11-23): byte-matching, hand-written, no steering form, plus generated, read and
    passed (names_report's `read_passed`; none when names were not computed)."""
    rp = (n or {}).get("read_passed", {"functions": 0, "bytes": 0})
    return {"functions": q["complete"]["functions"] + rp["functions"], "bytes": q["complete"]["bytes"] + rp["bytes"]}


def print_quality(q: dict, overall: dict, n: dict | None = None) -> None:
    total = overall["bytes"]
    print("| Completeness | Functions | Bytes | Share of code |")
    print("|---|---|---|---|")
    for k, label in QUALITIES.items():
        v = q[k]
        print(f"| {label} | {v['functions']:,} | {v['bytes'] / 1e6:.2f} MB | {100 * v['bytes'] / total:.2f}% |")
    e = q["equivalent_readable"]
    print(f"(of the verified-equivalent ones, {e['functions']:,} have no steering form: they need only a match, later.)")
    h = headline(q, n)
    rp = (n or {}).get("read_passed", {"functions": 0, "bytes": 0})
    print(f"**{HEADLINE}** (the headline): **{100 * h['bytes'] / total:.2f}%** ({h['functions']:,} functions): "
          f"byte-matching, hand-written, no steering form {100 * q['complete']['bytes'] / total:.2f}% plus generated, "
          f"read and passed (`data/family_reads.csv`) {100 * rp['bytes'] / total:.2f}%. Names are a second figure, "
          "below, not a cut to it.")


def quality_line(q: dict, overall: dict, n: dict | None = None) -> str:
    total = overall["bytes"]

    def pct(k: str) -> str:
        return f"{100 * q[k]['bytes'] / total:.2f}%"
    h = headline(q, n)
    rp = h["bytes"] - q["complete"]["bytes"]
    not_yet = q["match_steered"]["bytes"] + q["match_generated"]["bytes"] - rp
    return (f"Completeness, whole binary (`tools/progress.py --quality`): {HEADLINE.lower()} "
            f"**{100 * h['bytes'] / total:.2f}%** ({h['functions']:,} functions; byte-matching, hand-written, no "
            f"steering form {pct('complete')}, plus generated, read and passed {100 * rp / total:.2f}%); byte-matching "
            f"but not clean in form yet {100 * not_yet / total:.2f}% (generated source not yet read "
            f"{100 * (q['match_generated']['bytes'] - rp) / total:.2f}%, hand-written with a steering form "
            f"{pct('match_steered')}); identical behaviour with other bytes {pct('equivalent')} "
            f"({q['equivalent']['functions']:,} functions, a match for later); functional (same results on every input "
            f"tried, tier 3) {pct('functional')} ({q['functional']['functions']:,} functions). Names are a second "
            "figure, on the next line.")


# Names (CLAUDE.md, "Purpose and completion" and "Names"). A placeholder is a name that says only where something
# is: `fn_<addr>`, an `Unk...` type (not `UnkNoArg`, which names a fact: the original leaves that argument register
# unloaded; not `Unknown`), a member `field_0x..`, `unk_<addr>` or `unk_0x..`, a global `g_unk_...`, and a virtual
# slot `vf_<slot>` (a number, not a role). `refl_...` is an inferred role (refl_GetClass, refl_TypeOf) that reads
# as what the function does, so it is a name; so is `pad_0x..`, the layout's own mark for bytes nothing reads.
# Comments, string contents and preprocessor lines are not tested.
PLACEHOLDER = re.compile(r"\bfn_[0-9a-fA-F]{6,}\b|\bUnk(?!NoArg\b)(?![a-z])\w*|\bfield_(?:0x)?[0-9a-fA-F]+\b"
                         r"|\bunk_(?:0x)?[0-9a-fA-F]+\b|\bg_unk_\w+|\bvf_(?:0x[0-9a-fA-F]+|\d+)\b")
# Loose (a function's own name, class and members real; placeholder callees allowed): in its body, a reference to
# another function is allowed (`fn_<addr>` called or passed as a pointer, `vf_<slot>`), and so is an `Unk...` class
# that only qualifies a callee (`UnkFoo::fn_x(`). Its members, globals and the types of what it touches still count.
_LOOSE_OK = re.compile(r"fn_|vf_")
_QUALIFIES_CALL = re.compile(r"(?:\s*::\s*~?\w+)+\s*(?:<[^<>;{}()]*>)?\s*\(")
_PARENS, _BRACES = re.compile(r"[()]"), re.compile(r"[{}]")
_NOT_CODE = re.compile(r"//[^\n]*|/\*.*?\*/|\"(?:\\.|[^\"\\\n])*\"|'(?:\\.|[^'\\\n])*'|^[ \t]*#[^\n]*(?:\\\n[^\n]*)*",
                       re.S | re.M)
_INIT = re.compile(r"\s*[\w:~<>,\s]+?\s*([({])")
NAME_KINDS = {"loose": "Hand-written clean in form, names loose (its own name, class and members real; placeholder "
                       "callees allowed)",
              "strict": "Hand-written clean in form, names strict (no placeholder anywhere in it)"}
# Of the loose ones, where the function list's own name comes from (owner, 2026-11-23: inferred names count as named,
# as their own tier). "unlisted": the source spells a real name, the list's row does not (a placeholder name or
# `name_source` unknown); not counted as named until tools/rename.py records it.
SOURCED = frozenset({"rtti", "reflection", "string", "symbol", "library"})
NAME_TIERS = {"named_sourced": "of which own name from a source (`name_source` rtti, reflection, string, symbol, "
                               "library)",
              "named_inferred": "of which own name inferred (`name_source` inferred, `data/names.csv`)",
              "named_unlisted": "of which a real name in the source only, not in `data/functions/` (not counted as "
                                "named)"}


def name_tier(row: dict) -> str:
    """A key of NAME_TIERS for a function that passes the loose test, by its row in data/functions/."""
    if PLACEHOLDER.search(row.get("name", "")):
        return "named_unlisted"
    src = row.get("name_source", "")
    return "named_sourced" if src in SOURCED else "named_inferred" if src == "inferred" else "named_unlisted"
FAMILY_READS = ROOT / "data" / "family_reads.csv"
FAMILY_COLUMNS = ["family", "scope", "generator_commit", "date", "sample_size", "verdict", "reader", "notes"]


def code_only(text: str) -> str:
    """`text` with comments, string and character contents and preprocessor lines blanked to spaces; line breaks
    and offsets kept."""
    return _NOT_CODE.sub(lambda m: re.sub(r"[^\n]", " ", m.group(0)), text.replace("\r\n", "\n"))


def placeholders(code: str, loose: bool = False) -> list[str]:
    """The placeholder names in `code` (already code_only); loose skips those that refer to another function."""
    return [m.group(0) for m in PLACEHOLDER.finditer(code)
            if not (loose and (_LOOSE_OK.match(m.group(0))
                               or (m.group(0).startswith("Unk") and _QUALIFIES_CALL.match(code, m.end()))))]


def short_name(name: str) -> str:
    """The last part of a function's name as check.py's cache gives it: `bool ns::C<T>::f(int) const` -> `f`."""
    out, depth = [], 0
    for ch in name:
        if ch == "(" and depth == 0:
            break
        depth += (ch == "<") - (ch == ">")
        if depth == 0 and ch != ">":
            out.append(ch)
    words = "".join(out).split()
    return words[-1].split("::")[-1] if words else ""


def _close(code: str, i: int, rx: re.Pattern) -> int | None:
    """The offset just past the bracket that closes the one at `i`."""
    depth = 0
    for m in rx.finditer(code, i):
        depth += 1 if m.group(0) in "({" else -1
        if depth == 0:
            return m.end()
    return None


def definitions(code: str, name: str) -> list[tuple[str, str]]:
    """(signature, body) of each definition of the function `name` (its last part) in `code` (code_only): one
    that starts at column 0, a one-line definition included. One the tool does not find (indented, made by a
    macro) is not here."""
    out: list[tuple[str, str]] = []
    if not name:
        return out
    rx = re.compile(r"^(?=[^\s#])[^\n;{}]*?(?<![\w~])" + re.escape(name) + r"\s*(?:<[^\n;{}()]*>)?\s*\(", re.M)
    for m in rx.finditer(code):
        i = _close(code, m.end() - 1, _PARENS)
        if i is None:
            continue
        rest = re.compile(r"[^;{:]*").match(code, i).end()
        if rest >= len(code) or code[rest] == ";":
            continue  # a declaration
        if code[rest] == ":" and code.startswith("::", rest):
            continue
        if code[rest] == ":":  # a constructor's initializers
            j = rest + 1
            while True:
                e = _INIT.match(code, j)
                if not e:
                    break
                k = _close(code, e.start(1), _PARENS if e.group(1) == "(" else _BRACES)
                if k is None:
                    break
                j = re.compile(r"\s*").match(code, k).end()
                if j < len(code) and code[j] == ",":
                    j += 1
                    continue
                break
            rest = j
            if rest >= len(code) or code[rest] != "{":
                continue
        end = _close(code, rest, _BRACES)
        if end is not None:
            out.append((code[m.start():rest], code[rest:end]))
    return out


def names_of(code: str, name: str) -> tuple[bool, bool, bool]:
    """(loose, strict, by_file) for the function `name` in a file (code_only): by its own definitions where
    definitions() finds them, else by the whole file. Its signature is its own name, class and parameter types,
    so every placeholder there counts in both figures."""
    defs = definitions(code, short_name(name))
    if not defs:
        return not placeholders(code, loose=True), not placeholders(code), True
    loose = all(not placeholders(sig) and not placeholders(body, loose=True) for sig, body in defs)
    strict = all(not placeholders(sig + body) for sig, body in defs)
    return loose, strict, False


LOOSE_REASONS = {"own_name": "its own name is a placeholder",
                 "own_class": "its own class or namespace is a placeholder",
                 "signature_type": "a parameter or return type is a placeholder",
                 "member": "a member it touches (`field_0x..`, `unk_0x..`)",
                 "global": "a global it touches (`g_unk_...`, `unk_<addr>`)",
                 "body_type": "a local or other type in its body (`Unk...`)"}


def loose_reason(code: str, name: str) -> str | None:
    """Why the function `name` fails the loose test (a key of LOOSE_REASONS), by the first reason in that order;
    None when it passes. By the whole file where definitions() does not find it, as names_of."""
    short = short_name(name)
    parts, depth, cur = [], 0, ""  # the cache's name, cut at top-level `::`, template arguments kept
    for ch in name:
        if ch == "(" and depth == 0:
            break
        depth += (ch == "<") - (ch == ">")
        cur = "" if ch == " " and depth == 0 else cur + ch
        if depth == 0 and cur.endswith("::"):
            parts.append(cur[:-2])
            cur = ""
    structor = bool(parts) and cur.lstrip("~") == parts[-1].split("<")[0]  # named for its class: the class decides
    if not structor and (PLACEHOLDER.fullmatch(short) or placeholders(code_only(cur))):
        return "own_name"  # its template arguments are part of its name
    defs = definitions(code, short)
    quals = parts
    sigs, bodies = [sig for sig, _ in defs], [body for _, body in defs] if defs else [code]
    for sig in sigs:
        m = re.search(r"((?:[\w~]+\s*(?:<[^()]*?>)?\s*::\s*)+)~?" + re.escape(short) + r"\s*(?:<[^()]*>)?\s*\(\s*$",
                      sig[:sig.find(short + "(") + len(short) + 1] if short + "(" in sig else "")
        if m:
            quals.append(m.group(1))
    if any(placeholders(q) for q in quals):
        return "own_class"
    if any(placeholders(sig) for sig in sigs):
        return "signature_type"
    found = [t for b in bodies for t in placeholders(b, loose=True)]
    if any(t.startswith("field_") or t.startswith("unk_0x") for t in found):
        return "member"
    if any(t.startswith("g_unk_") or t.startswith("unk_") for t in found):
        return "global"
    return "body_type" if found else None


def read_family_reads(path: Path = FAMILY_READS) -> list[dict]:
    """The rows of data/family_reads.csv: the sampled reads of generated families (CLAUDE.md, "Purpose")."""
    import csv
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as fh:
        return [r for r in csv.DictReader(fh) if (r.get("family") or "").strip()]


def passed_scopes(reads: list[dict], root: Path = ROOT) -> list[tuple[list[str], set[int]]]:
    """The scopes whose family passed its latest read: per family, the row with the latest date (the later row on
    a tie) decides, pass or fail for the whole family. A scope is a path glob relative to the repository, or the
    path of a list file (`.txt`, `.csv`, `.lst`) with one glob or function address per line (`#` comments)."""
    latest: dict[str, dict] = {}
    for r in reads:
        f = r["family"].strip()
        if f not in latest or (r.get("date") or "") >= (latest[f].get("date") or ""):
            latest[f] = r
    out = []
    for r in latest.values():
        if (r.get("verdict") or "").strip().lower() != "pass":
            continue
        scope = (r.get("scope") or "").strip()
        items = [scope]
        listed = root / scope
        if scope and listed.suffix in (".txt", ".csv", ".lst") and listed.is_file():
            items = [ln.split("#")[0].strip() for ln in listed.read_text(encoding="utf-8").splitlines()]
        globs, addrs = [], set()
        for it in filter(None, items):
            try:
                addrs.add(int(it, 16))
            except ValueError:
                globs.append(it)
        out.append((globs, addrs))
    return out


def covered(scopes: list[tuple[list[str], set[int]]], path: str, address: int) -> bool:
    import fnmatch
    return any(address in addrs or any(fnmatch.fnmatchcase(path, g) for g in globs) for globs, addrs in scopes)


def names_report(version: str, rows: list[dict], qa: dict[int, tuple[str, bool]],
                 reads_path: Path = FAMILY_READS) -> dict:
    """The figures CLAUDE.md, "Purpose and completion", defines and the completeness table does not: the names of
    the byte-matching, hand-written functions with no steering form (`complete`), loose and strict, tested on each
    function's own text (by file when the tool does not find its definition), and the loose ones by where the
    function list's name comes from (NAME_TIERS); per generated directory, the matching functions whose file has no
    placeholder ("clean, not yet read"); and those of them a passing sampled read in data/family_reads.csv
    covers ("read and passed"). Reporting only."""
    by_addr = {int(r["address"], 16): r for r in rows}
    zero = lambda: {"functions": 0, "bytes": 0}  # noqa: E731
    out = {"loose": zero(), "strict": zero(), "by_file": zero(), "loose_fails": {k: zero() for k in LOOSE_REASONS}, "own_name_listed": zero(),
           **{k: zero() for k in NAME_TIERS},
           "generated": {d: {"matching": zero(), "clean": zero()} for d in sorted(GENERATED_DIRS)},
           "read_passed": zero()}
    scopes = passed_scopes(read_family_reads(reads_path))
    files: dict[Path, str | None] = {}

    def scan(src: Path) -> str | None:
        """The file's code (code_only), read once per run."""
        if src not in files:
            files[src] = code_only(src.read_text(encoding="utf-8", errors="replace")) if src.is_file() else None
        return files[src]

    def add(key: dict, a: int) -> None:
        key["functions"] += 1
        key["bytes"] += int(by_addr[a]["size"])

    done: set[int] = set()
    clean_file: dict[Path, bool] = {}
    for rel, entries in cache_objects(version):
        if not isinstance(entries, list):
            continue
        gen = rel.parts[0] if rel.parts[0] in GENERATED_DIRS else None
        mine = [e for e in entries if e.get("address") in by_addr and e["address"] not in done
                and qa.get(e["address"], ("",))[0] == ("match_generated" if gen else "complete")]
        if not mine:
            continue
        src = source_of(rel)
        s = scan(src)
        for e in mine:
            a = e["address"]
            if a in done:
                continue  # a second symbol of the same function
            done.add(a)
            if gen:
                add(out["generated"][gen]["matching"], a)
                if s is None:
                    continue
                if src not in clean_file:
                    clean_file[src] = not placeholders(s)
                if not clean_file[src]:
                    continue
                if covered(scopes, src.relative_to(ROOT).as_posix(), a):
                    add(out["read_passed"], a)
                else:
                    add(out["generated"][gen]["clean"], a)
                continue
            if s is None:
                continue
            loose, strict, by_file = names_of(s, e.get("name", ""))
            for k, v in (("loose", loose), ("strict", strict), ("by_file", by_file)):
                if v:
                    add(out[k], a)
            if loose:
                add(out[name_tier(by_addr[a])], a)
            else:
                why = loose_reason(s, e.get("name", "")) or "body_type"
                add(out["loose_fails"][why], a)
                if why == "own_name" and not PLACEHOLDER.search(by_addr[a].get("name", "")):
                    add(out["own_name_listed"], a)  # the function list has a real name the source does not use yet
    return out


def print_names(n: dict, q: dict, overall: dict) -> None:
    total = overall["bytes"]

    def row(label: str, v: dict, mark: str = "") -> None:
        print(f"| {mark}{label}{mark} | {v['functions']:,} | {v['bytes'] / 1e6:.2f} MB | "
              f"{mark}{100 * v['bytes'] / total:.2f}%{mark} |")
    print("| Names, beside the headline (CLAUDE.md, \"Purpose and completion\") | Functions | Bytes | Share of code |")
    print("|---|---|---|---|")
    row(NAME_KINDS["loose"], n["loose"])
    for k, label in NAME_TIERS.items():
        row(label, n[k])
    row(NAME_KINDS["strict"], n["strict"])
    for d, g in n["generated"].items():
        row(f"Generated `src/{d}/`, clean, not yet read (matching, file free of placeholders; "
            f"{g['matching']['functions']:,} match)", g["clean"])
    row("Generated, read and passed (`data/family_reads.csv`)", n["read_passed"])
    print()
    print("| Hand-written clean in form, failing loose, by the first reason | Functions | Bytes | Share of code |")
    print("|---|---|---|---|")
    for k, label in LOOSE_REASONS.items():
        row(label, n["loose_fails"][k])
    print(f"(of those whose own name is a placeholder, {n['own_name_listed']['functions']:,} have a real name in "
          "data/functions/ that their source does not use yet.)")
    print(f"(names are tested on each such function's own text; {n['by_file']['functions']:,} of them by file, "
          "where the tool did not find the definition. Placeholders: fn_<addr>, Unk... types other than UnkNoArg, "
          "field_0x.., unk_<addr>, g_unk_..., vf_<slot>; refl_ roles are names. Comments and strings do not count. "
          "Names are a floor a tool can see, not a reading.)")
    print(named_line(n, overall))


def named(n: dict) -> dict:
    """Named (owner, 2026-11-23): names loose, own name from a source plus own name inferred."""
    s, i = n["named_sourced"], n["named_inferred"]
    return {"functions": s["functions"] + i["functions"], "bytes": s["bytes"] + i["bytes"]}


def named_line(n: dict, overall: dict) -> str:
    total = overall["bytes"]

    def pct(v: dict) -> str:
        return f"{100 * v['bytes'] / total:.2f}%"
    s, i, u, a = n["named_sourced"], n["named_inferred"], n["named_unlisted"], named(n)
    return (f"Named (names loose, own name in the function list): **{pct(a)}** ({a['functions']:,} functions) = from a "
            f"source {pct(s)} ({s['functions']:,}) + inferred {pct(i)} ({i['functions']:,}); a real name in the "
            f"source only, not counted, {pct(u)} ({u['functions']:,}).")


def names_line(n: dict, q: dict, overall: dict) -> str:
    total = overall["bytes"]

    def pct(v: dict) -> str:
        return f"{100 * v['bytes'] / total:.2f}%"
    clean = {"functions": sum(g["clean"]["functions"] for g in n["generated"].values()),
             "bytes": sum(g["clean"]["bytes"] for g in n["generated"].values())}
    s, i, u, a = n["named_sourced"], n["named_inferred"], n["named_unlisted"], named(n)
    return (f"Names, whole binary (`tools/progress.py --quality`), a second figure beside the headline, not a cut to "
            f"it: named **{pct(a)}** ({a['functions']:,} functions) = own name from a source {pct(s)} "
            f"({s['functions']:,}) + own name inferred {pct(i)} ({i['functions']:,}); a real name in the source only, "
            f"not counted, {pct(u)} ({u['functions']:,}). Of byte-matching, hand-written, no steering form: names loose "
            f"{pct(n['loose'])} ({n['loose']['functions']:,} functions), names strict {pct(n['strict'])} "
            f"({n['strict']['functions']:,}), per function ({n['by_file']['functions']:,} by file); generated, clean, "
            f"not yet read {pct(clean)} ({clean['functions']:,}); generated, read and passed {pct(n['read_passed'])} "
            f"({n['read_passed']['functions']:,}). Names are a floor a tool can see, not a reading.")


def fuzzy_report(version: str, rows: list[dict], verified: frozenset[int] | None = None) -> dict:
    """Partial credit by closeness (module docstring, --fuzzy): {score_pct, near: drafts at 95% or more,
    by_band: bytes by similarity band of the functions not yet done}."""
    import csv
    verified = VERIFIED if verified is None else verified
    close: dict[int, float] = {}
    index = ROOT / "drafts" / "INDEX.csv"
    if index.is_file():
        with index.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                try:
                    v = float(r["similarity"]) / 100
                except ValueError:
                    continue  # a draft that does not build has no similarity
                a = int(r["address"], 16)
                close[a] = max(close.get(a, 0.0), v)
    cache = ROOT / "build" / "check" / version
    if cache.is_dir():
        for p in cache.rglob("*.o.diff.json"):
            try:
                entries = json.loads(p.read_text(encoding="utf-8"))["entries"]
            except (OSError, ValueError, KeyError, TypeError):
                continue
            for e in entries:
                if isinstance(e.get("address"), int) and isinstance(e.get("score"), (int, float)):
                    close[e["address"]] = max(close.get(e["address"], 0.0), float(e["score"]))
    total = credit = 0.0
    near = {"functions": 0, "bytes": 0}
    bands = {"95-100%": 0, "80-95%": 0, "50-80%": 0, "under 50%": 0}
    for r in rows:
        a, size = int(r["address"], 16), int(r["size"])
        total += size
        if r["status"] == "O" or (r["status"] == "m" and a in verified):
            credit += size
            continue
        v = close.get(a, 0.0)
        credit += size * v
        if a in close:
            bands["95-100%" if v >= 0.95 else "80-95%" if v >= 0.8 else "50-80%" if v >= 0.5 else "under 50%"] += size
            if v >= 0.95:
                near["functions"] += 1
                near["bytes"] += size
    return {"score_pct": 100 * credit / total if total else 0.0, "near": near, "by_band": bands,
            "src_scores": cache.is_dir()}


def fuzzy_line(f: dict) -> str:
    n = f["near"]
    return (f"Near misses, whole binary (`tools/progress.py --fuzzy`): {n['functions']:,} functions not yet done "
            f"are at 95% similarity or more ({n['bytes'] / 1e6:.2f} MB); fuzzy match, each function's bytes "
            f"weighted by its similarity, {f['score_pct']:.2f}%. Closeness, not completion: no threshold uses it.")


HANDOFF = ROOT / "docs" / "HANDOFF.md"
BEGIN, END = "<!-- progress:begin -->", "<!-- progress:end -->"
WHOLE, BOOT = "Whole binary (`L` excluded)", "Boot slice (from `nnMain`, direct calls)"
NAMES_PREFIXES = ("Names, whole binary", "Readable and matching, whole binary")  # the second: before 2026-11-23


def table_row(label: str, t: dict) -> str:
    b = t["by_status"]
    return (f"| {label} | {t['functions']:,} | {t['bytes'] / 1e6:.2f} MB | **{t['matching_bytes_pct']:.2f}%** "
            f"({b['O']['functions']:,}) | {t['usable_bytes_pct']:.2f}% | {b['U']['functions']:,} |")


def update_handoff(path: Path, overall: dict, boot: dict | None, quality_text: str | None = None,
                   fuzzy_text: str | None = None, names_text: str | None = None) -> None:
    """Replace the generated table in the handoff; keep the old boot row when no slice was computed, and the
    old completeness line when it was not computed (it needs check.py's cache)."""
    text = path.read_text(encoding="utf-8")
    if BEGIN not in text or END not in text:
        raise SystemExit(f"{path}: no {BEGIN} ... {END} block")
    head, rest = text.split(BEGIN, 1)
    old, tail = rest.split(END, 1)
    old_boot = next((ln for ln in old.splitlines() if ln.startswith(f"| {BOOT} |")), None)
    boot_row = table_row(BOOT, boot) if boot else old_boot
    cmd = "tools/progress.py --slice boot --handoff" if boot else "tools/progress.py --handoff"
    kept = "" if boot else " (the boot row is kept from the last `--slice boot` run)"
    lines = [f"Generated by `{cmd}` on {datetime.date.today().isoformat()} from `data/functions/`{kept}; "
             "rerun it rather than edit.", "",
             "| Scope | Functions | Bytes | `O` matching (bytes) | `O`+`m` usable | `U` left |",
             "|---|---|---|---|---|---|", table_row(WHOLE, overall)]
    if boot_row:
        lines.append(boot_row)
    if "verified" in overall:
        v = overall["verified"]
        lines += ["", f"Done, whole binary (`O`, or `m` that `tools/equiv_test.py` verified equivalent): "
                      f"**{overall['done_bytes_pct']:.2f}%**; {v['functions']:,} of the "
                      f"{overall['by_status']['m']['functions']:,} `m` functions are verified ({v['bytes']:,} bytes)."]
    quality_text = quality_text or next((ln for ln in old.splitlines() if ln.startswith("Completeness, whole binary")), None)
    if quality_text:
        lines += ["", quality_text]
    names_text = names_text or next((ln for ln in old.splitlines() if ln.startswith(NAMES_PREFIXES)), None)
    if names_text:
        lines += ["", names_text]
    fuzzy_text = fuzzy_text or next((ln for ln in old.splitlines() if ln.startswith("Near misses, whole binary")), None)
    if fuzzy_text:
        lines += ["", fuzzy_text]
    path.write_text(head + BEGIN + "\n" + "\n".join(lines) + "\n" + END + tail, encoding="utf-8", newline="\n")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--json", type=Path, help="also write the report as JSON")
    ap.add_argument("--top", type=int, default=15)
    ap.add_argument("--version", default="2.1.0", help="dump version whose Ghidra export a slice uses (default 2.1.0)")
    sl = ap.add_mutually_exclusive_group()
    sl.add_argument("--slice", choices=["boot"], help="also report the functions reachable from nnMain")
    sl.add_argument("--slice-from", metavar="NAME|ADDR", help="also report the functions reachable from this one")
    ap.add_argument("--handoff", nargs="?", const=HANDOFF, type=Path, metavar="PATH",
                    help="rewrite the generated status table in docs/HANDOFF.md (or PATH)")
    ap.add_argument("--origin", action="store_true",
                    help="split matched bytes into hand-typed and generated source (needs build/check/<version>/)")
    ap.add_argument("--quality", action="store_true",
                    help="the functions by the qualities each needs: byte-matching, and clean in form (the headline), "
                         "with names beside it (needs build/check/<version>/)")
    ap.add_argument("--fuzzy", action="store_true",
                    help="partial credit by similarity: drafts/INDEX.csv, and check.py's cache when it is there")
    ap.add_argument("--units", action="store_true",
                    help="the translation units by state (tools/tu_status.py has the per-unit views)")
    args = ap.parse_args()
    if args.handoff and args.slice_from:
        ap.error("--handoff takes the boot slice or none, not --slice-from")

    global VERIFIED, FUNCTIONAL
    VERIFIED = verified_addresses()
    FUNCTIONAL = functional_addresses()
    rows = load()
    library = [r for r in rows if r["status"] == "L"]
    game = [r for r in rows if r["status"] != "L"]
    overall = tally(game)
    by_sub = defaultdict(list)
    for r in game:
        by_sub[r["subsystem"].rstrip("?")].append(r)
    subs = {k: tally(v) for k, v in by_sub.items()}

    print(f"main: {len(rows)} functions; {len(library)} library (L, excluded); "
          f"{overall['functions']} counted, {overall['bytes']:,} bytes")
    print_status(overall)
    print()
    print("| Subsystem | Functions | Bytes | O | m+M+W |")
    print("|---|---|---|---|---|")
    for name, t in sorted(subs.items(), key=lambda kv: -kv[1]["bytes"])[:args.top]:
        b = t["by_status"]
        print(f"| {name} | {t['functions']:,} | {t['bytes']:,} | {b['O']['functions']} | "
              f"{b['m']['functions'] + b['M']['functions'] + b['W']['functions']} |")

    report = {"overall": overall, "library_functions": len(library), "subsystems": subs}
    if args.slice or args.slice_from:
        s = slice_report(args.version, rows, args.slice == "boot", args.slice_from)
        report["slice"] = s
        label = "boot slice" if s["kind"] == "boot" else "slice"
        print()
        print(f"{label}: reachable from {s['root']} ({s['root_address']}) by direct calls: "
              f"{s['functions']} functions; {s['library_functions']} library (L, excluded); "
              f"{s['overall']['functions']} counted, {s['overall']['bytes']:,} bytes")
        print_status(s["overall"])
        print()
        print(s["caveat"])

    if args.origin:
        report["origin"] = origin_report(args.version, game)
        print()
        print("matched bytes by origin of the source (hand-typed code is the completion metric):")
        print_origin(report["origin"], overall)

    cache = (ROOT / "build" / "check" / args.version).is_dir()
    if args.quality or (args.handoff and cache):
        qa = quality_by_address(args.version, game)
        report["quality"] = quality_report(args.version, game, qa=qa)
        report["names"] = names_report(args.version, game, qa)
    if args.quality:
        print()
        print("completeness by the qualities each function needs, byte-matching and clean in form (whole binary):")
        print_quality(report["quality"], overall, report["names"])
        print()
        print("names, a second figure beside the headline, and generated code that reads as source:")
        print_names(report["names"], report["quality"], overall)

    if args.fuzzy or args.handoff:
        report["fuzzy"] = fuzzy_report(args.version, game)
    if args.fuzzy:
        f = report["fuzzy"]
        print()
        print(fuzzy_line(f))
        print("bytes of the functions not yet done, by similarity: "
              + ", ".join(f"{k} {v / 1e6:.2f} MB" for k, v in f["by_band"].items())
              + ("" if f["src_scores"] else " (drafts only: no check.py cache for the scores of src/)"))

    if args.units:
        import tu_map
        import tu_status
        units = tu_status.build(tu_map.load_map(), rows, VERIFIED)
        report["units"] = tu_status.by_state(units)
        print()
        print("translation units by state (data/tu_map.csv; tools/tu_status.py --list STATE, --unit U):")
        print("\n".join(tu_status.state_table(units)))

    if args.handoff:
        update_handoff(args.handoff, overall, report["slice"]["overall"] if "slice" in report else None,
                       quality_line(report["quality"], overall, report["names"]) if "quality" in report else None,
                       fuzzy_line(report["fuzzy"]) if "fuzzy" in report else None,
                       names_line(report["names"], report["quality"], overall) if "names" in report else None)
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
