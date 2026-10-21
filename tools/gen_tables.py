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

The entries are recovered by a small register simulator over the straight-line code and
checked against both functions; any surprise stops the generator. The output is compiled and
diffed, and the result printed. The table's destructor isn't generated: it differs only in
register allocation.

Usage: python tools/gen_tables.py --version 2.1.0
Outputs are deterministic. Regenerate, never edit them.
"""

from __future__ import annotations

import argparse
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


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    args = ap.parse_args()

    original = asmdiff.Original(args.version)
    outputs = {f"strid_table_{TABLE_CTOR:x}.cpp": generate_strid_table(original)}

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
        for res in asmdiff.diff_object(obj, original):
            if res.address in seen:
                continue
            seen.add(res.address)
            state = "match" if res.matching else f"{res.score:.1%}"
            print(f"{path.relative_to(ROOT).as_posix()}: {res.address:#x} {res.orig_size} B {state}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
