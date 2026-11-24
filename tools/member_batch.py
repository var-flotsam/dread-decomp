#!/usr/bin/env python3
"""Give hand-written view structs' placeholder members (`field_0x..`) the names the game's reflection already has,
through the Claude Message Batches API, with a spend cap of its own.

Most hand-written code reads a reflected class through an `Unk...` view struct whose members are `field_0x34`.
data/reflection_fields.csv names the class's members by offset and type (tools/gen_fields.py --class prints a
class with its bases), so a member of a view can take its reflected name once three things are shown: the view is
a view of that class, the member sits at the reflected member's offset exactly, and the code uses it as that type.
This is sourced naming (name_source reflection), not inferred naming: no name comes from anywhere but the table.

  scan       every Unk... struct under src/game/ and include/game/ with field_0x members, its candidate classes
             (the directory's class and its char class, the class getters the code calls, reflected class names
             the code mentions) and how many placeholders sit at a candidate member's offset (no API)
  build      one request per view struct: its definition, its members' offsets and types as the compiler lays
             them out, the functions that use it (whole, bounded), and the candidates' reflected members. It asks
             which class the struct is a view of, or none, and per placeholder the reflected member at that offset
             whose type agrees with how the code uses it, or to keep the placeholder, with the evidence as facts
  build-review  a second, separate request per mapped struct: the same sources and the mapping with its evidence
             only (not the first answer's reasoning); it agrees or not with the class and with each member
  submit/status/fetch  as tools/name_batch.py, on the shared ledger build/name_batch/ledger.csv; `submit` refuses
             when this tool's runs (kind member-*) would pass --cap (40 dollars) or the ledger would pass --budget
  agree      the members both requests agree on whose offset matches exactly and whose type is compatible (same
             size and kind): rows for data/member_names.csv
  apply      the rename, re-runnable after merging main: for each row of data/member_names.csv whose struct still
             has the placeholder, the member is renamed in the struct and every use is renamed where the compiler
             reports it (-fsyntax-only on the file, or every hand-written includer of a header, until it compiles);
             anything else the compiler reports puts the struct back as it was
  cost       this tool's runs in the ledger

The acceptance test is tools/objsame.py in exact mode on every changed file and every includer of a changed
header (`objsame.py save --tag members --base origin/main` then `compare --tag members`).

Usage:
  python tools/member_batch.py scan [--top 60]
  python tools/member_batch.py build --run mem-p1 (--select 40 | --structs FILE) [--dry-run]   # FILE: path:Struct lines
  python tools/member_batch.py build-review --run mem-r1 --from mem-p1 [--dry-run]
  python tools/member_batch.py submit --run mem-p1 [--cap 40] [--budget 180]
  python tools/member_batch.py status --run mem-p1
  python tools/member_batch.py fetch --run mem-p1
  python tools/member_batch.py agree --propose mem-p1 --review mem-r1 [--write]   # --write: data/member_names.csv
  python tools/member_batch.py apply [--only path:Struct]
  python tools/member_batch.py cost
The key comes only from DREAD_NAMES_API_KEY (tools/name_batch.py, make_client). Requests carry only the project's
own C++ and reflected names. Standard library plus the anthropic SDK for the billed commands.
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
import name_batch as nb  # noqa: E402
from dread.paths import ROOT  # noqa: E402

TABLE = ROOT / "data" / "member_names.csv"
TABLE_COLUMNS = ["path", "struct", "placeholder", "name", "class", "offset", "reflected_type", "view_type",
                 "evidence"]
KIND_PROPOSE, KIND_REVIEW = "member-propose", "member-review"
DEFAULT_CAP = 40.0
SCAN_ROOTS = ("src/game", "include/game")
MAX_USES = 8
USE_MAX = 20_000            # a longer using function is left out whole, and the request says so
USES_TOTAL = 80_000
MAX_CANDIDATES = 6
UNKNOWN_OFFSET = 0xFFFFFFFF
PLACEHOLDER = re.compile(r"\bfield_0x([0-9a-f]+)\b")
VIEW_HEAD = re.compile(r"\b(?:struct|class|union)\s+(Unk\w*)\s*(?:final\s*)?(?::[^{]*)?$")

RULES = """\
You work on a decompilation project: the sources are the project's own C++ reconstruction of a game's compiled
code. Names no one has chosen yet are placeholders: fn_<address> for a function, g_unk_<address> for a global,
Unk... for a type, field_0x18 or pad_0x.. for a member (the hex number is the member's byte offset).

Many hand-written functions read an object of a reflected class through an Unk... "view" struct: a struct that
declares only the members the code touches, at the class's offsets, with placeholder names. The game's reflection
data names a reflected class's members with their byte offsets and types (shown below for each candidate class,
base classes included; a member whose offset is unknown cannot be mapped). Your job is to say whether the struct is
a view of one of the candidate classes, and which placeholder members are reflected members.

Rules:
- The struct is a view of a class only when the code shows the object is one: the function is that class's (its
  directory or name says so and the object is `this`/`self` of that class), it calls the class's getter, it casts
  to the class, or it reaches the object through a member that holds one. A struct that is the owner, an entity,
  a definition, a message or a helper object passed to the class's function is a view of something else: answer
  "none" unless a candidate is that thing. When unsure, answer "none".
- A placeholder member maps to a reflected member only when its offset (from the compiler's layout given below,
  not from its name) is exactly the reflected member's offset in that class, and the code uses it as that type: a
  bool tested or set to true/false, a float in float arithmetic, an int or enum compared or counted, a CStrId
  hashed or compared, a vector's three floats. A member declared as raw bytes (u8[N]) of exactly the reflected
  type's size may map when the code treats it as that type (passes its address to that type's functions, copies or
  destroys it whole) and the class is shown. A member of a different size or kind, or one the code uses in a way
  the reflected type does not allow, keeps its placeholder (reflected ""). Never invent a name: only reflected
  names of the chosen class (or its bases) at that exact offset.
- Evidence is facts in the code: which function, the statement or condition, the value stored or the call made,
  and why the object is of that class.
- A wrong name misleads every later reader; a placeholder is honest. When in doubt, keep the placeholder.
"""

REVIEW_RULES = RULES + """
Here you review a mapping someone else made, from the same sources. You are shown their answer: the class, and for
each mapped member the reflected name and their stated evidence. Read the code yourself; do not trust the evidence
because it is stated. Agree with the class only if the code shows the object is of that class. Agree with a member
only if the class is right, the offsets match exactly, the type agrees with the code's use, and the stated
evidence is true of the code. A statement that cannot be checked from what is shown means you do not agree.
"""

PROPOSE_SCHEMA = {
    "type": "object",
    "properties": {
        "class": {"type": "string"},
        "members": {"type": "array", "items": {
            "type": "object",
            "properties": {"member": {"type": "string"}, "reflected": {"type": "string"},
                           "evidence": {"type": "string"}},
            "required": ["member", "reflected", "evidence"], "additionalProperties": False}},
        "reason": {"type": "string"},
    },
    "required": ["class", "members", "reason"],
    "additionalProperties": False,
}
REVIEW_SCHEMA = {
    "type": "object",
    "properties": {
        "class_agree": {"type": "boolean"},
        "class_reason": {"type": "string"},
        "members": {"type": "array", "items": {
            "type": "object",
            "properties": {"member": {"type": "string"}, "agree": {"type": "boolean"}, "reason": {"type": "string"}},
            "required": ["member", "agree", "reason"], "additionalProperties": False}},
    },
    "required": ["class_agree", "class_reason", "members"],
    "additionalProperties": False,
}


# ---------------------------------------------------------------------------------------------------- reflection


class Reflection:
    """Reflected classes, their members by offset (bases included) and sizes, from the tracked tables."""

    def __init__(self, fields: list[dict] | None = None, types: list[dict] | None = None):
        if fields is None:
            import gen_fields
            fields = gen_fields.load()
        if types is None:
            with (ROOT / "data" / "reflection_types.csv").open(newline="", encoding="utf-8") as f:
                types = list(csv.DictReader(f))
        self.fields = fields
        self.types = {r["type"]: r for r in types}
        self.classes = {t for t, r in self.types.items() if r.get("kind") == "class"}
        self.by_short: dict[str, list[str]] = {}
        for c in sorted(self.classes):
            self.by_short.setdefault(c.split("::")[-1], []).append(c)
        self.getters = {r["getter"].lower(): t for t, r in self.types.items()
                        if r.get("kind") == "class" and r.get("getter")}
        self._layout: dict[str, list[tuple[int, str, str, str]]] = {}

    def layout(self, cls: str) -> list[tuple[int, str, str, str]]:
        """(offset, member, type, declaring class) of a class and its bases, by offset."""
        if cls not in self._layout:
            out, seen, c = [], set(), cls
            while c and c not in seen:
                seen.add(c)
                out += [(int(r["offset"], 16), r["name"], r["type"], c) for r in self.fields if r["class"] == c]
                c = self.types.get(c, {}).get("parent", "")
            self._layout[cls] = sorted(out)
        return self._layout[cls]

    def known(self, cls: str) -> list[tuple[int, str, str, str]]:
        return [m for m in self.layout(cls) if m[0] != UNKNOWN_OFFSET]

    def resolve(self, name: str) -> list[str]:
        if name in self.classes:
            return [name]
        return self.by_short.get(name.split("::")[-1], [])

    def describe(self, cls: str) -> str:
        t = self.types.get(cls, {})
        lines = [f"{cls}: {t.get('size') or '?'} bytes, base {t.get('parent') or 'none'}"]
        for off, member, typ, owner in self.layout(cls):
            where = f"   (declared in {owner})" if owner != cls else ""
            o = "offset unknown" if off == UNKNOWN_OFFSET else f"{off:#06x}"
            lines.append(f"  {o:>14}  {member:<32} {typ}{where}")
        return "\n".join(lines)


# ---------------------------------------------------------------------------------------------------- types


PRIMITIVE = {
    "bool": ("bool", 1), "_Bool": ("bool", 1),
    "char": ("int", 1), "signed char": ("int", 1), "unsigned char": ("int", 1), "u8": ("int", 1), "s8": ("int", 1),
    "short": ("int", 2), "unsigned short": ("int", 2), "u16": ("int", 2), "s16": ("int", 2),
    "int": ("int", 4), "unsigned int": ("int", 4), "unsigned": ("int", 4), "u32": ("int", 4), "s32": ("int", 4),
    "long": ("int", 8), "unsigned long": ("int", 8), "long long": ("int", 8), "unsigned long long": ("int", 8),
    "u64": ("int", 8), "s64": ("int", 8), "uintptr_t": ("int", 8), "size_t": ("int", 8),
    "float": ("float", 4), "f32": ("float", 4), "double": ("float", 8), "f64": ("float", 8),
}
VECTORS = {"CVector2D": 2, "CVector3D": 3, "CVector4D": 4}


def strip_cv(t: str) -> str:
    t = re.sub(r"\b(const|volatile|struct|class|union|enum)\b", " ", t)
    return re.sub(r"\s+", " ", t).strip()


def view_kind(typ: str, enums: set[str] = frozenset()) -> tuple[str, int, str]:
    """(kind, size, record name) of a member type as the compiler's layout dump spells it; size 0 when unknown.
    Kinds: bool, int, float, ptr, record, array:<elem kind>:<count>, unknown."""
    t = typ.strip()
    m = re.match(r"^(.*?)\s*((?:\[\d+\])+)$", t)
    if m:
        dims = [int(d) for d in re.findall(r"\[(\d+)\]", m.group(2))]
        k, s, rec = view_kind(m.group(1), enums)
        n = 1
        for d in dims:
            n *= d
        return f"array:{k}:{n}", s * n, rec
    if t.endswith("*") or t.endswith("&"):
        return "ptr", 8, ""
    is_enum = t.startswith("enum ")
    base = strip_cv(t)
    if base in PRIMITIVE:
        return PRIMITIVE[base] + ("",)
    if is_enum or base in enums:
        return "int", 4, base
    return "record", 0, base


def reflected_kind(typ: str, refl: Reflection | None = None) -> tuple[str, int, str]:
    """(kind, size, record name) of a reflected member type; size 0 when unknown."""
    t = typ.strip().strip('"')
    if t.endswith("*"):
        return "ptr", 8, ""
    base = strip_cv(t)
    if base in PRIMITIVE:
        return PRIMITIVE[base] + ("",)
    last = base.split("::")[-1]
    if last == "Enum" or re.match(r"^E[A-Z]", last):
        return "enum", 4, base
    size = 0
    if refl is not None and base in refl.types and refl.types[base].get("size"):
        size = int(refl.types[base]["size"], 16)
    return "record", size, base


def compatible(reflected: str, view_type: str, view_size: int, refl: Reflection | None = None,
               view_record: str = "", sizes: dict[str, int] | None = None) -> bool:
    """Same size and kind: a bool for a bool (or a one-byte int), an int or enum for a four-byte int, a float for a
    float, a pointer for a pointer, a class for a member of that class (or of the same size, a vector for its
    floats), and a class for opaque storage of exactly its size (a u8 array: storage of no kind, never a scalar).
    Anything else, or an unknown size, is not compatible. `sizes`: record sizes in bytes by unqualified name."""
    rk, rs, rrec = reflected_kind(reflected, refl)
    vk = view_type
    if rk == "record" and not rs and sizes:
        rs = sizes.get(rrec.split("::")[-1], 0) if "<" not in rrec else 0
    if rk == "record" and vk == "array:int:" + str(view_size) and rs and rs == view_size:
        return True
    if rk == "bool":
        return (vk == "bool" and view_size == 1) or (vk == "int" and view_size == 1)
    if rk == "int":
        return vk == "int" and view_size == rs
    if rk == "enum":
        return vk == "int" and view_size == 4
    if rk == "float":
        return vk == "float" and view_size == rs
    if rk == "ptr":
        return vk == "ptr"
    last = rrec.split("::")[-1]
    if last in VECTORS and vk == f"array:float:{VECTORS[last]}":
        return True
    if vk != "record" or not view_record:
        return False
    vlast = re.sub(r"<.*", "", view_record.split("::")[-1]) if "<" not in rrec else view_record
    if "<" in rrec:   # a template: the whole spelling must agree, namespaces aside
        norm = lambda s: re.sub(r"\b\w+::", "", s).replace(" ", "")  # noqa: E731
        return norm(rrec) == norm(view_record) and (not rs or rs == view_size)
    return vlast == last and (not rs or not view_size or rs == view_size)


# ---------------------------------------------------------------------------------------------------- structs


def find_views(text: str) -> list[dict]:
    """Unk... struct definitions in a file that declare field_0x.. members directly in their body:
    {struct, open, close, placeholders}."""
    m = nb.mask(text)
    sc = nb.Scopes(m)
    out = []
    for o, kind in sc.kind.items():
        if kind != "class" or o not in sc.match:
            continue
        head = m[sc.head_start(o):o]
        hm = VIEW_HEAD.search(head.strip())
        if not hm:
            continue
        close = sc.match[o]
        names = []
        for pm in PLACEHOLDER.finditer(m, o, close):
            inner = sc.enclosing(pm.start())
            if inner and inner[0] == o and pm.group(0) not in names:
                names.append(pm.group(0))
        if names:
            out.append({"struct": hm.group(1), "open": o, "close": close, "placeholders": names})
    return sorted(out, key=lambda v: v["open"])


def struct_text(text: str, view: dict) -> str:
    m = nb.mask(text)
    start = nb.Scopes(m).head_start(view["open"])
    end = view["close"] + 1
    if text[end:end + 1] == ";":
        end += 1
    return nb.trim_head(text[start:end])


def read(path: str) -> str:
    """A file's text with its line endings as they are (the rename writes them back unchanged)."""
    with open(ROOT / path, encoding="utf-8", errors="replace", newline="") as f:
        return f.read()


def hand_files() -> list[str]:
    out = subprocess.run(["git", "ls-files", "--", *SCAN_ROOTS], cwd=ROOT, capture_output=True, text=True,
                         encoding="utf-8").stdout.split()
    return sorted(p for p in out if p.endswith((".cpp", ".h")) and not p.startswith(nb.GENERATED))


def users_of(path: str, struct: str) -> list[str]:
    """Files whose code may use a struct: the file itself, or for a header the hand-written sources that mention
    the struct by name."""
    if path.endswith(".cpp"):
        return [path]
    found = subprocess.run(["git", "grep", "-l", "-w", "-F", "-e", struct, "--", "src", "include"], cwd=ROOT,
                           capture_output=True, text=True, encoding="utf-8").stdout.split()
    return [path] + sorted(f for f in found if f != path)


def generated_mentions(struct: str) -> bool:
    out = subprocess.run(["git", "grep", "-l", "-w", "-F", "-e", struct, "--",
                          *[g.rstrip("/") for g in nb.GENERATED], "include/reflected"],
                         cwd=ROOT, capture_output=True, text=True, encoding="utf-8").stdout
    return bool(out.strip())


def functions_naming(text: str, name: str) -> list[tuple[int, int]]:
    """(start, end) of each top-level function (at namespace scope, or a method inside a class body) whose head
    or body names `name`: a parameter of the struct's type counts as a use."""
    m = nb.mask(text)
    sc = nb.Scopes(m)
    spans = []
    for o, kind in sorted(sc.kind.items()):
        if kind != "code" or o not in sc.match:
            continue
        parent = sc.parent[o]
        if parent >= 0 and sc.kind[parent] == "code":
            continue
        start, end = sc.head_start(o), sc.match[o] + 1
        if "(" not in m[start:o] or not nb.words(m[start:end], name):
            continue
        spans.append((start, end))
    return spans


def uses(path: str, struct: str, files: list[str] | None = None) -> tuple[list[dict], list[str]]:
    """Whole top-level functions that use the struct, bounded: (uses, notes on anything left out)."""
    found = []
    for f in files if files is not None else users_of(path, struct):
        text = read(f)
        if f == path and f.endswith(".cpp") and len(text) <= nb.DEF_FILE_MAX:
            found.append({"path": f, "label": "(the whole file)", "text": text})
            continue
        for s, e in functions_naming(text, struct):
            found.append({"path": f, "label": nb.function_label(text, s, e), "text": nb.trim_head(text[s:e])})
    found.sort(key=lambda u: (u["path"] != path, len(u["text"]), u["path"], u["label"]))
    chosen, long_, over, total = [], [], [], 0
    for u in found:
        if u["label"] == "(the whole file)":
            chosen.append(u)
            total += len(u["text"])
        elif len(u["text"]) > USE_MAX:
            long_.append(u)
        elif len(chosen) < MAX_USES and total + len(u["text"]) <= USES_TOTAL:
            chosen.append(u)
            total += len(u["text"])
        else:
            over.append(u)
    notes = []
    if long_:
        notes.append(f"{len(long_)} using function(s) longer than {USE_MAX} characters were left out: "
                     + ", ".join(f"{u['label']} ({u['path']})" for u in long_) + ".")
    if over:
        notes.append(f"{len(over)} more using function(s) were left out: "
                     + ", ".join(f"{u['label']} ({u['path']})" for u in over) + ".")
    return chosen, notes


def candidates(path: str, struct: str, texts: list[str], refl: Reflection, users: list[str] = ()) -> list[str]:
    """Candidate classes, best first: the directory's class (and nested class) and its char class, the classes whose
    getters the code calls, then reflected class names the code mentions (casts, members of that type)."""
    out: list[str] = []

    def add(names):
        for n in names:
            if n not in out:
                out.append(n)

    for p in [path, *users]:
        parts = p.split("/")
        if parts[:2] in (["src", "game"], ["include", "game"]):
            dirs = parts[2:-1] + [Path(parts[-1]).stem]
            for i in range(len(dirs)):
                full = "::".join(dirs[:i + 1])
                if full in refl.classes:
                    add([full])
            for d in dirs[:1]:
                if d in refl.classes and d.startswith("C"):
                    add(refl.resolve("CCharClass" + d[1:]))
    joined = "\n".join(texts)
    for g in re.findall(r"\bfn_(71[0-9a-f]{8})\b", joined):
        c = refl.getters.get("0x" + g)
        if c:
            add([c])
    for c in re.findall(r"\b([A-Z]\w*(?:::\w+)*)::refl_GetClass\b", joined):
        add(refl.resolve(c))
    for word in sorted(set(re.findall(r"\b(C[A-Z]\w+)\b", nb.mask(joined))), key=joined.find):
        add(refl.resolve(word))
    return [c for c in out if refl.known(c)][:MAX_CANDIDATES]


def name_offset(placeholder: str) -> int:
    return int(PLACEHOLDER.match(placeholder).group(1), 16)


def prescore(placeholders: list[str], cands: list[str], refl: Reflection) -> int:
    offs = {m[0] for c in cands for m in refl.known(c)}
    return sum(1 for p in placeholders if name_offset(p) in offs)


# ---------------------------------------------------------------------------------------------------- the compiler


def compile_cmd(path: str, extra: list[str]) -> list[str]:
    import buildcfg
    cmd = buildcfg.cxx_command(Path(path), Path("nul"), extra)
    i = cmd.index("-c")
    return cmd[:i] + [path]


def record_layouts(path: str, runner=None) -> dict[str, dict]:
    """The compiler's record layouts in a file's translation unit: name -> {size, fields: [(offset, type, name)]}
    (direct members only). The file is compiled to an object (layouts are dumped only for records code needs)."""
    import tempfile
    runner = runner or (lambda cmd: subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                                                   encoding="utf-8", errors="replace"))
    with tempfile.TemporaryDirectory() as tmp:
        import buildcfg
        cmd = buildcfg.cxx_command(Path(path), Path(tmp) / "x.o", ["-Xclang", "-fdump-record-layouts"])
        out = runner(cmd)
    return parse_layouts(out.stdout)


def parse_layouts(dump: str) -> dict[str, dict]:
    layouts: dict[str, dict] = {}
    for block in dump.split("*** Dumping AST Record Layout")[1:]:
        lines = block.splitlines()
        body = [ln for ln in lines if "|" in ln]
        if not body:
            continue
        head = re.match(r"^\s*0 \| (?:struct|class|union) (.+?)\s*$", body[0])
        if not head:
            continue
        name = head.group(1)
        fields, size = [], 0
        for ln in body[1:]:
            sm = re.search(r"\[sizeof=(\d+)", ln)
            if sm:
                size = int(sm.group(1))
                break
            fm = re.match(r"^\s*(\d+)(?::\d+-\d+)? \|   (\S.*)$", ln)
            if not fm or fm.group(2).startswith(" "):
                continue
            rest = fm.group(2)
            if re.search(r"\((?:primary |virtual )?base\)$|\(vtable pointer\)$|\(empty\)$", rest):
                continue
            tm = re.match(r"^(.*\S)\s+(\w+)$", rest)
            if tm:
                fields.append((int(fm.group(1)), tm.group(1), tm.group(2)))
        layouts.setdefault(name, {"size": size, "fields": fields})
    return layouts


def layout_of(layouts: dict[str, dict], struct: str) -> dict | None:
    for name, lay in layouts.items():
        if name == struct or name.endswith("::" + struct):
            return lay
    return None


PROBE = """\
#include "base/global/CStrId.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CName.h"
#include "base/math/CVector.h"
struct MemberBatchProbe {
    base::global::CStrId a;
    base::global::CFilePathStrId b;
    base::global::CRntString c;
    base::global::CName d;
    base::math::CVector2D e;
    base::math::CVector3D f;
};
unsigned long MemberBatchProbeSize(MemberBatchProbe* p) { return sizeof(*p); }
"""
_PROBED: dict[str, int] = {}


def record_sizes(layouts: dict[str, dict]) -> dict[str, int]:
    """Record sizes in bytes by unqualified name (a name laid out twice with two sizes is left out)."""
    out: dict[str, int] = {}
    bad = set()
    for name, lay in layouts.items():
        last = name.split("::")[-1]
        if last.startswith("("):   # anonymous
            continue
        size = lay["size"]
        if last in out and out[last] != size:
            bad.add(last)
        out[last] = size
    return {k: v for k, v in out.items() if k not in bad}


def probed_sizes(runner=None) -> dict[str, int]:
    """Sizes of the record types reflection uses most (CStrId, CFilePathStrId, CRntString, CName, vectors), from
    the project's own headers as the compiler lays them out; cached for the run."""
    if not _PROBED:
        probe = nb.STATE / "member_probe.cpp"
        probe.parent.mkdir(parents=True, exist_ok=True)
        probe.write_text(PROBE, encoding="utf-8")
        _PROBED.update(record_sizes(record_layouts(probe.relative_to(ROOT).as_posix(), runner)))
        _PROBED.pop("MemberBatchProbe", None)
    return dict(_PROBED)


def member_types(layouts: dict[str, dict], struct: str) -> dict[str, dict]:
    """placeholder -> {offset, type, kind, size, record} for a struct's direct members."""
    lay = layout_of(layouts, struct)
    if not lay:
        return {}
    out = {}
    for off, typ, name in lay["fields"]:
        kind, size, rec = view_kind(typ)
        if kind == "record" or kind.startswith("array:record"):
            sub = layout_of(layouts, rec.split("::")[-1]) if rec else None
            if sub:
                n = int(kind.split(":")[2]) if kind.startswith("array:") else 1
                size = sub["size"] * n
        out[name] = {"offset": off, "type": typ, "kind": kind, "size": size, "record": rec}
    return out


# ---------------------------------------------------------------------------------------------------- scan


def scan(refl: Reflection, files: list[str] | None = None) -> list[dict]:
    rows = []
    for path in files if files is not None else hand_files():
        text = read(path)
        for v in find_views(text):
            users = users_of(path, v["struct"]) if path.endswith(".h") else [path]
            texts = [read(f) for f in users]
            cands = candidates(path, v["struct"], texts, refl, users[1:])
            rows.append({"path": path, "struct": v["struct"], "placeholders": v["placeholders"],
                         "candidates": cands, "score": prescore(v["placeholders"], cands, refl)})
    return rows


def pick(rows: list[dict], n: int, per_dir: int = 2, keep=lambda r: True) -> list[dict]:
    """The pilot set: structs with the most placeholders at a candidate's member offsets, from directories with
    real class names, at most `per_dir` per directory, each passing `keep`."""
    taken: dict[str, int] = {}
    out = []
    for r in sorted(rows, key=lambda r: (-r["score"], r["path"], r["struct"])):
        d = r["path"].rsplit("/", 1)[0]
        parts = r["path"].split("/")
        unnamed_dir = r["path"].startswith("src/") and (len(parts) < 4 or parts[2].startswith(("fn_", "Unk")))
        if r["score"] < 1 or unnamed_dir or taken.get(d, 0) >= per_dir or not keep(r):
            continue
        taken[d] = taken.get(d, 0) + 1
        out.append(r)
        if len(out) >= n:
            break
    return out


# ---------------------------------------------------------------------------------------------------- requests


def context(view: dict, refl: Reflection, layouts_for=record_layouts) -> dict:
    """Everything a request about one struct carries, and what the checks need afterwards."""
    path, struct = view["path"], view["struct"]
    text = read(path)
    v = next((x for x in find_views(text) if x["struct"] == struct), None)
    if v is None:
        raise SystemExit(f"member_batch: {path} has no view struct {struct}")
    users = users_of(path, struct)
    used, notes = uses(path, struct, users)
    texts = [read(f) for f in users]
    cands = view.get("candidates") or candidates(path, struct, texts, refl, users[1:])
    compiled = next((f for f in users if f.endswith(".cpp")), None)
    layouts = layouts_for(compiled) if compiled else {}
    mt = member_types(layouts, struct)
    sizes = {**(probed_sizes() if layouts_for is record_layouts else {}), **record_sizes(layouts)}
    return {"path": path, "struct": struct, "definition": struct_text(text, v), "placeholders": v["placeholders"],
            "members": mt, "candidates": cands, "uses": used, "notes": notes, "compiled": compiled, "sizes": sizes}


def context_text(ctx: dict, refl: Reflection) -> str:
    parts = [f"View struct {ctx['struct']}, defined in {ctx['path']}:\n" + nb.fence(ctx["path"], ctx["definition"])]
    if ctx["members"]:
        lines = [f"  {m['offset']:#06x}  {m['type']:<28} {name}" + ("   <- placeholder" if name in ctx["placeholders"]
                                                                     else "")
                 for name, m in sorted(ctx["members"].items(), key=lambda kv: kv[1]["offset"])]
        parts.append("Its members as the compiler lays them out (byte offset, type, name):\n" + "\n".join(lines) + "\n")
    else:
        parts.append("(The compiler's layout of the struct was not available; offsets are as the names say.)\n")
    parts.append("Candidate classes, with their reflected members (offset, name, type):\n\n"
                 + "\n\n".join(refl.describe(c) for c in ctx["candidates"]) + "\n")
    if ctx["uses"]:
        parts.append("Sources that use the struct (its defining file whole when it is short, else whole functions):\n"
                     + "\n".join(nb.fence(f"{u['path']}: {u['label']}", u["text"]) for u in ctx["uses"]))
    if ctx["notes"]:
        parts.append("Left out of this request: " + " ".join(ctx["notes"]) + "\n")
    return "\n".join(parts)


def custom_id(prefix: str, i: int, struct: str) -> str:
    return f"{prefix}-{i:03d}-{re.sub(r'[^A-Za-z0-9_-]', '', struct)}"[:64]


def make_params(rules: str, schema: dict, user: str, effort: str, cache: bool) -> dict:
    system = [{"type": "text", "text": rules}]
    if cache and len(rules) / nb.CHARS_PER_TOKEN >= nb.MIN_CACHE_TOKENS:
        system[0]["cache_control"] = nb.cache_control("5m")
    return {"model": nb.MODEL, "max_tokens": nb.MAX_TOKENS, "thinking": nb.THINKING,
            "output_config": {"effort": effort, "format": {"type": "json_schema", "schema": schema}},
            "system": system, "messages": [{"role": "user", "content": [{"type": "text", "text": user}]}]}


def build_propose(views: list[dict], refl: Reflection, effort: str = nb.EFFORT,
                  layouts_for=record_layouts) -> tuple[list[dict], list[dict]]:
    requests, meta = [], []
    for i, view in enumerate(views):
        ctx = context(view, refl, layouts_for)
        if not ctx["candidates"]:
            continue
        ask = (f"Placeholder members to decide: {', '.join(ctx['placeholders'])}.\n"
               f"Answer: class is one of {', '.join(ctx['candidates'])}, or \"none\". members has one entry per "
               "placeholder: reflected is the reflected member's name at that member's offset, or \"\" to keep the "
               "placeholder; evidence is one line of facts in the code (empty when kept). reason: one or two "
               "sentences on why the struct is a view of that class, or why none.")
        user = context_text(ctx, refl) + "\n" + ask
        cid = custom_id("mp", i, ctx["struct"])
        requests.append({"custom_id": cid, "params": make_params(RULES, PROPOSE_SCHEMA, user, effort, len(views) > 1)})
        meta.append({"custom_id": cid, "address": "0x0", "class": "", "notes": ctx["notes"], "path": ctx["path"],
                     "struct": ctx["struct"], "candidates": ctx["candidates"], "placeholders": ctx["placeholders"],
                     "members": ctx["members"], "sizes": ctx["sizes"], "uses": len(ctx["uses"])})
    return requests, meta


def build_review(proposals: list[dict], refl: Reflection, effort: str = nb.EFFORT,
                 layouts_for=record_layouts) -> tuple[list[dict], list[dict]]:
    """One review request per struct the first run mapped: the mapping and its evidence, not its reasoning."""
    requests, meta = [], []
    for i, p in enumerate(proposals):
        mapped = [m for m in p["members"] if m["reflected"]]
        if p["class"] in ("", "none") or not mapped:
            continue
        ctx = context({"path": p["path"], "struct": p["struct"], "candidates": p["candidates"]}, refl, layouts_for)
        lines = [f"  {m['member']} -> {m['reflected']}; evidence: {m['evidence']}" for m in mapped]
        ask = (f"The mapping under review: {ctx['struct']} is a view of {p['class']}.\n" + "\n".join(lines) + "\n"
               "Answer: class_agree, with class_reason (the facts in the code), and one entry in members for each "
               "mapped member above: agree, and a reason citing the code.")
        user = context_text(ctx, refl) + "\n" + ask
        cid = custom_id("mr", i, ctx["struct"])
        requests.append({"custom_id": cid, "params": make_params(REVIEW_RULES, REVIEW_SCHEMA, user, effort,
                                                                 len(proposals) > 1)})
        meta.append({"custom_id": cid, "address": "0x0", "class": p["class"], "notes": ctx["notes"],
                     "path": p["path"], "struct": p["struct"], "candidates": p["candidates"],
                     "placeholders": p["placeholders"], "members": ctx["members"], "mapping": mapped,
                     "from": p["custom_id"]})
    return requests, meta


# ---------------------------------------------------------------------------------------------------- results


def parse(result: dict, kind: str) -> tuple[dict | None, str]:
    """The answer of one batch result, or (None, why): errored, refused, truncated or outside the schema."""
    data, status = nb.answer_of(result)
    if data is None:
        return None, status
    if kind == KIND_PROPOSE:
        ok = isinstance(data.get("class"), str) and isinstance(data.get("members"), list) and all(
            isinstance(m, dict) and isinstance(m.get("member"), str) and isinstance(m.get("reflected"), str)
            for m in data["members"])
    else:
        ok = isinstance(data.get("class_agree"), bool) and isinstance(data.get("members"), list) and all(
            isinstance(m, dict) and isinstance(m.get("member"), str) and isinstance(m.get("agree"), bool)
            for m in data["members"])
    return (data, "ok") if ok else (None, "no verdict: answer outside the schema")


def my_spend() -> tuple[float, float]:
    """(dollars this tool's runs cost in the ledger, worst case of its runs submitted and not yet fetched)."""
    rows = nb.ledger_rows()
    spent = sum(float(r["dollars"]) for r in rows if r["kind"].startswith("member"))
    fetched = {r["run"] for r in rows}
    pending = 0.0
    if nb.STATE.is_dir():
        for b in nb.STATE.glob("*/batch.json"):
            run = b.parent.name
            meta = b.parent / "meta.json"
            if run in fetched or not meta.exists():
                continue
            if json.loads(meta.read_text(encoding="utf-8")).get("kind", "").startswith("member"):
                pending += float(json.loads(b.read_text(encoding="utf-8")).get("worst_dollars", 0))
    return spent, pending


def cmd_submit(args, client=None) -> int:
    d = nb.run_dir(args.run)
    if not (d / "requests.jsonl").exists():
        print(f"member_batch: no requests for {args.run}; run build or build-review first")
        return 1
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    if not info.get("kind", "").startswith("member"):
        print(f"member_batch: {args.run} is not a member run")
        return 1
    est = nb.estimate(nb.read_jsonl(d / "requests.jsonl"), info.get("counted_tokens") or None)
    spent, pending = my_spend()
    total = spent + pending + est["worst_dollars"]
    if total > args.cap:
        print(f"member_batch: REFUSED: this tool's ledger ${spent:.2f} + its pending batches ${pending:.2f} + this "
              f"batch's worst case ${est['worst_dollars']:.2f} = ${total:.2f}, over its cap of ${args.cap:.2f}. "
              "Nothing was sent.")
        return 2
    return nb.cmd_submit(argparse.Namespace(run=args.run, budget=args.budget), client=client)


def cmd_fetch(args, client=None) -> int:
    d = nb.run_dir(args.run)
    bid = nb.batch_id(args.run)
    info = json.loads((d / "meta.json").read_text(encoding="utf-8"))
    kind = info["kind"]
    client = client or nb.make_client()
    b = client.messages.batches.retrieve(bid)
    if b.processing_status != "ended":
        print(f"{args.run}: {bid} is {b.processing_status}; fetch once it has ended")
        return 1
    raw = [nb.as_dict(r) for r in client.messages.batches.results(bid)]
    with (d / "results.jsonl").open("w", encoding="utf-8", newline="\n") as fh:
        for r in raw:
            fh.write(json.dumps(r, sort_keys=True) + "\n")
    by_id = {r["custom_id"]: r for r in raw}
    total = {"input": 0, "write": 0, "read": 0, "output": 0, "write_1h": 0}
    cost, good, succeeded = 0.0, 0, 0
    answers = []
    for m in info["items"]:
        r = by_id.get(m["custom_id"])
        if r is None:
            data, status = None, "missing from the results"
        else:
            data, status = parse(r, kind)
            if (r.get("result") or {}).get("type") == "succeeded":
                succeeded += 1
                u = nb.usage_of(r)
                for k in total:
                    total[k] += u[k]
                cost += nb.dollars(u)
        good += data is not None
        answers.append({**{k: m[k] for k in ("custom_id", "path", "struct", "candidates", "placeholders")},
                        "status": status, "answer": data})
    (d / "answers.json").write_text(json.dumps(answers, indent=1), encoding="utf-8")
    row = {"run": args.run, "kind": kind, "batch_id": bid, "fetched": time.strftime("%Y-%m-%dT%H:%M:%S"),
           "requests": len(info["items"]), "succeeded": succeeded, "verdicts": good,
           "input_tokens": total["input"], "cache_write_tokens": total["write"], "cache_read_tokens": total["read"],
           "output_tokens": total["output"], "cache_reads_happened": "yes" if total["read"] else "no",
           "dollars": f"{cost:.4f}"}
    nb.append_ledger(row)
    print(f"{args.run}: {len(raw)} results, {good} with an answer; {d / 'answers.json'}")
    print(f"  tokens: input {total['input']:,}, cache write {total['write']:,}, cache read {total['read']:,}, "
          f"output {total['output']:,}; ${cost:.2f}")
    return 0


def load_answers(run: str) -> list[dict]:
    p = nb.run_dir(run) / "answers.json"
    if not p.exists():
        raise SystemExit(f"member_batch: {run} has no answers.json (fetch it first)")
    return json.loads(p.read_text(encoding="utf-8"))


def proposals_of(run: str) -> list[dict]:
    out = []
    for a in load_answers(run):
        if a["answer"]:
            out.append({**a, "class": a["answer"]["class"].strip(), "members": a["answer"]["members"]})
    return out


def agree(proposals: list[dict], reviews: list[dict], refl: Reflection, members_of,
          sizes_of=lambda p: {}) -> tuple[list[dict], list[dict]]:
    """(rows to apply, members left with why). A member is applied only when the review agrees with the class and
    with it, the reflected name is the chosen class's member at the compiler's offset of the placeholder exactly,
    and the types are compatible. `members_of(proposal)` gives the struct's member types (member_types),
    `sizes_of(proposal)` the record sizes of its translation unit."""
    by_struct = {(r["path"], r["struct"]): r for r in reviews}
    rows, left = [], []
    for p in proposals:
        cls = p["class"]
        if cls in ("", "none"):
            continue
        rev = by_struct.get((p["path"], p["struct"]))
        rans = (rev or {}).get("answer") or {}
        votes = {m["member"]: m for m in rans.get("members", [])}
        mt = members_of(p)
        names = set(mt)
        used_names: set[str] = set()
        for m in p["members"]:
            ph, name = m["member"], m["reflected"].strip()
            if not name:
                continue

            def leave(why):
                left.append({"path": p["path"], "struct": p["struct"], "placeholder": ph, "name": name,
                             "class": cls, "why": why})

            if not rans:
                leave("no review answer")
                continue
            if not rans.get("class_agree"):
                leave("review disagrees with the class")
                continue
            if not votes.get(ph, {}).get("agree"):
                leave("review disagrees with the member")
                continue
            if not m.get("evidence", "").strip():
                leave("no evidence stated")
                continue
            if cls not in p["candidates"] or ph not in p["placeholders"]:
                leave("class not a candidate, or member not a placeholder")
                continue
            if ph not in mt:
                leave("no compiler layout for the member")
                continue
            hit = [x for x in refl.known(cls) if x[1] == name]
            if not hit:
                leave("not a reflected member of the class with a known offset")
                continue
            off, _, rtype, _ = hit[0]
            v = mt[ph]
            if off != v["offset"]:
                leave(f"offset differs: reflected {off:#x}, view {v['offset']:#x}")
                continue
            if not compatible(rtype, v["kind"], v["size"], refl, v["record"], sizes_of(p)):
                leave(f"type not compatible: reflected {rtype}, view {v['type']}")
                continue
            if name in names or name in used_names:
                leave("the struct already has a member of that name")
                continue
            used_names.add(name)
            rows.append({"path": p["path"], "struct": p["struct"], "placeholder": ph, "name": name, "class": cls,
                         "offset": f"{off:#x}", "reflected_type": rtype, "view_type": v["type"],
                         "evidence": m.get("evidence", "").replace("\n", " ").strip()})
    return rows, left


def read_table(path: Path | None = None) -> list[dict]:
    path = path or TABLE
    if not path.is_file():
        return []
    with path.open(newline="", encoding="utf-8") as f:
        return list(csv.DictReader(f))


def write_table(rows: list[dict], path: Path | None = None) -> None:
    path = path or TABLE
    rows = sorted(rows, key=lambda r: (r["path"], r["struct"], int(r["offset"], 16)))
    with path.open("w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=TABLE_COLUMNS, lineterminator="\n")
        w.writeheader()
        w.writerows(rows)


# ---------------------------------------------------------------------------------------------------- the rename


ERROR = re.compile(r"^(.+?):(\d+):(\d+): error: (.*)$", re.M)


def rename_in_struct(text: str, struct: str, renames: dict[str, str]) -> str | None:
    """The text with the struct's direct members renamed, or None when the struct or a member is not found."""
    v = next((x for x in find_views(text) if x["struct"] == struct), None)
    if v is None or not set(renames) <= set(v["placeholders"]):
        return None
    m = nb.mask(text)
    sc = nb.Scopes(m)
    edits = []
    for old, new in renames.items():
        for p in nb.words(m[v["open"]:v["close"]], old):
            pos = v["open"] + p
            inner = sc.enclosing(pos)
            if inner and inner[0] == v["open"]:
                edits.append((pos, old, new))
    for pos, old, new in sorted(edits, reverse=True):
        text = text[:pos] + new + text[pos + len(old):]
    return text


def compile_errors(paths: list[str], runner=None) -> list[tuple[str, int, int, str]]:
    runner = runner or (lambda cmd: subprocess.run(cmd, cwd=ROOT, capture_output=True, text=True,
                                                   encoding="utf-8", errors="replace"))
    out = []
    for p in paths:
        res = runner(compile_cmd(p, ["-fsyntax-only", "-ferror-limit=0", "-fno-spell-checking",
                                     "-fno-caret-diagnostics", "-fno-color-diagnostics"]))
        errs = [(f.replace("\\", "/"), int(ln), int(col), msg) for f, ln, col, msg in ERROR.findall(res.stderr)]
        if res.returncode != 0 and not errs:
            errs = [(p, 0, 0, "compile failed without a located error")]
        out += errs
    return out


def rel_path(f: str) -> str:
    p = Path(f)
    try:
        return (p if p.is_absolute() else (ROOT / p)).resolve().relative_to(ROOT.resolve()).as_posix()
    except ValueError:
        return f


def apply_group(path: str, struct: str, renames: dict[str, str], compile_paths: list[str], errors=compile_errors,
                max_rounds: int = 6) -> tuple[bool, str, dict[str, str]]:
    """Rename a struct's members and every use the compiler reports. Returns (ok, why, {file: original text})
    and leaves the files as they were when not ok."""
    originals: dict[str, str] = {}
    text = read(path)
    new = rename_in_struct(text, struct, renames)
    if new is None:
        return False, "struct or member not found", {}
    originals[path] = text
    (ROOT / path).write_bytes(new.encode("utf-8"))
    olds = set(renames)

    def restore(why):
        for f, t in originals.items():
            (ROOT / f).write_bytes(t.encode("utf-8"))
        return False, why, {}

    for _ in range(max_rounds):
        errs = errors(compile_paths)
        if not errs:
            return True, "ok", originals
        fixes: dict[str, set[tuple[int, int, str]]] = {}
        for f, ln, col, msg in errs:
            named = re.findall(r"'([^']*)'", msg)
            old = next((n for n in named if n in olds), None)
            if old is None:
                return restore(f"unrelated error: {f}:{ln}: {msg}")
            if " in '" in msg:
                where = msg.rsplit(" in '", 1)[1].rstrip("'")
                where = re.sub(r"^(const |volatile )+", "", where).strip()
                if not (where == struct or where.endswith("::" + struct)):
                    return restore(f"error names another type: {f}:{ln}: {msg}")
            fixes.setdefault(rel_path(f), set()).add((ln, col, old))
        for f, spots in fixes.items():
            if f.startswith(nb.GENERATED) or f.startswith("include/reflected/") or not (ROOT / f).is_file():
                return restore(f"a use is in a file not to be edited: {f}")
            cur = originals.get(f) or read(f)
            originals.setdefault(f, cur)
            lines = read(f).split("\n")
            for ln, col, old in sorted(spots, reverse=True):
                line = lines[ln - 1]
                at = col - 1
                if line[at:at + len(old)] != old:
                    # clang places an offsetof's error on the offsetof itself: the member is its second argument
                    om = re.match(r"(?:__builtin_)?offsetof\s*\(\s*[\w:]+\s*,\s*(\w+)\s*\)", line[at:])
                    if not om or om.group(1) != old:
                        return restore(f"{f}:{ln}:{col} does not spell {old} (a macro?)")
                    at += om.start(1)
                lines[ln - 1] = line[:at] + renames[old] + line[at + len(old):]
            (ROOT / f).write_bytes("\n".join(lines).encode("utf-8"))
    return restore("still does not compile after the last round")


def includers(path: str) -> list[str]:
    if path.endswith(".cpp"):
        return [path]
    import baseline
    return sorted(baseline.expand({path}))


def cmd_apply(args) -> int:
    rows = read_table()
    groups: dict[tuple[str, str], dict[str, str]] = {}
    for r in rows:
        groups.setdefault((r["path"], r["struct"]), {})[r["placeholder"]] = r["name"]
    log = []
    done = skipped = failed = 0
    for (path, struct), renames in sorted(groups.items()):
        if args.only and f"{path}:{struct}" not in args.only:
            continue
        if not (ROOT / path).is_file():
            print(f"  skip {path}:{struct}: the file is gone")
            skipped += 1
            continue
        v = next((x for x in find_views(read(path)) if x["struct"] == struct), None)
        todo = {o: n for o, n in renames.items() if v and o in v["placeholders"]}
        if not todo:
            print(f"  skip {path}:{struct}: nothing left to rename")
            skipped += 1
            continue
        ok, why, originals = apply_group(path, struct, todo, includers(path))
        log.append({"path": path, "struct": struct, "renames": todo, "ok": ok, "why": why,
                    "files": sorted(originals)})
        if ok:
            done += 1
            print(f"  {path}:{struct}: {len(todo)} renamed in {len(originals)} file(s)")
        else:
            failed += 1
            print(f"  FAILED {path}:{struct}: {why}")
    out = nb.STATE / "member_apply.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(log, indent=1), encoding="utf-8")
    print(f"member_batch apply: {done} struct(s) renamed, {skipped} skipped, {failed} failed; {out}")
    print("  next: python tools/objsame.py save --tag members --base origin/main && "
          "python tools/objsame.py compare --tag members")
    return 1 if failed else 0


# ---------------------------------------------------------------------------------------------------- commands


def cmd_scan(args) -> int:
    refl = Reflection()
    rows = scan(refl)
    hit = [r for r in rows if r["score"]]
    print(f"{len(rows)} view structs with placeholder members, {sum(len(r['placeholders']) for r in rows)} "
          f"placeholders; {len(hit)} structs have {sum(r['score'] for r in hit)} placeholders at a candidate "
          f"class's reflected offset")
    for r in sorted(hit, key=lambda r: -r["score"])[:args.top]:
        print(f"  {r['score']:3}  {r['path']}:{r['struct']}  [{', '.join(r['candidates'])}]")
    out = nb.STATE / "member_scan.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(rows, indent=1), encoding="utf-8")
    print(f"  {out}")
    return 0


def cmd_build(args, client=None) -> int:
    refl = Reflection()
    if args.structs:
        views = []
        for ln in Path(args.structs).read_text(encoding="utf-8").splitlines():
            ln = ln.split("#", 1)[0].strip()
            if ln:
                path, struct = ln.rsplit(":", 1)
                views.append({"path": path, "struct": struct})
    else:
        views = pick(scan(refl), args.select, keep=lambda r: not generated_mentions(r["struct"]))
    requests, meta = build_propose(views, refl, args.effort)
    info = nb.write_run(args.run, KIND_PROPOSE, requests, meta, {"ttl": "5m", "effort": args.effort}, client,
                        args.dry_run)
    nb.print_summary(args.run, info)
    return 0


def cmd_build_review(args, client=None) -> int:
    refl = Reflection()
    requests, meta = build_review(proposals_of(getattr(args, "from")), refl, args.effort)
    if not requests:
        print("member_batch: nothing was mapped; no review to build")
        return 1
    info = nb.write_run(args.run, KIND_REVIEW, requests, meta, {"ttl": "5m", "effort": args.effort,
                                                                "from_run": getattr(args, "from")}, client,
                        args.dry_run)
    nb.print_summary(args.run, info)
    return 0


def cmd_agree(args) -> int:
    refl = Reflection()
    props = proposals_of(args.propose)
    revs = load_answers(args.review)
    meta = {m["custom_id"]: m for m in json.loads((nb.run_dir(args.propose) / "meta.json").read_text(
        encoding="utf-8"))["items"]}
    rows, left = agree(props, revs, refl, lambda p: meta[p["custom_id"]]["members"],
                       lambda p: meta[p["custom_id"]].get("sizes", {}))
    mapped = [p for p in props if p["class"] not in ("", "none")]
    print(f"{len(props)} answered, {len(mapped)} mapped to a class, "
          f"{sum(1 for p in mapped for m in p['members'] if m['reflected'])} members proposed; "
          f"{len(rows)} agreed and checked, {len(left)} left")
    whys: dict[str, int] = {}
    for x in left:
        k = re.sub(r":.*", "", x["why"])
        whys[k] = whys.get(k, 0) + 1
    for k, n in sorted(whys.items(), key=lambda kv: -kv[1]):
        print(f"  left {n:3}: {k}")
    out = nb.run_dir(args.review) / "agree.json"
    out.write_text(json.dumps({"rows": rows, "left": left}, indent=1), encoding="utf-8")
    if args.write:
        keep = [r for r in read_table() if (r["path"], r["struct"], r["placeholder"]) not in
                {(x["path"], x["struct"], x["placeholder"]) for x in rows}]
        write_table(keep + rows)
        print(f"  {TABLE.relative_to(ROOT).as_posix()}: {len(keep) + len(rows)} rows")
    return 0


def cmd_cost(args) -> int:
    rows = [r for r in nb.ledger_rows() if r["kind"].startswith("member")]
    for r in rows:
        print(f"{r['run']:20} {r['kind']:15} {r['requests']:>4} req  ${float(r['dollars']):.2f}")
    spent, pending = my_spend()
    total = sum(float(r["dollars"]) for r in nb.ledger_rows())
    print(f"this tool ${spent:.2f} (pending worst ${pending:.2f}, cap ${DEFAULT_CAP:.0f}); ledger total ${total:.2f}")
    return 0


def main(argv: list[str] | None = None) -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    sub = ap.add_subparsers(dest="cmd", required=True)
    p = sub.add_parser("scan")
    p.add_argument("--top", type=int, default=60)
    for name in ("build", "build-review"):
        p = sub.add_parser(name)
        p.add_argument("--run", required=True)
        p.add_argument("--dry-run", action="store_true", help="write the requests and estimate; no API contact")
        p.add_argument("--effort", default=nb.EFFORT, choices=["low", "medium", "high", "xhigh", "max"])
        if name == "build":
            g = p.add_mutually_exclusive_group(required=True)
            g.add_argument("--select", type=int, help="the N best-scored structs (scan)")
            g.add_argument("--structs", help="a file of path:Struct lines")
        else:
            p.add_argument("--from", required=True, help="the propose run")
    p = sub.add_parser("submit")
    p.add_argument("--run", required=True)
    p.add_argument("--cap", type=float, default=DEFAULT_CAP, help="this tool's own spend cap in dollars")
    p.add_argument("--budget", type=float, default=nb.DEFAULT_BUDGET, help="the shared ledger's budget")
    for name in ("status", "fetch"):
        sub.add_parser(name).add_argument("--run", required=True)
    p = sub.add_parser("agree")
    p.add_argument("--propose", required=True)
    p.add_argument("--review", required=True)
    p.add_argument("--write", action="store_true", help="merge the rows into data/member_names.csv")
    p = sub.add_parser("apply")
    p.add_argument("--only", nargs="*", help="path:Struct groups to apply")
    sub.add_parser("cost")
    args = ap.parse_args(argv)
    return {"scan": cmd_scan, "build": cmd_build, "build-review": cmd_build_review, "submit": cmd_submit,
            "status": nb.cmd_status, "fetch": cmd_fetch, "agree": cmd_agree, "apply": cmd_apply,
            "cost": cmd_cost}[args.cmd](args)


if __name__ == "__main__":
    raise SystemExit(main())
