#!/usr/bin/env python3
"""Function-pointer tables of main (virtual tables, mostly) and the slot families they form: which
functions share a slot, so one slot name names every override and a function's matched siblings can
be read beside it.

Method: every R_AARCH64_RELATIVE relocation whose target is the start of a function in
data/functions is a pointer slot; a maximal run of consecutive slots (8 bytes apart) is a table.
The game is built without RTTI, so a class's virtual table is such a run, bounded by the two words
in front of it that carry no function pointer; runs under --min-slots (3) are left out. Tables are
grouped into families: two tables belong together when they hold the same function at the same slot
(a derived class that inherits an implementation), bodies of 8 bytes or less excepted, because the
linker folds those across unrelated classes. A family is therefore an approximation of one class
hierarchy, not a recovered one: a derived class that overrides every slot starts a family of its
own, and a long table that is not a virtual table (a registration table) is a family of one.

build/vtables/<version>/slots.csv columns: table,slot,function,family
  table: the address of the table's first slot; slot: the index in it; function: the address the
  slot holds; family: the family's number, largest first by the functions it holds. The file is
  derived from the user's dump and is never tracked.

Usage:
  python tools/vtables.py --version 2.1.0                   # write build/vtables/2.1.0/slots.csv and print the summary
  python tools/vtables.py --version 2.1.0 --summary --top 20   # the largest families: tables, slots, functions, unnamed, U
  python tools/vtables.py --version 2.1.0 --function 7100b8ebb8   # its tables and slots, and its siblings by status
  python tools/vtables.py --version 2.1.0 --family 0 [--slot 12]  # a family slot by slot: implementations, unnamed, matched
The query modes read slots.csv and data/functions/ only.
"""

from __future__ import annotations

import argparse
import csv
import struct
import sys
from collections import defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread import functions  # noqa: E402
from dread.paths import BUILD, orig  # noqa: E402

BASE = 0x7100000000
R_AARCH64_RELATIVE = 1027
PT_LOAD, PT_DYNAMIC, DT_RELA, DT_RELASZ = 1, 2, 7, 8
COLUMNS = ["table", "slot", "function", "family"]
FOLDED = 8          # bodies this small are shared by unrelated tables (identical-code folding)
MATCHED = ("O", "m")


def relative_pointers(elf: bytes) -> dict[int, int]:
    """Slot address -> the address its R_AARCH64_RELATIVE relocation stores, both absolute."""
    if elf[:4] != b"\x7fELF" or elf[4] != 2:
        raise ValueError("not a 64-bit ELF file")
    phoff, = struct.unpack_from("<Q", elf, 0x20)
    phentsize, phnum = struct.unpack_from("<HH", elf, 0x36)
    loads, dynamic = [], None
    for i in range(phnum):
        ptype, _, off, vaddr, _, filesz, _, _ = struct.unpack_from("<IIQQQQQQ", elf, phoff + i * phentsize)
        if ptype == PT_LOAD:
            loads.append((vaddr, off, filesz))
        elif ptype == PT_DYNAMIC:
            dynamic = (off, filesz)
    if dynamic is None:
        raise ValueError("no dynamic segment")

    def offset(vaddr: int) -> int:
        for start, off, size in loads:
            if start <= vaddr < start + size:
                return off + vaddr - start
        raise ValueError(f"address 0x{vaddr:x} is in no loaded segment")

    rela = relasz = None
    for at in range(dynamic[0], dynamic[0] + dynamic[1], 16):
        tag, value = struct.unpack_from("<qQ", elf, at)
        if tag == DT_RELA:
            rela = value
        elif tag == DT_RELASZ:
            relasz = value
        elif tag == 0:
            break
    if rela is None or relasz is None:
        raise ValueError("no DT_RELA table")
    # nso2elf.py writes main.elf module-relative; an image already at its base keeps its addresses.
    bias = BASE if min(v for v, _, _ in loads) < BASE else 0
    out = {}
    start = offset(rela)
    for at in range(start, start + relasz, 24):
        r_offset, r_info, r_addend = struct.unpack_from("<QQq", elf, at)
        if r_info & 0xFFFFFFFF == R_AARCH64_RELATIVE:
            out[r_offset + bias] = r_addend + bias
    return out


def find_tables(pointers: dict[int, int], starts: set[int], min_slots: int = 3) -> list[tuple[int, list[int]]]:
    """(address, functions by slot) of every run of at least `min_slots` consecutive function pointers."""
    slots = {a: t for a, t in pointers.items() if t in starts}
    tables = []
    for a in sorted(slots):
        if a - 8 in slots:
            continue
        run, at = [], a
        while at in slots:
            run.append(slots[at])
            at += 8
        if len(run) >= min_slots:
            tables.append((a, run))
    return tables


def group_families(tables: list[tuple[int, list[int]]], sizes: dict[int, int]) -> list[int]:
    """The family number of each table: tables holding one function (over FOLDED bytes) at one slot
    are joined; numbered from 0, the family with the most distinct functions first."""
    parent = list(range(len(tables)))

    def find(x: int) -> int:
        while parent[x] != x:
            parent[x] = parent[parent[x]]
            x = parent[x]
        return x

    first: dict[tuple[int, int], int] = {}
    for i, (_, run) in enumerate(tables):
        for slot, fn in enumerate(run):
            if sizes.get(fn, 0) <= FOLDED:
                continue
            j = first.setdefault((slot, fn), i)
            if j != i:
                parent[find(i)] = find(j)
    members: dict[int, set[int]] = defaultdict(set)
    for i, (_, run) in enumerate(tables):
        members[find(i)].update(run)
    lowest = {}
    for i, (addr, _) in enumerate(tables):
        lowest.setdefault(find(i), addr)
    order = sorted(members, key=lambda r: (-len(members[r]), lowest[r]))
    number = {r: n for n, r in enumerate(order)}
    return [number[find(i)] for i in range(len(tables))]


def build(version: str, min_slots: int) -> list[dict]:
    elf = orig(version) / "main.elf"
    if not elf.exists():
        sys.exit(f"{elf} is missing: tools/setup.py and tools/nso2elf.py produce it from your dump")
    sizes = {int(r["address"], 16): int(r["size"]) for r in functions.rows()}
    tables = find_tables(relative_pointers(elf.read_bytes()), set(sizes), min_slots)
    families = group_families(tables, sizes)
    return [{"table": f"0x{addr:x}", "slot": slot, "function": f"0x{fn:x}", "family": fam}
            for (addr, run), fam in zip(tables, families) for slot, fn in enumerate(run)]


def slots_path(version: str) -> Path:
    return BUILD / "vtables" / version / "slots.csv"


def write(version: str, rows: list[dict]) -> Path:
    path = slots_path(version)
    path.parent.mkdir(parents=True, exist_ok=True)
    with path.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=COLUMNS, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    return path


def read(version: str) -> list[dict]:
    path = slots_path(version)
    if not path.exists():
        sys.exit(f"{path} is missing: run tools/vtables.py --version {version} first")
    with path.open(newline="", encoding="utf-8") as fh:
        return [{"table": int(r["table"], 16), "slot": int(r["slot"]), "function": int(r["function"], 16),
                 "family": int(r["family"])} for r in csv.DictReader(fh)]


def sibling_map(version: str) -> dict[int, set[int]]:
    """Function -> the functions that share one of its (family, slot) places, itself included; empty when
    slots.csv has not been built. The lean kit shows a worker a matched sibling (tools/batch/lean/similar.py)."""
    if not slots_path(version).exists():
        return {}
    places: dict[tuple[int, int], set[int]] = defaultdict(set)
    for s in read(version):
        places[(s["family"], s["slot"])].add(s["function"])
    out: dict[int, set[int]] = defaultdict(set)
    for fns in places.values():
        if len(fns) > 1:
            for fn in fns:
                out[fn] |= fns
    return dict(out)


def unnamed(row: dict) -> bool:
    return row["name_source"] == "unknown"


def tally(addresses, funcs: dict[int, dict]) -> dict:
    """Counts and bytes of a set of functions, library code left out."""
    rows = [funcs[a] for a in set(addresses) if a in funcs and funcs[a]["status"] != "L"]
    nameless = [r for r in rows if unnamed(r)]
    undone = [r for r in rows if r["status"] == "U"]

    def size(rs):
        return sum(int(r["size"]) for r in rs)

    return {"functions": len(rows), "bytes": size(rows), "unnamed": len(nameless), "unnamed_bytes": size(nameless),
            "U": len(undone), "U_bytes": size(undone)}


def summary(slots: list[dict], funcs: dict[int, dict], top: int) -> list[str]:
    families: dict[int, list[dict]] = defaultdict(list)
    for s in slots:
        families[s["family"]].append(s)
    total = tally((s["function"] for s in slots), funcs)
    out = [f"tables: {len({s['table'] for s in slots}):,} in {len(families):,} families; "
           f"functions in them: {total['functions']:,} ({total['bytes']:,} bytes), "
           f"unnamed {total['unnamed']:,} ({total['unnamed_bytes']:,} bytes), "
           f"U {total['U']:,} ({total['U_bytes']:,} bytes)",
           "", "| Family | Tables | Slots | Functions | Unnamed | Unnamed bytes | U | U bytes |", "|---|---|---|---|---|---|---|---|"]
    for fam in sorted(families)[:top]:
        rows = families[fam]
        t = tally((s["function"] for s in rows), funcs)
        out.append(f"| {fam} | {len({s['table'] for s in rows}):,} | {max(s['slot'] for s in rows) + 1} | "
                   f"{t['functions']:,} | {t['unnamed']:,} | {t['unnamed_bytes']:,} | {t['U']:,} | {t['U_bytes']:,} |")
    return out


def family_view(slots: list[dict], funcs: dict[int, dict], family: int, slot: int | None) -> list[str]:
    rows = [s for s in slots if s["family"] == family]
    if not rows:
        sys.exit(f"no family {family}")
    by_slot: dict[int, set[int]] = defaultdict(set)
    for s in rows:
        by_slot[s["slot"]].add(s["function"])
    if slot is not None:
        out = [f"family {family}, slot {slot}: {len(by_slot[slot])} implementations", "",
               "| Function | Name | Size | Status |", "|---|---|---|---|"]
        for a in sorted(by_slot[slot]):
            r = funcs[a]
            out.append(f"| 0x{a:x} | {r['name']} | {r['size']} | {r['status']} |")
        return out
    out = [f"family {family}: {len({s['table'] for s in rows}):,} tables, {len(by_slot)} slots", "",
           "| Slot | Implementations | Unnamed | Matched | U | A name already given |", "|---|---|---|---|---|---|"]
    for n in sorted(by_slot):
        rs = [funcs[a] for a in by_slot[n]]
        named = sorted({r["name"].rsplit("::", 1)[-1] for r in rs if not unnamed(r)})
        out.append(f"| {n} | {len(rs)} | {sum(unnamed(r) for r in rs)} | {sum(r['status'] in MATCHED for r in rs)} | "
                   f"{sum(r['status'] == 'U' for r in rs)} | {', '.join(named[:3])} |")
    return out


def resolve(text: str, funcs: dict[int, dict]) -> int:
    try:
        address = int(text, 16)
        if address in funcs:
            return address
    except ValueError:
        pass
    hits = [a for a, r in funcs.items() if r["name"] == text]
    if len(hits) != 1:
        sys.exit(f"{text}: {'no such function' if not hits else f'{len(hits)} functions carry that name; give an address'}")
    return hits[0]


def function_view(slots: list[dict], funcs: dict[int, dict], address: int, limit: int) -> list[str]:
    mine = [s for s in slots if s["function"] == address]
    r = funcs[address]
    if not mine:
        return [f"0x{address:x} {r['name']}: in no table of {len({s['table'] for s in slots}):,}"]
    out = [f"0x{address:x} {r['name']} ({r['size']} bytes, {r['status']}): in {len(mine)} table slots"]
    for family, slot in sorted({(s["family"], s["slot"]) for s in mine}):
        siblings = {s["function"] for s in slots if s["family"] == family and s["slot"] == slot} - {address}
        rows = sorted((funcs[a] for a in siblings),
                      key=lambda x: (x["status"] not in MATCHED, abs(int(x["size"]) - int(r["size"]))))
        matched = sum(x["status"] in MATCHED for x in rows)
        out += ["", f"family {family}, slot {slot}: {len(rows)} siblings, {matched} matched "
                    f"(matched first, then nearest in size)", "", "| Function | Name | Size | Status |", "|---|---|---|---|"]
        out += [f"| {x['address']} | {x['name']} | {x['size']} | {x['status']} |" for x in rows[:limit]]
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--min-slots", type=int, default=3, help="shortest run that counts as a table (default 3)")
    ap.add_argument("--summary", action="store_true", help="print the summary from slots.csv without rebuilding it")
    ap.add_argument("--function", help="address or data/functions name: its slots and its siblings")
    ap.add_argument("--family", type=int, help="list a family slot by slot")
    ap.add_argument("--slot", type=int, help="with --family: the implementations of one slot")
    ap.add_argument("--top", type=int, default=10, help="rows to print (default 10)")
    args = ap.parse_args()

    funcs = {int(r["address"], 16): r for r in functions.rows()}
    if args.function is not None:
        lines = function_view(read(args.version), funcs, resolve(args.function, funcs), args.top)
    elif args.family is not None:
        lines = family_view(read(args.version), funcs, args.family, args.slot)
    elif args.summary:
        lines = summary(read(args.version), funcs, args.top)
    else:
        path = write(args.version, build(args.version, args.min_slots))
        lines = [f"wrote {path}", *summary(read(args.version), funcs, args.top)]
    print("\n".join(lines))
    return 0


if __name__ == "__main__":
    sys.exit(main())
