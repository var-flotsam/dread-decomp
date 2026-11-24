#!/usr/bin/env python3
"""Review, and propose, inferred function names through the Claude Message Batches API, with a hard spend stop.

The inferred-names tier (CLAUDE.md, "Names") needs every name checked by a reader that did not choose it
(PR #649: right, vague or wrong, and is the stated evidence true of the code). This builds one batch request per
name from the project's own reconstructed C++ (the function's defining file, up to 6 calling functions, whole)
and the class's reflected members (tools/gen_fields.py), submits it, and reads the verdicts back by custom_id.
A second mode asks for names for a class's unnamed matched functions; its proposals then go through the review
in a separate batch that sees only the name and the evidence line. Only names judged `right` with their
evidence judged true are applied, each through tools/rename.py.

State lives in build/name_batch/<run>/ (untracked): requests.jsonl, meta.json, batch.json, results.jsonl, and
verdicts.csv (a review run) or proposals.csv (a propose run). build/name_batch/ledger.csv gets one row per
fetched run: its tokens by kind, whether cache reads happened, and dollars at the batch rate. `submit` refuses
when the ledger total plus the estimates of runs submitted and not yet fetched plus a worst-case estimate of the
new batch would pass --budget.

The API key comes only from the environment variable DREAD_NAMES_API_KEY, passed to anthropic.Anthropic(api_key=...);
ANTHROPIC_API_KEY is never read (a key there would also bill Claude Code's own use on the machine), and the tool
exits naming the variable when it is unset. The key is never printed, logged or written. The anthropic package is
imported only by the commands that call the API.

Usage:
  python tools/name_batch.py build-review --run R --branch origin/claude/<b> [--dry-run]   # one request per name the branch adds
  python tools/name_batch.py build-review --run R --proposals build/name_batch/P/proposals.csv [--dry-run]
  python tools/name_batch.py build-propose --run P --classes FILE [--rev origin/main] [--dry-run]   # FILE: one class directory per line
  python tools/name_batch.py submit --run R [--budget 180]   # create the batch (refuses past the budget)
  python tools/name_batch.py status --run R                  # processing status and request counts
  python tools/name_batch.py fetch --run R                   # results, verdicts.csv or proposals.csv, a ledger row
  python tools/name_batch.py apply --run R                   # on a clean origin/main: rename the `right` names; apply.md for the PR
  python tools/name_batch.py cost                            # the ledger and its total
Building without --dry-run also counts each request's input tokens with the API's free token counter.
Standard library plus the official anthropic SDK; sends only the project's own C++ and reflection member names.
"""

from __future__ import annotations

import argparse
import csv
import json
import re
import subprocess
import sys
import time
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
from dread.paths import BUILD, ROOT  # noqa: E402

# The model and its settings (Claude API reference shipped with Claude Code 2.1.330, read 2026-11-24).
MODEL = "claude-opus-5-5"
EFFORT = "high"            # output_config.effort; this model's default is medium
MAX_TOKENS = 16000         # thinking counts toward it
# Thinking: always on for this model; {"type": "adaptive"} is the documented equivalent of omitting it.
THINKING = {"type": "adaptive"}
# Refusal fallbacks: not set. The reference (python/claude-api/batches.md, README.md "Refusal Fallbacks") says
# a batch request cannot set `fallbacks` (the parameter is rejected on the Batches API), so a refused request
# comes back with stop_reason "refusal" and counts here as no verdict.

# Dollars per million tokens for MODEL at the standard rate, from the same reference (shared/models.md and
# shared/prompt-caching.md, as of 2026-11-24): input 4, output 20, cache reads 0.20 (0.05x), cache writes
# 1.25x input for the 5-minute TTL and 2x for the 1-hour TTL. The Batches API takes 50% off every token,
# cache reads and writes included (shared/cost-optimization.md, 2.5). Confirm against the live price page
# before trusting a total.
PRICE = {"input": 4.00, "output": 20.00, "cache_read": 0.20, "cache_write_5m": 5.00, "cache_write_1h": 8.00}
BATCH_DISCOUNT = 0.5
# The minimum cacheable prefix for MODEL (shared/prompt-caching.md): a shorter prefix silently does not cache.
MIN_CACHE_TOKENS = 512
CHARS_PER_TOKEN = 3.0      # a conservative estimate for C++ and English (real text runs nearer 3.5 to 4)

# The only place the API key is read from (never ANTHROPIC_API_KEY, which Claude Code itself would bill to).
KEY_VARIABLE = "DREAD_NAMES_API_KEY"

STATE = BUILD / "name_batch"
LEDGER = STATE / "ledger.csv"
LEDGER_COLUMNS = ["run", "kind", "batch_id", "fetched", "requests", "succeeded", "verdicts", "input_tokens",
                  "cache_write_tokens", "cache_read_tokens", "output_tokens", "cache_reads_happened", "dollars"]
DEFAULT_BUDGET = 180.0

# Bounds on what one request carries (characters of source).
MAX_CALLERS = 6
DEF_FILE_MAX = 40_000       # a longer defining file is cut to the function itself, and the request says so
CALLER_MAX = 20_000         # a longer calling function is left out whole, and the request says so
CLASS_SOURCES_MAX = 120_000  # the class's shared block; defining files past it go into each request instead

# Generated directories (.gitattributes): their code is regenerated, not read as source.
GENERATED = ("src/reflection/", "src/reflection_types/", "src/reflection_statics/", "src/clones/",
             "src/tables/", "src/lifted/")
CUSTOM_ID = re.compile(r"^[A-Za-z0-9_-]{1,64}$")
PLACEHOLDER = re.compile(r"^fn_([0-9a-f]+)$")
VERDICTS = ("right", "vague", "wrong")

NAMING = """\
How names work in this project:
- The sources are the project's own C++ reconstruction of a game's compiled code. Names no one has chosen yet are
  placeholders: fn_<address> for a function, g_unk_<address> for a global, Unk... or UnkStruct_0x40 for a type,
  field_0x18 or pad_0x.. for a member, vf_0x<offset> for the virtual function at that byte offset of a vtable.
  refl_ marks a reflection role. "+0x548" in a comment is a byte offset into the object.
- Real names come only from the game's own metadata: run-time type names, reflection (a class's members by name,
  type and offset, given below when the class is reflected), and strings the code uses.
- An inferred name is chosen by reading what the function does. A function of a class's unit written as a free
  function is named <Class>_<VerbPhrase> (CTrainUsableComponent_GetUseAction); one written as a member keeps the
  Class::Method form. The verb phrase says what the code does, specifically enough to tell the function from its
  siblings in the same class, and claims nothing the code does not show (no purpose guessed from a callee's
  placeholder name, no "train animation" where the code only compares an id).
- Evidence is stated as facts in the code: the members it reads or writes by their reflected names and offsets,
  the strings it uses, the callees it calls, the conditions it tests, and what its callers do with it.
- A vague name (Update, Handle, Process, Run, Use, DoStep and the like, or any name that would fit many functions)
  or a name the code does not support is worse than keeping the placeholder: a placeholder is honest, a wrong name
  misleads every later reader. When in doubt, keep the placeholder.
"""

REVIEW_RULES = """\
You review one proposed name for one function of a decompilation project. You judge it alone: you are shown the
function's sources, some of its callers, the class's reflected members, the proposed name and the evidence its
proposer stated, and nothing else. Read the code yourself; do not trust the evidence because it is stated.

""" + NAMING + """
Verdict:
- right: the name says what the function does, specifically, and every claim the name makes is shown by the code.
- vague: nothing in it is false, but it is too general to tell the function from others, or it claims a purpose
  the code does not show.
- wrong: a claim the name makes is contradicted by the code, or is not supported by anything in it.
evidence_true: true only when every factual statement in the stated evidence is true of the code shown (offsets,
member names, callees, strings, conditions, what callers do). A statement that cannot be checked from what is
shown makes it false; say which one in the reason.
reason: one to three sentences that cite the facts in the code the verdict rests on. Where a source was left out
of the request (the request says so), judge only from what is shown.
"""

PROPOSE_RULES = """\
You propose a name for one unnamed function of a decompilation project, or decline to. You are shown the
function's sources, some of its callers and the class's reflected members.

""" + NAMING + """
Answer:
- decision "name": name is <Class>_<VerbPhrase> for the class given (a C++ identifier, no parameters), and
  evidence is one line of facts in the code that support it (members by reflected name and offset, strings,
  callees, conditions, what callers do). Name only what the code shows.
- decision "decline": name is empty, and evidence says what the function does as far as the code shows it. Decline
  whenever the best name would be vague or would rest on a guess; a declined function keeps its placeholder, which
  is better than a weak name.
reason: one or two sentences on why this name and not a nearby one, or why you declined.
"""

REVIEW_SCHEMA = {
    "type": "object",
    "properties": {
        "verdict": {"type": "string", "enum": list(VERDICTS)},
        "evidence_true": {"type": "boolean"},
        "reason": {"type": "string"},
    },
    "required": ["verdict", "evidence_true", "reason"],
    "additionalProperties": False,
}
PROPOSE_SCHEMA = {
    "type": "object",
    "properties": {
        "decision": {"type": "string", "enum": ["name", "decline"]},
        "name": {"type": "string"},
        "evidence": {"type": "string"},
        "reason": {"type": "string"},
    },
    "required": ["decision", "name", "evidence", "reason"],
    "additionalProperties": False,
}

VERDICT_COLUMNS = ["custom_id", "address", "name", "class", "verdict", "evidence_true", "reason", "stated_evidence",
                   "status"]
PROPOSAL_COLUMNS = ["custom_id", "address", "placeholder", "class", "rev", "decision", "name", "evidence", "reason",
                    "status"]


# ---------------------------------------------------------------------------------------------------- git


def git(*args: str) -> str:
    out = subprocess.run(["git", *args], cwd=ROOT, capture_output=True, text=True, encoding="utf-8",
                         errors="replace")
    if out.returncode != 0:
        raise SystemExit(f"name_batch: git {' '.join(args)} failed: {out.stderr.strip()}")
    return out.stdout


class Tree:
    """Files at one revision, read through git (no checkout)."""

    def __init__(self, rev: str):
        self.rev = rev
        self._files: dict[str, str] = {}

    def read(self, path: str) -> str:
        if path not in self._files:
            self._files[path] = git("show", f"{self.rev}:{path}")
        return self._files[path]

    def grep(self, word: str, paths: tuple[str, ...] = ("src",), generated: bool = False) -> list[str]:
        """Files under `paths` that use `word` as a whole word (generated directories left out unless asked)."""
        spec = list(paths) + ([] if generated else [f":(exclude){g}**" for g in GENERATED])
        out = subprocess.run(["git", "grep", "-l", "-w", "-F", "-e", word, self.rev, "--", *spec], cwd=ROOT,
                             capture_output=True, text=True, encoding="utf-8", errors="replace")
        if out.returncode not in (0, 1):
            raise SystemExit(f"name_batch: git grep failed: {out.stderr.strip()}")
        prefix = f"{self.rev}:"
        return sorted(ln[len(prefix):] if ln.startswith(prefix) else ln for ln in out.stdout.splitlines() if ln)

    def ls(self, directory: str) -> list[str]:
        return sorted(git("ls-tree", "-r", "--name-only", self.rev, "--", directory).split())


# ---------------------------------------------------------------------------------------------------- C++ text


def mask(text: str) -> str:
    """The text with comments, string and character literals and preprocessor lines blanked (same length and
    line breaks), so braces and names can be counted in code only."""
    out = list(text)
    i, n = 0, len(text)
    line_start = True
    while i < n:
        c = text[i]
        if line_start and c == "#":
            j = i
            while j < n and text[j] != "\n":
                j = j + 2 if text[j] == "\\" and j + 1 < n and text[j + 1] == "\n" else j + 1
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
            continue
        if c == "\n":
            line_start = True
            i += 1
            continue
        if c in " \t\r":
            i += 1
            continue
        line_start = False
        if text.startswith("//", i):
            j = text.find("\n", i)
            j = n if j < 0 else j
            for k in range(i, j):
                out[k] = " "
            i = j
        elif text.startswith("/*", i):
            j = text.find("*/", i + 2)
            j = n if j < 0 else j + 2
            for k in range(i, j):
                if out[k] != "\n":
                    out[k] = " "
            i = j
        elif c in "\"'":
            j = i + 1
            while j < n and text[j] != c and text[j] != "\n":
                j += 2 if text[j] == "\\" else 1
            j = min(j + 1, n)
            for k in range(i, j):
                out[k] = " "
            i = j
        else:
            i += 1
    return "".join(out)


class Scopes:
    """Brace structure of a masked file: for each brace, its kind ('ns' for namespace and extern blocks, 'class'
    for a struct, class or union body, 'code' for everything else) and its match."""

    def __init__(self, masked: str):
        self.m = masked
        self.match: dict[int, int] = {}
        self.kind: dict[int, str] = {}
        self.parent: dict[int, int] = {}   # open brace -> enclosing open brace (-1 at file scope)
        stack: list[int] = []
        for i, c in enumerate(masked):
            if c == "{":
                if stack and self.kind[stack[-1]] == "code":
                    kind = "code"
                else:
                    head = masked[self.head_start(i):i]
                    if re.search(r"\bnamespace\b[\w:\s]*$", head) or re.search(r"\bextern\s*$", head):
                        kind = "ns"
                    elif re.search(r"\benum\b", head):
                        kind = "code"
                    elif re.search(r"\b(struct|class|union)\b", head) and "(" not in head:
                        kind = "class"
                    else:
                        kind = "code"
                self.kind[i] = kind
                self.parent[i] = stack[-1] if stack else -1
                stack.append(i)
            elif c == "}" and stack:
                self.match[stack.pop()] = i

    def head_start(self, pos: int) -> int:
        """Start of the declaration that ends at `pos`: after the last ';', '{' or '}' before it."""
        j = pos - 1
        while j >= 0 and self.m[j] not in ";{}":
            j -= 1
        return j + 1

    def enclosing(self, pos: int) -> list[int]:
        """The open braces around `pos`, innermost first."""
        best = -1
        for o, c in self.match.items():
            if o < pos < c and (best < 0 or o > best):
                best = o
        chain = []
        while best >= 0:
            chain.append(best)
            best = self.parent[best]
        return chain

    def outer_code(self, pos: int) -> int:
        """The outermost 'code' brace around `pos`, or -1 when `pos` is at namespace or class scope."""
        found = -1
        for o in self.enclosing(pos):
            if self.kind[o] == "code":
                found = o
        return found


def words(masked: str, word: str) -> list[int]:
    return [m.start() for m in re.finditer(rf"(?<![\w]){re.escape(word)}(?![\w])", masked)]


def close_paren(masked: str, i: int) -> int:
    depth = 0
    for j in range(i, len(masked)):
        if masked[j] == "(":
            depth += 1
        elif masked[j] == ")":
            depth -= 1
            if depth == 0:
                return j
    return -1


def trim_head(text: str) -> str:
    """Drop leading blank and preprocessor lines and stray `using` lines from an extracted function."""
    lines = text.splitlines()
    while lines and (not lines[0].strip() or lines[0].lstrip().startswith("#")):
        lines.pop(0)
    return "\n".join(lines).rstrip() + "\n"


def find_definition(text: str, name: str) -> tuple[int, int] | None:
    """(start, end) of the definition of `name` in a file, or None. A macro use at namespace scope
    (`MACRO(Name, ...)`) counts as a definition: (start, end) is then that line."""
    last = name.split("::")[-1]
    m = mask(text)
    sc = Scopes(m)
    macro = None
    for p in words(m, last):
        if sc.outer_code(p) >= 0:
            continue
        rest = m[p + len(last):]
        k = len(rest) - len(rest.lstrip())
        q = p + len(last) + k
        if q < len(m) and m[q] == "(":
            e = close_paren(m, q)
            if e < 0:
                continue
            j = e + 1
            depth = 0
            while j < len(m):
                ch = m[j]
                if ch == "(":
                    depth += 1
                elif ch == ")":
                    depth -= 1
                elif depth == 0 and ch in "{;=":
                    break
                j += 1
            if j < len(m) and m[j] == "{" and j in sc.match:
                return sc.head_start(p), sc.match[j] + 1
            continue
        before = m[:p].rstrip()
        if before.endswith("(") and re.search(r"[A-Z_][A-Z0-9_]*\s*\($", before) and macro is None:
            s = m.rfind("\n", 0, p) + 1
            e = m.find("\n", p)
            macro = (s, len(m) if e < 0 else e + 1)
    return macro


def function_label(text: str, start: int, end: int) -> str:
    """The name a calling function is defined under (the identifier before its first parenthesis)."""
    head = mask(text[start:end])
    brace = head.find("{")
    head = head[:brace if brace >= 0 else len(head)]
    for m in re.finditer(r"([A-Za-z_][\w:]*)\s*\(", head):
        if m.group(1) not in ("__attribute__", "alignas", "__declspec", "decltype", "sizeof"):
            return m.group(1)
    found = re.findall(r"[A-Za-z_][\w:]*", head)
    return found[-1] if found else "(unnamed)"


def callers_in(text: str, name: str, skip: tuple[int, int] | None = None) -> list[tuple[int, int]]:
    """Whole top-level functions (or initialisers) of a file that use `name` in their code."""
    last = name.split("::")[-1]
    m = mask(text)
    sc = Scopes(m)
    spans = []
    for p in words(m, last):
        o = sc.outer_code(p)
        if o < 0:
            continue
        start, end = sc.head_start(o), sc.match.get(o, len(m) - 1) + 1
        if end < len(m) and m[end:].lstrip().startswith(";"):
            end = m.index(";", end) + 1
        if skip and skip[0] <= start and end <= skip[1]:
            continue
        if (start, end) not in spans:
            spans.append((start, end))
    return spans


# ---------------------------------------------------------------------------------------------------- context


def class_of(name: str, path: str | None = None) -> str:
    """The class a name belongs to: `Class::Method` -> Class, `CFoo_Bar` -> CFoo; else the class directory."""
    if "::" in name:
        return name.split("::")[-2]
    if "_" in name and not PLACEHOLDER.match(name):
        return name.split("_")[0]
    if path:
        parts = path.split("/")
        if len(parts) >= 3 and not parts[-2].startswith(("game", "fn_")):
            return parts[-2]
    return ""


def reflected_members(cls: str, runner=None) -> str:
    """tools/gen_fields.py's layout of the class and of its char class, when reflection knows them."""
    runner = runner or _gen_fields
    out = []
    for c in dict.fromkeys([cls, ("CCharClass" + cls[1:]) if cls.startswith("C") else ""]):
        if c:
            text = runner(c)
            if text:
                out.append(text.rstrip())
    return "\n\n".join(out)


def _gen_fields(cls: str) -> str:
    out = subprocess.run([sys.executable, str(ROOT / "tools" / "gen_fields.py"), "--class", cls], cwd=ROOT,
                         capture_output=True, text=True, encoding="utf-8", errors="replace")
    return out.stdout if out.returncode == 0 else ""


def gather(tree: Tree, name: str, max_callers: int = MAX_CALLERS) -> dict:
    """The function's defining file and calling functions at `tree`'s revision, bounded, with notes on anything
    left out."""
    notes: list[str] = []
    files = tree.grep(name.split("::")[-1])
    def_path, def_span = None, None
    for f in files:
        span = find_definition(tree.read(f), name)
        if span:
            def_path, def_span = f, span
            break
    if not def_path:
        notes.append("The function's definition was not found in the hand-written sources; none is shown.")
        def_text = ""
    else:
        full = tree.read(def_path)
        if len(full) <= DEF_FILE_MAX:
            def_text = full
        else:
            def_text = trim_head(full[def_span[0]:def_span[1]])
            notes.append(f"{def_path} is {len(full)} characters; only the function itself is shown from it.")
    callers = []
    in_def_file = 0
    for f in files:
        text = tree.read(f)
        skip = def_span if f == def_path else None
        for s, e in callers_in(text, name, skip):
            if f == def_path and len(text) <= DEF_FILE_MAX:
                in_def_file += 1
                continue
            callers.append({"path": f, "label": function_label(text, s, e), "text": trim_head(text[s:e])})
    # Callers in the same directory first, then the shortest: more callers fit, and none is cut.
    here = def_path.rsplit("/", 1)[0] if def_path else ""
    callers.sort(key=lambda c: (c["path"].rsplit("/", 1)[0] != here, len(c["text"]), c["path"], c["label"]))
    chosen, too_long, over = [], [], []
    for c in callers:
        if len(c["text"]) > CALLER_MAX:
            too_long.append(c)
        elif len(chosen) < max_callers:
            chosen.append(c)
        else:
            over.append(c)
    if too_long:
        notes.append(f"{len(too_long)} calling function(s) longer than {CALLER_MAX} characters were left out: "
                     + ", ".join(f"{c['label']} ({c['path']})" for c in too_long) + ".")
    if over:
        notes.append(f"{len(over)} more calling function(s) were left out (at most {max_callers} are shown): "
                     + ", ".join(f"{c['label']} ({c['path']})" for c in over) + ".")
    return {"def_path": def_path, "def_text": def_text, "def_whole": bool(def_path) and def_text == tree.read(def_path),
            "callers": chosen, "callers_in_def_file": in_def_file, "notes": notes}


def fence(path: str, text: str) -> str:
    return f"--- {path} ---\n```cpp\n{text.rstrip()}\n```\n"


# ---------------------------------------------------------------------------------------------------- requests


def cache_control(ttl: str) -> dict:
    return {"type": "ephemeral"} if ttl == "5m" else {"type": "ephemeral", "ttl": "1h"}


def build_requests(kind: str, items: list[dict], tree_for, members_for, ttl: str = "5m",
                   effort: str = EFFORT) -> tuple[list[dict], list[dict]]:
    """Requests for `items` (dicts with address, name, class, and for a review the stated evidence), grouped so a
    class's requests are adjacent. Shared content comes first with cache breakpoints: the rules (system), then per
    class its members and defining files; what differs per function comes last, after the last breakpoint.
    Returns (requests, meta items)."""
    rules = REVIEW_RULES if kind == "review" else PROPOSE_RULES
    schema = REVIEW_SCHEMA if kind == "review" else PROPOSE_SCHEMA
    caching = ttl != "off"
    groups: dict[str, list[dict]] = {}
    for it in sorted(items, key=lambda x: (x["class"], int(x["address"], 16))):
        groups.setdefault(it["class"], []).append(it)
    system = [{"type": "text", "text": rules}]
    if caching and len(items) > 1 and len(rules) / CHARS_PER_TOKEN >= MIN_CACHE_TOKENS:
        system[0]["cache_control"] = cache_control(ttl)
    requests, meta = [], []
    for cls, group in groups.items():
        ctxs = [(it, gather(tree_for(it), it["lookup"])) for it in group]
        # The class's shared block: its reflected members and the defining files of its functions.
        shared_files: dict[str, str] = {}
        size = 0
        for _, ctx in ctxs:
            p = ctx["def_path"]
            if p and ctx["def_whole"] and p not in shared_files and size + len(ctx["def_text"]) <= CLASS_SOURCES_MAX:
                shared_files[p] = ctx["def_text"]
                size += len(ctx["def_text"])
        members = members_for(cls) if cls else ""
        shared = [f"Class: {cls or '(none: a free function)'}\n"]
        shared.append("Reflected members (offset, name, type; members of a base class say which):\n"
                      + (members if members else "(this class is not reflected, or no class applies)") + "\n")
        if shared_files:
            shared.append("Sources of the class's functions under review (the project's reconstruction):\n")
            shared += [fence(p, t) for p, t in shared_files.items()]
        shared_text = "\n".join(shared)
        shared_block = {"type": "text", "text": shared_text}
        if caching and len(group) > 1 and (len(rules) + len(shared_text)) / CHARS_PER_TOKEN >= MIN_CACHE_TOKENS:
            shared_block["cache_control"] = cache_control(ttl)
        for it, ctx in ctxs:
            parts = []
            if kind == "review":
                parts.append(f"Function at {it['address']}, in the sources as {it['lookup']}.\n"
                             f"Proposed name: {it['name']}\nStated evidence: {it['evidence']}\n")
            else:
                parts.append(f"Function at {it['address']}, in the sources as {it['lookup']}, in class {cls}.\n"
                             f"Propose a name of the form {cls}_<VerbPhrase>, or decline.\n")
            if ctx["def_path"] in shared_files:
                parts.append(f"It is defined in {ctx['def_path']}, shown above in full.\n")
            elif ctx["def_path"]:
                parts.append(f"It is defined in {ctx['def_path']}:\n" + fence(ctx["def_path"], ctx["def_text"]))
            if ctx["callers_in_def_file"]:
                parts.append(f"{ctx['callers_in_def_file']} use(s) of it are in its own file, shown in full.\n")
            if ctx["callers"]:
                parts.append("Calling functions elsewhere (whole):\n"
                             + "\n".join(fence(f"{c['path']}: {c['label']}", c["text"]) for c in ctx["callers"]))
            elif not ctx["callers_in_def_file"]:
                parts.append("No hand-written source calls it by this name (it may be reached through a table, a "
                             "virtual slot or generated code).\n")
            if ctx["notes"]:
                parts.append("Left out of this request: " + " ".join(ctx["notes"]) + "\n")
            parts.append("Answer in the JSON form required." if kind == "review" else
                         "Answer in the JSON form required; decline rather than guess.")
            cid = f"{'rev' if kind == 'review' else 'prop'}-{int(it['address'], 16):x}"
            params = {
                "model": MODEL,
                "max_tokens": MAX_TOKENS,
                "thinking": THINKING,
                "output_config": {"effort": effort, "format": {"type": "json_schema", "schema": schema}},
                "system": system,
                "messages": [{"role": "user", "content": [shared_block, {"type": "text", "text": "\n".join(parts)}]}],
            }
            requests.append({"custom_id": cid, "params": params})
            entry = {k: it[k] for k in ("address", "name", "class", "lookup", "evidence", "rev") if k in it}
            entry.update({"custom_id": cid, "def_path": ctx["def_path"], "callers": len(ctx["callers"]),
                          "notes": ctx["notes"]})
            meta.append(entry)
    ids = [r["custom_id"] for r in requests]
    if len(set(ids)) != len(ids) or not all(CUSTOM_ID.match(i) for i in ids):
        raise SystemExit("name_batch: custom ids are not unique or not of the allowed form")
    return requests, meta


def request_chars(params: dict) -> tuple[int, int]:
    """(characters before the last cache breakpoint, characters after it) of a request's input."""
    blocks = list(params["system"]) + [b for msg in params["messages"] for b in msg["content"]]
    cached, total, upto = 0, 0, 0
    for b in blocks:
        total += len(b["text"])
        if "cache_control" in b:
            upto = total
    cached = upto
    return cached, total - cached + len(json.dumps(params["output_config"]["format"]))


def estimate(requests: list[dict], counted: dict[str, int] | None = None, ttl: str = "5m") -> dict:
    """Token and dollar estimates for a batch. `worst` assumes no cache read lands (every request writes its
    cached prefix) and every request uses all of max_tokens; `likely` assumes each class's first request writes
    and the rest of the class read."""
    write_rate = PRICE["cache_write_1h" if ttl == "1h" else "cache_write_5m"]
    tokens = worst = likely = 0.0
    seen_prefix: set[str] = set()
    for r in requests:
        cached_c, rest_c = request_chars(r["params"])
        if counted and r["custom_id"] in counted:
            total_t = counted[r["custom_id"]]
            cached_t = min(total_t, cached_c / CHARS_PER_TOKEN)
            rest_t = total_t - cached_t
        else:
            cached_t, rest_t = cached_c / CHARS_PER_TOKEN, rest_c / CHARS_PER_TOKEN
        tokens += cached_t + rest_t
        out = MAX_TOKENS * PRICE["output"]
        worst += cached_t * write_rate + rest_t * PRICE["input"] + out
        key = json.dumps([r["params"]["system"], r["params"]["messages"][0]["content"][0]], sort_keys=True)
        first = key not in seen_prefix
        seen_prefix.add(key)
        likely += (cached_t * (write_rate if first else PRICE["cache_read"]) + rest_t * PRICE["input"]
                   + MAX_TOKENS * 0.4 * PRICE["output"])
    return {"requests": len(requests), "input_tokens": int(tokens),
            "worst_dollars": round(worst / 1e6 * BATCH_DISCOUNT, 2),
            "likely_dollars": round(likely / 1e6 * BATCH_DISCOUNT, 2)}


def run_dir(run: str) -> Path:
    if not re.match(r"^[A-Za-z0-9_.-]+$", run):
        raise SystemExit(f"name_batch: run name {run!r}: use letters, digits, '.', '_' and '-'")
    return STATE / run


def write_run(run: str, kind: str, requests: list[dict], meta: list[dict], extra: dict, client=None,
              dry_run: bool = True) -> dict:
    d = run_dir(run)
    if (d / "batch.json").exists():
        raise SystemExit(f"name_batch: {d} already has a submitted batch; pick a new run name")
    d.mkdir(parents=True, exist_ok=True)
    with (d / "requests.jsonl").open("w", encoding="utf-8", newline="\n") as fh:
        for r in requests:
            fh.write(json.dumps(r, sort_keys=True) + "\n")
    counted = {}
    if not dry_run:
        client = client or make_client()
        for r in requests:
            p = r["params"]
            res = client.messages.count_tokens(model=p["model"], system=p["system"], messages=p["messages"])
            counted[r["custom_id"]] = res.input_tokens
    est = estimate(requests, counted or None, extra.get("ttl", "5m"))
    info = {"kind": kind, "items": meta, "estimate": est, "counted_tokens": counted, **extra}
    (d / "meta.json").write_text(json.dumps(info, indent=1, sort_keys=True), encoding="utf-8")
    return info


def print_summary(run: str, info: dict) -> None:
    est = info["estimate"]
    classes = len({m["class"] for m in info["items"]})
    left = sum(1 for m in info["items"] if m["notes"])
    print(f"{run}: {est['requests']} requests in {classes} class group(s); ~{est['input_tokens']:,} input tokens "
          f"({'counted' if info['counted_tokens'] else 'estimated at ' + str(CHARS_PER_TOKEN) + ' chars/token'}); "
          f"batch cost worst ${est['worst_dollars']:.2f}, likely ${est['likely_dollars']:.2f}; "
          f"{left} request(s) note something left out")
    print(f"  requests: {run_dir(run) / 'requests.jsonl'}")


# ---------------------------------------------------------------------------------------------------- build commands


def names_added(branch: str) -> list[dict]:
    """Rows of data/names.csv the branch adds or changes, relative to its merge base with origin/main."""
    base = git("merge-base", "origin/main", branch).strip()

    def rows(rev):
        return {r["address"]: r for r in csv.DictReader(git("show", f"{rev}:data/names.csv").splitlines())}

    old, new = rows(base), rows(branch)
    return [r for a, r in sorted(new.items(), key=lambda kv: int(kv[0], 16))
            if a not in old or old[a]["name"] != r["name"]]


def cmd_build_review(args, tree_factory=Tree, members=reflected_members, client=None) -> int:
    items = []
    extra = {"ttl": args.cache_ttl, "effort": args.effort}
    if args.branch:
        rev = args.branch
        for r in names_added(rev):
            items.append({"address": r["address"], "name": r["name"], "lookup": r["name"],
                          "class": class_of(r["name"]), "evidence": r["reason"], "rev": rev})
        extra["branch"] = rev
    else:
        with open(args.proposals, newline="", encoding="utf-8") as fh:
            for r in csv.DictReader(fh):
                if r["decision"] != "name" or not r["name"]:
                    continue
                items.append({"address": r["address"], "name": r["name"], "lookup": r["placeholder"],
                              "class": r["class"], "evidence": r["evidence"], "rev": r["rev"]})
        extra["proposals"] = str(args.proposals)
        extra["from_run"] = Path(args.proposals).resolve().parent.name
    if not items:
        print("name_batch: no names to review")
        return 1
    trees: dict[str, Tree] = {}

    def tree_for(it):
        return trees.setdefault(it["rev"], tree_factory(it["rev"]))

    requests, meta = build_requests("review", items, tree_for, members, args.cache_ttl, args.effort)
    info = write_run(args.run, "review", requests, meta, extra, client, args.dry_run)
    print_summary(args.run, info)
    return 0


def unnamed_matched(directory: str, tree: Tree, statuses: dict[str, str]) -> tuple[list[str], list[str]]:
    """The fn_<addr> functions defined in a class directory whose status is O, and those skipped because a
    generated directory refers to them (a clone family shared with other units, PR #649)."""
    found, skipped = [], []
    for f in tree.ls(directory):
        if not f.endswith(".cpp"):
            continue
        text = tree.read(f)
        for name in sorted(set(re.findall(r"\bfn_[0-9a-f]{10}\b", mask(text)))):
            if statuses.get(name) != "O" or name in found + skipped or not find_definition(text, name):
                continue
            if tree.grep(name, paths=tuple(g.rstrip("/") for g in GENERATED), generated=True):
                skipped.append(name)
            else:
                found.append(name)
    return found, skipped


def function_statuses() -> dict[str, str]:
    from dread import functions
    return {r["name"]: r["status"] for r in functions.rows()}


def cmd_build_propose(args, tree_factory=Tree, members=reflected_members, statuses=None, client=None) -> int:
    tree = tree_factory(args.rev)
    statuses = statuses if statuses is not None else function_statuses()
    items = []
    for line in Path(args.classes).read_text(encoding="utf-8").splitlines():
        directory = line.split("#", 1)[0].strip().rstrip("/")
        if not directory:
            continue
        if "/" not in directory:
            directory = f"src/game/{directory}"
        cls = directory.rsplit("/", 1)[-1]
        found, skipped = unnamed_matched(directory, tree, statuses)
        if skipped:
            print(f"  {cls}: skipped {len(skipped)} referenced from generated code: {', '.join(skipped)}")
        for name in found:
            addr = "0x" + PLACEHOLDER.match(name).group(1)
            items.append({"address": addr, "name": name, "lookup": name, "class": cls, "rev": args.rev})
    if not items:
        print("name_batch: no unnamed matched functions in the listed directories")
        return 1
    requests, meta = build_requests("propose", items, lambda it: tree, members, args.cache_ttl, args.effort)
    info = write_run(args.run, "propose", requests, meta, {"ttl": args.cache_ttl, "effort": args.effort,
                                                          "rev": args.rev}, client, args.dry_run)
    print_summary(args.run, info)
    return 0


# ---------------------------------------------------------------------------------------------------- API commands


def make_client(environ=None, factory=None):
    """The SDK client with the key from KEY_VARIABLE, passed by the documented `api_key=` argument. There is no
    fallback to ANTHROPIC_API_KEY: a key there would also be billed for Claude Code's own use on this machine.
    The value is never printed, logged or written."""
    import os
    key = (os.environ if environ is None else environ).get(KEY_VARIABLE, "")
    if not key.strip():
        raise SystemExit(f"name_batch: set {KEY_VARIABLE} to the API key for this tool (ANTHROPIC_API_KEY is not "
                         f"read); nothing was sent")
    if factory is None:
        import anthropic
        factory = anthropic.Anthropic
    return factory(api_key=key)


def read_jsonl(path: Path) -> list[dict]:
    return [json.loads(ln) for ln in path.read_text(encoding="utf-8").splitlines() if ln.strip()]


def ledger_rows() -> list[dict]:
    if not LEDGER.is_file():
        return []
    with LEDGER.open(newline="", encoding="utf-8") as fh:
        return list(csv.DictReader(fh))


def committed_spend(exclude: str = "") -> tuple[float, float]:
    """(dollars in the ledger, estimated dollars of runs submitted but not yet fetched)."""
    rows = ledger_rows()
    spent = sum(float(r["dollars"]) for r in rows)
    fetched = {r["run"] for r in rows}
    pending = 0.0
    if STATE.is_dir():
        for b in STATE.glob("*/batch.json"):
            run = b.parent.name
            if run in fetched or run == exclude:
                continue
            pending += float(json.loads(b.read_text(encoding="utf-8")).get("worst_dollars", 0))
    return spent, pending


def cmd_submit(args, client=None) -> int:
    d = run_dir(args.run)
    if (d / "batch.json").exists():
        print(f"name_batch: {args.run} was already submitted ({d / 'batch.json'})")
        return 1
    if not (d / "requests.jsonl").exists():
        print(f"name_batch: no requests for {args.run}; run build-review or build-propose first")
        return 1
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    requests = read_jsonl(d / "requests.jsonl")
    est = estimate(requests, info.get("counted_tokens") or None, info.get("ttl", "5m"))
    spent, pending = committed_spend(exclude=args.run)
    total = spent + pending + est["worst_dollars"]
    if total > args.budget:
        print(f"name_batch: REFUSED: ledger ${spent:.2f} + pending batches ${pending:.2f} + this batch's worst case "
              f"${est['worst_dollars']:.2f} = ${total:.2f}, over the budget of ${args.budget:.2f}. Nothing was sent.")
        return 2
    client = client or make_client()
    try:
        from anthropic.types.message_create_params import MessageCreateParamsNonStreaming
        from anthropic.types.messages.batch_create_params import Request
    except ImportError:  # both are TypedDicts: plain dicts of the same shape (a test's fake client, no SDK)
        MessageCreateParamsNonStreaming = Request = dict
    batch = client.messages.batches.create(requests=[
        Request(custom_id=r["custom_id"], params=MessageCreateParamsNonStreaming(**r["params"])) for r in requests])
    (d / "batch.json").write_text(json.dumps({"id": batch.id, "submitted": time.strftime("%Y-%m-%dT%H:%M:%S"),
                                              "requests": len(requests), "worst_dollars": est["worst_dollars"]},
                                             indent=1), encoding="utf-8")
    print(f"{args.run}: batch {batch.id} created, {len(requests)} requests, status {batch.processing_status}; "
          f"worst case ${est['worst_dollars']:.2f} (ledger ${spent:.2f}, pending ${pending:.2f}, "
          f"budget ${args.budget:.2f})")
    return 0


def batch_id(run: str) -> str:
    p = run_dir(run) / "batch.json"
    if not p.exists():
        raise SystemExit(f"name_batch: {run} has not been submitted")
    return json.loads(p.read_text(encoding="utf-8"))["id"]


def cmd_status(args, client=None) -> int:
    client = client or make_client()
    b = client.messages.batches.retrieve(batch_id(args.run))
    c = b.request_counts
    print(f"{args.run}: {b.id} {b.processing_status}; processing {c.processing}, succeeded {c.succeeded}, "
          f"errored {c.errored}, canceled {c.canceled}, expired {c.expired}")
    return 0


def as_dict(obj) -> dict:
    if isinstance(obj, dict):
        return obj
    if hasattr(obj, "to_dict"):
        return obj.to_dict()
    raise TypeError(f"cannot read a result of type {type(obj).__name__}")


def parse_result(result: dict, schema_kind: str) -> tuple[dict | None, str]:
    """(the answer, status) of one batch result. Anything but a finished turn with valid JSON is no verdict:
    errored, canceled or expired requests, stop_reason refusal or max_tokens (truncated), bad JSON."""
    res = result.get("result") or {}
    rtype = res.get("type")
    if rtype != "succeeded":
        err = (res.get("error") or {}).get("type") if rtype == "errored" else ""
        return None, f"{rtype or 'missing'}{': ' + err if err else ''}"
    msg = res.get("message") or {}
    stop = msg.get("stop_reason")
    if stop in ("refusal", "max_tokens"):
        return None, f"no verdict: stop_reason {stop}"
    if stop != "end_turn":
        return None, f"no verdict: stop_reason {stop}"
    text = next((b.get("text", "") for b in msg.get("content", []) if b.get("type") == "text"), None)
    if text is None:
        return None, "no verdict: no text block"
    try:
        data = json.loads(text)
    except json.JSONDecodeError:
        return None, "no verdict: not JSON"
    if schema_kind == "review":
        if data.get("verdict") not in VERDICTS or not isinstance(data.get("evidence_true"), bool):
            return None, "no verdict: answer outside the schema"
    elif data.get("decision") not in ("name", "decline"):
        return None, "no verdict: answer outside the schema"
    return data, "ok"


def usage_of(result: dict) -> dict:
    msg = ((result.get("result") or {}).get("message") or {})
    u = msg.get("usage") or {}
    cc = u.get("cache_creation") or {}
    return {"input": u.get("input_tokens") or 0, "write": u.get("cache_creation_input_tokens") or 0,
            "read": u.get("cache_read_input_tokens") or 0, "output": u.get("output_tokens") or 0,
            "write_1h": cc.get("ephemeral_1h_input_tokens") or 0}


def dollars(u: dict) -> float:
    w1h = u["write_1h"]
    w5m = max(0, u["write"] - w1h)
    d = (u["input"] * PRICE["input"] + w5m * PRICE["cache_write_5m"] + w1h * PRICE["cache_write_1h"]
         + u["read"] * PRICE["cache_read"] + u["output"] * PRICE["output"])
    return d / 1e6 * BATCH_DISCOUNT


def cmd_fetch(args, client=None) -> int:
    d = run_dir(args.run)
    bid = batch_id(args.run)
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    kind = info["kind"]
    client = client or make_client()
    b = client.messages.batches.retrieve(bid)
    if b.processing_status != "ended":
        print(f"{args.run}: {bid} is {b.processing_status}; fetch once it has ended")
        return 1
    raw = [as_dict(r) for r in client.messages.batches.results(bid)]
    with (d / "results.jsonl").open("w", encoding="utf-8", newline="\n") as fh:
        for r in raw:
            fh.write(json.dumps(r, sort_keys=True) + "\n")
    by_id = {r["custom_id"]: r for r in raw}   # keyed by custom_id: results come back in any order
    total = {"input": 0, "write": 0, "read": 0, "output": 0, "write_1h": 0}
    cost = 0.0
    rows = []
    good = succeeded = 0
    for m in info["items"]:
        r = by_id.get(m["custom_id"])
        if r is None:
            data, status = None, "missing from the results"
        else:
            data, status = parse_result(r, kind)
            if (r.get("result") or {}).get("type") == "succeeded":
                succeeded += 1
                u = usage_of(r)
                for k in total:
                    total[k] += u[k]
                cost += dollars(u)
        good += data is not None
        if kind == "review":
            rows.append({"custom_id": m["custom_id"], "address": m["address"], "name": m["name"], "class": m["class"],
                         "verdict": data["verdict"] if data else "", "evidence_true":
                         ("yes" if data["evidence_true"] else "no") if data else "",
                         "reason": data["reason"] if data else "", "stated_evidence": m.get("evidence", ""),
                         "status": status})
        else:
            rows.append({"custom_id": m["custom_id"], "address": m["address"], "placeholder": m["lookup"],
                         "class": m["class"], "rev": m["rev"], "decision": data["decision"] if data else "",
                         "name": data["name"] if data else "", "evidence": data["evidence"] if data else "",
                         "reason": data["reason"] if data else "", "status": status})
    out = d / ("verdicts.csv" if kind == "review" else "proposals.csv")
    with out.open("w", newline="", encoding="utf-8") as fh:
        w = csv.DictWriter(fh, fieldnames=VERDICT_COLUMNS if kind == "review" else PROPOSAL_COLUMNS,
                           lineterminator="\n")
        w.writeheader()
        w.writerows(rows)
    row = {"run": args.run, "kind": kind, "batch_id": bid, "fetched": time.strftime("%Y-%m-%dT%H:%M:%S"),
           "requests": len(info["items"]), "succeeded": succeeded, "verdicts": good,
           "input_tokens": total["input"], "cache_write_tokens": total["write"], "cache_read_tokens": total["read"],
           "output_tokens": total["output"], "cache_reads_happened": "yes" if total["read"] else "no",
           "dollars": f"{cost:.4f}"}
    if any(r["run"] == args.run for r in ledger_rows()):
        print(f"  ledger: {args.run} is already recorded; not added again")
    else:
        LEDGER.parent.mkdir(parents=True, exist_ok=True)
        new = not LEDGER.exists()
        with LEDGER.open("a", newline="", encoding="utf-8") as fh:
            w = csv.DictWriter(fh, fieldnames=LEDGER_COLUMNS, lineterminator="\n")
            if new:
                w.writeheader()
            w.writerow(row)
    print(f"{args.run}: {len(raw)} results, {good} with an answer; {out}")
    print(f"  tokens: input {total['input']:,}, cache write {total['write']:,}, cache read {total['read']:,}, "
          f"output {total['output']:,}; cache reads {'happened' if total['read'] else 'did NOT happen'}; "
          f"${cost:.2f}")
    if kind == "review":
        counts = {v: sum(1 for r in rows if r["verdict"] == v) for v in VERDICTS}
        print("  " + ", ".join(f"{v} {n}" for v, n in counts.items())
              + f", no verdict {sum(1 for r in rows if not r['verdict'])}")
    return 0


def run_cost(run: str) -> float:
    return sum(float(r["dollars"]) for r in ledger_rows() if r["run"] == run)


def choose(rows: list[dict]) -> tuple[list[dict], list[dict], list[dict]]:
    """(applied, rejected, no verdict): only `right` with the evidence judged true is applied."""
    applied = [r for r in rows if r["verdict"] == "right" and r["evidence_true"] == "yes"]
    rejected = [r for r in rows if r["verdict"] and r not in applied]
    none = [r for r in rows if not r["verdict"]]
    return applied, rejected, none


def cmd_apply(args, runner=None) -> int:
    d = run_dir(args.run)
    vpath = d / "verdicts.csv"
    if not vpath.exists():
        print(f"name_batch: {args.run} has no verdicts.csv (fetch a review run first)")
        return 1
    if runner is None:
        if git("status", "--porcelain", "--untracked-files=no").strip():
            print("name_batch: the work tree has changes; apply needs a clean tree at origin/main")
            return 1
        if git("rev-parse", "HEAD").strip() != git("rev-parse", "origin/main").strip():
            print("name_batch: HEAD is not origin/main; check out origin/main (on a new branch) first")
            return 1

        def runner(addr, name, reason):
            return subprocess.run([sys.executable, str(ROOT / "tools" / "rename.py"), addr, name, "--reason", reason],
                                  cwd=ROOT, capture_output=True, text=True, encoding="utf-8", errors="replace")
    with vpath.open(newline="", encoding="utf-8") as fh:
        rows = list(csv.DictReader(fh))
    applied, rejected, none = choose(rows)
    landed, failed = [], []
    for r in applied:
        res = runner(r["address"], r["name"], r["stated_evidence"])
        (landed if res.returncode == 0 else failed).append((r, (res.stdout + res.stderr).strip()))
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    spend = run_cost(args.run) + (run_cost(info["from_run"]) if info.get("from_run") else 0.0)
    lines = [f"Names reviewed by run `{args.run}` ({MODEL}, Message Batches): {len(rows)} reviewed, "
             f"{len(landed)} applied, {len(rejected)} rejected, {len(none)} without a verdict"
             + (f", {len(failed)} failed to apply" if failed else "") + ".", "",
             "Applied (verdict right, evidence judged true):", "",
             "| Address | Name | Evidence in the code |", "|---|---|---|"]
    lines += [f"| `{r['address']}` | `{r['name']}` | {r['stated_evidence']} |" for r, _ in landed]
    lines += ["", "Rejected (placeholder kept):", ""]
    lines += [f"- `{r['address']}` `{r['name']}`: {r['verdict']}, evidence "
              f"{'true' if r['evidence_true'] == 'yes' else 'not true'}; {r['reason']}" for r in rejected]
    lines += ["", "No verdict (placeholder kept):", ""]
    lines += [f"- `{r['address']}` `{r['name']}`: {r['status']}" for r in none]
    if failed:
        lines += ["", "Judged right, but tools/rename.py refused:", ""]
        lines += [f"- `{r['address']}` `{r['name']}`: {msg.splitlines()[-1] if msg else 'failed'}" for r, msg in failed]
    per = f"${spend / len(landed):.2f}" if landed else "n/a"
    lines += ["", f"Cost: ${spend:.2f} at the batch rate (ledger), {per} per landed name."]
    (d / "apply.md").write_text("\n".join(lines) + "\n", encoding="utf-8")
    print(f"{args.run}: applied {len(landed)}, rejected {len(rejected)}, no verdict {len(none)}"
          + (f", rename failed {len(failed)}" if failed else "") + f"; {d / 'apply.md'}")
    print(f"  cost per landed name: {per} (${spend:.2f} in all). Nothing was committed or pushed.")
    return 0


def cmd_cost(args) -> int:
    rows = ledger_rows()
    if not rows:
        print("name_batch: the ledger is empty")
    for r in rows:
        print(f"{r['run']:24} {r['kind']:8} {r['requests']:>5} req  in {int(r['input_tokens']):>10,}  "
              f"write {int(r['cache_write_tokens']):>10,}  read {int(r['cache_read_tokens']):>10,}  "
              f"out {int(r['output_tokens']):>9,}  cache reads {r['cache_reads_happened']:3}  ${float(r['dollars']):.2f}")
    spent, pending = committed_spend()
    print(f"total ${spent:.2f}; submitted and not fetched (worst case) ${pending:.2f}; "
          f"budget default ${DEFAULT_BUDGET:.0f}")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)

    def build_opts(p):
        p.add_argument("--run", required=True)
        p.add_argument("--dry-run", action="store_true", help="write the requests and estimate; no API contact")
        p.add_argument("--effort", default=EFFORT, choices=["low", "medium", "high", "xhigh", "max"])
        p.add_argument("--cache-ttl", default="5m", choices=["5m", "1h", "off"],
                       help="cache breakpoint TTL (hits inside a batch are best-effort)")

    p = sub.add_parser("build-review")
    build_opts(p)
    src = p.add_mutually_exclusive_group(required=True)
    src.add_argument("--branch")
    src.add_argument("--proposals")
    p = sub.add_parser("build-propose")
    build_opts(p)
    p.add_argument("--classes", required=True)
    p.add_argument("--rev", default="origin/main")
    p = sub.add_parser("submit")
    p.add_argument("--run", required=True)
    p.add_argument("--budget", type=float, default=DEFAULT_BUDGET)
    for name in ("status", "fetch", "apply"):
        sub.add_parser(name).add_argument("--run", required=True)
    sub.add_parser("cost")
    args = ap.parse_args(argv)
    return {"build-review": cmd_build_review, "build-propose": cmd_build_propose, "submit": cmd_submit,
            "status": cmd_status, "fetch": cmd_fetch, "apply": cmd_apply, "cost": cmd_cost}[args.cmd](args)


if __name__ == "__main__":
    raise SystemExit(main())
