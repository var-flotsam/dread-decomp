"""Shared PyGhidra setup for the tools/ghidra scripts.

Ghidra runs headless in-process via PyGhidra. Projects live in build/ghidra/<version>/
and exports in build/ghidra_export/<version>/. Both are gitignored.
"""

from __future__ import annotations

import glob
import os
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
IMAGE_BASE = 0x7100000000
PROJECT_NAME = "dread"


def find_ghidra() -> Path:
    if "GHIDRA_INSTALL_DIR" in os.environ:
        return Path(os.environ["GHIDRA_INSTALL_DIR"])
    hits = sorted(glob.glob(os.path.expandvars(r"%LOCALAPPDATA%\Programs\ghidra_*_PUBLIC")))
    if not hits:
        raise SystemExit("Ghidra not found: set GHIDRA_INSTALL_DIR")
    return Path(hits[-1])


def find_jdk() -> None:
    if "JAVA_HOME" in os.environ:
        return
    hits = sorted(glob.glob(r"C:\Program Files\Eclipse Adoptium\jdk-21*"))
    if hits:
        os.environ["JAVA_HOME"] = hits[-1]


def start(max_heap: str = "16g"):
    """Start the JVM with Ghidra loaded. Safe to call more than once."""
    import pyghidra
    if pyghidra.started():
        return
    find_jdk()
    from pyghidra.launcher import HeadlessPyGhidraLauncher
    launcher = HeadlessPyGhidraLauncher(install_dir=find_ghidra())
    launcher.add_vmargs(f"-Xmx{max_heap}")
    launcher.start()


def project_dir(version: str) -> Path:
    return ROOT / "build" / "ghidra" / version


def export_dir(version: str) -> Path:
    d = ROOT / "build" / "ghidra_export" / version
    d.mkdir(parents=True, exist_ok=True)
    return d


def open_project(version: str, create: bool = False):
    import pyghidra
    d = project_dir(version)
    d.mkdir(parents=True, exist_ok=True)
    return pyghidra.open_project(str(d), PROJECT_NAME, create=create)
