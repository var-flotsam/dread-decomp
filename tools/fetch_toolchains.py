#!/usr/bin/env python3
"""Download candidate upstream LLVM/Clang releases for compiler identification.

These are open-source LLVM releases, not Nintendo SDK files. Each Windows
installer is downloaded once and extracted with 7-Zip (nothing is installed
and PATH is untouched) into %LOCALAPPDATA%\\dread-decomp\\toolchains\\llvm-<ver>\\.

SHA-256 of each installer is pinned in data/toolchains.json. The first
download records the hash; later downloads must match it.

Usage: python tools/fetch_toolchains.py [VERSION ...]   (default: all in CANDIDATES)
"""

from __future__ import annotations

import hashlib
import json
import os
import subprocess
import sys
import urllib.request
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
PINS = ROOT / "data" / "toolchains.json"
DEST = Path(os.environ.get("LOCALAPPDATA", Path.home())) / "dread-decomp" / "toolchains"
SEVEN_ZIP = r"C:\Program Files\7-Zip\7z.exe"

CANDIDATES = {
    "4.0.1": "https://releases.llvm.org/4.0.1/LLVM-4.0.1-win64.exe",
    "8.0.1": "https://github.com/llvm/llvm-project/releases/download/llvmorg-8.0.1/LLVM-8.0.1-win64.exe",
    "9.0.0": "https://releases.llvm.org/9.0.0/LLVM-9.0.0-win64.exe",
    "9.0.1": "https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/LLVM-9.0.1-win64.exe",
    "10.0.0": "https://github.com/llvm/llvm-project/releases/download/llvmorg-10.0.0/LLVM-10.0.0-win64.exe",
    "11.1.0": "https://github.com/llvm/llvm-project/releases/download/llvmorg-11.1.0/LLVM-11.1.0-win64.exe",
    "12.0.1": "https://github.com/llvm/llvm-project/releases/download/llvmorg-12.0.1/LLVM-12.0.1-win64.exe",
}


def clang_path(version: str) -> Path:
    return DEST / f"llvm-{version}" / "bin" / "clang++.exe"


def fetch(version: str, pins: dict) -> None:
    target = DEST / f"llvm-{version}"
    if clang_path(version).is_file():
        print(f"{version}: already present at {target}")
        return
    url = CANDIDATES[version]
    DEST.mkdir(parents=True, exist_ok=True)
    installer = DEST / url.rsplit("/", 1)[1]
    print(f"{version}: downloading {url}", flush=True)
    urllib.request.urlretrieve(url, installer)
    digest = hashlib.sha256(installer.read_bytes()).hexdigest()
    pinned = pins.get(version, {}).get("sha256")
    if pinned and pinned != digest:
        installer.unlink()
        raise SystemExit(f"{version}: sha256 {digest} does not match pinned {pinned}")
    pins[version] = {"url": url, "sha256": digest}
    subprocess.run([SEVEN_ZIP, "x", "-y", f"-o{target}", str(installer)], check=True,
                   stdout=subprocess.DEVNULL)
    installer.unlink()
    if not clang_path(version).is_file():
        raise SystemExit(f"{version}: clang++.exe not found after extraction in {target}")
    print(f"{version}: extracted to {target} (sha256 {digest})")


def main() -> int:
    versions = sys.argv[1:] or list(CANDIDATES)
    pins = json.loads(PINS.read_text()) if PINS.is_file() else {}
    try:
        for v in versions:
            fetch(v, pins)
    finally:
        PINS.write_text(json.dumps(pins, indent=2, sort_keys=True) + "\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
