# tools/batch/

The kits behind the parallel agent runs. Two things are here, and they are not alike:

- **`lean/` is the kit to use**, rewritten on 2026-11-12 for the rules made since the runs of 2026-11-09 to
  2026-11-11: the proof tiers (`CLAUDE.md`, "Completion"), one declaration per shared symbol, `drafts/`, and who
  merges. It runs in place from this folder.
- **`overnight/` is a record** of how the batch runs of those three days were made. Its worker rules contradict
  the rules of today (below); do not run it.

**Bulk runs are on hold** (owner, 2026-11-11; `docs/HANDOFF.md`, "Direction"). A kit that follows the rules is
a precondition for lifting the hold, not the decision: each batch needs the owner's go-ahead.

## lean/: one worker per function

`lean/COORDINATOR.md` is the procedure the coordinating session follows, `lean/worker.md` the brief each
worker gets.

| File | What it does |
|---|---|
| `prepare.py` | builds the queue and each function's context (`context.py`: annotated disassembly and the headers that already declare its symbols; game-derived, so under `build/lean/`, never committed) |
| `coord.py` | hands out jobs (`wave`), records results, then `land` (grade the matches again, check their declarations, stage them) and `file` (every non-match becomes a graded draft under `drafts/`) |
| `try.py` | the short-diff grader a worker uses |
| `declfile.py` | a new file's declarations against the tree (`tools/decl_check.py`'s `file_problems`) |
| `usage.py` | token use per job and per match, from the worker transcripts |
| `select_large.py` | a size-stratified sample of large boot-path functions |

What changed from the kit the runs used, and why:

| The runs' rule | Now | Because |
|---|---|---|
| A non-match is deleted, or kept in `src/` as `NON_MATCHING(minor)` on the worker's word | A worker reports `match`, `registers` or `differs`; the coordinator files every non-match in `drafts/`, and only a function that passes `tools/equiv_test.py` enters `src/` without matching | Tier 2 needs the machine test; the test's first run found four real bugs in functions marked minor by hand |
| A close non-match gets another model attempt | A registers-only result gets none: `tools/permute.py` tries it for free, then the machine test judges it | The tiers: a permuter, not model attempts, turns a registers-only difference into a match |
| Barriers, inlining attributes and unions are tools for matching (`tips.md`) | A worker does not add one only to match; `tools/fakematch.py` lists them for the coordinator | Readability outranks a match |
| Copy a declaration, and the copy stays | Copy it with the types its definition has; the coordinator moves copies to unit headers (`tools/decl_plan.py`, `tools/hub_migrate.py`) and CI fails what is left | The runs doubled the copies of declarations in two days |
| The coordinator opens the pull request and does not merge | The coordinator merges its own work on green checks, never with `--admin` | Owner, 2026-11-11 |
| Scripts copied into each worktree's `build/lean/`, with a helper module that was never tracked | Runs in place; nothing to copy | The copies diverged between worktrees |

What has been measured (details in `docs/JOURNAL.md`):

- Every worker starts at about 100,000 tokens of context, so one-function workers cost two to eight times a
  batch worker per match.
- 40-function test, 2026-11-12: giving workers the unit headers and the reflected members changed nothing
  (7 of 20 against 8 of 20 matches); about 310,000 Sonnet tokens per match.
- Pilot of 50 ready boot-path functions of 130 to 600 bytes, 2026-11-12: 8 matches, 10 registers-only, about
  736,000 tokens per match. Stay at 400 bytes or less. The step after the workers was not only mechanical:
  of three copied symbols the tools moved one, and three files were edited by hand.
- Permuter spike, 2026-11-12: source reorderings matched 27 of 248 registers-only functions (11%) for an hour
  of CPU and no tokens.
- Functions up to 600 bytes matched at 50 to 70% on the first runs' first pass, larger ones at 10 to 20%; the
  figures behind the larger-function recipes came from 7 to 20 functions each with several things changed at
  once. Treat them as leads, not rates.

The kit's steps after the workers were tried on the pilot's results on 2026-11-12 (the queue, the job prompts,
filing drafts: the same rows the pilot's one-off scripts wrote). The worker brief as rewritten has not been
run by a worker: the first batch that uses it is also its test.

## overnight/: the record

One worker per batch of about 30 functions of one translation unit. `coord.py` holds the ledger and the
worker prompt, `system_prompt.txt` an earlier single-shot prompt, `finish.sh`, `step.sh`, `trym.sh`,
`requeue.py` and `mkopus.py` the glue around them. The scripts expect to sit in `build/overnight/` of a
worktree and carry a `<repos>` placeholder where its path was. Its rules that no longer hold: a worker never
edits another file and copies every declaration it needs; non-matching code is kept out of the tree; a
function is dropped early on a low first score.

## tips.md

Matching tips the workers collected and read. Many are true observations about Clang 9 (compare order, store
order, `fmin` against `fminnm`); many others are ways to steer the compiler (untyped stores, unions, inlining
attributes), which `lean/worker.md` now rules out. A worker reads it with that rule above it.

Not here, and never to be tracked: each function's context file and the Ghidra views given to some workers
(game-derived), the objects, the ledgers and per-function results. The journal has the runs' totals.
