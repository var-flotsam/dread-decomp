"""Repository locations the tools share. Machine-specific locations stay in tools/hostenv.py."""

from __future__ import annotations

from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent.parent
DATA = ROOT / "data"
FUNCTIONS = DATA / "functions"
SRC = ROOT / "src"
INCLUDE = ROOT / "include"
BUILD = ROOT / "build"


def orig(version: str) -> Path:
    """orig/<version>/: the user's dump as tools/setup.py lays it out (never tracked)."""
    return ROOT / "orig" / version
