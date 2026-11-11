#!/usr/bin/env python3
"""Overnight run: work list, per-function drafting context, batches and ledger (run once)."""
import bisect
import csv
import json
import re
import subprocess
import sys
from collections import defaultdict
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build" / "overnight"
sys.path.insert(0, str(ROOT / "build" / "pilot"))
import pilot  # noqa: E402

TOP_LEVEL = {"base", "engine", "game"}
NAMED_SOURCES = ("reflection", "init_call", "names", "library")
BATCH = 30


def unit_dir(hint):
    parts = [re.sub(r"[^A-Za-z0-9_]+", "_", p).strip("_") for p in hint.split("::")]
    parts = [p for p in parts if p]
    return "/".join(parts if parts[0] in TOP_LEVEL else ["game", *parts])


def main():
    funcs = pilot.load_functions()
    defined = set()
    out = subprocess.run(["git", "grep", "-hoE", r"fn_[0-9a-f]{10}\(", "--", "src", ":!src/lifted"],
                         cwd=ROOT, capture_output=True, text=True).stdout
    # only definitions count, but a cheap superset is fine: we skip anything already written by hand
    defs = subprocess.run(["git", "grep", "-hE", r"^[A-Za-z].*\bfn_[0-9a-f]{10}\(.*\)\s*(const\s*)?\{?$", "--",
                           "src", ":!src/lifted"], cwd=ROOT, capture_output=True, text=True).stdout
    for m in re.finditer(r"\bfn_([0-9a-f]{10})\(", defs):
        defined.add(int(m.group(1), 16))
    work = sorted((a for a, (sz, _, st) in funcs.items() if st == "U" and 24 <= sz <= 600 and a not in defined),
                  key=lambda a: (funcs[a][0], a))
    rows = list(csv.DictReader(open(ROOT / "data" / "tu_map.csv", newline="")))
    starts = [int(r["start"], 16) for r in rows]

    def unit_of(a):
        i = bisect.bisect_right(starts, a) - 1
        if i >= 0 and a <= int(rows[i]["end"], 16):
            return rows[i]
        return None

    units = defaultdict(list)
    meta = {}
    for a in work:
        u = unit_of(a)
        if u is None:
            continue
        hints = u["class_hint"].split(";") if u["class_hint"] else []
        named = bool(hints) and u["hint_source"] in NAMED_SOURCES and u["confidence"] == "high"
        key = u["index"]
        units[key].append(a)
        if key not in meta:
            d = unit_dir(hints[0]) if named else None
            meta[key] = {"index": int(key), "named": named, "dir": d,
                         "stem": d.split("/")[-1] if d else None, "hint": hints[0] if hints else ""}

    # context files
    (OUT / "ctx").mkdir(parents=True, exist_ok=True)
    binary = pilot.Binary()
    hidx = pilot.header_index()
    (OUT / "system_prompt.txt").write_text(pilot.system_prompt(), encoding="utf-8")
    for key, addrs in units.items():
        m = meta[key]
        for a in addrs:
            size = funcs[a][0]
            asm, syms = binary.disasm(a, size, funcs)
            decls = sorted({f'#include "{hidx[s][0]}"  // {hidx[s][1]}' for s in syms if s in hidx})
            txt = [f"fn_{a:x} ({size} bytes), unit {m['index']} hint '{m['hint']}'",
                   f"target file dir: src/{m['dir']}/" if m["dir"] else "unit has no reliable name",
                   "", "Disassembly (calls, globals, strings annotated; read only, never copy into src/):", asm]
            if decls:
                txt += ["", "Headers that already declare some of these symbols:", *decls]
            (OUT / "ctx" / f"{a:x}.txt").write_text("\n".join(txt) + "\n", encoding="utf-8")

    # batches: named units first (by smallest function), one unit per batch; small units share
    order = sorted(units, key=lambda k: (not meta[k]["named"], funcs[units[k][0]][0]))
    batches = []
    pending = None
    for k in order:
        addrs = units[k]
        chunks = [addrs[i:i + BATCH] for i in range(0, len(addrs), BATCH)]
        for ci, ch in enumerate(chunks):
            if len(chunks) == 1 and len(ch) < BATCH // 2:
                if pending is None or len(pending["addrs"]) + len(ch) > BATCH or pending["named"] != meta[k]["named"]:
                    pending = {"addrs": [], "units": [], "named": meta[k]["named"], "part": {}}
                    batches.append(pending)
                pending["addrs"] += ch
                pending["units"].append(k)
            else:
                batches.append({"addrs": ch, "units": [k], "named": meta[k]["named"], "part": {k: ci}})
    ledger = {"meta": meta, "batches": []}
    for i, b in enumerate(batches):
        bid = f"b{i:04d}"
        ledger["batches"].append({"id": bid, "units": b["units"], "named": b["named"], "part": b["part"],
                                  "addrs": [f"{a:x}" for a in b["addrs"]], "status": "queued", "results": {}})
    (OUT / "ledger.json").write_text(json.dumps(ledger, indent=1), encoding="utf-8")
    print(f"work {len(work)} functions, {len(units)} units, {len(batches)} batches "
          f"({sum(b['named'] for b in batches)} named); already defined skipped: {len(defined)}")


if __name__ == "__main__":
    main()
