#!/usr/bin/env python3
"""Status per translation unit: each unit of data/tu_map.csv with its functions counted by status.

The function list (data/functions/) says how far each function is; this says how far each unit is, the
way OdysseyDecomp's data/file_list.yml does per object file. It is derived, never a second source of
truth: a unit's row is its functions' rows of data/functions/ added up, with the verdicts of
data/equivalence.csv for the `m` ones. Everything it reads is tracked, so it needs no game data.

data/tu_status.csv columns:
  index,start,end,name,confidence: the unit, from data/tu_map.csv (name: its primary class hint, else
    UnkTU<index>; a name is only as sure as the confidence beside it);
  subsystem: the subsystem most of the unit's code bytes are in;
  state: matched (every function `O`), done (every function `O` or `m` verified equivalent by
    tools/equiv_test.py), partial (some of it written), untouched (every function `U`), library (only `L`
    functions; excluded from the totals, as in tools/progress.py), empty (no listed function);
  functions,bytes: the unit's game functions (`L` excluded) and their code bytes;
  O,m,verified,M,W,U,L: functions by status; verified: the `m` ones with a passing verdict;
  matched_bytes: bytes of the `O` functions; done_bytes: those and the verified `m` ones.

  (no option)     units by state, and the partial units with the fewest functions left
  --list STATE    the units in one state (--sort left|bytes|index, --top N)
  --unit U        one unit's functions and their statuses (U: an index, a unit name, a function's address
                  or its data/functions name)
  --quality       add the two qualities per unit (tools/progress.py --quality): how many functions are
                  byte-matching and readable, and the units where all are. Needs build/check/<version>/
  --write         rewrite data/tu_status.csv
  --check         exit 1 when data/tu_status.csv differs from what --write would write
  --summary F     append the units by state to F as markdown (for $GITHUB_STEP_SUMMARY), with the units
                  the change moves when --base is given

The pull request check (.github/workflows/policy.yml) writes the summary with --base. data/tu_status.csv
is refreshed with --write by the session that merges a round, like the README's status card: a file every
branch regenerated would conflict wherever two branches work in the same unit.

Usage: python tools/tu_status.py [--list partial] [--sort left] [--top 20]
       python tools/tu_status.py --unit CActor
       python tools/tu_status.py --quality [--version 2.1.0]
       python tools/tu_status.py --write | --check
       python tools/tu_status.py --summary FILE [--base REF]
"""

from __future__ import annotations

import argparse
import csv
import io
import subprocess
import sys
from collections import Counter
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import progress  # noqa: E402
import tu_map  # noqa: E402
from dread import functions  # noqa: E402
from dread.paths import DATA, ROOT  # noqa: E402

OUT = DATA / "tu_status.csv"
STATUSES = ["O", "m", "M", "W", "U", "L"]
COLUMNS = ["index", "start", "end", "name", "confidence", "subsystem", "state", "functions", "bytes",
           "O", "m", "verified", "M", "W", "U", "L", "matched_bytes", "done_bytes"]
STATES = {
    "matched": "every function `O`",
    "done": "every function `O` or verified equivalent",
    "partial": "some of it written",
    "untouched": "every function `U`",
    "library": "`L` only, not counted",
    "empty": "no listed function",
}
GAME_STATES = ("matched", "done", "partial", "untouched")
# With --quality: the keys of progress.QUALITIES counted per unit, and the unit-level reading of them.
QUALITY_KEYS = list(progress.QUALITIES)


def unit_name(u: dict) -> str:
    """The unit's primary class hint, else UnkTU<index> (the name tools/tu_map.py gives its folder)."""
    hint = u["class_hint"].split(";")[0] if u["class_hint"] else ""
    return hint or f"UnkTU{u['index']}"


def state(row: dict) -> str:
    game = row["functions"]
    if not game:
        return "library" if row["L"] else "empty"
    if row["O"] == game:
        return "matched"
    if row["O"] + row["verified"] == game:
        return "done"
    if row["U"] == game:
        return "untouched"
    return "partial"


def members(units: list[dict], rows: list[dict]) -> tuple[dict[int, list[dict]], list[dict]]:
    """Unit index -> its rows of the function list, and the rows that fall in no unit."""
    starts = tu_map.unit_starts(units)
    inside: dict[int, list[dict]] = {u["index"]: [] for u in units}
    outside: list[dict] = []
    for r in rows:
        u = tu_map.find_unit(units, int(r["address"], 16), starts)
        (inside[u["index"]] if u else outside).append(r)
    return inside, outside


def build(units: list[dict], rows: list[dict], verified: frozenset[int],
          quality: dict[int, tuple[str, bool]] | None = None) -> list[dict]:
    """One row per unit (COLUMNS; counts as ints). With `quality` (progress.quality_by_address) each row also
    counts its functions by the keys of progress.QUALITIES."""
    inside, _ = members(units, rows)
    out = []
    for u in units:
        row = {"index": u["index"], "start": u["start"], "end": u["end"], "name": unit_name(u),
               "confidence": u["confidence"], "functions": 0, "bytes": 0, "verified": 0,
               "matched_bytes": 0, "done_bytes": 0, **{s: 0 for s in STATUSES}}
        if quality is not None:
            row.update({k: 0 for k in QUALITY_KEYS})
        by_sub: Counter = Counter()
        for r in inside[u["index"]]:
            size, a = int(r["size"]), int(r["address"], 16)
            row[r["status"]] += 1
            by_sub[r["subsystem"].rstrip("?")] += size
            if r["status"] == "L":
                continue
            row["functions"] += 1
            row["bytes"] += size
            ver = r["status"] == "m" and a in verified
            row["verified"] += ver
            if r["status"] == "O":
                row["matched_bytes"] += size
            if r["status"] == "O" or ver:
                row["done_bytes"] += size
            if quality is not None and a in quality:
                row[quality[a][0]] += 1
        # The largest subsystem; ties go to the name that sorts first, so the file does not depend on row order.
        row["subsystem"] = min(by_sub, key=lambda k: (-by_sub[k], k)) if by_sub else ""
        row["state"] = state(row)
        out.append(row)
    return out


def render(table: list[dict]) -> str:
    buf = io.StringIO()
    w = csv.DictWriter(buf, fieldnames=COLUMNS, lineterminator="\n", extrasaction="ignore")
    w.writeheader()
    w.writerows(table)
    return buf.getvalue()


def left(row: dict) -> int:
    """Functions of the unit that are not `O` yet."""
    return row["functions"] - row["O"]


def by_state(table: list[dict]) -> dict[str, dict]:
    out = {s: {"units": 0, "functions": 0, "bytes": 0} for s in STATES}
    for r in table:
        t = out[r["state"]]
        t["units"] += 1
        t["functions"] += r["functions"] or r["L"]
        t["bytes"] += r["bytes"]
    return out


def state_table(table: list[dict]) -> list[str]:
    """The units by state, as a markdown table (the terminal and the job summary print the same one)."""
    t = by_state(table)
    game_units = sum(t[s]["units"] for s in GAME_STATES)
    game_bytes = sum(t[s]["bytes"] for s in GAME_STATES)
    out = ["| State | Units | Share of units | Functions | Bytes | Share of code |", "|---|---|---|---|---|---|"]
    for s in STATES:
        v = t[s]
        if not v["units"]:
            continue
        if s in GAME_STATES:
            out.append(f"| {s} ({STATES[s]}) | {v['units']:,} | {100 * v['units'] / game_units:.1f}% | "
                       f"{v['functions']:,} | {v['bytes']:,} | {100 * v['bytes'] / game_bytes:.1f}% |")
        else:
            out.append(f"| {s} ({STATES[s]}) | {v['units']:,} | | {v['functions']:,} | | |")
    return out


def unit_line(r: dict) -> str:
    rest = ", ".join(f"{r[s]} {s}" for s in ("m", "M", "W", "U") if r[s])
    pct = 100 * r["matched_bytes"] / r["bytes"] if r["bytes"] else 0.0
    return (f"{r['index']:>5} {r['state']:<9} {r['O']:>5}/{r['functions']:<5} {pct:>6.1f}% {r['bytes']:>8,} "
            f"{r['confidence']:<6} {r['name'][:48]:<48} {rest}")


HEADER = f"{'index':>5} {'state':<9} {'O':>5}/{'fns':<5} {'match':>7} {'bytes':>8} {'conf':<6} {'name':<48} not O"
SORTS = {
    "left": lambda r: (left(r), -r["bytes"], r["index"]),
    "bytes": lambda r: (-r["bytes"], r["index"]),
    "index": lambda r: r["index"],
}


def print_report(table: list[dict], outside: list[dict], listed: int, top: int) -> None:
    where = Counter(r["subsystem"] for r in outside)
    print(f"translation units: {len(table):,} in data/tu_map.csv; {len(outside):,} of the {listed:,} listed functions "
          "fall in none of them" + (f" ({', '.join(f'{n:,} {k}' for k, n in where.most_common(3))})" if where else ""))
    print()
    print("\n".join(state_table(table)))
    near = sorted((r for r in table if r["state"] == "partial"), key=SORTS["left"])[:top]
    if near:
        print()
        print(f"partial units with the fewest functions left ({len(near)} shown; --list partial for all):")
        print(HEADER)
        for r in near:
            print(unit_line(r))


def print_quality(table: list[dict]) -> None:
    """The two qualities per unit. A unit is complete when every function of it is byte-matching and readable."""
    game = [r for r in table if r["state"] in GAME_STATES]
    complete = [r for r in game if r["complete"] == r["functions"]]
    generated = [r for r in game if r["state"] == "matched" and r["match_generated"] == r["functions"]]
    some = [r for r in game if 0 < r["complete"] < r["functions"]]
    print()
    print("units by the two qualities each function needs, byte-matching and readable (tools/progress.py --quality):")
    print(f"  complete (every function byte-matching and readable): {len(complete):,} units, "
          f"{sum(r['bytes'] for r in complete):,} bytes")
    print(f"  matched by generated source alone: {len(generated):,} units, {sum(r['bytes'] for r in generated):,} bytes")
    print(f"  with some complete functions: {len(some):,} units")
    near = sorted(some, key=lambda r: (r["functions"] - r["complete"], -r["bytes"], r["index"]))[:15]
    if near:
        print("  nearest to complete (functions that are not byte-matching and readable yet):")
        for r in near:
            print(f"  {r['index']:>5} {r['complete']:>5}/{r['functions']:<5} {r['name'][:48]:<48} "
                  + ", ".join(f"{r[k]} {k}" for k in QUALITY_KEYS[1:] if r[k]))


def resolve_unit(units: list[dict], text: str) -> dict:
    """A unit by index, by name (its primary hint, or any class it holds), or by a function in it."""
    t = text.strip()
    if t.isdigit() and int(t) < len(units):
        return units[int(t)]
    named = [u for u in units if unit_name(u) == t] or [u for u in units if t in u["class_hint"].split(";")]
    if len(named) == 1:
        return named[0]
    if named:
        raise SystemExit(f"{t}: {len(named)} units have this name; pass an index: "
                         + ", ".join(str(u["index"]) for u in named[:8]))
    address = tu_map.resolve_function(t, tu_map.load_functions())
    u = tu_map.find_unit(units, address)
    if u is None:
        raise SystemExit(f"0x{address:x}: not in any unit")
    return u


def show_unit(units: list[dict], rows: list[dict], verified: frozenset[int], text: str) -> None:
    u = resolve_unit(units, text)
    row = next(r for r in build([u], rows, verified))
    hints = u["class_hint"].split(";") if u["class_hint"] else []
    print(f"unit {u['index']}: {row['name']} ({u['start']}-{u['end']}, confidence {u['confidence']}, "
          f"{row['subsystem'] or 'no subsystem'}): {row['state']}")
    print(f"  {row['O']} of {row['functions']} functions `O`, {row['matched_bytes']:,} of {row['bytes']:,} bytes; "
          + (", ".join(f"{row[s]} {s}" for s in STATUSES[1:] if row[s]) or "nothing else")
          + (f"; {row['verified']} of the m verified equivalent" if row["m"] else ""))
    if len(hints) > 1:
        print(f"  also in this unit: {', '.join(hints[1:])}")
    print(f"  folder for a new source: src/{tu_map.source_folder(u)}")
    for r in members([u], rows)[0][u["index"]]:
        mark = " (verified equivalent)" if r["status"] == "m" and int(r["address"], 16) in verified else ""
        print(f"  {r['status']} {r['address']} {int(r['size']):>6} {r['name']}{mark}")


# ---------------------------------------------------------------------------------------------
# The job summary

def git_show(ref: str, path: str) -> str | None:
    p = subprocess.run(["git", "show", f"{ref}:{path}"], cwd=ROOT, capture_output=True, encoding="utf-8")
    return p.stdout if p.returncode == 0 else None


def verified_in(text: str | None) -> frozenset[int]:
    """The passing rows of a data/equivalence.csv text, as progress.verified_addresses reads the file."""
    import equiv_test
    if not text:
        return frozenset()
    return frozenset(int(r["address"], 16) for r in csv.DictReader(io.StringIO(text))
                     if r["verdict"] == "pass" and r["rules"] == str(equiv_test.RULES))


def table_at(ref: str, units: list[dict]) -> list[dict]:
    """The table for the function list and the verdicts committed at `ref`, over today's units."""
    import status_card
    return build(units, status_card.rows_at(ref), verified_in(git_show(ref, "data/equivalence.csv")))


MOVED_LIMIT = 40


def summary(table: list[dict], base: list[dict] | None, ref: str | None) -> list[str]:
    out = ["## Translation units", "", *state_table(table), ""]
    if base is None:
        return out
    before = {r["index"]: r for r in base}
    moved = [r for r in table if any(before[r["index"]][k] != r[k] for k in ("state", *STATUSES, "verified"))]
    if not moved:
        return out + [f"No unit changes against `{ref}`.", ""]
    finished = sum(r["state"] in ("matched", "done") and before[r["index"]]["state"] != r["state"] for r in moved)
    out += [f"This change moves {len(moved):,} unit{'s' if len(moved) != 1 else ''} against `{ref}`"
            + (f"; {finished:,} newly matched or done" if finished else "") + ".", "",
            "| Unit | Name | State | `O` of functions | Left |", "|---|---|---|---|---|"]
    for r in moved[:MOVED_LIMIT]:
        b = before[r["index"]]
        st = r["state"] if b["state"] == r["state"] else f"{b['state']} → **{r['state']}**"
        rest = ", ".join(f"{r[s]} `{s}`" for s in ("m", "M", "W", "U") if r[s]) or "nothing"
        out.append(f"| {r['index']} | `{r['name']}` | {st} | {b['O']:,} → {r['O']:,} of {r['functions']:,} | {rest} |")
    if len(moved) > MOVED_LIMIT:
        out += ["", f"({len(moved) - MOVED_LIMIT:,} more units not shown.)"]
    return out + [""]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    mode = ap.add_mutually_exclusive_group()
    mode.add_argument("--write", action="store_true", help="rewrite data/tu_status.csv")
    mode.add_argument("--check", action="store_true", help="exit 1 when data/tu_status.csv is out of date")
    mode.add_argument("--list", choices=list(STATES), metavar="STATE", help="the units in one state: " + ", ".join(STATES))
    mode.add_argument("--unit", metavar="INDEX|NAME|ADDR", help="one unit's functions and their statuses")
    ap.add_argument("--sort", choices=list(SORTS), default="left", help="order of --list (default: fewest functions left)")
    ap.add_argument("--top", type=int, default=20, help="rows to show (default 20; 0 for all with --list)")
    ap.add_argument("--quality", action="store_true",
                    help="add byte-matching and readable per unit (needs build/check/<version>/)")
    ap.add_argument("--version", default="2.1.0", help="dump version whose check cache --quality reads (default 2.1.0)")
    ap.add_argument("--summary", type=Path, help="append the units by state as markdown to this file")
    ap.add_argument("--base", help="with --summary: the ref to compare the units against")
    args = ap.parse_args()

    units = tu_map.load_map()
    rows = functions.rows()
    verified = progress.verified_addresses()
    if args.unit:
        show_unit(units, rows, verified, args.unit)
        return 0
    quality = None
    if args.quality:
        quality = progress.quality_by_address(args.version, [r for r in rows if r["status"] != "L"], verified)
    table = build(units, rows, verified, quality)

    if args.write:
        OUT.write_text(render(table), encoding="utf-8", newline="\n")
        print(f"wrote {OUT.relative_to(ROOT).as_posix()}: {len(table):,} units")
    elif args.check:
        if not OUT.is_file() or OUT.read_text(encoding="utf-8") != render(table):
            print(f"tu_status: {OUT.relative_to(ROOT).as_posix()} is out of date; run python tools/tu_status.py --write")
            return 1
        print(f"tu_status: {OUT.relative_to(ROOT).as_posix()} is current")
    elif args.list:
        picked = sorted((r for r in table if r["state"] == args.list), key=SORTS[args.sort])
        print(HEADER)
        for r in picked[:args.top or None]:
            print(unit_line(r))
        print(f"{len(picked):,} {args.list} units" + (f" ({args.top} shown; --top 0 for all)" if 0 < args.top < len(picked) else ""))
    elif not args.summary:
        print_report(table, members(units, rows)[1], len(rows), args.top)
    if quality is not None and not (args.write or args.check):
        print_quality(table)
    if args.summary:
        base = table_at(args.base, units) if args.base else None
        with args.summary.open("a", encoding="utf-8") as fh:
            fh.write("\n".join(summary(table, base, args.base)) + "\n")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
