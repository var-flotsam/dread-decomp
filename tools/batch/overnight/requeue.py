#!/usr/bin/env python3
"""requeue.py <bid>: unattempted (skipped, no similarity) functions of a merged batch go into a new queued batch (once)."""
import json, sys
from pathlib import Path
OUT = Path(__file__).resolve().parent
L = json.loads((OUT/"ledger.json").read_text(encoding="utf-8"))
b = next(x for x in L["batches"] if x["id"] == sys.argv[1])
if b.get("retry"):
    print("already a retry"); sys.exit()
skip = [a for a in b["addrs"] if b["results"].get(a, {"outcome": "skipped"}).get("outcome") == "skipped"
        and not b["results"].get(a, {}).get("best_similarity")]
if not skip:
    print("none"); sys.exit()
n = max(int(x["id"][1:]) for x in L["batches"]) + 1
us = [u for u in b["units"] if any(a in skip for a in b["addrs"])]
L["batches"].append({"id": f"b{n:04d}", "units": b["units"], "named": b["named"], "part": b["part"],
                     "addrs": skip, "status": "queued", "results": {}, "retry": 1})
(OUT/"ledger.json").write_text(json.dumps(L, indent=1), encoding="utf-8")
print(f"requeued {len(skip)} as b{n:04d}")
