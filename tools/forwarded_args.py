#!/usr/bin/env python3
"""Matched functions whose original reads argument registers beyond the parameters they declare.

On AArch64 a register nobody touches costs no instruction, so a function can declare fewer
parameters than the original passes on and still match byte for byte: `fn(a, b)` forwarding x2-x4
untouched to a virtual call compiles to the same bytes as the full signature. The x86-64 port
compiles the same source and drops those arguments (fn_7100087ecc dropped an allocator, alignment
and flags and crashed the boot). This tool lists every byte-matching function in build/check/<ver>/
whose original code reads x0-x7 or v0-v7 beyond its declared parameters before writing them.

Declared parameters come from the demangled symbol (tools/port_link.py's param_list and
arg_class): integers, pointers and references take the next x register, float and double the
next v register, `this` (a member function: tools/port_link.py's class_qualified) takes x0, the
float vectors in HFA (CVector2D/3D/4D, CColor4F by value) take one v register per float, and any
other named type by value counts as one x register (an aggregate or an enum; such hits are marked
`+A`, since a 9-16 byte struct takes two). Variadic functions are skipped, and so are definitions emitted under another declaration's symbol
(`asm("_Z13fn_...v")`, as the in-place constructors reflection declares `void()`), whose symbol does not
show their parameters.

The original's reads come from tools/abi_facts.py's liveness over the original instructions, run
in four tiers; each register is reported with the first tier that makes it live at entry:
  read      the function itself reads it (stores it, computes with it, compares it)
  call      it reaches a direct call or tail branch untouched, and that callee reads it
            (abi_facts' facts of the callee)
  indirect  it reaches an indirect call (blr/br) untouched, and the function's direct callers set
            it before calling (abi_facts' `fwd`); or x0 reaching an indirect call
  slot      only the open rule below makes it live, but another implementation of the same
            virtual slot reads it in one of the tiers above: the slot's signature has it. Slots
            come from tables of function pointers in the original's data (runs of RELATIVE
            relocations to function starts, RTTI or not); two tables are of one hierarchy when
            they hold the same function at another index (one shared by more than SHARED_MAX
            tables, a pure-virtual stub or a common no-op, does not count).
  open      only abi_facts' rule for a function with an indirect call and no direct caller (a
            virtual function): every x register untouched up to the call may be an argument.
            Nothing in the bytes confirms it (fn_7100087ecc's x4-x6 were of this kind: one
            implementation, no direct caller), so these candidates are listed only with --open
            and not counted as hits; the port's crash or the callee's signature decides.

A register marked `*` is also one the function's direct callers set before calling it (abi_facts'
evidence: written in the block before the call, not read again): the callers agree that the
argument exists. A hit with no `*` and no direct caller setting it may still be real (a virtual
function, a caller that sets it in an earlier block) or a callee's read on a path these callers
never take.

With --calls the tool looks at each call instead: a byte-matching function that declares an
argument register and leaves it untouched up to a direct call or tail branch (on every path from
the entry, tools/abi_facts.py's `untouched`), whose callee reads it (READ and CALL tiers above),
while the callee's symbol in our object (the CALL26/JUMP26 relocation) declares fewer parameters:
our C++ call does not pass the argument on. `fn_X(self)` and `fn_X(self, arg)` compile to the same
bytes when `arg` arrives in x1 (fn_71005d0884 called its base without `arg`); the x86-64 port drops
it. A call through `reinterpret_cast<...>(fn_X)` names fn_X's declared signature, not the cast's:
calls whose source casts the callee (a `reinterpret_cast` naming it) are listed apart, as `cast`,
and not counted. A callee whose own declaration is short (a hit of the default mode) shows up
here too, at each caller; fix the callee's signature first.

Boot-slice functions (reachable from nnMain by direct calls, tools/callgraph.py) are listed first.
The objects must be current: run tools/check.py first. Needs capstone, llvm-cxxfilt and the
Ghidra export (for callers and the slice).

Usage: python tools/forwarded_args.py --version 2.1.0 [--calls] [--open] [--boot] [--json OUT] [--file SRC]
"""

from __future__ import annotations

import argparse
import json
import re
import sys
from dataclasses import dataclass, field
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))

ROOT = Path(__file__).resolve().parent.parent
TIERS = ("read", "call", "indirect", "slot", "open")
READ, CALL, INDIRECT, SLOT, OPEN = range(5)
SHARED_MAX = 50  # a function in more tables than this does not relate them (vtable_siblings)
INT_REGS = tuple(range(8))
FLOAT_REGS = tuple(range(100, 108))
# Homogeneous float aggregates passed by value (include/base/math, include/base/color): floats each.
HFA = {"CVector2D": 2, "CVector3D": 3, "CVector4D": 4, "CColor4F": 4}
# Library types by value of 9-16 bytes that take two x registers (a pointer and a size).
PAIRS = ("initializer_list", "basic_string_view")
GENERATED = ("src/reflection/", "src/reflection_types/", "src/reflection_statics/", "src/clones/", "src/tables/",
             "src/lifted/")  # generator output (.gitattributes): fix the generator, not the file


@dataclass
class Declared:
    ints: int
    floats: int
    aggregate: bool = False  # a named type by value among the parameters (count uncertain)


def declared(demangled: str) -> Declared | None:
    """Argument registers a definition's signature takes on AArch64; None for a variadic function
    or a symbol without a parameter list."""
    import port_link
    params = port_link.param_list(demangled)
    if params is None:
        return None
    ints = 1 if port_link.class_qualified(demangled) else 0
    floats, agg = 0, False
    for p in params:
        if p.strip() == "void":
            continue
        c = port_link.arg_class(p)
        if c == "V":
            return None
        if c == "F":
            floats += 1
        elif c == "A" and hfa(p):
            floats += hfa(p)
        elif c == "A" and pair(p):
            ints += 2
        else:
            ints += 1
            agg |= c == "A"
    return Declared(ints, floats, agg)


def hfa(param: str) -> int:
    """Floats of a by-value HFA parameter (HFA), else 0."""
    words = param.replace("const", " ").replace("volatile", " ").split()
    return HFA.get(words[0].rsplit("::", 1)[-1], 0) if len(words) == 1 else 0


def pair(param: str) -> bool:
    """A by-value library type of two words (PAIRS), such as std::initializer_list<T>."""
    words = param.split("<", 1)[0].replace("const", " ").replace("volatile", " ").split()
    return len(words) == 1 and words[0].rsplit("::", 1)[-1] in PAIRS and param.rstrip().endswith(">")


def beyond(regs, d: Declared) -> list[int]:
    """The argument registers in `regs` past the declared ones (x8 is never an argument here)."""
    return sorted(k for k in regs if (k in INT_REGS and k >= d.ints) or (k in FLOAT_REGS and k - 100 >= d.floats))


def reg_name(k: int) -> str:
    return f"x{k}" if k < 100 else f"v{k - 100}"


class Analyzer:
    """Tiered argument liveness over the original's instructions, on top of an abi_facts.AbiFacts
    (its decoding, block and caller evidence; its import facts for C library calls). A direct
    callee counts with the registers it reads itself, worked out here the same way (READ and CALL
    tiers, recursively; INDIRECT adds the callee's own indirect rule), not with abi_facts' facts,
    which add everything any caller of an indirect-calling callee happens to set. A callee that
    reads all of v0-v7 is variadic (its prologue saves every argument register): it counts as
    reading nothing, since only its format says how many arguments it takes."""

    MAX_DEPTH = 48

    def __init__(self, af):
        self.af = af
        self._args: dict[tuple[int, int], frozenset | None] = {}
        self._busy: set[tuple[int, int]] = set()

    def tiers(self, addr: int) -> dict[int, int] | None:
        """Argument register -> the first tier (READ..OPEN) in which it is live at addr's entry;
        None when the instructions cannot be decoded."""
        ins = self.af.decode(addr)
        if ins is None:
            return None
        first: dict[int, int] = {}
        for tier in (READ, CALL, INDIRECT, OPEN):
            for k in self.live_in(addr, ins, tier, 0):
                first.setdefault(k, tier)
        return first

    def live_in(self, addr: int, ins: list, tier: int, depth: int) -> frozenset:
        import abi_facts
        live, _ = _Probe(self, tier, depth).liveness(addr, ins, depth)
        return frozenset(k for k in live[0] if k in abi_facts.ARGS and k != 8)

    def callee_args(self, target: int, tier: int, depth: int) -> set:
        """What a direct call of target reads, at `tier` (CALL or INDIRECT)."""
        import abi_facts
        if self.af.import_name(target) is not None:
            f = self.af.facts(target)
            return set(f.args) if f is not None else set()
        key = (target, tier)
        if key not in self._args:
            ins = self.af.decode(target)
            if ins is None or key in self._busy or depth > self.MAX_DEPTH:
                return set()  # unknown, a cycle or a deep chain: claims nothing (not memoized)
            self._busy.add(key)
            try:
                self._args[key] = self.live_in(target, ins, tier, depth + 1)
            finally:
                self._busy.discard(key)
        args = self._args[key]
        return set() if abi_facts.FLOAT_ARGS <= args else set(args)


def tiered_live_in(af, addr: int) -> dict[int, int] | None:
    """Analyzer(af).tiers(addr), for one function."""
    return Analyzer(af).tiers(addr)


class _Probe:
    """An AbiFacts view whose calls read only what the tier allows (see the module docstring)."""

    def __init__(self, analyzer: Analyzer, tier: int, depth: int):
        import abi_facts
        self.an, self.af, self.tier, self.depth = analyzer, analyzer.af, tier, depth
        self._base = abi_facts.AbiFacts

    def __getattr__(self, name):
        return getattr(self.af, name)

    def liveness(self, addr, ins, depth, ret_uses=frozenset()):
        return self._base.liveness(self, addr, ins, depth, ret_uses)

    def call_uses(self, target, block, k, depth, reg=None):
        if target is not None:
            if self.tier == READ:
                return set()
            return self.an.callee_args(target, min(self.tier, INDIRECT), self.depth)
        if self.tier >= INDIRECT:
            return self._base.call_uses(self.af, None, block, k, depth, reg)
        return {reg} if reg is not None else set()

    def fwd_of(self, addr, ins):
        return self.af.fwd_of(addr, ins) if self.tier >= INDIRECT else frozenset()

    def is_open(self, addr, ins):
        return self.af.is_open(addr, ins) if self.tier >= OPEN else False


def pointer_tables(orig) -> list[list[int]]:
    """Runs of consecutive 8-byte words that RELATIVE relocations point at function starts:
    vtables (with or without RTTI) and other tables of function pointers, as absolute addresses."""
    import asmdiff
    starts = set(orig.sizes)
    words = {off: asmdiff.BASE + r.addend for off, r in orig.img.relocs.items()
             if r.type == asmdiff.R_AARCH64_RELATIVE and asmdiff.BASE + r.addend in starts}
    tables: list[list[int]] = []
    last = None
    for off in sorted(words):
        if last is not None and off == last + 8:
            tables[-1].append(words[off])
        else:
            tables.append([words[off]])
        last = off
    return tables


def vtable_siblings(tables: list[list[int]]) -> dict[int, set[int]]:
    """Function -> the other functions in the same slot of a related table (SLOT in the docstring)."""
    count: dict[int, int] = {}
    for t in tables:
        for f in set(t):
            count[f] = count.get(f, 0) + 1
    # tables related through a shared entry at the same index: (index, function) -> tables
    by_entry: dict[tuple[int, int], list[int]] = {}
    for n, t in enumerate(tables):
        for i, f in enumerate(t):
            if count[f] <= SHARED_MAX:
                by_entry.setdefault((i, f), []).append(n)
    out: dict[int, set[int]] = {}
    for n, t in enumerate(tables):
        related: set[int] = set()
        for i, f in enumerate(t):
            related.update(by_entry.get((i, f), ()))
        related.discard(n)
        for i, f in enumerate(t):
            sib = {tables[m][i] for m in related if i < len(tables[m]) and tables[m][i] != f}
            if sib:
                out.setdefault(f, set()).update(sib)
    return out


@dataclass
class Hit:
    address: int
    file: str
    symbol: str
    demangled: str
    decl: Declared
    regs: dict[int, int] = field(default_factory=dict)  # beyond-declared register -> tier
    boot: bool = False
    callers: frozenset = frozenset()  # registers of `regs` the direct callers set (`*`)

    @property
    def generated(self) -> bool:
        return self.file.startswith(GENERATED)

    def counted(self, with_open: bool) -> dict[int, int]:
        return {k: t for k, t in self.regs.items() if with_open or t < OPEN}

    def row(self, with_open: bool) -> dict:
        regs = self.counted(with_open)
        return {"address": f"0x{self.address:x}", "file": self.file, "boot": self.boot,
                "generated": self.generated,
                "declared": {"ints": self.decl.ints, "floats": self.decl.floats, "aggregate": self.decl.aggregate},
                "regs": {reg_name(k): TIERS[t] for k, t in sorted(regs.items())},
                "callers_set": [reg_name(k) for k in sorted(self.callers) if k in regs], "demangled": self.demangled}


def source_of(obj: Path, check_dir: Path, src_dir: Path) -> Path | None:
    rel = obj.relative_to(check_dir).with_suffix("")
    for ext in (".cpp", ".c", ".cc"):
        p = (src_dir / rel).with_suffix(ext)
        if p.is_file():
            return p
    return None


def matched_functions(version: str, only: str | None = None) -> list[tuple[int, str, str]]:
    """(address, symbol, source path relative to the repository) of each byte-matching function in
    build/check/<version>/ whose source still exists in src/ (stale objects are ignored)."""
    return [(a, s, rel) for a, s, rel, _ in matched_objects(version, only)]


def matched_objects(version: str, only: str | None = None) -> list[tuple[int, str, str, Path]]:
    """matched_functions, with the object each function was compiled into."""
    check_dir = ROOT / "build" / "check" / version
    if not check_dir.is_dir():
        raise SystemExit(f"{check_dir.relative_to(ROOT).as_posix()} is missing: run tools/check.py --version {version}")
    out = []
    for dj in sorted(check_dir.rglob("*.o.diff.json")):
        obj = dj.with_name(dj.name[:-len(".diff.json")])
        src = source_of(obj, check_dir, ROOT / "src")
        if src is None:
            continue
        rel = src.relative_to(ROOT).as_posix()
        if only and rel != only.replace("\\", "/"):
            continue
        for e in json.loads(dj.read_text(encoding="utf-8")).get("entries", []):
            if e.get("matching"):
                out.append((int(e["address"]), e["symbol"], rel, obj))
    return out


def scan(version: str, with_open: bool = False, only: str | None = None) -> tuple[list[Hit], dict]:
    import abi_facts
    import asmdiff
    import callgraph
    funcs = matched_functions(version, only)
    seen: set[int] = set()
    funcs = [f for f in funcs if not (f[0] in seen or seen.add(f[0]))]  # C1/C2 aliases: once per address
    dem = asmdiff.demangle([s for _, s, _ in funcs], strip=False)
    orig = asmdiff.Original(version)
    rows = callgraph.load_rows()
    graph = callgraph.CallGraph(version, rows)
    boot = graph.reachable(callgraph.NNMAIN)
    af = abi_facts.AbiFacts(orig, rows, graph)
    af.version = version
    stats = {"matched": len(funcs), "variadic_or_unmangled": 0, "renamed": 0, "undecodable": 0}
    texts: dict[str, str] = {}

    def renamed(sym: str, rel: str) -> bool:
        """A definition emitted under another declaration's symbol (`asm("_Z13fn_...v")`, as the
        in-place constructors reflection declares `void()`): the symbol says nothing of its parameters."""
        if rel not in texts:
            texts[rel] = (ROOT / rel).read_text(encoding="utf-8", errors="replace")
        return f'asm("{sym}")' in texts[rel]
    an = Analyzer(af)
    siblings = vtable_siblings(pointer_tables(orig))
    strong: dict[int, frozenset] = {}

    def confirmed(g: int) -> frozenset:
        """Argument registers a sibling implementation reads in a tier below SLOT."""
        if g not in strong:
            live = an.tiers(g) or {}
            strong[g] = frozenset(k for k, t in live.items() if t < SLOT)
        return strong[g]

    hits = []
    for addr, sym, rel in funcs:
        if renamed(sym, rel):
            stats["renamed"] += 1
            continue
        d = declared(dem.get(sym, sym))
        if d is None:
            stats["variadic_or_unmangled"] += 1
            continue
        live = an.tiers(addr)
        if live is None:
            stats["undecodable"] += 1
            continue
        regs = {k: live[k] for k in beyond(live, d)}
        if any(t == OPEN for t in regs.values()):
            seen_slot = set().union(*(confirmed(g) for g in siblings.get(addr, ()))) if addr in siblings else set()
            regs = {k: SLOT if t == OPEN and k in seen_slot else t for k, t in regs.items()}
        h = Hit(addr, rel, sym, dem.get(sym, sym), d, regs, addr in boot,
                af.evidence(addr) & frozenset(regs) if regs else frozenset())
        if h.counted(with_open):
            hits.append(h)
    hits.sort(key=lambda h: (not h.boot, h.generated, h.address))
    return hits, stats


@dataclass
class CallHit:
    """A call in a matched function that does not pass on arguments the caller holds untouched."""
    caller: int
    file: str
    symbol: str      # the caller's
    demangled: str
    offset: int      # of the call within the caller
    callee: int      # the original's target
    callee_symbol: str
    callee_demangled: str
    callee_decl: Declared
    regs: frozenset  # the caller's declared argument registers the callee reads beyond its declaration
    cast: bool = False   # the source calls the callee through a reinterpret_cast
    boot: bool = False

    @property
    def generated(self) -> bool:
        return self.file.startswith(GENERATED)

    def row(self) -> dict:
        return {"caller": f"0x{self.caller:x}", "file": self.file, "offset": f"0x{self.offset:x}",
                "callee": f"0x{self.callee:x}", "callee_symbol": self.callee_demangled,
                "callee_declared": {"ints": self.callee_decl.ints, "floats": self.callee_decl.floats,
                                    "aggregate": self.callee_decl.aggregate},
                "regs": [reg_name(k) for k in sorted(self.regs)], "cast": self.cast, "boot": self.boot,
                "generated": self.generated, "demangled": self.demangled}


def declared_regs(d: Declared) -> frozenset:
    """The argument registers a declaration takes (x0.., v0..)."""
    return frozenset(range(min(d.ints, 8))) | frozenset(100 + k for k in range(min(d.floats, 8)))


def dropped(held, caller: Declared, reads, callee: Declared) -> frozenset:
    """Registers the caller declares and holds untouched at the call, which the callee reads beyond
    its own declaration: arguments our call does not pass on."""
    return frozenset(beyond(set(held) & declared_regs(caller) & set(reads), callee))


def call_sites(af, addr: int, relocs: dict) -> list[tuple[int, int, str, frozenset]]:
    """(offset, original target, our symbol, registers untouched from the entry) for each direct
    call or tail branch with a CALL26/JUMP26 relocation in our object."""
    import asmdiff
    held = af.untouched(addr)
    out = []
    for off, r in sorted(relocs.items()):
        if r.type not in (asmdiff.CALL26, asmdiff.JUMP26) or r.section:
            continue
        site = held.get(addr + off)
        if site is None or site[0] is None:
            continue  # a branch within the function, or a call nothing reaches
        out.append((off, site[0], r.symbol, site[1]))
    return out


def casts_callee(text: str, name: str) -> bool:
    """Whether the source may call `name` through a reinterpret_cast (to a wider signature): a cast
    of `name` itself, or, when the file names it, a cast to a function pointer type of a template
    parameter or variable (`reinterpret_cast<u32 (*)(UnkSelf*, void*)>(Step)`, Step = name)."""
    if re.search(r"reinterpret_cast\s*<[^;]*?>\s*\(\s*&?\s*(?:[\w:]+::)?" + re.escape(name) + r"\s*\)", text):
        return True
    return re.search(r"\b" + re.escape(name) + r"\b", text) is not None and re.search(
        r"reinterpret_cast\s*<[^;]*?\(\s*\*\s*\)[^;]*?>\s*\(\s*(?!fn_)[A-Za-z_]\w*\s*\)", text) is not None


def bare_name(demangled: str) -> str:
    """fn_7100001234 of `ns::C::fn_7100001234(int) const`."""
    import asmdiff
    return asmdiff.strip_params(demangled).rsplit("::", 1)[-1]


def scan_calls(version: str, only: str | None = None) -> tuple[list[CallHit], dict]:
    import abi_facts
    import asmdiff
    import callgraph
    funcs = matched_objects(version, only)
    seen: set[int] = set()
    funcs = [f for f in funcs if not (f[0] in seen or seen.add(f[0]))]  # C1/C2 aliases: once per address
    orig = asmdiff.Original(version)
    rows = callgraph.load_rows()
    graph = callgraph.CallGraph(version, rows)
    boot = graph.reachable(callgraph.NNMAIN)
    af = abi_facts.AbiFacts(orig, rows, graph)
    af.version = version
    an = Analyzer(af)
    objs: dict[Path, dict] = {}
    texts: dict[str, str] = {}
    sites = []
    stats = {"matched": len(funcs), "calls": 0, "caller_undeclared": 0, "callee_undeclared": 0}
    for addr, sym, rel, obj in funcs:
        if obj not in objs:
            objs[obj] = {f.symbol: f for f in asmdiff.read_object(obj)}
        f = objs[obj].get(sym)
        if f is None:
            continue
        for off, target, callee, held in call_sites(af, addr, f.relocs):
            sites.append((addr, sym, rel, off, target, callee, held))
    dem = asmdiff.demangle(sorted({s[1] for s in sites} | {s[5] for s in sites}), strip=False)
    hits = []
    for addr, sym, rel, off, target, callee, held in sites:
        stats["calls"] += 1
        if rel not in texts:
            texts[rel] = (ROOT / rel).read_text(encoding="utf-8", errors="replace")
        mine, theirs = declared(dem.get(sym, sym)), declared(dem.get(callee, callee))
        if mine is None or f'asm("{sym}")' in texts[rel]:
            stats["caller_undeclared"] += 1
            continue
        if theirs is None:
            stats["callee_undeclared"] += 1
            continue
        regs = dropped(held, mine, an.callee_args(target, CALL, 0), theirs)
        if not regs:
            continue
        cd = dem.get(callee, callee)
        hits.append(CallHit(addr, rel, sym, dem.get(sym, sym), off, target, callee, cd, theirs, regs,
                            casts_callee(texts[rel], bare_name(cd)), addr in boot))
    hits.sort(key=lambda h: (h.cast, not h.boot, h.generated, h.caller, h.offset))
    return hits, stats


def print_calls(hits: list[CallHit], stats: dict, boot_only: bool) -> None:
    for h in hits:
        if boot_only and not h.boot:
            continue
        d = h.callee_decl
        decl = f"{d.ints}x{'+A' if d.aggregate else ''}" + (f" {d.floats}v" if d.floats else "")
        print(f"0x{h.caller:x}+0x{h.offset:x} {'cast ' if h.cast else '     '}{'boot ' if h.boot else '     '}"
              f"{'gen ' if h.generated else '    '}{h.file}  calls {bare_name(h.callee_demangled)} (0x{h.callee:x}, "
              f"declared {decl})  drops {' '.join(reg_name(k) for k in sorted(h.regs))}")
    real = [h for h in hits if not h.cast]
    print(f"{len(real)} calls drop an argument ({sum(h.boot for h in real)} in the boot slice, "
          f"{sum(h.generated for h in real)} in generated sources, {len({h.caller for h in real})} callers); "
          f"{len(hits) - len(real)} more through a reinterpret_cast of the callee (not counted); "
          f"{stats['calls']} direct calls in {stats['matched']} matched functions, skipped "
          f"{stats['caller_undeclared']} with a variadic, unmangled or asm-labelled caller and "
          f"{stats['callee_undeclared']} with a variadic or unmangled callee")


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--calls", action="store_true",
                    help="list calls that do not pass on an argument the caller holds untouched")
    ap.add_argument("--open", action="store_true", help="also list registers only the open rule makes live")
    ap.add_argument("--boot", action="store_true", help="list boot-slice hits only")
    ap.add_argument("--file", help="only this source (repository-relative path)")
    ap.add_argument("--json", metavar="OUT", help="also write the hits as JSON")
    args = ap.parse_args()
    if args.calls:
        chits, cstats = scan_calls(args.version, args.file)
        print_calls(chits, cstats, args.boot)
        print(callgraph_caveat())
        if args.json:
            Path(args.json).write_text(json.dumps({"stats": cstats, "calls": [h.row() for h in chits]}, indent=1)
                                       + "\n", encoding="utf-8")
        return 0
    hits, stats = scan(args.version, args.open, args.file)
    shown = [h for h in hits if h.boot or not args.boot]
    for h in shown:
        regs = h.counted(args.open)
        d = h.decl
        decl = f"{d.ints}x{'+A' if d.aggregate else ''}" + (f" {d.floats}v" if d.floats else "")
        print(f"0x{h.address:x} {'boot ' if h.boot else '     '}{'gen ' if h.generated else '    '}{h.file}  declared {decl}  forwards "
              + " ".join(f"{reg_name(k)}({TIERS[t]}{'*' if k in h.callers else ''})" for k, t in sorted(regs.items())))
    n_boot = sum(h.boot for h in hits)
    n_gen = sum(h.generated for h in hits)
    n_conf = sum(bool(h.callers & frozenset(h.counted(args.open))) for h in hits)
    print(f"{len(hits)} hits ({n_boot} in the boot slice, {n_gen} in generated sources, {n_conf} with a register the callers set) among {stats['matched']} matched functions; "
          f"skipped {stats['variadic_or_unmangled']} variadic or unmangled, {stats['renamed']} under another declaration's symbol (asm label), {stats['undecodable']} undecodable"
          + ("" if args.open else "; open-rule-only registers not counted (--open)"))
    print(callgraph_caveat())
    if args.json:
        Path(args.json).write_text(json.dumps({"stats": stats, "hits": [h.row(args.open) for h in hits]}, indent=1)
                                   + "\n", encoding="utf-8")
    return 0


def callgraph_caveat() -> str:
    import callgraph
    return "Boot slice: " + callgraph.CAVEAT


if __name__ == "__main__":
    raise SystemExit(main())
