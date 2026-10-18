#!/usr/bin/env python3
"""Generate src/clones/: explicit instantiations of hand-matched templates for repeated functions.

Many functions in main are copies of one template (instantiations whose code doesn't depend on
the template argument, or only on a size; the linker didn't fold them). One hand-matched
template covers all of them:

  1. Each kind in KINDS names a verified reference address and the template that matches it.
     The template lives in a hand-written header; the instantiation names a tag type
     `UnkClone_fn_<addr>`, so every copy gets its own symbol.
  2. Candidates are the functions whose code equals the reference's: instruction for
     instruction with branch targets inside the function made relative ("exact"), or mnemonic
     for mnemonic ("shape", for kinds that read their constants from the candidate's code).
     Reference code is read from the binary at run time; nothing derived from code is stored here.
  3. All candidates are compiled in one scratch translation unit; only byte-identical
     functions are written out.

It skips functions already defined elsewhere in src/ (the scan tools/gen_lift.py uses), and
gen_lift.py skips the tags written here, so the two can run in either order.

Usage: python tools/gen_clones.py --version 2.1.0
Outputs are deterministic. Regenerate, never edit them.
"""

from __future__ import annotations

import argparse
import csv
import re
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path
from typing import Callable

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "clones"
CHUNK = 1000  # instantiations per generated file
BRANCHES = {"b", "bl", "cbz", "cbnz", "tbz", "tbnz", "adr"}
TARGET_RE = re.compile(r"#0x[0-9a-f]{6,}")

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
Insns = list[tuple[str, str]]  # (mnemonic, operands)


@dataclass
class Kind:
    # Verified reference address, or None for a probe kind: its shape (mnemonics) comes from
    # compiling the instantiation with the tag UnkProbe_<kind>, so templates matched for one
    # element type can find their instances for another.
    ref: int | None
    header: str  # where the template is defined
    # The explicit instantiation for one candidate, given its tag and instructions (None to skip).
    instantiate: Callable[[str, Insns], str | None]
    exact: bool = True  # match whole instructions (else mnemonics only)
    # A scan kind (ref None) tries every candidate: scan(orig, addr, tag, instructions) returns
    # the declarations and instantiation, or None when the function doesn't fit.
    scan: Callable[[asmdiff.Original, int, str, Insns], str | None] | None = None


def elem_size(insns: Insns) -> int | None:
    """The element size of an UnkVector method: 1 << the shift of the allocation size."""
    for m, o in insns:
        hit = re.fullmatch(r"lsl x1, x8, #(\d)", f"{m} {o}")
        if hit:
            return 1 << int(hit.group(1))
    return None


# Element types for collection templates (see UnkClone.h): a pointer, trivially copyable bytes
# (sized from the code), an owning pointer and a copyable handle.
ELEMENTS = {"ptr": "{tag}*", "elem": "UnkCloneElem<{tag}, {size}>", "owner": "UnkCloneOwner<{tag}>",
            "handle": "UnkCloneHandle<{tag}>"}

# Collection templates: header, explicit instantiation ({T} is the element type), verified
# reference per element type, and whether the element size is read from the code ("elem" only).
# Element types without a reference become probe kinds.
COLLECTIONS = {
    "vector_grow_at": ("base/UnkVector.h", "template {T}& base::UnkVector<{T}>::GrowAt(u32);",
                       {"elem": 0x71000305D8, "owner": 0x71002B9ED8, "handle": 0x7100020CD0}),
    "vector_insert": ("base/UnkVector.h", "template void base::UnkVector<{T}>::Insert({T}&&, u32);",
                      {"elem": 0x71000D37D8, "owner": 0x71002B9C74}),
    "vector_free": ("base/UnkVector.h", "template void base::UnkVector<{T}>::Free();", {"owner": 0x71002B9B2C}),
    "vector_move": ("base/UnkVector.h",
                    "template base::UnkVector<{T}>& base::UnkVector<{T}>::operator=(base::UnkVector<{T}>&&);",
                    {"owner": 0x71002B9BBC}),
    "iter_assign": ("base/UnkCollectionIter.h",
                    "template base::UnkCollectionIter<{T}>& base::UnkCollectionIter<{T}>::operator=("
                    "const base::UnkCollectionIter<{T}>&);", {"ptr": 0x71000BC1B0, "owner": 0x71002B8E3C}),
    "iter_value_equal": ("base/UnkCollectionIter.h",
                         "template bool base::operator==(const base::UnkIterValue<{T}>&, "
                         "const base::UnkIterValue<{T}>&);", {"ptr": 0x71000CD4CC}),
    "iter_value_move": ("base/UnkCollectionIter.h",
                        "template base::UnkIterValue<{T}>& base::UnkIterValue<{T}>::operator=("
                        "base::UnkIterValue<{T}>&&);", {"owner": 0x71002B954C}),
    "iter_begin": ("base/UnkCollectionIter.h",
                   "template void base::refl_Begin(const base::UnkVector<{T}>*, base::UnkCollectionIter<{T}>*);",
                   {"ptr": 0x71000CC564, "owner": 0x71002BA12C}),
    "iter_end": ("base/UnkCollectionIter.h",
                 "template void base::refl_End(const base::UnkVector<{T}>*, base::UnkCollectionIter<{T}>*);",
                 {"owner": 0x71002BA230}),
}


def collection(fmt: str, element: str) -> Callable[[str, Insns], str | None]:
    def build(tag: str, insns: Insns) -> str | None:
        size = 8
        if element == "elem":
            size = elem_size(insns)
            if size is None:
                return None
        return fmt.format(T=ELEMENTS[element].format(tag=tag, size=size))
    return build


BASE = 0x7100000000
RELEASE = 0x71000803A4  # string pool release
OPERATOR_DELETE = 0x7100083C00
VTABLE_RE = re.compile(r"x(\d+), \[x\1, #(0x[0-9a-f]+)\]")


def destructor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """A destructor that stores a vtable, releases string-id members and calls its base
    (UnkDestructors.h). Facts: the vtable (through the GOT), the member offsets in release
    order, the base destructor and, for the deleting form, the free function."""
    if not any(m == "str" and o.endswith(", [x0]") for m, o in insns[:10]):
        return None
    pages: dict[str, int] = {}
    vtable = None
    members: list[str] = []
    calls: list[int] = []
    for i, (m, o) in enumerate(insns[:-1]):
        regs = [r.strip() for r in o.split(",")]
        if m == "adrp":
            pages[regs[0]] = int(regs[1][1:], 16)
        elif m == "ldr" and vtable is None and (hit := VTABLE_RE.fullmatch(o)) and f"x{hit.group(1)}" in pages:
            got = pages[f"x{hit.group(1)}"] + int(hit.group(2), 16)
            nxt = insns[i + 1]
            if nxt == ("add", f"x{hit.group(1)}, x{hit.group(1)}, #0x10"):
                target = orig.img.pointer(got - BASE)
                if target is None:
                    return None
                vtable = BASE + target
        elif m == "ldr" and (hit := re.fullmatch(r"(x\d+), \[x(?:0|19), #(0x[0-9a-f]+)\]", o)) \
                and any(x == ("cbz", f"{hit.group(1)}, {t}") for x in insns[i + 1:i + 3]
                        for t in [x[1].split(", ")[-1]]):
            # A refcounted pointer's count is decremented with subs; a string id's with sub
            # (after a compare with the pool's empty string).
            after = [x[0] for x in insns[i + 1:i + 7]]
            kind = "UnkRefPtrAt" if "subs" in after and "sub" not in after else "UnkStrIdAt"
            members.append(f"base::{kind}<{int(hit.group(2), 16):#x}>")
        elif m in ("bl", "b") and o.startswith("#0x"):
            t = int(o[1:], 16)
            if t != RELEASE and t not in calls:
                calls.append(t)
    m, o = insns[-1]
    if m == "b" and o.startswith("#0x") and int(o[1:], 16) not in calls:
        calls.append(int(o[1:], 16))
    deleting = OPERATOR_DELETE in calls
    bases = [c for c in calls if c != OPERATOR_DELETE]
    if vtable is None or len(bases) > 1 or not (bases or deleting) or len(set(members)) != len(members):
        return None
    lines = [f"extern char g_unk_{vtable:x}[];"] + [f"void fn_{c:x}(char*);" for c in bases]
    lines += [f"template <> struct base::UnkDtorTraits<{tag}> {{",
              f"    static char* vtable() {{ return g_unk_{vtable:x}; }}"]
    if bases:
        lines.append(f"    static void base(char* p) {{ fn_{bases[0]:x}(p); }}")
    fn = "refl_Dtor" if not deleting else "refl_DeletingDtor" if bases else "refl_DeletingDtorNoBase"
    lines += ["};", f"template void base::{fn}<{tag}{''.join(', ' + x for x in members)}>(char*);"]
    return "\n".join(lines)


MAKE_VALUE = 0x71000A18D4  # builds a reflected value from a type and a pointer
LOAD_TYPES = {("ldr", "w"): "u32", ("ldr", "x"): "u64", ("ldrb", "w"): "u8", ("ldrh", "w"): "u16",
              ("ldr", "s"): "f32", ("ldr", "d"): "f64"}


def field_value(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """A property read (UnkReflValue.h): the loaded type and offset and the type getter."""
    if len(insns) > 24 or ("mov", "x19, x8") not in insns[:6] or ("ldr", "x8, [x1, #8]") not in insns[:8]:
        return None
    calls = [int(o[1:], 16) for m, o in insns if m == "bl"]
    if len(calls) != 2 or calls[1] != MAKE_VALUE:
        return None
    for m, o in insns:
        if (hit := re.fullmatch(r"([wxsd])\d+, \[x8(?:, #(0x[0-9a-f]+|\d+))?\]", o)) and (m, hit.group(1)) in LOAD_TYPES:
            t, off = LOAD_TYPES[(m, hit.group(1))], int(hit.group(2) or "0", 0)
            break
    else:
        return None
    return "\n".join([f"const base::reflection::CType* fn_{calls[0]:x}();",
                      f"template <> struct base::reflection::UnkFieldTraits<{tag}> {{",
                      f"    static const CType* type() {{ return fn_{calls[0]:x}(); }}", "};",
                      f"template base::reflection::UnkReflValue base::reflection::refl_GetFieldValue<{tag}, {t}, "
                      f"{off:#x}>(void*, char*);"])


KINDS = {
    "dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=destructor),
    "field_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_value),
    "register_named": Kind(0x7100072940, "base/UnkNamedRegistry.h", lambda tag, _: (
        f"template base::UnkNamedObject* base::refl_RegisterNamed<{tag}>("
        "base::UnkNamedRegistry*, base::UnkNamedObject*, const char*);")),
    "id_pair_assign": Kind(0x7100B96DEC, "base/UnkIdPair.h", lambda tag, _: (
        f"template base::UnkIdPair<{tag}>& base::UnkIdPair<{tag}>::operator=(const base::UnkIdPair<{tag}>&);")),
    "deferred_add": Kind(0x71002AEEBC, "base/UnkDeferredSet.h", lambda tag, _: (
        f"template void base::UnkDeferredSet<{tag}>::Add({tag}*);")),
    "deferred_remove": Kind(0x71002AE87C, "base/UnkDeferredSet.h", lambda tag, _: (
        f"template void base::UnkDeferredSet<{tag}>::Remove({tag}*);")),
}
# Verified kinds first, so that a function matching both a reference and a probe takes the
# reference's instantiation.
for _name, (_header, _fmt, _refs) in COLLECTIONS.items():
    for _el, _ref in _refs.items():
        KINDS[f"{_name}_{_el}"] = Kind(_ref, _header, collection(_fmt, _el), exact=_el != "elem")
for _name, (_header, _fmt, _refs) in COLLECTIONS.items():
    trivial = "ptr" in _refs or "elem" in _refs
    for _el in ("owner", "handle") + (() if trivial else ("ptr",)):
        if _el not in _refs:
            KINDS[f"{_name}_{_el}"] = Kind(None, _header, collection(_fmt, _el), exact=False)


def instructions(orig: asmdiff.Original, addr: int) -> Insns:
    """The function's instructions, with branch targets inside it relative to its start."""
    end = addr + orig.sizes[addr]
    out = []
    for _, _, m, o in CS.disasm_lite(orig.function(addr), addr):
        if m in BRANCHES or m.startswith("b."):
            def rel(t: re.Match) -> str:
                v = int(t.group(0)[1:], 16)
                return f"@{v - addr:+x}" if addr <= v < end else t.group(0)
            o = TARGET_RE.sub(rel, o)
        out.append((m, o))
    return out


def defined_in_src() -> set[int]:
    """Addresses already defined by sources outside src/clones/ (the scan tools/gen_lift.py uses)."""
    import gen_lift  # noqa: E402
    skip = set()
    for f in (ROOT / "src").rglob("*.cpp"):
        if OUT in f.parents:
            continue
        text = f.read_text(encoding="utf-8")
        for line in text.splitlines():
            m = gen_lift.NAME_RE.match(line)
            if m and ("{" in line or not line.rstrip().endswith(";")):
                skip.add(int(m.group(1), 16))
        skip |= {int(a, 16) for a in gen_lift.TAG_RE.findall(text)}
    return skip


PRELUDE = ['#include "UnkClone.h"']
WORK = ROOT / "build" / "gen_clones"


def probe_shapes() -> dict[str, tuple[int, list[str]]]:
    """Size and mnemonics of each probe kind's instantiation, compiled with the project flags."""
    probes = {name: k for name, k in KINDS.items() if k.ref is None and not k.scan}
    if not probes:
        return {}
    lines = PRELUDE + [f'#include "{h}"' for h in sorted({k.header for k in probes.values()})]
    for name, k in probes.items():
        lines += [f"struct UnkProbe_{name};", k.instantiate(f"UnkProbe_{name}", [("lsl", "x1, x8, #3")])]
    WORK.mkdir(parents=True, exist_ok=True)
    src, obj = WORK / "probes.cpp", WORK / "probes.o"
    src.write_text("\n".join(lines) + "\n", encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"probes failed to compile:\n{r.stderr[:4000]}")
    out = {}
    for sym, code, _ in asmdiff.object_functions(obj):
        for name in probes:
            if f"UnkProbe_{name}" in sym and (name not in out or len(code) > out[name][0]):
                out[name] = (len(code), [m for _, _, m, _ in CS.disasm_lite(code, 0)])
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()
    orig = asmdiff.Original(args.version)

    rows = []
    for f in sorted((ROOT / "data" / "functions").glob("main_*.csv")):
        with f.open(newline="", encoding="utf-8") as fh:
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O")]
    taken = defined_in_src()
    probes = probe_shapes()
    refs = {}  # in KINDS order: verified kinds before probes
    for name, k in KINDS.items():
        if k.ref is not None:
            ins = instructions(orig, k.ref)
            refs[name] = (orig.sizes[k.ref], ins if k.exact else [m for m, _ in ins])
        elif name in probes:
            refs[name] = probes[name]
    found: dict[int, tuple[str, str]] = {}  # address -> (kind, instantiation)
    for a in rows:
        if a in taken or a not in orig.sizes:
            continue
        ins = None
        for name, (size, code) in refs.items():
            if orig.sizes[a] != size:
                continue
            ins = ins or instructions(orig, a)
            if (ins if KINDS[name].exact and KINDS[name].ref is not None else [m for m, _ in ins]) == code:
                tag = f"UnkClone_fn_{a:x}"
                line = KINDS[name].instantiate(tag, ins)
                if line:
                    found[a] = (name, f"struct {tag};\n{line}")
                break
        if a not in found:
            ins = ins or instructions(orig, a)
            for name, k in KINDS.items():
                if k.scan and (block := k.scan(orig, a, f"UnkClone_fn_{a:x}", ins)):
                    found[a] = (name, f"struct UnkClone_fn_{a:x};\n{block}")
                    break

    def includes(addrs: list[int]) -> list[str]:
        return PRELUDE + [f'#include "{h}"' for h in sorted({KINDS[found[a][0]].header for a in addrs})]

    WORK.mkdir(parents=True, exist_ok=True)
    src, obj = WORK / "candidates.cpp", WORK / "candidates.o"
    src.write_text("\n".join(includes(list(found)) + [found[a][1] for a in sorted(found)]) + "\n",
                   encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"candidates failed to compile:\n{r.stderr[:4000]}")
    ok = sorted({res.address for res in asmdiff.diff_object(obj, orig) if res.matching} & set(found))

    if OUT.exists():
        shutil.rmtree(OUT)
    OUT.mkdir(parents=True)
    note = ["// Generated by tools/gen_clones.py from main; regenerate rather than edit.",
            "// Explicit instantiations of hand-matched templates for repeated functions,",
            "// byte-identical to the original."]
    for k in range(0, len(ok), CHUNK):
        part = ok[k:k + CHUNK]
        (OUT / f"clones_{k // CHUNK:02d}.cpp").write_text(
            "\n".join(note + includes(part) + [found[a][1] for a in part]) + "\n", encoding="utf-8", newline="\n")
    counts = {name: (sum(1 for a in found if found[a][0] == name), sum(1 for a in ok if found[a][0] == name))
              for name in KINDS}
    print(f"{len(ok)} of {len(found)} candidates byte-identical ({sum(orig.sizes[a] for a in ok)} bytes); "
          f"per kind (candidates, matching): {counts}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
