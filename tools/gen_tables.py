#!/usr/bin/env python3
"""Generate src/tables/: large functions that only walk a fixed table of string literals.

Some of the largest functions in main are one statement repeated per table entry. Their source
is a short template plus the entries, and the entries (string literals, lengths, member
offsets) can be read back from the code:

  strid_table   A struct of 1,198 string ids, built from literals with precomputed CRC64 hashes
                (constructor fn_7100031894, id constructor fn_71000406dc; destructor
                fn_71000407f0 not generated), and a function that passes each literal with an empty
                temporary to fn_7100065b14 (fn_7100057eb8). One original translation unit,
                in that order.
  video_map     The loading screen's setup (fn_7100ef378c): two widgets, then 79 inserts of
                (key, video path, label, five flags) into a chained hash map, each inlined.
                Two entries copy a global path instead of building one from a literal. The
                two globals the widget creation reads through the GOT (the scene object and
                a string id) are named from their relocation targets.
  boss_rush     The boss rush setup (fn_7100db8dec): twelve stages, each a few string ids, flags,
                a hash and two arrays of (item, amount) pushes, every push inlined. Emitted as
                NON_MATCHING(minor): the code is the original's block for block, but register
                allocation differs from the third stage on (spills and rematerialised constants).

The entries are recovered by a small register simulator over the straight-line code and
checked against both functions; any surprise stops the generator. The output is compiled and
diffed, and the result printed. The table's destructor isn't generated: it differs only in
register allocation.

Generating video_map also depends on CFilePathStrId's copy being a defaulted copy of its pointer
(UnkFilePathStrIdBase), as UnkStrIdBase's is for CStrId: the pointer is then copied as an integer,
which fixes the operand order of the compare in its copy constructor.

Usage: python tools/gen_tables.py --version 2.1.0
Outputs are deterministic. Regenerate, never edit them.
"""

from __future__ import annotations

import argparse
import struct
import subprocess
import sys
from pathlib import Path

import capstone

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import buildcfg  # noqa: E402
from image import BASE  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
OUT = ROOT / "src" / "tables"

CS = capstone.Cs(capstone.CS_ARCH_ARM64, capstone.CS_MODE_ARM)

# strid_table addresses
TABLE_CTOR = 0x7100031894
ID_CTOR = 0x71000406dc
TABLE_DTOR = 0x71000407f0
NAMES_FN = 0x7100057eb8
CRC64 = 0x7100001570
TMP_CTOR = 0x710007e704
TMP_DTOR = 0x710007ecd4
NAME_SINK = 0x7100065b14

# video_map addresses
VIDEO_FN = 0x7100ef378c
PATH_CTOR = 0x71000166c8
STRID_CTOR = 0x71000003d4
WIDGET_CTOR = 0x710021d3d4  # the first call: a widget on the scene object's +0x218 for a string id
GLOBAL_PATH = 0x71729db168  # a CFilePathStrId copied by two entries
VIDEO_DEFAULT_FLAGS = (1, 1, 0, 0, 0)
# Keys inserted with the copy taken inside Insert (InsertCopy): the original has the two
# moved-from ids in the other order for these, and only this contiguous run reproduces it.
VIDEO_COPY_KEYS = range(49, 72)

# boss_rush addresses
RUSH_FN = 0x7100db8dec
RUSH_STRID_RELEASE = 0x71000803a4
RUSH_STAGES = 0x1b8  # offset of the first stage in the owner
RUSH_STAGE_SIZE = 0xa8
RUSH_STAGE_COUNT = 12
# Stage fields by offset: (C++ type, name). Ids are UnkStrId: its destructor lets the moved-from
# temporaries fold away exactly as in the original.
RUSH_FIELDS = {0x0: "id", 0x8: "id", 0x10: "id", 0x18: "id", 0x20: "u32", 0x28: "id", 0x30: "id",
               0x38: "id", 0x40: "f32", 0x48: "hash", 0x50: "f32", 0x58: "vec", 0x80: "vec"}


class TableError(Exception):
    pass


def imm(text: str) -> int:
    text = text.strip().lstrip("#")
    return int(text, 0)


def reg(name: str) -> str:
    """x/w register names to one key."""
    return "x" + name[1:] if name[0] in "wx" and name[1:].isdigit() else name


def simulate_calls(original: asmdiff.Original, address: int) -> list[tuple[int, list]]:
    """(callee, [x0..x4]) for each call of a straight-line function, values symbolic:
    ("arg0", off) = first argument + off, ("abs", addr), ("imm", n), ("fp", off), ("sp", off),
    ("ret", call index); None when unknown. The final tail call counts as a call."""
    code = original.function(address)
    insns = list(CS.disasm(code, address))
    regs: dict[str, tuple | None] = {f"x{k}": (f"arg{k}", 0) for k in range(8)}
    regs["x29"], regs["sp"] = ("fp", 0), ("sp", 0)
    out: list[tuple[int, list]] = []

    def plus(value, off):
        return (value[0], value[1] + off) if value and value[0] not in ("imm", "ret") else None

    for n, i in enumerate(insns):
        ops = [o.strip() for o in i.op_str.split(",")] if i.op_str else []
        m = i.mnemonic
        if m == "adrp":
            regs[reg(ops[0])] = ("abs", imm(ops[1]))
        elif m in ("add", "sub") and len(ops) >= 3 and ops[2].startswith("#"):
            off = imm(ops[2]) << (imm(ops[3].split()[-1]) if len(ops) == 4 else 0)
            regs[reg(ops[0])] = plus(regs.get(reg(ops[1])), off if m == "add" else -off)
        elif m == "add" and len(ops) == 3:
            b = regs.get(reg(ops[2]))
            regs[reg(ops[0])] = plus(regs.get(reg(ops[1])), b[1]) if b and b[0] == "imm" else None
        elif m == "mov" and ops[1].startswith("#"):
            regs[reg(ops[0])] = ("imm", imm(ops[1]))
        elif m == "mov":
            regs[reg(ops[0])] = regs.get(reg(ops[1]))
        elif m == "bl" or (m == "b" and n == len(insns) - 1):
            out.append((imm(ops[0]), [regs.get(f"x{k}") for k in range(5)]))
            for k in range(19):
                regs.pop(f"x{k}", None)
            regs["x0"] = ("ret", len(out) - 1)
        elif m == "str" and len(ops) == 3 and ops[1].endswith("]"):  # post-index writeback
            base = reg(ops[1].strip("[]"))
            regs[base] = plus(regs.get(base), imm(ops[2]))
        elif m in ("stp", "ldp", "str", "ret"):
            pass
        elif m in ("cbz", "cbnz", "b") or m.startswith("b."):
            raise TableError(f"{address:#x}: unexpected branch at {i.address:#x}")
        elif ops:
            regs[reg(ops[0])] = None
    return out


def cstring(original: asmdiff.Original, address: int) -> bytes:
    data = original.img.data
    end = data.index(b"\0", address - BASE)
    return data[address - BASE:end]


def literal(s: bytes) -> str:
    """A C++ string literal for s (octal escapes are always three digits)."""
    out = []
    for c in s:
        ch = chr(c)
        if ch in '"\\':
            out.append("\\" + ch)
        elif 32 <= c < 127 and ch != "?":
            out.append(ch)
        else:
            out.append(f"\\{c:03o}")
    return '"' + "".join(out) + '"'


def strid_entries(original: asmdiff.Original) -> list[bytes]:
    calls = simulate_calls(original, TABLE_CTOR)
    if len(calls) % 2:
        raise TableError("strid_table: odd number of calls")
    entries = []
    for k in range(0, len(calls), 2):
        (hash_fn, h), (ctor, c) = calls[k], calls[k + 1]
        if hash_fn != CRC64 or ctor != ID_CTOR:
            raise TableError(f"strid_table: unexpected calls {hash_fn:#x}, {ctor:#x}")
        this, s, n, hashed, flag = c
        if (this != ("arg0", 8 * (len(entries) + 1)) or not s or s[0] != "abs" or h[0] != s
                or h[1] != n or not n or n[0] != "imm" or hashed != ("ret", k) or flag != ("imm", 1)):
            raise TableError(f"strid_table: entry {len(entries)} doesn't fit the pattern")
        text = cstring(original, s[1])
        if len(text) != n[1]:
            raise TableError(f"strid_table: entry {len(entries)} length {n[1]} != strlen {len(text)}")
        entries.append(text)

    # fn_7100057eb8: an empty temporary per entry, passed with the literal; all destroyed at the end.
    calls = simulate_calls(original, NAMES_FN)
    count = len(entries)
    if len(calls) != 3 * count:
        raise TableError("strid_table: names function has an unexpected number of calls")
    for k, text in enumerate(entries):
        (tc, t), (sink, a) = calls[2 * k], calls[2 * k + 1]
        if tc != TMP_CTOR or sink != NAME_SINK or not t[1] or cstring(original, t[1][1]) != b"":
            raise TableError(f"strid_table: names entry {k} doesn't fit the pattern")
        if not a[0] or a[0][0] != "abs" or cstring(original, a[0][1]) != text or a[1] != t[0]:
            raise TableError(f"strid_table: names entry {k} differs from the table")
        if calls[2 * count + k][0] != TMP_DTOR or calls[2 * count + k][1][0] != calls[2 * (count - 1 - k)][1][0]:
            raise TableError(f"strid_table: temporaries not destroyed in reverse order")
    return entries


HEADER = """\
// GENERATED by tools/gen_tables.py: do not edit; regenerate.
// A struct of {count} string ids built from literals with precomputed CRC64 hashes, and a function
// that passes each literal to fn_7100065b14 (one original translation unit, in this order).

#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

// Placeholder tags naming the generated functions (the struct is not instantiated otherwise).
struct UnkTable_fn_{id_ctor:x};
struct UnkTable_fn_{table_ctor:x};

// A string id built from a literal whose length and hash the caller computed (CStrId's layout).
// Unlike CStrId's constructor, it doesn't check the string, and its destructor decrements and
// tests in two steps (ref_count is a union member, so the id and count are reloaded).
template <class Tag>
struct UnkStrIdHashed {{
    UnkStringData* data;

    UnkStrIdHashed(const char* str, u64 length, u64 hash, bool flag);
    ~UnkStrIdHashed() {{
        if (UnkStringPool* pool = g_unk_7101d47108) {{
            if (data && data != &pool->empty) {{
                --data->ref_count;
                if (data->ref_count == 0) {{
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }}
            }}
            data = &pool->empty;
        }}
    }}
}};

using UnkStrIdH = UnkStrIdHashed<UnkTable_fn_{id_ctor:x}>;
#define UNK_STRID(s) UnkStrIdH(s, sizeof(s) - 1, fn_7100001570(s, sizeof(s) - 1), true)

template <class Tag>
struct UnkStrIdTable {{
    u32 unk_0x0 = 0;
"""

FOOTER = """\
    UnkStrIdTable();
    // fn_{table_dtor:x}, the implicit destructor, isn't generated: with every id's destructor
    // inlined it differs only in register allocation (the pool alternates between x8 and x0).
}};

template <class Tag>
UnkStrIdTable<Tag>::UnkStrIdTable() {{}}
template UnkStrIdTable<UnkTable_fn_{table_ctor:x}>::UnkStrIdTable();

template <class Tag>
UnkStrIdHashed<Tag>::UnkStrIdHashed(const char* str, u64 length, u64 hash, bool flag) {{
    data = nullptr;
    if (!g_unk_7101d47108)
        UnkEnsureStringPool();
    data = fn_71000804f8(g_unk_7101d47108, hash);
    if (data)
        ++data->ref_count;
    else
        data = fn_71000801e4(g_unk_7101d47108, hash, str, length, flag);
}}
template UnkStrIdH::UnkStrIdHashed(const char* str, u64 length, u64 hash, bool flag);

// A {tmp_size:#x}-byte value built from a string (fn_710007e704, destroyed by fn_710007ecd4).
struct UnkStrValue;
void fn_710007e704(UnkStrValue* self, const char* str);
void fn_710007ecd4(UnkStrValue* self);
struct UnkStrValue {{
    alignas(8) u8 unk_0x0[{tmp_size:#x}];

    UnkStrValue(const char* str) {{ fn_710007e704(this, str); }}
    ~UnkStrValue() {{ fn_710007ecd4(this); }}
}};
void fn_7100065b14(const char* str, UnkStrValue& value);

void fn_{names_fn:x}() {{
"""


def generate_strid_table(original: asmdiff.Original) -> str:
    entries = strid_entries(original)
    fmt = dict(count=len(entries), id_ctor=ID_CTOR, table_ctor=TABLE_CTOR, table_dtor=TABLE_DTOR,
               names_fn=NAMES_FN, tmp_size=0x30)
    lines = [HEADER.format(**fmt)]
    for k, text in enumerate(entries):
        lines.append(f"    UnkStrIdH id_{8 * (k + 1):#x} = UNK_STRID({literal(text)});\n")
    lines.append(FOOTER.format(**fmt))
    for k, text in enumerate(entries):
        lines.append(f'    UnkStrValue v{k}("");\n    fn_7100065b14({literal(text)}, v{k});\n')
    lines.append("}\n")
    return "".join(lines)


def video_entries(original: asmdiff.Original) -> list[tuple[int, bytes | None, bytes, tuple[int, ...]]]:
    """(key, path literal or None for the global path, label, flags) per insert of fn_7100ef378c.

    Each insert allocates its node through a virtual call (blr); the literals are the arguments of
    the path and string-id constructors before it, the key and flags the node stores after it."""
    insns = list(CS.disasm(original.function(VIDEO_FN), VIDEO_FN))
    calls = [k for k, i in enumerate(insns) if i.mnemonic == "blr"]

    def literal_arg(k: int) -> bytes:
        # adrp x1, page; add x1, x1, #lo (the string argument of the call at k)
        lo = None
        for i in reversed(insns[max(0, k - 10):k]):
            if i.mnemonic == "add" and i.op_str.startswith("x1, x1, #"):
                lo = imm(i.op_str.split(", ")[2])
            if i.mnemonic == "adrp" and i.op_str.startswith("x1,") and lo is not None:
                return cstring(original, imm(i.op_str.split(", ")[1]) + lo)
        raise TableError(f"video_map: no string argument for the call at {insns[k].address:#x}")

    sizes = {"str": 4, "strb": 1, "strh": 2, "stur": 4, "sturb": 1, "sturh": 2}
    out = []
    start = 0
    for n, b in enumerate(calls):
        path = label = None
        global_path = False
        for k in range(start, b):
            i = insns[k]
            if i.mnemonic == "bl" and imm(i.op_str) == PATH_CTOR:
                path = literal_arg(k)
            elif i.mnemonic == "bl" and imm(i.op_str) == STRID_CTOR:
                label = literal_arg(k)
            elif i.mnemonic == "ldr" and "[" in i.op_str and "#" in i.op_str:
                # adrp xN, page; ldr xM, [xN, #lo] (the global path's pointer)
                base = i.op_str.split("[")[1].split(",")[0]
                for j in reversed(insns[max(start, k - 4):k]):
                    if j.mnemonic == "adrp" and j.op_str.split(",")[0] == base:
                        if imm(j.op_str.split(", ")[1]) + imm(i.op_str.split("#")[1].rstrip("]")) == GLOBAL_PATH:
                            global_path = True
                        break
        if label is None or (path is None) == (not global_path):
            raise TableError(f"video_map: insert {n} has no label or an unclear path")
        consts: dict[str, int] = {}  # by register number
        key = None
        flags: dict[int, int] = {}
        end = calls[n + 1] if n + 1 < len(calls) else len(insns)
        for k in range(start, end):
            i = insns[k]
            ops = [o.strip() for o in i.op_str.split(",")]
            if i.mnemonic == "mov" and ops[1].startswith("#"):
                consts[ops[0][1:]] = imm(ops[1]) & 0xFFFFFFFF
            elif i.mnemonic == "movk":
                shift = imm(ops[2].split()[-1]) if len(ops) > 2 else 0
                r = ops[0][1:]
                consts[r] = (consts.get(r, 0) & ~(0xFFFF << shift)) | (imm(ops[1]) << shift)
            elif k > b and i.mnemonic in sizes and ops[1].startswith("[x0"):
                value = 0 if ops[0] in ("wzr", "xzr") else consts.get(ops[0][1:])
                off = imm(ops[2].rstrip("]")) if len(ops) > 2 else 0
                size = 8 if sizes[i.mnemonic] == 4 and ops[0].startswith("x") else sizes[i.mnemonic]
                if value is None:
                    if off == 0 or off < 0x1d and off + size > 0x18:
                        raise TableError(f"video_map: unknown key or flags stored at {i.address:#x}")
                    continue  # the path and label pointers
                if off == 0 and size == 4:
                    key = value
                for q in range(size):
                    if 0x18 <= off + q <= 0x1c:
                        flags[off + q] = (value >> (8 * q)) & 0xFF
            if k > b and i.mnemonic == "b":
                break
        if key is None or sorted(flags) != list(range(0x18, 0x1d)) or any(v > 1 for v in flags.values()):
            raise TableError(f"video_map: insert {n} has no key or unexpected flags")
        out.append((key, None if global_path else path, label, tuple(flags[o] for o in range(0x18, 0x1d))))
        start = b + 1
    return out


def video_globals(original: asmdiff.Original) -> tuple[int, int]:
    """(scene, id): the two globals the widget creation at the start of fn_7100ef378c reads
    through the GOT, from the relocations of the slots it loads. The scene is the one whose
    object is dereferenced (its +0x218 is the call's first argument), the id the one passed as
    its fourth."""
    relocs = original.img.relocs
    regs: dict[str, tuple | None] = {}
    scene = ident = None
    for i in CS.disasm(original.function(VIDEO_FN), VIDEO_FN):
        ops = [o.strip() for o in i.op_str.split(",")] if i.op_str else []
        m = i.mnemonic
        base = regs.get(reg(ops[1].strip("[]"))) if len(ops) > 1 and ops[1].startswith("[") else None
        if m == "adrp":
            regs[reg(ops[0])] = ("page", imm(ops[1]))
        elif m == "ldr" and len(ops) == 3 and base and base[0] == "page":
            slot = base[1] + imm(ops[2].rstrip("]"))
            r = relocs.get(slot - BASE)
            if r is None:
                raise TableError(f"video_map: no relocation for the GOT slot {slot:#x}")
            regs[reg(ops[0])] = ("got", r.addend + BASE)
        elif m == "ldr" and len(ops) == 2 and base and base[0] == "got":
            scene = base[1]
            regs[reg(ops[0])] = ("object", scene)
        elif m == "mov" and ops[0] == "x3" and (regs.get(reg(ops[1])) or ("",))[0] == "got":
            ident = regs[reg(ops[1])][1]
        elif m == "bl":
            if imm(ops[0]) != WIDGET_CTOR or scene is None or ident is None:
                raise TableError(f"video_map: unexpected widget creation at {i.address:#x}")
            return scene, ident
        elif ops and m not in ("str", "stp", "cmp"):
            regs[reg(ops[0])] = None
    raise TableError("video_map: no widget creation at the start")


VIDEO_HEADER = """\
// GENERATED by tools/gen_tables.py: do not edit; regenerate.
// The loading screen's setup: two widgets, then {count} videos (path, label, flags) inserted by key
// into a chained hash map (the insert inlined each time).

#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "types.h"

using namespace base::global;

#define HIDDEN __attribute__((visibility("hidden")))

struct UnkVideo {{
    CFilePathStrId path;
    CStrId label;
    bool unk_0x10;
    bool unk_0x11;
    bool unk_0x12;
    bool unk_0x13;
    bool unk_0x14;

    UnkVideo(const CFilePathStrId& p, const CStrId& l, bool a = true, bool b = true, bool c = false,
             bool d = false, bool e = false)
        : path(p), label(l), unk_0x10(a), unk_0x11(b), unk_0x12(c), unk_0x13(d), unk_0x14(e) {{}}
}};

// A node's copy of an UnkVideo, moved in: the ids are taken over and the source's set to empty.
struct UnkVideoValue {{
    UnkStringData* path;
    UnkStringData* label;
    u8 unk_0x10;
    u8 unk_0x11;
    u8 unk_0x12;
    u8 unk_0x13;
    u8 unk_0x14;

    UnkVideoValue(UnkVideo& v) {{
        path = v.path.data;
        v.path.data = &g_unk_7101d47200->empty;
        label = v.label.data;
        v.label.data = &g_unk_7101d47108->empty;
        unk_0x10 = v.unk_0x10;
        unk_0x11 = v.unk_0x11;
        unk_0x12 = v.unk_0x12;
        unk_0x13 = v.unk_0x13;
        unk_0x14 = v.unk_0x14;
    }}
}};

struct UnkVideoNode {{
    u32 key;
    UnkVideoValue value;
    UnkVideoNode* next = nullptr;
    UnkVideoNode* prev = nullptr;
    UnkVideoNode* chain = nullptr;  // the next node in the same bucket

    UnkVideoNode(u32 k, UnkVideo& v) : key(k), value(v) {{}}
}};

// A key and a copy of the value, passed by value to Insert.
struct UnkVideoEntry {{
    u32 key;
    UnkVideo value;

    UnkVideoEntry(u32 k, const UnkVideo& v) : key(k), value(v) {{}}
}};

// The map's layout as base/UnkHashMap.h describes it (allocator, count, buckets, node list).
struct UnkVideoMap {{
    base::UnkAllocator* alloc;
    u32 alloc_flags;
    u32 count;
    UnkVideoNode** buckets;
    u32 used;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkVideoNode* tail;
    UnkVideoNode* head;

    __attribute__((always_inline)) void Add(u32 key, UnkVideo& value) {{
        UnkVideoNode** bucket = nullptr;
        if (used) {{
            u32 index = key % bucket_count;
            bucket = buckets + index;
        }}
        UnkVideoNode* first = *bucket;
        UnkVideoNode* n = static_cast<UnkVideoNode*>(alloc->vf_0x40(sizeof(UnkVideoNode), 8, alloc_flags));
        if (n)
            new (n) UnkVideoNode(key, value);
        n->chain = first;
        *bucket = n;
        if (!head) {{
            head = n;
        }} else if (tail) {{
            // Not a typed field store: the original reloads the tail after it.
            *reinterpret_cast<UnkVideoNode**>(reinterpret_cast<char*>(n) + 0x28) = tail;
            tail->next = n;
        }}
        tail = n;
        ++count;
    }}
    __attribute__((always_inline)) void Insert(UnkVideoEntry entry) {{ Add(entry.key, entry.value); }}
    // The same with the copy taken here (the moved-from ids are released in the other order).
    __attribute__((always_inline)) void InsertCopy(u32 key, const UnkVideo& value) {{
        UnkVideo copy(value);
        Add(key, copy);
    }}
}};

struct UnkRender {{
    u8 unk_0x0[0x78];
    u8* unk_0x78;
    u8 unk_0x80[0x8];
    union {{  // a store the original reloads around
        u32 unk_0x88;
    }};
}};
struct UnkWidget {{
    u8 unk_0x0[0x18];
    u8 unk_0x18[0x160];
    UnkRender* unk_0x178;
}};
struct UnkScene {{
    u8 unk_0x0[0x218];
    void* unk_0x218;
}};
struct UnkWidgetArg {{
    u64 unk_0x0;
    u32 unk_0x8;

    UnkWidgetArg() : unk_0x0(0), unk_0x8(0) {{}}
}};
extern UnkScene* g_unk_{scene_global:x};  // the global object (reached through the GOT)
extern CStrId g_unk_{id_global:x};  // a global string id
extern u8 unk_71729db160[] HIDDEN;
extern CFilePathStrId unk_{global_path:x} HIDDEN;
void fn_710021d3d4(void* a, void* b, UnkWidget** out, const CStrId* id);
bool fn_7100219ae0(UnkScene* scene, UnkWidget* widget, u32 layer);
void fn_71001c3504(UnkRender* render, void* p);
void fn_710020ac34(void* p, u32 v);
void fn_710019c284(u8* p);
void fn_710019c564(u8* p, f32 a, f32 b, f32 c, f32 d);
void fn_710020a4c4(void* p, const UnkWidgetArg& arg);

struct UnkLoadingScreen {{
    UnkVideoMap videos;
    u8 unk_0x48[0x108 - 0x48];
    UnkWidget* unk_0x108;
    UnkWidget* unk_0x110;
}};
void fn_7100f003e8(UnkLoadingScreen* self, s32 v);
void fn_7100f004dc(UnkLoadingScreen* self);

inline UnkWidget* UnkCreateWidget(u32 layer) {{
    UnkScene* scene = g_unk_{scene_global:x};
    UnkWidget* widget = nullptr;
    fn_710021d3d4(scene->unk_0x218, nullptr, &widget, &g_unk_{id_global:x});
    fn_7100219ae0(scene, widget, layer);
    return widget;
}}

inline void UnkSetUpWidget(UnkWidget*& w, u32 layer) {{
    w = UnkCreateWidget(layer);
    fn_71001c3504(w->unk_0x178, unk_71729db160);
    fn_710020ac34(w->unk_0x18, 0);
    u8* p = w->unk_0x178->unk_0x78;
    p[0x181] = 0;
    fn_710019c284(p);
    w->unk_0x178->unk_0x88 = 0;
    fn_710019c564(w->unk_0x178->unk_0x78, 1.0f, 1.0f, 1.0f, 1.0f);
    fn_710020a4c4(w->unk_0x18, UnkWidgetArg());
}}

void fn_{video_fn:x}(UnkLoadingScreen* self) {{
    UnkSetUpWidget(self->unk_0x108, 2);
    UnkSetUpWidget(self->unk_0x110, 3);
    fn_7100f003e8(self, -0x16);
    fn_7100f004dc(self);
"""


def generate_video_map(original: asmdiff.Original) -> str:
    entries = video_entries(original)
    scene, ident = video_globals(original)
    lines = [VIDEO_HEADER.format(count=len(entries), global_path=GLOBAL_PATH, video_fn=VIDEO_FN,
                                 scene_global=scene, id_global=ident)]
    for key, path, label, flags in entries:
        args = list(flags)
        while args and args[-1] == VIDEO_DEFAULT_FLAGS[len(args) - 1]:
            args.pop()
        extra = "".join(", true" if f else ", false" for f in args)
        p = f"CFilePathStrId({literal(path)}, true)" if path else f"CFilePathStrId(unk_{GLOBAL_PATH:x})"
        value = f"UnkVideo({p}, CStrId({literal(label)}, true){extra})"
        if key in VIDEO_COPY_KEYS:
            lines.append(f"    self->videos.InsertCopy({key}, {value});\n")
        else:
            lines.append(f"    self->videos.Insert(UnkVideoEntry({key}, {value}));\n")
    lines.append("}\n")
    return "".join(lines)


def rush_ops(original: asmdiff.Original) -> list[tuple]:
    """fn_7100db8dec's statements in order: ("strid", offset, text), ("hash", offset, text),
    ("store", offset, value), ("push", array offset, item address, amount bits).

    Ids are the string-id constructor's literal and the owner field the result is stored to;
    pushes are recognised by the allocator call (vf_0x40 of the allocator at array + 0x20) and
    the element's two stores after it; the item is a GOT entry, so its target comes from the
    relocation."""
    insns = list(CS.disasm(original.function(RUSH_FN), RUSH_FN))
    relocs = original.img.relocs

    def got(slot: int) -> int:
        r = relocs.get(slot - BASE)
        if r is None:
            raise TableError(f"boss_rush: no relocation for the GOT slot {slot:#x}")
        return r.addend + BASE

    allocs = {imm(i.op_str.split("#")[1].rstrip("]")) for k, i in enumerate(insns)
              if i.mnemonic == "ldr" and i.op_str.startswith("x0, [x19, #") and insns[k + 2].mnemonic == "lsl"}
    array_fields = {a - d for a in allocs for d in (0x20, 0x18, 0x14)}  # data, count, capacity
    regs: dict[str, tuple | None] = {}
    ops: list = []
    pending = None
    push = None
    allocations = 0
    last_id = None
    for k, i in enumerate(insns):
        m = i.mnemonic
        ops_ = [o.strip() for o in i.op_str.split(",")] if i.op_str else []
        value = lambda r: ("c", 0) if r in ("wzr", "xzr") else regs.get(reg(r))  # noqa: E731
        if m == "adrp":
            regs[reg(ops_[0])] = ("page", imm(ops_[1]))
        elif m == "add" and len(ops_) == 3 and ops_[2].startswith("#") and (regs.get(reg(ops_[1])) or ("",))[0] == "page":
            regs[reg(ops_[0])] = ("addr", regs[reg(ops_[1])][1] + imm(ops_[2]))
        elif (m == "ldr" and len(ops_) == 3 and ops_[2].startswith("#")
              and (regs.get(reg(ops_[1].lstrip("["))) or ("",))[0] == "page"):
            regs[reg(ops_[0])] = ("got", got(regs[reg(ops_[1].lstrip("["))][1] + imm(ops_[2].rstrip("]"))))
        elif m == "mov" and ops_[1].startswith("#"):
            regs[reg(ops_[0])] = ("c", imm(ops_[1]) & 0xFFFFFFFF)
        elif m == "movk":
            shift = imm(ops_[2].split()[-1]) if len(ops_) > 2 else 0
            v = regs.get(reg(ops_[0]))
            regs[reg(ops_[0])] = ("c", ((v[1] if v and v[0] == "c" else 0) & ~(0xFFFF << shift)) | (imm(ops_[1]) << shift))
        elif m == "mov":
            regs[reg(ops_[0])] = value(ops_[1])
        elif m in ("bl", "blr"):
            if m == "bl" and imm(ops_[0]) == STRID_CTOR:
                pending = ("strid", cstring(original, regs["x1"][1]))
            elif m == "bl" and imm(ops_[0]) == CRC64:
                text = cstring(original, regs["x0"][1])
                if regs["x1"] != ("c", len(text)):
                    raise TableError(f"boss_rush: hash length at {i.address:#x}")
                pending = ("hash", text, len(ops))
                ops.append(None)
            elif m == "blr":
                allocations += 1
                if allocations % 2:  # the allocation; the second blr frees the old array
                    base = next((imm(j.op_str.split("#")[1].rstrip("]")) for j in reversed(insns[k - 8:k])
                                 if j.mnemonic == "ldr" and j.op_str.startswith("x0, [x19, #")), None)
                    if base not in allocs:
                        raise TableError(f"boss_rush: no allocator for the call at {i.address:#x}")
                    push = [base - 0x20, None, None]
            for n in range(19):
                regs.pop(f"x{n}", None)
        elif m == "add" and ops_[-1] == "uxtw #4":
            regs[reg(ops_[0])] = ("element",)
        elif m in ("str", "strb", "strh") and "[" in i.op_str:
            mem = i.op_str.split("[")[1].rstrip("]!")
            base = mem.split(",")[0]
            off = imm(mem.split("#")[1]) if "#" in mem else 0
            if base == "x19":
                if pending and pending[0] == "strid" and ops_[0].startswith("x"):
                    ops.append(("strid", off, pending[1]))
                    last_id, pending = off, None
                elif m == "str" and ops_[0] == "x8" and off == last_id:
                    pass  # the same id stored on the no-pool path
                elif pending and pending[0] == "hash" and ops_[0] == "x0":
                    ops[pending[2]] = ("hash", off, pending[1])
                    pending = None
                elif off not in array_fields:
                    ops.append(("store", off, value(ops_[0])))
            elif push is not None and regs.get(reg(base)) == ("element",):
                v = value(ops_[0])
                if off == 0 and v and v[0] == "got":
                    push[1] = v[1]
                elif off == 8 and v and v[0] == "c":
                    push[2] = v[1]
                else:
                    raise TableError(f"boss_rush: unexpected element store at {i.address:#x}")
                if push[1] is not None and push[2] is not None:
                    ops.append(("push", *push))
                    push = None
        elif ops_ and m not in ("cmp", "cbz", "cbnz", "b", "ret", "stp", "ldp", "str", "tbz", "tbnz") and not m.startswith("b."):
            regs[reg(ops_[0])] = None
    if pending or push or None in ops or allocations % 2:
        raise TableError("boss_rush: unfinished statement")
    return ops


def rush_float(bits: int) -> str:
    """The shortest float literal with these bits."""
    f = struct.unpack("<f", struct.pack("<I", bits))[0]
    for digits in range(1, 10):
        text = f"{f:.{digits}g}"
        if struct.unpack("<f", struct.pack("<f", float(text)))[0] == f:
            break
    return (text if "." in text or "e" in text else text + ".0") + "f"


RUSH_HEADER = """\
// GENERATED by tools/gen_tables.py: do not edit; regenerate.
// The boss rush setup: for each of twelve stages, its string ids, flags and hash, then two arrays
// of (item, amount) pairs, every push inlined.

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using namespace base::global;

{items}
// An item and an amount. The empty destructor makes it non-trivially copyable, so a growing
// array moves the elements one by one (a 16-byte copy each), as in the original.
struct UnkItemAmount {{
    void* item;
    f32 amount;
    ~UnkItemAmount() {{}}
}};

struct UnkBossRushStage {{
    UnkStrId unk_0x0;
    UnkStrId unk_0x8;
    UnkStrId unk_0x10;
    UnkStrId unk_0x18;
    u32 unk_0x20;
    UnkStrId unk_0x28;
    UnkStrId unk_0x30;
    UnkStrId unk_0x38;
    f32 unk_0x40;
    u64 unk_0x48;  // a CRC64
    f32 unk_0x50;
    base::UnkVector<UnkItemAmount> unk_0x58;
    base::UnkVector<UnkItemAmount> unk_0x80;
}};
static_assert(sizeof(UnkBossRushStage) == {stage_size:#x}, "stage layout");

// An object and a function, registered or stored together.
struct UnkCallback {{
    void* obj;
    void (*fn)();
}};

struct UnkBossRush {{
    u8 pad_0x0[{stages:#x}];
    UnkBossRushStage stages[{count}];
    u8 pad_{end:x}[0xc88 - {end:#x}];
    UnkCallback unk_0xc88;
}};

extern void* g_unk_7101d47330;
void fn_71011995c0(void* owner, const UnkCallback& callback);
void fn_7100dd1358();
void fn_7100dd13f4();
void fn_7100dc82b4(UnkBossRush* self);

#define UNK_HASH(s) fn_7100001570(s, sizeof(s) - 1)

// NON_MATCHING(minor): register allocation differs from the third stage on (spills and
// rematerialised constants); the instructions are otherwise the original's, block for block.
void fn_{fn:x}(UnkBossRush* self) {{
    fn_71011995c0(g_unk_7101d47330, UnkCallback{{self, fn_7100dd1358}});
"""


def generate_boss_rush(original: asmdiff.Original) -> str:
    ops = rush_ops(original)
    calls = [i for i in CS.disasm(original.function(RUSH_FN), RUSH_FN) if i.mnemonic == "bl"]
    callees = {imm(i.op_str) for i in calls} - {STRID_CTOR, CRC64, RUSH_STRID_RELEASE}
    if callees != {0x71011995c0, 0x7100dc82b4}:
        raise TableError(f"boss_rush: unexpected calls {sorted(map(hex, callees))}")
    end = RUSH_STAGES + RUSH_STAGE_SIZE * RUSH_STAGE_COUNT
    items = sorted({op[2] for op in ops if op[0] == "push"})
    lines = [RUSH_HEADER.format(items="".join(f"extern char g_unk_{a:x}[];\n" for a in items),
                                stage_size=RUSH_STAGE_SIZE, stages=RUSH_STAGES, count=RUSH_STAGE_COUNT,
                                end=end, fn=RUSH_FN)]

    def field(off: int, kind: str) -> str:
        n, r = divmod(off - RUSH_STAGES, RUSH_STAGE_SIZE)
        if not 0 <= n < RUSH_STAGE_COUNT or RUSH_FIELDS.get(r) != kind:
            raise TableError(f"boss_rush: {kind} at owner offset {off:#x}")
        return f"self->stages[{n}].unk_{r:#x}"

    for op in ops:
        if op[0] == "strid":
            lines.append(f"    {field(op[1], 'id')} = UnkStrId({literal(op[2])}, true);\n")
        elif op[0] == "hash":
            lines.append(f"    {field(op[1], 'hash')} = UNK_HASH({literal(op[2])});\n")
        elif op[0] == "push":
            lines.append(f"    {field(op[1], 'vec')}.EmplaceBack({{g_unk_{op[2]:x}, {rush_float(op[3])}}});\n")
        elif op[1] < end:
            if op[2] is None or op[2][0] != "c":
                raise TableError(f"boss_rush: unknown value stored at {op[1]:#x}")
            n, r = divmod(op[1] - RUSH_STAGES, RUSH_STAGE_SIZE)
            kind = RUSH_FIELDS.get(r)
            text = str(op[2][1]) if kind == "u32" else rush_float(op[2][1])
            lines.append(f"    {field(op[1], kind)} = {text};\n")
        elif not (op[1] == 0xc88 or (op[1] == 0xc90 and op[2] == ("got", 0x7100dd13f4))):
            raise TableError(f"boss_rush: unexpected store to {op[1]:#x}")
    lines.append("    fn_7100dc82b4(self);\n    self->unk_0xc88 = UnkCallback{self, fn_7100dd13f4};\n}\n")
    return "".join(lines)


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()

    original = asmdiff.Original(args.version)
    outputs = {f"strid_table_{TABLE_CTOR:x}.cpp": generate_strid_table(original),
               f"video_map_{VIDEO_FN:x}.cpp": generate_video_map(original),
               f"boss_rush_{RUSH_FN:x}.cpp": generate_boss_rush(original)}

    OUT.mkdir(parents=True, exist_ok=True)
    for f in OUT.glob("*.cpp"):
        if f.name not in outputs:
            f.unlink()
    obj_dir = ROOT / "build" / "gen_tables" / args.version
    obj_dir.mkdir(parents=True, exist_ok=True)
    for name, text in outputs.items():
        path = OUT / name
        path.write_text(text, encoding="utf-8", newline="\n")
        obj = obj_dir / path.with_suffix(".o").name
        r = subprocess.run(buildcfg.cxx_command(path, obj), capture_output=True, text=True)
        if r.returncode != 0:
            print(f"BUILD FAILED {path.relative_to(ROOT)}\n{r.stderr}")
            return 1
        seen = set()
        for res in asmdiff.diff_object(obj, original, strict=True):
            if res.address in seen:
                continue
            seen.add(res.address)
            # reloc: byte-identical, but a relocation names the wrong callee, global or GOT slot
            state = "match" if res.matching else "reloc" if res.reloc_mismatches else f"{res.score:.1%}"
            print(f"{path.relative_to(ROOT).as_posix()}: {res.address:#x} {res.orig_size} B {state}")
            for m in res.reloc_mismatches:
                print(f"  reloc  {asmdiff.format_mismatch(m)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
