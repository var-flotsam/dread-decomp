#!/usr/bin/env python3
"""Compile src/compiler_id/*.cpp with every candidate toolchain and flag set, and
score each configuration against the original binary with tools/asmdiff.py.

Usage: python tools/compiler_matrix.py --version 2.1.0 [--only 9.0.1,10.0.0] [-j 8]

Writes build/compiler_matrix/<version>/results.json and prints a ranking:
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
SOURCES = sorted((ROOT / "src" / "compiler_id").glob("*.cpp"))

BASE_FLAGS = ["--target=aarch64-none-elf", "-std=c++1z", "-fno-exceptions", "-fno-rtti",
              "-ffunction-sections", "-I", str(ROOT / "include"), "-c"]
OPT = ["-O1", "-O2", "-O3", "-Os"]
EXTRA = {
    "": [],
    "fast": ["-ffast-math"],
    "a57": ["-mcpu=cortex-a57"],
    "a57+fast": ["-mcpu=cortex-a57", "-ffast-math"],
}


def run_config(version: str, compiler: str, opt: str, extra: str, original) -> dict:
    name = f"clang-{compiler} {opt} {extra}".strip()
    out = ROOT / "build" / "compiler_matrix" / version / name.replace(" ", "_").replace("+", "_")
    out.mkdir(parents=True, exist_ok=True)
    clang = fetch_toolchains.clang_path(compiler)
    funcs = {}
    for src in SOURCES:
        obj = out / (src.stem + ".o")
        cmd = [str(clang), *BASE_FLAGS, opt, *EXTRA[extra], str(src), "-o", str(obj)]
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
            "matched": sum(f["match"] for f in scored), "total": len(SOURCES),
            "mean_score": round(sum(f["score"] for f in scored) / len(SOURCES), 4),
            "functions": funcs}


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--only", help="comma-separated compiler versions")
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    compilers = args.only.split(",") if args.only else list(fetch_toolchains.CANDIDATES)
    original = asmdiff.Original(args.version)
    configs = list(itertools.product(compilers, OPT, EXTRA))
    with ThreadPoolExecutor(args.j) as pool:
        results = list(pool.map(lambda c: run_config(args.version, *c, original), configs))
    results.sort(key=lambda r: (-r["matched"], -r["mean_score"], r["config"]))

    out = ROOT / "build" / "compiler_matrix" / args.version / "results.json"
    out.write_text(json.dumps(results, indent=1), encoding="utf-8")
    print(f"{'config':32} matched  mean")
    for r in results[:25]:
        print(f"{r['config']:32} {r['matched']:>3}/{r['total']}  {r['mean_score']:.1%}")
    print(f"... {len(results)} configs; full results in {out.relative_to(ROOT)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
