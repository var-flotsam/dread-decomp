#!/usr/bin/env python3
"""Semantic lifts for the port: a source for every boot-slice function nobody has written yet.

Milestone M1 of docs/PORT_PLAN.md needs a definition for every function reachable from nnMain. Hand
matching cannot cover the 2,148 `U` functions of the boot slice in time, and tools/gen_lift.py keeps
only byte-identical lifts. This tool lifts them with gen_lift's lifters in **semantic mode** and
accepts a lift that behaves like the original, whatever its bytes:

  1. Worklist: build/port/link/boot_missing.csv (tools/port_link.py; `U` rows, by callers in the
     slice), --only, or --top N of it. Functions now defined in src/ are skipped, except the
     overrides: src/lifted definitions that read more argument registers than they declare
     (byte-identical on AArch64, where the registers pass through; an x86-64 compile loses them).
     They are listed in build/port/passthrough.json (--passthrough writes just that), added to the
     worklist (--no-overrides leaves them out), and an accepted lift of one is marked in the
     manifest with "overrides": <its src/lifted file>, so tools/port_link.py links it instead.
  2. Lift every gen_lift variant with opts {"semantic"}: a direct call passes every argument
     register the callee reads (tools/abi_facts.py), so nothing reaches a callee by a register
     the C call does not name (a float only passed on as f64, which keeps all its bits); a
     callee's x8 result is placed by C++17 copy elision; calls with stack arguments are
     rejected; every condition follows the flags exactly (carry and overflow after adds/cmn,
     C and V clear after ands/tst), and flat mode also keeps NZCV in locals for a condition in
     another block; Clang's compressed jump tables become a switch; string literals stay the
     game's own addresses; division by zero and shift amounts follow AArch64; flat-mode argument
     registers read before their first write in address order are the incoming arguments, other
     registers zero-initialised locals.
  3. Compile each variant for AArch64 with the project compiler and SEMANTIC_FLAGS (no strict
     aliasing, wrapping signed arithmetic, null checks kept), the C semantics the x86-64 build
     uses too.
  4. Oracle (b): run the original and each compiled variant, in variant order, as Ghidra P-code on
     --inputs seeds (tools/pcode_emu.py); the first variant whose calls, memory writes and result
     agree with the original's on every conclusive input is accepted.
  5. Compile the accepted lifts for x86-64 (tools/port_sweep.py's compiler and flags, plus
     SEMANTIC_FLAGS): one source and one object per function under <out>/, which
     tools/port_link.py --lifted <out> links.

Semantic lifts never count as matching and never enter src/ (tools/check.py does not see them).
Where they live is open decision 2 of docs/PORT_PLAN.md; until the owner decides otherwise they are
generated into build/port/lifted/ (gitignored) with GENERATOR (the version below) and the blob hashes
of the generator sources in <out>/manifest.json, so a stale tree is recognisable. Each function has
an outcome in the manifest and the report: accepted, rejected by the oracle (with the first
difference), inconclusive (no input ran to a comparable end: time-outs, an instruction the emulator
does not model), rejected for an x86-64 compile error, or rejected by the lifter (its reason, as
gen_lift --report gives it).

Usage:
  python tools/port_lift.py --version 2.1.0                      # the whole worklist
  python tools/port_lift.py --version 2.1.0 --top 200            # its first 200 entries
  python tools/port_lift.py --version 2.1.0 --only 710007fe68,7101047270 [-v]
Needs the project compiler, the host clang++ (tools/port_sweep.py), Ghidra with PyGhidra
(tools/ghidra/pcode.py, run as a subprocess) and build/ghidra_export/<version>/ (callers).
"""

from __future__ import annotations

import argparse
import bisect
import csv
import hashlib
import json
import os
import pickle
import re
import subprocess
import sys
import time
from collections import Counter
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import abi_facts  # noqa: E402
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
import callgraph  # noqa: E402
import gen_lift  # noqa: E402
import pcode_emu  # noqa: E402
import port_sweep  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
GENERATOR = "port_lift 2"  # 2: "overrides" entries
SOURCES = ["tools/port_lift.py", "tools/gen_lift.py", "tools/abi_facts.py", "tools/pcode_emu.py", "tools/ghidra/pcode.py"]
DEFAULT_OUT = ROOT / "build" / "port" / "lifted"
PARTIAL_OUT = ROOT / "build" / "port" / "lifted_partial"
WORKLIST = ROOT / "build" / "port" / "link" / "boot_missing.csv"
FACTS = ROOT / "build" / "port" / "abi_facts.pickle"  # abi_facts.AbiFacts cache (depends on the binary only)
SEMANTIC_FLAGS = ["-fno-strict-aliasing", "-fwrapv", "-fno-delete-null-pointer-checks"]
OPTS = frozenset({"semantic"})
INPUTS = 6          # oracle seeds always run
MAX_SEEDS = 40      # more seeds while they reach new instructions of the original ...
PATIENCE = 10       # ... and one of the last PATIENCE did
NOTE = ["// Generated by tools/port_lift.py (semantic lift for the port; never a match). Regenerate, never edit.",
        "// It behaves like the original on the oracle's inputs (tools/pcode_emu.py); see manifest.json."]
# Each x86-64 source opens its function with DREAD_PORT_LIFTED(<address>): nothing by default; a boot
# built with -DDREAD_PORT_LIFTED(a)=dread_port_lifted(a) (and that function in the runtime) can log which
# semantic lifts the game runs, the first step of oracle (c).
TRACE_HOOK = ["#ifndef DREAD_PORT_LIFTED", "#define DREAD_PORT_LIFTED(address) ((void)0)", "#endif"]
RET_KIND = {"void": None, "char*": "x64", "char&": "x64", "u64": "x64", "u32": "x32", "u16": "x16",
            "u8": "x8", "bool": "x8", "f32": "f32", "f64": "f64", "UnkResult": None}
# Run-time roots: nnMain hands the game's main, per-frame and shutdown functions to the SDK's loop as
# function pointers, and main stores four callbacks, so the slice by direct calls from nnMain misses
# what they reach.
RUNTIME_ROOTS = (0x7100000980, 0x7100000d40, 0x7100000cf0, 0x7100015fc0, 0x710109c880, 0x7100001714, 0x7100001718)
PASSTHROUGH = ROOT / "build" / "port" / "passthrough.json"


def blob(path: str) -> str:
    data = (ROOT / path).read_bytes()
    return hashlib.sha1(b"blob %d\0" % len(data) + data).hexdigest()


def worklist(path: Path) -> list[int]:
    with path.open(newline="", encoding="utf-8") as fh:
        return [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] == "U"]


# --- byte-identical lifts that lose arguments on x86-64 ----------------------------------------------

def boot_reach(version: str, rows: dict) -> set[int]:
    """The boot path: everything reachable by direct calls from nnMain or a run-time root."""
    graph = callgraph.CallGraph(version, rows)
    reach: set[int] = set()
    for r in (callgraph.NNMAIN,) + RUNTIME_ROOTS:
        reach |= graph.reachable(r)
    return reach


def declared(block: str) -> tuple[int, int]:
    """(integer, float) parameters of a src/lifted definition (gen_lift.Lifter.signature's form:
    the integers, then the floats, each `<type> <name>`)."""
    line = next(ln for ln in block.splitlines() if gen_lift.NAME_RE.match(ln) and ln.endswith("{"))
    inner = line[line.index("(", line.index(" fn_")) + 1:line.rindex(")")].strip()
    types = [p.strip().rsplit(" ", 1)[0] for p in inner.split(",")] if inner else []
    floats = sum(t in ("f32", "f64") for t in types)
    return len(types) - floats, floats


def passthrough(af: abi_facts.AbiFacts, reach: set[int]) -> list[dict]:
    """src/lifted definitions that read more argument registers (abi_facts) than their parameter
    list declares. On AArch64 the registers pass through, so the bytes match; compiled for x86-64
    the values are lost unless the call is a pure tail call. tools/gen_lift.py passes on every
    register it can (pass-through); these are the ones it cannot express byte-identically, which
    get a semantic lift that overrides the src/lifted copy (manifest "overrides")."""
    out = []
    for a, block in sorted(gen_lift.read_out().items()):
        f = af.facts(a)
        if f is None:
            continue
        ints, floats = declared(block)
        if f.ints > ints or f.floats > floats:
            out.append({"address": f"{a:#x}", "file": lifted_file(a), "lift_ints": ints, "lift_floats": floats,
                        "reads_ints": f.ints, "reads_floats": f.floats, "boot": a in reach})
    return out


def lifted_file(a: int) -> str:
    return f"src/lifted/lifted_{(a - gen_lift.BASE) >> gen_lift.RANGE_BITS:03x}.cpp"


# --- compiling ---------------------------------------------------------------------------------------

def compile_attempts(lifted: list[dict], work: Path) -> list[tuple[Path | None, set[int]]]:
    """Each attempt's definitions in one AArch64 scratch TU (gen_lift.build_all's loop: functions
    with errors are dropped and the rest compiled again). Returns (object, compiled addresses)."""
    work.mkdir(parents=True, exist_ok=True)

    def build(k: int) -> tuple[Path | None, set[int]]:
        defs = dict(lifted[k])
        if not defs:
            return None, set()
        src, obj = work / f"attempt_{k}.cpp", work / f"attempt_{k}.o"
        for _ in range(6):
            decls = sorted({d for _, ds in defs.values() for d in ds})
            lines = gen_lift.HEADER + decls
            owner = {}
            for a in sorted(defs):
                owner[len(lines) + 1] = a
                lines += defs[a][0].split("\n")
            src.write_text("\n".join(lines) + "\n", encoding="utf-8")
            r = subprocess.run(buildcfg.cxx_command(src, obj, ["-ferror-limit=0", *SEMANTIC_FLAGS]),
                               capture_output=True, text=True)
            if r.returncode == 0:
                return obj, set(defs)
            starts = sorted(owner)
            bad = set()
            for m in re.finditer(re.escape(src.name) + r":(\d+):\d+: error", r.stderr):
                i = bisect.bisect_right(starts, int(m.group(1))) - 1
                if i >= 0:
                    bad.add(owner[starts[i]])
            if not bad:
                return None, set()
            defs = {a: d for a, d in defs.items() if a not in bad}
            if not defs:
                return None, set()
        return None, set()

    with ThreadPoolExecutor() as pool:
        return list(pool.map(build, range(len(lifted))))


def object_symbols(objs: list[Path]) -> dict[str, str]:
    """Demangled names of every symbol of the objects (llvm-nm, then one llvm-cxxfilt call)."""
    nm = port_sweep.find_nm(port_sweep.host_cxx())
    names = set()
    for o in objs:
        r = subprocess.run([nm, str(o)], capture_output=True, text=True, encoding="utf-8", errors="replace")
        for line in r.stdout.splitlines():
            parts = line.split()
            if parts:
                names.add(parts[-1])
    return asmdiff.demangle(sorted(names), strip=False)


# --- P-code ------------------------------------------------------------------------------------------

def pcode_db(words: set[int], cache: Path) -> dict:
    """P-code of every word (tools/ghidra/pcode.py on the ones the cache lacks)."""
    db = {"ops": {}, "userops": {}, "registers": {}, "opcodes": {}}
    if cache.is_file():
        with cache.open("rb") as fh:
            db = pickle.load(fh)
    todo = sorted(w for w in words if w not in db["ops"] and not pcode_emu.native(w))
    if todo:
        cache.parent.mkdir(parents=True, exist_ok=True)
        # (per process: two runs may share the cache, and the last to finish keeps its words)
        req, res = cache.with_suffix(f".{os.getpid()}.req.txt"), cache.with_suffix(f".{os.getpid()}.new.pickle")
        req.write_text("".join(f"0 {w:08x}\n" for w in todo), encoding="utf-8")
        t = time.time()
        r = subprocess.run([sys.executable, str(ROOT / "tools" / "ghidra" / "pcode.py"), str(req), str(res)],
                           capture_output=True, text=True, encoding="utf-8", errors="replace")
        if r.returncode != 0:
            raise SystemExit(f"tools/ghidra/pcode.py failed:\n{r.stderr[-3000:]}")
        with res.open("rb") as fh:
            new = pickle.load(fh)
        db["ops"].update({w: ops for (_, w), ops in new["ops"].items()})
        for k in ("userops", "registers", "opcodes"):
            db[k] = new[k]
        tmp = cache.with_suffix(f".{os.getpid()}.tmp")
        with tmp.open("wb") as fh:
            pickle.dump(db, fh)
        os.replace(tmp, cache)
        req.unlink()
        res.unlink()
        print(f"P-code: {len(todo)} new instruction words from Ghidra in {time.time() - t:.0f} s"
              f" ({len(db['ops'])} cached in {port_sweep.rel(cache)})")
    pcode_emu.set_opcodes(db["opcodes"])
    r = db["registers"]
    e = pcode_emu
    if r and (r["x0"][0], r["q0"][0], r["q1"][0], r["sp"][0], r["NG"][0]) != (e.X0, e.Q0, e.Q0 + e.QSTRIDE, e.SP, e.NG):
        raise SystemExit(f"Ghidra's AArch64 register layout differs from tools/pcode_emu.py's constants: {r}")
    return db


# --- the oracle --------------------------------------------------------------------------------------

class Oracle:
    def __init__(self, orig: asmdiff.Original, facts: abi_facts.AbiFacts, db: dict, inputs: int):
        self.orig, self.af, self.db, self.inputs = orig, facts, db, inputs
        self.image = pcode_emu.ImageMemory(orig.img)
        self.preset_orig = {}
        for slot in gen_lift.GUARDS:  # the guards the lifter turns into lift_guard helpers: taken
            p = orig.img.pointer(slot - pcode_emu.BASE)
            if p is not None:
                self.preset_orig[pcode_emu.BASE + p] = 1
        self.orig_runs: dict[int, list] = {}
        self.dicts: dict[int, tuple] = {}

    def image_word(self, a: int) -> int:
        off = a - pcode_emu.BASE
        return int.from_bytes(self.image.data[off:off + 4], "little")

    def machine(self, seed, inside, word, lift, preset, dictionary=()):
        m = pcode_emu.Machine(self.db, self.image, seed, inside, word, lift, self.af.import_name,
                              self.af.facts, preset, dictionary)
        m.setup()
        return m

    def originals(self, addr: int) -> list:
        """The original's runs the lift is compared on: (seed, machine or why it was inconclusive).
        The first `inputs` seeds always; then more, while one of the last PATIENCE seeds reached an
        instruction none before it did (up to MAX_SEEDS): only those are kept, so the comparison
        covers every path the inputs found at the cost of a few runs."""
        if addr not in self.orig_runs:
            size = self.orig.sizes[addr]
            self.dicts[addr] = dictionary(self.orig.function(addr))
            runs, seen, idle = [], set(), 0
            for seed in range(MAX_SEEDS):
                if seed >= self.inputs and (idle >= PATIENCE or len(seen) * 4 >= size):
                    break
                m = self.machine(seed, lambda a: addr <= a < addr + size, self.image_word, None, self.preset_orig,
                                 self.dicts[addr])
                try:
                    m.run(addr)
                except pcode_emu.Inconclusive as e:
                    if seed < self.inputs:
                        runs.append((seed, str(e)))
                    idle += 1
                    continue
                new = m.visited - seen
                if seed < self.inputs or new:
                    runs.append((seed, m))
                    seen |= m.visited
                    idle = 0
                else:
                    idle += 1
            self.orig_runs[addr] = runs
        return self.orig_runs[addr]

    def check(self, addr: int, lo: pcode_emu.LiftObject, ret: str | None) -> tuple[str, str, int, set]:
        """('pass' | 'fail' | 'inconclusive', detail, conclusive runs, original addresses covered)."""
        start, size = lo.functions[addr]
        bad = [n for p, ns in lo.unresolved.items() if start <= p < start + size for n in ns]
        if bad:
            return "inconclusive", f"unresolved {bad[0]}", 0, set()
        text = lo.text
        word = lambda a: pcode_emu._word(lo, a)  # noqa: E731
        preset = {**self.preset_orig, **{g: 1 for g in lo.guards}}
        agreed, why, covered = 0, "", set()
        for seed, mo in self.originals(addr):
            if isinstance(mo, str):
                why = why or f"original: {mo}"
                continue
            covered |= mo.visited
            ml = self.machine(seed, lambda a: text[0] <= a < text[1], word, lo, preset, self.dicts[addr])
            try:
                ml.run(start)
            except pcode_emu.Inconclusive as e:
                why = why or f"lift: {e}"
                continue
            d = pcode_emu.compare(mo, ml, ret)
            if d == "skip":
                why = why or "original faulted where the lift did not"
                continue
            if d:
                self.last = (mo, ml)
                return "fail", f"input {seed}: {d}", agreed, covered
            agreed += 1
        if not agreed:
            return "inconclusive", why or "no input", 0, covered
        return "pass", why, agreed, covered


_CTX: dict = {}


def _init_worker(version: str, lo_paths: list, db_path: Path, inputs: int, debug: bool) -> None:
    """An oracle process: the image, the P-code, the facts cache; objects load on first use."""
    orig = asmdiff.Original(version)
    af = abi_facts.AbiFacts(orig, callgraph.load_rows())
    af.version = version
    af.load(FACTS)
    with db_path.open("rb") as fh:
        db = pickle.load(fh)
    pcode_emu.set_opcodes(db["opcodes"])
    _CTX.update(oracle=Oracle(orig, af, db, inputs), lo_paths=lo_paths, los={}, debug=debug)


def _lift_object(k: int) -> pcode_emu.LiftObject:
    los = _CTX["los"]
    if k not in los:
        with _CTX["lo_paths"][k].open("rb") as fh:
            los[k] = pickle.load(fh)
    return los[k]


def _check(job: tuple[int, list]) -> tuple[int, int | None, dict, str, str]:
    """One function's attempts in order until one passes: (address, accepted attempt or None,
    its info, the verdict that rejected it, debug text)."""
    a, attempts = job
    oracle = _CTX["oracle"]
    fail, debug = "", []
    for k, ret in attempts:
        try:
            verdict, detail, runs, covered = oracle.check(a, _lift_object(k), ret)
        except Exception as e:  # an emulator gap: count it, keep going
            verdict, detail, runs, covered = "inconclusive", f"emulator: {type(e).__name__}: {e}", 0, set()
        if _CTX["debug"]:
            debug.append(f"--- {a:x} attempt {k}: {verdict} {detail} (result compared: {ret})")
            if verdict == "fail":
                for name, m in zip(("original", "lift"), oracle.last):
                    debug.append(f"  {name}: {m.steps} steps, x0={m.x(0):#x}")
                    debug += [f"    {ev}  [{len(mem)} bytes written]" for ev, mem in m.events]
        if verdict == "pass":
            size = oracle.orig.sizes[a]
            return a, k, {"attempt": k, "oracle_inputs": runs, "coverage": len(covered) * 4 / max(size, 1),
                          "note": detail}, "", "\n".join(debug)
        if not fail or verdict == "fail":
            fail = f"{verdict}: {detail}"
    oracle.orig_runs.pop(a, None)
    return a, None, {}, fail, "\n".join(debug)


def dictionary(code: bytes) -> tuple:
    """Constants of a function: compare and move immediates (and their neighbours), for the
    oracle's dictionary inputs."""
    vals = set()
    for x in gen_lift.CS.disasm(code, 0):
        if x.mnemonic in ("cmp", "cmn", "ccmp", "ccmn", "mov", "movz", "movk", "tst", "subs", "adds"):
            for op in x.operands:
                if op.type == gen_lift.arm64.ARM64_OP_IMM and 0 <= op.imm < 1 << 32:
                    vals |= {op.imm, op.imm + 1, max(op.imm - 1, 0)}
    return tuple(sorted(vals))


def ret_kind(definition: str, used: bool | None) -> str | None:
    rtype = definition.split(" fn_", 1)[0].strip()
    if used is False:
        return None
    return RET_KIND.get(rtype, "x64")


# --- the oracle's own test ---------------------------------------------------------------------------

# One-point faults a lifter could make: an offset, a comparison, an access width, a constant.
MUTATIONS = [
    (re.compile(r"\) \+ (\d+)\)"), lambda m: f") + {int(m.group(1)) + 8})"),
    (re.compile(r" == "), lambda m: " != "),
    (re.compile(r" != "), lambda m: " == "),
    (re.compile(r" < "), lambda m: " <= "),
    (re.compile(r" > "), lambda m: " >= "),
    (re.compile(r"\*\(u32\*\)"), lambda m: "*(u16*)"),
    (re.compile(r"\*\(u64\*\)"), lambda m: "*(u32*)"),
    (re.compile(r"= (0x[0-9a-f]+|\d+);"), lambda m: f"= {int(m.group(1), 0) + 1};"),
]


def mutate(definition: str, rnd) -> str | None:
    """`definition` with one fault in its body (never in the signature line), or None."""
    head, _, body = definition.partition("\n")
    def line_of(m) -> str:
        return body[body.rfind("\n", 0, m.start()) + 1:body.find("\n", m.end())]

    # (a flat-mode local's zero initialiser is never read before a write, and most flag locals
    # are never read at all: changes there are equivalent)
    sites = [(rx, f, m) for rx, f in MUTATIONS for m in rx.finditer(body)
             if not re.match(r"\s*(u64 X\d+ = 0;$|f[NZCV] = )", line_of(m))]
    if not sites:
        return None
    rx, f, m = rnd.choice(sites)
    return head + "\n" + body[:m.start()] + f(m) + body[m.end():]


def mutation_test(n: int, accepted: dict, orig, af, out: Path, inputs: int, resolve, used: dict,
                  info: dict) -> list[str]:
    """Faults injected into n accepted lifts: how many the oracle rejects (killed), accepts
    (survived: an equivalent change, or a path the inputs never ran), or cannot decide."""
    import random
    rnd = random.Random(1)
    mutants = {}
    for a in sorted(rnd.sample(sorted(accepted), min(n, len(accepted)))):
        d = mutate(accepted[a][0], rnd)
        if d:
            mutants[a] = (d, accepted[a][1])
    work = out.parent / (out.name + "_work") / "mutants"
    obj, comp = compile_attempts([mutants], work)[0]
    if obj is None:
        return ["mutants: none compiled"]
    demangled = object_symbols([obj])
    lo = pcode_emu.load_object(obj, resolve, demangled)
    words = set(lo.words)
    for a in comp:
        code = orig.function(a)
        words |= {int.from_bytes(code[i:i + 4], "little") for i in range(0, len(code), 4)}
    db = pcode_db(words, out.parent / "pcode_cache.pickle")
    oracle = Oracle(orig, af, db, inputs)
    tally: Counter = Counter()
    full: Counter = Counter()  # on functions whose every instruction the inputs ran
    survivors = []
    for a in sorted(comp):
        try:
            v, d, _, _ = oracle.check(a, lo, ret_kind(mutants[a][0], used.get(a)))
        except Exception:
            v = "inconclusive"
        kind = {"fail": "killed", "pass": "survived"}.get(v, "inconclusive")
        tally[kind] += 1
        if info.get(a, {}).get("coverage", 0) >= 0.95:
            full[kind] += 1
        if v == "pass":
            survivors.append(f"{a:x}")
    total = sum(tally.values())
    return [f"mutation test: {total} one-point faults in accepted lifts: {tally['killed']} rejected by the oracle"
            f" ({tally['killed'] / max(total, 1):.0%}), {tally['survived']} accepted, {tally['inconclusive']} inconclusive",
            f"  on functions the inputs ran completely: {full['killed']} of {sum(full.values())} rejected"
            f" ({full['killed'] / max(sum(full.values()), 1):.0%})",
            f"  survivors (equivalent changes or paths the inputs never ran): {' '.join(survivors[:20])}"]


# --- x86-64 ------------------------------------------------------------------------------------------

def host_flags() -> tuple[str, list[str]]:
    cxx = port_sweep.host_cxx()
    target = port_sweep.default_target(cxx)
    flags = [f"--target={target}", *port_sweep.HOST_FLAGS, "-w", *SEMANTIC_FLAGS, *buildcfg.system_includes(),
             *port_sweep.project_includes()]
    return cxx, flags


def write_host(out: Path, accepted: dict[int, tuple[str, set[str]]], info: dict[int, dict]) -> dict[int, str]:
    """One source and one x86-64 object per accepted lift. Returns {address: error} for the ones
    that do not compile."""
    src_dir, obj_dir = out / "src", out / "obj"
    for d in (src_dir, obj_dir):
        d.mkdir(parents=True, exist_ok=True)
        for f in d.iterdir():
            f.unlink()
    cxx, flags = host_flags()

    def one(a: int) -> tuple[int, str]:
        definition, decls = accepted[a]
        i = info[a]
        head = NOTE + [f"// fn_{a:x}: {i['size']} bytes, {i['oracle_inputs']} inputs agreed, "
                       f"{i['coverage']:.0%} of the original's instructions executed."]
        src = src_dir / f"fn_{a:x}.cpp"
        first, _, body = definition.partition("\n")
        traced = f"{first}\n    DREAD_PORT_LIFTED({a:#x});\n{body}"  # oracle (c): which lifts a boot runs
        src.write_text("\n".join(head + gen_lift.HEADER + TRACE_HOOK + sorted(decls) + [traced, ""]),
                       encoding="utf-8", newline="\n")
        r = subprocess.run([cxx, *flags, str(src), "-o", str(obj_dir / f"fn_{a:x}.o")], capture_output=True,
                           text=True, encoding="utf-8", errors="replace")
        return a, ("" if r.returncode == 0 else (r.stderr.strip().splitlines() or ["?"])[0][-300:])

    with ThreadPoolExecutor() as pool:
        return {a: e for a, e in pool.map(one, sorted(accepted)) if e}


# --- main --------------------------------------------------------------------------------------------

BANDS = gen_lift.BANDS


def table(sizes: dict[int, int], outcome: dict[int, str]) -> list[str]:
    rows: dict[str, list[int]] = {}
    for a, o in outcome.items():
        rows.setdefault(o, []).append(a)

    def cells(group):
        out = []
        for _, lo, hi in BANDS + [("total", 0, 1 << 62)]:
            s = [sizes[a] for a in group if lo <= sizes[a] < hi]
            out.append(f"{len(s)}/{sum(s) / 1024:.0f}K" if s else "-")
        return out

    first = ["accepted"]
    order = sorted(rows, key=lambda r: (r not in first, -sum(sizes[a] for a in rows[r]), r))
    t = [["outcome"] + [b[0] for b in BANDS] + ["total"]] + [[r] + cells(rows[r]) for r in order] \
        + [["all"] + cells(list(outcome))]
    w = [max(len(row[i]) for row in t) for i in range(len(t[0]))]
    return ["  ".join(row[0].ljust(w[0]) if i == 0 else c.rjust(w[i]) for i, c in enumerate(row)) for row in t]


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--worklist", default=str(WORKLIST), help="CSV with address and status columns (default: %(default)s)")
    ap.add_argument("--only", help="ADDR[,ADDR...] or @file instead of the worklist")
    ap.add_argument("--top", type=int, help="the first N worklist entries")
    ap.add_argument("--out", help=f"output directory (default: {port_sweep.rel(DEFAULT_OUT)}; with --only, --top or "
                                  f"--debug {port_sweep.rel(PARTIAL_OUT)}, so a partial run never replaces the "
                                  f"set tools/port_link.py links)")
    ap.add_argument("--inputs", type=int, default=INPUTS, help="oracle seeds per function (default %(default)s)")
    ap.add_argument("--jobs", type=int, default=12, help="oracle processes (default: up to %(default)s)")
    ap.add_argument("--mutants", type=int, default=0,
                    help="test the oracle: inject one fault into N accepted lifts and count how many it rejects")
    ap.add_argument("-v", "--verbose", action="store_true", help="print each function's outcome")
    ap.add_argument("--debug", help="ADDR[,ADDR...]: lift only these and print every attempt, and the events of "
                                    "the first disagreeing input")
    ap.add_argument("--passthrough", action="store_true",
                    help=f"only list the src/lifted definitions that lose arguments on x86-64 -> {port_sweep.rel(PASSTHROUGH)}")
    ap.add_argument("--no-overrides", action="store_true",
                    help="leave those definitions alone (by default each gets a semantic lift that overrides it, "
                         "added to the worklist unless --only or --top is given)")
    args = ap.parse_args()
    sys.stdout.reconfigure(line_buffering=True)  # progress lines appear as they happen in a log file
    if args.debug:
        args.only = args.debug
    t0 = time.time()
    out = Path(args.out) if args.out else PARTIAL_OUT if (args.only or args.top) else DEFAULT_OUT
    orig = asmdiff.Original(args.version)
    rows = callgraph.load_rows()
    af = abi_facts.AbiFacts(orig, rows)
    af.version = args.version
    af.load(FACTS)
    overrides: dict[int, str] = {}  # address -> the src/lifted file whose definition a semantic lift replaces
    if args.passthrough or not args.no_overrides:
        pt = passthrough(af, boot_reach(args.version, rows))
        PASSTHROUGH.write_text(json.dumps(pt, indent=1) + "\n", encoding="utf-8")
        print(f"passthrough: {len(pt)} src/lifted definitions read more argument registers than they declare "
              f"({sum(e['boot'] for e in pt)} on the boot path) -> {port_sweep.rel(PASSTHROUGH)}")
        if args.passthrough:
            af.save(FACTS)
            return 0
        overrides = {int(e["address"], 16): e["file"] for e in pt}
    wl = gen_lift.parse_only(args.only) if args.only else worklist(Path(args.worklist))
    if args.top:
        wl = wl[:args.top]
    elif not args.only:
        listed = set(wl)
        wl += [a for a in sorted(overrides) if a not in listed]
    skip = gen_lift.defined_outside(Path("/nonexistent"))  # every definition in src/, src/lifted included
    hand = gen_lift.defined_outside(gen_lift.OUT)  # ... and outside src/lifted: never overridden
    outcome: dict[int, str] = {}
    todo = []
    for a in wl:
        if a in skip and not (a in overrides and a not in hand):
            outcome[a] = "skipped: defined in src/"
        elif orig.sizes.get(a, gen_lift.MAX_SIZE + 1) > gen_lift.MAX_SIZE:
            outcome[a] = "rejected: size cap"
        else:
            todo.append(a)
    overrides = {a: f for a, f in overrides.items() if a in skip and a not in hand and a in set(todo)}
    sizes = {a: orig.sizes.get(a, 0) for a in wl}
    print(f"port_lift: {len(wl)} functions in the worklist, {len(todo)} to lift"
          + (f" ({len(overrides)} of them overriding their src/lifted copy)" if overrides else ""))

    # 1-2. lift
    gen_lift.Lifter.facts = af.facts
    why: dict[int, tuple[int, str]] = {}
    lifted = gen_lift.lift_serial(orig, todo, why, OPTS)
    lifted_any = set().union(*lifted) if lifted else set()
    print(f"lifted: {len(lifted_any)} of {len(todo)} in at least one variant ({time.time() - t0:.0f} s)")

    # 3. compile for AArch64
    work = out.parent / (out.name + "_work")
    built = compile_attempts(lifted, work / "aarch64")
    compiled_any = set().union(*[c for _, c in built])
    print(f"compiled (AArch64): {len(compiled_any)} ({time.time() - t0:.0f} s)")
    objs = [o for o, _ in built if o]
    demangled = object_symbols(objs)
    import_by_name = {}
    for a, r in rows.items():
        if r["name"].startswith(callgraph.IMPORT_PREFIX):
            import_by_name.setdefault(r["name"][len(callgraph.IMPORT_PREFIX):], a)

    def resolve(name: str) -> int | None:
        m = asmdiff.TARGET_RE.match(asmdiff.strip_params(name))
        if m:
            return int(m.group(1), 16)
        return import_by_name.get(name.split("(")[0])

    lo_paths: list[Path | None] = []
    words = set()
    for k, (o, _) in enumerate(built):
        if o is None:
            lo_paths.append(None)
            continue
        lo = pcode_emu.load_object(o, resolve, demangled)
        words |= lo.words
        p = work / "aarch64" / f"attempt_{k}.lo.pickle"
        with p.open("wb") as fh:
            pickle.dump(lo, fh)
        lo_paths.append(p)
    print(f"laid out {len([p for p in lo_paths if p])} objects ({time.time() - t0:.0f} s)")

    # 4. P-code and the oracle
    for a in compiled_any:
        code = orig.function(a)
        words |= {int.from_bytes(code[i:i + 4], "little") for i in range(0, len(code), 4)}
    db = pcode_db(words, out.parent / "pcode_cache.pickle")
    db_path = work / "pcode.pickle"  # this run's own copy for the workers (another run may replace the cache)
    with db_path.open("wb") as fh:
        pickle.dump(db, fh)
    jobs = []
    for a in sorted(compiled_any):
        used = af.ret_used(a)  # here, so that the workers find it in the cache
        jobs.append((a, [(k, ret_kind(lifted[k][a][0], used)) for k, (_, comp) in enumerate(built) if a in comp]))
    af.save(FACTS)
    print(f"facts: callers' use of {len(jobs)} results ({time.time() - t0:.0f} s)")
    ctx = (args.version, lo_paths, db_path, args.inputs, bool(args.debug))
    accepted: dict[int, tuple[str, set[str]]] = {}
    info: dict[int, dict] = {}
    last_fail: dict[int, str] = {}
    nproc = 1 if args.debug or len(jobs) < 16 else min(os.cpu_count() or 1, args.jobs)
    if nproc == 1:
        _init_worker(*ctx)
        results = map(_check, jobs)
        pool = None
    else:
        import multiprocessing
        pool = multiprocessing.get_context("spawn").Pool(nproc, _init_worker, ctx)
        results = pool.imap_unordered(_check, jobs, chunksize=4)
    for n, (a, k, inf, fail, debug) in enumerate(results):
        if debug:
            print(debug)
        if k is not None:
            accepted[a] = lifted[k][a]
            info[a] = {"size": sizes[a], **inf}
        elif fail:
            last_fail[a] = fail
        if (n + 1) % 100 == 0:
            print(f"  oracle: {n + 1}/{len(jobs)} functions, {len(accepted)} accepted ({time.time() - t0:.0f} s)")
    if pool:
        pool.close()
        pool.join()
    print(f"oracle: {len(accepted)} accepted of {len(compiled_any)} compiled ({time.time() - t0:.0f} s)")

    if args.mutants:
        used = {a: af.ret_used(a) for a in accepted}
        for line in mutation_test(args.mutants, accepted, orig, af, out, args.inputs, resolve, used, info):
            print(line)

    # 5. x86-64
    errors = write_host(out, accepted, info)
    for a in errors:
        del accepted[a]
    for a in todo:
        if a in accepted:
            outcome[a] = "accepted"
        elif a in errors:
            outcome[a] = "rejected: x86-64 compile error"
        elif a in last_fail:
            v = last_fail[a].split(":", 1)[0]
            outcome[a] = "rejected by the oracle" if v == "fail" else "inconclusive"
        elif a in lifted_any:
            outcome[a] = "rejected: compile error (AArch64)"
        else:
            r = why.get(a, (0, "?"))[1]
            outcome[a] = f"rejected: {r}"
    report = table(sizes, outcome)
    print("\n".join(report))
    reasons = Counter(re.sub(r"input \d+: ", "", re.sub(r"0x[0-9a-f]+|\d+", "N", last_fail[a]))
                      for a in todo if a in last_fail and a not in accepted)
    print("oracle verdicts on rejected functions (first difference, numbers elided):")
    for r, c in reasons.most_common(25):
        print(f"  {c:5d}  {r}")
    if args.verbose:
        for a in wl:
            print(f"  {a:x} {sizes[a]:6d}  {outcome[a]}" + (f"  [{last_fail[a]}]" if a in last_fail and a not in accepted else ""))
    manifest = {
        "generator": GENERATOR, "sources": {s: blob(s) for s in SOURCES}, "version": args.version,
        "build_id": "646761F643AFEBB379EDD5E6A5151AF2CEF93DC1", "inputs": args.inputs,
        "semantic_flags": SEMANTIC_FLAGS, "host_flags": host_flags()[1],
        "worklist": args.only or port_sweep.rel(Path(args.worklist)), "seconds": round(time.time() - t0),
        "functions": {f"{a:x}": {"outcome": outcome[a], "size": sizes[a], **info.get(a, {}),
                                 **({"detail": last_fail[a]} if a in last_fail and a not in accepted else {}),
                                 **({"error": errors[a]} if a in errors else {}),
                                 **({"overrides": overrides[a]} if a in overrides else {})} for a in wl},
        "report": report,
    }
    (out / "manifest.json").write_text(json.dumps(manifest, indent=1) + "\n", encoding="utf-8")
    if overrides:
        print(f"overrides: {len([a for a in overrides if a in accepted])} of {len(overrides)} src/lifted definitions "
              f"that lose arguments on x86-64 have an accepted semantic lift")
    print(f"{len(accepted)} semantic lifts in {port_sweep.rel(out)} (sources in src/, x86-64 objects in obj/,"
          f" manifest.json); {time.time() - t0:.0f} s")
    return 0


if __name__ == "__main__":
    sys.exit(main())
