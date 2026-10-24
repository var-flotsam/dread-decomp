#!/usr/bin/env python3
"""Recover main's translation-unit (TU) layout from .init_array, so new sources can be named and
placed by the unit (class) they belong to instead of by address slice.

Method: every entry of .init_array (an R_AARCH64_RELATIVE addend in .rela.dyn) is a TU's static
initialiser (Clang's _GLOBAL__sub_I_<file>, emitted last in its TU's text), and the entries are
in link order, which is address order here. Consecutive initialisers therefore bound TU spans;
a TU without a static initialiser merges into the span of the next one. Spans are refined with
the reflection TU groups (a run of class getters whose guards and descriptors are laid out
together is one TU, see tools/gen_reflection.py): a span holding several unrelated groups is
split in front of each later group's reflection boilerplate, which opens its TU (the class
attribute helper fn_0x70 first, then the getter: the getter is the second function of its unit
in 93% of one-group units). A span is named by the reflected classes whose getters it holds,
else by the class its initialiser registers, else by a source-path-like string it references,
else by the qualifier of its named functions. Lua, dlmalloc (address ranges from
data/functions) and nn::atk (from its first named function to the end of .text) are marked as
library units.

data/tu_map.csv columns: index,start,end,functions,bytes,init_fn,class_hint,hint_source,confidence
  start/end: the unit's address range (end exclusive); functions: data/functions rows in it;
  init_fn: the unit's static initialiser, when it has one; class_hint: the primary name first,
  the unit's other classes after it, ';'-separated; hint_source: reflection | init_call | string
  | names | library | (empty); confidence: high = bounded on both sides (by an initialiser, or
  by a library carve-out) and ending with its own initialiser, at most one reflection group and
  at most 64 KB; medium = a piece of a span split between groups, a span whose groups are nested
  types of one class, or a bounded span over 64 KB with a group (it holds several TUs; the group
  names only one of them); low = library units (lua, dlmalloc, nn::atk: the label names a
  library, not a TU), units without an initialiser of their own that are not split pieces (the
  first unit, the text after the last initialiser, the text in front of a library carve-out),
  and group-less spans over 64 KB.

Usage:
  python tools/tu_map.py --version 2.1.0        # write data/tu_map.csv, print the sanity checks and layout statistics
  python tools/tu_map.py --function 71003f13fc  # the unit of a function and its hint, for naming a file
  python tools/tu_map.py --function CActor::refl_GetClass   # the same, by a data/functions name
  python tools/tu_map.py --report               # coverage, confidence histogram, U functions per hint
  python tools/tu_map.py --top 20               # the largest units
The query modes read data/tu_map.csv and data/functions/ only; they need no game data.
"""

from __future__ import annotations

import argparse
import bisect
import csv
import json
import re
import sys
from collections import Counter, defaultdict
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
import policy_check  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "data" / "tu_map.csv"
COLUMNS = ["index", "start", "end", "functions", "bytes", "init_fn", "class_hint", "hint_source", "confidence"]
BASE = 0x7100000000
NNMAIN = 0x71000018F0
LARGE_SPAN = 64 * 1024
# Library units carved out by address range: data/functions subsystem -> unit name.
RANGE_LIBRARIES = {"lua": "lua", "dlmalloc": "dlmalloc"}
# nn::atk is statically linked at the end of .text: every unit from its first directly named
# function onwards is a library unit.
ATK_SUBSYSTEM = "middleware/nn-atk"
PATH_RE = re.compile(r"[.](h|hpp|hh|inl|c|cc|cpp|lua)$", re.I)
CLASS_RE = re.compile(r"^[CST][A-Z]")   # Mercury's class/struct/template naming (CActor, SParams, TFoo)
UNNAMED = ("fn_", "FUN_", "<EXTERNAL>", "thunk_")
HAND_SLICES = ROOT / "src" / "game_s2"
FILE_ADDR_RE = re.compile(r"^fn_(7[0-9a-f]{9})[.]cpp$")


@dataclass
class Fn:
    address: int
    size: int
    name: str
    name_source: str
    subsystem: str
    status: str


@dataclass
class Unit:
    lo: int                      # first function (index into the function list)
    hi: int                      # one past the last function
    init_fn: int | None = None
    bounded_left: bool = False   # starts right after another unit's initialiser
    bounded_right: bool = False  # ends with its own initialiser
    blocks: list = field(default_factory=list)   # reflection groups (lists of gen_reflection.Getter)
    split: bool = False          # a piece of a span split between groups, or a merged nested-type span
    library: str = ""
    hint: str = ""
    source: str = ""


def load_functions() -> list[Fn]:
    out = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                out.append(Fn(int(r["address"], 16), int(r["size"]), r["name"], r["name_source"],
                              r["subsystem"], r["status"]))
    out.sort(key=lambda x: x.address)
    return out


def qualifier(name: str) -> str | None:
    """The scope of a named function, for a hint: `ns::CFoo::Bar<T>` -> `ns::CFoo`; a bare name as is."""
    if name.startswith(UNNAMED):
        return None
    base = name.split("<", 1)[0]
    return base.rsplit("::", 1)[0] if "::" in base else base


# ---------------------------------------------------------------------------------------------
# Building the map (needs the dump and the recon/Ghidra exports)

def read_init_array(orig) -> list[int]:
    import nso2elf
    img = orig.img
    d = img.mod.dyn
    addr, size = d[nso2elf.DT["INIT_ARRAY"]], d[nso2elf.DT["INIT_ARRAYSZ"]]
    out = []
    for off in range(addr, addr + size, 8):
        p = img.pointer(off)
        if p is None:  # stored as data bytes rather than as a RELATIVE addend
            p = img.i64(off)
            if p >= BASE:
                p -= BASE
        out.append(BASE + p)
    return out


def reflection_blocks(orig, funcs: list[Fn]) -> tuple[list[list], dict[str, str], set[str]]:
    """The reflection TU groups (as in gen_reflection.main), type -> parent, and all reflected type names."""
    import gen_reflection as gr
    with (ROOT / "data" / "reflection_types.csv").open(newline="", encoding="utf-8") as f:
        all_rows = list(csv.DictReader(f))
    rows = [r for r in all_rows if r["kind"] == "class"]
    getters = {int(r["getter"], 16) for r in rows}
    inits = {f.address for f in funcs if f.name.endswith("::refl_InitClass")}
    ref_sig = gr.shape(orig.function(gr.REFERENCE_GETTER))
    gs = [gr.Getter(r["type"], int(r["getter"], 16)) for r in rows]
    for g in gs:
        gr.analyse(orig, g, getters, ref_sig, inits)
    located = sorted((g for g in gs if g.guard and g.descriptor), key=lambda g: g.guard)
    blocks: list[list] = []
    for g in located:
        if blocks and g.guard - blocks[-1][-1].guard == 8:
            blocks[-1].append(g)
        else:
            blocks.append([g])
    return blocks, {r["type"]: r["parent"] for r in rows}, {r["type"] for r in all_rows}


def load_calls(version: str) -> dict[int, list[int]]:
    calls = {}
    path = ROOT / "build" / "ghidra_export" / version / "main_functions.jsonl"
    with path.open(encoding="utf-8") as f:
        for line in f:
            j = json.loads(line)
            calls[int(j["entry"], 16)] = [int(x, 16) for x in j["calls"]]
    return calls


def load_path_strings(version: str) -> dict[int, list[str]]:
    """Function -> source-path-like strings it references (a directory separator plus a source suffix)."""
    out = defaultdict(list)
    path = ROOT / "build" / "ghidra_export" / version / "main_strings.jsonl"
    with path.open(encoding="utf-8") as f:
        for line in f:
            s = json.loads(line)
            v = s["value"]
            if not PATH_RE.search(v) or not ("/" in v or chr(92) in v) or policy_check.HOME_PATH.search(v):
                continue
            for fn in s["funcs"]:
                out[int(fn, 16)].append(v)
    return out


def related(b1: list, b2: list, types: set[str]) -> bool:
    """Two groups that belong to one TU: a class and its nested types (CFoo and CFoo::CTunableFoo),
    or two nested types of one class (CFoo::CParams and CFoo::CTunableFoo). The enclosing scope is
    a class when it is a reflected type or is named like one (CFoo, SFoo, TFoo); GUI and
    behaviortree are namespaces."""
    def outer(n: str) -> str | None:
        return n.rsplit("::", 1)[0] if "::" in n else None

    def is_class(scope: str) -> bool:
        return scope in types or bool(CLASS_RE.match(scope.rsplit("::", 1)[-1]))
    for x in (g.type for g in b1):
        for y in (g.type for g in b2):
            if x.startswith(y + "::") or y.startswith(x + "::"):
                return True
            if outer(x) is not None and outer(x) == outer(y) and is_class(outer(x)):
                return True
    return False


def split_groups(u: Unit, addrs: list[int], types: set[str]) -> list[Unit]:
    """Split a span holding several unrelated reflection groups before each later group's first
    getter (or the few reflection helpers directly in front of it)."""
    blocks = sorted(u.blocks, key=lambda b: min(g.address for g in b))
    groups: list[list] = [[blocks[0]]]
    for b in blocks[1:]:
        if related(groups[-1][-1], b, types):
            groups[-1].append(b)
        else:
            groups.append([b])
    if len(groups) == 1:
        u.split = len(blocks) > 1
        return [u]

    def index(a: int) -> int:
        return bisect.bisect_left(addrs, a)
    cuts: list[int] = []
    for k in range(1, len(groups)):
        first = min(g.address for b in groups[k] for g in b)
        i0 = index(first)
        floor = addrs[max(i0 - 4, u.lo)]
        cands = [first] + [x for b in groups[k] for g in b for x in (g.init, g.fn_0x70, g.fn_0xe0)
                           if x and floor <= x < first]
        cut = index(min(cands))
        # The helpers may not pull the cut back past the previous groups' getters or the previous
        # cut: every piece keeps its own getters and is non-empty.
        last_prev = max(index(g.address) for b in groups[k - 1] for g in b)
        cut = max(cut, last_prev + 1, (cuts[-1] if cuts else u.lo) + 1)
        cuts.append(cut)
    pieces = []
    lo = u.lo
    for k, cut in enumerate(cuts + [u.hi]):
        p = Unit(lo, cut, split=True, bounded_left=u.bounded_left if k == 0 else False,
                 bounded_right=u.bounded_right if cut == u.hi else False, blocks=groups[k])
        if u.init_fn is not None and addrs[lo] <= u.init_fn <= addrs[cut - 1]:
            p.init_fn = u.init_fn
        pieces.append(p)
        lo = cut
    for p in pieces:
        assert p.hi > p.lo, f"empty piece at 0x{addrs[p.lo]:x} when splitting the span at 0x{addrs[u.lo]:x}"
        assert all(p.lo <= index(g.address) < p.hi for b in p.blocks for g in b), \
            f"a getter falls outside its piece 0x{addrs[p.lo]:x}-0x{addrs[p.hi - 1]:x} (interleaved groups)"
    return pieces


def carve(units: list[Unit], funcs: list[Fn], lo_addr: int, hi_addr: int, name: str) -> list[Unit]:
    """Make [lo_addr, hi_addr) its own library unit, cutting the units it overlaps. The piece after
    the range is bounded on the left by it (as surely as by an initialiser); the piece in front of
    it has no initialiser of its own."""
    out: list[Unit] = []
    for u in units:
        if funcs[u.hi - 1].address < lo_addr or funcs[u.lo].address >= hi_addr:
            out.append(u)
            continue
        sub = [f.address for f in funcs[u.lo:u.hi]]
        a = u.lo + bisect.bisect_left(sub, lo_addr)
        b = u.lo + bisect.bisect_left(sub, hi_addr)
        before = [blk for blk in u.blocks if min(g.address for g in blk) < lo_addr]
        after = [blk for blk in u.blocks if min(g.address for g in blk) >= hi_addr]
        if a > u.lo:
            out.append(Unit(u.lo, a, bounded_left=u.bounded_left, blocks=before,
                            init_fn=u.init_fn if u.init_fn is not None and u.init_fn < lo_addr else None))
        if out and out[-1].library == name and out[-1].hi == a:
            out[-1].hi = b          # the range spans several units: one library unit
        else:
            out.append(Unit(a, b, library=name))
        if b < u.hi:
            out.append(Unit(b, u.hi, bounded_left=True, bounded_right=u.bounded_right, blocks=after,
                            init_fn=u.init_fn if u.init_fn is not None and u.init_fn >= hi_addr else None))
    return out


def rank_classes(u: Unit, funcs: list[Fn], callers: dict[int, set[int]], parents: dict[str, str]) -> list[str]:
    """The unit's classes, primary first: an outer class before its nested types, then the getter
    most of the unit's functions call, then a child before its parent, then by address."""
    classes = [g for b in u.blocks for g in b]
    names = {g.type for g in classes}
    lo_addr, hi_addr = funcs[u.lo].address, funcs[u.hi - 1].address + 1

    def key(g):
        nested = any(g.type.startswith(n + "::") for n in names if n != g.type)
        in_unit = sum(1 for c in callers.get(g.address, ()) if lo_addr <= c < hi_addr)
        is_parent = any(parents.get(n) == g.type for n in names if n != g.type)
        return (nested, -in_unit, is_parent, g.address)
    return [g.type for g in sorted(classes, key=key)]


def confidence(u: Unit, funcs: list[Fn]) -> str:
    if u.library:  # the label names a library, not a TU
        return "low"
    if u.split:
        return "medium"
    size = funcs[u.hi - 1].address + funcs[u.hi - 1].size - funcs[u.lo].address
    if not (u.bounded_left and u.bounded_right and len(u.blocks) <= 1):
        return "low"
    if size <= LARGE_SPAN:
        return "high"
    # A span this large holds several TUs (the median TU is about 2 KB): the group names it, but
    # the span's other functions aren't necessarily that class's.
    return "medium" if u.blocks else "low"


def build(version: str) -> tuple[list[Unit], list[Fn], dict]:
    import asmdiff
    orig = asmdiff.Original(version)
    funcs = [f for f in load_functions() if orig.img.in_text(f.address - BASE)]
    addrs = [f.address for f in funcs]
    inits = read_init_array(orig)
    notes: dict = {"inits": len(inits), "not_function_starts": [a for a in inits if a not in orig.sizes],
                   "descending": sum(1 for a, b in zip(inits, inits[1:]) if b <= a)}
    inits = sorted(a for a in inits if a in orig.sizes)

    # 1. Initialiser spans: each ends with its initialiser; the text after the last one is one unit.
    units: list[Unit] = []
    lo = 0
    for k, a in enumerate(inits):
        i = bisect.bisect_left(addrs, a)
        assert addrs[i] == a
        units.append(Unit(lo, i + 1, init_fn=a, bounded_left=k > 0, bounded_right=True))
        lo = i + 1
    if lo < len(addrs):
        units.append(Unit(lo, len(addrs), bounded_left=True))

    # 2. Reflection groups: attach each to the unit of its first getter; split spans with several.
    blocks, parents, types = reflection_blocks(orig, funcs)
    starts = [addrs[u.lo] for u in units]

    def unit_of(a: int) -> int:
        return bisect.bisect_right(starts, a) - 1
    straddle = []
    for b in blocks:
        ks = {unit_of(g.address) for g in b}
        if len(ks) > 1:
            straddle.append(b)
        units[unit_of(min(g.address for g in b))].blocks.append(b)
    notes["straddling_groups"] = [[g.type for g in b] for b in straddle]
    refined: list[Unit] = []
    notes["split_spans"] = notes["split_pieces"] = notes["nested_spans"] = 0
    for u in units:
        pieces = split_groups(u, addrs, types) if len(u.blocks) > 1 else [u]
        if len(pieces) > 1:
            notes["split_spans"] += 1
            notes["split_pieces"] += len(pieces)
        elif len(u.blocks) > 1:
            notes["nested_spans"] += 1
        refined.extend(pieces)
    units = refined
    notes["init_only"] = sum(1 for u in units if u.hi - u.lo == 1 and u.init_fn is not None)

    # Where the reflection boilerplate sits in a one-group unit (the model says: at the TU start).
    pos = Counter()
    offsets = []
    for u in units:
        if len(u.blocks) != 1 or u.split:
            continue
        first = min(u.blocks[0], key=lambda g: g.address)
        i = bisect.bisect_left(addrs, first.address) - u.lo
        if i == 0:
            pos["getter_first"] += 1
        elif i == 1 and addrs[u.lo] in {g.fn_0x70 for g in u.blocks[0]}:
            pos["getter_second"] += 1
        else:
            pos["other"] += 1
        span = funcs[u.hi - 1].address + funcs[u.hi - 1].size - addrs[u.lo]
        offsets.append((first.address - addrs[u.lo]) / span)
    offsets.sort()
    notes["getter_position"] = pos
    notes["getter_offset_median"] = offsets[len(offsets) // 2] if offsets else 0.0

    # 3. Library units: Lua and dlmalloc by address range, nn::atk from its first named function.
    for subsystem, name in RANGE_LIBRARIES.items():
        members = [f for f in funcs if f.subsystem == subsystem]
        if members:
            units = carve(units, funcs, members[0].address, members[-1].address + members[-1].size, name)
    first_atk = next((f.address for f in funcs if f.subsystem == ATK_SUBSYSTEM), None)
    if first_atk is not None:
        for u in units:
            if funcs[u.hi - 1].address >= first_atk and not u.library:
                u.library = "nn::atk"

    # 4. Hints.
    calls = load_calls(version)
    getter_types = {g.address: g.type for b in blocks for g in b}
    callers: dict[int, set[int]] = defaultdict(set)
    for fn, cs in calls.items():
        for c in cs:
            if c in getter_types:
                callers[c].add(fn)
    path_strings = load_path_strings(version)
    starts = [addrs[u.lo] for u in units]
    string_units = defaultdict(set)
    for fn, vals in path_strings.items():
        for v in vals:
            string_units[v].add(unit_of(fn))
    registers_elsewhere = []
    # Bare (unqualified) names count as hints only when unique in the whole list: nnMain or malloc
    # name their unit, a label shared by hundreds of functions (operator.delete) names nothing.
    bare_counts = Counter(f.name for f in funcs if "::" not in f.name)
    for k, u in enumerate(units):
        members = funcs[u.lo:u.hi]
        names = Counter(q for f in members if f.name_source != "unknown" for q in [qualifier(f.name)]
                        if q and ("::" in f.name or bare_counts[f.name] == 1))
        if u.library:
            u.hint, u.source = u.library, "library"
        elif u.blocks:
            u.hint = ";".join(rank_classes(u, funcs, callers, parents))
            u.source = "reflection"
        else:
            registered = [getter_types[c] for c in calls.get(u.init_fn, []) if c in getter_types] if u.init_fn else []
            strings = sorted({v for f in members for v in path_strings.get(f.address, ()) if len(string_units[v]) <= 2})
            if registered:
                u.hint, u.source = ";".join(dict.fromkeys(registered)), "init_call"
            elif strings:
                u.hint, u.source = strings[0], "string"
            elif names:
                u.hint, u.source = names.most_common(1)[0][0], "names"
        if u.init_fn is not None:
            own = {g.type for b in u.blocks for g in b}
            registered = {getter_types[c] for c in calls.get(u.init_fn, []) if c in getter_types}
            if registered - own:
                registers_elsewhere.append((k, u.init_fn, sorted(registered - own)))
            if len(u.blocks) == 1:
                notes["one_group_with_init"] = notes.get("one_group_with_init", 0) + 1
                notes["registers_own"] = notes.get("registers_own", 0) + bool(registered & own)
    notes["registers_elsewhere"] = registers_elsewhere
    large = [u for u in units if funcs[u.hi - 1].address + funcs[u.hi - 1].size - funcs[u.lo].address > LARGE_SPAN]
    notes["large"] = (len(large), sum(1 for u in large if u.blocks))
    return units, funcs, notes


def unit_rows(units: list[Unit], funcs: list[Fn]) -> list[dict]:
    rows = []
    for k, u in enumerate(units):
        start = funcs[u.lo].address
        end = funcs[u.hi - 1].address + funcs[u.hi - 1].size
        rows.append({"index": k, "start": f"0x{start:x}", "end": f"0x{end:x}", "functions": u.hi - u.lo,
                     "bytes": end - start, "init_fn": f"0x{u.init_fn:x}" if u.init_fn is not None else "",
                     "class_hint": u.hint, "hint_source": u.source, "confidence": confidence(u, funcs)})
    return rows


def hand_slice_checks(rows: list[dict]) -> list[str]:
    """The functions of src/game_s2 sources sharing a hand-written header should fall into one or
    two adjacent units."""
    out = []
    if not HAND_SLICES.is_dir():
        return out
    includes: dict[str, list[int]] = defaultdict(list)
    for cpp in sorted(HAND_SLICES.glob("fn_*.cpp")):
        m = FILE_ADDR_RE.match(cpp.name)
        if not m:
            continue
        text = cpp.read_text(encoding="utf-8")
        for h in sorted(HAND_SLICES.glob("*.h")):
            if f'"{h.name}"' in text:
                includes[h.name].append(int(m.group(1), 16))
    starts = unit_starts(rows)
    for header, addrs in sorted(includes.items()):
        units = [find_unit(rows, a, starts) for a in addrs]
        ks = sorted({u["index"] for u in units if u})
        ok = bool(ks) and ks[-1] - ks[0] <= 1
        hints = "; ".join(f"{k}: {rows[k]['class_hint'].split(';')[0] or '-'}" for k in ks)
        out.append(f"  {'ok ' if ok else 'BAD'} {header}: {len(addrs)} functions in unit(s) {ks} ({hints})")
    return out


def write(version: str) -> int:
    units, funcs, notes = build(version)
    rows = unit_rows(units, funcs)
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with OUT.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, COLUMNS, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    print(f"{OUT.relative_to(ROOT)}: {len(rows)} units from {notes['inits']} initialisers "
          f"({notes['init_only']} units are just their initialiser); {OUT.stat().st_size} bytes")
    bad = 0
    if notes["not_function_starts"] or notes["descending"]:
        bad += 1
        print(f"  BAD .init_array: {len(notes['not_function_starts'])} entries are not function starts, "
              f"{notes['descending']} are out of address order")
    print("checks:")
    nn = find_unit(rows, NNMAIN)
    bad += not nn
    print(f"  {'ok ' if nn else 'BAD'} nnMain 0x{NNMAIN:x}: unit {nn['index'] if nn else '?'} "
          f"{nn['start'] if nn else ''}-{nn['end'] if nn else ''}, init {nn['init_fn'] if nn else ''}, "
          f"hint {nn['class_hint'] if nn else ''!r}")
    for name in RANGE_LIBRARIES.values():
        lib = [r for r in rows if r["hint_source"] == "library" and r["class_hint"] == name]
        ok = len(lib) == 1
        bad += not ok
        print(f"  {'ok ' if ok else 'BAD'} {name}: {len(lib)} unit(s) " +
              ", ".join(f"{r['index']} {r['start']}-{r['end']} ({r['functions']} functions)" for r in lib))
    atk = [r for r in rows if r["hint_source"] == "library" and r["class_hint"] == "nn::atk"]
    bad += not atk
    print(f"  {'ok ' if atk else 'BAD'} nn::atk: {len(atk)} units from {atk[0]['start'] if atk else '?'} "
          f"to the end of .text ({sum(r['functions'] for r in atk)} functions)")
    ok = not notes["straddling_groups"]
    bad += not ok
    print(f"  {'ok ' if ok else 'BAD'} reflection groups straddling two units: {len(notes['straddling_groups'])}")
    for line in hand_slice_checks(rows):
        bad += "BAD" in line
        print(line)
    re_ = notes["registers_elsewhere"]
    print(f"  note: {len(re_)} initialisers register a class whose getter is in another unit "
          f"(the registration of a class can sit in a neighbouring initialiser-only unit): " +
          ", ".join(f"unit {k} -> {c[0]}" for k, _, c in re_[:4]) + (" ..." if len(re_) > 4 else ""))
    pos = notes["getter_position"]
    print("layout:")
    print(f"  one-group units (not split): {sum(pos.values())}; the group's first getter is the unit's first "
          f"function in {pos['getter_first']}, its second (after its fn_0x70 attribute helper) in "
          f"{pos['getter_second']}, elsewhere in {pos['other']}; median offset {100 * notes['getter_offset_median']:.1f}% "
          f"of the unit's bytes")
    print(f"  the initialiser registers the unit's own class in {notes.get('registers_own', 0)} of "
          f"{notes.get('one_group_with_init', 0)} one-group units with an initialiser")
    print(f"  spans split between unrelated groups: {notes['split_spans']} (into {notes['split_pieces']} pieces); "
          f"spans whose groups are nested types of one class, kept together: {notes['nested_spans']}")
    print(f"  units over {LARGE_SPAN // 1024} KB: {notes['large'][0]} ({notes['large'][1]} with a reflection group, "
          f"{notes['large'][0] - notes['large'][1]} without)")
    report(rows)
    return 1 if bad else 0


# ---------------------------------------------------------------------------------------------
# Query modes (from data/tu_map.csv)

def load_map() -> list[dict]:
    if not OUT.is_file():
        raise SystemExit(f"{OUT.relative_to(ROOT)} is missing: run tools/tu_map.py --version <ver> first")
    with OUT.open(newline="", encoding="utf-8") as f:
        rows = list(csv.DictReader(f))
    for r in rows:
        r["index"], r["functions"], r["bytes"] = int(r["index"]), int(r["functions"]), int(r["bytes"])
        r["start_i"], r["end_i"] = int(r["start"], 16), int(r["end"], 16)
    return rows


def unit_starts(rows: list[dict]) -> list[int]:
    """The units' start addresses, for bisecting; computed once per lookup loop."""
    if "start_i" not in rows[0]:
        for r in rows:
            r["start_i"], r["end_i"] = int(r["start"], 16), int(r["end"], 16)
    return [r["start_i"] for r in rows]


def find_unit(rows: list[dict], address: int, starts: list[int] | None = None) -> dict | None:
    if starts is None:
        starts = unit_starts(rows)
    k = bisect.bisect_right(starts, address) - 1
    if k >= 0 and address < rows[k]["end_i"]:
        return rows[k]
    return None


def resolve_function(text: str, fns: list[Fn]) -> int:
    """A data/functions name, or an address (hex; fn_ and 0x prefixes allowed)."""
    t = text.strip()
    named = [f.address for f in fns if f.name == t]
    if len(named) == 1:
        return named[0]
    if named:
        raise SystemExit(f"{t}: {len(named)} functions have this name; pass an address: "
                         + ", ".join(f"0x{a:x}" for a in named[:6]))
    body = t.lower()
    for prefix in ("fn_", "0x"):
        if body.startswith(prefix):
            body = body[len(prefix):]
    try:
        a = int(body, 16)
    except ValueError:
        raise SystemExit(f"{t}: not a hex address and not a name in data/functions/") from None
    return a if a >= BASE else BASE + a


def show_function(rows: list[dict], text: str) -> int:
    all_fns = load_functions()
    address = resolve_function(text, all_fns)
    fn = next((f for f in all_fns if f.address == address), None)
    u = find_unit(rows, address)
    if u is None:
        print(f"0x{address:x}: not in any unit (outside .text?)")
        return 1
    label = f"{fn.name} ({fn.status}, {fn.subsystem})" if fn else "(no function starts here)"
    hints = u["class_hint"].split(";") if u["class_hint"] else []
    print(f"0x{address:x} {label}")
    print(f"  unit {u['index']}: {u['start']}-{u['end']}, {u['functions']} functions, {u['bytes']} bytes, "
          f"initialiser {u['init_fn'] or 'none'}, confidence {u['confidence']}")
    print(f"  hint: {hints[0] if hints else '(none)'}" + (f" [{u['hint_source']}]" if u["hint_source"] else ""))
    if len(hints) > 1:
        print(f"  also in this unit: {', '.join(hints[1:])}")
    if hints and u["hint_source"] in ("reflection", "init_call", "names", "library"):
        stem = re.sub(r"[^A-Za-z0-9_]+", "_", hints[0].replace("::", "_")).strip("_")
        print(f"  suggested file stem: {stem}")
    return 0


def report(rows: list[dict]) -> None:
    fns = load_functions()
    total_f, total_b = len(rows), sum(r["bytes"] for r in rows)
    conf = Counter(r["confidence"] for r in rows)
    src = Counter(r["hint_source"] or "(none)" for r in rows)
    covered = 0
    u_total = u_class = u_any = 0
    u_bytes = {"class": 0, "any": 0, "total": 0}
    starts = unit_starts(rows)
    for f in fns:
        u = find_unit(rows, f.address, starts)
        if u is None:
            continue
        covered += 1
        if f.status == "U":
            u_total += 1
            u_bytes["total"] += f.size
            if u["hint_source"] in ("reflection", "init_call"):
                u_class += 1
                u_bytes["class"] += f.size
            if u["hint_source"]:
                u_any += 1
                u_bytes["any"] += f.size
    print(f"units: {total_f}; functions: {sum(r['functions'] for r in rows)} ({covered} of {len(fns)} listed "
          f"functions fall in a unit); bytes: {total_b}")
    print("confidence: " + ", ".join(f"{k} {conf.get(k, 0)}" for k in ("high", "medium", "low")))
    print("hint sources: " + ", ".join(f"{k} {v}" for k, v in src.most_common()))
    print(f"U functions: {u_total} ({u_bytes['total']} bytes); in a unit with a class hint "
          f"(reflection or init_call): {u_class} ({u_bytes['class']} bytes); with any hint: {u_any} ({u_bytes['any']} bytes)")


def top(rows: list[dict], n: int) -> None:
    print(f"{'index':>5} {'start':>12} {'end':>12} {'fns':>5} {'bytes':>7} conf   hint")
    for r in sorted(rows, key=lambda r: -r["bytes"])[:n]:
        hint = (r["class_hint"].split(";")[0] or "-")[:60]
        print(f"{r['index']:>5} {r['start']:>12} {r['end']:>12} {r['functions']:>5} {r['bytes']:>7} {r['confidence']:<6} "
              f"{hint}" + (f" [{r['hint_source']}]" if r["hint_source"] else ""))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", help="write data/tu_map.csv for this dump")
    ap.add_argument("--function", metavar="ADDR|NAME",
                    help="print the unit of this function (hex address, fn_/0x prefix allowed, or a data/functions name)")
    ap.add_argument("--report", action="store_true", help="coverage and confidence summary")
    ap.add_argument("--top", type=int, metavar="N", help="the N largest units")
    args = ap.parse_args()
    if not (args.version or args.function or args.report or args.top):
        ap.error("nothing to do: pass --version to build, or --function/--report/--top to query")
    rc = 0
    if args.version:
        rc = write(args.version)
    if args.function or args.report or args.top:
        rows = load_map()
        if args.function:
            rc |= show_function(rows, args.function)
        if args.report and not args.version:
            report(rows)
        if args.top:
            top(rows, args.top)
    return rc


if __name__ == "__main__":
    sys.exit(main())
