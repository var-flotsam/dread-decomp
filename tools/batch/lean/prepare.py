#!/usr/bin/env python3
"""Lean run: work queue and per-function context (run once in the run's worktree, from origin/main).

  prepare.py --sample 50            stratified test sample (seeded)
  prepare.py --all [--max 2000]     every remaining status-U function from 24 B up to --max bytes
  prepare.py --addrs FILE [--model sonnet --budget 3]
                                    exactly the addresses in FILE (one hex address per line); the options
                                    go to build/lean/config.json, which coord.py follows for every job
  prepare.py --drafts DIR [DIR...]  the still-U functions that have a draft DIR/<addr>.cpp or DIR/fn_<addr>.cpp;
                                    each draft is copied to build/lean/drafts/ and the worker starts from it
  --effort high                     the Agent effort for every job (config.json)
  --ghidra                          save Ghidra's view of each function to build/lean/ghidra/<addr>.txt (read-only
                                    reference for the worker; build/ is never committed). Sequential: Ghidra's
                                    project admits one process, so workers must not run tools/ghidra themselves
  --prior GLOB                      results/*.json files of an earlier run: its best score and blocker go into
                                    the brief of a function it did not match
  --refs N                          reference matches per function (default 3; 0: none): the hand-written functions
                                    that already match and look most like it, with their source, at the end of its
                                    context (similar.py)

Writes build/lean/queue.json (one entry per function: addr, size, tier, target file, prior attempt) and
build/lean/ctx/<addr>.txt (annotated disassembly + headers that already declare its symbols + reference matches). The kit runs
in place from tools/batch/lean/; everything it writes is under build/lean/ of the worktree.
"""
import argparse
import glob
import json
import random
import re
import subprocess
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "build" / "lean"
sys.path.insert(0, str(ROOT / "tools"))
import context  # noqa: E402  (this folder)
import similar  # noqa: E402  (this folder)
import tu_map  # noqa: E402


def tier(size):
    return "small" if size <= 600 else "large"


def main():
    ap = argparse.ArgumentParser()
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--sample", type=int)
    g.add_argument("--all", action="store_true")
    g.add_argument("--addrs", type=Path)
    g.add_argument("--drafts", type=Path, nargs="+")
    ap.add_argument("--effort", help="Agent effort for every job, e.g. high")
    ap.add_argument("--ghidra", action="store_true")
    ap.add_argument("--model", help="one model for every job (no retries); default: coord.py's tiers")
    ap.add_argument("--budget", type=int, default=3)
    ap.add_argument("--max", type=int, default=2000)
    ap.add_argument("--prior", default="")
    ap.add_argument("--refs", type=int, default=3, help="reference matches per function (0: none)")
    ap.add_argument("--seed", type=int, default=1109)
    a = ap.parse_args()

    OUT.mkdir(parents=True, exist_ok=True)
    funcs = context.load_functions()
    defs = subprocess.run(["git", "grep", "-hE", r"^[A-Za-z].*\bfn_[0-9a-f]{10}\(.*\)\s*(const\s*)?\{?$", "--",
                           "src", ":!src/lifted"], cwd=ROOT, capture_output=True, text=True).stdout
    defined = {int(m.group(1), 16) for m in re.finditer(r"\bfn_([0-9a-f]{10})\(", defs)}
    prior = {}
    for p in glob.glob(a.prior) if a.prior else []:
        found = json.load(open(p, encoding="utf-8"))
        for r in found if isinstance(found, list) else [found]:
            ad = str(r.get("addr", "")).replace("0x", "").replace("fn_", "")
            if r.get("outcome") != "match":
                prior[ad] = {"outcome": r.get("outcome"), "best": r.get("best_similarity") or r.get("best") or 0,
                             "blocker": r.get("blocker", "")}
    draft_src = {}
    for d in a.drafts or []:
        for f in d.glob("*.cpp"):
            try:
                draft_src[int(f.stem.replace("fn_", ""), 16)] = f
            except ValueError:
                pass
    if a.addrs or a.drafts:
        want = ({int(x.strip(), 16) for x in a.addrs.read_text().split() if x.strip()} if a.addrs
                else set(draft_src))
        work = [x for x in want if funcs.get(x, (0, 0, ""))[2] == "U" and x not in defined]
    else:
        work = [x for x, (sz, _, st) in funcs.items() if st == "U" and 24 <= sz <= a.max and x not in defined]
    cfg = ({"model": a.model, "budget": a.budget, "effort": a.effort, "ghidra": a.ghidra, "refs": a.refs} if a.model
           else {"effort": a.effort, "ghidra": a.ghidra, "refs": a.refs})
    (OUT / "config.json").write_text(json.dumps(cfg), encoding="utf-8")

    units = tu_map.load_map()
    starts = tu_map.unit_starts(units)
    entries = []
    for x in work:
        u = tu_map.find_unit(units, x, starts)
        if u is None:
            continue
        d = tu_map.source_folder(u)
        hints = u["class_hint"].split(";") if u["class_hint"] else []
        ad = f"{x:x}"
        entries.append({"addr": ad, "size": funcs[x][0], "tier": tier(funcs[x][0]), "unit": int(u["index"]),
                        "hint": hints[0] if hints else "", "dir": f"src/{d}",
                        "file": f"src/{d}/{d.split('/')[-1]}_<What>_l{ad[-6:]}.cpp", "prior": prior.get(ad)})

    if a.sample:
        rng = random.Random(a.seed)
        fresh_small = [e for e in entries if e["tier"] == "small" and not e["prior"]]
        retry_small = [e for e in entries if e["tier"] == "small" and e["prior"] and e["prior"]["best"] >= 50]
        large = [e for e in entries if e["tier"] == "large"]
        n = a.sample
        k_retry = min(round(n * 0.2), len(retry_small))
        k_fresh = round(n * 0.6) - k_retry
        entries = (rng.sample(fresh_small, min(k_fresh, len(fresh_small))) + rng.sample(retry_small, k_retry) +
                   rng.sample(large, min(n - k_fresh - k_retry, len(large))))
    # small first (cheapest wins first), then large; prior failures last within each tier
    entries.sort(key=lambda e: (e["tier"] != "small", e["prior"] is not None, e["size"]))

    (OUT / "ctx").mkdir(parents=True, exist_ok=True)
    binary = context.Binary()
    hidx = context.header_index()
    index = similar.Index(funcs, similar.definitions(), binary.read) if a.refs else None

    def unit_of(address):
        u = tu_map.find_unit(units, address, starts)
        return u and int(u["index"])

    for e in entries:
        x = int(e["addr"], 16)
        asm, syms = binary.disasm(x, e["size"], funcs)
        decls = sorted({f'#include "{hidx[s][0]}"  // {hidx[s][1]}' for s in syms if s in hidx})
        txt = [f"fn_{e['addr']} ({e['size']} bytes), unit {e['unit']} hint '{e['hint']}'", "",
               "Disassembly (calls, globals, strings annotated; read only, never copy into src/):", asm]
        if decls:
            txt += ["", "Headers that already declare some of these symbols:", *decls]
        if index:
            refs = index.nearest(x, e["size"], a.refs)
            e["refs"] = [{"addr": f"{r['addr']:x}", "kind": r["kind"], "lined": r["lined"], "named": r["named"]}
                         for r in refs]
            txt += ["", *similar.render(refs, lambda t: funcs[t][1] if t in funcs else f"g_unk_{t:x}", unit_of,
                                        e["unit"], lambda ra, rs: binary.disasm(ra, rs, funcs)[0])]
        (OUT / "ctx" / f"{e['addr']}.txt").write_text("\n".join(txt) + "\n", encoding="utf-8")
    if draft_src:
        (OUT / "drafts").mkdir(parents=True, exist_ok=True)
        for e in entries:
            src = draft_src.get(int(e["addr"], 16))
            if src:
                (OUT / "drafts" / f"{e['addr']}.cpp").write_bytes(src.read_bytes())
    if a.ghidra:
        (OUT / "ghidra").mkdir(parents=True, exist_ok=True)
        for e in entries:
            r = subprocess.run([sys.executable, str(ROOT / "tools" / "ghidra" / "decompile.py"), "--version", "2.1.0",
                                e["addr"]], cwd=ROOT, capture_output=True, text=True)
            if "Ghidra decompilation" not in r.stdout:
                sys.exit(f"prepare: no Ghidra view for {e['addr']}: {(r.stdout or r.stderr).strip()[:200]}")
            (OUT / "ghidra" / f"{e['addr']}.txt").write_text(r.stdout, encoding="utf-8")
    for e in entries:
        e["status"] = "queued"
        e["attempts"] = []
    (OUT / "queue.json").write_text(json.dumps(entries, indent=1), encoding="utf-8")
    print(f"queue: {len(entries)} functions; tiers {dict(Counter(e['tier'] for e in entries))}; "
          f"with a prior failed attempt {sum(1 for e in entries if e['prior'])}; skipped as defined {len(defined)}"
          + (f"; with a reference match {sum(1 for e in entries if e.get('refs'))}" if a.refs else ""))


if __name__ == "__main__":
    main()
