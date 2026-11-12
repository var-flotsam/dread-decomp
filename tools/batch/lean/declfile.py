#!/usr/bin/env python3
"""declfile.py <file.cpp>...: decl_check problems for the fn_/g_unk_ symbols these files declare or define.

Problems: a declaration whose signature is not one that another file already uses for that symbol
(copy one of the listed ones exactly), a symbol that a header in include/ declares (include it), a
declaration of a function that another file defines with other types (copy the definition's signature;
not reported when another file already declares it the same way, which a worker cannot settle), or
a definition here whose types are not those of a declaration elsewhere (use that parameter list). A
namespace written out on one side is the same type and is not a problem.
Exit 1 when there is a problem. One parse of the tree, a few seconds. The check itself is
tools/decl_check.py's `file_problems`.
"""
import os
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "tools"))
os.chdir(ROOT)
import decl_check  # noqa: E402


def main():
    files = {Path(a).resolve().relative_to(ROOT).as_posix() for a in sys.argv[1:]}
    found = decl_check.file_problems(files)
    for line in found:
        print(line)
    print(f"declfile: {len(found)} problems")
    return 1 if found else 0


if __name__ == "__main__":
    sys.exit(main())
