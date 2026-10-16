"""Reconstruct reflection attribute and member-field functions (a class getter's x3/x4).

Both kinds receive the class descriptor and are sequences of two statement forms
(include/base/reflection/UnkReflAttributes.h):

  class attribute:  fn_710009dd6c(cls)->Add<T>(KEY(), value);
  member field:     UnkReflField* field0 = fn_7100096524(cls, CStrId("name", true), TYPE(), offset, -1, -1);
                    field0->attrs.Add<T>(KEY(), value);   // zero or more

and, at the end of the CTunable* classes' field functions, a registration with a global
registry that is created on first use.

parse() turns one function's code into C++ by walking its calls and the values stored
between them. The result is only a candidate. verify() compiles every candidate in one
scratch translation unit and keeps those that are byte-identical to the original.
"""

from __future__ import annotations

import struct
import subprocess
import sys
from dataclasses import dataclass, field
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
sys.path.insert(0, str(Path(__file__).resolve().parent / "recon"))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
from image import BASE  # noqa: E402

GET_ATTRS = 0x710009DD6C
ADD_ATTR = 0x71000934B8
ADD_FIELD = 0x7100096524
CSTRID_CTOR = 0x71000003D4
CSTRID_RELEASE = 0x71000803A4  # inlined ~CStrId
GLOBAL_CSTRID = 0x7101D470E0   # g_unk_7101d470e0
ALLOC = 0x71000839A0           # allocation (size, flags)
TUNABLE_REGISTRY_CTOR = 0x710013AC14
# Calls that are never attribute keys; any other call in key position is one.
NOT_KEYS = {GET_ATTRS, ADD_ATTR, ADD_FIELD, CSTRID_CTOR, CSTRID_RELEASE, ALLOC, TUNABLE_REGISTRY_CTOR}
# Value type getter -> C++ type, as in UnkReflTypeOf.
VALUE_TYPES = {0x7100005178: "u32", 0x7100014768: "s32", 0x71000081E8: "const char*", 0x7100009658: "base::global::CStrId",
               0x710000ABBC: "f32", 0x710000D228: "bool"}

PRELUDE = [
    '#include "base/reflection/UnkReflAttributes.h"',
    "using base::global::CStrId;",
    "using base::reflection::CClass;",
    "using base::reflection::UnkReflField;",
    "using base::reflection::UnkTunable;",
    "using base::reflection::UnkTunableRegistry;",
]
INT_TYPES = {1: "u8", 2: "s16", 4: "s32", 8: "s64"}  # enum-like values typed by a global descriptor


@dataclass
class Event:
    kind: str                # "bl" or "value"
    target: int = 0          # bl
    regs: dict = field(default_factory=dict)  # bl: register facts at the call
    value: tuple = ()        # value: (kind, payload)


def c_string(s: str) -> str:
    out = []
    for ch in s:
        if ch in '"\\':
            out.append("\\" + ch)
        elif 0x20 <= ord(ch) < 0x7F:
            out.append(ch)
        else:
            out.extend(f"\\{b:03o}" for b in ch.encode("utf-8"))
    return '"' + "".join(out) + '"'


def f32_literal(bits: int) -> str | None:
    """Shortest decimal literal that reads back as these float bits; None if not finite."""
    import math
    v = struct.unpack("<f", struct.pack("<I", bits))[0]
    if not math.isfinite(v):
        return None
    for digits in range(1, 10):
        s = f"{v:.{digits}g}"
        try:
            if struct.unpack("<I", struct.pack("<f", float(s)))[0] == bits:
                break
        except OverflowError:
            continue
    s = repr(float(s))  # the shortest digits, in Python's plain or exponent style
    return s + "f"


def _key(cs: capstone.Cs, reg: int) -> str:
    """Register identity: w8/x8 -> '8'; fp registers keep their bank."""
    name = cs.reg_name(reg)
    if name in ("wzr", "xzr"):
        return "zr"
    return name[1:] if name[0] in "wx" else name


def events(orig: asmdiff.Original, addr: int) -> list[Event] | None:
    cs = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)
    cs.detail = True
    page: dict[str, int] = {}
    facts: dict[str, tuple] = {"zr": ("imm", 0)}  # register -> ("imm", n) | ("str", s) | ("ptr", a) | ("global", a)
    out: list[Event] = []
    for x in cs.disasm(orig.function(addr), addr):
        m, o = x.mnemonic, x.operands
        written = {_key(cs, r) for r in x.regs_access()[1]}
        new: dict[str, tuple] = {}
        if m == "adrp":
            page[_key(cs, o[0].reg)] = o[1].imm
        elif m == "add" and len(o) == 3 and _key(cs, o[1].reg) in page and o[2].type == capstone.arm64.ARM64_OP_IMM:
            a = page[_key(cs, o[1].reg)] + o[2].imm
            s = orig.img.cstring(a - BASE)
            new[_key(cs, o[0].reg)] = ("str", s) if s is not None else ("addr", a)
        elif m in ("add", "sub") and len(o) == 3 and o[2].type == capstone.arm64.ARM64_OP_IMM \
                and facts.get(_key(cs, o[1].reg), ("",))[0] == "imm" and not o[2].shift.type:
            v = facts[_key(cs, o[1].reg)][1]
            new[_key(cs, o[0].reg)] = ("imm", (v + o[2].imm if m == "add" else v - o[2].imm) & (2**64 - 1))
        elif m == "ldr" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_MEM:
            base = _key(cs, o[1].mem.base)
            if base in page:
                target = orig.img.pointer(page[base] + o[1].mem.disp - BASE)
                if target is not None:
                    new[_key(cs, o[0].reg)] = ("ptr", BASE + target)
            elif facts.get(base, ("",))[0] == "ptr" and o[1].mem.disp == 0:
                new[_key(cs, o[0].reg)] = ("global", facts[base][1])
        elif m in ("mov", "movz", "orr") and o[-1].type == capstone.arm64.ARM64_OP_IMM \
                and (m != "orr" or _key(cs, o[1].reg) == "zr"):
            new[_key(cs, o[0].reg)] = ("imm", o[-1].imm & 0xFFFFFFFFFFFFFFFF)
        elif m == "mov" and len(o) == 2 and o[1].type == capstone.arm64.ARM64_OP_REG:
            src = facts.get(_key(cs, o[1].reg))
            if src:
                new[_key(cs, o[0].reg)] = src
        elif m == "movk":
            prev = facts.get(_key(cs, o[0].reg))
            if prev and prev[0] == "imm":
                shift = o[1].shift.value if o[1].shift.type else 0
                new[_key(cs, o[0].reg)] = ("imm", (prev[1] & ~(0xFFFF << shift)) | (o[1].imm << shift))
        elif m == "fmov" and o[1].type == capstone.arm64.ARM64_OP_FP:
            new[_key(cs, o[0].reg)] = ("fimm", o[1].fp)
        elif m in ("str", "strb", "strh", "stur", "sturb", "sturh") and \
                cs.reg_name(o[1].mem.base) in ("sp", "x29", "fp"):
            fact = facts.get(_key(cs, o[0].reg))
            if fact:
                width = {"strb": 1, "sturb": 1, "strh": 2, "sturh": 2}.get(m)
                if width is None:
                    width = 8 if cs.reg_name(o[0].reg)[0] in "xd" else 4
                out.append(Event("value", value=(fact[0], fact[1], width)))
        elif m in ("ldrb", "strb") and o[1].type == capstone.arm64.ARM64_OP_MEM and o[1].mem.disp == 0                 and facts.get(_key(cs, o[1].mem.base), ("",))[0] == "ptr":
            # Run-once flag (a global bool): test, then set to 1.
            g = facts[_key(cs, o[1].mem.base)][1]
            if m == "ldrb":
                out.append(Event("value", value=("flagtest", g, 1)))
            elif facts.get(_key(cs, o[0].reg)) == ("imm", 1):
                out.append(Event("value", value=("flagset", g, 1)))
        elif m == "cbz" and cs.reg_name(o[0].reg) == "x0" and not any(e.kind == "bl" for e in out):
            out.append(Event("value", value=("nullcheck", 0, 8)))
        elif m == "stp" and cs.reg_name(o[2].mem.base) in ("sp", "x29", "fp"):
            # The type half of a value ref, when it is a global descriptor.
            fact = facts.get(_key(cs, o[0].reg))
            if fact and fact[0] == "ptr":
                out.append(Event("value", value=("reftype", fact[1], 8)))
        elif m == "bl":
            out.append(Event("bl", target=o[0].imm, regs=dict(facts)))
            for r in [str(i) for i in range(19)] + ["s0", "d0"]:
                facts.pop(r, None)
                page.pop(r, None)
            continue
        elif m in ("b", "br", "blr"):
            if m != "b":
                return None
        for r in written:
            facts.pop(r, None)
            if m != "adrp":
                page.pop(r, None)
        facts.update(new)
        facts["zr"] = ("imm", 0)
    return out


def signed(v: int, width: int) -> int:
    v &= (1 << (8 * width)) - 1
    return v - (1 << (8 * width)) if v >> (8 * width - 1) else v


def value_expr(type_name: str, vals: list[tuple]) -> str | None:
    """C++ expression for a stored attribute value, or None."""
    stores = [v for v in vals if v[0] in ("imm", "str", "global", "fimm")]
    if len(stores) != 1:
        return None
    kind, payload, width = stores[0]
    if type_name == "const char*" and kind == "str" and width == 8:
        return c_string(payload)
    if type_name == "base::global::CStrId" and kind == "global" and payload == GLOBAL_CSTRID and width == 8:
        return "CStrId(g_unk_7101d470e0)"
    if type_name == "bool" and kind == "imm" and width == 1 and payload in (0, 1):
        return "true" if payload else "false"
    if type_name == "u32" and kind == "imm" and width == 4:
        return f"{payload & 0xFFFFFFFF:#x}u"
    if type_name == "s32" and kind == "imm" and width == 4:
        return str(signed(payload, 4))
    if type_name == "f32" and width == 4:
        if kind == "imm":
            return f32_literal(payload & 0xFFFFFFFF)
        if kind == "fimm":
            return f32_literal(struct.unpack("<I", struct.pack("<f", payload))[0])
    return None


def parse(orig: asmdiff.Original, addr: int, class_getters: set[int]) -> tuple[list[str], str] | None:
    """(extra declarations, definition) for one x3/x4 function, or None if the shape is unknown."""
    evs = events(orig, addr)
    if not evs:
        return None
    decls: list[str] = []
    body: list[str] = []
    i = 0
    n = len(evs)

    def next_bl(j: int) -> tuple[int, list[tuple]]:
        """Index of the next call at or after j, and the values stored before it."""
        vals = []
        while j < n and evs[j].kind != "bl":
            vals.append(evs[j].value)
            j += 1
        return j, vals

    def is_key(j: int) -> bool:
        return j < n and evs[j].kind == "bl" and evs[j].target not in NOT_KEYS \
            and evs[j].target not in VALUE_TYPES and evs[j].target not in class_getters

    def attribute(j: int, target: str) -> tuple[int, str] | None:
        """KEY call at j, its value, the value type getter and the insert."""
        j, vals = next_bl(j)
        if vals or not is_key(j):
            return None
        key = evs[j].target
        decls.append(f"const base::reflection::UnkReflAttrKey& fn_{key:x}();")
        j, vals = next_bl(j + 1)
        if j >= n:
            return None
        if evs[j].target == ADD_ATTR:
            # Integer (enum) value typed by a global descriptor: no getter call.
            stores = [v for v in vals if v[0] == "imm"]
            types = [v[1] for v in vals if v[0] == "reftype"]
            if len(types) != 1 or len(stores) != 1 or len(vals) != 2 or stores[0][2] not in INT_TYPES:
                return None
            width = stores[0][2]
            decls.append(f"extern base::reflection::CType g_unk_{types[0]:x};")
            return j + 1, (f"    {target}Add<{INT_TYPES[width]}>(fn_{key:x}(), &g_unk_{types[0]:x}, "
                           f"{signed(stores[0][1], width)});")
        if evs[j].target not in VALUE_TYPES and evs[j].target not in NOT_KEYS \
                and evs[j].target not in class_getters:
            # Integer (enum) value typed by its own type getter.
            getter = evs[j].target
            stores = [v for v in vals if v[0] == "imm"]
            if len(stores) != 1 or len(vals) != 1 or stores[0][2] not in INT_TYPES:
                return None
            width = stores[0][2]
            k, after = next_bl(j + 1)
            if k >= n or evs[k].target != ADD_ATTR or after:
                return None
            decls.append(f"const base::reflection::CType* fn_{getter:x}();")
            return k + 1, (f"    {target}Add<{INT_TYPES[width]}>(fn_{key:x}(), fn_{getter:x}(), "
                           f"{signed(stores[0][1], width)});")
        if evs[j].target == CSTRID_CTOR:
            s = evs[j].regs.get("1")
            if vals or not s or s[0] != "str" or evs[j].regs.get("2") != ("imm", 1):
                return None
            expr, type_name = f"CStrId({c_string(s[1])}, true)", "base::global::CStrId"
            j, vals = next_bl(j + 1)
            if j >= n or evs[j].target != 0x7100009658:
                return None
        else:
            type_name = VALUE_TYPES.get(evs[j].target)
            expr = value_expr(type_name, vals) if type_name else None
        if expr is None:
            return None
        j, vals = next_bl(j + 1)
        if j >= n or evs[j].target != ADD_ATTR or vals:
            return None
        type_arg = "CStrId" if type_name == "base::global::CStrId" else type_name
        return j + 1, f"    {target}Add<{type_arg}>(fn_{key:x}(), {expr});"

    def skip_release(j: int) -> int:
        """Skip the release calls of inlined ~CStrId (and nothing else)."""
        while j < n and evs[j].kind == "bl" and evs[j].target == CSTRID_RELEASE:
            j += 1
        return j

    fields = 0
    # Optional run-once prologue: if (flag) return; flag = true; if (!cls) return;
    head = [e.value for e in evs[:3] if e.kind == "value"]
    if head and head[0][0] == "flagtest":
        g = head[0][1]
        if head != [("flagtest", g, 1), ("flagset", g, 1), ("nullcheck", 0, 8)]:
            return None
        decls.append(f"extern bool g_unk_{g:x};")
        body += [f"    if (g_unk_{g:x})", "        return;", f"    g_unk_{g:x} = true;",
                 "    if (!cls)", "        return;"]
        i = 3
    while True:
        i = skip_release(i)
        if i >= n:
            break
        j, vals = next_bl(i)
        if j >= n:
            return None
        if vals:
            return None
        if evs[j].target == GET_ATTRS:
            r = attribute(j + 1, "fn_710009dd6c(cls)->")
            if r is None:
                return None
            i, line = r
            body.append(line)
        elif evs[j].target == CSTRID_CTOR:
            s = evs[j].regs.get("1")
            if not s or s[0] != "str" or evs[j].regs.get("2") != ("imm", 1):
                return None
            j, vals = next_bl(j + 1)
            if j >= n or vals or evs[j].target == CSTRID_CTOR:
                return None
            field_type = None
            if evs[j].target == ADD_FIELD:
                t = evs[j].regs.get("2")
                if not t or t[0] != "ptr":
                    return None
                field_type = t[1]
                decls.append(f"extern base::reflection::CType g_unk_{field_type:x};")
                type_expr = f"&g_unk_{field_type:x}"
            else:
                type_getter = evs[j].target
                if type_getter not in VALUE_TYPES:
                    ret = "base::reflection::CClass" if type_getter in class_getters else "base::reflection::CType"
                    decls.append(f"const {ret}* fn_{type_getter:x}();")
                type_expr = f"fn_{type_getter:x}()"
                j, vals = next_bl(j + 1)
                if j >= n or vals or evs[j].target != ADD_FIELD:
                    return None
            regs = evs[j].regs
            off = regs.get("3")
            if not off or off[0] != "imm" or regs.get("4") != ("imm", 2**64 - 1) or regs.get("5") != ("imm", 2**64 - 1):
                return None
            var = f"field{fields}"
            fields += 1
            body.append(f"    UnkReflField* {var} = fn_7100096524(cls, CStrId({c_string(s[1])}, true), "
                        f"{type_expr}, {off[1]:#x}, -1, -1);")
            i = j + 1
            while True:
                k = skip_release(i)
                kb, kvals = next_bl(k)
                if kvals or not is_key(kb):
                    i = k
                    break
                r = attribute(k, f"{var}->attrs.")
                if r is None:
                    return None
                i, line = r
                body.append(line)
        elif evs[j].target == ALLOC:
            # Tunable registration: create the global registry on first use, then register.
            regs = evs[j].regs
            size, flags = regs.get("0"), regs.get("1")
            slot = [v[1] for v in regs.values() if v[0] == "global"]
            if not size or size[0] != "imm" or not flags or flags[0] != "imm" or len(slot) != 1:
                return None
            j, vals = next_bl(j + 1)
            if j >= n or vals or evs[j].target != TUNABLE_REGISTRY_CTOR:
                return None
            j, vals = next_bl(j + 1)
            if j >= n or vals:
                return None
            regs = evs[j].regs
            obj, name = regs.get("1"), regs.get("2")
            if not obj or obj[0] != "ptr" or not name or name[0] != "str":
                return None
            register = evs[j].target
            decls += [f"extern UnkTunableRegistry* g_unk_{slot[0]:x};",
                      f"extern UnkTunable g_unk_{obj[1]:x};",
                      f"void fn_{register:x}(UnkTunableRegistry* registry, UnkTunable* obj, const char* name);"]
            body += [f"    UnkTunableRegistry* registry = g_unk_{slot[0]:x};",
                     "    if (!registry) {",
                     f"        registry = static_cast<UnkTunableRegistry*>(fn_71000839a0({size[1]:#x}, {flags[1]:#x}));",
                     "        fn_710013ac14(registry);",
                     f"        g_unk_{slot[0]:x} = registry;",
                     "    }",
                     f"    fn_{register:x}(registry, &g_unk_{obj[1]:x}, {c_string(name[1])});"]
            i = j + 1
        else:
            return None
    if not body:
        return None
    src = f"void fn_{addr:x}(CClass* cls) {{\n" + "\n".join(body) + "\n}"
    return list(dict.fromkeys(decls)), src


def verify(version: str, candidates: dict[int, tuple[list[str], str]], workdir: Path) -> set[int]:
    """Compile all candidates in one translation unit; return the addresses that match."""
    if not candidates:
        return set()
    workdir.mkdir(parents=True, exist_ok=True)
    src = workdir / "candidates.cpp"
    obj = workdir / "candidates.o"
    decls: list[str] = []
    seen: set[str] = set()
    for d, _ in candidates.values():
        for line in d:
            if line not in seen:
                seen.add(line)
                decls.append(line)
    text = PRELUDE + decls + [s for _, s in candidates.values()]
    src.write_text("\n".join(text) + "\n", encoding="utf-8")
    r = subprocess.run(buildcfg.cxx_command(src, obj), capture_output=True, text=True)
    if r.returncode != 0:
        raise SystemExit(f"candidate attribute functions failed to compile:\n{r.stderr[:4000]}")
    orig = asmdiff.Original(version)
    return {res.address for res in asmdiff.diff_object(obj, orig) if res.matching}
