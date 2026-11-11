#!/usr/bin/env python3
"""Coordinator helper: brief <bid> | start <bid> | merge <bid> | stats | next <n>."""
import json
import sys
from pathlib import Path

OUT = Path(__file__).resolve().parent
LED = OUT / "ledger.json"
REPO = "<repos>\\dread-decomp-overnight"


def load():
    return json.loads(LED.read_text(encoding="utf-8"))


def save(L):
    LED.write_text(json.dumps(L, indent=1), encoding="utf-8")


def find(L, bid):
    return next(b for b in L["batches"] if b["id"] == bid)


def brief(L, b):
    units = []
    for u in b["units"]:
        m = L["meta"][u]
        if m["dir"]:
            units.append(f"unit {u} ({m['hint']}) -> src/{m['dir']}/{m['stem']}_<what>_ov{b['id']}.cpp")
        else:
            units.append(f"unit {u} (no reliable name; hint '{m['hint']}') -> src/game/UnkTU{u}/UnkTU{u}_<what>_ov{b['id']}.cpp")
    extra = ""
    if b.get("opus"):
        extra = ("These are near-misses: each function already has a best draft (90%+, not matching) at "
                 "build/overnight/drafts/<addr>.cpp (some drafts are whole scratch files holding several functions; use "
                 "the part for your address and ignore the rest). Start from the draft, find the remaining difference with "
                 "try.py and fix it. At most 2 attempts per function.\n")
    return extra + f"""Decompile these status-U functions of dread-decomp into hand-written C++ that compiles byte-identically
with the project's Clang 9 (CLAUDE.md's conventions and hard rules apply). Don't read docs/ or browse src/ beyond one matched sibling file;
the brief, build/overnight/tips.md and the ctx file are enough.
IMPORTANT: work ONLY in the worktree {REPO} (cd there for every command; it is on branch
claude/overnight-decomp-part3). Never touch <repos>\\dread-decomp or any other checkout.
IMPORTANT: never guess a string literal (the ctx file may show only a prefix); if a function needs one you cannot read in full, leave that function out.
IMPORTANT: attempt EVERY function in the batch (2 attempts each). There is no overall time or token budget to stop at: do not report until every function has an outcome.
Batch {b["id"]}: {', '.join('fn_' + a for a in b['addrs'])}.
Translation unit(s) and the file each unit's functions go in (one file per unit; <what> is a short readable word):
  """ + "\n  ".join(units) + f"""
Context per function: build/overnight/ctx/<addr>.txt (annotated disassembly, existing header declarations; the
unit for each function is on its first line). Conventions and three matched examples: build/overnight/system_prompt.txt.
For each function: write the source, grade it with
  .venv/Scripts/python build/overnight/try.py <file>
(short diff: differing lines only) and revise from it. At most 2 attempts per function; don't stop early on a low
first score (one extra instruction near the top can make a near-match score under 50%). When a function ends at 90%
or better but not matching, save its best draft to build/overnight/drafts/<addr>.cpp (not under src/), record
best_similarity in the result, and do not retry it in this batch.
Re-run `.venv/Scripts/python tools/trymatch.py --version 2.1.0 <file> --obj-dir build/overnight/obj/{b['id']}` only as the final re-check of each src file. Tip: draft each
function first in a scratch file under build/overnight/scratch/{b['id']}/ and only add it to the src file once it
matches; re-run trymatch on the final src file at the end to confirm every function in it still MATCHES.
Files: write only NEW files as named above (one file may hold several functions of the unit). Never edit existing
files in src/, include/, data/, tools/ or docs/.
Keep in the file only functions that MATCH with no RELOC line; delete a function that does not match (never
leave NON_MATCHING code from this run). If no function in a file matches, delete the file.
Declarations: before declaring a callee or global locally, git grep for an existing declaration of it; if
include/ declares it, include that header; if another src file declares it, copy that exact signature (a
different signature fails decl_check; check with `.venv/Scripts/python tools/decl_check.py --symbol <name>`,
which is allowed and cheap). The signature must match textually, parameter type names included: when the existing
one uses a struct defined in another .cpp, forward-declare that struct name (`struct UnkFoo_b42;`) and cast to it
at the call site instead of inventing a new type. If the function you are DEFINING is declared elsewhere with a void return but the original returns a value, define it
with the correct return type anyway. A function you DEFINE must take exactly the parameter types of any existing
declaration of it (git grep fn_<addr>; declfile does not check definitions): use those types and cast inside (the C++ mangled name ignores the return type, so it still links) and note it in
the result's blocker field; parameter lists must still agree. Never redeclare a symbol that a header in include/ declares:
include the header. Use matched files in src/ as examples (tools/nearcopy.py may point at
a near copy). Read build/overnight/tips.md first (tips from earlier batches). Main tip: when the original reloads a field after a store that Clang's type-based alias analysis
lets ours keep in a register, write that store through an untyped path (__builtin_memcpy/memset or a plain char
pointer); this closed several near-misses. Write readable code with structs/named fields; no copied disassembly and no inline asm with instructions in it. Exception (CLAUDE.md): empty barriers `asm("" : "+r"(x))` and
`asm volatile("" ::: "memory")` are allowed when nothing else matches, each with a one-line comment directly above
it saying what ordering it keeps.
Do not run check.py, verify.py, decl_check.py (except --symbol), any tools/gen_*.py, port builds, Ghidra, or git commands (git grep
is fine). Do not copy disassembly or decompiler output into any file under src/ or include/.
Before finishing, run `.venv/Scripts/python build/overnight/declfile.py <each src file you wrote>` (a few seconds):
it lists every declaration in your files whose signature is not one already used elsewhere, or that include/
already declares. Fix every problem it lists (copy the listed signature exactly, forward-declaring its struct names
and casting at the call site, or include the header), re-run trymatch on the file, and repeat until it reports 0
problems. A function you cannot keep matching that way must be removed.
Finish by writing build/overnight/results/{b['id']}.json: a JSON list with one entry per function
{{"addr": "71...", "outcome": "match"|"differs"|"skipped", "best_similarity": <0-100>, "file": "src/..." or null,
"blocker": "<few words, for non-matches>"}}, and reply with one line of counts (match/differs/skipped).
"""


def main():
    L = load()
    cmd = sys.argv[1]
    if cmd == "brief":
        print(brief(L, find(L, sys.argv[2])))
    elif cmd == "start":
        find(L, sys.argv[2])["status"] = "running"
        save(L)
    elif cmd == "next":
        n = int(sys.argv[2])
        print(" ".join(b["id"] for b in L["batches"] if b["status"] == "queued")[: n * 6])
    elif cmd == "merge":
        b = find(L, sys.argv[2])
        p = OUT / "results" / f"{b['id']}.json"
        res = json.loads(p.read_text(encoding="utf-8")) if p.exists() else []
        for r in res:
            b["results"][str(r.get("addr", "")).replace("0x", "").replace("fn_", "")] = r
        b["status"] = "done"
        save(L)
        files = sorted({r["file"] for r in res if r.get("outcome") == "match" and r.get("file")})
        m = sum(r.get("outcome") == "match" for r in res)
        print(json.dumps({"matched": m, "total": len(b["addrs"]), "files": files,
                          "units": [L["meta"][u]["stem"] or f"UnkTU{u}" for u in b["units"]]}))
    elif cmd == "stats":
        st = {}
        for b in L["batches"]:
            st[b["status"]] = st.get(b["status"], 0) + 1
        res = [r for b in L["batches"] for r in b["results"].values()]
        print(st, "attempted", len(res), "matched", sum(r.get("outcome") == "match" for r in res))


if __name__ == "__main__":
    main()
