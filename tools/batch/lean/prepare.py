#!/usr/bin/env python3
"""Lean run: work queue and per-function context (run once in the run's worktree, after merging origin/main).

  prepare.py --sample 50            stratified test sample (seeded)
  prepare.py --all [--max 2000]     every remaining status-U function from 24 B up to --max bytes
  prepare.py --addrs FILE [--model fable --budget 3 --minor]
                                    exactly the addresses in FILE (one hex address per line); the options
                                    go to build/lean/config.json, which coord.py follows for every job
  prepare.py --drafts DIR [DIR...]  the still-U functions that have a draft DIR/<addr>.cpp; each draft is copied
                                    to build/lean/drafts/ and the worker starts from it
  --effort high                     the Agent effort for every job (config.json)
  --ghidra                          save Ghidra's view of each function to build/lean/ghidra/<addr>.txt (read-only
                                    reference for the worker; build/ is never committed). Sequential: Ghidra's
                                    project admits one process, so workers must not run tools/ghidra themselves

Writes build/lean/queue.json (one entry per function: addr, size, tier, target file, prior attempt) and
build/lean/ctx/<addr>.txt (annotated disassembly + headers that already declare its symbols).
Prior attempts come from the overnight run's results (--prior, a glob of results/*.json files).
"""
import argparse
import bisect
import csv
import glob
import json
import random
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build" / "lean"
sys.path.insert(0, str(ROOT / "build" / "pilot"))
import pilot  # noqa: E402

TOP_LEVEL = {"base", "engine", "game"}
NAMED_SOURCES = ("reflection", "init_call", "names", "library")
DEFAULT_PRIOR = str(ROOT.parent / "dread-decomp-overnight" / "build" / "overnight" / "results" / "*.json")


def unit_dir(hint):
    parts = [re.sub(r"[^A-Za-z0-9_]+", "_", p).strip("_") for p in hint.split("::")]
    parts = [p for p in parts if p]
    return "/".join(parts if parts[0] in TOP_LEVEL else ["game", *parts])


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
    ap.add_argument("--minor", action="store_true",
                    help="a function left with only a registers difference may end as NON_MATCHING(minor)")
    ap.add_argument("--max", type=int, default=2000)
    ap.add_argument("--prior", default=DEFAULT_PRIOR)
    ap.add_argument("--seed", type=int, default=1109)
    a = ap.parse_args()

    funcs = pilot.load_functions()
    defs = subprocess.run(["git", "grep", "-hE", r"^[A-Za-z].*\bfn_[0-9a-f]{10}\(.*\)\s*(const\s*)?\{?$", "--",
                           "src", ":!src/lifted"], cwd=ROOT, capture_output=True, text=True).stdout
    defined = {int(m.group(1), 16) for m in re.finditer(r"\bfn_([0-9a-f]{10})\(", defs)}
    prior = {}
    for p in glob.glob(a.prior):
        for r in json.load(open(p, encoding="utf-8")):
            ad = str(r.get("addr", "")).replace("0x", "").replace("fn_", "")
            if r.get("outcome") != "match":
                prior[ad] = {"outcome": r.get("outcome"), "best": r.get("best_similarity") or 0,
                             "blocker": r.get("blocker", "")}
    draft_src = {}
    if a.drafts:
        for d in a.drafts:
            for f in d.glob("*.cpp"):
                try:
                    draft_src[int(f.stem, 16)] = f
                except ValueError:
                    pass
    if a.addrs or a.drafts:
        want = ({int(l.strip(), 16) for l in a.addrs.read_text().split() if l.strip()} if a.addrs
                else set(draft_src))
        work = [x for x in want if funcs.get(x, (0, 0, ""))[2] == "U" and x not in defined]
    else:
        work = [x for x, (sz, _, st) in funcs.items() if st == "U" and 24 <= sz <= a.max and x not in defined]
    cfg = ({"model": a.model, "budget": a.budget, "minor": a.minor, "effort": a.effort, "ghidra": a.ghidra}
           if a.model else {})
    (OUT / "config.json").write_text(json.dumps(cfg), encoding="utf-8")

    rows = list(csv.DictReader(open(ROOT / "data" / "tu_map.csv", newline="")))
    starts = [int(r["start"], 16) for r in rows]

    def unit_of(x):
        i = bisect.bisect_right(starts, x) - 1
        return rows[i] if i >= 0 and x <= int(rows[i]["end"], 16) else None

    entries = []
    for x in work:
        u = unit_of(x)
        if u is None:
            continue
        hints = u["class_hint"].split(";") if u["class_hint"] else []
        named = bool(hints) and u["hint_source"] in NAMED_SOURCES and u["confidence"] == "high"
        d = unit_dir(hints[0]) if named else f"game/UnkTU{u['index']}"
        stem = d.split("/")[-1]
        ad = f"{x:x}"
        entries.append({"addr": ad, "size": funcs[x][0], "tier": tier(funcs[x][0]), "unit": int(u["index"]),
                        "hint": hints[0] if hints else "", "dir": f"src/{d}",
                        "file": f"src/{d}/{stem}_<What>_l{ad[-6:]}.cpp", "prior": prior.get(ad)})

    if a.sample:
        rng = random.Random(a.seed)
        fresh_small = [e for e in entries if e["tier"] == "small" and not e["prior"]]
        retry_small = [e for e in entries if e["tier"] == "small" and e["prior"] and e["prior"]["best"] >= 50]
        large = [e for e in entries if e["tier"] == "large"]
        n = a.sample
        k_fresh, k_retry = round(n * 0.4), round(n * 0.2)
        pick = (rng.sample(fresh_small, min(k_fresh, len(fresh_small))) +
                rng.sample(retry_small, min(k_retry, len(retry_small))) +
                rng.sample(large, min(n - k_fresh - k_retry, len(large))))
        entries = pick
    # small first (cheapest wins first), then large; prior failures last within each tier
    entries.sort(key=lambda e: (e["tier"] != "small", e["prior"] is not None, e["size"]))

    (OUT / "ctx").mkdir(parents=True, exist_ok=True)
    binary = pilot.Binary()
    hidx = pilot.header_index()
    for e in entries:
        x = int(e["addr"], 16)
        asm, syms = binary.disasm(x, e["size"], funcs)
        decls = sorted({f'#include "{hidx[s][0]}"  // {hidx[s][1]}' for s in syms if s in hidx})
        txt = [f"fn_{e['addr']} ({e['size']} bytes), unit {e['unit']} hint '{e['hint']}'", "",
               "Disassembly (calls, globals, strings annotated; read only, never copy into src/):", asm]
        if decls:
            txt += ["", "Headers that already declare some of these symbols:", *decls]
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
    from collections import Counter
    print(f"queue: {len(entries)} functions; tiers {dict(Counter(e['tier'] for e in entries))}; "
          f"with a prior failed attempt {sum(1 for e in entries if e['prior'])}; skipped as defined {len(defined)}")


if __name__ == "__main__":
    main()
