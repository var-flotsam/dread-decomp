#!/usr/bin/env python3
"""Build libvpx's VP9 decoder as a static library for the port's movie runtime.

The game's movies are VP9 in WebM (101 files, video only); port/runtime/src/movie.cpp decodes them
with libvpx. This configures and builds the pinned source (python tools/fetch_sources.py
libvpx-1.14.1) as a VP9-decoder-only, position-independent static library with libvpx's portable C
target (generic-gnu): no assembler is needed and every host builds the same code. Output:
<cache>/build/libvpx-1.14.1-<target>/ with libvpx.a and include/vpx/ (tools/hostenv.py cache_dir;
in the port's container that is build/cache). tools/port_link.py --nvn compiles the runtime with
those headers and links the library.

Needs a POSIX shell, make and perl, and a C compiler for the target: run it in the port's
container for Linux (port/docker/run.sh). --target x86_64-w64-mingw32 cross-compiles for Windows
with x86_64-w64-mingw32-clang on PATH (llvm-mingw).

Usage:
  port/docker/run.sh python3 tools/build_libvpx.py                         # x86_64-linux-gnu
  port/docker/run.sh python3 tools/build_libvpx.py --target x86_64-w64-mingw32
  python tools/build_libvpx.py --print-dir [--target T]                    # where the build goes
"""

from __future__ import annotations

import argparse
import os
import shutil
import subprocess
import sys
from pathlib import Path

import hostenv
from fetch_sources import source_dir

NAME = "libvpx-1.14.1"
TARGETS = {
    # target triple -> (CROSS prefix, C compiler)
    "x86_64-linux-gnu": ("", "clang"),
    "x86_64-w64-mingw32": ("x86_64-w64-mingw32-", "x86_64-w64-mingw32-clang"),
}
CONFIGURE = ["--target=generic-gnu", "--enable-static", "--disable-shared", "--enable-pic",
             "--disable-vp8", "--disable-vp9-encoder", "--enable-vp9-decoder", "--disable-examples",
             "--disable-tools", "--disable-docs", "--disable-unit-tests", "--disable-webm-io",
             "--disable-libyuv", "--enable-multithread", "--size-limit=4096x4096"]


def build_dir(target: str) -> Path:
    return hostenv.cache_dir() / "build" / f"{NAME}-{target}"


def library(target: str) -> Path | None:
    """libvpx.a for `target` when it has been built, else None."""
    lib = build_dir(target) / "libvpx.a"
    return lib if lib.is_file() else None


def include_dir(target: str) -> Path:
    return build_dir(target) / "include"


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    ap.add_argument("--target", default="x86_64-linux-gnu", choices=sorted(TARGETS))
    ap.add_argument("--print-dir", action="store_true", help="print the build directory and stop")
    ap.add_argument("--jobs", type=int, default=os.cpu_count() or 4)
    args = ap.parse_args()
    out = build_dir(args.target)
    if args.print_dir:
        print(out)
        return 0
    src = source_dir(NAME)
    if not (src / "configure").is_file():
        raise SystemExit(f"{NAME} is not fetched: python tools/fetch_sources.py {NAME}")
    for tool in ("sh", "make", "perl"):
        if not shutil.which(tool):
            raise SystemExit(f"build_libvpx: needs {tool} (run it in the port's container: port/docker/run.sh)")
    cross, cc = TARGETS[args.target]
    if not shutil.which(cc):
        raise SystemExit(f"build_libvpx: no {cc} on PATH for {args.target}")
    work = out / "work"
    if work.exists():
        shutil.rmtree(work)
    work.mkdir(parents=True)
    # LD too: libvpx's generic target links its configure checks with gcc by default.
    env = dict(os.environ, CROSS=cross, CC=cc, LD=cc, AR=f"{cross}ar" if cross else "ar",
               CFLAGS="-O2 -fPIC -fvisibility=hidden")
    if cross and not shutil.which(env["AR"]):
        env["AR"] = "llvm-ar"
    steps = [["sh", str(src / "configure"), *CONFIGURE], ["make", f"-j{args.jobs}", "libvpx.a"]]
    for cmd in steps:
        print("build_libvpx:", " ".join(cmd), flush=True)
        r = subprocess.run(cmd, cwd=work, env=env)
        if r.returncode != 0:
            raise SystemExit(f"build_libvpx: {cmd[0]} {cmd[1]} failed ({r.returncode})")
    shutil.copy2(work / "libvpx.a", out / "libvpx.a")
    inc = include_dir(args.target) / "vpx"
    if inc.exists():
        shutil.rmtree(inc)
    inc.mkdir(parents=True)
    for h in ("vpx_decoder.h", "vp8dx.h", "vp8.h", "vpx_codec.h", "vpx_frame_buffer.h", "vpx_image.h",
              "vpx_integer.h", "vpx_ext_ratectrl.h", "vpx_tpl.h"):
        if (src / "vpx" / h).is_file():
            shutil.copy2(src / "vpx" / h, inc / h)
    shutil.rmtree(work)
    print(f"build_libvpx: {out / 'libvpx.a'} ({(out / 'libvpx.a').stat().st_size // 1024} KB), headers in {inc.parent}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
