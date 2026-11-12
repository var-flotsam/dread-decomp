#!/usr/bin/env python3
"""Wrong addresses in functions that do not match: what a function refers to, against the original.

tools/check.py verifies every callee, global and GOT slot a *matching* function names, instruction by
instruction. In a function that does not match the instructions do not line up, so nothing is verified
there: a `NON_MATCHING` function can call the wrong function or read the wrong global and stay
"minor: same behaviour". This compares sets, which needs no line-up: every function a non-matching
function calls and every address it forms, against the same for the original function.

  ours       the relocations of our function in the object: a call's target, the object behind a GOT
             access, the address of a direct (adrp) access, each resolved to an original address as
             tools/check.py resolves names (fn_<addr>, g_unk_<addr>, a name unique in data/functions/);
             an import by its name.
  original   the targets of its bl and of its b out of the function (a PLT stub by the import it
             names), and every address it forms with adr or adrp + add/load. An address that holds a
             relocated pointer (a GOT slot, or a pointer variable) counts as both: the cell, which code
             may read directly, and the object it points to, which code reaches through the GOT.

Reported, most telling first:
  slot       ours names an address inside the original's GOT: a global named after its GOT slot. The
             object is what the slot points to; nobody's source names a slot.
  call       ours calls a function the original function never calls, or the reverse.
  global     ours refers to a global the original function never refers to, or the reverse.
  near       the same, with an address of the other side close by (0x40 bytes for what ours names, 0x1000
             for what the original forms): most often one object reached from another base or by an
             offset the relocation does not carry. Listed, never counted as a failure.
Not compared: string literals and constant-pool entries (tools/check.py compares their content in matching
functions; here they are section-relative and carry no name), the original's own references into .rodata
for the same reason, branches inside the function, and names that resolve to no address (counted). When
one of our function's names resolves to no address, only what ours names is checked: that the original
refers to something "ours never does" cannot be said without knowing what that name stands for.
A function whose difference is registers-only (tools/asmdiff.py classify) has the same instructions as the
original, so any finding in it is a real difference; in other non-matching functions a missing or extra
call can be the reason the function does not match and not a wrong name.

Needs the dump (the original's code); compiles with the project compiler like tools/trymatch.py.

Usage:
  python tools/ref_check.py --version 2.1.0                    # every NON_MATCHING file under src/
  python tools/ref_check.py --version 2.1.0 FILE.cpp [...]     # the non-matching functions of these files
  python tools/ref_check.py --version 2.1.0 --drafts           # every draft of drafts/INDEX.csv
  ... --json OUT       # every finding
  ... --all            # also list functions with nothing to report
Exit 1 when a registers-only function has a `call` or `global` finding, or any function a `slot` finding.
"""

from __future__ import annotations

import argparse
import collections
import csv
import json
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

WORK = ROOT / "build" / "ref_check"
NEAR = 0x40      # ours names an address this close to one the original forms: probably a member of it
SAME = 0x1000    # the original forms an address this close to one ours names: ours reaches it by an offset
JUMP26, CALL26, GOT_PAGE = 282, 283, 311
ADRP_TYPES = {275, 276}


@dataclass
class Refs:
    """What one version of a function refers to."""
    calls: set[int] = field(default_factory=set)      # functions called or branched to, outside the function
    imports: set[str] = field(default_factory=set)    # imports called, and data imports named, by name
    got: set[int] = field(default_factory=set)        # ours: objects reached through the GOT
    direct: set[int] = field(default_factory=set)     # addresses formed directly (adr, adrp + add/load)
    cells: dict[int, int | None] = field(default_factory=dict)  # original: pointer cell -> what it points to
    unresolved: int = 0                               # ours: names that stand for no known address


def compare(ours: Refs, theirs: Refs, rodata: tuple[int, int] = (0, 0),
            got: tuple[int, int] = (0, 1 << 64)) -> list[tuple[str, str]]:
    """[(kind, text)] for what the two reference sets disagree on (kinds: slot, call, global, near).
    `rodata` and `got` are the address ranges of the original's read-only data and of its GOT."""
    out: list[tuple[str, str]] = []
    targets = {t for t in theirs.cells.values() if t is not None}
    known = theirs.direct | set(theirs.cells) | targets

    def near(a: int, pool, window: int = NEAR) -> bool:
        return any(abs(a - b) <= window for b in pool)

    for c in sorted(ours.calls - theirs.calls):
        out.append(("call", f"calls {c:#x}, which the original function never calls"))
    reverse = ours.unresolved == 0  # with a name we cannot place, "ours never does" cannot be said
    for c in sorted(theirs.calls - ours.calls) if reverse else ():
        out.append(("call", f"the original calls {c:#x}, ours never does"))
    for n in sorted(ours.imports - theirs.imports):
        out.append(("call", f"names the import {n}, which the original function never does"))
    for n in sorted(theirs.imports - ours.imports) if reverse else ():
        out.append(("call", f"the original names the import {n}, ours never does"))
    for a in sorted(ours.got):
        if a in targets or a in theirs.direct:
            continue  # the same object (through the GOT here, directly there: visibility, not identity)
        if a in theirs.cells and not got[0] <= a < got[1]:
            continue  # a pointer variable or a table of pointers, which the original reads directly
        if a in theirs.cells:
            t = theirs.cells[a]
            out.append(("slot", f"names {a:#x}, a GOT slot of the original that points to "
                                f"{f'{t:#x}' if t is not None else 'an import'}: the object is what it points to"))
        else:
            out.append(("near" if near(a, known) else "global",
                        f"refers to {a:#x}, which the original function never does"))
    for a in sorted(ours.direct):
        if a in known:
            continue
        out.append(("near" if near(a, known) else "global", f"refers to {a:#x}, which the original function never does"))
    mine = ours.got | ours.direct
    if not reverse:
        return out
    for cell, t in sorted(theirs.cells.items()):
        if t is None or cell in mine or t in mine:
            continue  # (a data import is compared by its name, with the imports)
        out.append(("near" if near(t, mine, SAME) else "global",
                    f"the original refers to {t:#x} (through the cell at {cell:#x}), ours never does"))
    for a in sorted(theirs.direct - mine):
        if rodata[0] <= a < rodata[1]:
            continue  # a string, a constant or a table: ours reaches its own copy without a name
        out.append(("near" if near(a, mine, SAME) else "global", f"the original refers to {a:#x}, ours never does"))
    return out


def got_range(original) -> tuple[int, int]:
    """The address range of the original's .got: it follows .got.plt (DT_PLTGOT, three reserved entries and
    one per PLT relocation) and is a run of relocated 8-byte cells."""
    import asmdiff
    mod = original.img.mod
    start = mod.dyn[3] + 8 * (3 + mod.dyn[2] // 24)  # DT_PLTGOT, DT_PLTRELSZ
    end = start
    while end in original.img.relocs:
        end += 8
    return asmdiff.BASE + start, asmdiff.BASE + end


def ours_refs(relocs: dict, resolve, is_import=lambda name: False, plt=lambda address: None) -> Refs:
    """The references of our function: its relocations, each symbol resolved by `resolve(symbol)` to an
    original address or None. `plt(address)` names the import behind a PLT stub: a call to the stub by its
    address (fn_<stub>) is a call of that import."""
    r = Refs()
    for _, rel in sorted(relocs.items()):
        if rel.section or rel.type not in (CALL26, JUMP26, GOT_PAGE) and rel.type not in ADRP_TYPES:
            continue
        target = resolve(rel.symbol)
        if target is None:
            if is_import(rel.symbol):
                r.imports.add(rel.symbol)
            else:
                r.unresolved += 1
            continue
        if rel.type in (CALL26, JUMP26):
            name = plt(target + rel.addend)
            if name is not None:
                r.imports.add(name)
            else:
                r.calls.add(target + rel.addend)
        elif rel.type == GOT_PAGE:
            r.got.add(target + rel.addend)
        else:
            r.direct.add(target + rel.addend)
    return r


def formed_addresses(code: bytes, address: int) -> tuple[set[int], set[int]]:
    """(addresses, branch targets) of a function, read straight down its instructions: every absolute
    address it forms with adr or adrp and then carries through `add #imm`, `mov` and a load or store's
    displacement, also in a second register (a base kept across calls in a callee-saved register and
    used with further offsets). A register's address is forgotten when anything else writes it, and
    x0-x18 at a call. No control flow is followed, so this is a close guess, not a proof."""
    import capstone
    from capstone import arm64_const as c
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    known: dict[int, int] = {}
    addresses: set[int] = set()
    targets: set[int] = set()
    scratch = {getattr(c, f"ARM64_REG_X{n}") for n in range(19)}
    for i in cs.disasm(code, address):
        ops = i.operands
        m = i.mnemonic
        if m in ("bl", "b") and ops and ops[0].type == c.ARM64_OP_IMM:
            targets.add(ops[0].imm)
        if m in ("bl", "blr"):
            for reg in scratch:
                known.pop(reg, None)
            continue
        if m in ("adrp", "adr"):
            known[ops[0].reg] = ops[1].imm
            if m == "adr":
                addresses.add(ops[1].imm)
            continue
        for o in ops:
            if o.type == c.ARM64_OP_MEM and o.mem.base in known and not o.mem.index:
                addresses.add(known[o.mem.base] + o.mem.disp)
        dst = ops[0].reg if ops and ops[0].type == c.ARM64_OP_REG else None
        if m == "add" and len(ops) == 3 and ops[1].type == c.ARM64_OP_REG and ops[1].reg in known                 and ops[2].type == c.ARM64_OP_IMM:
            known[dst] = known[ops[1].reg] + ops[2].imm
            addresses.add(known[dst])
            continue
        if m == "mov" and len(ops) == 2 and ops[1].type == c.ARM64_OP_REG and ops[1].reg in known:
            known[dst] = known[ops[1].reg]
            continue
        if dst is not None and not m.startswith(("str", "stp", "stur", "cmp", "cmn", "tst", "b", "cb", "tb")):
            known.pop(dst, None)
    return addresses, targets


def original_refs(original, address: int) -> Refs:
    """The references of the original function at `address`."""
    code = original.function(address)
    end = address + len(code)
    r = Refs()
    addresses, targets = formed_addresses(code, address)
    for target in targets:
        if address <= target < end:
            continue
        name = original.plt_import(target)
        if name is not None:
            r.imports.add(name)
        else:
            r.calls.add(target)
    for a in addresses:
        if address <= a < end:
            continue  # a place in the function itself: a literal, a jump table, a label
        target, name = original.got_slot(a)
        if target is None and name is None:
            r.direct.add(a)
            continue
        r.cells[a] = target
        if name is not None and target is None:
            r.imports.add(name)
    return r


def object_findings(obj: Path, original, by_name, rodata, got) -> list[dict]:
    """One row per non-matching function of an object: its address, name, kind of difference, how many of
    its names resolve to no address, and its findings."""
    import asmdiff
    import check
    funcs = asmdiff.read_object(obj)
    codes = {f.symbol: f.code for f in funcs}
    results, names, _, targets = check.diff_with_targets(funcs, original, by_name)
    out = []
    for res in results:
        if res.bytes_match:
            continue
        kind = asmdiff.classify(original.function(res.address), codes[res.symbol]).split(" ")[0]
        mine = ours_refs(res.relocs, targets, lambda sym: sym in original.dynsym, original.plt_import)
        found = compare(mine, original_refs(original, res.address), rodata, got)
        out.append({"address": f"{res.address:#x}", "function": names.get(res.symbol, res.symbol),
                    "difference": kind, "unresolved": mine.unresolved,
                    "findings": [{"kind": k, "text": t} for k, t in found]})
    return out


def failing(row: dict) -> list[dict]:
    """The findings of a row that fail: a slot anywhere, a call or global in a registers-only function."""
    return [f for f in row["findings"]
            if f["kind"] == "slot" or row["difference"] == "registers" and f["kind"] != "near"]


def ranges(original) -> tuple[tuple[int, int], tuple[int, int]]:
    """(the original's read-only data range, its GOT range)."""
    import asmdiff
    img = original.img
    return (asmdiff.BASE + img.rodata[0], asmdiff.BASE + img.rodata[1]), got_range(original)


# --- the command line ---------------------------------------------------------------------------------

def sources(args) -> list[Path]:
    if args.drafts:
        with (ROOT / "drafts" / "INDEX.csv").open(newline="", encoding="utf-8") as fh:
            return sorted({ROOT / r["file"] for r in csv.DictReader(fh)})
    if args.files:
        return [p.resolve() for p in args.files]
    r = subprocess.run(["git", "grep", "-l", "NON_MATCHING", "--", "src"], cwd=ROOT, capture_output=True, text=True)
    return [ROOT / p for p in r.stdout.split() if p.endswith(".cpp")]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--drafts", action="store_true", help="every draft of drafts/INDEX.csv")
    ap.add_argument("--json", type=Path, help="write every finding")
    ap.add_argument("--all", action="store_true", help="also list the functions with nothing to report")
    args = ap.parse_args()

    import asmdiff
    import buildcfg
    import check
    from concurrent.futures import ThreadPoolExecutor
    original = asmdiff.Original(args.version)
    rows, _ = check.load_csv()
    by_name = check.csv_by_name(rows)
    rodata, got = ranges(original)
    files = sources(args)

    def build(src: Path):
        rel = src.relative_to(ROOT) if src.is_relative_to(ROOT) else Path(src.name)
        obj = WORK / rel.with_suffix(".o")
        obj.parent.mkdir(parents=True, exist_ok=True)
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        return src, obj, r.returncode == 0
    with ThreadPoolExecutor(8) as ex:
        built = list(ex.map(build, files))

    out = []
    judged = unbuilt = 0
    for src, obj, ok in built:
        rel = src.relative_to(ROOT).as_posix() if src.is_relative_to(ROOT) else str(src)
        if not ok:
            unbuilt += 1
            continue
        for o in object_findings(obj, original, by_name, rodata, got):
            judged += 1
            out.append({"file": rel, **o})
    bad = [o for o in out if o["findings"]]
    order = {"slot": 0, "call": 1, "global": 2, "near": 3}
    for o in sorted(out, key=lambda o: (min((order[f["kind"]] for f in o["findings"]), default=9),
                                        o["difference"] != "registers", o["file"])):
        if not o["findings"] and not args.all:
            continue
        print(f"{o['address']}  {o['difference']:10}  {o['file']}  {o['function']}")
        for f in o["findings"]:
            print(f"    {f['kind']:6} {f['text']}")
    kinds = collections.Counter(f["kind"] for o in bad for f in o["findings"])
    strict = [o for o in bad if failing(o)]
    print(f"ref_check: {judged} non-matching functions in {len(files) - unbuilt} files ({unbuilt} files do not build); "
          f"{len(bad)} with a finding ({', '.join(f'{k} {v}' for k, v in sorted(kinds.items())) or 'none'}); "
          f"{len(strict)} fail (registers-only with a call or global finding, or a slot finding); "
          f"{sum(o['unresolved'] for o in out)} names resolve to no address and were not compared")
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(out, indent=1), encoding="utf-8")
    return 1 if strict else 0


if __name__ == "__main__":
    sys.exit(main())
