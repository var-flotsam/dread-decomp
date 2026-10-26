#!/usr/bin/env python3
"""Download candidate upstream LLVM/Clang releases for compiler identification.

These are open-source LLVM releases, not Nintendo SDK files. Each one is downloaded once and
extracted (nothing is installed and PATH is untouched) into <cache>/toolchains/llvm-<ver>/,
where <cache> comes from tools/hostenv.py (%LOCALAPPDATA%\\dread-decomp on Windows,
~/Library/Caches/dread-decomp on macOS).

  Windows: the win64 installers, extracted with 7-Zip; every candidate version.
  macOS:   only the project compiler, from the official x86_64 tarball (there is no arm64
           build of LLVM 9); on Apple Silicon it runs under Rosetta 2. Extracted with tarfile.
           Untested on hardware: check.py must reproduce the PC's O count before it is used.

SHA-256 of each download is pinned in data/toolchains.json (macOS entries carry a `-macos`
suffix). The first download records the hash; later downloads must match it.

Also fetches objdiff / objdiff-cli (TOOLS) into <cache>/tools/, and glslang (ARCHIVE_TOOLS, the
Khronos GLSL compiler for tools/bshdat.py --compile) into <cache>/tools/glslang-<ver>/bin/.

Usage: python tools/fetch_toolchains.py [VERSION or TOOL ...]   (default: everything)
       python tools/fetch_toolchains.py glslang-16.6.0               (glslang only)
"""

from __future__ import annotations

import hashlib
import io
import json
import shutil
import subprocess
import sys
import tarfile
import urllib.request
import zipfile
from pathlib import Path

import hostenv

ROOT = Path(__file__).resolve().parent.parent
PINS = ROOT / "data" / "toolchains.json"
DEST = hostenv.cache_dir() / "toolchains"
SEVEN_ZIP = r"C:\Program Files\7-Zip\7z.exe"

WINDOWS_CANDIDATES = {
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
MACOS_CANDIDATES = {
    "9.0.1": "https://github.com/llvm/llvm-project/releases/download/llvmorg-9.0.1/"
             "clang+llvm-9.0.1-x86_64-apple-darwin.tar.xz",
}
CANDIDATES = WINDOWS_CANDIDATES if hostenv.WINDOWS else MACOS_CANDIDATES if hostenv.MACOS else {}


def pin_key(name: str) -> str:
    """The data/toolchains.json key: the bare name on Windows, `<name>-<platform>` elsewhere."""
    return name if hostenv.WINDOWS else f"{name}-{hostenv.PLATFORM}"


def clang_path(version: str) -> Path:
    return DEST / f"llvm-{version}" / "bin" / hostenv.exe("clang++")


def extract_tarball(archive: Path, target: Path) -> None:
    """Unpack an LLVM release tarball (one top-level folder) as target."""
    tmp = target.with_name(target.name + ".tmp")
    if tmp.exists():
        shutil.rmtree(tmp)
    with tarfile.open(archive, mode="r:*") as tar:
        tar.extractall(tmp, filter="data")
    tops = list(tmp.iterdir())
    if len(tops) != 1 or not tops[0].is_dir():
        raise SystemExit(f"{archive.name}: expected one top-level folder, found {len(tops)} entries")
    tops[0].rename(target)
    tmp.rmdir()


def fetch(version: str, pins: dict) -> None:
    target = DEST / f"llvm-{version}"
    if clang_path(version).is_file():
        print(f"{version}: already present at {target}")
        return
    if version not in CANDIDATES:
        raise SystemExit(f"{version}: no LLVM build listed for {hostenv.PLATFORM} "
                         f"(listed: {', '.join(CANDIDATES) or 'none'})")
    url = CANDIDATES[version]
    DEST.mkdir(parents=True, exist_ok=True)
    installer = DEST / url.rsplit("/", 1)[1]
    print(f"{version}: downloading {url}", flush=True)
    urllib.request.urlretrieve(url, installer)
    digest = hashlib.sha256(installer.read_bytes()).hexdigest()
    key = pin_key(version)
    pinned = pins.get(key, {}).get("sha256")
    if pinned and pinned != digest:
        installer.unlink()
        raise SystemExit(f"{version}: sha256 {digest} does not match pinned {pinned}")
    pins[key] = {"url": url, "sha256": digest}
    if hostenv.WINDOWS:
        subprocess.run([SEVEN_ZIP, "x", "-y", f"-o{target}", str(installer)], check=True,
                       stdout=subprocess.DEVNULL)
    else:
        extract_tarball(installer, target)
    installer.unlink()
    if not clang_path(version).is_file():
        raise SystemExit(f"{version}: {clang_path(version).name} not found after extraction in {target}")
    print(f"{version}: extracted to {target} (sha256 {digest})")


# Single-executable tools (encounter/objdiff, Apache-2.0). SHA-256 values are the digests
# GitHub publishes for these release assets.
OBJDIFF_VERSION = "3.8.2"
_OBJDIFF = f"https://github.com/encounter/objdiff/releases/download/v{OBJDIFF_VERSION}"
WINDOWS_TOOLS = {
    f"objdiff-cli-{OBJDIFF_VERSION}": (
        f"{_OBJDIFF}/objdiff-cli-windows-x86_64.exe",
        "36d229ac6ce74a26b47f42cf86ea8e808d8cd834a54aa62d7ed3f47b765d971b"),
    f"objdiff-{OBJDIFF_VERSION}": (
        f"{_OBJDIFF}/objdiff-windows-x86_64.exe",
        "ce9d877a8cd6f133a962fb86d66f0348b9b88d735cea0fd52ae61bb9ad4d6691"),
}
MACOS_TOOLS = {
    f"objdiff-cli-{OBJDIFF_VERSION}": (
        f"{_OBJDIFF}/objdiff-cli-macos-arm64",
        "767c67e13b7b5e6ca545e7d2235e47c7bceac07d47a52776f74a52914cb4085a"),
    f"objdiff-{OBJDIFF_VERSION}": (
        f"{_OBJDIFF}/objdiff-macos-arm64",
        "0b60fb1586f2d6efa880730a816c433327a75bb279d3411a6bf7df9dc1e1f169"),
}
TOOLS = WINDOWS_TOOLS if hostenv.WINDOWS else MACOS_TOOLS if hostenv.MACOS else {}

# Tools shipped as archives: only their bin/ folder is kept, in <cache>/tools/<name>/bin/.
# glslang (KhronosGroup/glslang, BSD-3-Clause and others), the reference GLSL compiler, used by
# tools/bshdat.py --compile. SHA-256 values are the digests GitHub publishes for the assets.
# Only the Windows zip has been fetched so far; the macOS and Linux tarballs are untested.
GLSLANG_VERSION = "16.6.0"
_GLSLANG = f"https://github.com/KhronosGroup/glslang/releases/download/{GLSLANG_VERSION}/glslang-{GLSLANG_VERSION}"
ARCHIVE_TOOLS = {
    f"glslang-{GLSLANG_VERSION}": {
        "windows": (f"{_GLSLANG}-windows-x86_64-release.zip",
                    "82bf434e69b9bb4829de7e2b4bc2c5e7a7861e53d66cf75e5cc70f5f694a8d9b"),
        "macos": (f"{_GLSLANG}-macos-universal-release.tar.gz",
                  "4f8c05f2888a73ed0b7b274bc524af9130e09ae94b43c0f3f9ed83cfae852834"),
        "linux": (f"{_GLSLANG}-linux-x86_64-release.tar.gz",
                  "2c34071f56ecf39d16233294d3739cf43f981fd39080f2fd9dccb782101191ce"),
    },
}


def tool_path(name: str) -> Path:
    return DEST.parent / "tools" / hostenv.exe(name)


def fetch_archive_tool(name: str, pins: dict) -> None:
    target = DEST.parent / "tools" / name
    if (target / "bin").is_dir() and any((target / "bin").iterdir()):
        print(f"{name}: already present at {target}")
        return
    url, expected = ARCHIVE_TOOLS[name][hostenv.PLATFORM]
    print(f"{name}: downloading {url}", flush=True)
    data = urllib.request.urlopen(url, timeout=300).read()
    digest = hashlib.sha256(data).hexdigest()
    if digest != expected:
        raise SystemExit(f"{name}: sha256 {digest} does not match published {expected}")
    tmp = target.with_name(target.name + ".tmp")
    if tmp.exists():
        shutil.rmtree(tmp)
    (tmp / "bin").mkdir(parents=True)
    if url.endswith(".zip"):
        with zipfile.ZipFile(io.BytesIO(data)) as z:
            members = [m for m in z.infolist() if m.filename.startswith("bin/") and not m.is_dir()]
            for m in members:
                (tmp / "bin" / Path(m.filename).name).write_bytes(z.read(m))
    else:
        with tarfile.open(fileobj=io.BytesIO(data), mode="r:*") as tar:
            members = [m for m in tar.getmembers() if "/bin/" in f"/{m.name}" and (m.isfile() or m.issym())]
            for m in members:
                if m.issym():
                    (tmp / "bin" / Path(m.name).name).symlink_to(Path(m.linkname).name)
                else:
                    out = tmp / "bin" / Path(m.name).name
                    out.write_bytes(tar.extractfile(m).read())
                    out.chmod(0o755)
    if not members:
        raise SystemExit(f"{name}: no bin/ folder in {url}")
    if target.exists():
        shutil.rmtree(target)
    tmp.rename(target)
    pins[pin_key(name)] = {"url": url, "sha256": digest}
    print(f"{name}: {len(members)} files saved to {target / 'bin'}")


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
    if not hostenv.WINDOWS:
        path.chmod(0o755)
    pins[pin_key(name)] = {"url": url, "sha256": digest}
    print(f"{name}: saved to {path}")


def main() -> int:
    versions = sys.argv[1:] or [*CANDIDATES, *TOOLS, *ARCHIVE_TOOLS]
    pins = json.loads(PINS.read_text()) if PINS.is_file() else {}
    try:
        for v in versions:
            if v in TOOLS:
                fetch_tool(v, pins)
            elif v in ARCHIVE_TOOLS:
                fetch_archive_tool(v, pins)
            else:
                fetch(v, pins)
    finally:
        PINS.write_text(json.dumps(pins, indent=2, sort_keys=True) + "\n", newline="\n")
    return 0


if __name__ == "__main__":
    sys.exit(main())
