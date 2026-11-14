#!/usr/bin/env python3
"""Reference matches for a worker: the functions that already match, hand-written, and are of most use to
someone about to write this one. prepare.py puts them at the end of each function's context with their source.

tools/nearcopy.py finds copies (the same instructions at the same places); this finds neighbours, of two kinds:

  alike   its instructions line up with this function's. Registers and constants are taken out of both, and
          the measure is the share of the two sequences that pairs up in order (`lined`, 0 to 1). It shows how
          this compiler's output was written as source.
  names   it names the callees and globals this function names (`named`: the share of this function's, a
          rare one weighing more than one everything calls). It shows their declarations and the types
          passed to them, however different its code.

The first reference is the most alike, the second the one that names the most, the rest the next most alike;
where one kind has none the other fills in. Candidates for `alike` are shortlisted by the features two
functions share (single instructions, runs of three, callees and globals; a feature weighs more the fewer
functions have it) and then lined up one by one.

A reference is our own source: a function whose status is O, defined under src/ outside the generated
directories, in a file with none of the forms worker.md rules out (a worker copies what it is shown).

  similar.py ADDR [ADDR...] [--refs 3]     print the references of these functions (needs the dump)
"""
import difflib
import math
import re
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
import fakematch  # noqa: E402
import nearcopy  # noqa: E402
from progress import GENERATED_DIRS  # noqa: E402

MIN_LINED = 0.45      # an `alike` reference lines up with at least this share
MIN_NAMED = 0.5       # a `names` reference names at least this share of the callees and globals, and two of them
SIZE_RATIO = 2.0      # an `alike` reference is between half and twice the function's size
NAMES_RATIO = 3.0     # a `names` reference is at most this many times it
SHORTLIST = 12        # candidates lined up per function
COMMON = 0.2          # a feature more than this share of the references have does not propose candidates
WHOLE_FILE = 120      # a reference file up to this many lines is shown whole, a longer one as the function alone
PAIR_BYTES = 800      # the first reference comes with its own disassembly up to this size
MIN_BYTES = 24

_REGS = [(re.compile(r"\b[xw]29\b"), "fp"), (re.compile(r"\bx30\b"), "lr"), (re.compile(r"\bx\d+\b"), "x"),
         (re.compile(r"\bw\d+\b"), "w"), (re.compile(r"\b([bhsdqv])\d+\b"), r"\1")]
_IMM = re.compile(r"#-?(?:0x[0-9a-f]+|\d+(?:\.\d+)?(?:e[+-]?\d+)?)")
_INLINING = re.compile(r"\b(always_inline|flatten|noinline)\b")
_DEF_START = re.compile(r"^[A-Za-z_]")
_NOT_A_DEF = re.compile(r"^(extern|typedef|using|static_assert|struct|class|union|enum|namespace|template|return)\b")
_FN = re.compile(r"\bfn_([0-9a-f]{10})\s*\(")


def tokens(code, addr=0):
    """One string per instruction: the mnemonic and its operands with register numbers and constants removed."""
    from capstone import CS_ARCH_ARM64, CS_MODE_ARM, Cs
    out = []
    for _, _, mnemonic, ops in Cs(CS_ARCH_ARM64, CS_MODE_ARM).disasm_lite(code, addr):
        ops = _IMM.sub("#", ops)
        for rx, to in _REGS:
            ops = rx.sub(to, ops)
        out.append(f"{mnemonic} {ops}".strip())
    return out


def features(toks, refs=()):
    """The set a function is compared by: single instructions, runs of three, the callees and globals named."""
    out = {"1 " + t for t in toks}
    out.update("3 " + " | ".join(toks[i:i + 3]) for i in range(len(toks) - 2))
    out.update(f"ref {r:x}" for r in refs)
    return out


def definitions(root=ROOT):
    """address -> (file relative to the repository, line) of each `fn_<addr>` defined in a hand-written source.
    The first definition in path order wins."""
    found = {}
    for p in sorted((root / "src").rglob("*.cpp")):
        rel = p.relative_to(root).as_posix()
        if rel.split("/")[1] in GENERATED_DIRS:
            continue
        lines = p.read_text(encoding="utf-8", errors="replace").splitlines()
        for i, ln in enumerate(lines):
            if not _DEF_START.match(ln) or _NOT_A_DEF.match(ln):
                continue
            m = _FN.search(ln.split("//")[0])
            if not m:
                continue
            # a definition reaches `{` before `;`; a declaration ends in `;`
            rest = " ".join(x.split("//")[0] for x in lines[i:i + 8])
            rest = rest[rest.index(m.group(0)):]
            brace, semi = rest.find("{"), rest.find(";")
            if brace >= 0 and (semi < 0 or brace < semi):
                found.setdefault(int(m.group(1), 16), (rel, i + 1))
    return found


def readable(rel, text):
    """False for a file with a form worker.md rules out: it is not shown to a worker as an example."""
    return not _INLINING.search(text) and not fakematch.scan_text(rel, text)


class Index:
    """The references and their features. `code(address, size)` gives a function's bytes."""

    def __init__(self, funcs, defs, code, root=ROOT):
        self.funcs, self.defs, self.code, self.root = funcs, defs, code, root
        self.toks, self.refs, self.feats, self.posting = {}, {}, {}, defaultdict(list)
        for a in sorted(defs):
            size, _, status = funcs.get(a, (0, "", ""))
            if status != "O" or size < MIN_BYTES:
                continue
            b = code(a, size)
            self.toks[a] = t = tokens(b, a)
            self.refs[a] = nearcopy.refs(a, b)
            self.feats[a] = f = features(t, self.refs[a])
            for x in f:
                self.posting[x].append(a)
        n = max(len(self.feats), 1)
        self.weight = {x: math.log(1 + n / len(v)) for x, v in self.posting.items()}
        self.unseen = math.log(1 + n)   # a feature no reference has
        self.total = {a: sum(self.weight[x] for x in f) for a, f in self.feats.items()}
        self.common = max(COMMON * n, 20)
        self._readable = {}

    def _clean(self, a):
        rel = self.defs[a][0]
        if rel not in self._readable:
            self._readable[rel] = readable(rel, (self.root / rel).read_text(encoding="utf-8", errors="replace"))
        return self._readable[rel]

    def _candidates(self, feats):
        """reference -> the weight of the features it shares, over those that are not common to most."""
        shared = defaultdict(float)
        for x in feats:
            post = self.posting.get(x, ())
            if len(post) <= self.common:
                for a in post:
                    shared[a] += self.weight[x]
        return shared

    def nearest(self, addr, size, k=3):
        """Up to k references for the function at addr: dicts with addr, size, file, line, lined, named, shared
        (the callees and globals both name), of (how many this function names) and same (the share of
        instructions equal place for place, for a reference of the same length)."""
        b = self.code(addr, size)
        toks = tokens(b, addr)
        refs = set(nearcopy.refs(addr, b))
        mine = features(toks, refs)
        mine_total = sum(self.weight.get(x, self.unseen) for x in mine)
        ref_weight = {r: self.weight.get(f"ref {r:x}", self.unseen) for r in refs}
        usable = {a: w for a, w in self._candidates(mine).items()
                  if a != addr and self.funcs[a][0] <= size * NAMES_RATIO}

        def named(a):
            both = refs & set(self.refs[a])
            return (sum(ref_weight[r] for r in both) / sum(ref_weight.values()) if refs else 0.0), sorted(both)

        def lined(a):
            return difflib.SequenceMatcher(None, toks, self.toks[a], autojunk=False).ratio()

        def entry(a, how):
            share, both = named(a)
            same = None
            if len(self.toks[a]) == len(toks):
                same = sum(x == y for x, y in zip(self.toks[a], toks)) / max(len(toks), 1)
            return {"addr": a, "size": self.funcs[a][0], "kind": how, "lined": round(lined(a), 2),
                    "named": round(share, 2), "shared": both, "of": len(refs), "same": same,
                    "file": self.defs[a][0], "line": self.defs[a][1]}

        # alike: shortlist by shared features over all features, then line each one up
        short = sorted(((w / (mine_total + self.total[a] - w), -abs(self.funcs[a][0] - size), -a)
                        for a, w in usable.items() if size / SIZE_RATIO <= self.funcs[a][0] <= size * SIZE_RATIO),
                       reverse=True)
        alike, seen = [], []
        for _, _, neg in short:
            a = -neg
            if len(alike) >= SHORTLIST:
                break
            if self.toks[a] in seen or not self._clean(a):   # a second copy of a reference adds nothing
                continue
            seen.append(self.toks[a])
            alike.append((lined(a), a))
        alike = [a for score, a in sorted(alike, key=lambda t: (-t[0], t[1])) if score >= MIN_LINED]
        # names: the callees and globals this function names
        names = []
        for a in usable:
            share, both = named(a)
            if share >= MIN_NAMED and len(both) >= min(2, len(refs)):
                names.append((share, len(both), a))
        names = [a for _, _, a in sorted(names, key=lambda t: (-t[0], -t[1], abs(self.funcs[t[2]][0] - size), t[2]))]
        names = next(([a] for a in names if a not in alike[:1] and self._clean(a)), [])

        picked = [(a, "alike") for a in alike[:1]] + [(a, "names") for a in names]
        picked += [(a, "alike") for a in alike[1:] if a not in names]
        return [entry(a, how) for a, how in picked[:k]]


def excerpt(lines, line):
    """The function defined at `line` (1-based) with the comment above it, to its closing brace."""
    start = line - 1
    while start > 0 and lines[start - 1].lstrip().startswith("//"):
        start -= 1
    end = next((i for i in range(line - 1, len(lines)) if lines[i].startswith("}")), len(lines) - 1)
    return lines[start:end + 1]


def render(refs, name=lambda a: f"fn_{a:x}", unit_of=lambda a: None, unit=None, listing=None, root=ROOT):
    """The section of a context file that shows these references. `listing(addr, size)` is the annotated
    disassembly of a reference; it is given for the first `alike` one only."""
    if not refs:
        return ["No function that already matches is close to this one: there is no reference."]
    out = ["Reference matches: functions that already match and are closest to this one. Their source is ours:",
           "read it for how this compiler's output was written as source, and for the types and declarations of",
           "what both functions use. It is a guide, not a template: yours does what YOUR disassembly does."]
    paired = False
    for n, r in enumerate(refs, 1):
        what = [f"{r['size']} bytes", f"{r['lined']:.0%} of its instructions line up with yours"]
        if r.get("same") is not None and r["same"] >= 0.9:
            what.append(f"{r['same']:.0%} equal place for place")
        if r["shared"]:
            what.append(f"names {len(r['shared'])} of your {r['of']} callees and globals")
        if unit is not None and unit_of(r["addr"]) == unit:
            what.append("same unit")
        out += ["", f"[{n}] fn_{r['addr']:x}, {'alike in code' if r['kind'] == 'alike' else 'names what you name'} "
                    f"({', '.join(what)}): {r['file']} line {r['line']}"]
        if r["shared"]:
            out.append("    both name: " + ", ".join(name(a) for a in r["shared"][:8])
                       + (f" and {len(r['shared']) - 8} more" if len(r["shared"]) > 8 else ""))
        lines = (root / r["file"]).read_text(encoding="utf-8", errors="replace").splitlines()
        if len(lines) > WHOLE_FILE:
            lines = excerpt(lines, r["line"])
            out.append("    (the function alone; its includes and types are in the file)")
        out += ["    --- source ---", *lines]
        if r["kind"] == "alike" and not paired and listing and r["size"] <= PAIR_BYTES:
            paired = True
            out += ["    --- the original's disassembly of that source (read only) ---", listing(r["addr"], r["size"])]
    return out


def main():
    import argparse
    import context  # this folder; needs the dump
    import tu_map
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("addr", nargs="+")
    ap.add_argument("--refs", type=int, default=3)
    args = ap.parse_args()
    funcs = context.load_functions()
    binary = context.Binary()
    index = Index(funcs, definitions(), binary.read)
    units = tu_map.load_map()
    starts = tu_map.unit_starts(units)

    def unit_of(a):
        u = tu_map.find_unit(units, a, starts)
        return u and u["index"]

    for x in args.addr:
        a = int(x.replace("fn_", ""), 16)
        refs = index.nearest(a, funcs[a][0], args.refs)
        print(f"== fn_{a:x} ({funcs[a][0]} bytes)")
        print("\n".join(render(refs, lambda t: funcs[t][1] if t in funcs else f"g_unk_{t:x}", unit_of, unit_of(a),
                               lambda ra, rs: binary.disasm(ra, rs, funcs)[0])))
    return 0


if __name__ == "__main__":
    sys.exit(main())
