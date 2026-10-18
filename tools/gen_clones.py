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
    ref: int     # verified reference address
    header: str  # where the template is defined
    # The explicit instantiation for one candidate, given its tag and instructions (None to skip).
    instantiate: Callable[[str, Insns], str | None]
    exact: bool = True  # match whole instructions (else mnemonics only)


def elem_size(insns: Insns) -> int | None:
    """The element size of an UnkVector method: 1 << the shift of the allocation size."""
    for m, o in insns:
        hit = re.fullmatch(r"lsl x1, x8, #(\d)", f"{m} {o}")
        if hit:
            return 1 << int(hit.group(1))
    return None


def vector(method: str, element: str) -> Callable[[str, Insns], str | None]:
    """UnkVector<element>::method for one candidate. `element` is "elem" (trivially copyable,
    sized from the code), "owner" or "handle" (pointer-sized; see UnkClone.h)."""
    signature = {"GrowAt": ("{t}&", "(u32)"), "Insert": ("void", "({t}&&, u32)")}[method]

    def build(tag: str, insns: Insns) -> str | None:
        size = elem_size(insns)
        if size is None or (element != "elem" and size != 8):
            return None
        t = {"elem": f"UnkCloneElem<{tag}, {size}>", "owner": f"UnkCloneOwner<{tag}>",
             "handle": f"UnkCloneHandle<{tag}>"}[element]
        ret, params = signature
        return f"template {ret.format(t=t)} base::UnkVector<{t}>::{method}{params.format(t=t)};"
    return build


KINDS = {
    "register_named": Kind(0x7100072940, "base/UnkNamedRegistry.h", lambda tag, _: (
        f"template base::UnkNamedObject* base::refl_RegisterNamed<{tag}>("
        "base::UnkNamedRegistry*, base::UnkNamedObject*, const char*);")),
    "vector_grow_at": Kind(0x71000305D8, "base/UnkVector.h", vector("GrowAt", "elem"), exact=False),
    "vector_grow_at_owner": Kind(0x71002B9ED8, "base/UnkVector.h", vector("GrowAt", "owner"), exact=False),
    "vector_grow_at_handle": Kind(0x7100020CD0, "base/UnkVector.h", vector("GrowAt", "handle"), exact=False),
    "vector_insert": Kind(0x71000D37D8, "base/UnkVector.h", vector("Insert", "elem"), exact=False),
    "vector_insert_owner": Kind(0x71002B9C74, "base/UnkVector.h", vector("Insert", "owner"), exact=False),
}


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
    refs = {}
    for name, k in KINDS.items():
        ins = instructions(orig, k.ref)
        refs[name] = (orig.sizes[k.ref], ins if k.exact else [m for m, _ in ins])
    found: dict[int, tuple[str, str]] = {}  # address -> (kind, instantiation)
    for a in rows:
        if a in taken or a not in orig.sizes:
            continue
        ins = None
        for name, (size, code) in refs.items():
            if orig.sizes[a] != size:
                continue
            ins = ins or instructions(orig, a)
            if (ins if KINDS[name].exact else [m for m, _ in ins]) == code:
                tag = f"UnkClone_fn_{a:x}"
                line = KINDS[name].instantiate(tag, ins)
                if line:
                    found[a] = (name, f"struct {tag};\n{line}")
                break

    def includes(addrs: list[int]) -> list[str]:
        return PRELUDE + [f'#include "{h}"' for h in sorted({KINDS[found[a][0]].header for a in addrs})]

    work = ROOT / "build" / "gen_clones"
    work.mkdir(parents=True, exist_ok=True)
    src, obj = work / "candidates.cpp", work / "candidates.o"
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
