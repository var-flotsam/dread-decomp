# tools/batch/

The kits behind the parallel agent runs of 2026-11-09 to 2026-11-11, which wrote about a third of the
hand-written code. They lived only in gitignored `build/` folders of the worktrees that ran them, in
several diverging copies; these are the latest versions, tracked so the method is in the repository.

**They are a record of how those runs were made, not the way to make the next one.** The owner put bulk
runs on hold on 2026-11-11 (`docs/HANDOFF.md`, "Direction") until workers can share generated class and
declaration headers, and the worker rules below contradict decisions made since:

- they tell a worker never to edit another file and to copy an existing declaration into its own
  (which doubled the copies of declarations in two days);
- they keep non-matching code out of the tree (near-misses now go to `drafts/`, and a function that is
  verified equivalent counts);
- they stop a function early on a low first score (which cut off functions that were close).

**Update, 2026-11-12.** The lean kit ran once more, for the 40-function test of shared headers
(`docs/JOURNAL.md`): copied into a worktree's `build/lean/`, with `prepare.py`'s helper module `pilot.py`
(not tracked; in the owner's local archive of the earlier runs) under `build/pilot/`. The test found no gain from
giving workers the unit headers and the reflected members. What it did show is the step a coordinator now needs
between the workers and the commit, because CI fails a copied declaration since that day:
`tools/decl_plan.py --write --grown origin/main` and `tools/hub_migrate.py --plan` move what the new files
copied into unit headers, and a file whose copied symbol cannot be planned waits in `drafts/`
(`lean/COORDINATOR.md`, step 5). The workers' own rule (copy a declaration, edit no other file) can stay as it
is for that reason: parallel workers must not edit shared headers, and the coordinator's step undoes the copies.

Rewrite the briefs and the coordinator against the new rules before running anything from here.

## What is here

| Path | What it is |
|---|---|
| `lean/` | One worker per function, a model and attempt budget per size tier, retries from a draft. `COORDINATOR.md` is the brief the coordinating session followed, `worker.md` the brief each worker got. `prepare.py` builds the queue and each function's context, `coord.py` hands out jobs and commits matches, `try.py` is the short-diff grader, `declfile.py` checks a new file's declarations against existing ones, `usage.py` sums token use from the transcripts, `select_large.py` picks large functions |
| `overnight/` | The batch runs: one worker per batch of about 30 functions of one translation unit. `coord.py` holds the ledger and the worker prompt, `system_prompt.txt` an earlier single-shot prompt, `finish.sh`, `step.sh`, `trym.sh`, `requeue.py` and `mkopus.py` the glue around them |
| `tips.md` | Matching tips the workers collected and read. Many are ways to steer the compiler (untyped stores, unions, inlining attributes): read them with `tools/fakematch.py` in mind |

Not here, and never to be tracked: each function's context file and the Ghidra views given to some
workers (game-derived), the objects, and the scratch folders. The ledgers and per-function results stay
local too; the journal has the runs' totals.

## Running one as it was

The scripts expect to sit in `build/lean/` (or `build/overnight/`) of a worktree and find the repository
two levels up; `overnight/coord.py` and the three shell scripts carry a `<repos>` placeholder where the
worktree's path was. Copy a folder there to rerun it. `tools/worktree_setup.py` prepares the worktree.

## What the runs measured

Batches cost a half to an eighth of one-function workers per match, because every worker starts at about
100,000 tokens of context. Functions up to 600 bytes matched at 50 to 70% on a first pass; larger ones at
10 to 20%. The measurements behind the larger-function recipes used 7 to 20 functions each and changed
several things at once: treat them as leads, not rates.
