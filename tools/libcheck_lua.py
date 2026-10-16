#!/usr/bin/env python3
"""Identify the embedded Lua 5.1 release and its build flags.

Builds upstream Lua 5.1.x standard libraries (from tools/fetch_sources.py) with a
float lua_Number (the game's build uses float: math.abs is `fabs s0`) and compares
every library C function against main. C function names are mapped to addresses
by matching each source luaL_Reg array to the binary's binding tables
(build/recon/<ver>/main_lua_tables.json, from tools/recon/run.py).

Usage: python tools/libcheck_lua.py --version 2.1.0 [--lua 5.1.4,5.1.5] [--compiler 9.0.1]
Writes build/libcheck/lua/<version>/results.json and prints matches per config.
"""

from __future__ import annotations

import argparse
import itertools
import json
import re
import shutil
import subprocess
import sys
from pathlib import Path

sys.path.insert(0, str(Path(__file__).resolve().parent))
import asmdiff  # noqa: E402
import check  # noqa: E402
import fetch_sources  # noqa: E402
import fetch_toolchains  # noqa: E402

ROOT = Path(__file__).resolve().parent.parent
LIBS = ["lmathlib.c", "lbaselib.c", "lstrlib.c", "ltablib.c"]
REG_RE = re.compile(r"static\s+const\s+luaL_Reg\s+\w+\s*\[\]\s*=\s*\{(.*?)\};", re.S)
ENTRY_RE = re.compile(r"\{\s*\"(\w+)\"\s*,\s*(\w+)\s*\}")

# luaconf.h edits for a float lua_Number (the binary's math library is single precision).
FLOAT_PATCH = [
    ("#define LUA_NUMBER_DOUBLE\n", ""),
    ("#define LUA_NUMBER\tdouble", "#define LUA_NUMBER\tfloat"),
    ('#define LUA_NUMBER_SCAN\t\t"%lf"', '#define LUA_NUMBER_SCAN\t\t"%f"'),
    ('#define LUA_NUMBER_FMT\t\t"%.14g"', '#define LUA_NUMBER_FMT\t\t"%.7g"'),
    ("#define lua_str2number(s,p)\tstrtod((s), (p))", "#define lua_str2number(s,p)\tstrtof((s), (p))"),
]

GAME_FLAGS = [f for f in check.CFLAGS if f not in ("-I", str(ROOT / "include"), "-c", "-std=c++1z",
                                                    "-fno-rtti", "-fno-exceptions")]
CONFIGS = {
    "c++ game-flags": ["-x", "c++", "-std=c++1z", "-fno-rtti", "-fno-exceptions", *GAME_FLAGS],
    "c game-flags": ["-x", "c", "-std=gnu99", *GAME_FLAGS],
    "c++ game-flags -O2": ["-x", "c++", "-std=c++1z", "-fno-rtti", "-fno-exceptions",
                           *[("-O2" if f == "-O3" else f) for f in GAME_FLAGS]],
    "c++ game-flags no-unwind": ["-x", "c++", "-std=c++1z", "-fno-rtti", "-fno-exceptions",
                                 *[f for f in GAME_FLAGS if f != "-fasynchronous-unwind-tables"]],
}


def prepare(version: str) -> Path:
    src = fetch_sources.source_dir(f"lua-{version}") / "src"
    if not src.is_dir():
        raise SystemExit(f"lua-{version} not fetched; run tools/fetch_sources.py lua-{version}")
    out = ROOT / "build" / "libcheck" / "lua" / f"lua-{version}"
    if out.exists():
        shutil.rmtree(out)
    shutil.copytree(src, out)
    conf = (out / "luaconf.h").read_text(encoding="latin-1")
    for old, new in FLOAT_PATCH:
        if old not in conf:
            raise SystemExit(f"lua-{version}: luaconf.h patch anchor not found: {old!r}")
        conf = conf.replace(old, new)
    (out / "luaconf.h").write_text(conf, encoding="latin-1")
    return out


def address_map(src_dir: Path, tables: list[dict]) -> dict[str, int]:
    """C function name -> address, by matching source luaL_Reg arrays to binary tables."""
    by_names = {tuple(e["name"] for e in t["entries"]): t for t in tables}
    out: dict[str, int] = {}
    for lib in LIBS:
        text = (src_dir / lib).read_text(encoding="latin-1")
        for body in REG_RE.findall(text):
            entries = ENTRY_RE.findall(body)
            key = tuple(n for n, _ in entries)
            table = by_names.get(key)
            if table is None:
                continue
            for (_, cfunc), e in zip(entries, table["entries"]):
                out.setdefault(cfunc, int(e["func"], 16))
    return out


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--version", required=True)
    ap.add_argument("--lua", default="5.1,5.1.1,5.1.2,5.1.3,5.1.4,5.1.5")
    ap.add_argument("--compiler", default=check.COMPILER)
    args = ap.parse_args()

    original = asmdiff.Original(args.version)
    tables = json.loads((ROOT / "build" / "recon" / args.version / "main_lua_tables.json").read_text())
    clang = fetch_toolchains.clang_path(args.compiler)
    results = []
    for ver, (cfg, flags) in itertools.product(args.lua.split(","), CONFIGS.items()):
        src = prepare(ver.removeprefix("lua-")) if not (ROOT / "build" / "libcheck" / "lua" /
                                                        f"lua-{ver}").exists() else \
            ROOT / "build" / "libcheck" / "lua" / f"lua-{ver}"
        amap = address_map(src, tables)
        objdir = src / "obj" / cfg.replace(" ", "_")
        objdir.mkdir(parents=True, exist_ok=True)
        funcs = {}
        for lib in LIBS:
            obj = objdir / lib.replace(".c", ".o")
            cmd = [str(clang), *flags, "-I", str(src), "-I", str(ROOT / "include" / "libc"),
                   "-DLUA_LIB", "-c", str(src / lib), "-o", str(obj)]
            r = subprocess.run(cmd, capture_output=True, text=True)
            if r.returncode != 0:
                raise SystemExit(f"lua-{ver} {cfg} {lib}: build failed\n{r.stderr[-2000:]}")
            names = check.demangle([n for n, _, _ in asmdiff.object_functions(obj)])
            for res in asmdiff.diff_object(obj, original, lambda s: amap.get(names.get(s, s))):
                funcs[names.get(res.symbol, res.symbol)] = {"match": res.matching, "score": round(res.score, 4)}
        n = len(funcs)
        matched = sum(f["match"] for f in funcs.values())
        mean = sum(f["score"] for f in funcs.values()) / n if n else 0
        results.append({"lua": ver, "config": cfg, "mapped": n, "matched": matched, "mean": round(mean, 4),
                        "functions": funcs})
        print(f"lua-{ver:6} {cfg:28} {matched:3}/{n:<3} exact  {mean:.1%}", flush=True)

    out = ROOT / "build" / "libcheck" / "lua" / args.version / "results.json"
    out.parent.mkdir(parents=True, exist_ok=True)
    out.write_text(json.dumps(results, indent=1), encoding="utf-8")
    return 0


if __name__ == "__main__":
    sys.exit(main())
