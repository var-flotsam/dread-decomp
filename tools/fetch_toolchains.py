#!/usr/bin/env python3
"""Download candidate upstream LLVM/Clang releases for compiler identification.

These are open-source LLVM releases, not Nintendo SDK files. Each Windows
installer is downloaded once and extracted with 7-Zip (nothing is installed
and PATH is untouched) into %LOCALAPPDATA%\\dread-decomp\\toolchains\\llvm-<ver>\\.

SHA-256 of each installer is pinned in data/toolchains.json. The first
download records the hash; later downloads must match it.

Also fetches objdiff / objdiff-cli (TOOLS) into %LOCALAPPDATA%\\dread-decomp\\tools\\.

Usage: python tools/fetch_toolchains.py [VERSION or TOOL ...]   (default: everything)
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
    "6.0.1": "https://releases.llvm.org/6.0.1/LLVM-6.0.1-win64.exe",
    "7.0.1": "https://releases.llvm.org/7.0.1/LLVM-7.0.1-win64.exe",
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


# Single-executable tools (encounter/objdiff, Apache-2.0). SHA-256 values are the digests
# GitHub publishes for these release assets.
OBJDIFF_VERSION = "3.8.2"
TOOLS = {
    f"objdiff-cli-{OBJDIFF_VERSION}": (
        f"https://github.com/encounter/objdiff/releases/download/v{OBJDIFF_VERSION}/objdiff-cli-windows-x86_64.exe",
        "36d229ac6ce74a26b47f42cf86ea8e808d8cd834a54aa62d7ed3f47b765d971b"),
    f"objdiff-{OBJDIFF_VERSION}": (
        f"https://github.com/encounter/objdiff/releases/download/v{OBJDIFF_VERSION}/objdiff-windows-x86_64.exe",
        "ce9d877a8cd6f133a962fb86d66f0348b9b88d735cea0fd52ae61bb9ad4d6691"),
}


def tool_path(name: str) -> Path:
    return DEST.parent / "tools" / f"{name}.exe"


def fetch_tool(name: str, pins: dict) -> None:
    path = tool_path(name)
    if path.is_file():
        print(f"{name}: already present at {path}")
        return
    url, expected = TOOLS[name]
    print(f"{name}: downloading {url}", flush=True)
    data = urllib.request.urlopen(url, timeout=300).read()
    digest = hashlib.sha256(data).hexdigest()
    if digest != expected:
        raise SystemExit(f"{name}: sha256 {digest} does not match published {expected}")
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    pins[name] = {"url": url, "sha256": digest}
    print(f"{name}: saved to {path}")


def main() -> int:
    versions = sys.argv[1:] or [*CANDIDATES, *TOOLS]
    pins = json.loads(PINS.read_text()) if PINS.is_file() else {}
    try:
        for v in versions:
            if v in TOOLS:
                fetch_tool(v, pins)
            else:
                fetch(v, pins)
    finally:
        PINS.write_text(json.dumps(pins, indent=2, sort_keys=True) + "\n", newline="\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
