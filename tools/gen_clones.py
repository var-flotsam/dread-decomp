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
R_AARCH64_RELATIVE = 1027

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
            "handle": "UnkCloneHandle<{tag}>", "word": "UnkCloneWord<{tag}>", "strid": "UnkCloneStrId<{tag}>",
            # Two template arguments: a word element keyed by a string id (keyed collections).
            "word_strkey": "UnkCloneWord<{tag}>, UnkCloneStrId<{tag}>", "strid_b": "UnkCloneStrIdB<{tag}>"}

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
    "iter_value_equal_by_value": ("base/UnkCollectionIter.h",
                                  "template bool base::refl_IterValueEqualByValue(const base::UnkIterValue<{T}>&, "
                                  "const base::UnkIterValue<{T}>&);", {"word": 0x71000CEA70, "word_strkey": 0x7100120684}),
    "iter_value_move": ("base/UnkCollectionIter.h",
                        "template base::UnkIterValue<{T}>& base::UnkIterValue<{T}>::operator=("
                        "base::UnkIterValue<{T}>&&);", {"owner": 0x71002B954C}),
    "iter_value_move_construct": ("base/UnkCollectionIter.h",
                                  "template void base::refl_IterValueMove<{T}>(void*, const void*);",
                                  {"word": 0x71000CEB44, "owner": 0x71002433A0}),
    "iter_value_destroy": ("base/UnkCollectionIter.h", "template void base::refl_IterValueDestroy<{T}>(void*);",
                           {"strid": 0x71000CD5D8, "strid_b": 0x71000CFD94, "word_strkey": 0x71000DE4F4}),
    "iter_value_copy": ("base/UnkCollectionIter.h",
                        "template void* base::refl_IterValueCopy<{T}>(void*, const void*);", {"word": 0x71000CEBA4}),
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
    order, the base destructor and, for the deleting form, the free function. A second vtable
    store is an inlined base destructor: the members after it are the base's."""
    if not any(m == "str" and o.endswith(", [x0]") for m, o in insns[:10]):
        return None
    pages: dict[str, int] = {}
    vtables: list[tuple[int, int]] = []  # (vtable, members released before its store)
    members: list[str] = []
    calls: list[int] = []
    for i, (m, o) in enumerate(insns[:-1]):
        regs = [r.strip() for r in o.split(",")]
        if m == "adrp":
            pages[regs[0]] = int(regs[1][1:], 16)
        elif m == "ldr" and len(vtables) < 2 and (hit := VTABLE_RE.fullmatch(o)) and f"x{hit.group(1)}" in pages:
            got = pages[f"x{hit.group(1)}"] + int(hit.group(2), 16)
            nxt = insns[i + 1]
            if nxt == ("add", f"x{hit.group(1)}, x{hit.group(1)}, #0x10"):
                target = orig.img.pointer(got - BASE)
                if target is None:
                    return None
                vtables.append((BASE + target, len(members)))
        elif m == "ldr" and (hit := re.fullmatch(r"(x\d+), \[x(?:0|19), #(0x[0-9a-f]+|\d)\]", o)) \
                and any(x == ("cbz", f"{hit.group(1)}, {t}") for x in insns[i + 1:i + 3]
                        for t in [x[1].split(", ")[-1]]):
            # A refcounted pointer's count is decremented with subs; a string id's with sub
            # (after a compare with the pool's empty string).
            after = [x[0] for x in insns[i + 1:i + 7]]
            kind = "UnkRefPtrAt" if "subs" in after and "sub" not in after else "UnkStrIdAt"
            members.append(f"base::{kind}<{int(hit.group(2), 0):#x}>")
        elif m in ("bl", "b") and o.startswith("#0x"):
            t = int(o[1:], 16)
            if t != RELEASE and t not in calls:
                calls.append(t)
    m, o = insns[-1]
    if m == "b" and o.startswith("#0x") and int(o[1:], 16) not in calls:
        calls.append(int(o[1:], 16))
    deleting = OPERATOR_DELETE in calls
    bases = [c for c in calls if c != OPERATOR_DELETE]
    if not vtables or len(bases) > 1 or not (bases or deleting or members) or len(set(members)) != len(members):
        return None
    vtable = vtables[0][0]
    if len(vtables) == 2:
        base_vt, split = vtables[1]
        lo, hi = members[:split], members[split:]
        if bases or base_vt == vtable:
            return None
        fn = "refl_DeletingDtorInlineBase" if deleting else "refl_DtorInlineBase"
        return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({vtable, base_vt})] + [
            f"template <> struct base::UnkDtorTraits<{tag}> {{",
            f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
            f"    static char* base_vtable() {{ return g_unk_{base_vt:x}; }}", "};",
            f"template void base::{fn}<{tag}, base::UnkMembers<{', '.join(lo)}>, "
            f"base::UnkMembers<{', '.join(hi)}>>(char*);"])
    lines = [f"extern char g_unk_{vtable:x}[];"] + [f"void fn_{c:x}(char*);" for c in bases]
    lines += [f"template <> struct base::UnkDtorTraits<{tag}> {{",
              f"    static char* vtable() {{ return g_unk_{vtable:x}; }}"]
    if bases:
        lines.append(f"    static void base(char* p) {{ fn_{bases[0]:x}(p); }}")
    fn = ("refl_Dtor" if bases else "refl_DtorNoBase") if not deleting else \
        "refl_DeletingDtor" if bases else "refl_DeletingDtorNoBase"
    lines += ["};", f"template void base::{fn}<{tag}{''.join(', ' + x for x in members)}>(char*);"]
    return "\n".join(lines)


def list_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_ListDtor (UnkDestructors.h): the vtable (through the GOT), the allocator's offset and
    the node link's offset."""
    if len(insns) != 25 or insns[6:8] != [("add", "x8, x8, #0x10"), ("str", "x8, [x0]")] \
            or insns[10] != ("ldr", "x1, [x20, #0x10]!") or insns[14] != ("ldr", "x8, [x8, #0x48]") \
            or insns[16] != ("blr", "x8") or insns[19:21] != [("str", "wzr, [x19, #8]"), ("stp", "xzr, xzr, [x20]")]:
        return None
    alloc = re.fullmatch(r"x0, \[x19, #(0x[0-9a-f]+|\d)\]", insns[12][1])
    link = re.fullmatch(r"x21, \[x1(?:, #(0x[0-9a-f]+|\d))?\]", insns[15][1])
    vtable = got_target(orig, insns, 4)
    if not alloc or not link or vtable is None:
        return None
    return "\n".join([f"extern char g_unk_{vtable:x}[];",
                      f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}", "};",
                      f"template void base::refl_ListDtor<{tag}, {int(alloc.group(1), 16):#x}, "
                      f"{int(link.group(1) or '0', 16):#x}>(char*);"])


def list_deleting_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_ListDeletingDtor (UnkDestructors.h): as list_dtor."""
    if len(insns) != 21 or insns[-1] != ("b", f"#{OPERATOR_DELETE:#x}") or insns[5] != ("ldr", "x1, [x0, #0x10]") \
            or insns[7:9] != [("add", "x8, x8, #0x10"), ("str", "x8, [x0]")] or insns[12] != ("ldr", "x8, [x8, #0x48]") \
            or insns[14] != ("blr", "x8"):
        return None
    alloc = re.fullmatch(r"x0, \[x19, #(0x[0-9a-f]+|\d)\]", insns[10][1])
    link = re.fullmatch(r"x20, \[x1(?:, #(0x[0-9a-f]+|\d))?\]", insns[13][1])
    vtable = got_target(orig, insns, 3)
    if not alloc or not link or vtable is None:
        return None
    return "\n".join([f"extern char g_unk_{vtable:x}[];",
                      f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}", "};",
                      f"template void base::refl_ListDeletingDtor<{tag}, {int(alloc.group(1), 16):#x}, "
                      f"{int(link.group(1) or '0', 16):#x}>(char*);"])


def linked_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_LinkedDtor / refl_LinkedDeletingDtor (UnkDestructors.h): both vtables (through the
    GOT), the secondary base's offset, the unlink function and the base destructor."""
    if not 30 <= len(insns) <= 40 or insns[7:12] != [("add", "x8, x8, #0x10"), ("str", "x8, [x0]"), ("add", "x8, x9, #0x10"),
                                                      ("mov", "x1, x0"), ("mov", "x19, x0")]:
        return None
    sub = re.fullmatch(r"x8, \[x1, #(0x[0-9a-f]+)\]!", insns[12][1])
    if insns[12][0] != "str" or not sub or insns[13] != ("ldr", f"x0, [x0, #{int(sub.group(1), 16) + 0x18:#x}]"):
        return None
    vtable, link_vtable = got_target(orig, insns, 3), got_target(orig, insns, 5)
    calls = [int(o[1:], 16) for m, o in insns if m in ("bl", "b") and o.startswith("#0x")]
    if vtable is None or link_vtable is None or insns[18][0] != "bl":
        return None
    unlink = int(insns[18][1][1:], 16)
    deleting = calls[-1] == OPERATOR_DELETE
    bases = {c for c in calls if c not in (unlink, OPERATOR_DELETE)}
    if len(bases) != 1:
        return None
    base = bases.pop()
    fn = "refl_LinkedDeletingDtor" if deleting else "refl_LinkedDtor"
    return "\n".join([f"extern char g_unk_{vtable:x}[];", f"extern char g_unk_{link_vtable:x}[];",
                      f"void fn_{base:x}(char*);", f"void fn_{unlink:x}(char*, char*);",
                      f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
                      f"    static void base(char* p) {{ fn_{base:x}(p); }}", "};",
                      f"template <> struct base::UnkLinkTraits<{tag}> {{",
                      f"    static char* link_vtable() {{ return g_unk_{link_vtable:x}; }}",
                      f"    static void unlink(char* list, char* link) {{ fn_{unlink:x}(list, link); }}", "};",
                      f"template void base::{fn}<{tag}, {int(sub.group(1), 16):#x}>(char*);"])


STATE_RE = re.compile(r"w8, \[x0, #(0x[0-9a-f]+)\]")


def iter_value_destroy_vt(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """An iterator value's destroy helper (refl_IterValueDestroy) whose element (at +8, after a
    u32 index) stores a vtable and releases string-id or refcounted members. Facts: the state
    offset (= 8 + the element size), the vtable and the members' offsets in the element."""
    head = [m for m, _ in insns[:9]]
    if head[:3] != ["stp", "stp", "add"] or "ldr" not in head:
        return None
    state = next((int(h.group(1), 16) for m, o in insns[:6] if m == "ldr" and (h := STATE_RE.fullmatch(o))), None)
    if state is None or state % 8 or state < 0x10 or ("cmp", "w8, #2") not in insns[4:8]:
        return None
    pages: dict[str, int] = {}
    vtable = None
    members: list[str] = []
    for i, (m, o) in enumerate(insns[:-1]):
        regs = [r.strip() for r in o.split(",")]
        if m == "adrp":
            pages[regs[0]] = int(regs[1][1:], 16)
        elif m == "ldr" and vtable is None and (hit := VTABLE_RE.fullmatch(o)) and f"x{hit.group(1)}" in pages:
            got = pages[f"x{hit.group(1)}"] + int(hit.group(2), 16)
            if insns[i + 1] == ("add", f"x{hit.group(1)}, x{hit.group(1)}, #0x10") \
                    and insns[i + 2] == ("str", f"x{hit.group(1)}, [x19, #8]"):
                target = orig.img.pointer(got - BASE)
                if target is None:
                    return None
                vtable = BASE + target
        elif m == "ldr" and (hit := re.fullmatch(r"(x\d+), \[x19, #(0x[0-9a-f]+)\]", o)) \
                and any(x == ("cbz", f"{hit.group(1)}, {t}") for x in insns[i + 1:i + 3]
                        for t in [x[1].split(", ")[-1]]):
            after = [x[0] for x in insns[i + 1:i + 7]]
            kind = "UnkRefPtrAt" if "subs" in after and "sub" not in after else "UnkStrIdAt"
            off = int(hit.group(2), 16) - 8
            if off <= 0:
                return None
            members.append(f"base::{kind}<{off:#x}>")
        elif m in ("bl", "b") and o.startswith("#0x") and int(o[1:], 16) not in (RELEASE,) \
                and not addr <= int(o[1:], 16) < addr + orig.sizes[addr]:
            return None
    if vtable is None or not members or len(set(members)) != len(members):
        return None
    elem = f"base::UnkDtorElem<{tag}, {state - 8:#x}{''.join(', ' + x for x in members)}>"
    return "\n".join([f"extern char g_unk_{vtable:x}[];",
                      f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}", "};",
                      f"template void base::refl_IterValueDestroy<{elem}>(void*);"])


def iter_element_value(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """Returns an iterator value's element as a reflected value (refl_GetIterElementValue).
    Facts: the state offset (= 8 + the element size) and the element's type: a getter call, or
    (leaf copies) a descriptor loaded through the GOT."""
    if not insns or insns[-1] != ("b", f"#{MAKE_VALUE:#x}") or ("mov", "w3, #1") not in insns:
        return None
    if len(insns) == 21 and insns[3:6] == [("mov", "x19, x8"), ("ldr", "x8, [x1, #8]"), ("add", "x20, x8, #8")] \
            and insns[7] == ("cmp", "w9, #2") and insns[9] == ("ldr", "x20, [x20]"):
        state_insn, m, o = insns[6], *insns[10]
        if m != "bl" or not o.startswith("#0x"):
            return None
        getter = int(o[1:], 16)
        decl, expr = f"const base::reflection::CType* fn_{getter:x}();", f"fn_{getter:x}()"
    elif len(insns) == 14 and insns[:3] == [("mov", "x0, x8"), ("ldr", "x8, [x1, #8]"), ("add", "x2, x8, #8")] \
            and insns[4] == ("cmp", "w9, #2") and insns[6] == ("ldr", "x2, [x2]") \
            and insns[7][0] == "adrp" and insns[7][1].startswith("x1, #0x") and insns[8][0] == "ldr":
        state_insn = insns[3]
        hit = re.fullmatch(r"x1, \[x1, #(0x[0-9a-f]+)\]", insns[8][1])
        if not hit:
            return None
        reloc = orig.img.relocs.get(int(insns[7][1][5:], 16) + int(hit.group(1), 16) - BASE)
        if reloc is None or reloc.type != R_AARCH64_RELATIVE:
            return None
        desc = BASE + reloc.addend
        decl, expr = f"extern char g_unk_{desc:x}[];", f"reinterpret_cast<const CType*>(g_unk_{desc:x})"
    else:
        return None
    hit = re.fullmatch(r"w9, \[x8, #(0x[0-9a-f]+)\]", state_insn[1])
    if state_insn[0] != "ldr" or not hit:
        return None
    state = int(hit.group(1), 16)
    if state % 8 or state < 0x10:
        return None
    return "\n".join([decl,
                      f"template <> struct base::reflection::UnkFieldTraits<{tag}> {{",
                      f"    static const CType* type() {{ return {expr}; }}", "};",
                      f"template base::reflection::UnkReflValue base::reflection::refl_GetIterElementValue<"
                      f"{tag}, UnkCloneBlock<{tag}, {state - 8:#x}>>(void*, char*);"])


MEMSET = 0x7101238DF0


def hash_map_clear(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_HashMapClear (UnkHashMap.h) with string-id keys: the key's pool (through the GOT),
    the key's offset in a node and the link's."""
    if len(insns) != 60 or insns[4] != ("ldr", "w8, [x0, #0xc]") or ("bl", f"#{MEMSET:#x}") not in insns:
        return None
    link = re.fullmatch(r"x22, \[x20, #(0x[0-9a-f]+|\d)\]", insns[11][1])
    key = re.fullmatch(r"x8, \[x20(?:, #(0x[0-9a-f]+|\d))?\]", insns[16][1])
    pool = got_target(orig, insns, 9)
    if insns[16][0] != "str" or not link or not key or pool is None:
        return None
    elem = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return (f"template void base::refl_HashMapClear<{elem}, {int(key.group(1) or '0', 16):#x}, "
            f"{int(link.group(1), 16):#x}>(char*);")


def iter_assign_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """UnkCollectionIter's copy-assignment whose element has an out-of-line destructor. Facts: the
    index width, the state offset (= 0x18 + the element size) and the element's destructor."""
    if len(insns) > 48 or insns[3:5] != [("mov", "x19, x0"), ("cmp", "x0, x1")] or ("add", "x0, x19, #0x18") not in insns:
        return None
    key = {("ldr", "w8, [x1]"): "u32", ("ldr", "x8, [x1]"): "u64"}.get(insns[6])
    hit = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+|\d)\]", insns[8][1])
    calls = {o for m, o in insns if m == "bl"}
    if key is None or insns[8][0] != "ldr" or not hit or len(calls) != 1:
        return None
    state = int(hit.group(1), 16)
    if state % 8 or state <= 0x18:
        return None
    dtor = int(calls.pop()[1:], 16)
    it = f"base::UnkCollectionIter<UnkCloneDtorElem<{tag}, {state - 0x18:#x}>{', u64' if key == 'u64' else ''}>"
    return "\n".join([f"void fn_{dtor:x}(char*);",
                      f"template <> struct UnkCloneDtorTraits<{tag}> {{",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};",
                      f"template {it}& {it}::operator=(const {it}&);"])


def iter_element_dyn_value(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_GetIterElementDynValue (UnkReflValue.h): the state offset (= 8 + the element size)."""
    if len(insns) != 24 or insns[3:6] != [("mov", "x19, x8"), ("ldr", "x8, [x1, #8]"), ("add", "x20, x8, #8")] \
            or insns[7] != ("cmp", "w9, #2") or insns[9:14] != [("ldr", "x20, [x20]"), ("ldr", "x8, [x20]"),
                                                            ("mov", "x0, x20"), ("ldr", "x8, [x8]"), ("blr", "x8")] \
            or insns[-1] != ("b", f"#{MAKE_VALUE:#x}"):
        return None
    hit = re.fullmatch(r"w9, \[x8, #(0x[0-9a-f]+)\]", insns[6][1])
    if insns[6][0] != "ldr" or not hit or int(hit.group(1), 16) % 8 or int(hit.group(1), 16) < 0x10:
        return None
    return (f"template base::reflection::UnkReflValue base::reflection::refl_GetIterElementDynValue<"
            f"{tag}, UnkCloneBlock<{tag}, {int(hit.group(1), 16) - 8:#x}>>(void*, char*);")


def iter_value_destroy_strid_elem(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_IterValueDestroy for a pair whose only non-trivial part is one string id: the key
    (offset 0) or a member of the element. Facts: the state offset, the id's offset and pool."""
    if len(insns) != 32 or insns[4:9] != [("mov", "x19, x0"), ("cmp", "w8, #2"), ("b.eq", "@+70"), ("cmp", "w8, #1"),
                                          ("b.ne", "@+74")] or insns[24] != ("bl", f"#{RELEASE:#x}"):
        return None
    state = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[3][1])
    off = re.fullmatch(r"x8, \[x19(?:, #(0x[0-9a-f]+|\d))?\]", insns[13][1])
    pool = got_target(orig, insns, 9)
    if insns[3][0] != "ldr" or not state or not off or pool is None:
        return None
    state, off = int(state.group(1), 16), int(off.group(1) or "0", 16)
    if state % 8 or off % 8 or off + 8 > state:
        return None
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    if off == 0:
        pair = f"UnkCloneBlock<{tag}, {state - 8:#x}>, {ident}"
    else:
        pair = f"UnkCloneStrIdElem<{tag}, {off - 8:#x}, {state - 8:#x}, {ident}>"
    return f"template void base::refl_IterValueDestroy<{pair}>(void*);"


STRID_POOL = 0x7101D47108  # the string-id pool pointer (UnkCloneStrId)


def vector_move_poly(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """UnkVector<T>::operator=(&&) for polymorphic elements (UnkClonePolyElem): the destructor's
    vtable slot and the element size."""
    if len(insns) != 43 or insns[4:6] != [("mov", "x19, x0"), ("cmp", "x0, x1")] \
            or insns[12:15] != [("ldr", "x8, [x19]"), ("add", "x0, x8, x21"), ("ldr", "x8, [x0]")] \
            or insns[16] != ("blr", "x8") or insns[37] != ("stp", "xzr, xzr, [x20]"):
        return None
    base = {("ldr", "x8, [x8]"): "UnkClonePolyA", ("ldr", "x8, [x8, #8]"): "UnkClonePolyB"}.get(insns[15])
    size = re.fullmatch(r"x21, x21, #(0x[0-9a-f]+)", insns[19][1])
    if base is None or insns[19][0] != "add" or not size or int(size.group(1), 16) % 8:
        return None
    v = f"base::UnkVector<UnkClonePolyElem<{tag}, {base}, {int(size.group(1), 16):#x}>>"
    return f"template {v}& {v}::operator=({v}&&);"


def id_record_assign(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """UnkCloneIdRecord::operator= (UnkCloneHelpers.h): the id's pool and the last field's width."""
    if len(insns) != 41 or insns[4:6] != [("mov", "x19, x0"), ("cmp", "x0, x1")] \
            or insns[7:11] != [("ldr", "w8, [x1, #8]"), ("str", "w8, [x19, #8]"), ("ldr", "x8, [x19, #0x10]"),
                               ("ldr", "x21, [x1, #0x10]")] or insns[32] != ("bl", f"#{RELEASE:#x}"):
        return None
    t = {("ldr", "w8, [x20, #0x18]"): "u32", ("ldr", "x8, [x20, #0x18]"): "u64"}.get(insns[34])
    pool = got_target(orig, insns, 14)
    if t is None or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    rec = f"UnkCloneIdRecord<{tag}, {ident}, {t}>"
    return f"template {rec}& {rec}::operator=(const {rec}&);"


def strid_member_ctor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_StrIdMemberCtor (UnkCloneHelpers.h): the base constructor, the vtable (through the
    GOT) and the string id's offset."""
    if len(insns) != 49 or insns[3] != ("mov", "x19, x0") or insns[4][0] != "bl" or insns[8:10] != [
            ("add", "x8, x8, #0x10"), ("str", "x8, [x19]")]:
        return None
    off = re.fullmatch(r"xzr, \[x19, #(0x[0-9a-f]+|\d)\]", insns[7][1])
    vtable, pool = got_target(orig, insns, 5), got_target(orig, insns, 10)
    if insns[7][0] != "str" or not off or vtable is None or pool != STRID_POOL:
        return None
    base = int(insns[4][1][1:], 16)
    return "\n".join([f"extern char g_unk_{vtable:x}[];", f"void fn_{base:x}(char*);",
                      f"template <> struct base::UnkCtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
                      f"    static void base(char* p) {{ fn_{base:x}(p); }}", "};",
                      f"template void base::refl_StrIdMemberCtor<{tag}, {int(off.group(1), 16):#x}>(char*);"])


def iter_value_destroy_strid2(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_IterValueDestroy for a pair whose non-trivial parts are two string ids (the key and an
    element member, or two element members). Facts: the state offset, both offsets and the pool."""
    if len(insns) != 50 or insns[4:9] != [("mov", "x19, x0"), ("cmp", "w8, #2"), ("b.eq", "@+a0"), ("cmp", "w8, #1"),
                                          ("b.ne", "@+a4")] or insns[36] != ("bl", f"#{RELEASE:#x}"):
        return None
    state = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[3][1])
    hi = re.fullmatch(r"x9, \[x19(?:, #(0x[0-9a-f]+|\d))?\]", insns[13][1])
    lo = re.fullmatch(r"x8, \[x19(?:, #(0x[0-9a-f]+|\d))?\]", insns[25][1])
    pool = got_target(orig, insns, 9)
    if insns[3][0] != "ldr" or not state or not hi or not lo or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    state, hi, lo = int(state.group(1), 16), int(hi.group(1) or "0", 16), int(lo.group(1) or "0", 16)
    if state % 8 or hi % 8 or lo % 8 or not lo < hi < state:
        return None
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    if lo == 0:
        pair = f"UnkCloneStrIdElem<{tag}, {hi - 8:#x}, {state - 8:#x}, {ident}>, {ident}"
    else:
        pair = f"UnkCloneStrIdElem2<{tag}, {lo - 8:#x}, {hi - 8:#x}, {state - 8:#x}, {ident}>"
    # Too large to inline into refl_IterValueDestroy: the original's destroy is Reset itself.
    return f"template void base::UnkIterValue<{pair}>::Reset();"


def iter_value_copy_assign_elem(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_IterValueCopy keyed by a string id, for an element with an out-of-line copy-assignment
    (UnkCloneAssignElem). Facts: the state offset, the key's pool and the element's assignment."""
    if len(insns) != 66 or insns[4:6] != [("mov", "x19, x0"), ("cmp", "x0, x1")] or insns[48] != ("bl", f"#{RELEASE:#x}") \
            or insns[60][0] != "bl":
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[7][1])
    pool = got_target(orig, insns, 30)
    if insns[7][0] != "ldr" or not state or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    state = int(state.group(1), 16)
    if state % 8 or state < 0x10:
        return None
    assign = int(insns[60][1][1:], 16)
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return "\n".join([f"void fn_{assign:x}(void*, const void*);",
                      f"template <> struct UnkCloneAssignTraits<{tag}> {{",
                      f"    static void assign(void* dst, const void* src) {{ fn_{assign:x}(dst, src); }}", "};",
                      f"template void* base::refl_IterValueCopy<UnkCloneAssignElem<{tag}, {state - 8:#x}>, {ident}>("
                      "void*, const void*);"])


FILE_PATH_POOL = 0x7101D47200  # CFilePathStrId's string pool (UnkCloneStrIdB)


def iter_value_destroy_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """An iterator value's destroy helper (refl_IterValueDestroy) whose element (at +8) has an
    out-of-line destructor, keyed by a u32 index or a string id. Facts: the state offset (= 8 +
    the element size), the element's destructor, and the key's string pool."""
    if [m for m, _ in insns[:3]] != ["stp", "stp", "add"]:
        return None
    state = next((int(h.group(1), 16) for m, o in insns[3:6] if m == "ldr" and (h := STATE_RE.fullmatch(o))), None)
    if state is None or state % 8 or state < 0x10 or ("cmp", "w8, #2") not in insns[4:8]:
        return None
    try:
        i = insns.index(("add", "x0, x19, #8"))
    except ValueError:
        return None
    m, o = insns[i + 1]
    if m != "bl" or not o.startswith("#0x"):
        return None
    dtor = int(o[1:], 16)
    pages: dict[str, int] = {}
    pool = None
    for m, o in insns[i + 2:]:
        regs = [r.strip() for r in o.split(",")]
        if m == "adrp":
            pages[regs[0]] = int(regs[1][1:], 16)
        elif m == "ldr" and pool is None and (hit := re.fullmatch(r"(x\d+), \[(x\d+), #(0x[0-9a-f]+)\]", o)) \
                and hit.group(2) in pages:
            target = orig.img.pointer(pages[hit.group(2)] + int(hit.group(3), 16) - BASE)
            pool = None if target is None else BASE + target
        elif m in ("bl", "b") and o.startswith("#0x") and int(o[1:], 16) != RELEASE:
            return None
    releases = (("bl", f"#{RELEASE:#x}") in insns) or (("b", f"#{RELEASE:#x}") in insns)
    key = "" if not releases else f", UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f", UnkCloneStrId<{tag}>"
    return "\n".join([f"void fn_{dtor:x}(char*);",
                      f"template <> struct UnkCloneDtorTraits<{tag}> {{",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};",
                      f"template void base::refl_IterValueDestroy<UnkCloneDtorElem<{tag}, {state - 8:#x}>{key}>(void*);"])


REFINE_TYPE = 0x710009DCD8  # refines a type descriptor for an object (UnkReflTypedPtr.h)


def pointee_typed(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """A reflected pointer type's helper returning the pointee with its dynamic type
    (refl_PointeeTyped). Facts: the pointee's type getter and, when the getter is in the same
    TU, the descriptor address the original uses instead of its return value."""
    head = insns[:6]
    if [m for m, _ in head[:3]] != ["stp", "stp", "add"] or head[3] != ("ldr", "x20, [x0]") \
            or head[4] != ("mov", "x19, x8") or head[5][0] != "cbz":
        return None
    m, o = insns[6]
    if m != "bl" or not o.startswith("#0x"):
        return None
    getter = int(o[1:], 16)
    descriptor = None
    if insns[7][0] == "adrp" and insns[8][0] == "add" and insns[7][1].startswith("x0, ") \
            and insns[8][1].startswith("x0, x0, #"):
        descriptor = int(insns[7][1].split("#")[1], 16) + int(insns[8][1].split("#")[1], 16)
    if insns.count(("bl", f"#{REFINE_TYPE:#x}")) != 2:
        return None
    decl = [f"const base::reflection::CType* fn_{getter:x}();",
            f"namespace folded {{ const base::reflection::CType* fn_{getter:x}(); }}"]
    if descriptor is None:
        lines = decl + [f"template <> struct base::reflection::UnkTypedPtrTraits<{tag}> {{",
                        f"    static char* get() {{ return (char*)fn_{getter:x}(); }}",
                        f"    static char* get_static() {{ return (char*)folded::fn_{getter:x}(); }}", "};"]
    else:
        lines = decl + [f'extern char g_unk_{descriptor:x}[] __attribute__((visibility("hidden")));',
                        f"template <> struct base::reflection::UnkTypedPtrTraits<{tag}> {{",
                        f"    static char* get() {{ fn_{getter:x}(); return g_unk_{descriptor:x}; }}",
                        f"    static char* get_static() {{ folded::fn_{getter:x}(); return g_unk_{descriptor:x}; }}",
                        "};"]
    return "\n".join(lines + [f"template base::reflection::UnkReflTypedPtr "
                              f"base::reflection::refl_PointeeTyped<{tag}>(void* const*);"])


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


def field_set_strid(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """A string-id property write (UnkReflValue.h): the field offset, from the accesses through
    the register loaded from [x1, #8]."""
    if len(insns) > 60 or any(m in ("bl", "blr") and o != f"#{RELEASE:#x}" for m, o in insns):
        return None
    base = next((o.split(",")[0] for m, o in insns if m == "ldr" and o.endswith(", [x1, #8]")), None)
    if base is None:
        return None
    offs = {int(h.group(1) or "0", 0) for _, o in insns
            if (h := re.search(rf"\[{base}(?:, #(0x[0-9a-f]+|\d+))?\]", o))}
    if len(offs) != 1 or not any(o == "x8, #2" or o.startswith("x8, #0x100000002") for m, o in insns if m == "mov"):
        return None
    return (f"template base::reflection::UnkReflValue base::reflection::refl_SetFieldStrId<{tag}, {offs.pop():#x}>("
            "void*, char*, base::reflection::UnkReflValue* const*);")


STORE_LOADS = {"str": "ldr", "strb": "ldrb", "strh": "ldrh"}


def field_set_value(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """A scalar property write (refl_SetFieldValue, UnkReflValue.h): the stored type and the field
    offset, from the store through the register loaded from [x1, #8]."""
    if not 12 <= len(insns) <= 24 or insns[-1][0] != "ret" or any(m in ("bl", "blr", "b") for m, _ in insns) \
            or not re.fullmatch(r"x\d+, \[x2\]", insns[0][1]):
        return None
    base = next((o.split(",")[0] for m, o in insns if m == "ldr" and o.endswith(", [x1, #8]")), None)
    if base is None:
        return None
    found = None
    for i, (m, o) in enumerate(insns[1:], 1):
        hit = re.fullmatch(rf"([wxsd])\d+, \[{base}(?:, #(0x[0-9a-f]+|\d+))?\]", o)
        if hit and m in STORE_LOADS:
            if found is not None:
                return None
            lm, lo = insns[i - 1]
            t = LOAD_TYPES.get((lm, hit.group(1)))
            if lm != STORE_LOADS[m] or t is None or not lo.startswith(o.split(",")[0] + ", ["):
                return None
            found = (t, int(hit.group(2) or "0", 0))
    if found is None or not any(m == "movk" and o.endswith("#1, lsl #32") for m, o in insns):
        return None
    t, off = found
    return (f"template base::reflection::UnkReflValue base::reflection::refl_SetFieldValue<{tag}, {t}, {off:#x}>("
            "void*, char*, base::reflection::UnkReflValue* const*);")


IS_KIND_OF = 0x710009D50C  # fn_710009d50c(type, class): an is-kind-of test (UnkReflCast.h)


def cast_calls(insns: Insns) -> tuple[int, int] | None:
    """The base method and type getter of an override that caches a cast pointer."""
    calls = [o for m, o in insns if m in ("bl", "b") and o.startswith("#0x")]
    if len(calls) != 3 or calls[2] != f"#{IS_KIND_OF:#x}" or sum(m == "blr" for m, _ in insns) != 1:
        return None
    return int(calls[0][1:], 16), int(calls[1][1:], 16)


def static_desc(insns: Insns) -> int | None:
    """The descriptor passed to the is-kind-of test as a direct address (adrp + add into x1), when
    the type getter is called only to initialise it."""
    for i in range(len(insns) - 1):
        page = re.fullmatch(r"x1, #(0x[0-9a-f]+)", insns[i][1])
        lo = re.fullmatch(r"x1, x1, #(0x[0-9a-f]+|\d+)", insns[i + 1][1])
        if insns[i][0] == "adrp" and page and insns[i + 1][0] == "add" and lo:
            return int(page.group(1), 16) + int(lo.group(1), 0)
    return None


def cast_traits(tag: str, calls: tuple[int, int], ret: str, params: str, args: str,
                desc: int | None = None) -> list[str]:
    base, getter = calls
    get = (f"fn_{getter:x}(); return reinterpret_cast<const CType*>(g_unk_{desc:x});" if desc is not None
           else f"return fn_{getter:x}();")
    return ([f'extern char g_unk_{desc:x}[] __attribute__((visibility("hidden")));'] if desc is not None else []) + [
            f"const base::reflection::CType* fn_{getter:x}();",
            f"{ret} fn_{base:x}({params});",
            f"template <> struct base::reflection::UnkCastTraits<{tag}> {{",
            f"    static const CType* type() {{ {get} }}",
            f"    static {ret} base({params}) {{ {'return ' if ret != 'void' else ''}fn_{base:x}({args}); }}", "};"]


def call_and_cast_arg(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_CallAndCastArg (UnkReflCast.h): the base method, type getter and cache offset."""
    if len(insns) > 32 or ("mov", "x20, x1") not in insns[:8] or ("mov", "x19, x0") not in insns[:8] \
            or not any(m == "cbz" and o.startswith("x20,") for m, o in insns) or not (calls := cast_calls(insns)):
        return None
    offs = [int(h.group(1), 16) for m, o in insns if m == "str" and (h := re.fullmatch(r"x8, \[x19, #(0x[0-9a-f]+|\d)\]", o))]
    if len(offs) != 1:
        return None
    return "\n".join(cast_traits(tag, calls, "void", "char* s, base::reflection::UnkReflObject* a", "s, a") +
                     [f"template void base::reflection::refl_CallAndCastArg<{tag}, {offs[0]:#x}>("
                      "char*, base::reflection::UnkReflObject*);"])


def call_and_cast_field(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_CallAndCastField (UnkReflCast.h): the base method, type getter and both offsets."""
    if len(insns) > 32 or ("mov", "x19, x0") not in insns[:8] or ("and", "w0, w20, #1") not in insns \
            or not (calls := cast_calls(insns)):
        return None
    srcs = [int(h.group(1), 16) for m, o in insns if m == "ldr" and (h := re.fullmatch(r"x21, \[x19, #(0x[0-9a-f]+|\d)\]", o))]
    dsts = [int(h.group(1), 16) for m, o in insns if m == "str" and (h := re.fullmatch(r"x8, \[x19, #(0x[0-9a-f]+|\d)\]", o))]
    if not dsts and ("str", "x8, [x19, x9]") in insns and (wide := wide_constant32(insns, "w9")) is not None:
        dsts = [wide]
    if len(srcs) != 1 or len(dsts) != 1:
        return None
    return "\n".join(cast_traits(tag, calls, "bool", "char* s", "s", static_desc(insns)) +
                     [f"template bool base::reflection::refl_CallAndCastField<{tag}, {srcs[0]:#x}, {dsts[0]:#x}>(char*);"])


def got_target(orig: asmdiff.Original, insns: Insns, i: int) -> int | None:
    """The address a GOT entry points to, for an adrp at insns[i] and the ldr after it."""
    (m, o), (m2, o2) = insns[i], insns[i + 1]
    page = re.fullmatch(r"(x\d+), #(0x[0-9a-f]+|\d)", o)
    hit = re.fullmatch(r"(x\d+), \[(x\d+), #(0x[0-9a-f]+|\d)\]", o2)
    if m != "adrp" or m2 != "ldr" or not page or not hit or hit.group(2) != page.group(1):
        return None
    reloc = orig.img.relocs.get(int(page.group(2), 16) + int(hit.group(3), 16) - BASE)
    return None if reloc is None or reloc.type != R_AARCH64_RELATIVE else BASE + reloc.addend


def wide_constant(insns: Insns, reg: str) -> int | None:
    """A 64-bit constant built in `reg` by mov + movk."""
    value = None
    for m, o in insns:
        if m == "mov" and (h := re.fullmatch(rf"{reg}, #(0x[0-9a-f]+|\d+)", o)):
            value = int(h.group(1), 0)
        elif m == "movk" and value is not None and (h := re.fullmatch(rf"{reg}, #(0x[0-9a-f]+|\d+), lsl #(\d+)", o)):
            shift = int(h.group(2))
            value = (value & ~(0xFFFF << shift)) | (int(h.group(1), 0) << shift)
            if shift == 48:
                return value
    return None


def wide_constant32(insns: Insns, reg: str) -> int | None:
    """A 32-bit constant built in `reg` by mov + movk."""
    value = None
    for m, o in insns:
        if m == "mov" and (h := re.fullmatch(rf"{reg}, #(0x[0-9a-f]+|\d+)", o)):
            value = int(h.group(1), 0)
        elif m == "movk" and value is not None and (h := re.fullmatch(rf"{reg}, #(0x[0-9a-f]+|\d+), lsl #16", o)):
            return value | (int(h.group(1), 0) << 16)
    return None


SET_NOTIFY = 0x7100FDD2B4  # fn_7100fdd2b4(target, &id) (refl_SetFieldMinNotify)


def field_set_min_notify(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_SetFieldMinNotify (UnkReflValue.h): the field offset, the target global and the id."""
    if len(insns) > 48 or ("fmax", "s0, s0, s1") not in insns or ("bl", f"#{SET_NOTIFY:#x}") not in insns:
        return None
    offs = [int(h.group(1) or "0", 0) for m, o in insns if m == "str" and (h := re.fullmatch(r"s0, \[x8(?:, #(0x[0-9a-f]+|\d))?\]", o))]
    adrps = [i for i, (m, _) in enumerate(insns) if m == "adrp"]
    if len(offs) != 1 or len(adrps) != 1 or (target := got_target(orig, insns, adrps[0])) is None \
            or (ident := wide_constant(insns, "x8")) is None:
        return None
    return "\n".join([f"extern char* g_unk_{target:x};",
                      f"template <> struct base::reflection::UnkSetterTraits<{tag}> {{",
                      f"    static char* target() {{ return g_unk_{target:x}; }}", "};",
                      f"template base::reflection::UnkReflValue base::reflection::refl_SetFieldMinNotify<{tag}, "
                      f"{offs[0]:#x}, {ident:#x}>(void*, char*, base::reflection::UnkReflValue* const*);"])


ALLOC = 0x71000839A0  # fn_71000839a0(size, flags)


def field_set_apply(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_SetFieldApply (UnkReflValue.h) on a lazily created singleton: the stored type and
    offset, the singleton global, its size and constructor, and the call made on it."""
    if not 8 <= len(insns) <= 48 or not re.fullmatch(r"x\d+, \[x2\]", insns[4][1]):
        return None
    calls = [int(o[1:], 16) for m, o in insns if m == "bl"]
    if len(calls) != 3 or calls[0] != ALLOC or ("mov", "w1, #3") not in insns:
        return None
    size = next((int(h.group(1), 0) for m, o in insns if m == "mov" and (h := re.fullmatch(r"w0, #(0x[0-9a-f]+|\d+)", o))), None)
    stores = [(i, m, h) for i, (m, o) in enumerate(insns)
              if m in STORE_LOADS and (h := re.fullmatch(r"([wxsd])\d+, \[x8(?:, #(0x[0-9a-f]+|\d+))?\]", o))]
    adrps = [i for i, (m, _) in enumerate(insns) if m == "adrp"]
    if size is None or len(stores) != 1 or len(adrps) != 1 or (target := got_target(orig, insns, adrps[0])) is None:
        return None
    i, m, h = stores[0]
    t = LOAD_TYPES.get((STORE_LOADS[m], h.group(1)))
    if t is None or insns[i - 1][0] != STORE_LOADS[m]:
        return None
    _, ctor, apply = calls
    return "\n".join([f"extern char* g_unk_{target:x};", f"void fn_{ctor:x}(char*);", f"void fn_{apply:x}(char*);",
                      "void* fn_71000839a0(u64 size, u32 flags);",
                      f"template <> struct base::reflection::UnkSetterTraits<{tag}> {{",
                      "    static void apply() {",
                      f"        char* s = g_unk_{target:x};",
                      "        if (!s) {",
                      f"            s = static_cast<char*>(fn_71000839a0({size:#x}, 3));",
                      f"            fn_{ctor:x}(s);",
                      f"            g_unk_{target:x} = s;",
                      "        }",
                      f"        fn_{apply:x}(s);", "    }", "};",
                      f"template base::reflection::UnkReflValue base::reflection::refl_SetFieldApply<{tag}, {t}, "
                      f"{int(h.group(2) or '0', 0):#x}>(void*, char*, base::reflection::UnkReflValue* const*);"])


CSTRID_CTOR = 0x71000003D4


def enum_values(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """Registers named values with one callee: CStrId(name) then callee(x0, id, value) per value
    (enum value lists such as fn_7100028ed8). Emitted as a plain function."""
    import refl_attrs  # noqa: E402
    page: dict[str, int] = {}
    pending: str | None = None
    name: str | None = None
    w2: int | None = None
    values: list[tuple[str, int]] = []
    callee = None
    for m, o in insns:
        ops = [x.strip() for x in o.split(",")]
        if m == "adrp":
            page[ops[0]] = int(ops[1][1:], 16)
        elif m == "add" and ops[0] == "x1" and ops[1] in page and ops[2].startswith("#"):
            pending = orig.img.cstring(page[ops[1]] + int(ops[2][1:], 16) - BASE)
        elif m == "mov" and ops[0] == "w2":
            w2 = 0 if ops[1] == "wzr" else int(ops[1][1:], 0) if ops[1].startswith("#") else None
        elif m == "bl":
            t = int(ops[0][1:], 16)
            if t == CSTRID_CTOR:
                if pending is None or name is not None:
                    return None
                name, pending = pending, None
            elif t == RELEASE:
                continue
            else:
                if name is None or w2 is None or (callee is not None and t != callee):
                    return None
                callee = t
                values.append((name, w2 & 0xFFFFFFFF))
                name = w2 = None
        elif m in ("blr", "br"):
            return None
    if callee is None or len(values) < 2 or name is not None:
        return None
    body = [f"    fn_{callee:x}(e, base::global::CStrId({refl_attrs.c_string(n)}, true), {v:#x});" for n, v in values]
    return "\n".join([f"void fn_{callee:x}(void* e, const base::global::CStrId& name, u32 value);",
                      f"void fn_{addr:x}(void* e) {{", *body, "}"])


def got_at(orig: asmdiff.Original, insns: Insns, i: int, j: int) -> int | None:
    """got_target for an adrp at insns[i] whose ldr is at insns[j]."""
    (m, o), (m2, o2) = insns[i], insns[j]
    page = re.fullmatch(r"(x\d+), #(0x[0-9a-f]+|\d)", o)
    hit = re.fullmatch(r"(x\d+), \[(x\d+), #(0x[0-9a-f]+|\d)\]", o2)
    if m != "adrp" or m2 != "ldr" or not page or not hit or hit.group(2) != page.group(1):
        return None
    reloc = orig.img.relocs.get(int(page.group(2), 16) + int(hit.group(3), 16) - BASE)
    return None if reloc is None or reloc.type != R_AARCH64_RELATIVE else BASE + reloc.addend


def got_list(orig: asmdiff.Original, insns: Insns) -> list[int | None]:
    """The GOT targets a function loads, in order: each adrp with the next ldr off its register."""
    out = []
    for i, (m, o) in enumerate(insns):
        if m != "adrp":
            continue
        reg = o.split(",")[0]
        for j in range(i + 1, min(i + 8, len(insns))):
            m2, o2 = insns[j]
            if m2 == "ldr" and re.fullmatch(rf"x\d+, \[{reg}, #(0x[0-9a-f]+|\d)\]", o2):
                out.append(got_at(orig, insns, i, j))
                break
            if m2 == "add" and o2.startswith(f"{reg}, {reg}, "):
                break
    return out


def calls(insns: Insns) -> list[int]:
    return [int(o[1:], 16) for m, o in insns if m == "bl"]


def strid_ident(tag: str, pool: int) -> str:
    return f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"


def derived_copy_ctor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_DerivedCopyCtor (UnkCloneHelpers.h): both vtables and the width of the field at 0x18 (1 to 8 bytes)."""
    if len(insns) != 13 or insns[5] != ("ldp", "x8, x9, [x1, #8]"):
        return None
    base, vt = got_target(orig, insns, 0), got_at(orig, insns, 4, 6)
    t = {("ldr", "w8, [x1, #0x18]"): "u32", ("ldr", "x8, [x1, #0x18]"): "u64", ("ldrb", "w8, [x1, #0x18]"): "u8",
         ("ldrh", "w8, [x1, #0x18]"): "u16"}.get(insns[10])
    if base is None or vt is None or not t:
        return None
    return "\n".join([f"extern char g_unk_{base:x}[];", f"extern char g_unk_{vt:x}[];",
                      f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
                      f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_DerivedCopyCtor<{tag}, {t}>(char*, const char*);"])


def derived_copy_ctor_48(orig, addr, tag, insns):
    """refl_DerivedCopyCtor48: both vtables (through the GOT)."""
    if len(insns) != 29 or insns[6] != ("ldp", "x8, x9, [x1, #0x10]") or insns[11] != ("stp", "x9, x10, [x0, #0x18]") \
            or insns[23] != ("ldr", "w8, [x1, #0x44]") or insns[27] != ("str", "x8, [x0]"):
        return None
    base, vt = got_target(orig, insns, 0), got_at(orig, insns, 22, 24)
    if base is None or vt is None:
        return None
    return "\n".join([f"extern char g_unk_{base:x}[];", f"extern char g_unk_{vt:x}[];",
                      f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
                      f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_DerivedCopyCtor48<{tag}>(char*, const char*);"])


def keyed_pair_iter_assign(tag, ins):
    """refl_KeyedPairIterAssign: the pair's size (from the state offset) and destructor."""
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", ins[11][1])
    if not state or int(state.group(1), 16) < 0x18 or int(state.group(1), 16) % 8:
        return None
    it = f"base::UnkKeyedPairIter<{tag}, {int(state.group(1), 16) - 8:#x}>"
    return dtor_traits(tag, ins[17]) + f"template {it}* base::refl_KeyedPairIterAssign({it}*, const {it}*);"


def set_matrix(orig, addr, tag, insns):
    """refl_SetMatrix: the comparison's operand order and the mask global (addressed directly)."""
    if len(insns) != 74 or insns[6] != ("bl", "#0x710002614c") or insns[66] != ("orr", "w8, w8, #8") \
            or insns[70] != ("strb", "w8, [x19, #0xfa]") or insns[64][0] != "adrp" or insns[65][0] != "ldr":
        return None
    page = re.fullmatch(r"x9, #(0x[0-9a-f]+)", insns[64][1])
    off = re.fullmatch(r"w9, \[x9, #(0x[0-9a-f]+|\d)\]", insns[65][1])
    order = {"x12, x13, [x0]": "false", "x10, x11, [x0]": "true"}.get(insns[8][1])
    if not page or not off or order is None:
        return None
    g = int(page.group(1), 16) + int(off.group(1), 0)
    return "\n".join([f'extern u32 g_unk_{g:x} __attribute__((visibility("hidden")));',
                      f"template <> struct base::UnkSetMatrixTraits<{tag}> {{",
                      f"    static u32 mask() {{ return g_unk_{g:x}; }}", "};",
                      f"template void base::refl_SetMatrix<{tag}, {order}>(char*, const UnkMatrix44*);"])


def value_map_insert(orig, addr, tag, insns):
    """refl_ValueMapInsert: the value's size (the node's, less 0x20), its copy constructor and the
    keys' string pool."""
    if len(insns) != 66 or insns[11] != ("ldp", "w9, w8, [x8, #0x30]") or insns[43][0] != "bl" \
            or insns[49] != ("ldr", "x9, [x8, #0x40]!") or insns[60] != ("str", "w8, [x19, #0xc]"):
        return None
    size = re.fullmatch(r"w1, #(0x[0-9a-f]+)", insns[21][1])
    pool = got_target(orig, insns, 32)
    if not size or int(size.group(1), 16) % 8 or int(size.group(1), 16) <= 0x20 or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    copy = int(insns[43][1][1:], 16)
    m = f"base::UnkValueMap<{tag}, {int(size.group(1), 16) - 0x20:#x}, g_unk_{pool:x}>"
    return "\n".join([f"void fn_{copy:x}(void*, const void*);", f"template <> struct base::UnkValueMapTraits<{tag}> {{",
                      f"    static void copy(void* d, const void* s) {{ fn_{copy:x}(d, s); }}", "};",
                      f"template void base::refl_ValueMapInsert({m}*, base::global::UnkStringData* const*, const void*);"])


def shared_copy_ctor(orig, addr, tag, insns):
    """refl_SharedCopyCtor: both vtables (through the GOT)."""
    if len(insns) != 56 or insns[0] != ("str", "wzr, [x0, #8]") or insns[43] != ("ldr", "x8, [x1, #0xc8]") \
            or insns[47] != ("ldxr", "x9, [x8]") or insns[54] != ("str", "x8, [x0]"):
        return None
    base, vt = got_target(orig, insns, 1), got_target(orig, insns, 51)
    if base is None or vt is None:
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({base, vt})] + [
        f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
        f"template void base::refl_SharedCopyCtor<{tag}>(char*, const char*);"])


def index_pair_iter_assign(tag, ins):
    """refl_IndexPairIterAssign: the pair's size (from the state offset) and destructor."""
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", ins[8][1])
    if not state or int(state.group(1), 16) < 0x20 or int(state.group(1), 16) % 8:
        return None
    it = f"base::UnkIndexPairIter<{tag}, {int(state.group(1), 16) - 0x10:#x}>"
    return dtor_traits(tag, ins[17]) + f"template {it}* base::refl_IndexPairIterAssign({it}*, const {it}*);"


def strid_copy_ctor(orig, addr, tag, insns):
    """refl_StrIdCopyCtor: the three vtables (through the GOT)."""
    if len(insns) != 30 or insns[4] != ("ldp", "x8, x9, [x1, #8]") or insns[10] != ("ldr", "x8, [x1, #0x18]") \
            or insns[22] != ("ldp", "x8, x9, [x1, #0x20]") or insns[28] != ("str", "x8, [x0]"):
        return None
    base, mid, pool, vt = (got_target(orig, insns, i) for i in (0, 6, 12, 24))
    if None in (base, mid, vt) or pool != STRID_POOL:
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({base, mid, vt})] + [
        f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
        f"    static char* mid_vtable() {{ return g_unk_{mid:x}; }}",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
        f"template void base::refl_StrIdCopyCtor<{tag}>(char*, const char*);"])


def strid_value_facts(orig: asmdiff.Original, insns: Insns, n: int, value: int) -> tuple | None:
    """UnkStrIdValueVec.h: both vtables (through the GOT, around the string pool) and the value's
    width (its copy at insns[value])."""
    if len(insns) != n:
        return None
    gots = got_list(orig, insns)
    if len(gots) < 3 or gots[1] != STRID_POOL or None in gots[:3]:
        return None
    m, o = insns[value]
    t = {"w8, [x22, #0x10]": "u32", "x8, [x22, #0x10]": "u64"}.get(o)
    return (gots[0], gots[2], t, calls(insns)) if m == "ldr" and t else None


def strid_value_traits(tag: str, base: int, vt: int, decls: list[str], members: list[str]) -> list[str]:
    return [f"extern char g_unk_{base:x}[];", f"extern char g_unk_{vt:x}[];", *decls,
            f"template <> struct base::UnkStrIdValueTraits<{tag}> {{",
            f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
            f"    static char* vtable() {{ return g_unk_{vt:x}; }}", *members, "};"]


def strid_value_resize(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_StrIdValueResize: the vtables and value type; a value initialised to 1.0f is an f32."""
    one = ("mov", "w24, #0x3f800000") in insns
    facts = strid_value_facts(orig, insns, 130 if one else 129, 37)
    if not facts:
        return None
    base, vt, t, _ = facts
    t = "f32" if one else t
    init = "    static void init(f32& v) { v = 1.0f; }" if one else f"    static void init({t}&) {{}}"
    vec = f"base::UnkVector<base::UnkStrIdValueElem<{tag}, {t}>>"
    return "\n".join(strid_value_traits(tag, base, vt, [], [init]) +
                     [f"template void base::refl_StrIdValueResize<{tag}, {t}>({vec}*, u32);"])


def strid_value_grow_at(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_StrIdValueGrowAt: the vtables, value type and the resize it calls."""
    facts = strid_value_facts(orig, insns, 93, 49)
    if not facts or len(facts[3]) != 1:
        return None
    base, vt, t, (callee,) = facts
    vec = f"base::UnkVector<base::UnkStrIdValueElem<{tag}, {t}>>"
    return "\n".join(strid_value_traits(tag, base, vt, [f"void fn_{callee:x}(void*, u32);"],
                                        [f"    static void resize(void* v, u32 n) {{ fn_{callee:x}(v, n); }}"]) +
                     [f"template base::UnkStrIdValueElem<{tag}, {t}>& base::refl_StrIdValueGrowAt<{tag}, {t}>({vec}*, u32);"])


def strid_value_vec_copy(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_StrIdValueVecCopy: the vtables, value type and the push-back it calls."""
    facts = strid_value_facts(orig, insns, 92, 43)
    if not facts or len(facts[3]) != 1:
        return None
    base, vt, t, (callee,) = facts
    vec = f"base::UnkVector<base::UnkStrIdValueElem<{tag}, {t}>>"
    return "\n".join(strid_value_traits(tag, base, vt, [f"void fn_{callee:x}(void*, const void*);"],
                                        [f"    static void push_back(void* v, const void* e) {{ fn_{callee:x}(v, e); }}"]) +
                     [f"template void base::refl_StrIdValueVecCopy<{tag}, {t}>({vec}*, const {vec}*);"])


def strid_value_insert(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_StrIdValueInsert: the vtables and value type."""
    if len(insns) != 253 or calls(insns) or insns[240][0] != "ldr":
        return None
    gots = got_list(orig, insns)
    t = {"w9, [x20, #0x10]": "u32", "x9, [x20, #0x10]": "u64"}.get(insns[240][1])
    if len(gots) < 3 or gots[1] != STRID_POOL or None in gots[:3] or not t:
        return None
    base, vt = gots[0], gots[2]
    vec = f"base::UnkVector<base::UnkStrIdValueElem<{tag}, {t}>>"
    return "\n".join(strid_value_traits(tag, base, vt, [], []) +
                     [f"template void base::refl_StrIdValueInsert<{tag}, {t}>({vec}*, "
                      f"const base::UnkStrIdValueElem<{tag}, {t}>&, u32);"])


def ref_pair_array_deleting_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_RefPairArrayDeletingDtor (UnkDestructors.h): the vtable."""
    if len(insns) != 48 or insns[12] != ("add", "x21, x19, #0x14") or insns[18] != ("add", "x21, x21, #0x48"):
        return None
    vt = got_target(orig, insns, 4)
    if vt is None or insns[47] != ("b", f"#{OPERATOR_DELETE:#x}"):
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_RefPairArrayDeletingDtor<{tag}, 0xc, 0x48, 0, 8>(char*);"])


def list_owner_deleting_dtor(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_ListOwnerDeletingDtor (UnkDestructors.h): the three vtables and the nodes' link."""
    if len(insns) != 56 or insns[31] != ("bl", "#0x71000837f4") or insns[55] != ("b", f"#{OPERATOR_DELETE:#x}"):
        return None
    vt, member, base = got_target(orig, insns, 4), got_at(orig, insns, 22, 25), got_target(orig, insns, 38)
    link = re.fullmatch(r"x21, \[x1, #(0x[0-9a-f]+|\d)\]", insns[17][1])
    link2 = re.fullmatch(r"x20, \[x1, #(0x[0-9a-f]+|\d)\]", insns[47][1])
    if None in (vt, member, base) or not link or not link2 or link.group(1) != link2.group(1):
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({vt, member, base})] + [
        f"template <> struct base::UnkListOwnerTraits<{tag}> {{",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
        f"    static char* member_vtable() {{ return g_unk_{member:x}; }}", "};",
        f"template void base::refl_ListOwnerDeletingDtor<{tag}, {int(link.group(1), 16):#x}>(char*);"])


def iter_sync_strid_elem(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_IterSync over elements whose only non-trivial part is a string id at their start:
    the state offset (= 0x18 + the element size) and the id's pool."""
    if len(insns) != 68 or insns[27] != ("bl", f"#{RELEASE:#x}") or insns[34] != ("stp", "x19, x20, [x19, #0x10]"):
        return None
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", insns[7][1])
    pool = got_target(orig, insns, 12)
    if not state or pool not in (STRID_POOL, FILE_PATH_POOL) or insns[16] != ("ldr", "x8, [x19, #0x18]"):
        return None
    size = int(state.group(1), 16) - 0x18
    if size <= 0 or size % 8:
        return None
    return (f"template void base::refl_IterSync(base::UnkCollectionIter<UnkCloneStrIdElem<{tag}, 0, {size:#x}, "
            f"{strid_ident(tag, pool)}>, u32>*);")


def chained_set_clear(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_ChainedSetClear (UnkHashMap.h): the key's pool, the nodes' link and the bucket accessor."""
    if len(insns) != 66 or insns[5] != ("ldr", "w8, [x0, #0x44]") or insns[56] != ("bl", f"#{MEMSET:#x}"):
        return None
    pool = got_target(orig, insns, 10)
    link = re.fullmatch(r"x24, \[x21, #(0x[0-9a-f]+|\d)\]", insns[35][1])
    if pool not in (STRID_POOL, FILE_PATH_POOL) or not link or insns[19][0] != "bl" or insns[38] != ("ldr", "x8, [x21]"):
        return None
    bucket = int(insns[19][1][1:], 16)
    return "\n".join([f"char** fn_{bucket:x}(char*, u32);", f"template <> struct base::UnkChainedSetTraits<{tag}> {{",
                      f"    static char** bucket(char* self, u32 i) {{ return fn_{bucket:x}(self, i); }}", "};",
                      f"template void base::refl_ChainedSetClear<{tag}, {strid_ident(tag, pool)}, "
                      f"{int(link.group(1), 16):#x}>(char*);"])


def cache_add(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_CacheAdd (UnkCache.h): the bucket accessor."""
    if len(insns) != 97 or got_list(orig, insns) != [STRID_POOL, FILE_PATH_POOL, 0x7101A19610]:
        return None
    callees = calls(insns)
    if len(callees) != 3 or callees[:2] != [0x71000171A0, 0x7100019FC0] or insns[88] != ("add", "x0, x19, #8"):
        return None
    return "\n".join([f"char** fn_{callees[2]:x}(char*, u32);", f"template <> struct base::UnkCacheTraits<{tag}> {{",
                      f"    static char** bucket(char* b, u32 i) {{ return fn_{callees[2]:x}(b, i); }}", "};",
                      f"template void* base::refl_CacheAdd<{tag}>(base::UnkCacheOwner*, const char*, u32, void*);"])


def iter_sync_callbacks(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_IterSync over UnkCallbackList(Short) (UnkCloneHelpers.h): both vtables; the state
    offset picks the element."""
    if len(insns) != 123 or insns[16:18] != [("add", "x8, x8, #0x10"), ("str", "x8, [x19, #0x18]")]:
        return None
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", insns[8][1])
    vt, base = got_target(orig, insns, 13), got_target(orig, insns, 26)
    if not state or vt is None or base is None or insns[15] != ("ldr", "x0, [x19, #0x50]"):
        return None
    elem = {0x58: "UnkCallbackListShort", 0x60: "UnkCallbackList"}.get(int(state.group(1), 16))
    if not elem:
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"extern char g_unk_{base:x}[];",
                      f"template <> struct base::UnkCallbackTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}",
                      f"    static char* base_vtable() {{ return g_unk_{base:x}; }}", "};",
                      f"template void base::refl_IterSync(base::UnkCollectionIter<base::{elem}<{tag}>, u32>*);"])


HASH_KEY_CALLS = [0x710007BE8C, 0x710007C3F0, 0x710007C3F0, 0x710007C3F0, 0x710007C3F0, 0x710007C1A4, 0x710007B750,
                  0x710007C0E4, 0x7101238DA0, 0x7100000250, 0x7101238DB0]


def hash_key_fields(orig, addr: int, tag: str, insns) -> str | None:
    """refl_HashKeyFields (UnkCloneHelpers.h): the field offset and the static's source string id."""
    if len(insns) != 89 or insns[5] != ("mov", "w1, #0x16") or insns[31] != ("ldr", "x8, [x8, #0x38]"):
        return None
    if [int(o[1:], 16) for m, o in insns if m == "bl"] != HASH_KEY_CALLS:
        return None
    a = re.fullmatch(r"x8, \[x19(?:, #(0x[0-9a-f]+|\d+))?\]", insns[22][1])
    b = re.fullmatch(r"x0, \[x19, #(0x[0-9a-f]+|\d+)\]", insns[29][1])
    c = re.fullmatch(r"x1, x19, #(0x[0-9a-f]+|\d+)", insns[38][1])
    if insns[22][0] != "ldr" or insns[29][0] != "ldr" or insns[38][0] != "add" or not (a and b and c):
        return None
    off = int(a.group(1) or "0", 0)
    if int(b.group(1), 0) != off + 8 or int(c.group(1), 0) != off + 0x10:
        return None
    source = got_target(orig, insns, 63)
    if source is None:
        return None
    return "\n".join([f"namespace hash_key {{ extern base::global::CStrId g_unk_{source:x}; }}",
                      f"template <> struct UnkHashKeyTraits<{tag}> {{",
                      f"    static const base::global::CStrId& source() {{ return hash_key::g_unk_{source:x}; }}", "};",
                      f"template u32 refl_HashKeyFields<{tag}, {off:#x}>(char*);"])


_PUSH_PAIR_REF: list = []


def push_pair(orig, addr: int, tag: str, insns) -> str | None:
    """refl_PushPair (UnkCloneHelpers.h): the two callees whose results are appended. Every
    other instruction must equal fn_710007e34c's."""
    if len(insns) != 103 or insns[5][0] != "bl" or insns[50][0] != "bl":
        return None
    if not _PUSH_PAIR_REF:
        _PUSH_PAIR_REF.extend(instructions(orig, 0x710007E34C))
    if any(a != b for i, (a, b) in enumerate(zip(insns, _PUSH_PAIR_REF)) if i not in (5, 50)):
        return None
    first, second = int(insns[5][1][1:], 16), int(insns[50][1][1:], 16)
    return "\n".join([f"namespace push_pair {{ void* fn_{first:x}(); void* fn_{second:x}(); }}",
                      f"template <> struct base::UnkPushPairTraits<{tag}> {{",
                      f"    static void* first() {{ return push_pair::fn_{first:x}(); }}",
                      f"    static void* second() {{ return push_pair::fn_{second:x}(); }}", "};",
                      f"template void base::refl_PushPair<{tag}>(char*);"])


_DEFERRED_INVOKE_REF: list = []
_DEFERRED_INVOKE_ARGS_3152c = {"x1, [x20]": "u64", "w1, [x20]": "u32", "s0, [x20]": "f32", "d0, [x20]": "f64"}


def deferred_invoke(orig, addr: int, tag: str, insns) -> str | None:
    """refl_DeferredInvoke (UnkCloneHelpers.h): the item getter and the argument's type. Every
    other instruction must equal fn_710003152c's."""
    if len(insns) != 134 or insns[18][0] != "bl" or insns[20][0] != "ldr":
        return None
    arg = _DEFERRED_INVOKE_ARGS_3152c.get(insns[20][1])
    if arg is None:
        return None
    if not _DEFERRED_INVOKE_REF:
        _DEFERRED_INVOKE_REF.extend(instructions(orig, 0x710003152C))
    if any(a != b for i, (a, b) in enumerate(zip(insns, _DEFERRED_INVOKE_REF)) if i not in (18, 20)):
        return None
    at = int(insns[18][1][1:], 16)
    cb = f"base::UnkDeferredCallback<{arg}>"
    return "\n".join([f"namespace deferred_invoke {{ {cb}** fn_{at:x}(base::UnkVector<{cb}*>*, u32); }}",
                      f"template <> struct base::UnkDeferredInvokeTraits<{tag}> {{",
                      f"    static {cb}** at(base::UnkVector<{cb}*>* v, u32 i) {{ return deferred_invoke::fn_{at:x}(v, i); }}",
                      "};",
                      f"template void base::refl_DeferredInvoke<{tag}, {arg}>(base::UnkDeferredSet<{cb}>*, const {arg}*);"])


_DEFERRED_INVOKE_PTR_REF: list = []


def deferred_invoke_ptr(orig, addr: int, tag: str, insns) -> str | None:
    """refl_DeferredInvokePtr (UnkCloneHelpers.h): the item getter. Every other instruction must
    equal fn_710006ca34's."""
    if len(insns) != 134 or insns[18][0] != "bl" or insns[20] != ("mov", "x1, x20"):
        return None
    if not _DEFERRED_INVOKE_PTR_REF:
        _DEFERRED_INVOKE_PTR_REF.extend(instructions(orig, 0x710006CA34))
    if any(a != b for i, (a, b) in enumerate(zip(insns, _DEFERRED_INVOKE_PTR_REF)) if i != 18):
        return None
    at = int(insns[18][1][1:], 16)
    cb = "base::UnkDeferredCallback<const void*>"
    return "\n".join([f"namespace deferred_invoke {{ {cb}** fn_{at:x}(base::UnkVector<{cb}*>*, u32); }}",
                      f"template <> struct base::UnkDeferredInvokeTraits<{tag}> {{",
                      f"    static {cb}** at(base::UnkVector<{cb}*>* v, u32 i) {{ return deferred_invoke::fn_{at:x}(v, i); }}",
                      "};",
                      f"template void base::refl_DeferredInvokePtr<{tag}>(base::UnkDeferredSet<{cb}>*, const void*);"])


# (element offset, list size) -> a reference copy of that layout
_CALLBACK_DESTROY_REFS_3aad4 = {(0x10, 0x40): 0x710023AAD4, (0, 0x40): 0x7100245A68, (0x10, 0x48): 0x710090B050,
                          (0, 0x48): 0x710090C30C}
_CALLBACK_DESTROY_INSNS: dict = {}


def callback_list_destroy(orig, addr: int, tag: str, insns) -> str | None:
    """Destroys an iterator (refl_IterDestroy) or iterator value (refl_IterValueDestroy) holding
    an UnkCallbackList or UnkCallbackListShort. Facts: the layout (from the state and vtable
    store offsets) and the two vtables."""
    if len(insns) != 62:
        return None
    state = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[4][1])
    vt = re.fullmatch(r"x8, \[x19(?:, #(0x[0-9a-f]+|\d))?\]", insns[14][1])
    if insns[4][0] != "ldr" or insns[14][0] != "str" or not state or not vt:
        return None
    base = int(vt.group(1) or "0", 0) - 8
    size = int(state.group(1), 16) - base - 8
    ref = _CALLBACK_DESTROY_REFS_3aad4.get((base, size))
    if ref is None:
        return None
    if ref not in _CALLBACK_DESTROY_INSNS:
        _CALLBACK_DESTROY_INSNS[ref] = instructions(orig, ref)
    if any(a != b for i, (a, b) in enumerate(zip(insns, _CALLBACK_DESTROY_INSNS[ref])) if i not in (10, 11, 23, 24)):
        return None
    vtable, base_vtable = got_target(orig, insns, 10), got_target(orig, insns, 23)
    if vtable is None or base_vtable is None:
        return None
    lst = f"base::{'UnkCallbackListShort' if size == 0x40 else 'UnkCallbackList'}<{tag}>"
    inst = (f"template void base::refl_IterDestroy(base::UnkCollectionIter<{lst}, u32>*);" if base
            else f"template void base::refl_IterValueDestroy<{lst}>(void*);")
    return "\n".join([f"extern char g_unk_{vtable:x}[];", f"extern char g_unk_{base_vtable:x}[];",
                      f"template <> struct base::UnkCallbackTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
                      f"    static char* base_vtable() {{ return g_unk_{base_vtable:x}; }}", "};", inst])


_POLY_STRID_MOVE_REF: list = []


def iter_value_move_poly_strid(orig, addr: int, tag: str, insns) -> str | None:
    """refl_IterValueMove of an UnkPolyStrIdElem: the two vtables and the field's type (u32 or
    u64). Every other instruction must equal fn_7100f5ee20's."""
    if len(insns) != 58:
        return None
    field = {("ldr", "w8, [x8]"): "u32", ("ldr", "x8, [x8]"): "u64"}.get(insns[55])
    if field is None or insns[56] != ("str", f"{'w' if field == 'u32' else 'x'}8, [x0, #0x18]"):
        return None
    if not _POLY_STRID_MOVE_REF:
        _POLY_STRID_MOVE_REF.extend(instructions(orig, 0x7100F5EE20))
    if any(a != b for i, (a, b) in enumerate(zip(insns, _POLY_STRID_MOVE_REF))
           if i not in (9, 10, 24, 25, 36, 37, 50, 51, 55, 56)):
        return None
    base_vtable, vtable = got_target(orig, insns, 9), got_target(orig, insns, 24)
    if (base_vtable is None or vtable is None or got_target(orig, insns, 36) != base_vtable
            or got_target(orig, insns, 50) != vtable):
        return None
    return "\n".join([f"extern char g_unk_{vtable:x}[];", f"extern char g_unk_{base_vtable:x}[];",
                      f"template <> struct base::UnkPolyStrIdTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
                      f"    static char* base_vtable() {{ return g_unk_{base_vtable:x}; }}", "};",
                      f"template void base::refl_IterValueMove<base::UnkPolyStrIdElem<{tag}, {field}>>(void*, const void*);"])


_CHAIN_MAP_ERASE_REF: list = []


def chain_map_erase(orig, addr: int, tag: str, insns) -> str | None:
    """refl_ChainMapErase (UnkCloneHelpers.h): the bucket getter and the keys' string pool.
    Every other instruction must equal fn_7100020fa8's."""
    if len(insns) != 96 or insns[20][0] != "bl" or insns[37] != insns[20] or insns[54] != insns[20]:
        return None
    if not _CHAIN_MAP_ERASE_REF:
        _CHAIN_MAP_ERASE_REF.extend(instructions(orig, 0x7100020FA8))
    if any(a != b for i, (a, b) in enumerate(zip(insns, _CHAIN_MAP_ERASE_REF)) if i not in (20, 37, 54, 65, 66)):
        return None
    pool = got_target(orig, insns, 65)
    if pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    ident = f"base::UnkStrIdMemberOf<g_unk_{pool:x}>"
    at = int(insns[20][1][1:], 16)
    return "\n".join([f"namespace chain_map {{ void* fn_{at:x}(void*, u32); }}",
                      f"template <> struct base::UnkChainMapTraits<{tag}> {{",
                      "    template <class M>",
                      f"    static auto at(M* m, u32 b) {{ return static_cast<decltype(m->first_node)*>(chain_map::fn_{at:x}(m, b)); }}",
                      "};",
                      f"template void base::refl_ChainMapErase(base::UnkChainMap<{tag}, {ident}>*, "
                      f"base::UnkChainMapIter<{tag}, {ident}>*);"])


def vector_free_poly(orig, addr, tag, insns):
    """UnkVector<T>::Free() for polymorphic elements (UnkClonePolyElem): the destructor's vtable
    slot and the element size."""
    if len(insns) != 39 or insns[4:6] != [("ldr", "x1, [x0]"), ("cbz", "x1, @+8c")] \
            or insns[18:21] != [("ldr", "x8, [x19]"), ("add", "x0, x8, x21"), ("ldr", "x8, [x0]")] \
            or insns[32] != ("ldr", "x8, [x8, #0x48]") or insns[34] != ("str", "wzr, [x19, #0xc]"):
        return None
    slots = {("ldr", "x8, [x8]"): "UnkClonePolyA", ("ldr", "x8, [x8, #8]"): "UnkClonePolyB"}
    base = slots.get(insns[11])
    if base is None or slots.get(insns[21]) != base:
        return None
    size = re.fullmatch(r"w21, #(0x[0-9a-f]+)", insns[17][1])
    if insns[17][0] != "mov" or not size or insns[25] != ("add", f"x21, x21, {size.group(0)[5:]}"):
        return None
    size = int(size.group(1), 16)
    if size % 8 or size < 8:
        return None
    v = f"base::UnkVector<UnkClonePolyElem<{tag}, {base}, {size:#x}>>"
    return f"template void {v}::Free();"


def vector_clear_poly(orig, addr, tag, insns):
    """UnkVector<T>::Clear() for polymorphic elements (UnkClonePolyElem): the destructor's vtable
    slot and the element size."""
    if len(insns) != 24 or insns[9:11] != [("ldr", "x8, [x19]"), ("add", "x0, x8, x20")] \
            or insns[19] != ("str", "wzr, [x19, #8]") or insns[13] != ("blr", "x8"):
        return None
    base = {("ldr", "x8, [x8]"): "UnkClonePolyA", ("ldr", "x8, [x8, #8]"): "UnkClonePolyB"}.get(insns[12])
    size = re.fullmatch(r"x20, x20, #(0x[0-9a-f]+)", insns[16][1])
    if base is None or insns[16][0] != "add" or not size or int(size.group(1), 16) % 8:
        return None
    return f"template void base::UnkVector<UnkClonePolyElem<{tag}, {base}, {size.group(1)}>>::Clear();"


def value_holder_deleting_dtor(orig, addr, tag, insns):
    """refl_ValueHolderDeletingDtor: the base class's vtable (through the GOT)."""
    if len(insns) != 18 or insns[3] != ("stp", "xzr, xzr, [x0, #0x60]") or insns[8] != ("stp", "xzr, xzr, [x0, #0x30]") \
            or insns[11] != ("add", "x0, x0, #0x18") or insns[13] != ("bl", f"#{0x71000A1C60:#x}") \
            or insns[17] != ("b", f"#{OPERATOR_DELETE:#x}"):
        return None
    vt = got_at(orig, insns, 4, 9)
    if vt is None:
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"template <> struct base::UnkValueHolderTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_ValueHolderDeletingDtor(base::UnkValueHolder<{tag}>*);"])


def callback_base_dtor(orig, addr, tag, insns):
    """refl_CallbackBaseDtor / refl_CallbackBaseDeletingDtor: the class's vtable, the list's two
    vtables (through the GOT), the list's offset and the base destructor."""
    deleting = len(insns) == 63
    if len(insns) not in (61, 63) or insns[8:12] != [("add", "x8, x8, #0x10"), ("str", "x8, [x0]"),
                                                     ("add", "x8, x9, #0x10"), ("mov", "x19, x0")] \
            or insns[20] != ("ldr", "x8, [x8, #0x10]") or insns[35] != ("ldr", "x8, [x9, #0x40]") \
            or insns[53] != ("ldr", "x8, [x8, #0x48]"):
        return None
    sub = re.fullmatch(r"x8, \[x0, #(0x[0-9a-f]+|\d)\]", insns[12][1])
    if insns[12][0] != "str" or not sub:
        return None
    sub = int(sub.group(1), 16)
    if insns[13] != ("ldr", f"x0, [x0, #{sub + 0x38:#x}]") or insns[26] != ("str", f"x8, [x19, #{sub:#x}]") \
            or insns[50] != ("ldr", f"x0, [x19, #{sub + 0x28:#x}]"):
        return None
    if deleting:
        if insns[57][0] != "bl" or insns[-1] != ("b", f"#{OPERATOR_DELETE:#x}"):
            return None
        base = insns[57][1]
    else:
        if insns[-1][0] != "b":
            return None
        base = insns[-1][1]
    if not base.startswith("#0x"):
        return None
    base = int(base[1:], 16)
    vtable, sub_vtable, sub_base = got_target(orig, insns, 4), got_target(orig, insns, 6), \
        got_target(orig, insns, 22)
    if None in (vtable, sub_vtable, sub_base):
        return None
    fn = "refl_CallbackBaseDeletingDtor" if deleting else "refl_CallbackBaseDtor"
    return "\n".join([f"extern char g_unk_{v:x}[];" for v in sorted({vtable, sub_vtable, sub_base})] + [
        f"void fn_{base:x}(char*);",
        f"template <> struct base::UnkDtorTraits<{tag}> {{",
        f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
        f"    static void base(char* p) {{ fn_{base:x}(p); }}", "};",
        f"template <> struct base::UnkCallbackTraits<{tag}> {{",
        f"    static char* vtable() {{ return g_unk_{sub_vtable:x}; }}",
        f"    static char* base_vtable() {{ return g_unk_{sub_base:x}; }}", "};",
        f"template void base::{fn}<{tag}, {sub:#x}>(char*);"])


def _traits_e9f00(tag, vtable, base_vtable):
    return [f"extern char g_unk_{v:x}[];" for v in sorted({vtable, base_vtable})] + [
        f"template <> struct base::UnkPoolListTraits<{tag}> {{",
        f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
        f"    static char* base_vtable() {{ return g_unk_{base_vtable:x}; }}", "};"]


def pool_list_ctor(orig, addr, tag, insns):
    """refl_PoolListCtor (base::UnkPoolList): both vtables (through the GOT) and the block size."""
    if len(insns) != 70 or insns[29] != ("bl", "#0x710008377c") or insns[37] != ("bl", "#0x71000837f4") \
            or insns[62] != ("bl", "#0x710008378c") or insns[63] != ("add", "x8, x19, #0x30") \
            or got_target(orig, insns, 21) != 0x7101A1A4C8:
        return None
    shift = re.fullmatch(r"x20, x8, #(\d)", insns[50][1])
    elem = re.fullmatch(r"w3, #(0x[0-9a-f]+|\d)", insns[61][1])
    if insns[50][0] != "lsl" or not shift or insns[61][0] != "mov" or not elem \
            or int(elem.group(1), 0) != 1 << int(shift.group(1)):
        return None
    base_vtable, vtable = got_target(orig, insns, 5), got_target(orig, insns, 18)
    if base_vtable is None or vtable is None:
        return None
    e = 1 << int(shift.group(1))
    t = f"base::UnkPoolList<{tag}, 0, {e:#x}>"
    return "\n".join(_traits_e9f00(tag, vtable, base_vtable) + [
        f"template void base::refl_PoolListCtor<{tag}, 0, {e:#x}>({t}*, u32, u32, u32, base::UnkAllocator*);"])


def pool_list_dtor(orig, addr, tag, insns):
    """refl_PoolListDtor (base::UnkPoolList): both vtables (through the GOT) and the node link's
    offset."""
    if len(insns) != 62 or insns[8:10] != [("mov", "w8, #1"), ("strb", "w8, [x0, #0x6c]")] \
            or insns[31] != ("bl", "#0x71000837f4") or insns[11] != ("ldr", "x1, [x21, #0x10]!"):
        return None
    link = re.fullmatch(r"x20, \[x1(?:, #(0x[0-9a-f]+|\d))?\]", insns[17][1])
    if insns[17][0] != "ldr" or not link or insns[47] != insns[17]:
        return None
    vtable, base_vtable = got_target(orig, insns, 4), got_target(orig, insns, 38)
    if vtable is None or base_vtable is None or got_target(orig, insns, 52) != base_vtable:
        return None
    nxt = int(link.group(1) or "0", 16)
    t = f"base::UnkPoolList<{tag}, {nxt:#x}, 0x10>"
    return "\n".join(_traits_e9f00(tag, vtable, base_vtable) + [
        f"template void base::refl_PoolListDtor<{tag}, {nxt:#x}, 0x10>({t}*);"])


_PAGE_aab70 = re.compile(r"(x\d+), #(0x[0-9a-f]+)")
_LO_aab70 = re.compile(r"(x\d+), \[(x\d+), #(0x[0-9a-f]+|\d)\]")
_ADD_aab70 = re.compile(r"(x\d+), (x\d+), #(0x[0-9a-f]+|\d)")


def _page_aab70(insns, i):
    m, o = insns[i]
    hit = _PAGE_aab70.fullmatch(o)
    return (hit.group(1), int(hit.group(2), 16)) if m == "adrp" and hit else (None, None)


def _direct_aab70(insns, i):
    """adrp at i + add at i + 1: the address."""
    reg, page = _page_aab70(insns, i)
    m, o = insns[i + 1]
    hit = _ADD_aab70.fullmatch(o)
    if reg is None or m != "add" or not hit or hit.group(1) != reg or hit.group(2) != reg:
        return None
    return page + int(hit.group(3), 0)


def _got_aab70(orig, insns, i, j):
    """adrp at i + ldr from the GOT at j: the target."""
    reg, page = _page_aab70(insns, i)
    m, o = insns[j]
    hit = _LO_aab70.fullmatch(o)
    if reg is None or m != "ldr" or not hit or hit.group(2) != reg:
        return None
    reloc = orig.img.relocs.get(page + int(hit.group(3), 0) - BASE)
    return None if reloc is None or reloc.type != R_AARCH64_RELATIVE else BASE + reloc.addend


def type_static_init(orig, addr, tag, insns):
    """refl_TypeStaticInit (base::UnkTypeInitTraits): this unit's ids block, the type's name, the
    type object, its factory and initialiser, and the list it is added to."""
    if len(insns) != 81 or insns[7] != ("mov", "w8, #6") or insns[12] != ("str", "w8, [x19], #8") \
            or insns[14] != ("bl", "#0x71000003d4") or insns[22] != ("bl", "#0x7100000250") \
            or insns[75] != ("bl", "#0x710009ec00") or insns[46][0] != "bl" \
            or _direct_aab70(insns, 9) != 0x7101618FAA or _direct_aab70(insns, 24) != 0x71015BA597 \
            or _direct_aab70(insns, 17) != 0x7101A19000 or _direct_aab70(insns, 68) != 0x7101A19000 \
            or _got_aab70(orig, insns, 15, 16) != 0x710000051C or _got_aab70(orig, insns, 64, 66) != 0x7100096604 \
            or _got_aab70(orig, insns, 47, 48) != STRID_POOL:
        return None
    ids, name = _direct_aab70(insns, 5), _direct_aab70(insns, 33)
    typ, factory, lst = _got_aab70(orig, insns, 38, 40), _got_aab70(orig, insns, 39, 41), _got_aab70(orig, insns, 72, 73)
    if None in (ids, name, typ, factory, lst) or _got_aab70(orig, insns, 65, 67) != typ:
        return None
    init = int(insns[46][1][1:], 16)
    return "\n".join([
        f"extern char g_unk_{ids:x}[] __attribute__((visibility(\"hidden\")));",
        f"extern const char g_unk_{name:x}[] __attribute__((visibility(\"hidden\")));",
        f"extern char g_unk_{typ:x}[];", f"extern char g_unk_{lst:x}[];", f"void fn_{factory:x}();",
        f"void fn_{init:x}(char*, const base::global::CStrId&, u64, u64, void (*)(), u64);",
        f"template <> struct base::UnkTypeInitTraits<{tag}> {{",
        f"    static base::UnkTypeInitIds* ids() {{ return reinterpret_cast<base::UnkTypeInitIds*>(g_unk_{ids:x}); }}",
        f"    static const char* name() {{ return g_unk_{name:x}; }}",
        f"    static char* type() {{ return g_unk_{typ:x}; }}",
        f"    static char* list() {{ return g_unk_{lst:x}; }}",
        f"    static void init(char* t, const base::global::CStrId& n) {{ fn_{init:x}(t, n, 0, 0, fn_{factory:x}, 0); }}",
        "};", f"template void base::refl_TypeStaticInit<{tag}>();"])


def map_iter_sync_dtor(orig, addr, tag, insns):
    """refl_MapIterSync for a string-id key and an element with an out-of-line destructor
    (UnkCloneDtorElem): the state offset (= 0x10 + the element size), the destructor and the
    key's pool."""
    if len(insns) != 73 or insns[4:7] != [("ldr", "x20, [x0]"), ("mov", "x19, x0"), ("cbz", "x20, @+a8")] \
            or insns[8] != ("add", "x21, x20, #8") or insns[13] != ("add", "x0, x19, #0x10") \
            or insns[30] != ("bl", f"#{RELEASE:#x}") or insns[37] != ("stp", "x20, x21, [x19, #8]") \
            or insns[14][0] != "bl" or insns[48] != insns[14]:
        return None
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", insns[7][1])
    if insns[7][0] != "ldr" or not state or insns[42] != insns[7]:
        return None
    state = int(state.group(1), 16)
    if state % 8 or state <= 0x10 or insns[34] != ("str", f"wzr, [x19, #{state:#x}]") \
            or insns[68] != insns[34]:
        return None
    pool = got_target(orig, insns, 15)
    if pool not in (STRID_POOL, FILE_PATH_POOL) or got_target(orig, insns, 49) != pool:
        return None
    dtor = int(insns[14][1][1:], 16)
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return "\n".join([f"void fn_{dtor:x}(char*);",
                      f"template <> struct UnkCloneDtorTraits<{tag}> {{",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};",
                      f"template void base::refl_MapIterSync(base::UnkMapIter<UnkCloneDtorElem<{tag}, "
                      f"{state - 0x10:#x}>, {ident}>*);"])


ELEM_BASE_VTABLE = 0x7101CAB1F8
_VALUE_5fbb8 = {"w8": "UnkCloneElem<{tag}, 4>", "x8": "UnkCloneElem<{tag}, 8>"}


def _got_5fbb8(orig, insns, i, j):
    """The GOT target for an adrp at i and its ldr at j (not adjacent)."""
    return got_target(orig, [insns[i], insns[j]], 0)


def id_value_push_back(orig, addr, tag, insns):
    """refl_IdValuePushBack (base::UnkIdValueElem): the element's vtable (through the GOT) and the
    value's width."""
    if len(insns) != 116 or insns[29:31] != [("mov", "w8, #0x18"), ("umull", "x1, w25, w8")] \
            or insns[51] != ("ldr", "x8, [x8, #8]") or insns[47] != ("str", "x23, [x21]") \
            or insns[81] != ("ldr", "x8, [x8, #0x48]") \
            or _got_5fbb8(orig, insns, 20, 22) != ELEM_BASE_VTABLE or _got_5fbb8(orig, insns, 74, 77) != ELEM_BASE_VTABLE \
            or _got_5fbb8(orig, insns, 21, 24) != STRID_POOL:
        return None
    vtable = _got_5fbb8(orig, insns, 23, 26)
    value = _VALUE_5fbb8.get(insns[48][1].split(",")[0])
    if vtable is None or _got_5fbb8(orig, insns, 73, 76) != vtable or value is None \
            or insns[48] != ("ldr", f"{insns[48][1].split(',')[0]}, [x22, #0x10]"):
        return None
    e = f"base::UnkIdValueElem<{tag}, {value.format(tag=tag)}>"
    return "\n".join([f"extern char g_unk_{vtable:x}[];",
                      f"template <> struct base::UnkIdValueTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}", "};",
                      f"template void base::refl_IdValuePushBack(base::UnkVector<{e}>*, const {e}&);"])


def id_value_assign(orig, addr, tag, insns):
    """refl_IdValueAssign (base::UnkIdValueElem): the element's vtable (through the GOT), the
    value's width and the push_back it calls."""
    if len(insns) != 105 or insns[7:9] != [("mov", "x19, x0"), ("cmp", "x0, x1")] \
            or insns[18] != ("ldr", "x8, [x8, #8]") or insns[54] != ("str", "x27, [x28]") \
            or insns[82] != ("ldr", "x8, [x8, #0x48]") or insns[93][0] != "bl" \
            or not insns[93][1].startswith("#0x") \
            or _got_5fbb8(orig, insns, 43, 45) != ELEM_BASE_VTABLE or _got_5fbb8(orig, insns, 44, 47) != STRID_POOL:
        return None
    vtable = _got_5fbb8(orig, insns, 46, 49)
    reg = insns[55][1].split(",")[0]
    value = _VALUE_5fbb8.get(reg)
    if vtable is None or value is None or insns[55] != ("ldr", f"{reg}, [x22, #0x10]"):
        return None
    push = int(insns[93][1][1:], 16)
    e = f"base::UnkIdValueElem<{tag}, {value.format(tag=tag)}>"
    return "\n".join([f"extern char g_unk_{vtable:x}[];",
                      f"void fn_{push:x}(base::UnkVector<{e}>*, const {e}&);",
                      f"template <> struct base::UnkIdValueTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vtable:x}; }}",
                      f"    static void push_back(base::UnkVector<{e}>* v, const {e}& x) {{ fn_{push:x}(v, x); }}", "};",
                      f"template base::UnkVector<{e}>* base::refl_IdValueAssign(base::UnkVector<{e}>*, "
                      f"const base::UnkVector<{e}>*);"])


def owner_vec_destroy(orig, addr, tag, insns):
    """refl_IterDestroy / refl_IterValueDestroy on UnkCloneOwnerVec: the state's offset picks one."""
    if len(insns) != 42 or insns[3][0] != "ldr" or insns[20] != ("blr", "x8") or insns[36] != ("blr", "x8"):
        return None
    m = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[3][1])
    if not m or insns[19][1] != "x8, [x8, #0x10]" or insns[35][1] != "x8, [x8, #0x48]":
        return None
    state = int(m.group(1), 16)
    if state == 0x40:
        return f"template void base::refl_IterDestroy(base::UnkCollectionIter<UnkCloneOwnerVec<{tag}>, u32>*);"
    if state == 0x30:
        return f"template void base::refl_IterValueDestroy<UnkCloneOwnerVec<{tag}>, u32>(void*);"
    return None


def call_and_cast_field_register(orig, addr, tag, insns):
    """refl_CallAndCastFieldRegister (0x71003b8880.h): the base method, type getter, source and
    destination offsets, the registered sub-object, the member offset and the callee."""
    if len(insns) != 43 or insns[17] != ("bl", f"#{IS_KIND_OF:#x}") or insns[35] != ("ldr", "x8, [x20, x9]") \
            or insns[25] != ("ldr", "w10, [x20, #0x1618]") or insns[32] != ("stur", "x0, [x8, #4]"):
        return None
    calls = [int(o[1:], 16) for m, o in (insns[5], insns[14], insns[37]) if m == "bl" and o.startswith("#0x")]
    src = re.fullmatch(r"x21, \[x20, #(0x[0-9a-f]+|\d)\]", insns[6][1])
    off = re.fullmatch(r"x1, x8, #(0x[0-9a-f]+|\d)", insns[36][1])
    dst, sub = wide_constant32(insns[22:24], "w9"), wide_constant32(insns[26:28], "w8")
    if len(calls) != 3 or not src or not off or dst is None or sub is None:
        return None
    base, getter, callee = calls
    return "\n".join(cast_traits(tag, (base, getter), "bool", "char* s", "s") + [
        f"void fn_{callee:x}(char*, char*);",
        f"template <> struct base::reflection::UnkCastRegisterTraits<{tag}> {{",
        f"    static void call(char* a, char* b) {{ fn_{callee:x}(a, b); }}", "};",
        f"template bool base::reflection::refl_CallAndCastFieldRegister<{tag}, {int(src.group(1), 0):#x}, "
        f"{dst:#x}, {sub:#x}, {int(off.group(1), 0):#x}>(char*);"])


def iter_value_move_assign_index(orig, addr, tag, insns):
    """UnkIterValue::operator=(&&) with a plain index (u32 or u64) for an element with an
    out-of-line copy-assignment (UnkCloneAssignElem). Facts: the state offset, index width and assignment."""
    if len(insns) != 45 or insns[3:5] != [("mov", "x19, x0"), ("cmp", "x0, x1")] or insns[32][0] != "bl" \
            or insns[26] != ("add", "x1, x1, #8") or insns[39] != ("ldr", "x1, [x1, #8]"):
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[6][1])
    width = re.fullmatch(r"([wx])8, \[x1\]", insns[13][1])
    if insns[6][0] != "ldr" or not state or not width:
        return None
    state = int(state.group(1), 16)
    if state % 8 or state < 0x10:
        return None
    k = f"UnkCloneIndexU64<{tag}>" if width.group(1) == "x" else "u32"
    assign = int(insns[32][1][1:], 16)
    t = f"UnkCloneAssignElem<{tag}, {state - 8:#x}>"
    return "\n".join([f"void fn_{assign:x}(void*, const void*);",
                      f"template <> struct UnkCloneAssignTraits<{tag}> {{",
                      f"    static void assign(void* dst, const void* src) {{ fn_{assign:x}(dst, src); }}", "};",
                      f"template base::UnkIterValue<{t}, {k}>& base::UnkIterValue<{t}, {k}>::operator=("
                      f"base::UnkIterValue<{t}, {k}>&&);"])


def iter_value_copy_assign_index(orig, addr, tag, insns):
    """UnkIterValue::operator=(const&) with a plain index (u32 or u64) for an element with an
    out-of-line copy-assignment (UnkCloneAssignElem). Facts: the state offset, index width and assignment."""
    if len(insns) != 38 or insns[3:5] != [("mov", "x19, x0"), ("cmp", "x0, x1")] or insns[33][0] != "bl" \
            or insns[24] != ("add", "x1, x1, #8") or insns[29] != ("add", "x0, x19, #8"):
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[6][1])
    width = re.fullmatch(r"([wx])8, \[x8\]", insns[12][1])
    if insns[6][0] != "ldr" or not state or not width:
        return None
    state = int(state.group(1), 16)
    if state % 8 or state < 0x10:
        return None
    k = f"UnkCloneIndexU64<{tag}>" if width.group(1) == "x" else "u32"
    assign = int(insns[33][1][1:], 16)
    t = f"UnkCloneAssignElem<{tag}, {state - 8:#x}>"
    return "\n".join([f"void fn_{assign:x}(void*, const void*);",
                      f"template <> struct UnkCloneAssignTraits<{tag}> {{",
                      f"    static void assign(void* dst, const void* src) {{ fn_{assign:x}(dst, src); }}", "};",
                      f"template base::UnkIterValue<{t}, {k}>& base::UnkIterValue<{t}, {k}>::operator=("
                      f"const base::UnkIterValue<{t}, {k}>&);"])


def call_with_owner_id(orig, addr, tag, insns):
    """refl_CallWithOwnerId: the base method, the member's offset and the method it calls."""
    if len(insns) != 38 or insns[4][0] != "bl" or insns[19][0] != "bl" or insns[5] != ("ldr", "x8, [x19, #0x18]") \
            or insns[8] != ("ldr", "x8, [x8, #0x30]") or insns[31] != ("bl", f"#{RELEASE:#x}") \
            or got_at(orig, insns, 7, 10) != STRID_POOL:
        return None
    off = re.fullmatch(r"x0, x19, #(0x[0-9a-f]+)", insns[6][1])
    if insns[6][0] != "add" or not off:
        return None
    base, call = int(insns[4][1][1:], 16), int(insns[19][1][1:], 16)
    return "\n".join([f"void fn_{base:x}(char*);", f"void fn_{call:x}(char*, base::UnkStrIdMember*);",
                      f"template <> struct base::UnkCallWithIdTraits<{tag}> {{",
                      f"    static void base(char* p) {{ fn_{base:x}(p); }}",
                      f"    static void call(char* m, base::UnkStrIdMember* id) {{ fn_{call:x}(m, id); }}", "};",
                      f"template void base::refl_CallWithOwnerId<{tag}, {off.group(1)}>(char*);"])


def iter_value_from_ref(tag, ins):
    """refl_IterValueFromRef: the value's size (from the state offset)."""
    state = re.fullmatch(r"wzr, \[x0, #(0x[0-9a-f]+)\]", ins[0][1])
    if not state or int(state.group(1), 16) < 0x10 or int(state.group(1), 16) % 8:
        return None
    v = f"base::UnkIterValue<UnkCloneBlock<{tag}, {int(state.group(1), 16) - 8:#x}>>"
    return f"template void base::refl_IterValueFromRef({v}*, const {v}::Ref*);"


def iter_value_move_ool(orig, addr, tag, insns):
    """refl_IterValueMove for a u32 index and an element with an out-of-line move constructor
    (UnkCloneMoveElem): the state offset and the move."""
    if len(insns) != 17 or insns[7] != ("ldr", "w8, [x1], #8") or insns[13] != ("ldr", "w8, [x8]") \
            or insns[9][0] != "b" or insns[15] != insns[9]:
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[1][1])
    if not state or int(state.group(1), 16) < 0x10 or int(state.group(1), 16) % 8:
        return None
    move = int(insns[9][1][1:], 16)
    return "\n".join([f"void fn_{move:x}(void*, void*);", f"template <> struct UnkCloneMoveTraits<{tag}> {{",
                      f"    static void move(void* dst, void* src) {{ fn_{move:x}(dst, src); }}", "};",
                      f"template void base::refl_IterValueMove<UnkCloneMoveElem<{tag}, {int(state.group(1), 16) - 8:#x}>>("
                      "void*, const void*);"])


def iter_value_move_ool_strid(orig, addr, tag, insns):
    """The same keyed by a string id: the state offset, the keys' pool and the move."""
    if len(insns) != 27 or insns[10] != ("str", "x8, [x0], #8") or insns[14][0] != "b" or insns[25] != insns[14]:
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[1][1])
    pool = got_target(orig, insns, 8)
    if not state or int(state.group(1), 16) < 0x10 or int(state.group(1), 16) % 8 \
            or pool not in (STRID_POOL, FILE_PATH_POOL) or got_target(orig, insns, 20) != pool:
        return None
    move = int(insns[14][1][1:], 16)
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return "\n".join([f"void fn_{move:x}(void*, void*);", f"template <> struct UnkCloneMoveTraits<{tag}> {{",
                      f"    static void move(void* dst, void* src) {{ fn_{move:x}(dst, src); }}", "};",
                      f"template void base::refl_IterValueMove<UnkCloneMoveElem<{tag}, {int(state.group(1), 16) - 8:#x}>, "
                      f"{ident}>(void*, const void*);"])


def keyed_find(orig, addr, tag, insns):
    """refl_KeyedFind: the array's offset (from the count's) and the value's size."""
    if len(insns) != 27 or insns[25] != ("csel", "x0, xzr, x9, eq") or insns[7] != ("ldr", "x13, [x10]"):
        return None
    count = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[0][1])
    size = re.fullmatch(r"w9, #(0x[0-9a-f]+)", insns[17][1])
    if not count or not size or int(count.group(1), 16) < 0x10 or int(size.group(1), 16) % 8:
        return None
    off = int(count.group(1), 16) - 0x10
    return f"template void* base::refl_KeyedFind<{tag}, {off:#x}, {size.group(1)}>(char*, const base::UnkStrIdMember*);"


def keyed_find_then(orig, addr, tag, insns):
    """refl_KeyedFindThen: the value's size and the call."""
    if len(insns) != 31 or insns[0] != ("ldr", "w8, [x0, #0x10]") or insns[23][0] != "b" \
            or insns[7] != ("ldr", "x13, [x10]") or insns[29] != ("b.ne", "@+5c"):
        return None
    size = re.fullmatch(r"w9, #(0x[0-9a-f]+)", insns[17][1])
    if not size or int(size.group(1), 16) % 8:
        return None
    call = int(insns[23][1][1:], 16)
    return "\n".join([f"void fn_{call:x}(char*, void*);", f"template <> struct base::UnkKeyedFoundTraits<{tag}> {{",
                      f"    static void found(char* s, void* v) {{ fn_{call:x}(s, v); }}", "};",
                      f"template void base::refl_KeyedFindThen<{tag}, 0, {size.group(1)}>(char*, const base::UnkStrIdMember*);"])


def strid_dtor_at(orig, addr, tag, insns):
    """refl_StrIdDtorAt: the id's offset and pool."""
    if len(insns) != 26 or insns[19] != ("bl", f"#{RELEASE:#x}") or insns[5] != ("mov", "x19, x0"):
        return None
    pool = got_target(orig, insns, 3)
    at = re.fullmatch(r"x8, \[x19(?:, #(0x[0-9a-f]+|\d))?\]", insns[8][1])
    if pool not in (STRID_POOL, FILE_PATH_POOL) or not at or insns[22][1] != insns[8][1]:
        return None
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return f"template void base::refl_StrIdDtorAt<{tag}, {int(at.group(1) or '0', 0):#x}, {ident}>(char*);"


def iter_value_destroy_key16(orig, addr, tag, insns):
    """refl_IterValueDestroy over a 16-byte key with a string id at +8 and an element with an
    out-of-line destructor: the state offset (the element's size), the destructor and the pool."""
    if len(insns) != 34 or insns[9] != ("add", "x0, x19, #0x10") or insns[10][0] != "bl" \
            or insns[15] != ("ldr", "x8, [x19, #8]") or insns[26] != ("bl", f"#{RELEASE:#x}"):
        return None
    state = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[3][1])
    pool = got_target(orig, insns, 11)
    if not state or int(state.group(1), 16) < 0x18 or int(state.group(1), 16) % 8 \
            or pool not in (STRID_POOL, FILE_PATH_POOL) or insns[30] != ("str", f"wzr, [x19, #{state.group(1)}]"):
        return None
    ident = f"base::UnkStrIdMemberOf<g_unk_{pool:x}>"
    return dtor_traits(tag, insns[10]) + (
        f"template void base::refl_IterValueDestroy<UnkCloneDtorElem<{tag}, {int(state.group(1), 16) - 0x10:#x}>, "
        f"UnkCloneStrIdElem<{tag}, 8, 16, {ident}>>(void*);")


def keyed_find_assign(orig, addr, tag, insns):
    """refl_KeyedFindAssign: the value's size and its assignment."""
    if len(insns) != 36 or insns[2] != ("ldr", "w8, [x0, #0x10]") or insns[25] != ("mov", "x1, x2") \
            or insns[26][0] != "bl" or insns[9] != ("ldr", "x13, [x10]"):
        return None
    size = re.fullmatch(r"w9, #(0x[0-9a-f]+)", insns[19][1])
    if not size or int(size.group(1), 16) % 8:
        return None
    assign = int(insns[26][1][1:], 16)
    return "\n".join([f"void fn_{assign:x}(void*, const void*);", f"template <> struct UnkCloneAssignTraits<{tag}> {{",
                      f"    static void assign(void* dst, const void* src) {{ fn_{assign:x}(dst, src); }}", "};",
                      f"template void base::refl_KeyedFindAssign<{tag}, 0, {size.group(1)}>(char*, "
                      f"const base::UnkStrIdMember*, const UnkCloneAssignElem<{tag}, {size.group(1)}>*);"])


def vector_free_dtor(orig, addr, tag, insns):
    """UnkVector<T>::Free() for elements with an out-of-line destructor (UnkCloneDtorElem): the
    element's size and destructor."""
    if len(insns) != 35 or insns[10][0] != "bl" or insns[18] != insns[10] or insns[26] != ("str", "wzr, [x19, #8]") \
            or insns[30] != ("str", "wzr, [x19, #0xc]") or insns[28] != ("ldr", "x8, [x8, #0x48]"):
        return None
    size = re.fullmatch(r"w21, #(0x[0-9a-f]+)", insns[15][1])
    if not size or int(size.group(1), 16) % 8 or insns[21] != ("add", f"x21, x21, #{size.group(1)}"):
        return None
    return dtor_traits(tag, insns[10]) + f"template void base::UnkVector<UnkCloneDtorElem<{tag}, {size.group(1)}>>::Free();"


def value_holder_dtor(orig, addr, tag, insns):
    """refl_ValueHolderDtor: the base class's vtable (through the GOT)."""
    if len(insns) != 11 or insns[0] != ("stp", "xzr, xzr, [x0, #0x60]") or insns[8] != ("stp", "xzr, xzr, [x0, #0x30]") \
            or insns[6] != ("add", "x8, x0, #0x18") or insns[10] != ("b", f"#{0x71000A1C60:#x}"):
        return None
    vt = got_at(orig, insns, 1, 4)
    if vt is None:
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"template <> struct base::UnkValueHolderTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_ValueHolderDtor(base::UnkValueHolder<{tag}>*);"])


def zeroed_ctor(orig, addr, tag, insns):
    """refl_ZeroedCtor: the class's vtable (through the GOT)."""
    if len(insns) != 7 or insns[2:4] != [("str", "wzr, [x0, #0x20]"), ("stp", "xzr, xzr, [x0, #0x10]")] \
            or insns[5] != ("str", "x8, [x0]"):
        return None
    vt = got_target(orig, insns, 0)
    if vt is None:
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"template <> struct base::UnkDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_ZeroedCtor<{tag}>(char*);"])


def dtor_call_clear(orig, addr, tag, insns):
    """refl_DtorCallClear: the method, the base's vtable, the cleared field and the next base."""
    if len(insns) != 14 or insns[3] != ("mov", "x19, x0") or insns[4][0] != "bl" or insns[13][0] != "b" \
            or insns[9] != ("str", "x8, [x19]"):
        return None
    vt = got_at(orig, insns, 5, 6)
    field = re.fullmatch(r"([wx])zr, \[x19, #(0x[0-9a-f]+|\d)\]", insns[7][1])
    if vt is None or insns[7][0] != "str" or not field or int(field.group(2), 0) < 8:
        return None
    call, base = int(insns[4][1][1:], 16), int(insns[13][1][1:], 16)
    t = "u64" if field.group(1) == "x" else "u32"
    return "\n".join([f"extern char g_unk_{vt:x}[];"] + [f"void fn_{x:x}(char*);" for x in sorted({call, base})] + [
        f"template <> struct base::UnkDtorTraits<{tag}> {{", f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
        f"template <> struct base::UnkDtorCallTraits<{tag}> {{",
        f"    static void call(char* p) {{ fn_{call:x}(p); }}", f"    static void base(char* p) {{ fn_{base:x}(p); }}", "};",
        f"template void base::refl_DtorCallClear<{tag}, {t}, {int(field.group(2), 0):#x}>(char*);"])


def add_f32(orig, addr, tag, insns):
    """refl_AddF32: the field's offset."""
    if len(insns) != 4 or insns[1] != ("fadd", "s0, s1, s0") or insns[3] != ("ret", ""):
        return None
    off = re.fullmatch(r"s1, \[x0, #(0x[0-9a-f]+|\d)\]", insns[0][1])
    if insns[0][0] != "ldr" or not off or insns[2] != ("str", f"s0, [x0, #{off.group(1)}]"):
        return None
    return f"template void base::refl_AddF32<{tag}, {int(off.group(1), 0):#x}>(char*, f32);"


def pair_counts(orig, addr, tag, insns):
    """refl_InitPairCounts (through x0) or refl_MakePairCounts (returned through x8)."""
    for base, fn, sig in (("x0", "refl_InitPairCounts", "(base::UnkPairCounts<{t}>*)"), ("x8", "refl_MakePairCounts", "()")):
        r = "x8" if base == "x0" else "x9"
        if insns == [("mov", f"{r}, #2"), ("movk", f"{r}, #1, lsl #32"), ("str", f"{r}, [{base}, #0x10]"),
                     ("stp", f"xzr, xzr, [{base}]"), ("ret", "")]:
            ret = "void" if base == "x0" else f"base::UnkPairCounts<{tag}>"
            return f"template {ret} base::{fn}<{tag}>{sig.format(t=tag)};"
    return None


def vector_clear_dtor(orig, addr, tag, insns):
    """UnkVector<T>::Clear() for elements with an out-of-line destructor (UnkCloneDtorElem): the
    element's size and destructor."""
    if len(insns) != 22 or insns[4] != ("ldr", "w8, [x0, #8]") or insns[8] != ("mov", "x21, xzr") \
            or insns[11][0] != "bl" or insns[17] != ("str", "wzr, [x19, #8]"):
        return None
    size = re.fullmatch(r"x20, x20, #(0x[0-9a-f]+)", insns[14][1])
    if not size or int(size.group(1), 16) % 8:
        return None
    return dtor_traits(tag, insns[11]) + f"template void base::UnkVector<UnkCloneDtorElem<{tag}, {size.group(1)}>>::Clear();"


def iter_value_destroy_refptr(orig, addr, tag, insns):
    """refl_IterValueDestroy for a u32 index and an element holding one refcounted pointer: the
    state offset (the element's size) and the pointer's offset."""
    if len(insns) != 22 or insns[16] != ("ldr", "x8, [x8, #0x10]") or insns[12] != ("subs", "w8, w8, #1"):
        return None
    state = re.fullmatch(r"w8, \[x0, #(0x[0-9a-f]+)\]", insns[3][1])
    ptr = re.fullmatch(r"x0, \[x19, #(0x[0-9a-f]+|\d)\]", insns[9][1])
    if not state or not ptr or insns[18] != ("str", f"wzr, [x19, #{state.group(1)}]"):
        return None
    size, off = int(state.group(1), 16) - 8, int(ptr.group(1), 0) - 8
    if size < 8 or size % 8 or not 0 <= off <= size - 8:
        return None
    return (f"template void base::refl_IterValueDestroy<UnkCloneStrIdElem<{tag}, {off:#x}, {size:#x}, "
            f"base::UnkRefPtrMember>>(void*);")


def owner_dtor(orig, addr, tag, insns):
    """refl_OwnerDtor (0x71001f2ae4.h): both vtables (through the GOT) and the owned object's
    destructor."""
    if len(insns) != 53 or insns[5] != ("ldr", "x20, [x0, #0x108]") or insns[14] != ("bl", f"#{OPERATOR_DELETE:#x}") \
            or insns[23] != ("bl", "#0x7100216934") or insns[30] != ("bl", "#0x710001737c") or insns[12][0] != "bl" \
            or got_target(orig, insns, 31) != STRID_POOL or insns[46] != ("bl", f"#{RELEASE:#x}"):
        return None
    vt, base_vt = got_target(orig, insns, 3), got_target(orig, insns, 15)
    if vt is None or base_vt is None:
        return None
    dtor = int(insns[12][1][1:], 16)
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"extern char g_unk_{base_vt:x}[];", f"void fn_{dtor:x}(char*);",
                      f"template <> struct base::UnkOwnerDtorTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}",
                      f"    static char* base_vtable() {{ return g_unk_{base_vt:x}; }}",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};",
                      f"template void base::refl_OwnerDtor<{tag}>(char*);"])


def push_then_next(orig, addr, tag, insns):
    """refl_PushThenNext (0x7100302b38.h): the getter and the tail-called next step."""
    if len(insns) != 55 or insns[5][0] != "bl" or insns[54][0] != "b" or insns[46] != ("str", "x20, [x21, w8, uxtw #3]") \
            or insns[6] != ("ldp", "w8, w9, [x19, #0x10]") or insns[34] != ("bl", "#0x7101238e00"):
        return None
    get, nxt = int(insns[5][1][1:], 16), int(insns[54][1][1:], 16)
    return "\n".join([f"const base::reflection::CType* fn_{get:x}();", f"void fn_{nxt:x}(char*);",
                      f"template <> struct base::UnkPushNextTraits<{tag}> {{",
                      f"    static const base::reflection::CType* get() {{ return fn_{get:x}(); }}",
                      f"    static void next(char* p) {{ fn_{nxt:x}(p); }}", "};",
                      f"template void base::refl_PushThenNext<{tag}>(char*);"])


def hash_map_init(orig, addr, tag, insns):
    """refl_HashMapInit (0x7100075958.h): the node size."""
    if len(insns) != 85 or insns[13] != ("bl", "#0x71000837f4") or insns[38] != ("bl", "#0x710008378c") \
            or insns[39] != ("add", "x8, x20, #0x48") or insns[78] != ("bl", "#0x7101238df0"):
        return None
    size = re.fullmatch(r"w24, #(0x[0-9a-f]+|\d)", insns[26][1])
    if insns[26][0] != "mov" or not size:
        return None
    return f"template void base::refl_HashMapInit<{tag}, {int(size.group(1), 0):#x}>(char*, u32, u32);"


def push_two_types(orig, addr, tag, insns):
    """refl_PushTwoTypes (0x7100601f24.h): the first type (through the GOT) and the getter."""
    if len(insns) != 103 or insns[46] != ("str", "x9, [x20, w8, uxtw #3]") or insns[50][0] != "bl" \
            or insns[95] != ("str", "x20, [x21, w8, uxtw #3]") or insns[32] != ("bl", "#0x7101238e00"):
        return None
    first = got_target(orig, insns, 44)
    if first is None:
        return None
    get = int(insns[50][1][1:], 16)
    return "\n".join([f"extern char g_unk_{first:x}[];", f"const base::reflection::CType* fn_{get:x}();",
                      f"template <> struct base::UnkPushTwoTraits<{tag}> {{",
                      f"    static const base::reflection::CType* first() {{ return reinterpret_cast<const base::reflection::CType*>(g_unk_{first:x}); }}",
                      f"    static const base::reflection::CType* get() {{ return fn_{get:x}(); }}", "};",
                      f"template void base::refl_PushTwoTypes<{tag}>(char*);"])


def keyed_set_assign(orig, addr, tag, insns):
    """refl_KeyedSetAssign: the lookup, the id's pool and offset, and the value's width."""
    if len(insns) != 38 or insns[4:6] != [("ldr", "w1, [x1]"), ("mov", "x19, x2")] or insns[6][0] != "bl" \
            or insns[30] != ("bl", f"#{RELEASE:#x}") or insns[37] != ("ret", ""):
        return None
    off = re.fullmatch(r"x8, \[x0(?:, #(0x[0-9a-f]+|\d))?\]", insns[7][1])
    val = re.fullmatch(r"([wx])8, \[x19, #(0x[0-9a-f]+|\d)\]", insns[32][1])
    pool = got_target(orig, insns, 12)
    if insns[7][0] != "ldr" or not off or not val or insns[32][0] != "ldr" or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    off = int(off.group(1) or "0", 16)
    if int(val.group(2), 16) != off + 8:
        return None
    find = int(insns[6][1][1:], 16)
    t = "u32" if val.group(1) == "w" else "u64"
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    return "\n".join([f"char* fn_{find:x}(void*, u32);",
                      f"template <> struct base::UnkKeyedSetTraits<{tag}> {{",
                      f"    static char* find(void* m, u32 k) {{ return fn_{find:x}(m, k); }}", "};",
                      f"template void base::refl_KeyedSetAssign<{tag}, {ident}, {t}, {off:#x}>(void*, const u32*, const char*);"])


def keyed_set_iter_value_copy(orig, addr, tag, insns):
    """refl_IterValueCopy over a UnkKeyedSetEntry: the state offset, the id's pool and offset and
    the value's width."""
    if len(insns) != 74 or insns[4:7] != [("mov", "x19, x0"), ("cmp", "x0, x1"), ("b.eq", "@+114")] \
            or insns[65] != ("bl", f"#{RELEASE:#x}") or insns[69] != ("mov", "x0, x19"):
        return None
    state = re.fullmatch(r"w8, \[x1, #(0x[0-9a-f]+)\]", insns[7][1])
    off = re.fullmatch(r"x8, \[x21(?:, #(0x[0-9a-f]+|\d))?\]", insns[32][1])
    val = re.fullmatch(r"([wx])8, \[x20, #(0x[0-9a-f]+|\d)\]", insns[67][1])
    pool = got_target(orig, insns, 47)
    if not state or not off or not val or insns[67][0] != "ldr" or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    state, off = int(state.group(1), 16), int(off.group(1) or "0", 16)
    if int(val.group(2), 16) != off + 8 or off % 8 or state != off + 0x18:
        return None
    t = "u32" if val.group(1) == "w" else "u64"
    ident = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    entry = f"base::UnkKeyedSetEntry<{tag}, {ident}, {t}, {off:#x}>"
    return f"template void* base::refl_IterValueCopy<{entry}>(void*, const void*);"


def _vt_iter_606b8(orig, tag, insns, which, frame_i, sync_i, base_i):
    frame = re.fullmatch(r"sp, sp, #(0x[0-9a-f]+)", insns[0][1])
    state = re.fullmatch(r"wzr, \[sp, #(0x[0-9a-f]+)\]", insns[frame_i][1])
    base, pool = got_target(orig, insns, base_i), got_target(orig, insns, base_i + 4)
    if insns[0][0] != "sub" or not frame or not state or insns[sync_i][0] != "bl" or base is None \
            or pool != STRID_POOL:
        return None
    size = int(state.group(1), 16) - 0x20
    if size < 0x10 or size % 8:
        return None
    sync = int(insns[sync_i][1][1:], 16)
    e = f"base::UnkStrIdVtElem<{tag}, {size:#x}>"
    return "\n".join([f"extern char g_unk_{base:x}[];", f"void fn_{sync:x}(void*);",
                      f"template <> struct base::UnkVtIterTraits<{tag}> {{",
                      f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
                      f"    static void sync(void* it) {{ fn_{sync:x}(it); }}", "};",
                      f"template void base::refl_Vt{which}(const base::UnkVector<{e}>*, "
                      f"base::UnkCollectionIter<{e}>*);"])


def vt_iter_end(orig, addr, tag, insns):
    """refl_VtEnd (fn_7100f606b8): the element size (from the stride), base vtable and sync."""
    if len(insns) != 61 or insns[4] != ("ldr", "w8, [x0, #8]") or insns[47] != ("bl", f"#{RELEASE:#x}"):
        return None
    stride = re.fullmatch(r"w10, #(0x[0-9a-f]+)", insns[5][1])
    out = _vt_iter_606b8(orig, tag, insns, "End", 10, 22, 28)
    if not stride or not out or f"{int(stride.group(1), 16):#x}>" not in out:
        return None
    return out


def vt_iter_begin(orig, addr, tag, insns):
    """refl_VtBegin (fn_7100f605d0): the element size (from the frame), base vtable and sync."""
    if len(insns) != 58 or insns[4:6] != [("ldr", "x8, [x0]"), ("str", "wzr, [sp, #8]")] \
            or insns[44] != ("bl", f"#{RELEASE:#x}"):
        return None
    return _vt_iter_606b8(orig, tag, insns, "Begin", 7, 19, 25)


def vt_iter_sync(orig, addr, tag, insns):
    """refl_IterSync over UnkStrIdVtElem (0x7100f606b8.h): the element size (from the state
    offset) and the base vtable."""
    if len(insns) != 76 or insns[4:7] != [("ldr", "x20, [x0, #8]"), ("mov", "x19, x0"), ("cbz", "x20, @+ac")] \
            or insns[31] != ("bl", f"#{RELEASE:#x}") or insns[38] != ("stp", "x19, x20, [x19, #0x10]"):
        return None
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", insns[7][1])
    base, pool = got_target(orig, insns, 12), got_target(orig, insns, 16)
    if insns[7][0] != "ldr" or not state or base is None or pool != STRID_POOL:
        return None
    size = int(state.group(1), 16) - 0x18
    if size < 0x10 or size % 8:
        return None
    e = f"base::UnkStrIdVtElem<{tag}, {size:#x}>"
    return "\n".join([f"extern char g_unk_{base:x}[];",
                      f"template <> struct base::UnkVtIterTraits<{tag}> {{",
                      f"    static char* base_vtable() {{ return g_unk_{base:x}; }}", "};",
                      f"template void base::refl_IterSync(base::UnkCollectionIter<{e}, u32>*);"])


def owner_vec_insert(orig, addr, tag, insns):
    """refl_OwnerVecInsert: the EmplaceBack it tail-calls (the only varying fact)."""
    if len(insns) != 160 or insns[10:12] != [("cmp", "w22, w2"), ("b.ne", "@+54")] or insns[20][0] != "b" \
            or insns[62] != ("stp", "xzr, xzr, [x1], #0x28") or insns[121] != ("ldr", "x8, [x8, #0x10]") \
            or insns[149] != ("stp", "xzr, xzr, [x20]"):
        return None
    append = int(insns[20][1][1:], 16)
    e = f"UnkCloneOwnerVecHandle<{tag}>"
    return "\n".join([f"void fn_{append:x}(void*, void*);",
                      f"template <> struct base::UnkOwnerVecInsertTraits<{tag}> {{",
                      f"    static void append(void* v, void* e) {{ fn_{append:x}(v, e); }}", "};",
                      f"template void base::refl_OwnerVecInsert(base::UnkVector<{e}>*, {e}*, u32);"])


def dtor_traits(tag, call):
    """UnkCloneDtorTraits naming the destructor `call` (a bl) calls."""
    dtor = int(call[1][1:], 16)
    return "\n".join([f"void fn_{dtor:x}(char*);", f"template <> struct UnkCloneDtorTraits<{tag}> {{",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};", ""])


def variant(tag, ins, fn, param):
    """UnkStrIdValueVariant: the value's size (from the state offset) and destructor."""
    state = re.fullmatch(r"w8, \[x19, #(0x[0-9a-f]+)\]", ins[7][1])
    if not state or int(state.group(1), 16) < 0x18:
        return None
    v = f"base::UnkStrIdValueVariant<{tag}, {int(state.group(1), 16) - 8:#x}>"
    return dtor_traits(tag, ins[14]) + f"template {v}& base::{fn}({v}*, const {param.format(v=v)});"


def keyed_value_array_dtor(tag, ins):
    """UnkKeyedValueArray's destructor: the value's size (the loop's stride) and destructor."""
    stride = re.fullmatch(r"w21, #(0x[0-9a-f]+)", ins[47][1])
    keys = re.fullmatch(r"x8, \[x0, #(0x[0-9a-f]+|\d)\]", ins[4][1])
    if not stride or not keys or int(keys.group(1), 0) < 8:
        return None
    e = f"UnkCloneDtorElem<{tag}, {stride.group(1)}>"
    return dtor_traits(tag, ins[42]) + (
        f"template void base::refl_KeyedValueArrayDtor<{tag}, {e}, {int(keys.group(1), 0) - 8:#x}>(char*);")


def iter_assign_block(tag, ins):
    """UnkCollectionIter::operator= over a block element: its size, from the state offset."""
    state = re.fullmatch(r"w9, \[x0, #(0x[0-9a-f]+)\]", ins[5][1])
    if not state or int(state.group(1), 16) < 0x20 or int(state.group(1), 16) % 8:
        return None
    it = f"base::UnkCollectionIter<UnkCloneBlock<{tag}, {int(state.group(1), 16) - 0x18:#x}>>"
    return f"template {it}& {it}::operator=(const {it}&);"


def keyed_value_get(orig, addr, tag, insns):
    """refl_GetKeyedValue: the value type's getter (called twice; the only varying fact)."""
    if len(insns) != 65 or insns[5] != ("ldr", "w8, [x0, #0x30]") or insns[8] != ("ldr", "x10, [x0, #0x28]") \
            or insns[22] != ("ldr", "x10, [x0, #0x20]") or insns[39] != insns[41] or insns[39][0] != "bl" \
            or insns[46] != ("bl", "#0x71000a16e0") or insns[50] != ("bl", "#0x710009d7a8"):
        return None
    get = int(insns[39][1][1:], 16)
    return "\n".join([f"const base::reflection::CType* fn_{get:x}();",
                      f"template <> struct base::reflection::UnkFieldTraits<{tag}> {{",
                      f"    static const CType* type() {{ return fn_{get:x}(); }}", "};",
                      f"template bool base::refl_GetKeyedValue<{tag}>(char*, const base::UnkStrIdMember*, void*);"])


# UnkCloneCalls.h: small functions around one or two calls; the callees come from UnkCallTraits.
OFF = r"#(0x[0-9a-f]+|\d+)"


def target(op: str) -> int:
    return int(op[1:], 16)


def call_traits(tag: str, decls: list[str], members: list[str]) -> list[str]:
    return decls + [f"template <> struct base::UnkCallTraits<{tag}> {{", *members, "};"]


def call_with_owner_child(orig, addr, tag, insns):
    """refl_CallWithOwnerChild: the owner's offset, get() and call()."""
    if len(insns) != 16 or insns[4] != ("ldr", "x19, [x8, #0x220]") or insns[6][0] != "bl" or insns[11][0] != "b" \
            or insns[13] != ("mov", "w0, wzr"):
        return None
    off = re.fullmatch(rf"x8, \[x0, {OFF}\]", insns[3][1])
    if insns[3][0] != "ldr" or not off:
        return None
    get, call = target(insns[6][1]), target(insns[11][1])
    return "\n".join(call_traits(tag, [f"char* fn_{get:x}(char*);", f"bool fn_{call:x}(char*, char*);"], [
        f"    static char* get(char* s) {{ return fn_{get:x}(s); }}",
        f"    static bool call(char* p, char* x) {{ return fn_{call:x}(p, x); }}"]) +
        [f"template bool base::refl_CallWithOwnerChild<{tag}, {int(off.group(1), 0):#x}>(char*);"])


def ctor_then_init(orig, addr, tag, insns):
    """refl_CtorThenInit: the base constructor, the vtable, the member's offset and constructor."""
    if len(insns) != 17 or insns[3] != ("mov", "x19, x0") or insns[4][0] != "bl" or insns[10][0] != "bl" \
            or insns[13] != ("ldr", "x1, [x8, #0x18]") or insns[16] != ("br", "x1") or insns[9] != ("str", "x8, [x19]"):
        return None
    vt = got_target(orig, insns, 5)
    off = re.fullmatch(rf"x0, x19, {OFF}", insns[8][1])
    if vt is None or insns[8][0] != "add" or not off:
        return None
    base, member = target(insns[4][1]), target(insns[10][1])
    decls = [f"extern char g_unk_{vt:x}[];"] + [f"void fn_{x:x}(char*);" for x in sorted({base, member})]
    return "\n".join(call_traits(tag, decls, [
        f"    static void base(char* s) {{ fn_{base:x}(s); }}", f"    static void member(char* s) {{ fn_{member:x}(s); }}",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}"]) +
        [f"template void base::refl_CtorThenInit<{tag}, {int(off.group(1), 0):#x}>(char*);"])


def cast_then_call(orig, addr, tag, insns):
    """refl_CastThenCall(Arg): the inner offset, the class getter and the call."""
    if len(insns) != 23 or insns[11][0] != "bl" or insns[14] != ("bl", f"#{IS_KIND_OF:#x}") or insns[19][0] != "b" \
            or insns[15] != ("tbz", "w0, #0, @+50"):
        return None
    first = {"x8, [x0, #8]": ("refl_CastThenCall", "(char*)"), "x8, [x1]": ("refl_CastThenCallArg", "(void*, char*)")}
    form = first.get(insns[3][1]) if insns[3][0] == "ldr" else None
    off = re.fullmatch(rf"x19, \[x8, {OFF}\]", insns[4][1])
    if not form or insns[4][0] != "ldr" or not off:
        return None
    cls, call = target(insns[11][1]), target(insns[19][1])
    return "\n".join(call_traits(tag, [f"const base::reflection::CType* fn_{cls:x}();", f"void fn_{call:x}(char*);"], [
        f"    static const base::reflection::CType* cls() {{ return fn_{cls:x}(); }}",
        f"    static void call(char* o) {{ fn_{call:x}(o); }}"]) +
        [f"template void base::{form[0]}<{tag}, {int(off.group(1), 0):#x}>{form[1]};"])


def find_by_id_call(orig, addr, tag, insns):
    """refl_FindByIdCall: the key's argument register and the call."""
    if len(insns) != 14 or insns[4] != ("ldr", "x0, [x8], #8") or insns[12][0] != "b" or insns[13] != ("ret", ""):
        return None
    key = re.fullmatch(r"w10, w([1-7])", insns[6][1])
    if insns[6][0] != "cmp" or not key:
        return None
    k, call = int(key.group(1)), target(insns[12][1])
    params = ["u64"] * (k - 1) + ["u32"]
    sig = ", ".join(params)
    named = ", ".join(f"{t} a{i}" for i, t in enumerate(params))
    args = ", ".join(f"a{i}" for i in range(len(params)))
    return "\n".join(call_traits(tag, [f"void fn_{call:x}(u32*, {sig});"], [
        f"    static void call(u32* p, {named}) {{ fn_{call:x}(p, {args}); }}"]) +
        [f"template void base::refl_FindByIdCall<{tag}, {k - 1}>(base::UnkVector<u32*>*, {sig});"])


def call_arg_return_one(orig, addr, tag, insns):
    """refl_CallArgReturnOne: the call and the result's type getter."""
    if len(insns) != 32 or insns[17][0] != "bl" or insns[18] != ("mov", "w8, #1") or insns[20][0] != "bl" \
            or insns[28] != ("bl", f"#{MAKE_VALUE:#x}") or insns[3] != ("ldr", "x0, [x1, #8]"):
        return None
    call, ty = target(insns[17][1]), target(insns[20][1])
    return "\n".join(call_traits(tag, [f"void fn_{call:x}(char*, const void*);",
                                        f"const base::reflection::CType* fn_{ty:x}();"], [
        f"    static void call(char* t, const void* v) {{ fn_{call:x}(t, v); }}",
        f"    static const base::reflection::CType* type() {{ return fn_{ty:x}(); }}"]) +
        [f"template base::reflection::UnkReflValue base::refl_CallArgReturnOne<{tag}>(void*, char*, "
         f"base::reflection::UnkReflValue* const*);"])


def elem_at(orig, addr, tag, insns):
    """refl_ElemAt: the element size (the shift)."""
    if len(insns) != 4 or insns[:2] != [("ldr", "x8, [x0]"), ("and", "x9, x1, #0xffffffff")] or insns[3] != ("ret", ""):
        return None
    shift = re.fullmatch(r"x0, x8, x9, lsl #(\d)", insns[2][1])
    if insns[2][0] != "add" or not shift:
        return None
    size = 1 << int(shift.group(1))
    return (f"template UnkCloneElem<{tag}, {size}>* base::refl_ElemAt(base::UnkVector<UnkCloneElem<{tag}, {size}>>*, "
            "u64);")


def copy_ctor3(orig, addr, tag, insns):
    """refl_CopyCtor3: the three vtables."""
    if len(insns) != 21 or insns[4] != ("ldp", "x10, x11, [x1, #0x18]") or insns[11] != ("stp", "x9, x8, [x0]") \
            or insns[18] != ("ldr", "x8, [x1, #0x30]"):
        return None
    vts = got_target(orig, insns, 0), got_target(orig, insns, 7), got_target(orig, insns, 14)
    if None in vts:
        return None
    decls = [f"extern char g_unk_{v:x}[];" for v in sorted(set(vts))]
    return "\n".join(call_traits(tag, decls, [
        f"    static char* {n}() {{ return g_unk_{v:x}; }}" for n, v in zip(("base_vtable", "mid_vtable", "vtable"), vts)]) +
        [f"template void base::refl_CopyCtor3<{tag}>(char*, const char*);"])


def get_lookup_f32(orig, addr, tag, insns):
    """refl_GetLookupF32: the lookup, the offset, the default and the f32 type getter."""
    if len(insns) != 23 or insns[3] != ("ldr", "x0, [x1, #8]") or insns[5][0] != "bl" or insns[11][0] != "bl" \
            or insns[19] != ("bl", f"#{MAKE_VALUE:#x}"):
        return None
    off = re.fullmatch(rf"s0, \[x0, {OFF}\]", insns[7][1])
    dflt = {"s0, wzr": "0.0f", "s0, #-1.00000000": "-1.0f"}.get(insns[9][1])
    if insns[7][0] != "ldr" or not off or insns[9][0] != "fmov" or not dflt:
        return None
    get, ty = target(insns[5][1]), target(insns[11][1])
    return "\n".join(call_traits(tag, [f"char* fn_{get:x}(char*);", f"const base::reflection::CType* fn_{ty:x}();"], [
        f"    static char* get(char* o) {{ return fn_{get:x}(o); }}", f"    static f32 dflt() {{ return {dflt}; }}",
        f"    static const base::reflection::CType* type() {{ return fn_{ty:x}(); }}"]) +
        [f"template base::reflection::UnkReflValue base::refl_GetLookupF32<{tag}, {int(off.group(1), 0):#x}>(void*, char*);"])


def copy_ctor2(orig, addr, tag, insns):
    """refl_CopyCtor2: both vtables."""
    if len(insns) != 15 or insns[4] != ("ldp", "x10, x11, [x1, #0x18]") or insns[11] != ("stp", "x9, x8, [x0]") \
            or insns[12] != ("ldr", "x8, [x1, #0x28]"):
        return None
    vts = got_target(orig, insns, 0), got_target(orig, insns, 7)
    if None in vts:
        return None
    decls = [f"extern char g_unk_{v:x}[];" for v in sorted(set(vts))]
    return "\n".join(call_traits(tag, decls, [
        f"    static char* {n}() {{ return g_unk_{v:x}; }}" for n, v in zip(("base_vtable", "vtable"), vts)]) +
        [f"template void base::refl_CopyCtor2<{tag}>(char*, const char*);"])


DEEP_RE = re.compile(rf"x9, \[x9, {OFF}\]")


def set_deep_u32(orig, addr, tag, insns):
    """refl_SetDeepU32: the middle and field offsets."""
    if len(insns) != 20 or insns[10] != ("ldr", "x9, [x9, #0x58]") or insns[13] != ("ldr", "x9, [x9, #0x78]") \
            or insns[0] != ("ldr", "x10, [x2]"):
        return None
    mid = DEEP_RE.fullmatch(insns[11][1])
    off = re.fullmatch(rf"w10, \[x9, {OFF}\]", insns[14][1])
    if insns[14][0] != "str" or not mid or not off:
        return None
    return (f"template base::reflection::UnkReflValue base::refl_SetDeepU32<{tag}, {int(mid.group(1), 0):#x}, "
            f"{int(off.group(1), 0):#x}>(void*, char*, base::reflection::UnkReflValue* const*);")


def get_deep_u32(orig, addr, tag, insns):
    """refl_GetDeepU32: the middle and field offsets and the value's type getter."""
    if len(insns) != 22 or insns[5] != ("ldr", "x8, [x8, #0x58]") or insns[7] != ("ldr", "x8, [x8, #0x78]") \
            or insns[10][0] != "bl" or insns[18] != ("bl", f"#{MAKE_VALUE:#x}"):
        return None
    mid = re.fullmatch(rf"x8, \[x8, {OFF}\]", insns[6][1])
    off = re.fullmatch(rf"w8, \[x8, {OFF}\]", insns[8][1])
    if not mid or not off or insns[8][0] != "ldr":
        return None
    ty = target(insns[10][1])
    return "\n".join(call_traits(tag, [f"const base::reflection::CType* fn_{ty:x}();"], [
        f"    static const base::reflection::CType* type() {{ return fn_{ty:x}(); }}"]) +
        [f"template base::reflection::UnkReflValue base::refl_GetDeepU32<{tag}, {int(mid.group(1), 0):#x}, "
         f"{int(off.group(1), 0):#x}>(void*, char*);"])


def set_bool(orig, addr, tag, insns):
    """refl_SetBool: the flag's offset."""
    if len(insns) != 6 or insns[1:4] != [("and", "w8, w1, #1"), ("cmp", "w9, w8"), ("b.eq", "@+14")] \
            or insns[5] != ("ret", ""):
        return None
    off = re.fullmatch(rf"w9, \[x0, {OFF}\]", insns[0][1])
    if insns[0][0] != "ldrb" or not off or insns[4] != ("strb", f"w8, [x0, #{off.group(1)}]"):
        return None
    return f"template void base::refl_SetBool<{tag}, {int(off.group(1), 0):#x}>(char*, bool);"


def set_flag_call(orig, addr, tag, insns):
    """refl_SetFlagCall: the flag, member and next offsets and the call."""
    if len(insns) != 21 or insns[6] != ("eor", "w8, w8, w1") or insns[12][0] != "bl" \
            or insns[17] != ("b", "#0x7100025a88"):
        return None
    flag = re.fullmatch(rf"w8, \[x0, {OFF}\]", insns[3][1])
    member = re.fullmatch(rf"x0, x0, {OFF}", insns[10][1])
    nxt = re.fullmatch(rf"x0, x19, {OFF}", insns[15][1])
    if insns[3][0] != "ldrb" or not flag or not member or not nxt:
        return None
    call = target(insns[12][1])
    offs = ", ".join(f"{int(h.group(1), 0):#x}" for h in (flag, member, nxt))
    return "\n".join(call_traits(tag, [f"void fn_{call:x}(char*, bool);"], [
        f"    static void call(char* p, bool v) {{ fn_{call:x}(p, v); }}"]) +
        [f"template void base::refl_SetFlagCall<{tag}, {offs}>(char*, bool);"])


def keyed_set(orig, addr, tag, insns):
    """refl_KeyedSet: the key/value width."""
    if len(insns) != 28 or insns[3] != ("mov", "x9, xzr") or insns[20][0] != "str":
        return None
    t = {"x8, [x9]": "u64", "w8, [x9]": "u32"}.get(insns[20][1])
    if not t:
        return None
    return f"template void base::refl_KeyedSet(base::UnkKeyedArray<{tag}, {t}, {t}>*, const {t}*, const {t}*);"


def value_map_clear(orig, addr, tag, insns):
    """refl_ValueMapClear: the key's pool, the link's offset and the value's destructor."""
    if len(insns) != 59 or insns[4] != ("ldr", "w8, [x0, #0xc]") or insns[22] != ("add", "x0, x20, #8") \
            or insns[23][0] != "bl" or ("bl", f"#{MEMSET:#x}") not in insns:
        return None
    link = re.fullmatch(rf"x22, \[x20, {OFF}\]", insns[21][1])
    pool = got_target(orig, insns, 9)
    if insns[21][0] != "ldr" or not link or pool not in (STRID_POOL, FILE_PATH_POOL):
        return None
    key = f"UnkCloneStrIdB<{tag}>" if pool == FILE_PATH_POOL else f"UnkCloneStrId<{tag}>"
    dtor = target(insns[23][1])
    return "\n".join([f"void fn_{dtor:x}(char*);", f"template <> struct base::UnkHashMapTraits<{tag}> {{",
                      f"    static void destroy(char* p) {{ fn_{dtor:x}(p); }}", "};",
                      f"template void base::refl_ValueMapClear<{tag}, {key}, {int(link.group(1), 0):#x}>(char*);"])


def keyed_array_copy(orig, addr, tag, insns):
    """refl_KeyedArrayCopy: the value size, reserve() and the value's copy constructor."""
    if len(insns) != 59 or insns[4] != ("stp", "xzr, xzr, [x0, #8]") or insns[15][0] != "bl" \
            or insns[25][0] != "bl" or got_target(orig, insns, 34) != STRID_POOL:
        return None
    size = re.fullmatch(rf"x21, x21, {OFF}", insns[28][1])
    if insns[28][0] != "add" or not size or int(size.group(1), 0) % 8:
        return None
    reserve, copy = target(insns[15][1]), target(insns[25][1])
    arr = f"base::UnkKeyedCopyArray<{tag}, {int(size.group(1), 0):#x}>"
    return "\n".join(call_traits(tag, [f"void fn_{reserve:x}(void*, u32);", f"void fn_{copy:x}(void*, const void*);"], [
        f"    static void reserve(void* s, u32 n) {{ fn_{reserve:x}(s, n); }}",
        f"    static void copy(void* d, const void* s) {{ fn_{copy:x}(d, s); }}"]) +
        [f"template void base::refl_KeyedArrayCopy({arr}*, const {arr}*);"])


def callback_list_dtor(orig, addr, tag, insns):
    """UnkCallbackList's destructor or deleting destructor: both vtables."""
    if len(insns) != 56 or insns[9] != ("ldr", "x0, [x0, #0x38]") or insns[31] != ("ldr", "x8, [x9, #0x40]"):
        return None
    vt, base = got_target(orig, insns, 4), got_target(orig, insns, 18)
    if vt is None or base is None:
        return None
    return "\n".join([f"extern char g_unk_{v:x}[];" for v in sorted({vt, base})] + [
        f"template <> struct base::UnkCallbackTraits<{tag}> {{", f"    static char* vtable() {{ return g_unk_{vt:x}; }}",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}", "};",
        f"template void base::refl_CallbackListDeletingDtor(base::UnkCallbackList<{tag}>*);"
        if insns[-1] == ("b", f"#{OPERATOR_DELETE:#x}") else f"template base::UnkCallbackList<{tag}>::~UnkCallbackList();"])


def iter_value_destroy_poly_vec_strid(tag, insns):
    """refl_IterValueDestroy over an array of 0x40-byte polymorphic elements keyed by a string id
    (the shape of fn_710023c3e0): the state, array and key offsets give the key's and the
    array's sizes."""
    state = re.fullmatch(rf"w8, \[x0, {OFF}\]", insns[4][1])
    vec = re.fullmatch(rf"x1, \[x19, {OFF}\]", insns[10][1])
    key = re.fullmatch(rf"x8, \[x19(?:, {OFF})?\]", insns[44][1])
    if not state or not vec or not key:
        return None
    s, v, k = int(state.group(1), 0), int(vec.group(1), 0), int(key.group(1) or "0", 0)
    if v - k != 8 or s - v < 0x28:
        return None
    elem = f"UnkClonePolyVec<{tag}>" if s - v == 0x28 else f"UnkClonePolyVecPad<{tag}, {s - v:#x}>"
    key_t = f"UnkCloneStrId<{tag}>" if k == 0 else f"UnkCloneStrIdElem<{tag}, {k:#x}, {v:#x}, base::UnkStrIdMember>"
    return f"template void base::refl_IterValueDestroy<{elem}, {key_t}>(void*);"


def strid_vec_dtor(orig, addr, tag, insns):
    """refl_StrIdVecDtor / refl_StrIdVecDeletingDtor: the vtable, the array's offset and the base
    destructor."""
    if len(insns) not in (54, 56) or ("ccmp", "x8, x9, #4, ne") not in insns or ("bl", f"#{RELEASE:#x}") not in insns:
        return None
    vt = got_target(orig, insns, 4)
    off = re.fullmatch(rf"x1, \[x0, {OFF}\]", insns[6][1])
    base = [o for m, o in insns[-6:] if m in ("bl", "b") and o != f"#{OPERATOR_DELETE:#x}"]
    if vt is None or insns[6][0] != "ldr" or not off or len(base) != 1:
        return None
    fn = "refl_StrIdVecDeletingDtor" if insns[-1] == ("b", f"#{OPERATOR_DELETE:#x}") else "refl_StrIdVecDtor"
    b = target(base[0])
    return "\n".join(call_traits(tag, [f"extern char g_unk_{vt:x}[];", f"void fn_{b:x}(char*);"], [
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}", f"    static void base(char* s) {{ fn_{b:x}(s); }}"]) +
        [f"template void base::{fn}<{tag}, {int(off.group(1), 0):#x}>(char*);"])


def path_record_copy_ctor(orig, addr, tag, insns):
    """refl_PathRecordCopyCtor (UnkCloneRecords.h): the middle and the class's vtables (through the
    GOT) around the string pools."""
    if len(insns) != 67 or insns[5] != ("bl", "#0x7100067ebc") or insns[46] != ("ldp", "x9, x10, [x20, #0x40]") \
            or insns[63] != ("str", "x8, [x19]"):
        return None
    mid, vt = got_target(orig, insns, 6), got_at(orig, insns, 58, 60)
    if mid is None or vt is None or got_target(orig, insns, 12) != STRID_POOL \
            or got_target(orig, insns, 25) != FILE_PATH_POOL:
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({mid, vt})] + [
        f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
        f"    static char* mid_vtable() {{ return g_unk_{mid:x}; }}",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
        f"template void base::refl_PathRecordCopyCtor<{tag}>(char*, const char*);"])


def callback_entry_dtor(orig, addr, tag, insns):
    """UnkCallbackEntry / UnkKeyedCallbackEntry destructors (UnkCloneRecords.h): both vtables; the
    list's offset picks the record."""
    if len(insns) != 75 or insns[20] != ("bl", f"#{RELEASE:#x}") or got_target(orig, insns, 4) != STRID_POOL:
        return None
    rec = {("str", "x8, [x19, #8]"): "UnkKeyedCallbackEntry", ("str", "x8, [x19]"): "UnkCallbackEntry"}.get(insns[28])
    vt, base = got_target(orig, insns, 24), got_target(orig, insns, 37)
    if not rec or vt is None or base is None or insns[41] != insns[28]:
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({vt, base})] + [
        f"template <> struct base::UnkCallbackTraits<{tag}> {{",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}", "};",
        f"template base::{rec}<{tag}>::~{rec}();"])


def strid_vec_record_copy_ctor(orig, addr, tag, insns):
    """refl_StrIdVecRecordCopyCtor (UnkCloneRecords.h): the three vtables (through the GOT)."""
    if len(insns) != 87 or insns[32] != ("bl", "#0x710000542c") or insns[60] != insns[32] \
            or insns[54] != ("udiv", "w10, w8, w9") or got_at(orig, insns, 35, 39) != STRID_POOL:
        return None
    base, mid, vt = (got_target(orig, insns, i) for i in (5, 13, 76))
    if None in (base, mid, vt):
        return None
    return "\n".join([f"extern char g_unk_{x:x}[];" for x in sorted({base, mid, vt})] + [
        f"template <> struct base::UnkCopyCtorTraits<{tag}> {{",
        f"    static char* base_vtable() {{ return g_unk_{base:x}; }}",
        f"    static char* mid_vtable() {{ return g_unk_{mid:x}; }}",
        f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
        f"template void base::refl_StrIdVecRecordCopyCtor<{tag}>(char*, const char*);"])


def add_function_holder(orig, addr, tag, insns):
    """refl_AddFunctionHolder (UnkCloneRecords.h): the holder's vtable (through the GOT, read on
    both paths)."""
    if len(insns) != 80 or insns[7] != ("mov", "w0, #0x40") or insns[62] != ("bl", "#0x71011cf394") \
            or insns[33] != ("ldr", "x8, [x8, #0x18]"):
        return None
    vt = got_target(orig, insns, 23)
    if vt is None or got_target(orig, insns, 37) != vt:
        return None
    return "\n".join([f"extern char g_unk_{vt:x}[];", f"template <> struct base::UnkFunctionHolderTraits<{tag}> {{",
                      f"    static char* vtable() {{ return g_unk_{vt:x}; }}", "};",
                      f"template void base::refl_AddFunctionHolder<{tag}>(void*, void*, const std::function<void()>&, "
                      "void*, u32);"])


def keyed_move_assign(orig, addr, tag, insns):
    """UnkKeyedMoveArray::operator= (UnkCloneRecords.h): the value size and its destructor."""
    if len(insns) != 81 or insns[37] != ("bl", f"#{RELEASE:#x}") or insns[44][0] != "bl" or insns[52] != insns[44] \
            or got_target(orig, insns, 11) != STRID_POOL or insns[74] != ("stp", "xzr, xzr, [x19]"):
        return None
    size = re.fullmatch(r"w22, #(0x[0-9a-f]+)", insns[49][1])
    if insns[49][0] != "mov" or not size or insns[55] != ("add", f"x22, x22, #{size.group(1)}"):
        return None
    a = f"base::UnkKeyedMoveArray<{tag}, {int(size.group(1), 16):#x}>"
    return dtor_traits(tag, insns[44]) + f"template {a}& {a}::operator=({a}&&);"


def keyed_move_array_clear(orig, addr, tag, insns):
    """UnkKeyedMoveArray::Clear (UnkCloneRecords.h): the value size and its destructor."""
    if len(insns) != 61 or insns[35] != ("bl", f"#{RELEASE:#x}") or insns[42][0] != "bl" or insns[50] != insns[42] \
            or got_target(orig, insns, 9) != STRID_POOL or insns[56] != ("str", "wzr, [x19, #0x10]"):
        return None
    size = re.fullmatch(r"w21, #(0x[0-9a-f]+)", insns[47][1])
    if insns[47][0] != "mov" or not size or insns[53] != ("add", f"x21, x21, #{size.group(1)}"):
        return None
    return dtor_traits(tag, insns[42]) + f"template void base::UnkKeyedMoveArray<{tag}, {int(size.group(1), 16):#x}>::Clear();"


def strid_vec_variant_reset(orig, addr, tag, insns):
    """UnkStrIdVecVariant / UnkKeyedStrIdVecVariant Reset (UnkCloneRecords.h): the state offset
    picks the record."""
    if len(insns) != 40 or insns[32] != ("bl", f"#{RELEASE:#x}") or insns[14] != ("ldr", "x8, [x8, #0x48]") \
            or got_target(orig, insns, 17) != STRID_POOL:
        return None
    rec = {("ldr", "w8, [x0, #0x30]"): "UnkStrIdVecVariant", ("ldr", "w8, [x0, #0x38]"): "UnkKeyedStrIdVecVariant"}
    r = rec.get(insns[3])
    return f"template void base::{r}<{tag}>::Reset();" if r else None


KINDS = {
    "strid_value_insert": Kind(None, "base/UnkStrIdValueVec.h", lambda tag, _: None, scan=strid_value_insert),
    "set_flag_notify": Kind(0x71001F31E8, "base/UnkCloneCalls.h", lambda tag, _: (
        f"template void base::refl_SetFlagNotify<{tag}>(char*, bool);")),
    "set_flag_virtual": Kind(0x71001F3344, "base/UnkCloneCalls.h", lambda tag, _: (
        f"template void base::refl_SetFlagVirtual<{tag}>(char*, bool);")),
    "call_with_owner_child": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=call_with_owner_child),
    "ctor_then_init": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=ctor_then_init),
    "cast_then_call": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=cast_then_call),
    "find_by_id_call": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=find_by_id_call),
    "call_arg_return_one": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=call_arg_return_one),
    "elem_at": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=elem_at),
    "copy_ctor3": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=copy_ctor3),
    "link_slot_assign": Kind(0x710023655C, "base/UnkCloneCalls.h", lambda tag, _: (
        f"template base::UnkLinkSlot<{tag}>& base::UnkLinkSlot<{tag}>::operator=(const base::UnkLinkSlot<{tag}>&);")),
    "get_lookup_f32": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=get_lookup_f32),
    "set_flag_notify_b": Kind(0x71001F32B0, "base/UnkCloneCalls.h", lambda tag, _: (
        f"template void base::refl_SetFlagNotifyB<{tag}>(char*, bool);")),
    "release_ref": Kind(0x7100234EF0, "base/UnkCloneCalls.h", lambda tag, _: (
        f"template void base::refl_ReleaseRef<{tag}>(base::UnkRefOwner<{tag}>*);")),
    "get_false": Kind(0x71002546D0, "base/UnkCloneCalls.h", lambda tag, _: "\n".join(call_traits(
        tag, ["const base::reflection::CType* fn_710000d228();"],
        ["    static const base::reflection::CType* type() { return fn_710000d228(); }"]) +
        [f"template base::reflection::UnkReflValue base::refl_GetFalse<{tag}>(void*);"])),
    "copy_ctor2": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=copy_ctor2),
    "set_deep_u32": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=set_deep_u32),
    "get_deep_u32": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=get_deep_u32),
    "set_bool": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=set_bool),
    "set_flag_call": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=set_flag_call),
    "keyed_set": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=keyed_set),
    "value_map_clear": Kind(None, "base/UnkHashMap.h", lambda tag, _: None, scan=value_map_clear),
    "keyed_array_copy": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=keyed_array_copy),
    "slist_insert_before": Kind(0x7100240A00, "base/UnkSList.h", lambda tag, _: (
        f"template void base::UnkSList<{tag}>::InsertBefore(const u64&, base::UnkSList<{tag}>::Node*);")),
    "callback_list_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=callback_list_dtor),
    "strid_vec_dtor": Kind(None, "base/UnkCloneCalls.h", lambda tag, _: None, scan=strid_vec_dtor),
    "strid_pair_record_assign": Kind(0x7100BC59FC, "base/UnkCloneRecords.h", lambda tag, _: (
        f"template base::UnkStrIdPairRecord<{tag}>& base::UnkStrIdPairRecord<{tag}>::operator=("
        f"const base::UnkStrIdPairRecord<{tag}>&);")),
    "keyed_vec_array_clear": Kind(0x710027E494, "base/UnkCloneRecords.h", lambda tag, _: (
        f"template void base::UnkKeyedVecArray<{tag}>::Clear();")),
    "path_record_copy_ctor": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None, scan=path_record_copy_ctor),
    "callback_entry_dtor": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None, scan=callback_entry_dtor),
    "strid_vec_record_copy_ctor": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None,
                                       scan=strid_vec_record_copy_ctor),
    "add_function_holder": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None, scan=add_function_holder),
    "keyed_move_assign": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None, scan=keyed_move_assign),
    "keyed_move_array_clear": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None,
                                   scan=keyed_move_array_clear),
    "strid_vec_variant_reset": Kind(None, "base/UnkCloneRecords.h", lambda tag, _: None,
                                    scan=strid_vec_variant_reset),
    "variant_assign": Kind(0x71000C3CC0, "base/UnkCloneHelpers.h", lambda tag, ins: variant(tag, ins, "refl_VariantAssign", "{v}*"),
                           exact=False),
    "variant_assign_pair": Kind(0x71000C3C14, "base/UnkCloneHelpers.h", lambda tag, ins: variant(
        tag, ins, "refl_VariantAssignPair", "{v}::Pair*"), exact=False),
    "keyed_value_array_dtor": Kind(0x710009ABF8, "base/UnkCloneHelpers.h", lambda tag, ins: keyed_value_array_dtor(tag, ins),
                                   exact=False),
    "iter_assign_block": Kind(0x71002CA39C, "base/UnkCollectionIter.h", lambda tag, ins: iter_assign_block(tag, ins),
                              exact=False),
    "vector_clear_poly": Kind(None, "base/UnkVector.h", lambda tag, _: None, scan=vector_clear_poly),
    "value_holder_deleting_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None,
                                       scan=value_holder_deleting_dtor),
    "derived_copy_ctor_48": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=derived_copy_ctor_48),
    "iter_value_destroy_poly_vec_key16": Kind(0x710023C3E0, "base/UnkCloneHelpers.h",
                                              iter_value_destroy_poly_vec_strid, exact=False),
    "keyed_pair_iter_assign": Kind(0x7100239CF4, "base/UnkCloneHelpers.h", lambda tag, ins: keyed_pair_iter_assign(tag, ins),
                                   exact=False),
    "value_map_insert": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=value_map_insert),
    "shared_copy_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=shared_copy_ctor),
    "set_matrix": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=set_matrix),
    "index_pair_iter_assign": Kind(0x71008932BC, "base/UnkCloneHelpers.h", lambda tag, ins: index_pair_iter_assign(tag, ins),
                                   exact=False),
    "iter_value_copy_assign_index": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None,
                                         scan=iter_value_copy_assign_index),
    "call_with_owner_id": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=call_with_owner_id),
    "iter_value_from_ref": Kind(0x71000C3B5C, "base/UnkCollectionIter.h", lambda tag, ins: iter_value_from_ref(tag, ins),
                                exact=False),
    "ref_handle_copy_ctor": Kind(0x7100234E38, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_RefHandleCopyCtor<{tag}>(base::UnkRefHandle*, const base::UnkRefHandle*);")),
    "iter_value_destroy_key16": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_key16),
    "vector_clear_dtor": Kind(None, "base/UnkVector.h", lambda tag, _: None, scan=vector_clear_dtor),
    "iter_value_destroy_refptr": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None,
                                      scan=iter_value_destroy_refptr),
    "add_f32": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=add_f32),
    "pair_counts": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=pair_counts),
    "value_holder_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=value_holder_dtor),
    "zeroed_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=zeroed_ctor),
    "dtor_call_clear": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=dtor_call_clear),
    "keyed_find_assign": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_find_assign),
    "vector_free_dtor": Kind(None, "base/UnkVector.h", lambda tag, _: None, scan=vector_free_dtor),
    "keyed_find": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_find),
    "keyed_find_then": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_find_then),
    "strid_dtor_at": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=strid_dtor_at),
    "iter_value_move_ool": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_move_ool),
    "iter_value_move_ool_strid": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None,
                                      scan=iter_value_move_ool_strid),
    "strid_copy_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=strid_copy_ctor),
    "keyed_value_get": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_value_get),
    "keyed_set_assign": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_set_assign),
    "keyed_set_iter_value_copy": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=keyed_set_iter_value_copy),
    "vt_iter_end": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=vt_iter_end),
    "vt_iter_begin": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=vt_iter_begin),
    "vt_iter_sync": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=vt_iter_sync),
    "owner_vec_insert": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=owner_vec_insert),
    "hash_key_fields": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=hash_key_fields),
    "push_pair": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=push_pair),
    "deferred_invoke": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=deferred_invoke),
    "deferred_invoke_ptr": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=deferred_invoke_ptr),
    "callback_list_destroy": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=callback_list_destroy),
    "iter_value_move_poly_strid": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=iter_value_move_poly_strid),
    "chain_map_erase": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=chain_map_erase),
    "vector_free_poly": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=vector_free_poly),
    "callback_base_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=callback_base_dtor),
    "pool_list_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=pool_list_ctor),
    "pool_list_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=pool_list_dtor),
    "type_static_init": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=type_static_init),
    "map_iter_sync_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=map_iter_sync_dtor),
    "id_value_push_back": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=id_value_push_back),
    "id_value_assign": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=id_value_assign),
    "owner_vec_destroy": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=owner_vec_destroy),
    "call_and_cast_field_register": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=call_and_cast_field_register),
    "iter_value_move_assign_index": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=iter_value_move_assign_index),
    "owner_dtor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=owner_dtor),
    "push_then_next": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=push_then_next),
    "hash_map_init": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=hash_map_init),
    "push_two_types": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=push_two_types),
    "derived_copy_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=derived_copy_ctor),
    "strid_value_resize": Kind(None, "base/UnkStrIdValueVec.h", lambda tag, _: None, scan=strid_value_resize),
    "strid_value_grow_at": Kind(None, "base/UnkStrIdValueVec.h", lambda tag, _: None, scan=strid_value_grow_at),
    "strid_value_vec_copy": Kind(None, "base/UnkStrIdValueVec.h", lambda tag, _: None, scan=strid_value_vec_copy),
    "ref_pair_array_deleting_dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=ref_pair_array_deleting_dtor),
    "list_owner_deleting_dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=list_owner_deleting_dtor),
    "iter_sync_strid_elem": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_sync_strid_elem),
    "chained_set_clear": Kind(None, "base/UnkHashMap.h", lambda tag, _: None, scan=chained_set_clear),
    "cache_add": Kind(None, "base/UnkCache.h", lambda tag, _: None, scan=cache_add),
    "iter_sync_callbacks_scan": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=iter_sync_callbacks),
    "dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=destructor),
    "iter_value_destroy_vt": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_vt),
    "iter_element_dyn_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=iter_element_dyn_value),
    "iter_element_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=iter_element_value),
    "pointee_typed": Kind(None, "base/reflection/UnkReflTypedPtr.h", lambda tag, _: None, scan=pointee_typed),
    "linked_dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=linked_dtor),
    "list_deleting_dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=list_deleting_dtor),
    "list_dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=list_dtor),
    "vector_move_poly": Kind(None, "base/UnkVector.h", lambda tag, _: None, scan=vector_move_poly),
    "id_record_assign": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=id_record_assign),
    "strid_member_ctor": Kind(None, "base/UnkCloneHelpers.h", lambda tag, _: None, scan=strid_member_ctor),
    "hash_map_clear": Kind(None, "base/UnkHashMap.h", lambda tag, _: None, scan=hash_map_clear),
    "iter_assign_dtor": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_assign_dtor),
    "iter_value_copy_assign_elem": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_copy_assign_elem),
    "iter_value_destroy_strid2": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_strid2),
    "iter_value_destroy_strid_elem": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_strid_elem),
    "iter_value_destroy_dtor": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_dtor),
    "enum_values": Kind(None, "base/global/CStrId.h", lambda tag, _: None, scan=enum_values),
    "call_and_cast_arg": Kind(None, "base/reflection/UnkReflCast.h", lambda tag, _: None, scan=call_and_cast_arg),
    "call_and_cast_field": Kind(None, "base/reflection/UnkReflCast.h", lambda tag, _: None, scan=call_and_cast_field),
    "field_set_min_notify": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_set_min_notify),
    "field_set_apply": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_set_apply),
    "field_set_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_set_value),
    "field_set_strid": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_set_strid),
    "field_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=field_value),
    "register_named": Kind(0x7100072940, "base/UnkNamedRegistry.h", lambda tag, _: (
        f"template base::UnkNamedObject* base::refl_RegisterNamed<{tag}>("
        "base::UnkNamedRegistry*, base::UnkNamedObject*, const char*);")),
    "id_pair_assign": Kind(0x7100B96DEC, "base/UnkIdPair.h", lambda tag, _: (
        f"template base::UnkIdPair<{tag}>& base::UnkIdPair<{tag}>::operator=(const base::UnkIdPair<{tag}>&);")),
    "deferred_add": Kind(0x71002AEEBC, "base/UnkDeferredSet.h", lambda tag, _: (
        f"template void base::UnkDeferredSet<{tag}>::Add({tag}*);")),
    "set_dynamic_value": Kind(0x71002FA764, "base/reflection/UnkReflCast.h", lambda tag, _: (
        f"template void base::reflection::refl_SetDynamicValue<{tag}>(char*, base::reflection::UnkReflObject*);")),
    "strid_list_ctor": Kind(0x71004F112C, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_StrIdListCtor<{tag}>(char*);")),
    "iter_sync_vt": Kind(0x7100F4AF08, "base/UnkCloneHelpers.h", lambda tag, _: (
        "extern char g_unk_7101cab1f8[];\n"
        f"template <> struct base::UnkDtorTraits<{tag}> {{\n"
        "    static char* vtable() { return g_unk_7101cab1f8; }\n};\n"
        f"template void base::refl_IterSync(base::UnkCollectionIter<base::UnkDtorElem<{tag}, 0x18, "
        "base::UnkStrIdAt<8>>, u32>*);")),
    "slot_pool_pair_dtor": Kind(0x71002880D4, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_SlotPoolPairDtor<{tag}>(char*);")),
    "vector_equal_word": Kind(0x71000CDDD8, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template bool base::refl_VectorEqual<{tag}>(const base::UnkVector<UnkCloneWord<{tag}>>*, "
        f"const base::UnkVector<UnkCloneWord<{tag}>>*);")),
    "keyed_array_reserve": Kind(0x710021D58C, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::UnkCloneKeyedArray<{tag}>::Reserve(u32);")),
    "iter_value_move_assign_strkey": Kind(0x7100120984, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkIterValue<UnkCloneWord<{tag}>, base::UnkStrIdKey<{tag}>>& base::refl_IterValueMoveAssign("
        f"base::UnkIterValue<UnkCloneWord<{tag}>, base::UnkStrIdKey<{tag}>>*, "
        f"base::UnkIterValue<UnkCloneWord<{tag}>, base::UnkStrIdKey<{tag}>>*);")),
    "iter_sync_owner_vec": Kind(0x71004A6D74, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_IterSync(base::UnkCollectionIter<UnkCloneOwnerVec<{tag}>, u32>*);")),
    "assign_handle": Kind(0x7100234F78, "base/UnkCloneHelpers.h", lambda tag, _: (
        "extern char g_unk_7101a209e0[];\n"
        f"template <> struct base::UnkRefSlotTraits<{tag}> {{\n"
        "    static char* vtable() { return g_unk_7101a209e0; }\n};\n"
        f"template base::UnkRefHandle* base::refl_AssignHandle<{tag}>(base::UnkRefHandle*, const base::UnkRefHandle*);")),
    "counted_deferred_add": Kind(0x71008415D8, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_CountedDeferredAdd(base::UnkCountedDeferredSet<{tag}>*, {tag}*);")),
    "strid_hash_erase": Kind(0x7100121358, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrIdHashNode<{tag}>* base::refl_StrIdHashErase(base::UnkStrIdHashMap<{tag}>*, "
        "const base::UnkStrIdMember*);")),
    "shared_record_assign": Kind(0x7100B8F094, "base/UnkSharedRecord.h", lambda tag, _: (
        f"template UnkSharedRecord<{tag}>& UnkSharedRecord<{tag}>::operator=(const UnkSharedRecord<{tag}>&);")),
    "vector_clear_wide_owner": Kind(0x71002B9890, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_ClearWide(base::UnkVector<UnkCloneOwner<{tag}>>*);")),
    "iter_destroy_vt": Kind(0x7100F4ADF8, "base/UnkCloneHelpers.h", lambda tag, _: (
        "extern char g_unk_7101cab1f8[];\n"
        f"template <> struct base::UnkDtorTraits<{tag}> {{\n"
        "    static char* vtable() { return g_unk_7101cab1f8; }\n};\n"
        f"template void base::refl_IterDestroy(base::UnkCollectionIter<base::UnkDtorElem<{tag}, 0x18, "
        "base::UnkStrIdAt<8>>, u32>*);")),
    "strid_record_assign": Kind(0x7100BBC1D0, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrIdRecord<{tag}>& base::UnkStrIdRecord<{tag}>::operator=("
        f"const base::UnkStrIdRecord<{tag}>&);")),
    "map_iter_sync_strkey": Kind(0x710011C448, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_MapIterSync(base::UnkMapIter<UnkCloneWord<{tag}>, UnkCloneStrId<{tag}>>*);")),
    "iter_sync_callbacks": Kind(0x710090B1C8, "base/UnkCloneHelpers.h", lambda tag, _: (
        "extern char g_unk_7101a33fa0[];\nextern char g_unk_7101a20998[];\n"
        f"template <> struct base::UnkCallbackTraits<{tag}> {{\n"
        "    static char* vtable() { return g_unk_7101a33fa0; }\n"
        "    static char* base_vtable() { return g_unk_7101a20998; }\n};\n"
        f"template void base::refl_IterSync(base::UnkCollectionIter<base::UnkCallbackList<{tag}>, u32>*);")),
    "strid_record_b_assign": Kind(0x71002E4FE8, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrIdRecordB<{tag}>& base::UnkStrIdRecordB<{tag}>::operator=("
        f"const base::UnkStrIdRecordB<{tag}>&);")),
    "plain_record_assign": Kind(0x710098E188, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkPlainRecord<{tag}>& base::UnkPlainRecord<{tag}>::operator=("
        f"const base::UnkPlainRecord<{tag}>&);")),
    "iter_value_copy_assign_strkey": Kind(0x7100120848, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkIterValue<UnkCloneWord<{tag}>, base::global::CStrId>& base::refl_IterValueCopyAssign("
        f"base::UnkIterValue<UnkCloneWord<{tag}>, base::global::CStrId>*, "
        f"const base::UnkIterValue<UnkCloneWord<{tag}>, base::global::CStrId>*);")),
    "strid_hash_insert": Kind(0x7100120F0C, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_StrIdHashInsert(base::UnkStrIdHashMap<{tag}>*, const base::global::CStrId*, "
        "const u64*);")),
    "iter_assign_refptr": Kind(0x7100237158, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkCollectionIter<UnkCloneRefPtr<{tag}>>& base::UnkCollectionIter<UnkCloneRefPtr<{tag}>>::"
        f"operator=(const base::UnkCollectionIter<UnkCloneRefPtr<{tag}>>&);")),
    "resize_sub_vecs": Kind(0x710065B1B0, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_ResizeSubVecs(base::UnkVector<base::UnkCloneSubVec<{tag}>>*, u32);")),
    "iter_value_assign_raw_strkey": Kind(0x71000C9490, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrKeyWordValue<{tag}>* base::refl_IterValueAssignRaw(base::UnkStrKeyWordValue<{tag}>*, "
        f"const base::UnkStrKeyWordValue<{tag}>*);")),
    "iter_value_assign_ref_strkey": Kind(0x71000C93EC, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrKeyWordValue<{tag}>* base::refl_IterValueAssignRef(base::UnkStrKeyWordValue<{tag}>*, "
        f"const base::UnkStrKeyWordValue<{tag}>::Ref*);")),
    "iter_value_assign_word": Kind(0x71000D3038, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_IterValueAssign(base::UnkIterValue<UnkCloneWord<{tag}>, u32>*, "
        f"const base::UnkIterValue<UnkCloneWord<{tag}>, u32>*);")),
    "strid_u16_record_assign": Kind(0x7100BA33CC, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrIdU16Record<{tag}>& base::UnkStrIdU16Record<{tag}>::operator=("
        f"const base::UnkStrIdU16Record<{tag}>&);")),
    "assign_handle_from_source": Kind(0x7100234CCC, "base/UnkCloneHelpers.h", lambda tag, _: (
        "extern char g_unk_7101a209e0[];\n"
        f"template <> struct base::UnkRefSlotTraits<{tag}> {{\n"
        "    static char* vtable() { return g_unk_7101a209e0; }\n};\n"
        f"template void base::refl_AssignHandleFromSource<{tag}>(base::UnkRefHandle*, const char*);")),
    "iter_value_move_from_owner_vec": Kind(0x71004A8728, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkIterValue<base::UnkVector<UnkCloneOwner<{tag}>>, u32>& base::refl_IterValueMoveFrom("
        f"base::UnkIterValue<base::UnkVector<UnkCloneOwner<{tag}>>, u32>*, "
        f"base::UnkIterValue<base::UnkVector<UnkCloneOwner<{tag}>>, u32>*);")),
    "deferred_remove": Kind(0x71002AE87C, "base/UnkDeferredSet.h", lambda tag, _: (
        f"template void base::UnkDeferredSet<{tag}>::Remove({tag}*);")),
    "strid_array_move": Kind(0x71000DF128, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template base::UnkStrIdArray<{tag}>& base::UnkStrIdArray<{tag}>::operator=(base::UnkStrIdArray<{tag}>&&);")),
    "vector_emplace_vec_handle": Kind(0x71004A940C, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::UnkVector<UnkCloneVecHandle<{tag}>>::EmplaceBack(UnkCloneVecHandle<{tag}>&&);")),
    "keyed_iter_sync_poly_vec": Kind(0x710023C550, "base/UnkCloneHelpers.h", lambda tag, _: (
        f"template void base::refl_KeyedIterSync(base::UnkKeyedIter<UnkClonePolyVec<{tag}>, UnkCloneStrId<{tag}>>*);")),
}
# Verified kinds first, so that a function matching both a reference and a probe takes the
# reference's instantiation.
for _name, (_header, _fmt, _refs) in COLLECTIONS.items():
    for _el, _ref in _refs.items():
        KINDS[f"{_name}_{_el}"] = Kind(_ref, _header, collection(_fmt, _el), exact=_el != "elem")
NO_PROBE = {("iter_value_copy", "owner"), ("iter_value_equal_by_value", "owner")}  # owners can't be copied
for _name, (_header, _fmt, _refs) in COLLECTIONS.items():
    trivial = "ptr" in _refs or "elem" in _refs or "word" in _refs
    for _el in ("owner", "handle") + (() if trivial else ("ptr",)):
        if _el not in _refs and (_name, _el) not in NO_PROBE:
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
            line = line.split("//")[0]  # a declaration with a trailing comment is still a declaration
            m = gen_lift.NAME_RE.match(line)
            if m and ("{" in line or not line.rstrip().endswith(";")):
                skip.add(int(m.group(1), 16))
            # A clone tag defines a function only where something is instantiated or specialised
            # with it; a forward declaration or alias that reuses one (src/game_lo/) doesn't.
            for t in gen_lift.TAG_RE.finditer(line):
                if not t.group(0).startswith("UnkClone_fn_") or line.lstrip().startswith("template"):
                    skip.add(int(t.group(1), 16))
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
            rows += [int(r["address"], 16) for r in csv.DictReader(fh) if r["status"] in ("U", "O", "W")]
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
