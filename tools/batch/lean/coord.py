#!/usr/bin/env python3
"""Lean run coordinator helper (all state in build/lean/queue.json; runs in place from tools/batch/lean/).

  coord.py wave <n>       record finished results, then print the next n jobs as JSON lines
                          {"addr", "model", "prompt"}: one Agent call each (model = the Agent tool's model)
  coord.py record         fold build/lean/results/*.json into the queue (wave does this first)
  coord.py land           grade every match again, check its declarations against the tree and `git add` it;
                          a file that does not match any more becomes a draft. Commits nothing
  coord.py file --tag T   every function that did not match goes to drafts/ with a graded row of
                          drafts/INDEX.csv (tools/drafts.py), `from` = T; the registers-only ones are listed in
                          build/lean/registers.txt for tools/permute.py and tools/equiv_test.py
  coord.py stats          counts by outcome, tier and model; per-tier match rates
  coord.py reset-running  requeue jobs left "running" by an interrupted wave

Model and budget per job (prepare.py --model/--budget replaces the first three with one job per function):
  small (<= 600 B), first pass        sonnet, 2 attempts
  large (> 600 B),  first pass        opus,   3 attempts
  a sonnet non-match with best >= 75  one opus retry from its draft, 2 attempts
  a registers-only result             no further model attempt: the permuter and the machine test take it
                                      from there (CLAUDE.md, "Completion", tier 2)
"""
import json
import subprocess
import sys
from collections import Counter
from pathlib import Path

KIT = Path(__file__).resolve().parent
ROOT = KIT.parents[2]
OUT = ROOT / "build" / "lean"
Q = OUT / "queue.json"
PY = sys.executable
RETRY_AT = 75
VERSION = "2.1.0"
CFG = json.loads((OUT / "config.json").read_text()) if (OUT / "config.json").exists() else {}
STATUS = {"match": "matched", "registers": "registers", "minor": "registers"}


def load():
    return json.loads(Q.read_text(encoding="utf-8"))


def save(q):
    Q.write_text(json.dumps(q, indent=1), encoding="utf-8")


def plan(e, cfg=None):
    """(model, budget) for e's next job, or None when it is finished."""
    cfg = CFG if cfg is None else cfg
    if cfg.get("model"):  # one model, one job per function (prepare.py --model)
        return None if e["attempts"] else (cfg["model"], cfg.get("budget", 3))
    if not e["attempts"]:
        if e["prior"]:
            return "sonnet", 2
        return ("sonnet", 2) if e["tier"] == "small" else ("opus", 3)
    last = e["attempts"][-1]
    if last["outcome"] in STATUS:
        return None
    if (len(e["attempts"]) == 1 and last["model"] == "sonnet" and not e["prior"]
            and (last.get("best") or 0) >= RETRY_AT):
        return "opus", 2
    return None


def prompt(e, model, budget, cfg=None):
    cfg = CFG if cfg is None else cfg
    lines = [f"Follow {(KIT / 'worker.md').relative_to(ROOT).as_posix()} exactly (read it first).",
             f"Worktree: {ROOT}",
             f"Address: {e['addr']} ({e['size']} bytes); context build/lean/ctx/{e['addr']}.txt",
             f"Target file: {e['file']}",
             f"Attempt budget: {budget}."]
    if cfg.get("ghidra"):
        lines.append(f"Ghidra's view of the function is in build/lean/ghidra/{e['addr']}.txt: read it for the control "
                     "flow and the order of calls only. Never copy its text, names or types into any file; write your "
                     "own readable source per worker.md. Don't run tools/ghidra yourself.")
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
        e["status"] = "queued" if plan(e) else STATUS.get(r.get("outcome"), "failed")
        p.rename(p.with_suffix(f".{len(e['attempts'])}.done"))
        n += 1
    return n


def wave(q, k):
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
    return jobs


def land(q):
    """Grade again every file a worker reported as a match; stage the ones that still are and whose
    declarations fit the tree."""
    ok = []
    for e in q:
        last = e["attempts"][-1] if e["attempts"] else {}
        f = last.get("file")
        if e["status"] != "matched" or not f:
            continue
        if not (ROOT / f).exists():
            print("MISSING", f)
            e["status"] = "failed"
            continue
        r = subprocess.run([PY, str(KIT / "try.py"), f], cwd=ROOT, capture_output=True, text=True)
        if r.returncode == 0 and "RELOC" not in r.stdout:
            ok.append(f)
        else:
            print("NOT A MATCH NOW (it will be filed as a draft):", f, r.stdout.strip().splitlines()[0:1])
            e["status"], last["outcome"] = "failed", "differs"
    save(q)
    if not ok:
        print("no matches to land")
        return 0
    d = subprocess.run([PY, str(KIT / "declfile.py"), *ok], cwd=ROOT, capture_output=True, text=True)
    print(d.stdout.strip() or d.stderr[-500:])
    if d.returncode != 0:
        print("declaration problems: fix them in those files (include the header, or copy the listed signature) and "
              "run `coord.py land` again; a match you cannot fix waits in drafts/:\n"
              f"  python tools/drafts.py --version {VERSION} add FILE ADDR --from TAG --note \"matches; held: ...\" --remove")
        return 1
    subprocess.run(["git", "add", *ok], cwd=ROOT, check=True)
    print(f"{len(ok)} matching files staged; nothing is committed")
    return 0


def file_drafts(q, tag):
    sys.path.insert(0, str(ROOT / "tools"))
    import drafts
    drafts.init(VERSION)
    rows = drafts.read_index()
    by_address = {int(r["address"], 16): r for r in rows}
    registers, filed = [], 0
    for e in q:
        if e["status"] not in ("registers", "failed") or not e["attempts"]:
            continue
        last = e["attempts"][-1]
        address = int(e["addr"], 16)
        kept = last.get("file") and (ROOT / last["file"]).is_file()
        src = ROOT / last["file"] if kept else OUT / "drafts" / f"{e['addr']}.cpp"
        if not src.is_file():
            print("NO DRAFT", e["addr"], last.get("blocker") or "")
            continue
        old = by_address.get(address)
        if old:
            new = drafts.grade(src, address)
            if float(new["similarity"] or 0) <= float(old["similarity"] or 0):
                print(f"kept the draft there is  0x{e['addr']}  ({old['similarity']}% against {new['similarity'] or 0}%)")
                if kept:
                    src.unlink()
                continue
        row = drafts.add(src, address, tag, note=(last.get("blocker") or "")[:110], replace=bool(old),
                         remove=bool(kept), rows=rows)
        by_address[address] = row
        filed += 1
        if row["difference"] == drafts.REGISTERS:
            registers.append(row["file"])
        print(",".join(row[f] for f in drafts.FIELDS)[:170])
    drafts.write_index(rows)
    (OUT / "registers.txt").write_text("\n".join(registers) + ("\n" if registers else ""), encoding="utf-8")
    print(f"{filed} drafts filed ({tag}); {len(registers)} registers-only, listed in build/lean/registers.txt")
    return 0


def stats(q):
    st = Counter(e["status"] for e in q)
    att = Counter((a["model"], a["outcome"]) for e in q for a in e["attempts"])
    tiers = {}
    for e in q:
        t = tiers.setdefault(e["tier"] + ("/prior" if e["prior"] else ""), Counter())
        t[e["status"]] += 1
    print(json.dumps({"status": st, "attempts": {f"{m}/{o}": c for (m, o), c in att.items()},
                      "tiers": tiers}, default=dict))


def main():
    cmd = sys.argv[1]
    q = load()
    if cmd in ("record", "wave"):
        n = record(q)
        save(q)
        if cmd == "record":
            print(f"recorded {n}")
            return 0
        jobs = wave(q, int(sys.argv[2]))
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
        stats(q)
    elif cmd == "land":
        return land(q)
    elif cmd == "file":
        if "--tag" not in sys.argv:
            sys.exit("coord.py file --tag <run name>")
        return file_drafts(q, sys.argv[sys.argv.index("--tag") + 1])
    else:
        sys.exit(__doc__)
    return 0


if __name__ == "__main__":
    sys.exit(main())
