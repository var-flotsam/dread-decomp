"""Host-platform details shared by the tools: the download cache folder, executable names and
llvm-cxxfilt.

Windows keeps its original locations (%LOCALAPPDATA%\\dread-decomp, C:\\Program Files\\LLVM).
macOS uses ~/Library/Caches/dread-decomp; other systems $XDG_CACHE_HOME/dread-decomp (default
~/.cache/dread-decomp), or ~/dread-decomp where an older checkout already put its downloads.
The macOS path is untested on hardware (docs/reference.md, "macOS").
"""

from __future__ import annotations

import os
import shutil
import sys
from pathlib import Path

WINDOWS = sys.platform == "win32"
MACOS = sys.platform == "darwin"
PLATFORM = "windows" if WINDOWS else "macos" if MACOS else "linux"


def cache_dir() -> Path:
    """Folder for downloaded toolchains, tools and sources (outside the repo)."""
    if WINDOWS:
        return Path(os.environ.get("LOCALAPPDATA", Path.home())) / "dread-decomp"
    if MACOS:
        return Path.home() / "Library" / "Caches" / "dread-decomp"
    legacy = Path.home() / "dread-decomp"
    if legacy.is_dir():
        return legacy
    return Path(os.environ.get("XDG_CACHE_HOME") or Path.home() / ".cache") / "dread-decomp"


def exe(name: str) -> str:
    """An executable's file name on this platform."""
    return f"{name}.exe" if WINDOWS else name


_WIN_CXXFILT = r"C:\Program Files\LLVM\bin\llvm-cxxfilt.exe"
_BREW_LLVM = ("/opt/homebrew/opt/llvm/bin", "/usr/local/opt/llvm/bin")  # Homebrew's llvm is keg-only


def cxxfilt() -> str:
    """llvm-cxxfilt: the Windows LLVM install, PATH, Homebrew's llvm, then the project toolchain's own."""
    if WINDOWS and Path(_WIN_CXXFILT).is_file():
        return _WIN_CXXFILT
    found = shutil.which("llvm-cxxfilt")
    if found:
        return found
    for d in _BREW_LLVM:
        if (Path(d) / "llvm-cxxfilt").is_file():
            return str(Path(d) / "llvm-cxxfilt")
    for d in sorted((cache_dir() / "toolchains").glob("llvm-*"), reverse=True):
        if (d / "bin" / exe("llvm-cxxfilt")).is_file():
            return str(d / "bin" / exe("llvm-cxxfilt"))
    return _WIN_CXXFILT if WINDOWS else "llvm-cxxfilt"
