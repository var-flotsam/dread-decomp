#!/usr/bin/env python3
"""try.py <file.cpp> [--full]: tools/trymatch.py with short output, for agents.

Prints each function's summary line (MATCH / similarity / differing insns), its RELOC lines, and only
the differing instructions with one line of context, at most 30 diff lines per function (--full: all).
A build failure prints the first 20 error lines. Objects go under build/lean/obj/<file stem>/ so
parallel agents never share one. Exit status as trymatch: 0 all match, 1 differs, 2 build failure.

The attempt cap. While a function's job is running (coord.py wave gave it a budget), each run that builds
and is graded is one attempt. try.py counts them in build/lean/attempts/<addr>.json and ends every run
with an ATTEMPT line: how many are graded, the best score, how many are left. When the budget is spent it
does not build any more (BUDGET SPENT, exit status 3). What does not count: a build failure (as many as the
budget are free, then each one counts), and CONFIRM runs after a match (declfile.py's fixes are graded again).
Outside a running job (coord.py land, a hand run) nothing is counted.
"""
import json
import re
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
OUT = ROOT / "build" / "lean"
CAP = 30
CONFIRM = 2   # runs after a match that do not count
SUMMARY = re.compile(r"^\s*(MATCH|[\d.]+%)\s+0x([0-9a-f]+)\s")


def job_of(src, queue):
    """The running queue entry that `src` is the source of, or None: by the file name's address suffix
    (`..._l<last six digits>.cpp`, or `<addr>.cpp` for a draft), else by the function the text defines."""
    running = [e for e in queue if e.get("status") == "running" and e.get("budget")]
    stem = Path(src).stem
    m = re.search(r"_l([0-9a-f]{6})$", stem)
    found = [e for e in running if e["addr"] == stem.replace("fn_", "") or (m and e["addr"][-6:] == m.group(1))]
    if len(found) != 1:
        try:
            text = Path(src).read_text(encoding="utf-8", errors="replace")
        except OSError:
            text = ""
        found = [e for e in found or running
                 if re.search(rf"^[A-Za-z_][^;\n]*\bfn_{e['addr']}\s*\([^;]*$", text, re.M)]
    return found[0] if len(found) == 1 else None


def load_state(entry, out=None):
    """This job's count so far. A count left by an earlier job of the same function is not this job's."""
    job = len(entry.get("attempts") or [])
    path = (out or OUT) / "attempts" / f"{entry['addr']}.json"
    try:
        state = json.loads(path.read_text(encoding="utf-8"))
    except (OSError, ValueError):
        state = {}
    if state.get("job") != job:
        state = {"addr": entry["addr"], "job": job, "graded": [], "build_failures": 0, "confirm": 0}
    state["budget"] = int(entry["budget"])
    return state, path


def spent(state):
    """Why try.py will not grade now, or None."""
    matched = any(g["match"] for g in state["graded"])
    if matched and state["confirm"] >= CONFIRM:
        return "the function matched and the confirmation runs are used"
    if not matched and len(state["graded"]) >= state["budget"]:
        return f"{len(state['graded'])} of {state['budget']} attempts are graded"
    return None


def best(state):
    return max((g["score"] for g in state["graded"]), default=0.0)


def spent_line(state, why):
    last = state["graded"][-1] if state["graded"] else {}
    return (f"BUDGET SPENT: {why} (best {best(state):.1f}%, last KIND {last.get('kind') or 'n/a'}). try.py did not "
            "build. Write the result now (worker.md, step 5).")


def grade_of(lines, addr, returncode):
    """(score 0-100, KIND, match) of the function at addr in trymatch's output."""
    score, kind, match, own = 0.0, "", False, False
    for l in lines:
        m = SUMMARY.match(l)
        if m:
            own = m.group(2) == addr
            if own:
                match = m.group(1) == "MATCH"
                score = 100.0 if match else float(m.group(1).rstrip("%"))
        elif own and l.startswith("  KIND"):
            kind = l.split(None, 1)[1].strip()
        elif own and l.startswith("  RELOC"):
            match = False
    return score, kind, match and returncode == 0


def count(state, lines, returncode):
    """Add this run to the count; the line that ends its output."""
    n, budget = len(state["graded"]), state["budget"]
    if any(g["match"] for g in state["graded"]):
        state["confirm"] += 1
        left = CONFIRM - state["confirm"]
        return f"CONFIRM run {state['confirm']} of {CONFIRM} after the match (not an attempt); {left} left."
    if returncode == 2:
        state["build_failures"] += 1
        free = budget - state["build_failures"]
        if free >= 0:
            return (f"BUILD FAILURE {state['build_failures']} (not an attempt; {free} more are free, then each one "
                    f"counts). ATTEMPT {n} of {budget} graded.")
        score, kind, match = 0.0, "build failure", False
    else:
        score, kind, match = grade_of(lines, state["addr"], returncode)
    state["graded"].append({"score": score, "kind": kind, "match": match})
    n += 1
    if match:
        return (f"ATTEMPT {n} of {budget}: MATCH. Run declfile.py and fix what it lists; {CONFIRM} confirmation runs "
                "are free.")
    if n >= budget:
        return (f"ATTEMPT {n} of {budget} graded (best {best(state):.1f}%): the budget is spent. Write the result now "
                "(worker.md, step 5); try.py will not grade this function again.")
    return f"ATTEMPT {n} of {budget} graded (best {best(state):.1f}%). {budget - n} left."


def main():
    args = [x for x in sys.argv[1:] if x != "--full"]
    full = "--full" in sys.argv
    src = args[0]
    entry = state = None
    if (OUT / "queue.json").exists():
        entry = job_of(src, json.loads((OUT / "queue.json").read_text(encoding="utf-8")))
    if entry:
        state, state_path = load_state(entry)
        why = spent(state)
        if why:
            print(spent_line(state, why))
            return 3
    py = ROOT / ".venv" / "Scripts" / "python.exe"
    if not py.exists():
        py = Path(sys.executable)
    p = subprocess.run([str(py), str(ROOT / "tools" / "trymatch.py"), "--version", "2.1.0", src, "-v",
                        "--obj-dir", str(ROOT / "build" / "lean" / "obj" / Path(src).stem), *args[1:]],
                       cwd=ROOT, capture_output=True, text=True)
    lines = (p.stdout + p.stderr).splitlines()
    deadline = None
    if entry:
        deadline = count(state, lines, p.returncode)
        state_path.parent.mkdir(parents=True, exist_ok=True)
        state_path.write_text(json.dumps(state), encoding="utf-8")
    if p.returncode == 2:
        errs = [l for l in lines if "error" in l.lower()] or lines
        print("\n".join(errs[:20]))
        if len(errs) > 20:
            print(f"... {len(errs) - 20} more error lines")
        if deadline:
            print(deadline)
        return 2
    out, block = [], []

    def flush():
        if not block:
            return
        keep = set()
        for i, l in enumerate(block):
            if l.lstrip().startswith("!!"):
                keep.update((i - 1, i, i + 1))
        shown, last = 0, -2
        for i in sorted(k for k in keep if 0 <= k < len(block)):
            if not full and shown >= CAP:
                out.append(f"      ... {sum(1 for l in block if l.lstrip().startswith('!!'))} differing in all; "
                           "fix the first ones, then rerun")
                break
            if i != last + 1:
                out.append("      ...")
            out.append(block[i])
            shown += 1
            last = i
        block.clear()

    for l in lines:
        if l.startswith("  ") and "RELOC" not in l and "KIND" not in l:
            block.append(l)
        else:
            flush()
            out.append(l)
    flush()
    print("\n".join(out))
    if deadline:
        print(deadline)
    return p.returncode


if __name__ == "__main__":
    sys.exit(main())
