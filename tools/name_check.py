#!/usr/bin/env python3
"""Does every name that is not an address stand for one address of the original?

tools/check.py verifies a relocation's target when the symbol is `fn_<addr>`, `g_unk_<addr>` or a name
that data/functions/ carries. A reference to anything else (a named constructor declared in a header, a
vtable by class name, a function-local static) is counted as unchecked: 92,122 of them on 2026-11-11.
The original still says where each such reference points, and one name must point to one place. This
collects, for every matching function, the address the original has at each reference to a name
check.py cannot resolve, and reports the names whose references point to more than one address: a
reference to the wrong function or object, or one name used for different things in different files
(two file-local types of one name, which the port's link cannot tell apart).

A name with internal linkage by construction (a function-local static or its guard) is compared within
its own source only. References the original does not encode as a whole address (an unpaired page or
low part) are left out. A function that is one branch to another (the 4-byte thunks in front of the
imports' stubs) counts as the function it branches to. Functions with the same code (the same
instructions once branch targets and page addresses are left out: several instantiations of one
template, a second copy of an inline function) count as one meaning too, and are counted apart.

Usage:
  python tools/name_check.py --version 2.1.0            # the report; exit 0
  python tools/name_check.py --version 2.1.0 --strict   # exit 1 when a name points to more than one address
Reads the objects tools/check.py built (run it first) and the user's own dump.
"""

from __future__ import annotations

import argparse
import struct
import sys
from collections import Counter, defaultdict
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
WHOLE = ("call26", "jump26", "adrp+lo12", "got")
SENTINEL = 0x10


def is_local(symbol: str) -> bool:
    """A symbol that names something else in every object: a function-local static, its guard, a label."""
    return symbol.startswith(("_ZZ", "_ZGV", ".L"))


def through_thunks(address: int, size_of, word_at) -> int:
    """The address a call to `address` ends at when `address` is a 4-byte function holding one `b`."""
    for _ in range(3):
        word = word_at(address) if size_of(address) == 4 else None
        if word is None or word & 0xFC000000 != 0x14000000:
            break
        offset = word & 0x3FFFFFF
        address += (offset - (1 << 26) if offset & (1 << 25) else offset) << 2
    return address


def masked(code: bytes) -> bytes:
    """A function's code with what depends on where it and its targets are left out: the offsets of b and
    bl, the page of adrp, and the low 12 bits added to or loaded through a register that holds a page."""
    out = []
    pages: set[int] = set()
    for (w,) in struct.iter_unpack("<I", code[:len(code) // 4 * 4]):
        rd, rn = w & 31, (w >> 5) & 31
        if w & 0x7C000000 == 0x14000000:                       # b, bl
            w &= 0xFC000000
        elif w & 0x9F000000 == 0x90000000:                     # adrp
            pages.add(rd)
            w &= 0x9F00001F
        elif w & 0xFFC00000 == 0x91000000 and rn in pages:      # add xd, xn, #lo12
            pages.discard(rd)
            w &= 0xFFC003FF
        elif w & 0x3B000000 == 0x39000000 and rn in pages:      # load/store through a page
            w &= 0xFFC003FF
        else:
            pages.discard(rd)
        out.append(w)
    return struct.pack(f"<{len(out)}I", *out)


def inconsistent(uses: dict[tuple[str, str], list[tuple[int, str, int]]],
                 code_of=None) -> list[tuple[str, int, list]]:
    """[(symbol, the address most references point to, the references that point elsewhere)] from
    {(source for a local symbol else "", symbol): [(address pointed to, source, function address)]},
    the names with the most stray references first. With `code_of(address)` (a function's masked code, or
    None), a name whose addresses all hold the same code is left out."""
    out = []
    for (_scope, symbol), refs in uses.items():
        count = Counter(a for a, _s, _f in refs)
        if len(count) < 2:
            continue
        if code_of is not None:
            codes = {code_of(a) for a in count}
            if len(codes) == 1 and None not in codes:
                continue
        best = max(count.values())
        usual = min(a for a, n in count.items() if n == best)
        out.append((symbol, usual, sorted(r for r in refs if r[0] != usual)))
    out.sort(key=lambda t: (-len(t[2]), t[0]))
    return out


def collect(version: str):
    """({(scope, symbol): references}, {symbol: demangled name}, code_of) over every object check.py built;
    code_of(address) is the masked code of the original's function at that address, or None."""
    import asmdiff
    import check

    original = asmdiff.Original(version)
    rows, _ = check.load_csv()
    by_name = check.csv_by_name(rows)
    loaded, symbols = [], set()
    for src in check.sources():
        obj = check.object_path(version, src)
        if obj.is_file():
            functions = asmdiff.read_object(obj)
            loaded.append((src, functions))
            symbols.update(check.object_symbols(functions))
    names: dict[str, str] = {}
    ordered = sorted(symbols)
    for i in range(0, len(ordered), 4000):
        names.update(asmdiff.demangle(ordered[i:i + 4000], strip=False))
    uses: dict[tuple[str, str], list[tuple[int, str, int]]] = defaultdict(list)

    def word_at(address: int) -> int | None:
        data = original.data_at(address, 4)
        return int.from_bytes(data, "little") if data else None
    for src, functions in loaded:
        rel = src.relative_to(ROOT).as_posix()
        diffs, _n, _f, targets = check.diff_with_targets(functions, original, by_name, names)
        unresolved: set[str] = set()

        def resolve(symbol: str, targets=targets, unresolved=unresolved) -> int:
            address = targets(symbol)
            if address is None:
                unresolved.add(symbol)
                return SENTINEL
            return address

        for res in diffs:
            if not res.bytes_match:
                continue
            for m in asmdiff.check_relocs(res, original, resolve).mismatches:
                if m.symbol in unresolved and m.kind in WHOLE and m.actual is not None and m.expected is not None:
                    target = m.actual - (m.expected - SENTINEL)
                    if m.kind in ("call26", "jump26"):
                        target = through_thunks(target, original.sizes.get, word_at)
                    uses[(rel if is_local(m.symbol) else "", m.symbol)].append((target, rel, res.address))

    def code_of(address: int) -> bytes | None:
        size = original.sizes.get(address)
        return masked(original.data_at(address, size)) if size else None
    return uses, names, code_of


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--strict", action="store_true", help="exit 1 when a name points to more than one address")
    args = ap.parse_args()
    uses, names, code_of = collect(args.version)
    found = inconsistent(uses, code_of)
    copies = len(inconsistent(uses)) - len(found)
    print(f"name_check: {sum(len(v) for v in uses.values())} references under {len(uses)} names that check.py cannot "
          f"resolve; {len(found)} names point to more than one address ({sum(len(s) for _n, _a, s in found)} "
          f"references off the usual one); {copies} more point to several copies of the same code")
    for symbol, usual, stray in found:
        print(f"{names.get(symbol, symbol)}\n    usually 0x{usual:x}")
        for address, src, function in stray:
            print(f"    0x{address:x} in fn_{function:x}  {src}")
    if found and args.strict:
        print("FAIL name_check: give each of these one meaning: correct the reference, or rename what is another thing")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
