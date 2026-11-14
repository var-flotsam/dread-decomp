#!/usr/bin/env python3
"""The machine test for tier 2: is a hand-written function that does not match equivalent to the original?

`CLAUDE.md`, "Completion": a function whose only difference from the original is register choice or
instruction order (`trymatch.py`'s KIND line says `registers`) counts as done when a machine test runs the
original and ours on the same inputs with the same writes and calls. This is that test. It compiles the
source with the project compiler, lays the object out beside the original (tools/pcode_emu.py, the engine
tools/port_lift.py's oracle uses for lifts) and runs both from the same register and memory states. A
function passes only under all of these rules:

  registers only     asmdiff.classify says `registers`: the same instructions, constants and offsets in
                     another order or with other registers. Anything else is not judged at all: outside
                     this class the test accepts wrong code (a wrong array index, an off-by-one survived
                     the calibration with full coverage).
  anchored           the object's own strings, statics, vtables and functions sit at the original's
                     addresses, so an address that escapes into memory or a call is the same number.
  every write        every byte either version writes to memory, a stack object included, is compared.
  the result         compared as the definition's return type declares it.
  every call         library calls such as memcpy are calls like any other: target and arguments compared.
  full coverage      every instruction of the original ran, and every conditional branch went both ways,
                     over the inputs tried. Without that the verdict is `inconclusive`, never `pass`.

Verdicts: `pass`, `fail` (a run differed: the detail names the input and what differed), `inconclusive`
(coverage, a name with no known address, an instruction the emulator does not know, time) and `not registers`.
Calibration, 2026-11-11 (docs/JOURNAL.md): of 300 byte-matching functions none failed (59 passed, 241 could
not be judged), and of 95 registers-only one-point mutants in 59 functions none that changes behaviour was
accepted, which bounds the miss rate near 3%, not at zero. A pass is evidence, not a proof; a permuter is
what turns it into a match.

Inputs: the oracle's (seeds, then a random walk kept for new instructions), and since 2026-11-13 a systematic
pass of this tool's own (more_inputs): each argument, each of the first memory words read and each of the
first call results, set to 0, 1, 2, the function's comparison constants and their neighbours, all bits set,
and for a memory word what the run's other words held. It reaches the null-argument path, the count that
hits zero and the `p == g_empty` branch, which is what full coverage most often lacked. Calibrated the same
day on 94 registers-only mutants: 64 rejected and none accepted without it, 66 rejected and none accepted
with it.

Needs the dump, the Ghidra-derived caches of the port oracle (build/port/abi_facts_v*.pickle and
pcode_cache.pickle; tools/worktree_setup.py --oracle copies them into a worktree) and, for names other than
fn_<addr>, build/port/addresses.json (tools/port_addresses.py). New instruction words go through
tools/ghidra/pcode.py, one process at a time.

Usage:
  python tools/equiv_test.py --version 2.1.0 FILE.cpp [...]   # the non-matching functions these files define
  python tools/equiv_test.py --version 2.1.0 --minor          # every file under src/ with NON_MATCHING(minor)
  python tools/equiv_test.py --version 2.1.0 --drafts         # the `registers or order` rows of drafts/INDEX.csv
  ... --write          # record the verdicts of functions under src/ in data/equivalence.csv
  ... --json OUT       # every verdict with its detail
  ... --budget SECONDS # per function (default 60)
data/equivalence.csv has one row per judged function of src/: address, the digest of the code that was judged
(sha256 of our function's bytes in the object and of what its relocations name, a function data/names.csv
names spelled as its fn_<addr> placeholder so that a rename leaves the row current), the verdict and the
rules' version. A row whose digest is not
the function's code any more says nothing: `current()` is how a reader of the table tells.
"""

from __future__ import annotations

import argparse
import collections
import csv
import hashlib
import json
import re
import subprocess
import sys
import time
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
sys.path.insert(0, str(Path(__file__).resolve().parent))

TABLE = ROOT / "data" / "equivalence.csv"
FIELDS = ["address", "digest", "verdict", "rules", "detail"]
RULES = 2  # raise when a rule above or the digest changes: rows of an older version are not current
#            (2: the digest covers what the relocations name, not the code bytes alone)
WORK = ROOT / "build" / "equiv"
SCALAR = {"void": None, "bool": "x8", "u8": "x8", "s8": "x8", "char": "x8", "u16": "x16", "s16": "x16",
          "u32": "x32", "s32": "x32", "int": "x32", "unsigned": "x32", "f32": "f32", "float": "f32",
          "f64": "f64", "double": "f64", "u64": "x64", "s64": "x64", "long": "x64", "size_t": "x64"}
DROPPED = re.compile(r"__attribute__\s*\(\(.*?\)\)|extern\s+\"C\"|\b(static|inline|HIDDEN|const|constexpr)\b")
COMMENT = re.compile(r"//[^\n]*|/\*.*?\*/", re.S)


# --- the parts that need no game data (tools/tests/test_equiv_test.py) -------------------------------

def digest(code: bytes, relocs=None) -> str:
    """The identity of a judged function: its bytes in the object and what its relocations name. The bytes
    alone are not enough: the relocated fields are still zero there, so the same bytes can call another
    function or read another global. `relocs` maps an offset to an object with type, symbol, addend,
    section and data (asmdiff.ObjReloc); a string or constant counts by its content, not its place. A symbol
    that names a function of data/names.csv counts as its placeholder would (dread.names.normalise_symbol): a
    rename changes no code, so it leaves the digest of every caller as it was."""
    from dread.names import normalise_symbol
    h = hashlib.sha256(code)
    for off, r in sorted((relocs or {}).items()):
        what = (r.data or b"").hex() if r.section else f"{normalise_symbol(r.symbol)}+{r.addend}"
        h.update(f"|{off}:{r.type}:{what}".encode())
    return h.hexdigest()[:32]


def return_type(text: str, address: int) -> str | None:
    """The return type the definition of fn_<address> is written with, attributes and storage dropped; None
    when the text has no such definition (a call or a declaration of it does not count)."""
    text = DROPPED.sub(" ", COMMENT.sub(" ", text))
    for m in re.finditer(rf"^([^;{{}}()=#]*?)\b(?:\w+::)*fn_{address:x}\s*\(", text, re.M):
        depth, i = 1, m.end()
        while depth and i < len(text):  # to the end of the parameter list
            depth += {"(": 1, ")": -1}.get(text[i], 0)
            i += 1
        rest = text[i:i + 80].lstrip()
        rtype = " ".join(m.group(1).split())
        if rtype and rest.startswith(("{", "const {", "noexcept {", "const noexcept {")):
            return rtype
    return None


def result_kind(rtype: str | None, shape=None) -> str | None:
    """What the engine compares as the result: a register kind, `kind*count` for a small class returned in
    registers (`shape` from the original's callers), or None for nothing or a class returned in memory,
    whose bytes are compared as writes."""
    if rtype is None:
        return None
    if "*" in rtype or "&" in rtype:
        return "x64"
    if rtype in SCALAR:
        return SCALAR[rtype]
    if shape:
        return {"x": "x64", "s": "f32", "d": "f64"}[shape[0]] + f"*{shape[1]}"
    return None


def conditional_branches(code: bytes, address: int) -> list[int]:
    """The addresses of b.cond, cbz/cbnz and tbz/tbnz in a function."""
    out = []
    for i in range(0, len(code) - 3, 4):
        w = int.from_bytes(code[i:i + 4], "little")
        if (w & 0xFF000010) == 0x54000000 or (w & 0x7C000000) == 0x34000000:
            out.append(address + i)
    return out


def one_way(branches: list[int], outcomes: set) -> list[int]:
    """The conditional branches that were not seen both taken and not taken."""
    return [b for b in branches if (b, True) not in outcomes or (b, False) not in outcomes]


def read_table(path: Path = TABLE) -> dict[int, dict]:
    if not path.is_file():
        return {}
    with path.open(newline="", encoding="utf-8") as fh:
        return {int(r["address"], 16): r for r in csv.DictReader(fh)}


def write_table(rows: dict[int, dict], path: Path = TABLE) -> None:
    with path.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, FIELDS, lineterminator="\n")
        w.writeheader()
        for a in sorted(rows):
            w.writerow({k: rows[a].get(k, "") for k in FIELDS})


def current(row: dict | None, code_digest: str) -> bool:
    """A table row counts when it is a pass, under today's rules, of exactly this code."""
    return bool(row) and row["verdict"] == "pass" and row["digest"] == code_digest and row["rules"] == str(RULES)


def short(detail: str, limit: int = 110) -> str:
    return " ".join(detail.replace(",", ";").split())[:limit]


# --- the engine's driver (needs the dump and the oracle's caches) ------------------------------------

class Context:
    """The original, its ABI facts and the name-to-address tables one run shares."""

    def __init__(self, version: str):
        import abi_facts
        import asmdiff
        import callgraph
        import port_lift
        self.version = version
        self.orig = asmdiff.Original(version)
        self.rows = callgraph.load_rows()
        self.af = abi_facts.AbiFacts(self.orig, self.rows)
        self.af.version = version
        self.af.load(port_lift.FACTS)
        self.import_by_name: dict[str, int] = {}
        for a, r in self.rows.items():
            if r["name"].startswith(callgraph.IMPORT_PREFIX):
                self.import_by_name.setdefault(r["name"][len(callgraph.IMPORT_PREFIX):], a)
        img = self.orig.img
        self.data_imports = {r.sym: int.from_bytes(img.data[off:off + 8], "little")
                             for off, r in img.relocs.items() if r.sym and r.type in (257, 1025)}
        self.by_name = collections.defaultdict(list)
        for a, r in self.rows.items():
            self.by_name[r["name"]].append(a)
        self.port: dict[str, int] = {}
        addresses = ROOT / "build" / "port" / "addresses.json"
        if addresses.is_file():
            self.port = {k: int(v["address"], 16)
                         for k, v in json.loads(addresses.read_text(encoding="utf-8"))["resolved"].items()}

    def address_of(self, mangled: str, demangled: str) -> int | None:
        """The original address a symbol name stands for, or None."""
        import asmdiff
        bare = asmdiff.strip_params(demangled)
        m = asmdiff.TARGET_RE.match(bare)
        if m:
            return int(m.group(1), 16)
        if len(self.by_name.get(bare, ())) == 1:
            return self.by_name[bare][0]
        return self.port.get(mangled)


def original_refs(code: bytes, address: int) -> set[int]:
    """The absolute addresses a function forms with adr, adrp + add and adrp + a load or store."""
    import capstone
    from capstone import arm64_const as c
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    pages: dict[int, int] = {}
    refs: set[int] = set()
    for i in cs.disasm(code, address):
        ops = i.operands
        if i.mnemonic == "adrp":
            pages[ops[0].reg] = ops[1].imm
            continue
        if i.mnemonic == "adr":
            refs.add(ops[1].imm)
            continue
        for o in ops:
            if o.type == c.ARM64_OP_MEM and o.mem.base in pages and not o.mem.index:
                refs.add(pages[o.mem.base] + o.mem.disp)
        if i.mnemonic == "add" and len(ops) == 3 and ops[1].type == c.ARM64_OP_REG and ops[1].reg in pages \
                and ops[2].type == c.ARM64_OP_IMM:
            refs.add(pages[ops[1].reg] + ops[2].imm)
        if ops and ops[0].type == c.ARM64_OP_REG \
                and not i.mnemonic.startswith(("str", "stp", "cmp", "tst", "b", "cb", "tb")):
            pages.pop(ops[0].reg, None)
    return refs


SOURCES: dict[Path, str] = {}  # object -> its source (a repository path), for the special members' labels


def special_address(sym: str, name: str, obj: Path) -> int | None:
    """A constructor's or destructor's address from its declaration (tools/dread/asm_labels.py): Clang 9
    ignores an asm label on one, so the object names only the mangled symbol."""
    from dread import asm_labels
    if not asm_labels.is_special(sym):
        return None
    import check
    return check.labels().lookup(sym, name, SOURCES.get(obj))[0]


def load(ctx: Context, obj: Path, addr: int):
    """The object laid out for judging the function at `addr`: (LiftObject, its strings the image lacks)."""
    import asmdiff
    import pcode_emu
    from elftools.elf.elffile import ELFFile
    symbols = set()
    with obj.open("rb") as fh:
        for s in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
            if s.name:
                symbols.add(s.name)
    demangled = asmdiff.demangle(sorted(symbols), strip=False)
    mangled = {d: m for m, d in demangled.items()}
    img = ctx.orig.img
    base = pcode_emu.BASE
    refs = original_refs(ctx.orig.function(addr), addr)
    missing: list[str] = []
    blobs: dict[str, bytes] = {}

    def resolve(name: str):
        if name.split("(")[0] in ctx.import_by_name:
            return ctx.import_by_name[name.split("(")[0]]
        sym = mangled.get(name, name)
        if sym in ctx.data_imports:
            return ctx.data_imports[sym]
        hit = ctx.address_of(sym, name)
        return hit if hit is not None else special_address(sym, name, obj)

    def anchor(name, section, offset, kind):
        if name is None:
            if not (section.name.startswith(".rodata.str") and section["sh_entsize"] == 1):
                return None  # constant pools, jump tables: values, not addresses; the object's own
            blob = blobs.setdefault(section.name, section.data())
            end = blob.find(b"\0", offset)
            text = blob[offset:end + 1]
            for r in sorted(refs):  # the place the original function itself points at, when it has one
                if img.data[r - base:r - base + len(text)] == text:
                    return r
            at = img.data.find(text, img.rodata[0], img.rodata[1])
            if at < 0:
                missing.append(text[:-1].decode("latin-1"))
                return False
            return base + at
        there = ctx.address_of(name, demangled.get(name, name))
        if there is None:
            there = special_address(name, demangled.get(name, name), obj)
        if kind == "STT_FUNC":
            return there + offset if there is not None else False
        if section["sh_flags"] & 1 or name.startswith(("_ZTV", "_ZTI", "_ZTS", "_ZGV")):
            return there + offset if there is not None else False  # state or identity: must be the original's
        return None  # read-only named data (a switch's table, a constant array): the object's own copy

    return pcode_emu.load_object(str(obj), resolve, demangled, anchor=anchor), missing


def words_of(ctx: Context, pairs) -> set[int]:
    """Every instruction word the original and our versions of these functions hold (for the P-code cache)."""
    import pcode_emu
    words: set[int] = set()
    for a, obj in pairs:
        lo, _ = load(ctx, obj, a)
        if a in lo.functions:
            start, size = lo.functions[a]
            words |= {pcode_emu._word(lo, x) for x in range(start, start + size, 4)}
        code = ctx.orig.function(a)
        words |= {int.from_bytes(code[i:i + 4], "little") for i in range(0, len(code), 4)}
    return words


def strict_same_arg(u, v, frame=None):
    """pcode_emu.same_arg without its tolerance for a stack object's padding: every byte both versions
    wrote counts, except a word that is a stack address in both (the frames are laid out alike, not at
    the same place)."""
    import pcode_emu
    if isinstance(u, tuple) and isinstance(v, tuple) and len(u) > 4 and len(v) > 4:
        a, b = u[1], v[1]
        n = min(len(a), len(b), u[4], v[4])
        j = 0
        while j < n:
            if j + 8 <= n and None not in a[j:j + 8] and None not in b[j:j + 8]:
                wa = int.from_bytes(bytes(a[j:j + 8]), "little")
                wb = int.from_bytes(bytes(b[j:j + 8]), "little")
                if pcode_emu.STACK_LO <= wa < pcode_emu.ENTRY_SP and pcode_emu.STACK_LO <= wb < pcode_emu.ENTRY_SP:
                    j += 8
                    continue
            if a[j] != b[j]:
                return False
            j += 1
        return True
    return _LOOSE(u, v, pcode_emu.ENTRY_SP if frame is None else frame)


_LOOSE = None


MORE_INPUTS = True       # (a test, or a comparison with the oracle's inputs alone, turns it off)
SYSTEMATIC = (0, 1, 2)   # the values the systematic inputs set each thing to
SYS_FLOATS = (0.0, 1.0, -1.0)
SYS_WORDS = 16           # of the memory words a run read, the first
SYS_CALLS = 8            # of the calls a run made, the first
SYS_READ = 32            # of the words a run read, the first: their values, for the other words
SYS_FRONTIER = 4         # runs each round starts from
SYS_ROUNDS = 2


def more_inputs(oracle, a: int) -> int:
    """Systematic inputs on top of the oracle's; returns how many runs were added to its list for `a`.

    Past its first seeds the oracle's search is a random walk (one input and one value per try, a few dozen
    tries) that keeps a run only for a new instruction. That misses what full coverage most often lacks: the
    null argument (`cbz x0`), the count that reaches zero, the branch whose two sides are both reached by
    other paths. So from each of the first runs kept, every argument register, each of the first memory
    words the run read and each of its first calls' results is set in turn to 0, 1, 2, the constants the
    function compares with and their neighbours, and all bits set, and a run is kept
    when it reaches an instruction or a branch outcome of this function that no run before it did. A second
    round starts from the runs the first one added. More inputs can only add comparisons: a function that
    differs on one of them fails where it was inconclusive before."""
    import pcode_emu
    import port_lift
    runs = oracle.originals(a)
    size = oracle.orig.sizes[a]
    own = set(conditional_branches(oracle.orig.function(a), a))
    seen: set = set()
    outs: set = set()

    def taken(m) -> set:
        return {o for o in m.outcomes if o[0] in own}
    for _, _, m in runs:
        if not isinstance(m, str):
            seen |= m.visited
            outs |= taken(m)

    def done() -> bool:
        return len(seen) * 4 >= size and not one_way(sorted(own), outs)
    f = oracle.af.facts(a)
    regs = sorted((f.args if f is not None else frozenset(range(8))) - {8})
    guard_words = {g & ~7 for g in oracle.preset_orig}
    img = oracle.orig.img

    def mutable(w: int) -> bool:  # as port_lift.Oracle.originals: not a guard, a relocated slot or read-only data
        off = w - pcode_emu.BASE
        return w not in guard_words and img.relocs.get(off) is None and not img.in_text(off) \
            and not img.rodata[0] <= off < img.rodata[1]
    floats = [pcode_emu.f_to(v, 4) for v in SYS_FLOATS]
    # with the constants the function compares with and their neighbours, and all bits set (a sentinel)
    consts = port_lift.compare_constants(oracle.orig.function(a))
    ints = list(dict.fromkeys([*SYSTEMATIC, *consts, *[c + 1 for c in consts], *[c - 1 for c in consts if c],
                               0xFFFFFFFF, 0xFFFFFFFFFFFFFFFF]))
    added = 0
    frontier = [r for r in runs if not isinstance(r[2], str)][:SYS_FRONTIER]
    for _ in range(SYS_ROUNDS):
        found = []
        for seed, ovr, base in frontier:
            keys = [("r", k) for k in regs] \
                + [("m", w) for w in [w for w in base.read_words if mutable(w)][:SYS_WORDS]] \
                + [("c", n) for n in range(1, min(base.calls, SYS_CALLS) + 1)]
            # a memory word also takes what the run's other words held: `if (p == g_empty)` compares two things
            # the function loaded, and only one of them is the input's to choose
            read = [v for v in dict.fromkeys(port_lift.held(base, w) for w in base.read_words[:SYS_READ]) if v > 2]
            for key in keys:
                if key in ovr:
                    continue
                pool = floats if key[0] == "r" and key[1] >= 100 else ints + read if key[0] == "m" else ints
                for v in pool:
                    if done() or oracle.out_of_time("systematic inputs"):
                        return added
                    o = {**ovr, key: port_lift.placed(base, key[1], v) if key[0] == "m" else v}
                    try:
                        m = oracle.run_original(a, seed, o)
                    except pcode_emu.Inconclusive:
                        continue
                    if m.visited - seen or taken(m) - outs:
                        runs.append((seed, o, m))
                        found.append((seed, o, m))
                        seen |= m.visited
                        outs |= taken(m)
                        added += 1
        frontier = found[:SYS_FRONTIER]
        if not frontier:
            break
    return added


def judge(ctx: Context, oracle, obj: Path, a: int, text: str, budget: float = 60.0) -> tuple[str, str]:
    """(verdict, detail) for the function at `a` of `obj` under the strict rules. The caller has checked
    that the difference is registers only."""
    import pcode_emu
    import port_lift
    global _LOOSE
    _LOOSE = _LOOSE or pcode_emu.same_arg
    window, modeled = pcode_emu.WINDOW, pcode_emu.MODELED
    pcode_emu.same_arg, pcode_emu.WINDOW, pcode_emu.MODELED = strict_same_arg, 256, frozenset()
    try:
        lo, missing = load(ctx, obj, a)
        oracle.retry_runs.clear()
        oracle.deadline, oracle.over = port_lift._clock() + budget, None
        if a not in lo.functions:
            return "inconclusive", "function not registered in the object"
        start, size = lo.functions[a]
        if missing and any(start <= p < start + size for p in lo.unresolved):
            return "fail", f"string not in the original: {missing[0][:40]!r}"
        rtype = return_type(text, a)
        if rtype is None:
            return "inconclusive", "no definition of the function found in the source: its result type is not known"
        ret = result_kind(rtype, ctx.af.result_shape(a))
        try:
            if MORE_INPUTS:
                more_inputs(oracle, a)
            v, d, _, covered = oracle.check(a, lo, ret)
            outcomes: set = set()
            for _, _, mo in oracle.orig_runs.get(a, ()):
                if not isinstance(mo, str):
                    outcomes |= mo.outcomes
        except Exception as e:  # the emulator on an instruction or a state it does not model
            return "inconclusive", f"emulator: {type(e).__name__}: {e}"
        finally:
            oracle.orig_runs.pop(a, None)
        if v != "pass":
            return v, d
        osize = ctx.orig.sizes[a]
        if len(covered) * 4 < osize:
            return "inconclusive", f"only {100 * len(covered) * 4 // osize}% of the original's instructions ran"
        branches = conditional_branches(ctx.orig.function(a), a)
        left = one_way(branches, outcomes)
        if left:
            return "inconclusive", f"{len(left)} of {len(branches)} conditional branches went only one way"
        return v, d
    finally:
        pcode_emu.same_arg, pcode_emu.WINDOW, pcode_emu.MODELED = _LOOSE, window, modeled


# --- the command line ---------------------------------------------------------------------------------

def sources(args) -> list[Path]:
    """The files to judge. For --drafts, ONLY[file] is the one function its row names (a draft file may hold
    its unit's other functions too, which have rows or files of their own)."""
    if args.minor:
        r = subprocess.run(["git", "grep", "-l", "NON_MATCHING(minor)", "--", "src"], cwd=ROOT, capture_output=True,
                           text=True)
        return [ROOT / p for p in r.stdout.split() if p.endswith(".cpp")]
    if args.drafts:
        with (ROOT / "drafts" / "INDEX.csv").open(newline="", encoding="utf-8") as fh:
            rows = [r for r in csv.DictReader(fh) if r["difference"] == "registers or order"]
        ONLY.update({(ROOT / r["file"]).resolve(): int(r["address"], 16) for r in rows})
        return [ROOT / r["file"] for r in rows]
    return [p.resolve() for p in args.files]


ONLY: dict[Path, int] = {}


def compile_all(files: list[Path]) -> list[tuple[Path, Path, str]]:
    """[(source, object, error)] with the project compiler; objects under build/equiv/."""
    import buildcfg
    from concurrent.futures import ThreadPoolExecutor

    def one(src: Path):
        rel = src.relative_to(ROOT) if src.is_relative_to(ROOT) else Path(src.name)
        obj = WORK / rel.with_suffix(".o")
        obj.parent.mkdir(parents=True, exist_ok=True)
        r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
        return src, obj, "" if r.returncode == 0 else (r.stderr.strip().splitlines() or ["compiler failed"])[0]
    with ThreadPoolExecutor(8) as ex:
        return list(ex.map(one, files))


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("files", nargs="*", type=Path)
    ap.add_argument("--minor", action="store_true", help="every file under src/ with NON_MATCHING(minor)")
    ap.add_argument("--drafts", action="store_true", help="the `registers or order` rows of drafts/INDEX.csv")
    ap.add_argument("--write", action="store_true", help="record verdicts of functions under src/ in data/equivalence.csv")
    ap.add_argument("--json", type=Path, help="write every verdict with its detail")
    ap.add_argument("--budget", type=float, default=60.0, help="seconds per function (default 60)")
    args = ap.parse_args()
    files = sources(args)
    if not files:
        ap.error("no files: name some, or --minor, or --drafts")

    import asmdiff
    import check
    import port_lift
    ctx = Context(args.version)
    rows, _ = check.load_csv()
    by_name = check.csv_by_name(rows)
    jobs = []  # (address, object, source text, source, code)
    out = []
    for src, obj, err in compile_all(files):
        rel = src.relative_to(ROOT).as_posix() if src.is_relative_to(ROOT) else str(src)
        if err:
            out.append({"file": rel, "address": "", "verdict": "inconclusive", "detail": f"does not compile: {err}"})
            continue
        funcs = asmdiff.read_object(obj)
        codes = {f.symbol: f.code for f in funcs}
        relocs = {f.symbol: f.relocs for f in funcs}
        SOURCES[obj] = rel
        results, _, _, _ = check.diff_with_targets(funcs, ctx.orig, by_name, source=rel)
        text = src.read_text(encoding="utf-8", errors="replace")
        for res in results:
            if res.bytes_match or ONLY.get(src.resolve(), res.address) != res.address:
                continue
            kind = asmdiff.classify(ctx.orig.function(res.address), codes[res.symbol]).split(" ")[0]
            row = {"file": rel, "address": f"{res.address:#x}", "size": res.orig_size,
                   "digest": digest(codes[res.symbol], relocs[res.symbol])}
            if kind != "registers":
                out.append({**row, "verdict": "not registers", "detail": f"the difference is {kind}: not judged"})
            else:
                jobs.append((res.address, obj, text, row))
    print(f"{len(files)} files, {len(jobs)} registers-only functions to judge, "
          f"{len(out)} left out (another kind of difference, or no build)", flush=True)
    if jobs:
        t = time.time()
        db = port_lift.pcode_db(words_of(ctx, [(a, obj) for a, obj, _, _ in jobs]),
                                ROOT / "build" / "port" / "pcode_cache.pickle")
        oracle = port_lift.Oracle(ctx.orig, ctx.af, db, port_lift.INPUTS)
        for n, (a, obj, text, row) in enumerate(jobs, 1):
            v, d = judge(ctx, oracle, obj, a, text, args.budget)
            out.append({**row, "verdict": v, "detail": d})
            if n % 25 == 0:
                print(f"  {n} of {len(jobs)} ({time.time() - t:.0f} s)", flush=True)
    counts = collections.Counter(o["verdict"] for o in out)
    for o in sorted(out, key=lambda o: (o["verdict"] != "pass", o["verdict"], o["file"])):
        print(f"{o['verdict']:13} {o['address']:>12}  {o['file']}  {short(o['detail'], 90)}")
    passed = sum(o.get("size", 0) for o in out if o["verdict"] == "pass")
    print(f"equiv_test: {counts['pass']} pass ({passed:,} bytes), {counts['fail']} fail, "
          f"{counts['inconclusive']} cannot be judged, {counts['not registers']} not registers-only")
    if args.json:
        args.json.parent.mkdir(parents=True, exist_ok=True)
        args.json.write_text(json.dumps(out, indent=1), encoding="utf-8")
    if args.write:
        table = read_table()
        n = 0
        for o in out:
            if o["file"].startswith("src/") and o["address"] and o["verdict"] != "not registers":
                table[int(o["address"], 16)] = {"address": o["address"], "digest": o["digest"], "verdict": o["verdict"],
                                                 "rules": str(RULES), "detail": short(o["detail"])}
                n += 1
        write_table(table)
        print(f"{n} rows written to {TABLE.relative_to(ROOT).as_posix()} ({len(table)} in all)")
    return 1 if counts["fail"] else 0


if __name__ == "__main__":
    sys.exit(main())
