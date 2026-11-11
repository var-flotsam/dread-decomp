#!/usr/bin/env python3
"""Lean run coordinator helper (all state in build/lean/queue.json).

  coord.py wave <n>     record finished results, then print the next n jobs as JSON lines
                        {"addr", "model", "prompt"}: one Agent call each (model = the Agent tool's model)
  coord.py record       fold build/lean/results/*.json into the queue (wave does this first)
  coord.py commit       commit matched files not yet committed (re-grade each, declfile), one commit
  coord.py stats        counts by outcome, tier and model; per-tier match rates
  coord.py reset-running  requeue jobs left "running" by an interrupted wave

Model and budget per job:
  small (<= 600 B), first pass        sonnet, 2 attempts
  large (> 600 B),  first pass        opus,   3 attempts
  a sonnet non-match with best >= 75  one opus retry from its draft, 2 attempts
  an overnight (Opus) failure         sonnet, 2 attempts (fresh eyes, cheap); no further retry
"""
import json
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
OUT = ROOT / "build" / "lean"
Q = OUT / "queue.json"
PY = str(ROOT / ".venv" / "Scripts" / "python.exe")
RETRY_AT = 75
CFG = json.loads((OUT / "config.json").read_text()) if (OUT / "config.json").exists() else {}


def load():
    return json.loads(Q.read_text(encoding="utf-8"))


def save(q):
    Q.write_text(json.dumps(q, indent=1), encoding="utf-8")


def plan(e):
    """(model, budget) for e's next job, or None when it is finished."""
    if CFG.get("model"):  # one model, one job per function (prepare.py --model)
        return None if e["attempts"] else (CFG["model"], CFG.get("budget", 3))
    if not e["attempts"]:
        if e["prior"]:
            return "sonnet", 2
        return ("sonnet", 2) if e["tier"] == "small" else ("opus", 3)
    last = e["attempts"][-1]
    if last["outcome"] == "match":
        return None
    if (len(e["attempts"]) == 1 and last["model"] == "sonnet" and not e["prior"]
            and (last.get("best") or 0) >= RETRY_AT):
        return "opus", 2
    return None


def prompt(e, model, budget):
    lines = [f"Follow {OUT / 'worker.md'} exactly (read it first).",
             f"Worktree: {ROOT}",
             f"Address: {e['addr']} ({e['size']} bytes); context build/lean/ctx/{e['addr']}.txt",
             f"Target file: {e['file']}",
             f"Attempt budget: {budget}."]
    if CFG.get("ghidra"):
        lines.append(f"Ghidra's view of the function is in build/lean/ghidra/{e['addr']}.txt: read it for the control "
                     "flow and the order of calls only. Never copy its text, names or types into any file; write your "
                     "own readable source per worker.md. Don't run tools/ghidra yourself. Get the calls and the "
                     "control flow right first (trymatch's KIND line), then the operations, then the registers.")
    if CFG.get("minor"):
        lines.append("If the budget ends and trymatch's KIND line for the function is `registers` (same instructions, "
                     "other order or registers), keep it in src as `// NON_MATCHING(minor): <what differs>` on the line "
                     "above the definition, record outcome \"minor\", and stop. Any other KIND: draft and delete, as usual.")
    draft = OUT / "drafts" / f"{e['addr']}.cpp"
    if draft.exists():
        last = e["attempts"][-1] if e["attempts"] else (e["prior"] or {})
        lines.append(f"Start from the previous best draft build/lean/drafts/{e['addr']}.cpp "
                     f"({last.get('best')}%, blocker: {last.get('blocker') or 'n/a'}); copy it to the target file and "
                     "grade it first. Fix what its KIND line names; don't rewrite parts that already match.")
    elif e["prior"]:
        lines.append(f"An earlier run reached {e['prior']['best']}% (blocker: {e['prior']['blocker'] or 'n/a'}).")
    return "\n".join(lines)


def record(q):
    by = {e["addr"]: e for e in q}
    n = 0
    for p in sorted((OUT / "results").glob("*.json")):
        try:
            r = json.loads(p.read_text(encoding="utf-8"))
        except (json.JSONDecodeError, OSError):
            continue
        e = by.get(str(r.get("addr", "")).replace("0x", "").replace("fn_", ""))
        if not e or e["status"] != "running":
            continue
        r["model"] = e.pop("running_model", "?")
        e["attempts"].append(r)
        e["status"] = "queued" if plan(e) else {"match": "matched", "minor": "minor"}.get(r.get("outcome"), "failed")
        p.rename(p.with_suffix(f".{len(e['attempts'])}.done"))
        n += 1
    return n


def main():
    cmd = sys.argv[1]
    q = load()
    if cmd in ("record", "wave"):
        n = record(q)
        save(q)
        if cmd == "record":
            print(f"recorded {n}")
            return
        k = int(sys.argv[2])
        jobs = []
        for e in q:
            if len(jobs) >= k:
                break
            if e["status"] != "queued":
                continue
            pl = plan(e)
            if not pl:
                e["status"] = "failed"
                continue
            e["status"], e["running_model"] = "running", pl[0]
            job = {"addr": e["addr"], "model": pl[0], "prompt": prompt(e, *pl)}
            if CFG.get("effort"):
                job["effort"] = CFG["effort"]
            jobs.append(job)
        save(q)
        for j in jobs:
            print(json.dumps(j))
        if not jobs:
            print("QUEUE EMPTY")
    elif cmd == "reset-running":
        for e in q:
            if e["status"] == "running":
                e["status"] = "queued"
                e.pop("running_model", None)
        save(q)
    elif cmd == "stats":
        from collections import Counter
        st = Counter(e["status"] for e in q)
        att = Counter((a["model"], a["outcome"]) for e in q for a in e["attempts"])
        tiers = {}
        for e in q:
            t = tiers.setdefault(e["tier"] + ("/prior" if e["prior"] else ""), Counter())
            t[e["status"]] += 1
        print(json.dumps({"status": st, "attempts": {f"{m}/{o}": c for (m, o), c in att.items()},
                          "tiers": tiers}, default=dict))
    elif cmd == "commit":
        done = OUT / "committed.txt"
        seen = set(done.read_text().split()) if done.exists() else set()
        files = sorted({a["file"] for e in q for a in e["attempts"]
                        if a.get("outcome") in ("match", "minor") and a.get("file") and a["file"] not in seen})
        minor = {a["file"] for e in q for a in e["attempts"] if a.get("outcome") == "minor"}
        ok = []
        for f in files:
            if not (ROOT / f).exists():
                print("MISSING", f)
                continue
            r = subprocess.run([PY, str(OUT / "try.py"), f], cwd=ROOT, capture_output=True, text=True)
            if r.returncode == 0 or (f in minor and r.returncode == 1 and "NON_MATCHING(minor)" in
                                     (ROOT / f).read_text(encoding="utf-8")):
                ok.append(f)
            else:
                print("NOT MATCHING NOW (left uncommitted):", f, r.stdout.strip().splitlines()[0:1])
        if not ok:
            print("nothing to commit")
            return
        d = subprocess.run([PY, str(OUT / "declfile.py"), *ok], cwd=ROOT, capture_output=True, text=True)
        print(d.stdout.strip().splitlines()[-1] if d.stdout.strip() else d.stderr[-500:])
        if d.returncode != 0:
            print(d.stdout[-3000:])
            print("declaration problems: fix them (or drop those files) before committing")
            sys.exit(1)
        subprocess.run(["git", "add", *ok], cwd=ROOT, check=True)
        msg = (f"Phase 4.7: lean run: {len(ok)} functions\n\n"
               "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>\n")
        subprocess.run(["git", "commit", "-q", "-m", msg], cwd=ROOT, check=True)
        with done.open("a") as fh:
            fh.write("\n".join(ok) + "\n")
        print(f"committed {len(ok)} files")


if __name__ == "__main__":
    main()
