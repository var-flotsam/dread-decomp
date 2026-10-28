"""One entry point for the tools: `python -m tools <tool> [args...]` from the repository root.

`<tool>` is a script's name without `.py` (`check`, `port_link`, `ghidra/decompile`; `-` works for
`_` and `.` for `/`). With no tool, prints the tools by group (the same groups as docs/commands.md).
The script runs under the repository's venv interpreter when there is one (`.venv/Scripts/python`
on Windows, `.venv/bin/python` elsewhere), so `python -m tools ...` works from any Python 3.

Usage: python -m tools                         # list the tools
       python -m tools check --version 2.1.0  # same as .venv/.../python tools/check.py --version 2.1.0
       python -m tools ghidra.decompile --version 2.1.0 nnMain
"""

from __future__ import annotations

import os
import subprocess
import sys
from pathlib import Path

TOOLS = Path(__file__).resolve().parent
ROOT = TOOLS.parent


def venv_python() -> Path | None:
    for p in (ROOT / ".venv" / "Scripts" / "python.exe", ROOT / ".venv" / "bin" / "python"):
        if p.is_file():
            return p
    return None


def listing() -> str:
    sys.path.insert(0, str(TOOLS))
    import gen_commands  # noqa: E402
    found = gen_commands.tools()
    lines = ["usage: python -m tools <tool> [args...]   (docs/commands.md has each tool's usage)", ""]
    listed = set()
    for title, names in gen_commands.GROUPS:
        present = [n for n in names if n in found]
        listed |= set(present)
        lines.append(f"{title}: {', '.join(present)}")
    rest = sorted(set(found) - listed)
    if rest:
        lines.append(f"Other: {', '.join(rest)}")
    return "\n".join(lines)


def main() -> int:
    if len(sys.argv) < 2 or sys.argv[1] in ("-h", "--help"):
        print(listing())
        return 0
    name = sys.argv[1].replace("-", "_").replace(".", "/")
    script = TOOLS / f"{name}.py"
    if not script.is_file():
        print(f"no tool {sys.argv[1]!r}\n\n{listing()}", file=sys.stderr)
        return 2
    py = venv_python()
    interpreter = str(py) if py and Path(sys.executable).resolve() != py.resolve() else sys.executable
    env = dict(os.environ, PYTHONIOENCODING=os.environ.get("PYTHONIOENCODING", "utf-8"))
    return subprocess.run([interpreter, str(script), *sys.argv[2:]], env=env).returncode


if __name__ == "__main__":
    raise SystemExit(main())
