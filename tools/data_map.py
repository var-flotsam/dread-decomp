#!/usr/bin/env python3
"""Recover which translation unit each address of main's .data and .bss belongs to, so a global can be
declared in its unit's header and, later, defined in its unit's source.

Method: the linker lays every section out in link order, which is the order of the units in .text
(tools/tu_map.py). The original's text is scanned for the data addresses each function names (adrp with
an add or a load/store, a load of a GOT slot followed to the address the slot holds). An address that the
functions of exactly one unit name is taken as that unit's: an anchor. Anchors that break the order are
dropped (the longest run in unit order is kept: 99.3% of them in both sections of 2.1.0), which is also
the evidence for the method. A unit's span in a section runs from its first anchor to its last. Between
two units' spans, an address that one of the two units names and no unit between them does extends that
unit's span, as long as the order holds. What is left between spans is not assigned.

A unit of tools/tu_map.py with medium or low confidence may hold several translation units; its span
here holds their data together, as its text span holds their functions.

data/data_map.csv columns: section,start,end,unit,anchors
  start/end: the first and last address known to be the unit's (end inclusive; the last object runs
  on past it by its size); unit: the index in data/tu_map.csv; anchors: the addresses behind the span.

Usage:
  python tools/data_map.py --version 2.1.0        # scan the original, write data/data_map.csv, print the evidence
  python tools/data_map.py --address 7101a1e1b8   # the unit of a data address (reads data/ only)
The scan reads the user's own dump; the query mode needs no game data.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import json
import struct
import sys
from collections import defaultdict
from dataclasses import dataclass
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import tu_map  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "data" / "data_map.csv"
SECTIONS = (".data", ".bss")
FIELDS = ["section", "start", "end", "unit", "anchors"]


@dataclass
class Span:
    section: str
    start: int
    end: int            # inclusive
    unit: int
    anchors: int


def named_addresses(code: bytes, address: int, got_slot) -> list[int]:
    """The addresses a function's code names through an adrp page: adrp + add, adrp + load/store, and for
    a 64-bit load of a GOT slot the address the slot holds (`got_slot(slot)` returns it, or None).

    The scan is linear: a register's page is forgotten at any other instruction that could write the
    register and at a call, so a reference can be missed, and one made up across a branch is rare (the
    order filter of spans() drops what does not fit)."""
    out: list[int] = []
    page: dict[int, int] = {}
    for i, w in enumerate(struct.unpack_from(f"<{len(code) // 4}I", code)):
        rd, rn = w & 31, (w >> 5) & 31
        if w & 0x9F000000 == 0x90000000:                              # adrp
            pc = address + 4 * i
            imm = ((w >> 5) & 0x7FFFF) << 2 | (w >> 29) & 3
            page[rd] = (pc & ~0xFFF) + ((imm - (1 << 21) if imm & (1 << 20) else imm) << 12)
        elif w & 0xFFC00000 == 0x91000000 and rn in page:              # add xd, xn, #imm
            out.append(page[rn] + ((w >> 10) & 0xFFF))
            page.pop(rd, None)
        elif w & 0x3B000000 == 0x39000000 and rn in page:              # load/store, unsigned offset
            scale = 4 if (w >> 26) & 1 and (w >> 23) & 1 else (w >> 30) & 3
            at = page[rn] + (((w >> 10) & 0xFFF) << scale)
            is_load = bool((w >> 22) & 1) or bool((w >> 23) & 1)
            held = got_slot(at) if is_load and scale == 3 and not (w >> 26) & 1 else None
            out.append(at if held is None else held)
            if is_load and not (w >> 26) & 1:
                page.pop(rd, None)
        elif w & 0xFC000000 == 0x94000000 or w & 0xFFFFFC1F == 0xD63F0000:   # bl, blr
            page = {r: p for r, p in page.items() if r > 18}
        else:
            page.pop(rd, None)
    return out


def in_order(anchors: list[tuple[int, int]]) -> list[tuple[int, int]]:
    """The longest subsequence of (address, unit), sorted by address, whose units never decrease."""
    tails: list[int] = []
    last: list[int] = []
    before: list[int | None] = []
    for i, (_a, unit) in enumerate(anchors):
        k = bisect.bisect_right(tails, unit)
        if k == len(tails):
            tails.append(unit)
            last.append(i)
        else:
            tails[k], last[k] = unit, i
        before.append(last[k - 1] if k else None)
    out = []
    i = last[-1] if last else None
    while i is not None:
        out.append(anchors[i])
        i = before[i]
    return out[::-1]


def spans(named: dict[int, set[int]], sections: dict[str, tuple[int, int]]) -> tuple[list[Span], dict[str, tuple[int, int]]]:
    """(the units' spans, per section (anchors, anchors kept in order)) from {address: units that name it}."""
    out: list[Span] = []
    evidence: dict[str, tuple[int, int]] = {}
    for section, (lo, hi) in sections.items():
        here = sorted(a for a in named if lo <= a < hi)
        single = [(a, next(iter(named[a]))) for a in here if len(named[a]) == 1]
        kept = in_order(single)
        evidence[section] = (len(single), len(kept))
        mine: list[Span] = []
        for a, unit in kept:
            if mine and mine[-1].unit == unit:
                mine[-1].end, mine[-1].anchors = a, mine[-1].anchors + 1
            else:
                mine.append(Span(section, a, a, unit, 1))
        for left, right in zip(mine, mine[1:]):
            gap = here[bisect.bisect_right(here, left.end):bisect.bisect_left(here, right.start)]
            sides = [{u for u in named[a] if left.unit <= u <= right.unit} for a in gap]
            n = 0
            while n < len(gap) and sides[n] in ({left.unit}, set()):     # forward, for the unit on the left
                if sides[n]:
                    left.end = gap[n]
                n += 1
            m = len(gap) - 1
            while m >= n and sides[m] in ({right.unit}, set()):          # backward, for the unit on the right
                if sides[m]:
                    right.start = gap[m]
                m -= 1
        out += mine
    return out, evidence


def write(found: list[Span]) -> None:
    with OUT.open("w", newline="", encoding="utf-8") as f:
        w = csv.writer(f, lineterminator="\n")
        w.writerow(FIELDS)
        for s in found:
            w.writerow([s.section, f"0x{s.start:x}", f"0x{s.end:x}", s.unit, s.anchors])


def load() -> list[Span]:
    if not OUT.is_file():
        raise SystemExit(f"{OUT.relative_to(ROOT).as_posix()} is missing: run tools/data_map.py --version <ver> first")
    with OUT.open(newline="", encoding="utf-8") as f:
        return [Span(r["section"], int(r["start"], 16), int(r["end"], 16), int(r["unit"]), int(r["anchors"]))
                for r in csv.DictReader(f)]


def finder(found: list[Span]):
    """address -> the unit index whose span holds it, or None."""
    found = sorted(found, key=lambda s: s.start)
    starts = [s.start for s in found]

    def find(address: int) -> int | None:
        i = bisect.bisect_right(starts, address) - 1
        return found[i].unit if i >= 0 and address <= found[i].end else None
    return find


def scan(version: str) -> tuple[dict[int, set[int]], dict[str, tuple[int, int]]]:
    """({data address: units whose functions name it}, {section: (start, end)}) from the user's dump."""
    import asmdiff
    from dread import functions

    original = asmdiff.Original(version)
    recon = json.loads((ROOT / "build" / "recon" / version / "main_segments.json").read_text(encoding="utf-8"))
    sections = {s["name"]: (int(s["start"], 16), int(s["start"], 16) + s["size"])
                for s in recon["sections"] if s["name"] in SECTIONS}
    lo, hi = min(a for a, _ in sections.values()), max(b for _, b in sections.values())
    units = tu_map.load_map()
    starts = [u["start_i"] for u in units]
    named: dict[int, set[int]] = defaultdict(set)

    def got_slot(slot: int) -> int | None:
        return original.got_slot(slot)[0]

    for row in functions.rows():
        address, size = int(row["address"], 16), int(row["size"])
        i = bisect.bisect_right(starts, address) - 1
        code = original.data_at(address, size)
        if i < 0 or address >= units[i]["end_i"] or not code:
            continue
        for target in named_addresses(code, address, got_slot):
            if lo <= target < hi:
                named[target].add(units[i]["index"])
    return named, sections


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="scan orig/<version> and write data/data_map.csv")
    ap.add_argument("--address", help="print the unit of a data address")
    args = ap.parse_args()
    if args.address:
        address = int(args.address.removeprefix("g_unk_").removeprefix("unk_"), 16)
        unit = finder(load())(address)
        if unit is None:
            print(f"0x{address:x}: in no unit's span (between two units, or outside .data and .bss)")
            return 1
        row = next(u for u in tu_map.load_map() if u["index"] == unit)
        print(f"0x{address:x}: unit {unit} ({row['start']}-{row['end']}), {row['class_hint'] or 'no class name'}, "
              f"{row['confidence']} confidence")
        return 0
    if not args.version:
        ap.error("give --version or --address")
    named, sections = scan(args.version)
    found, evidence = spans(named, sections)
    write(found)
    for section, (single, kept) in evidence.items():
        mine = [s for s in found if s.section == section]
        inside = sum(1 for a in named if sections[section][0] <= a < sections[section][1])
        print(f"{section}: {inside} addresses named, {single} by one unit, {kept} of those in unit order "
              f"({kept / max(1, single):.1%}); {len(mine)} units have a span")
    find = finder(found)
    print(f"{sum(1 for a in named if find(a) is not None)} of {len(named)} named addresses fall in a span; "
          f"{len(found)} spans written to {OUT.relative_to(ROOT).as_posix()}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
