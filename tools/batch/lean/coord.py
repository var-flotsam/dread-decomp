#!/usr/bin/env python3
"""Lean run coordinator helper (all state in build/lean/queue.json; runs in place from tools/batch/lean/).

  coord.py wave <n> [--per-worker K]
                          record finished results, then print the next n jobs as JSON lines
                          {"addr", "addrs", "model", "prompt"}: one Agent call each (model = the Agent tool's
                          model). A job holds up to K functions of one unit that get the same model (K from
                          --per-worker, else prepare.py --per-worker, else 1); each keeps its own budget
  coord.py record         fold build/lean/results/*.json into the queue (wave does this first)
  coord.py land           grade every match again, check its declarations against the tree and `git add` it;
                          a file that does not match any more becomes a draft. Commits nothing
  coord.py file --tag T   every function that did not match goes to drafts/ with a graded row of
                          drafts/INDEX.csv (tools/drafts.py), `from` = T; the registers-only ones are listed in
                          build/lean/registers.txt for tools/permute.py and tools/equiv_test.py
  coord.py stats          counts by outcome, tier and model; per-tier match rates; the attempt each match
                          came on (try.py's count), which is what the attempt budget is set from
  coord.py reset-running  requeue jobs left "running" by an interrupted wave

A worker with several functions pays its start-up once: every lean worker starts at about 100,000 tokens, so
one function per worker cost about 736,000 tokens a match in the pilot of 2026-11-12, where the batch runs of
2026-11-09 to 11-11, about 30 functions to a worker, came to about 10,000 a function (docs/JOURNAL.md).

Model and budget per job (prepare.py --model/--budget replaces the first three with one job per function):
  small (<= 600 B), first pass        sonnet, 2 attempts
  large (> 600 B),  first pass        opus,   3 attempts
  a sonnet non-match with best >= 75  one opus retry from its draft, 2 attempts
  a registers-only result             no further model attempt: the permuter and the machine test take it
                                      from there (CLAUDE.md, "Completion", tier 2)

The budget is a cap, not advice: wave writes it into the job's queue entry, and try.py counts the graded runs
against it, shows the worker how many are left after each one and stops grading when they are spent. record
takes try.py's count (not the worker's own) into the attempt: `graded` (each run's score) and `matched_on`.
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


def ordinal(n):
    return f"{n}{'th' if 10 <= n % 100 <= 20 else {1: 'st', 2: 'nd', 3: 'rd'}.get(n % 10, 'th')}"


def graded(e, out=None):
    """try.py's count for e's current job, or None."""
    try:
        state = json.loads(((out or OUT) / "attempts" / f"{e['addr']}.json").read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return None
    return state if state.get("job") == len(e["attempts"]) else None


def prompt(e, model, budget, cfg=None):
    cfg = CFG if cfg is None else cfg
    lines = [f"Follow {(KIT / 'worker.md').relative_to(ROOT).as_posix()} exactly (read it first).",
             f"Worktree: {ROOT}",
             f"Address: {e['addr']} ({e['size']} bytes); context build/lean/ctx/{e['addr']}.txt",
             f"Target file: {e['file']}",
             f"Attempt budget: {budget}. It is a cap: try.py counts each graded run, ends its output with how many "
             f"are left, and does not grade this function after the {ordinal(budget)}."]
    if cfg.get("refs"):
        lines.append("The context ends with the reference matches: functions that already match and look most like "
                     "this one, with their source. Read them before you write.")
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


def prompt_many(es, model, cfg=None):
    """One worker's brief for several functions, done in order, each with its own budget."""
    cfg = CFG if cfg is None else cfg
    head = [f"Follow {(KIT / 'worker.md').relative_to(ROOT).as_posix()} exactly (read it first, and only once).",
            f"Worktree: {ROOT}",
            f"You have {len(es)} functions, in this order. Finish each one (its result file written) before you "
            "start the next. Each has its own attempt budget, a cap that try.py enforces; one function's unused "
            "attempts never carry over to another."]
    blocks = []
    for i, e in enumerate(es, 1):
        lines = prompt(e, model, e["budget"], cfg).splitlines()[2:]   # without the two header lines
        blocks.append(f"Function {i} of {len(es)}:\n" + "\n".join("  " + x for x in lines))
    return "\n".join(head) + "\n\n" + "\n\n".join(blocks)


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
        state = graded(e)
        if state:  # what try.py counted, beside what the worker reported
            r["graded"] = [g["score"] for g in state["graded"]]
            r["matched_on"] = next((i for i, g in enumerate(state["graded"], 1) if g["match"]), None)
            r["budget"] = state["budget"]
        e["attempts"].append(r)
        e["status"] = "queued" if plan(e) else STATUS.get(r.get("outcome"), "failed")
        p.rename(p.with_suffix(f".{len(e['attempts'])}.done"))
        n += 1
    return n


def wave(q, k, per_worker=None, cfg=None):
    """Up to k jobs. A job takes up to per_worker queued functions of one unit that get the same model, in queue
    order; with per_worker 1 (the default) it is one function, as before."""
    cfg = CFG if cfg is None else cfg
    per = max(1, int(per_worker or cfg.get("per_worker") or 1))
    groups, open_group = [], {}
    for e in q:
        if e["status"] != "queued":
            continue
        pl = plan(e, cfg)
        if not pl:
            e["status"] = "failed"
            continue
        key = (e.get("unit"), pl[0])
        g = open_group.get(key)
        if g is None or len(g["entries"]) >= per:
            if len(groups) >= k:
                continue
            g = open_group[key] = {"model": pl[0], "entries": []}
            groups.append(g)
        e["status"], e["running_model"], e["budget"] = "running", pl[0], pl[1]
        g["entries"].append(e)
    jobs = []
    for g in groups:
        es = g["entries"]
        text = prompt(es[0], g["model"], es[0]["budget"], cfg) if len(es) == 1 else prompt_many(es, g["model"], cfg)
        job = {"addr": es[0]["addr"], "addrs": [e["addr"] for e in es], "model": g["model"], "prompt": text}
        if cfg.get("effort"):
            job["effort"] = cfg["effort"]
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


def attempt_stats(q):
    """On which graded attempt the matches came, and how many graded attempts the run made: the figures an
    attempt budget is set from. Jobs try.py did not count (an older run) are left out."""
    jobs = [a for e in q for a in e["attempts"] if "graded" in a]
    on = Counter(a["matched_on"] for a in jobs if a.get("outcome") == "match" and a.get("matched_on"))
    refs = Counter(("with a reference" if e.get("refs") else "without a reference", e["status"]) for e in q
                   if e["attempts"] and "refs" in e)
    return {"matched_on_attempt": {str(k): on[k] for k in sorted(on)},
            "graded_attempts": sum(len(a["graded"]) for a in jobs),
            "references": {f"{k}/{s}": c for (k, s), c in sorted(refs.items())}}


def stats(q):
    st = Counter(e["status"] for e in q)
    att = Counter((a["model"], a["outcome"]) for e in q for a in e["attempts"])
    tiers = {}
    for e in q:
        t = tiers.setdefault(e["tier"] + ("/prior" if e["prior"] else ""), Counter())
        t[e["status"]] += 1
    print(json.dumps({"status": st, "attempts": {f"{m}/{o}": c for (m, o), c in att.items()},
                      "tiers": tiers, **attempt_stats(q)}, default=dict))


def main():
    cmd = sys.argv[1]
    q = load()
    if cmd in ("record", "wave"):
        n = record(q)
        save(q)
        if cmd == "record":
            print(f"recorded {n}")
            return 0
        per = int(sys.argv[sys.argv.index("--per-worker") + 1]) if "--per-worker" in sys.argv else None
        jobs = wave(q, int(sys.argv[2]), per)
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
