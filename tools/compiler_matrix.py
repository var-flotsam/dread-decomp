#!/usr/bin/env python3
"""Compile a set of reconstructed functions with every candidate toolchain and flag set,
and score each configuration against the original binary with tools/asmdiff.py.

Sets:
  leaf     src/compiler_id/  10 leaf functions; axes: -O1/-O2/-O3/-Os x fast-math x cortex-a57
  nonleaf  src/flag_id/      20 calling functions; axes: -O2/-O3 x PIC model x unwind tables
                             x vectorisation / implicit float x frame pointer
                             (all with -mcpu=cortex-a57, settled by the leaf set)

Usage: python tools/compiler_matrix.py --version 2.1.0 [--set leaf|nonleaf] [--only 9.0.1,10.0.0]

Writes build/compiler_matrix/<version>/<set>/results.json and prints a ranking:
functions matched exactly, then mean per-function instruction match.
"""

from __future__ import annotations

import argparse
import itertools
import json
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import fetch_toolchains  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent

BASE_FLAGS = ["--target=aarch64-none-elf", "-std=c++1z", "-fno-exceptions", "-fno-rtti",
              "-ffunction-sections", "-I", str(ROOT / "include"), "-c"]

NOVEC = ["-fno-vectorize", "-fno-slp-vectorize"]

SETS = {
    "leaf": {
        "sources": "compiler_id",
        "opt": ["-O1", "-O2", "-O3", "-Os"],
        "extra": {
            "": [],
            "fast": ["-ffast-math"],
            "a57": ["-mcpu=cortex-a57"],
            "a57+fast": ["-mcpu=cortex-a57", "-ffast-math"],
        },
    },
    "nonleaf": {
        "sources": "flag_id",
        "opt": ["-O2", "-O3"],
        "extra": {
            # uw = -fasynchronous-unwind-tables (the original has .eh_frame for every function);
            # novec = -fno-vectorize -fno-slp-vectorize (no vector FP anywhere in game code).
            "a57": ["-mcpu=cortex-a57"],
            "a57 pic": ["-mcpu=cortex-a57", "-fPIC"],
            "a57 pic uw": ["-mcpu=cortex-a57", "-fPIC", "-fasynchronous-unwind-tables"],
            "a57 uw novec": ["-mcpu=cortex-a57", "-fasynchronous-unwind-tables", *NOVEC],
            "a57 pic uw novec": ["-mcpu=cortex-a57", "-fPIC", "-fasynchronous-unwind-tables", *NOVEC],
            "a57 pie uw novec": ["-mcpu=cortex-a57", "-fPIE", "-fasynchronous-unwind-tables", *NOVEC],
            # nif = -mno-implicit-float: no compiler-introduced SIMD (no q-register copies,
            # no movi zeroing, and both vectorisers off), as observed in game code.
            "a57 pic uw nif": ["-mcpu=cortex-a57", "-fPIC", "-fasynchronous-unwind-tables",
                               "-mno-implicit-float"],
            "a57 pic uw novec nofp": ["-mcpu=cortex-a57", "-fPIC", "-fasynchronous-unwind-tables", *NOVEC,
                                      "-fomit-frame-pointer"],
        },
    },
}


def run_config(version: str, set_name: str, compiler: str, opt: str, extra: str, original) -> dict:
    spec = SETS[set_name]
    sources = sorted((ROOT / "src" / spec["sources"]).glob("*.cpp"))
    name = f"clang-{compiler} {opt} {extra}".strip()
    out = ROOT / "build" / "compiler_matrix" / version / set_name / \
        name.replace(" ", "_").replace("+", "_")
    out.mkdir(parents=True, exist_ok=True)
    clang = fetch_toolchains.clang_path(compiler)
    funcs = {}
    for src in sources:
        obj = out / (src.stem + ".o")
        cmd = [str(clang), *BASE_FLAGS, opt, *spec["extra"][extra], str(src), "-o", str(obj)]
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode != 0:
            funcs[src.stem] = {"error": r.stderr.strip().splitlines()[-1] if r.stderr else "failed"}
            continue
        for res in asmdiff.diff_object(obj, original):
            funcs[src.stem] = {"match": res.matching, "score": round(res.score, 4),
                               "orig_size": res.orig_size, "our_size": res.our_size,
                               "diff_insns": len(res.diff_insns)}
    scored = [f for f in funcs.values() if "score" in f]
    return {"config": name, "compiler": compiler, "opt": opt, "extra": extra,
            "matched": sum(f["match"] for f in scored), "total": len(sources),
            "mean_score": round(sum(f["score"] for f in scored) / max(len(sources), 1), 4),
            "functions": funcs}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--set", default="leaf", choices=list(SETS))
    ap.add_argument("--only", help="comma-separated compiler versions")
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    compilers = args.only.split(",") if args.only else list(fetch_toolchains.CANDIDATES)
    original = asmdiff.Original(args.version)
    spec = SETS[args.set]
    configs = list(itertools.product(compilers, spec["opt"], spec["extra"]))
    with ThreadPoolExecutor(args.j) as pool:
        results = list(pool.map(lambda c: run_config(args.version, args.set, *c, original), configs))
    results.sort(key=lambda r: (-r["matched"], -r["mean_score"], r["config"]))

    out = ROOT / "build" / "compiler_matrix" / args.version / args.set / "results.json"
    out.write_text(json.dumps(results, indent=1), encoding="utf-8")
    print(f"{'config':34} matched  mean")
    for r in results[:25]:
        print(f"{r['config']:34} {r['matched']:>3}/{r['total']}  {r['mean_score']:.1%}")
    print(f"... {len(results)} configs; full results in {out.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
