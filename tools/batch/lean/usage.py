#!/usr/bin/env python3
"""usage.py [<session subagents dir>]: token use of a lean run's worker agents, per job and per match.

Without an argument it takes the most recently modified `subagents` directory under ~/.claude/projects/.
Each worker transcript is tied to its address by the "Address: <addr>" line of its brief, and to its
outcome through build/lean/queue.json. Prints totals by model and by outcome, and the cost per match,
next to the overnight run's figures for comparison.
"""
import json
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

OUT = Path(__file__).resolve().parents[3] / "build" / "lean"
KEYS = ("cache_read_input_tokens", "cache_creation_input_tokens", "input_tokens", "output_tokens")
OVERNIGHT_PER_MATCH = {"cache_read_input_tokens": 4_128_966, "cache_creation_input_tokens": 39_344,
                       "output_tokens": 14_392}


def newest_dir():
    dirs = list((Path.home() / ".claude" / "projects").glob("*/*/subagents"))
    return max(dirs, key=lambda d: max((f.stat().st_mtime for f in d.glob("*.jsonl")), default=0))


def main():
    d = Path(sys.argv[1]) if len(sys.argv) > 1 else newest_dir()
    q = {e["addr"]: e for e in json.loads((OUT / "queue.json").read_text(encoding="utf-8"))}
    jobs = []
    for f in d.glob("*.jsonl"):
        t, addr, model = Counter(), None, None
        for line in open(f, encoding="utf-8"):
            try:
                ev = json.loads(line)
            except json.JSONDecodeError:
                continue
            msg = ev.get("message") or {}
            if addr is None and ev.get("type") == "user":
                c = msg.get("content")
                text = c if isinstance(c, str) else " ".join(x.get("text", "") for x in c or [] if isinstance(x, dict))
                m = re.search(r"Address: ([0-9a-f]{10})", text)
                if m:
                    addr = m.group(1)
            u = msg.get("usage")
            if u and ev.get("type") == "assistant":
                model = msg.get("model", model)
                for k in KEYS:
                    t[k] += u.get(k) or 0
                t["turns"] += 1
        if addr and addr in q:
            jobs.append((addr, model, t))
    if not jobs:
        print(f"no lean worker transcripts in {d}")
        return
    by_model, by_out = defaultdict(Counter), defaultdict(Counter)
    total = Counter()
    for addr, model, t in jobs:
        e = q[addr]
        out = e["status"]
        by_model[model] += t
        by_model[model]["jobs"] += 1
        by_out[out] += t
        by_out[out]["jobs"] += 1
        total += t
    matched = sum(1 for e in q.values() if e["status"] == "matched")
    print(f"{d}\n{len(jobs)} worker jobs, {matched} functions matched")
    for name, grp in (("by model", by_model), ("by final status", by_out)):
        print(f"\n{name}:")
        for k, t in grp.items():
            n = t["jobs"]
            print(f"  {k}: {n} jobs, avg turns {t['turns'] / n:.0f}, avg cache read {t['cache_read_input_tokens'] / n:,.0f}, "
                  f"avg cache write {t['cache_creation_input_tokens'] / n:,.0f}, avg output {t['output_tokens'] / n:,.0f}")
    if matched:
        print("\nper matched function (this run vs overnight):")
        for k in ("cache_read_input_tokens", "cache_creation_input_tokens", "output_tokens"):
            ours = total[k] / matched
            base = OVERNIGHT_PER_MATCH[k]
            ratio = f"{base / ours:.1f}x less" if ours <= base else f"{ours / base:.1f}x more"
            print(f"  {k}: {ours:,.0f} vs {base:,} ({ratio})")
        print("  (model mix matters too: Sonnet tokens weigh less against the usage limit than Opus tokens)")


if __name__ == "__main__":
    main()
