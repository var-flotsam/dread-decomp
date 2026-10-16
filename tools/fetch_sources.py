#!/usr/bin/env python3
"""Download upstream third-party sources used to identify and build library code
linked into the game, and the libc++ headers the build uses (open source only; see docs/references.md).

Each archive is downloaded once and extracted to
%LOCALAPPDATA%\\dread-decomp\\sources\\<name>\\. SHA-256 is pinned in data/sources.json:
the first download records it, and later downloads must match.

Usage: python tools/fetch_sources.py [NAME ...]   (default: all in SOURCES)
"""

from __future__ import annotations

import hashlib
import io
import json
import os
import sys
import tarfile
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PINS = ROOT / "data" / "sources.json"
DEST = Path(os.environ.get("LOCALAPPDATA", Path.home())) / "dread-decomp" / "sources"

# Lua 5.1 patch releases (MIT), from the official lua.org archive.
SOURCES = {f"lua-5.1.{n}": f"https://www.lua.org/ftp/lua-5.1.{n}.tar.gz" for n in range(0, 6)}
SOURCES["lua-5.1"] = "https://www.lua.org/ftp/lua-5.1.tar.gz"
del SOURCES["lua-5.1.0"]
# libc++ headers matching the project compiler (Apache-2.0 with LLVM exception).
SOURCES["libcxx-9.0.1.src"] = ("https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/"
                               "libcxx-9.0.1.src.tar.xz")


def source_dir(name: str) -> Path:
    return DEST / name


def fetch(name: str, pins: dict) -> None:
    target = source_dir(name)
    if target.is_dir():
        print(f"{name}: already present at {target}")
        return
    url = SOURCES[name]
    print(f"{name}: downloading {url}", flush=True)
    data = urllib.request.urlopen(url, timeout=120).read()
    digest = hashlib.sha256(data).hexdigest()
    pinned = pins.get(name, {}).get("sha256")
    if pinned and pinned != digest:
        raise SystemExit(f"{name}: sha256 {digest} does not match pinned {pinned}")
    pins[name] = {"url": url, "sha256": digest}
    DEST.mkdir(parents=True, exist_ok=True)
    with tarfile.open(fileobj=io.BytesIO(data), mode="r:*") as tar:
        tar.extractall(DEST, filter="data")
    if not target.is_dir():
        raise SystemExit(f"{name}: expected {target} after extraction")
    print(f"{name}: extracted to {target} (sha256 {digest})")


def main() -> int:
    names = sys.argv[1:] or list(SOURCES)
    pins = json.loads(PINS.read_text()) if PINS.is_file() else {}
    try:
        for n in names:
            fetch(n, pins)
    finally:
        PINS.write_text(json.dumps(pins, indent=2, sort_keys=True) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
