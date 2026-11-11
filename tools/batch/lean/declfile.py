#!/usr/bin/env python3
"""declfile.py <file.cpp>...: decl_check problems for the fn_/g_unk_ symbols these files declare.

Problems: a declaration whose signature is not one that another file already uses for that symbol
(copy one of the listed ones exactly), or a symbol that a header in include/ declares (include it).
Exit 1 when there is a problem. One parse of the tree, a few seconds.
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import os  # noqa: E402

os.chdir(ROOT)
import decl_check  # noqa: E402


def main():
    files = {Path(a).as_posix() for a in sys.argv[1:]}
    r = decl_check.Report(decl_check.tree_files(None))
    bad = 0
    for name, ds in sorted(r.decls.items()):
        mine = [d for d in ds if d.path in files]
        if not mine:
            continue
        others = [d for d in ds if d.path not in files]
        other_sigs = {d.signature for d in others}
        inc = [d for d in others if d.path.startswith("include/")]
        for d in mine:
            why = None
            if inc:
                why = f"declared in {inc[0].path}: include that header and drop this declaration"
            elif other_sigs and d.signature not in other_sigs:
                why = "signature differs from every existing one; copy one of these exactly:\n    " + \
                      "\n    ".join(f"{s}   ({next(o.path for o in others if o.signature == s)})" for s in sorted(other_sigs))
            if why:
                bad += 1
                print(f"{d.path}:{d.line} {name}: {d.signature}\n  -> {why}")
    print(f"declfile: {bad} problems")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
