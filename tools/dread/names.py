"""The name map, data/names.csv: names chosen for functions the game's own metadata does not name.

The function list (data/functions/) is regenerated from the dump by tools/gen_functions_csv.py, which names a
function only from evidence the binary carries (symbols, vendored code, RTTI, reflection, strings). A name
a person chooses lives here instead, one row per function, and the generator and tools/rename.py apply it to
the function list the same way (`apply`), so it survives regeneration. Columns:

  address      the function, as in data/functions/ (0x7100219588)
  name         its qualified name as the definition demangles to it, parameters left out
               (UnkRenderManager::ProjectToScreen, or ProjectToScreen for a free function)
  name_source  inferred: worked out by reading what the function does (owner, 2026-11-14); a string the
               code uses that names it is cited in the reason
  reason       one line: what the name is based on

tools/check.py and tools/port_link.py resolve a name that is unique in the function list to its address, so
a function the map names keeps its identity in the matching and the port. The address stays the identity
everywhere else (data/equivalence.csv, drafts/, the unit and data maps).

Standard library only (CI's policy job reads it).
"""

from __future__ import annotations

import csv
import re
from pathlib import Path

from dread.paths import DATA

PATH = DATA / "names.csv"
COLUMNS = ["address", "name", "name_source", "reason"]
SOURCES = ("inferred",)
# Names tools/gen_functions_csv.py gives from the binary's own evidence: the map may not replace one.
GENERATED_SOURCES = ("symbol", "library", "rtti", "reflection", "string")

IDENT = r"[A-Za-z_][A-Za-z0-9_]*"
QUALIFIED = re.compile(rf"(?:{IDENT}::)*{IDENT}")
PLACEHOLDER = re.compile(r"(?:fn|unk|g_unk|sub|FUN)_[0-9a-fA-F]+$")
RESERVED_PREFIXES = ("refl_", "vf_")  # inferred-role names tools/gen_functions_csv.py gives
KEYWORDS = frozenset("""alignas alignof and asm auto bool break case catch char class const constexpr const_cast
continue decltype default delete do double dynamic_cast else enum explicit export extern false float for friend
goto if inline int long mutable namespace new noexcept not nullptr operator or private protected public register
reinterpret_cast return short signed sizeof static static_assert static_cast struct switch template this throw
true try typedef typeid typename union unsigned using virtual void volatile wchar_t while""".split())


def placeholder(address: int) -> str:
    return f"fn_{address:x}"


def read(path: Path = PATH) -> list[dict]:
    """The map's rows in file order (an absent file is an empty map)."""
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def write(rows: list[dict], path: Path = PATH) -> None:
    """Write the map sorted by address."""
    with path.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=COLUMNS, lineterminator="\n")
        w.writeheader()
        for r in sorted(rows, key=lambda r: int(r["address"], 16)):
            w.writerow({k: r[k] for k in COLUMNS})


def split(name: str) -> tuple[str, str]:
    """(qualifier, identifier): ("UnkFoo", "Bar") for UnkFoo::Bar, ("", "Bar") for Bar."""
    qual, _, ident = name.rpartition("::")
    return qual, ident


def name_problem(name: str) -> str | None:
    """Why `name` cannot be a map name, or None."""
    if not QUALIFIED.fullmatch(name):
        return "not a qualified C++ identifier (Class::Method or Function, no parameters)"
    parts = name.split("::")
    if any(p in KEYWORDS for p in parts):
        return "a C++ keyword"
    ident = parts[-1]
    if PLACEHOLDER.match(ident):
        return "an address placeholder, not a name"
    if ident.startswith(RESERVED_PREFIXES):
        return f"{'/'.join(RESERVED_PREFIXES)} names are the generator's"
    if ident.startswith("__") or re.match(r"_[A-Z]", ident):
        return "a name reserved to the implementation"
    return None


def row_problems(rows: list[dict]) -> list[str]:
    """What is wrong with the map on its own: columns, values, duplicates."""
    out = []
    seen_addr: dict[str, int] = {}
    seen_name: dict[str, str] = {}
    for i, r in enumerate(rows, 2):
        where = f"data/names.csv:{i}"
        if set(r) != set(COLUMNS) or any(r[k] is None for k in COLUMNS):
            out.append(f"{where}: columns must be {','.join(COLUMNS)}")
            continue
        if not re.fullmatch(r"0x[0-9a-f]+", r["address"]):
            out.append(f"{where}: address {r['address']!r} is not lower-case 0x hex")
            continue
        if r["address"] in seen_addr:
            out.append(f"{where}: {r['address']} is named again (line {seen_addr[r['address']]})")
        seen_addr[r["address"]] = i
        why = name_problem(r["name"])
        if why:
            out.append(f"{where}: {r['name']!r}: {why}")
        if r["name"] in seen_name:
            out.append(f"{where}: {r['name']} names {seen_name[r['name']]} too")
        seen_name[r["name"]] = r["address"]
        if r["name_source"] not in SOURCES:
            out.append(f"{where}: name_source {r['name_source']!r} is not one of {'|'.join(SOURCES)}")
        if not r["reason"].strip():
            out.append(f"{where}: a name needs its reason")
    return out


def apply(function_rows: list[dict], rows: list[dict]) -> list[str]:
    """Set each mapped function's name and name_source in `function_rows` (data/functions/ rows, changed in
    place). Returns the problems: an address the list does not have, a function the binary already names."""
    by_addr = {r["address"]: r for r in function_rows}
    out = []
    for r in rows:
        f = by_addr.get(r["address"])
        if f is None:
            out.append(f"{r['address']} ({r['name']}): not a function of data/functions/")
        elif f["name_source"] in GENERATED_SOURCES and f["name"] != r["name"]:
            out.append(f"{r['address']} ({r['name']}): the binary names it {f['name']} ({f['name_source']})")
        else:
            f["name"], f["name_source"] = r["name"], r["name_source"]
    return out


# --- Mangled symbols: a renamed function's symbol, spelled as its placeholder was -------------------------

def source_names(symbol: str) -> list[tuple[int, int, str]]:
    """The <source-name>s of an Itanium-mangled symbol, (start, end, identifier), where start is the length
    prefix. Read greedily: a digit run that is not part of a longer one and is followed by that many identifier
    characters is a name, skipped whole (so `7Bar3Foo` is one name, not `3Foo`). Not a demangler: a template
    argument's literal can be misread, which only matters where its digits spell a mapped name."""
    out = []
    i, n = 0, len(symbol)
    while i < n:
        if symbol[i].isdigit() and (i == 0 or not symbol[i - 1].isdigit()):
            j = i
            while j < n and symbol[j].isdigit():
                j += 1
            length = int(symbol[i:j])
            ident = symbol[j:j + length]
            if length and len(ident) == length and re.fullmatch(r"[A-Za-z_][A-Za-z0-9_]*", ident):
                out.append((i, j + length, ident))
                i = j + length
                continue
            i = j
            continue
        i += 1
    return out


class SymbolNormaliser:
    """Spells a symbol that names a mapped function as it was spelled with the function's placeholder name.

    data/equivalence.csv's digest (tools/equiv_test.py) covers what each relocation names. Renaming a callee
    changes its callers' relocation symbols, and with them every caller's digest, though the code is the same;
    normalising first keeps a verdict current across a rename. A symbol is matched by its run of consecutive
    source-names (the nested name: N7UnkFoo3BarE is UnkFoo::Bar), compared with the map's qualified names."""

    def __init__(self, rows: list[dict]):
        self.by_name = {r["name"]: placeholder(int(r["address"], 16)) for r in rows}
        self.idents = {split(n)[1] for n in self.by_name}

    def __call__(self, symbol: str) -> str:
        if not self.by_name or not symbol.startswith("_Z"):
            return symbol
        names = source_names(symbol)
        if not any(ident in self.idents for *_, ident in names):
            return symbol
        edits = []  # (start, end, replacement)
        k = 0
        while k < len(names):
            chain = [names[k]]
            while k + len(chain) < len(names) and names[k + len(chain)][0] == chain[-1][1]:
                chain.append(names[k + len(chain)])
            # The longest prefix of the run that is a mapped name (a run may continue into a parameter's type).
            for m in range(len(chain), 0, -1):
                ph = self.by_name.get("::".join(c[2] for c in chain[:m]))
                if ph:
                    s, e, _ = chain[m - 1]
                    edits.append((s, e, f"{len(ph)}{ph}"))
                    break
            k += len(chain)
        for s, e, rep in reversed(edits):
            symbol = symbol[:s] + rep + symbol[e:]
        return symbol


_normaliser: SymbolNormaliser | None = None


def normalise_symbol(symbol: str) -> str:
    """SymbolNormaliser over data/names.csv, read once."""
    global _normaliser
    if _normaliser is None:
        _normaliser = SymbolNormaliser(read())
    return _normaliser(symbol)
