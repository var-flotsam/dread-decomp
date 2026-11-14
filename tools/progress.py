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

`--quality` reports the two qualities each function needs, apart (owner, 2026-11-13): the bytes match, and the
source is readable. A function is complete with both. Identical behaviour with other bytes (verified
equivalent) has a row of its own: acceptable now, a match later. "Readable" is a floor a tool can see
(hand-written, with none of tools/fakematch.py's steering forms in the function), not a reading. The same
figures go under the handoff's table with --handoff when check.py's cache is there.

`--units` adds the translation units by state (matched, done, partial, untouched), from tools/tu_status.py,
which has the rest: one unit's functions, the units nearest to matched, data/tu_status.csv.

Usage: python tools/progress.py [--json build/progress.json] [--top 15]
       python tools/progress.py --slice boot [--version 2.1.0] [--json build/progress.json]
       python tools/progress.py --slice boot --handoff
       python tools/progress.py --slice-from CGameBlackboard::refl_GetClass
       python tools/progress.py --origin [--version 2.1.0]
       python tools/progress.py --quality [--version 2.1.0]
       python tools/progress.py --units
"""

from __future__ import annotations

import argparse
import datetime
import json
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


def verified_addresses() -> frozenset[int]:
    """The functions tools/equiv_test.py verified equivalent: the `pass` rows of data/equivalence.csv under
    the test's current rules. tools/check.py keeps the table true to the tree (a stale row fails it)."""
    import equiv_test
    return frozenset(a for a, r in equiv_test.read_table().items()
                     if r["verdict"] == "pass" and r["rules"] == str(equiv_test.RULES))


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
QUALITIES = {
    "complete": "Byte-matching and readable",
    "match_steered": "Byte-matching, hand-written with a steering form",
    "match_generated": "Byte-matching, generated source",
    "equivalent": "Identical behaviour, bytes differ (verified equivalent)",
    "unproven": "Written, not proven (`m`, `M`, `W` without a passing verdict)",
    "missing": "Not decompiled (`U`)",
}


def quality(status: str, hand: bool, steered: bool, verified: bool) -> str:
    """Which row of the completeness table a function is in. Two qualities, both needed (owner, 2026-11-13):
    the bytes match, and the source is readable. Identical behaviour with other bytes is a match for later."""
    if status == "O":
        return "match_generated" if not hand else "match_steered" if steered else "complete"
    if status == "U":
        return "missing"
    return "equivalent" if status == "m" and verified else "unproven"


def quality_by_address(version: str, rows: list[dict], verified: frozenset[int] | None = None) -> dict[int, tuple[str, bool]]:
    """Each function's row of the completeness table (a key of QUALITIES), and whether its source has a
    steering form. tools/tu_status.py --quality adds these up per translation unit; quality_report, below,
    for the whole binary, and its docstring says what readable means here."""
    import fakematch
    verified = VERIFIED if verified is None else verified
    cache = ROOT / "build" / "check" / version
    if not cache.is_dir():
        raise SystemExit(f"{cache}: no check cache; run tools/check.py --version {version} first")
    by_addr = {int(r["address"], 16): r for r in rows}
    hand: set[int] = set()
    steered: set[int] = set()
    seen: set[int] = set()
    for p in sorted(cache.rglob("*.o.diff.json")):
        rel = p.relative_to(cache)
        try:
            entries = json.loads(p.read_text(encoding="utf-8"))["entries"]
        except (OSError, ValueError, KeyError, TypeError):
            continue
        mine = [e for e in entries if e.get("address") in by_addr]
        seen.update(e["address"] for e in mine)
        if rel.parts[0] in GENERATED_DIRS or not mine:
            continue
        hand.update(e["address"] for e in mine)
        src = ROOT / "src" / (rel.as_posix()[:-len(".o.diff.json")] + ".cpp")
        if not src.is_file():
            continue
        flagged = {h.function.split("::")[-1] for h in fakematch.scan_text(src.relative_to(ROOT).as_posix(),
                                                                         src.read_text(encoding="utf-8", errors="replace"))
                   if h.kind in STEERING and h.function}
        steered.update(e["address"] for e in mine if e.get("name", "").split("::")[-1] in flagged)
    out: dict[int, tuple[str, bool]] = {}
    for a, r in by_addr.items():
        k = quality(r["status"], a in hand, a in steered, a in verified)
        if k == "match_generated" and a not in seen:
            k = "unproven"  # an O the cache does not know: the cache is older than the function list
        out[a] = (k, a in steered)
    return out


def quality_report(version: str, rows: list[dict], verified: frozenset[int] | None = None) -> dict:
    """The game's functions by the two qualities the project needs of each: byte-matching, and readable.

    Readable is measured as a floor, by what a tool can see: the function's source is hand-written (not in a
    generated directory) and has none of the forms that are there to steer the compiler (tools/fakematch.py's
    offset casts, `volatile`, empty blocks and barriers, attributed to the function they sit in). It cannot
    see a wrong comment, a meaningless name, a struct private to one file or a form inside a shared inline
    helper: reading does (tools/sample_review.py). Needs check.py's diff cache for where each function is
    defined."""
    by_addr = {int(r["address"], 16): r for r in rows}
    out = {k: {"functions": 0, "bytes": 0} for k in QUALITIES}
    out["equivalent_readable"] = {"functions": 0, "bytes": 0}
    for a, (k, steered) in quality_by_address(version, rows, verified).items():
        for key in [k] + (["equivalent_readable"] if k == "equivalent" and not steered else []):
            out[key]["functions"] += 1
            out[key]["bytes"] += int(by_addr[a]["size"])
    return out


def print_quality(q: dict, overall: dict) -> None:
    total = overall["bytes"]
    print("| Completeness | Functions | Bytes | Share of code |")
    print("|---|---|---|---|")
    for k, label in QUALITIES.items():
        v = q[k]
        mark = "**" if k == "complete" else ""
        print(f"| {mark}{label}{mark} | {v['functions']:,} | {v['bytes'] / 1e6:.2f} MB | {mark}{100 * v['bytes'] / total:.2f}%{mark} |")
    e = q["equivalent_readable"]
    print(f"(of the verified-equivalent ones, {e['functions']:,} have no steering form: they are readable and need only "
          "a match, later. Readable is a floor: see tools/progress.py's quality_report.)")


def quality_line(q: dict, overall: dict) -> str:
    total = overall["bytes"]

    def pct(k: str) -> str:
        return f"{100 * q[k]['bytes'] / total:.2f}%"
    return (f"Completeness, whole binary, by the two qualities each function needs (`tools/progress.py --quality`): "
            f"byte-matching and readable **{pct('complete')}** ({q['complete']['functions']:,} functions); byte-matching "
            f"but not readable yet {100 * (q['match_steered']['bytes'] + q['match_generated']['bytes']) / total:.2f}% "
            f"(generated source {pct('match_generated')}, hand-written with a steering form {pct('match_steered')}); "
            f"identical behaviour with other bytes {pct('equivalent')} ({q['equivalent']['functions']:,} functions, a "
            f"match for later). Readable is a floor a tool can see, not a reading.")


HANDOFF = ROOT / "docs" / "HANDOFF.md"
BEGIN, END = "<!-- progress:begin -->", "<!-- progress:end -->"
WHOLE, BOOT = "Whole binary (`L` excluded)", "Boot slice (from `nnMain`, direct calls)"


def table_row(label: str, t: dict) -> str:
    b = t["by_status"]
    return (f"| {label} | {t['functions']:,} | {t['bytes'] / 1e6:.2f} MB | **{t['matching_bytes_pct']:.2f}%** "
            f"({b['O']['functions']:,}) | {t['usable_bytes_pct']:.2f}% | {b['U']['functions']:,} |")


def update_handoff(path: Path, overall: dict, boot: dict | None, quality_text: str | None = None) -> None:
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
                    help="the functions by the two qualities each needs: byte-matching, and readable (needs "
                         "build/check/<version>/)")
    ap.add_argument("--units", action="store_true",
                    help="the translation units by state (tools/tu_status.py has the per-unit views)")
    args = ap.parse_args()
    if args.handoff and args.slice_from:
        ap.error("--handoff takes the boot slice or none, not --slice-from")

    global VERIFIED
    VERIFIED = verified_addresses()
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
        report["quality"] = quality_report(args.version, game)
    if args.quality:
        print()
        print("completeness by the two qualities each function needs, byte-matching and readable (whole binary):")
        print_quality(report["quality"], overall)

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
                       quality_line(report["quality"], overall) if "quality" in report else None)
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(report, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
