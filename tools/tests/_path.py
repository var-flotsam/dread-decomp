"""Puts tools/ on sys.path so the tests import the tools as the scripts import each other."""

import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent.parent
if str(TOOLS) not in sys.path:
    sys.path.insert(0, str(TOOLS))
