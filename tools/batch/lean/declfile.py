#!/usr/bin/env python3
"""declfile.py <file.cpp>...: decl_check problems for the fn_/g_unk_ symbols these files declare or define.

Problems: a declaration whose signature is not one that another file already uses for that symbol
(copy one of the listed ones exactly), a symbol that a header in include/ declares (include it), a
declaration of a function that another file defines with other types (copy the definition's signature;
not reported when another file already declares it the same way, which a worker cannot settle), or
a definition here whose types are not those of a declaration elsewhere (use that parameter list). A
namespace written out on one side is the same type and is not a problem.
Exit 1 when there is a problem. One parse of the tree, a few seconds.
"""
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools"))
import os  # noqa: E402

os.chdir(ROOT)
import decl_check  # noqa: E402


def bare(signature):
    return decl_check.QUALIFIER.sub("", signature)


def main():
    files = {Path(a).as_posix() for a in sys.argv[1:]}
    tree = decl_check.tree_files(None)
    r = decl_check.Report(tree)
    defined = {}
    for path, text in tree.items():
        if not path.startswith("include/") and "fn_" in text:
            for d in decl_check.definitions(text, path):
                defined[d.name] = d
    bad = 0
    for name, ds in sorted(r.decls.items()):
        mine = [d for d in ds if d.path in files]
        if not mine:
            continue
        others = [d for d in ds if d.path not in files]
        other_sigs = {d.signature for d in others}
        inc = [d for d in others if d.path.startswith("include/")]
        definition = defined.get(name)
        for d in mine:
            why = None
            if inc:
                why = f"declared in {inc[0].path}: include that header and drop this declaration"
            elif (definition and definition.path not in files and bare(definition.signature) != bare(d.signature)
                  and d.signature not in other_sigs):  # a difference other files already have is not this file's
                why = (f"{definition.path} defines it with other types; copy the definition's signature exactly:\n"
                       f"    {definition.signature}")
            elif other_sigs and d.signature not in other_sigs:
                why = "signature differs from every existing one; copy one of these exactly:\n    " + \
                      "\n    ".join(f"{s}   ({next(o.path for o in others if o.signature == s)})" for s in sorted(other_sigs))
            if why:
                bad += 1
                print(f"{d.path}:{d.line} {name}: {d.signature}\n  -> {why}")
    for name, d in sorted(defined.items()):
        if d.path not in files:
            continue
        sigs = sorted({o.signature for o in r.decls.get(name, []) if o.path != d.path})
        if sigs and all(bare(s) != bare(d.signature) for s in sigs):
            bad += 1
            print(f"{d.path}:{d.line} {name}: defined as {d.signature}\n  -> declared elsewhere with other types; "
                  f"use this parameter list and result:\n    {sigs[0]}")
    print(f"declfile: {bad} problems")
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
