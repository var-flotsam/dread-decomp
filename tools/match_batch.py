#!/usr/bin/env python3
"""Ask the Claude Message Batches API for candidate rewrites of functions, and keep only what a local compile proves.

Model output is only ever a candidate. Each request asks for K different full replacement texts of one function;
`harvest` splices each into a scratch copy of the function's file, compiles it with the project compiler and
keeps a candidate only when the function's bytes and relocation targets are the original's, every other function
of the file compiles to the code it had, and the text gained no steering form. Two pools:

  clean  functions that already match (O) whose text has a steering form tools/fakematch.py reports (an offset
         cast, `volatile`, an empty block, an empty asm barrier). A candidate is kept when the function still
         matches and fakematch reports no such form in it any more.
  near   drafts of drafts/INDEX.csv that differ from the original in registers or instruction order only (or,
         with --min-similarity, any draft at least that close). A candidate is kept on a byte match; `apply`
         writes it over the draft, and tools/drafts.py promote moves it to src/.

A request carries the project's own C++ (the function's file whole, the headers it includes trimmed to the
declarations the file uses), the rules for readable source, and for the function the differing instructions
(near) or the original's listing and what each barrier prevents (clean): short excerpts of one function, which
stay in build/ and in the request, never in a tracked file.

State lives in build/name_batch/<run>/ beside tools/name_batch.py's runs (run names start with `match-`):
requests.jsonl, meta.json, batch.json, results.jsonl, harvest.csv (one row per candidate: kept or not, why) and
kept/<custom_id>.cpp. The ledger, the submit lock and the budget check are name_batch's own
(build/name_batch/ledger.csv, kinds `match-clean` and `match-near`); `submit` also refuses when the match runs
together would pass --cap. The key comes only from DREAD_NAMES_API_KEY (tools/name_batch.py, make_client).

Usage:
  python tools/match_batch.py --version 2.1.0 build --run match-c1 --pool clean [--limit 30] [--spread] [--k 4] [--dry-run]
  python tools/match_batch.py --version 2.1.0 build --run match-n1 --pool near [--limit 30] [--min-similarity 95] [--dry-run]
  python tools/match_batch.py submit --run R [--budget 180] [--cap 80]   # create the batch (refuses past either)
  python tools/match_batch.py status --run R
  python tools/match_batch.py fetch --run R                              # results.jsonl and a ledger row
  python tools/match_batch.py --version 2.1.0 harvest --run R [--jobs 8] # compile every candidate; harvest.csv
  python tools/match_batch.py --version 2.1.0 apply --run R [--only ID,...]   # write the kept texts (re-checked first)
  python tools/match_batch.py cost                                       # the match runs in the ledger, and the total
Needs the dump for build, harvest and apply; the official anthropic SDK for the commands that call the API.
"""

from __future__ import annotations

import argparse
import csv
import hashlib
import json
import re
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import name_batch as nb  # noqa: E402
from dread.paths import ROOT  # noqa: E402

# The most capable model the Claude API reference lists (shipped with Claude Code 2.1.330, read 2026-11-24);
# --model takes any id tools/name_batch.py has prices for. Thinking is always on for it; a batch request cannot
# set refusal fallbacks, so a refused request comes back with no candidates.
MODEL = "claude-fable-5-2"
EFFORT = "high"
K = 4
DEFAULT_CAP = 80.0           # dollars for all match runs together, inside the shared ledger's budget
KIND = {"clean": "match-clean", "near": "match-near"}
PREFIX = {"clean": "cl", "near": "nm"}

FILE_MAX = 40_000            # a longer source file is not asked about (the request would be mostly other code)
HEADER_WHOLE = 5_000         # a shorter header goes in whole
HEADERS_MAX = 36_000         # characters of headers per request
LISTING_MAX = 220            # instructions of the original shown for a clean-pool function
DIFF_MAX = 160               # rows of a diff excerpt
STEERING = ("offset-cast", "volatile", "empty-block", "barrier")
REGISTERS = "registers or order"

RULES = """\
You rewrite one C++ function of a decompilation project. The project reconstructs a game's source so that each
function compiles to exactly the original machine code and reads as source a game programmer would have written.
Everything you return is only a candidate: each is compiled with the project compiler and compared with the
original byte for byte, and only a candidate that passes is kept. You cannot compile, so return several
genuinely different attempts (different hypotheses about the original source), not variations of spacing.
Deliberate only as far as it takes to form the hypotheses: an answer cut off by the output limit is lost whole,
so write the candidates once you have them instead of simulating the compiler at length.

The compiler is upstream Clang 9.0.1 for AArch64: --target=aarch64-none-elf -mcpu=cortex-a57 -O3 -fPIC
-fno-strict-aliasing -std=c++1z -fno-rtti -fno-exceptions. Code generation follows the source closely in ways you
can use: the type and signedness of a local or a member (a u64 loop index against a u32 one, int against u32, a
bool against a u8), a parameter by value or by reference, the order of declarations and of statements, whether a
value is read once into a local or read again, a comparison written the other way round, an early return against
a nested if, a switch against an if chain, a helper the original inlined (written once, called here), a loop
written with a pointer or with an index.

Names in the sources: fn_<address> is a function nobody has named, g_unk_<address> a global, Unk... a type,
field_0x18 or pad_0x.. a member, vf_0x<offset> a virtual function at that vtable offset. Keep the names that are
there and never rename what exists. Never write an fn_ or g_unk_ name that the file or the headers do not declare.
A type or member you have to add (a view of the part of an object the function reads) is named the same honest
way, because nobody knows its real name: the type Unk<What>_<the suffix the file's own types carry, or the
function's address>, unlike any type in the file or the headers; a member field_0x<offset> with its real type
and an `// 0x<offset>` comment; the bytes before it `u8 pad_0x<offset>[...]`. A descriptive name for a new member
or type (speed, launch_value, TunablesView) is a guess and is not accepted; use a real name only where the file
or the headers show it (an existing type that already has the member is better than a new view). A view is for
reading or writing a member of a real type: do not give a member a made-up size (`u8 field_0x98[8]`) only to
take its address, and do not add a second view of an object whose struct the file already defines, with other
member types. When the real fix is a member's type in a struct outside the function, which you cannot change,
say so in `change` (the struct, the member, the type) so a person can make it.

Readable source (the project's rules):
- Code no developer would write is an optimization not undone yet: look for the inlined helper (written once
  for every caller), the switch turned into a table or chain, the unrolled loop, the duplicated tail.
- A barrier (asm("" : "+r"(x)), asm volatile("" ::: "memory")), an offset cast (*(u32*)((char*)p + 0x10)),
  `volatile`, an empty `if (...) {}`, an inlining attribute, a union around a member or a goto usually stands
  for a missing helper, a wrong member or local type, or two variables merged into one. Never add one of these.
- No inline asm, no __attribute__, no #pragma, no new goto, no memcpy or memset standing in for a member store.
- A local never set is never read, passed or returned.
- Behaviour comes first: a candidate must do exactly what the function does now. Never change what is called,
  what is stored or the order of side effects to make the text prettier.

What you return, as JSON: `candidates`, a list of objects with
- `code`: the full replacement text of the function's definition, from its return type to its closing brace,
  with the same name and parameters unless a parameter's type is itself your hypothesis. It replaces the
  definition in the file shown; nothing else in the file and no header can change. Helper functions or small
  types the rewrite needs go in the same text directly before the definition (a `static inline` helper named for
  what it does, a struct with real member names where you know them). Use only declarations visible in the file
  and headers shown. Keep or update comments that describe the code; drop a comment about a form you removed.
- `change`: one line saying what you changed and the hypothesis behind it.

The two kinds of task:
- CLEAN: the function already matches, and its text has the steering form(s) the request lists. Rewrite it
  without them so that it still compiles to the same code, by undoing the optimization properly: the helper the
  original inlined, the right member type in a local view struct, two variables instead of one, a switch. For
  an offset cast, the usual fix is a typed access (a member of the struct the pointer really points at, declared
  in the text if no header has it, or indexing a typed pointer). For a barrier, the request shows what the
  compiler does without it: find the source shape that gives the original order naturally. Removing the form
  and leaving code that no longer matches is worthless; so is replacing it with another steering form.
- NEAR: the function is a draft that compiles to almost the original code. The request shows the rows where the
  two differ (original on the left, the draft's code on the right, `!!` marks a differing instruction; operands
  of address-forming instructions on the right are not yet relocated and differ only where marked). Find the
  source difference behind it: a swapped register pair is often the order two values are first computed or
  declared in, a value kept against reloaded is a local against a member read, an extra move is a parameter
  copied to a local, a differing compare is signedness or the operand order.
"""

SCHEMA = {
    "type": "object",
    "properties": {
        "candidates": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {"change": {"type": "string"}, "code": {"type": "string"}},
                "required": ["change", "code"],
                "additionalProperties": False,
            },
        },
    },
    "required": ["candidates"],
    "additionalProperties": False,
}

HARVEST_COLUMNS = ["custom_id", "pool", "address", "function", "path", "candidate", "kept", "why", "change"]
INCLUDE = re.compile(r'^[ \t]*#[ \t]*include[ \t]+"([^"]+)"', re.M)
SPECIFIC = re.compile(r"\b(?:fn_\w+|g_\w+|[A-Za-z_]\w*[A-Z]\w*|[A-Z]\w{2,})\b")
# Names a candidate may not bring in: a function or global nothing shown declares.
PLACEHOLDER = re.compile(r"\b(?:fn_[0-9a-f]{6,}|g_unk_\w+)\b")
# A member that is an array of bytes and not padding: an address taken through a made-up size, an offset cast in
# another form.
BYTE_MEMBER = re.compile(r"^[ \t]+(?:u8|s8|char|unsigned char)[ \t]+((?!pad_|unk_)\w+)[ \t]*\[[^\]]*\][ \t]*;", re.M)
NEW_TYPE = re.compile(r"\b(?:struct|class|union|enum(?:\s+class)?)\s+([A-Za-z_]\w*)\s*(?::[^;{()]*)?\{")
# Forms a candidate may not have more of than the text it replaces (fakematch's kinds are counted apart).
FORBIDDEN = {
    "goto": re.compile(r"\bgoto\b"),
    "inline asm": re.compile(r"\b(?:asm|__asm__)\b"),
    "an attribute": re.compile(r"__attribute__|\[\[|\bnoinline\b|\balways_inline\b|__declspec"),
    "a union": re.compile(r"\bunion\b"),
    "volatile": re.compile(r"\bvolatile\b"),
    "a pragma": re.compile(r"#\s*pragma\b"),
    "an untyped store": re.compile(r"\b(?:__builtin_)?mem(?:cpy|set|move)\s*\("),
}
_state: dict = {}


# ---------------------------------------------------------------------------------------------------- text


def sha(text: str) -> str:
    return hashlib.sha256(text.encode("utf-8")).hexdigest()[:16]


def read(path: str) -> str:
    return (ROOT / path).read_text(encoding="utf-8").replace("\r\n", "\n")


def definition_span(text: str, name: str) -> tuple[int, int] | None:
    """(start, end) of the definition of `name` in a file: from its first code token (comments above it stay
    out) to its closing brace. `name` may be qualified; its last component is tried too."""
    for cand in dict.fromkeys((name, name.split("::")[-1])):
        span = nb.find_definition(text, cand)
        if span is None:
            continue
        masked = nb.mask(text)
        s, e = span
        while s < e and masked[s].isspace():
            s += 1
        if s < e and masked[e - 1] == "}":
            return s, e
    return None


def comment_start(text: str, pos: int) -> int:
    """Where the block of `//` lines directly above the line of `pos` starts; `pos` when there is none."""
    line = text.rfind("\n", 0, pos) + 1
    if text[line:pos].strip():
        return pos
    s = line
    while s > 0:
        prev = text.rfind("\n", 0, s - 1) + 1
        if not text[prev:s].lstrip().startswith("//"):
            break
        s = prev
    return s if s != line else pos


def splice_span(text: str, name: str, replacement: str) -> tuple[str, int, int] | None:
    """(the file's text with the definition of `name` replaced, where the new text starts, where it ends), or
    None when the file does not define it. Helpers and types a replacement brings before the definition go
    above the comment the file has over the function, which stays (the replacement's own is used when the file
    has none)."""
    span = definition_span(text, name)
    if span is None:
        return None
    code = replacement.strip()
    inner = definition_span(code, name)
    cs = comment_start(text, span[0])
    if inner is None or inner[0] == 0:
        new = code
        cs = span[0]
    else:
        fs = comment_start(code, inner[0])
        prefix = code[:fs].rstrip()
        comment = text[cs:span[0]] or code[fs:inner[0]]
        new = (prefix + "\n\n" if prefix else "") + comment + code[inner[0]:]
    return text[:cs] + new + text[span[1]:], cs, cs + len(new)


def splice(text: str, name: str, replacement: str) -> str | None:
    """The file's text with the definition of `name` replaced, or None when the file does not define it."""
    got = splice_span(text, name, replacement)
    return got[0] if got else None


def line_of(text: str, pos: int) -> int:
    return text.count("\n", 0, pos) + 1


def steering_in(path: str, text: str, lo: int, hi: int) -> list[tuple[int, str]]:
    """(line, kind) of each steering form fakematch reports on lines lo..hi of a file's text."""
    import fakematch
    return sorted({(h.line, h.kind) for h in fakematch.scan_text(path, text)
                   if h.kind in STEERING and lo <= h.line <= hi})


def counts(text: str) -> dict[str, int]:
    import fakematch
    code = fakematch.strip_comments(text)
    return {what: len(rx.findall(code)) for what, rx in FORBIDDEN.items()}


def screen(path: str, old_text: str, name: str, code: str, pool: str, known: str,
           taken: frozenset[str] | set[str] = frozenset()) -> tuple[str | None, str]:
    """(the file's new text, "") when a candidate may be compiled, or (None, why not): it must define the
    function, add none of the FORBIDDEN forms, no fn_ or g_unk_ name that `known` (the file and the headers sent)
    does not have, define no type whose name the tree already has (`taken`), and leave fakematch nothing to
    report in it (clean) or nothing new (near)."""
    span = definition_span(old_text, name)
    if span is None:
        return None, "the file no longer defines the function"
    old_fn = old_text[span[0]:span[1]]
    new_text, new_lo, new_hi = splice_span(old_text, name, code)
    if definition_span(new_text, name) is None:
        return None, "the candidate does not define the function"
    before, after = counts(old_fn), counts(code)
    for what in FORBIDDEN:
        if after[what] > before[what]:
            return None, f"adds {what}"
    new_names = sorted(set(PLACEHOLDER.findall(nb.mask(code))) - set(PLACEHOLDER.findall(known)))
    if new_names:
        return None, f"a name nothing shown declares: {new_names[0]}"
    old_types = set(NEW_TYPE.findall(nb.mask(old_fn)))   # a type the replaced text itself defined may stay
    for t in NEW_TYPE.findall(nb.mask(code)):
        if t not in old_types and (t in taken or re.search(rf"\b{re.escape(t)}\b", nb.mask(known))):
            return None, f"defines a type the tree already has: {t}"
    blob = BYTE_MEMBER.search(nb.mask(code))
    if blob and not BYTE_MEMBER.search(nb.mask(old_fn)):
        return None, f"a byte-array member that is not padding: {blob.group(1)}"
    now = steering_in(path, new_text, line_of(new_text, new_lo), line_of(new_text, new_hi))
    was = steering_in(path, old_text, line_of(old_text, span[0]), line_of(old_text, span[1]))
    if pool == "clean" and now:
        return None, f"still has a steering form: {now[0][1]}"
    if pool == "near" and len(now) > len(was):
        return None, f"adds a steering form: {now[-1][1]}"
    return new_text, ""


# ---------------------------------------------------------------------------------------------------- headers


def chunks(text: str) -> list[tuple[int, int, bool]]:
    """A header as (start, end, structural) pieces: each declaration at namespace scope is a piece; a namespace's
    opening line and closing brace are structural pieces, always kept."""
    m = nb.mask(text)
    sc = nb.Scopes(m)
    out, start, depth, i, n = [], 0, 0, 0, len(m)
    while i < n:
        c = m[i]
        if c == "{":
            if depth == 0 and sc.kind.get(i) == "ns":
                out.append((start, i + 1, True))
                start = i + 1
            else:
                depth += 1
        elif c == "}":
            if depth == 0:
                out.append((start, i + 1, True))
                start = i + 1
            else:
                depth -= 1
                if depth == 0 and not re.match(r"\s*[;\w*&,=\[]", m[i + 1:i + 40]):
                    out.append((start, i + 1, False))   # a function body: no `;` follows
                    start = i + 1
        elif c == ";" and depth == 0:
            out.append((start, i + 1, False))
            start = i + 1
        i += 1
    if text[start:].strip():
        out.append((start, n, True))
    return out


def trim_header(text: str, wanted: set[str]) -> tuple[str, bool]:
    """The header with only the declarations that use a wanted identifier (short headers stay whole), and
    whether anything was left out."""
    if len(text) <= HEADER_WHOLE:
        return text, False
    m = nb.mask(text)
    out, dropped, gap = [], False, False
    for s, e, structural in chunks(text):
        if structural or wanted & set(SPECIFIC.findall(m[s:e])):
            if gap:
                out.append("\n// ...")
                gap = False
            out.append(text[s:e])
        else:
            dropped = gap = True
    return "".join(out).strip() + "\n", dropped


def resolve_include(name: str, from_dir: Path, root: Path) -> Path | None:
    for base in (from_dir, root / "include"):
        p = base / name
        if p.is_file():
            return p
    return None   # a library or system header: not sent


def headers_for(path: str, text: str, root: Path = ROOT, budget: int = HEADERS_MAX) -> tuple[list[tuple[str, str]], list[str]]:
    """The project headers a source includes, directly and through them once more, each trimmed to what the
    source uses: ([(path, text)], notes on what was left out)."""
    wanted = set(SPECIFIC.findall(nb.mask(text)))
    out: list[tuple[str, str]] = []
    notes: list[str] = []
    seen: set[Path] = set()
    level = [(root / path).parent / "x", text]
    queue = [(n, (root / path).parent) for n in INCLUDE.findall(text)]
    del level
    used = 0
    for depth in (1, 2):
        nxt = []
        for name, from_dir in queue:
            p = resolve_include(name, from_dir, root)
            if p is None or p.resolve() in seen:
                continue
            seen.add(p.resolve())
            raw = p.read_text(encoding="utf-8", errors="replace").replace("\r\n", "\n")
            body, dropped = trim_header(raw, wanted)
            rel = p.resolve().relative_to(root.resolve()).as_posix() if p.resolve().is_relative_to(root.resolve()) \
                else name
            if used + len(body) > budget:
                notes.append(f"{rel} (no room)")
                continue
            if not SPECIFIC.findall(nb.mask(body)) and dropped:
                continue
            used += len(body)
            out.append((rel + (" (trimmed to what the file uses)" if dropped else ""), body))
            wanted |= set(SPECIFIC.findall(nb.mask(body))) if depth == 1 and dropped else set()
            nxt += [(n, p.parent) for n in INCLUDE.findall(raw)]
        queue = nxt
    return out, notes


# ---------------------------------------------------------------------------------------------------- compiling


def init(version: str) -> None:
    if "orig" in _state:
        return
    import asmdiff
    import buildcfg
    import check
    _state.update(asmdiff=asmdiff, buildcfg=buildcfg, check=check, orig=asmdiff.Original(version))
    rows, _ = check.load_csv()
    _state["by_name"] = check.csv_by_name(rows)
    _state["sizes"] = {int(r["address"], 16): int(r["size"]) for r in rows.values()}
    _state["status"] = {int(r["address"], 16): r["status"] for r in rows.values()}


def compile_source(src: Path, source: str, obj: Path, keep: bool = False) -> list[dict] | str:
    """Every mapped function `src` defines, as it compiles now: [{address, name, difference, similarity,
    differing, digest}] (and with `keep` the diff result and code, for an excerpt); the first error line when it
    does not build. `source` is the repository path the text belongs to (a scratch copy keeps its file's)."""
    import drafts
    asmdiff, check = _state["asmdiff"], _state["check"]
    obj.parent.mkdir(parents=True, exist_ok=True)
    r = subprocess.run(_state["buildcfg"].cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        errors = [ln for ln in r.stderr.splitlines() if "error" in ln]
        return drafts.short((errors or r.stderr.splitlines() or ["no output"])[0], 200)
    funcs = asmdiff.read_object(obj)
    by_symbol = {f.symbol: f for f in funcs}
    results, names, full_names, targets = check.diff_with_targets(funcs, _state["orig"], _state["by_name"],
                                                                  source=source)
    out = []
    for res in results:
        if res.bytes_match:
            bad = asmdiff.check_relocs(res, _state["orig"], targets).mismatches
            bad += check.special_mismatches(res, _state["orig"], targets, full_names, source, set(by_symbol))
            difference = "wrong reference" if bad else "match"
        else:
            kind = asmdiff.classify(_state["orig"].function(res.address), by_symbol[res.symbol].code).split(" ")[0]
            difference = drafts.KINDS.get(kind, kind)
        f = {"address": res.address, "name": names.get(res.symbol, res.symbol), "difference": difference,
             "similarity": round(res.score * 100, 1), "differing": len(res.diff_insns),
             "digest": drafts.code_digest(by_symbol[res.symbol])}
        if keep:
            f["res"], f["code"] = res, by_symbol[res.symbol].code
        out.append(f)
    return out


def disassemble(code: bytes, base: int) -> list[str]:
    import capstone
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    return [f"{i.mnemonic} {i.op_str}" for i in cs.disasm(code, base)]


def diff_rows(left: list[str], right: list[str], differing: set[int], context: int = 3,
              limit: int = DIFF_MAX) -> str:
    """The rows of a side-by-side listing that differ, each with `context` rows around it."""
    n = max(len(left), len(right))
    marked = {i for i in range(n) if i in differing or i >= min(len(left), len(right))}
    shown = sorted({j for i in marked for j in range(max(0, i - context), min(n, i + context + 1))})
    out, last = [], -1
    for i in shown[:limit]:
        if i != last + 1:
            out.append("   ...")
        out.append(f"{'!!' if i in marked else '  '} {i:3} {left[i] if i < len(left) else '':40} "
                   f"{right[i] if i < len(right) else ''}".rstrip())
        last = i
    if len(shown) > limit:
        out.append(f"   ... ({len(shown) - limit} more rows not shown)")
    return "\n".join(out)


def excerpt(f: dict) -> str:
    """What separates a compiled function from the original: its kind, its shape and the differing rows."""
    asmdiff, orig = _state["asmdiff"], _state["orig"].function(f["address"])
    left, right = disassemble(orig, f["address"]), disassemble(f["code"], f["address"])
    return (f"KIND {asmdiff.classify(orig, f['code'])}\nSHAPE {asmdiff.structure(orig, f['code'])}\n"
            + diff_rows(left, right, set(f["res"].diff_insns)))


def listing(address: int) -> str:
    rows = disassemble(_state["orig"].function(address), address)
    if len(rows) > LISTING_MAX:
        return ""
    return "\n".join(f"{i:3} {r}" for i, r in enumerate(rows))


def scratch(run: str, *parts: str) -> Path:
    return nb.run_dir(run) / "work" / Path(*parts)


def compile_text(text: str, path: str, where: Path, keep: bool = False) -> list[dict] | str:
    """Compile `text` as a scratch copy of the repository file `path` (its relative includes rewritten)."""
    import drafts
    where.mkdir(parents=True, exist_ok=True)
    src = where / Path(path).name
    src.write_text(drafts.rehome(text, (ROOT / path).parent, where), encoding="utf-8", newline="\n")
    return compile_source(src, path, where / (Path(path).stem + ".o"), keep)


# ---------------------------------------------------------------------------------------------------- pools


def asked_before(pool: str, exclude: str = "") -> set[int]:
    """Addresses earlier runs of this pool already asked about."""
    out: set[int] = set()
    if nb.STATE.is_dir():
        for m in nb.STATE.glob("match-*/meta.json"):
            if m.parent.name == exclude or not (m.parent / "batch.json").exists():
                continue
            info = json.loads(m.read_text(encoding="utf-8"))
            if info.get("kind") == KIND[pool]:
                out |= {int(i["address"], 16) for i in info["items"]}
    return out


def clean_files() -> dict[str, list[tuple[int, str]]]:
    """Hand-written sources of src/ with a steering form: path -> [(line, kind)]."""
    import fakematch
    out: dict[str, list[tuple[int, str]]] = {}
    for f in fakematch.hand_written([ROOT / "src"]):
        if f.suffix != ".cpp":
            continue
        text = f.read_text(encoding="utf-8", errors="replace")
        if not any(w in text for w in ("asm", "volatile", "+", "{")):
            continue
        rel = f.relative_to(ROOT).as_posix()
        hits = sorted({(h.line, h.kind) for h in fakematch.scan_text(rel, text) if h.kind in STEERING})
        if hits:
            out[rel] = hits
    return out


def clean_candidates(run: str, jobs: int, max_size: int, skip: set[int]) -> list[dict]:
    """Matching functions with a steering form in their own text, smallest first."""
    files = {p: h for p, h in clean_files().items() if len(read(p)) <= FILE_MAX}

    def one(path: str) -> list[dict]:
        text = read(path)
        found = compile_source(ROOT / path, path, scratch(run, "base", re.sub(r"\W+", "_", path) + ".o"))
        if isinstance(found, str):
            return []
        out = []
        for f in found:
            span = definition_span(text, f["name"])
            if f["difference"] != "match" or span is None or _state["status"].get(f["address"]) != "O":
                continue
            lo, hi = line_of(text, span[0]), line_of(text, span[1])
            hits = [(ln, k) for ln, k in files[path] if lo <= ln <= hi]
            size = _state["sizes"].get(f["address"], 0)
            if hits and size <= max_size and f["address"] not in skip:
                out.append({"pool": "clean", "address": f"0x{f['address']:x}", "name": f["name"], "path": path,
                            "size": size, "hits": hits})
        return out
    with ThreadPoolExecutor(max_workers=jobs) as ex:
        items = [i for group in ex.map(one, sorted(files)) for i in group]
    return sorted(items, key=lambda i: (i["size"], i["address"]))


def near_candidates(min_similarity: float | None, max_size: int, skip: set[int]) -> list[dict]:
    """Drafts close to the original, fewest differing instructions first (grades as drafts/INDEX.csv has them;
    each is compiled again when its request is built)."""
    import drafts
    out = []
    for r in drafts.read_index():
        try:
            sim, differing = float(r["similarity"]), int(r["differing_instructions"])
        except ValueError:
            continue
        close = r["difference"] == REGISTERS if min_similarity is None else \
            (sim >= min_similarity and r["difference"] not in ("match", "build fails", "wrong reference"))
        if not close or int(r["size"]) > max_size or int(r["address"], 16) in skip:
            continue
        if not (ROOT / r["file"]).is_file() or len(read(r["file"])) > FILE_MAX:
            continue
        out.append({"pool": "near", "address": r["address"], "path": r["file"], "size": int(r["size"]),
                    "differing": differing, "difference": r["difference"]})
    return sorted(out, key=lambda i: (i["differing"], i["size"], i["address"]))


def take(items: list[dict], limit: int, spread: bool) -> list[dict]:
    """The first `limit` items, or with `spread` that many at even steps through the whole list."""
    if limit <= 0 or len(items) <= limit:
        return items
    if not spread:
        return items[:limit]
    return [items[(i * len(items)) // limit] for i in range(limit)]


def context(item: dict, run: str) -> dict | None:
    """What a request shows for one function, or None when it cannot be asked about (the file does not build,
    no definition is found, a draft that matches already)."""
    path, address = item["path"], int(item["address"], 16)
    text = read(path)
    found = compile_source(ROOT / path, path, scratch(run, "base", re.sub(r"\W+", "_", path) + ".o"), keep=True)
    if isinstance(found, str):
        return None
    f = next((x for x in found if x["address"] == address), None)
    if f is None or definition_span(text, f["name"]) is None:
        return None
    ctx = {"name": f["name"], "text": text, "sha": sha(text), "notes": [], "diff": "", "listing": "",
           "barriers": []}
    ctx["headers"], left_out = headers_for(path, text)
    if left_out:
        ctx["notes"].append("headers left out: " + ", ".join(left_out))
    if item["pool"] == "near":
        if f["difference"] in ("match", "wrong reference"):
            return None
        ctx["diff"] = excerpt(f)
        ctx["difference"], ctx["differing"] = f["difference"], f["differing"]
    else:
        ctx["listing"] = listing(address)
        import fakematch
        lines = text.split("\n")
        for ln, kind in item["hits"]:
            if kind != "barrier":
                continue
            raw = lines[ln - 1].strip()
            commented = ln >= 2 and lines[ln - 2].strip().startswith("//")
            without = fakematch.without_barrier(lines, ln, commented)
            if without is None:
                continue
            got = compile_text("\n".join(without), path, scratch(run, "nobarrier", f"{address:x}_{ln}"), keep=True)
            g = None if isinstance(got, str) else next((x for x in got if x["address"] == address), None)
            if g is None:
                continue
            ctx["barriers"].append({"line": ln, "text": raw, "without": "the function still matches without it"
                                    if g["difference"] == "match" else excerpt(g)})
    return ctx


# ---------------------------------------------------------------------------------------------------- requests


def max_tokens_for(fn_chars: int, k: int) -> int:
    """An output cap sized from the function: K rewrites of it, plus room to think (thinking counts, and took
    over 12,000 tokens in half of the first pilot's requests, which were lost whole)."""
    return int(min(48_000, max(32_000, 28_000 + k * (fn_chars / 3.0) * 1.5)))


def custom_id(pool: str, address: str) -> str:
    return f"{PREFIX[pool]}-{int(address, 16):x}"


def file_block(item: dict, ctx: dict) -> str:
    parts = []
    if ctx["headers"]:
        parts.append("Headers the file includes (the project's own):\n"
                     + "\n".join(nb.fence(p, t) for p, t in ctx["headers"]))
    parts.append(f"The source file, whole:\n" + nb.fence(item["path"], ctx["text"]))
    return "\n".join(parts)


def task_text(item: dict, ctx: dict, k: int) -> str:
    span = definition_span(ctx["text"], ctx["name"])
    lo, hi = line_of(ctx["text"], span[0]), line_of(ctx["text"], span[1])
    head = (f"Function: {ctx['name']} at {item['address']} ({item['size']} bytes of code), lines {lo} to {hi} of "
            f"{item['path']}.\n")
    parts = []
    if item["pool"] == "clean":
        parts.append("Task: CLEAN.\n" + head + "It matches the original now. Steering forms in it: "
                     + "; ".join(f"line {ln}: {kind}" for ln, kind in item["hits"]) + ".\n")
        for b in ctx["barriers"]:
            parts.append(f"Without the barrier on line {b['line']} (`{b['text']}`) the compiler gives (original "
                         f"left, without the barrier right):\n{b['without']}\n")
        if ctx["listing"]:
            parts.append("The original's code, which the function compiles to now:\n" + ctx["listing"] + "\n")
    else:
        parts.append("Task: NEAR.\n" + head + f"The draft differs from the original in {ctx['differing']} "
                     f"instruction(s):\n{ctx['diff']}\n")
    if ctx["notes"]:
        parts.append("Left out of this request: " + "; ".join(ctx["notes"]) + "\n")
    parts.append(f"Return {k} different candidates in the JSON form required.")
    return "\n".join(parts)


def build_requests(entries: list[tuple[dict, dict]], k: int = K, model: str = MODEL, effort: str = EFFORT,
                   cache: bool = True) -> tuple[list[dict], list[dict]]:
    """Requests for (item, context) pairs, ordered so the functions of one file are adjacent. The rules (system)
    are one cached prefix for the whole batch; a file's headers and text are a second cached block when more
    than one request shares them; what differs per function comes last."""
    entries = sorted(entries, key=lambda e: (e[0]["path"], int(e[0]["address"], 16)))
    per_file: dict[tuple[str, str], int] = {}
    for item, ctx in entries:
        per_file[(item["path"], ctx["sha"])] = per_file.get((item["path"], ctx["sha"]), 0) + 1
    system = [{"type": "text", "text": RULES}]
    if cache and len(entries) > 1 and len(RULES) / nb.CHARS_PER_TOKEN >= nb.MIN_CACHE_TOKENS:
        system[0]["cache_control"] = nb.cache_control("5m")
    requests, meta = [], []
    for item, ctx in entries:
        shared = {"type": "text", "text": file_block(item, ctx)}
        if cache and per_file[(item["path"], ctx["sha"])] > 1:
            shared["cache_control"] = nb.cache_control("5m")
        span = definition_span(ctx["text"], ctx["name"])
        cid = custom_id(item["pool"], item["address"])
        params = {
            "model": model,
            "max_tokens": max_tokens_for(span[1] - span[0], k),
            "thinking": nb.THINKING,
            "output_config": {"effort": effort, "format": {"type": "json_schema", "schema": SCHEMA}},
            "system": system,
            "messages": [{"role": "user", "content": [shared, {"type": "text", "text": task_text(item, ctx, k)}]}],
        }
        requests.append({"custom_id": cid, "params": params})
        meta.append({"custom_id": cid, "pool": item["pool"], "address": item["address"], "name": ctx["name"],
                     "path": item["path"], "sha": ctx["sha"], "size": item["size"], "k": k,
                     "hits": item.get("hits", []), "differing": ctx.get("differing", ""),
                     "difference": ctx.get("difference", ""), "notes": ctx["notes"]})
    ids = [r["custom_id"] for r in requests]
    if len(set(ids)) != len(ids) or not all(nb.CUSTOM_ID.match(i) for i in ids):
        raise SystemExit("match_batch: custom ids are not unique or not of the allowed form")
    return requests, meta


def candidates_of(result: dict) -> tuple[list[dict], str]:
    """(the candidates one batch result gave, status). Anything but a finished turn with JSON of the schema's
    shape gives none: an errored or expired request, a refusal, a truncated answer."""
    data, status = nb.answer_of(result)
    if data is None:
        return [], status
    cands = data.get("candidates")
    if not isinstance(cands, list):
        return [], "no candidates: answer outside the schema"
    good = [{"code": c["code"], "change": " ".join(str(c.get("change", "")).split())} for c in cands
            if isinstance(c, dict) and isinstance(c.get("code"), str) and c["code"].strip()]
    return good, "ok" if good else "no candidates"


# ---------------------------------------------------------------------------------------------------- commands


def check_run_name(run: str) -> None:
    if not run.startswith("match-"):
        raise SystemExit(f"match_batch: run name {run!r}: match runs are named match-<something>")


def cmd_build(args, client=None) -> int:
    check_run_name(args.run)
    init(args.version)
    skip = set() if args.again else asked_before(args.pool, exclude=args.run)
    if args.pool == "clean":
        pool = clean_candidates(args.run, args.jobs, args.max_size, skip)
    else:
        pool = near_candidates(args.min_similarity, args.max_size, skip)
    if args.only:
        wanted = {int(a, 16) for a in args.only.split(",")}
        pool = [i for i in pool if int(i["address"], 16) in wanted]
    chosen = take(pool, args.limit, args.spread)
    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        ctxs = list(ex.map(lambda i: context(i, args.run), chosen))
    entries = [(i, c) for i, c in zip(chosen, ctxs) if c is not None]
    if not entries:
        print(f"match_batch: nothing to ask about in pool {args.pool} ({len(pool)} candidates, {len(skip)} asked before)")
        return 1
    requests, meta = build_requests(entries, args.k, args.model, args.effort)
    info = nb.write_run(args.run, KIND[args.pool], requests, meta,
                        {"ttl": "5m", "effort": args.effort, "model": args.model, "k": args.k,
                         "version": args.version, "pool_size": len(pool)}, client, args.dry_run)
    est = info["estimate"]
    print(f"{args.run}: {est['requests']} requests of pool {args.pool} ({len(pool)} in the pool, "
          f"{len(chosen) - len(entries)} chosen but not askable), model {args.model}, K={args.k}; "
          f"~{est['input_tokens']:,} input tokens ({'counted' if info['counted_tokens'] else 'estimated'}); "
          f"batch cost worst ${est['worst_dollars']:.2f}, likely ${est['likely_dollars']:.2f}")
    print(f"  requests: {nb.run_dir(args.run) / 'requests.jsonl'}")
    return 0


def my_spend(exclude: str = "") -> tuple[float, float]:
    return nb.committed_spend(exclude=exclude, kinds="match")


def cmd_submit(args, client=None, lock=None) -> int:
    check_run_name(args.run)
    d = nb.run_dir(args.run)
    if not (d / "requests.jsonl").exists():
        print(f"match_batch: no requests for {args.run}; run build first")
        return 1
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    est = nb.estimate(nb.read_jsonl(d / "requests.jsonl"), info.get("counted_tokens") or None)
    spent, pending = my_spend(exclude=args.run)
    total = spent + pending + est["worst_dollars"]
    if total > args.cap:
        print(f"match_batch: REFUSED: match runs ${spent:.2f} + their pending batches ${pending:.2f} + this "
              f"batch's worst case ${est['worst_dollars']:.2f} = ${total:.2f}, over the match cap of "
              f"${args.cap:.2f}. Nothing was sent.")
        return 2
    # name_batch re-reads the ledger under its submit lock and checks the overall budget
    return nb.cmd_submit(argparse.Namespace(run=args.run, budget=args.budget), client=client, lock=lock)


def cmd_fetch(args, client=None) -> int:
    d = nb.run_dir(args.run)
    bid = nb.batch_id(args.run)
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    client = client or nb.make_client()
    b = client.messages.batches.retrieve(bid)
    if b.processing_status != "ended":
        print(f"{args.run}: {bid} is {b.processing_status}; fetch once it has ended")
        return 1
    raw = [nb.as_dict(r) for r in client.messages.batches.results(bid)]
    with (d / "results.jsonl").open("w", encoding="utf-8", newline="\n") as fh:
        for r in raw:
            fh.write(json.dumps(r, sort_keys=True) + "\n")
    total = {"input": 0, "write": 0, "read": 0, "output": 0, "write_1h": 0}
    cost, good, succeeded, ncand = 0.0, 0, 0, 0
    statuses: dict[str, int] = {}
    for r in raw:
        cands, status = candidates_of(r)
        statuses[status] = statuses.get(status, 0) + 1
        good += bool(cands)
        ncand += len(cands)
        if (r.get("result") or {}).get("type") == "succeeded":
            succeeded += 1
            u = nb.usage_of(r)
            for key in total:
                total[key] += u[key]
            cost += nb.dollars(u)
    nb.append_ledger({"run": args.run, "kind": info["kind"], "batch_id": bid,
                      "fetched": time.strftime("%Y-%m-%dT%H:%M:%S"), "requests": len(info["items"]),
                      "succeeded": succeeded, "verdicts": good, "input_tokens": total["input"],
                      "cache_write_tokens": total["write"], "cache_read_tokens": total["read"],
                      "output_tokens": total["output"], "cache_reads_happened": "yes" if total["read"] else "no",
                      "dollars": f"{cost:.4f}"})
    print(f"{args.run}: {len(raw)} results, {good} with candidates ({ncand} candidates); "
          + ", ".join(f"{s} {n}" for s, n in sorted(statuses.items())))
    print(f"  tokens: input {total['input']:,}, cache write {total['write']:,}, cache read {total['read']:,}, "
          f"output {total['output']:,}; ${cost:.2f}")
    return 0


def tree_types() -> set[str]:
    """Every type name the tracked sources define (src/, include/, drafts/): a candidate's new type needs another."""
    if "types" not in _state:
        out = subprocess.run(["git", "grep", "-h", "-o", "-E", r"\b(struct|class|union)\s+[A-Za-z_]\w*", "--", "src",
                              "include", "drafts"], cwd=ROOT, capture_output=True, text=True, encoding="utf-8",
                             errors="replace").stdout
        _state["types"] = {ln.split()[-1] for ln in out.splitlines() if ln.strip()}
    return _state["types"]


def judge(run: str, m: dict, index: int, cand: dict, base_text: str, base: list[dict] | str, known: str) -> dict:
    """One candidate's row of harvest.csv; a kept one's new file text is in row['text']."""
    row = {"custom_id": m["custom_id"], "pool": m["pool"], "address": m["address"], "function": m["name"],
           "path": m["path"], "candidate": index, "kept": "no", "why": "", "change": cand["change"][:300]}
    if isinstance(base, str):
        row["why"] = f"the file does not build as it is: {base}"
        return row
    new_text, why = screen(m["path"], base_text, m["name"], cand["code"], m["pool"], known, tree_types())
    if new_text is None:
        row["why"] = why
        return row
    got = compile_text(new_text, m["path"], scratch(run, "cand", m["custom_id"], str(index)))
    if isinstance(got, str):
        row["why"] = f"does not build: {got}"
        return row
    address = int(m["address"], 16)
    mine = next((f for f in got if f["address"] == address), None)
    if mine is None:
        row["why"] = "the object no longer defines the function"
    elif mine["difference"] != "match":
        row["why"] = f"{mine['difference']}, {mine['similarity']}% ({mine['differing']} differing instructions)"
    else:
        before = {f["address"]: f["digest"] for f in base if f["address"] != address}
        after = {f["address"]: f["digest"] for f in got if f["address"] != address}
        changed = sorted(a for a in before if after.get(a) != before[a])
        if changed:
            row["why"] = f"changes another function of the file: {changed[0]:#x}"
        else:
            row["kept"], row["why"], row["text"] = "yes", "matches" if m["pool"] == "near" else \
                "still matches, no steering form", new_text
    return row


def cmd_harvest(args) -> int:
    init(args.version)
    d = nb.run_dir(args.run)
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    by_id = {r["custom_id"]: r for r in nb.read_jsonl(d / "results.jsonl")}
    jobs = []
    bases: dict[str, tuple[str, list[dict] | str, str]] = {}
    rows = []
    for m in info["items"]:
        cands, status = candidates_of(by_id[m["custom_id"]]) if m["custom_id"] in by_id else ([], "missing")
        blank = {"custom_id": m["custom_id"], "pool": m["pool"], "address": m["address"], "function": m["name"],
                 "path": m["path"], "candidate": "", "kept": "no", "change": ""}
        if not cands:
            rows.append({**blank, "why": status})
            continue
        text = read(m["path"]) if (ROOT / m["path"]).is_file() else ""
        if sha(text) != m["sha"]:
            rows.append({**blank, "why": "the file changed since the request was built"})
            continue
        if m["path"] not in bases:
            base = compile_source(ROOT / m["path"], m["path"],
                                  scratch(args.run, "base", re.sub(r"\W+", "_", m["path"]) + ".h.o"))
            known = text + "\n".join(t for _, t in headers_for(m["path"], text)[0])
            bases[m["path"]] = (text, base, known)
        jobs += [(m, i, c) for i, c in enumerate(cands)]
    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        rows += list(ex.map(lambda j: judge(args.run, j[0], j[1], j[2], *bases[j[0]["path"]]), jobs))
    rows.sort(key=lambda r: (r["custom_id"], str(r["candidate"])))
    kept_dir = d / "kept"
    kept_dir.mkdir(exist_ok=True)
    kept: dict[str, dict] = {}
    for r in rows:
        if r["kept"] == "yes" and r["custom_id"] not in kept:
            kept[r["custom_id"]] = r
            (kept_dir / f"{r['custom_id']}.cpp").write_text(r["text"], encoding="utf-8", newline="\n")
        r.pop("text", None)
    with (d / "harvest.csv").open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=HARVEST_COLUMNS, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    n = len(info["items"])
    cost = nb.run_cost(args.run)
    why: dict[str, int] = {}
    for r in rows:
        if r["kept"] != "yes":
            key = re.sub(r"[:,].*", "", r["why"])
            why[key] = why.get(key, 0) + 1
    print(f"{args.run}: {n} functions, {len(jobs)} candidates, {len(kept)} functions kept "
          f"({len(kept) / n:.0%}); ${cost:.2f}" + (f", ${cost / len(kept):.2f} per kept function" if kept else ""))
    for key, count in sorted(why.items(), key=lambda kv: -kv[1]):
        print(f"  {count:4}  {key}")
    for cid, r in kept.items():
        print(f"  kept  {cid}  {r['path']}  candidate {r['candidate']}: {r['change'][:110]}")
    print(f"  table: {d / 'harvest.csv'}")
    return 0


def cmd_apply(args) -> int:
    """Write each kept candidate into its file, after checking it again against the file as it is now (an
    earlier apply may have changed another function of it)."""
    init(args.version)
    d = nb.run_dir(args.run)
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    by_id = {r["custom_id"]: r for r in nb.read_jsonl(d / "results.jsonl")}
    with (d / "harvest.csv").open(newline="", encoding="utf-8") as fh:
        kept = {}
        for r in csv.DictReader(fh):
            if r["kept"] == "yes":
                kept.setdefault(r["custom_id"], int(r["candidate"]))
    only = set(args.only.split(",")) if args.only else None
    done = 0
    for m in info["items"]:
        cid = m["custom_id"]
        if cid not in kept or (only is not None and cid not in only):
            continue
        cand = candidates_of(by_id[cid])[0][kept[cid]]
        text = read(m["path"])
        base = compile_source(ROOT / m["path"], m["path"], scratch(args.run, "apply", cid, "base.o"))
        known = text + "\n".join(t for _, t in headers_for(m["path"], text)[0])
        row = judge(args.run + "", m, kept[cid], cand, text, base, known)
        if row["kept"] != "yes":
            print(f"held   {cid}  {m['path']}: {row['why']}")
            continue
        (ROOT / m["path"]).write_text(row["text"], encoding="utf-8", newline="\n")
        done += 1
        print(f"wrote  {cid}  {m['path']}  ({m['name']})")
    print(f"match_batch: {done} file(s) written" + ("; move matched drafts with tools/drafts.py promote --write"
                                                   if info["kind"] == KIND["near"] and done else ""))
    return 0


def cmd_cost(args) -> int:
    rows = [r for r in nb.ledger_rows() if r["kind"].startswith("match")]
    for r in rows:
        print(f"  {r['run']:16} {r['kind']:12} {r['requests']:>4} requests  {r['verdicts']:>4} with candidates  "
              f"${float(r['dollars']):.2f}")
    mine = sum(float(r["dollars"]) for r in rows)
    spent, pending = nb.committed_spend()
    print(f"match runs: ${mine:.2f} (pending worst case ${my_spend()[1]:.2f}); whole ledger ${spent:.2f}, all "
          f"pending ${pending:.2f}")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    ap.add_argument("--version", default="2.1.0", help="the dump's version (build, harvest, apply)")
    sub = ap.add_subparsers(dest="cmd", required=True)
    b = sub.add_parser("build", help="choose functions of a pool and write their requests")
    b.add_argument("--run", required=True)
    b.add_argument("--pool", required=True, choices=sorted(KIND))
    b.add_argument("--limit", type=int, default=30, help="how many functions (0: the whole pool)")
    b.add_argument("--spread", action="store_true", help="take them at even steps through the pool, not from its start")
    b.add_argument("--k", type=int, default=K, help="candidates asked for per function")
    b.add_argument("--model", default=MODEL)
    b.add_argument("--effort", default=EFFORT, choices=["low", "medium", "high", "xhigh", "max"])
    b.add_argument("--max-size", type=int, default=2000, help="largest function, in bytes of code")
    b.add_argument("--min-similarity", type=float, help="near: any draft at least this close, not only registers-only")
    b.add_argument("--only", help="comma-separated addresses: only these functions of the pool")
    b.add_argument("--again", action="store_true", help="also functions an earlier run asked about")
    b.add_argument("--jobs", type=int, default=8)
    b.add_argument("--dry-run", action="store_true", help="no token count through the API (no key needed)")
    s = sub.add_parser("submit", help="create the batch")
    s.add_argument("--run", required=True)
    s.add_argument("--budget", type=float, default=nb.DEFAULT_BUDGET, help="the shared ledger's budget")
    s.add_argument("--cap", type=float, default=DEFAULT_CAP, help="dollars for all match runs together")
    for name, text in (("status", "processing status"), ("fetch", "results and the ledger row")):
        p = sub.add_parser(name, help=text)
        p.add_argument("--run", required=True)
    h = sub.add_parser("harvest", help="compile every candidate and write harvest.csv")
    h.add_argument("--run", required=True)
    h.add_argument("--jobs", type=int, default=8)
    a = sub.add_parser("apply", help="write the kept candidates into their files")
    a.add_argument("--run", required=True)
    a.add_argument("--only", help="comma-separated custom ids")
    sub.add_parser("cost", help="the match runs in the ledger")
    args = ap.parse_args(argv)
    return {"build": cmd_build, "submit": cmd_submit, "status": nb.cmd_status, "fetch": cmd_fetch,
            "harvest": cmd_harvest, "apply": cmd_apply, "cost": cmd_cost}[args.cmd](args)


if __name__ == "__main__":
    sys.exit(main())
