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


def cast_traits(tag: str, calls: tuple[int, int], ret: str, params: str, args: str) -> list[str]:
    base, getter = calls
    return [f"const base::reflection::CType* fn_{getter:x}();",
            f"{ret} fn_{base:x}({params});",
            f"template <> struct base::reflection::UnkCastTraits<{tag}> {{",
            f"    static const CType* type() {{ return fn_{getter:x}(); }}",
            f"    static {ret} base({params}) {{ {'return ' if ret != 'void' else ''}fn_{base:x}({args}); }}", "};"]


def call_and_cast_arg(orig: asmdiff.Original, addr: int, tag: str, insns: Insns) -> str | None:
    """refl_CallAndCastArg (UnkReflCast.h): the base method, type getter and cache offset."""
    if len(insns) > 32 or ("mov", "x20, x1") not in insns[:8] or ("mov", "x19, x0") not in insns[:8] \
            or not any(m == "cbz" and o.startswith("x20,") for m, o in insns) or not (calls := cast_calls(insns)):
        return None
    offs = [int(h.group(1), 16) for m, o in insns if m == "str" and (h := re.fullmatch(r"x8, \[x19, #(0x[0-9a-f]+)\]", o))]
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
    srcs = [int(h.group(1), 16) for m, o in insns if m == "ldr" and (h := re.fullmatch(r"x21, \[x19, #(0x[0-9a-f]+)\]", o))]
    dsts = [int(h.group(1), 16) for m, o in insns if m == "str" and (h := re.fullmatch(r"x8, \[x19, #(0x[0-9a-f]+)\]", o))]
    if len(srcs) != 1 or len(dsts) != 1:
        return None
    return "\n".join(cast_traits(tag, calls, "bool", "char* s", "s") +
                     [f"template bool base::reflection::refl_CallAndCastField<{tag}, {srcs[0]:#x}, {dsts[0]:#x}>(char*);"])


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


KINDS = {
    "dtor": Kind(None, "base/UnkDestructors.h", lambda tag, _: None, scan=destructor),
    "iter_value_destroy_vt": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_vt),
    "iter_element_value": Kind(None, "base/reflection/UnkReflValue.h", lambda tag, _: None, scan=iter_element_value),
    "pointee_typed": Kind(None, "base/reflection/UnkReflTypedPtr.h", lambda tag, _: None, scan=pointee_typed),
    "iter_value_destroy_dtor": Kind(None, "base/UnkCollectionIter.h", lambda tag, _: None, scan=iter_value_destroy_dtor),
    "enum_values": Kind(None, "base/global/CStrId.h", lambda tag, _: None, scan=enum_values),
    "call_and_cast_arg": Kind(None, "base/reflection/UnkReflCast.h", lambda tag, _: None, scan=call_and_cast_arg),
    "call_and_cast_field": Kind(None, "base/reflection/UnkReflCast.h", lambda tag, _: None, scan=call_and_cast_field),
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
    "deferred_remove": Kind(0x71002AE87C, "base/UnkDeferredSet.h", lambda tag, _: (
        f"template void base::UnkDeferredSet<{tag}>::Remove({tag}*);")),
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
