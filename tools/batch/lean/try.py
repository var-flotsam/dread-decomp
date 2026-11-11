#!/usr/bin/env python3
"""try.py <file.cpp> [--full]: tools/trymatch.py with short output, for agents.

Prints each function's summary line (MATCH / similarity / differing insns), its RELOC lines, and only
the differing instructions with one line of context, at most 30 diff lines per function (--full: all).
A build failure prints the first 20 error lines. Objects go under build/lean/obj/<file stem>/ so
parallel agents never share one. Exit status as trymatch: 0 all match, 1 differs, 2 build failure.
"""
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
CAP = 30


def main():
    args = [x for x in sys.argv[1:] if x != "--full"]
    full = "--full" in sys.argv
    src = args[0]
    py = ROOT / ".venv" / "Scripts" / "python.exe"
    if not py.exists():
        py = Path(sys.executable)
    p = subprocess.run([str(py), str(ROOT / "tools" / "trymatch.py"), "--version", "2.1.0", src, "-v",
                        "--obj-dir", str(ROOT / "build" / "lean" / "obj" / Path(src).stem), *args[1:]],
                       cwd=ROOT, capture_output=True, text=True)
    lines = (p.stdout + p.stderr).splitlines()
    if p.returncode == 2:
        errs = [l for l in lines if "error" in l.lower()] or lines
        print("\n".join(errs[:20]))
        if len(errs) > 20:
            print(f"... {len(errs) - 20} more error lines")
        return 2
    out, block = [], []

    def flush():
        if not block:
            return
        keep = set()
        for i, l in enumerate(block):
            if l.lstrip().startswith("!!"):
                keep.update((i - 1, i, i + 1))
        shown, last = 0, -2
        for i in sorted(k for k in keep if 0 <= k < len(block)):
            if not full and shown >= CAP:
                out.append(f"      ... {sum(1 for l in block if l.lstrip().startswith('!!'))} differing in all; "
                           "fix the first ones, then rerun")
                break
            if i != last + 1:
                out.append("      ...")
            out.append(block[i])
            shown += 1
            last = i
        block.clear()

    for l in lines:
        if l.startswith("  ") and "RELOC" not in l and "KIND" not in l:
            block.append(l)
        else:
            flush()
            out.append(l)
    flush()
    print("\n".join(out))
    return p.returncode


if __name__ == "__main__":
    sys.exit(main())
