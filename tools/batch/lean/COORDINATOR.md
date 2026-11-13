# Lean run: the coordinator's procedure

You coordinate one batch: one worker agent per function, then the steps that turn their results into a pull
request. Starting a batch needs the owner's go-ahead for that batch (`docs/HANDOFF.md`, "Direction": bulk runs
are on hold); this file is how to run one once it is given, not permission to. `CLAUDE.md`'s hard rules apply
throughout. Your own context is a cost too: don't read files you don't need, don't echo outputs back.

The kit runs in place from `tools/batch/lean/`; everything it writes is under `build/lean/` of the run's
worktree. `PY` below is `.venv/Scripts/python`.

## Setup (once; stop and report if a step fails)

1. A worktree of its own from `origin/main`: in the main checkout,
   `python tools/worktree_setup.py ../dread-decomp-<run> --new claude/<run>-<YYYYMMDD> --oracle` (the venv,
   `orig/`, the function bounds and the Ghidra export, the caches `tools/equiv_test.py` needs; it grades one
   known match as proof). Work only in that worktree from here on.
2. The queue. Pick the functions, then `PY tools/batch/lean/prepare.py --addrs FILE --model sonnet --budget 6
   --ghidra` (one model and one job per function, as the pilot of 2026-11-12 ran), or `--sample N` / `--all`
   for the tiered recipe in `coord.py`'s docstring. `PY tools/hubs.py --version 2.1.0 --ready --boot` lists
   functions whose callees are done; the pilot's result says to stay at 400 bytes or less.
3. `PY tools/decl_check.py > build/lean/decl_before.txt`: the declaration counts the batch must not raise.

## Workers

4. `PY tools/batch/lean/coord.py wave 8` prints up to 8 JSON jobs `{addr, model, prompt}`. In ONE message,
   start one Agent per job (subagent_type general-purpose, model = the job's model, prompt = the job's
   prompt verbatim, description "lean <addr>"), all in the foreground so the wave returns together. Don't
   inspect their work. Then the next wave, until it prints QUEUE EMPTY. After an interrupted wave:
   `coord.py reset-running`.

A worker ends with `match` (its file is in `src/`), `registers` (the same instructions in another order or
with other registers; no further model attempt is spent on it) or `differs`. Everything but a match is in
`build/lean/drafts/`.

## After the workers

5. **Matches.** `PY tools/batch/lean/coord.py land` grades each again, checks its declarations against the
   tree and stages it. Fix what it lists (include the header, or copy the listed signature); a match you
   cannot fix waits in `drafts/` (the command it prints).
6. **Everything else becomes a draft.** `PY tools/batch/lean/coord.py file --tag <run>` files each non-match
   under `drafts/` with a graded row of `drafts/INDEX.csv` and lists the registers-only ones in
   `build/lean/registers.txt`.
7. **The free try.** `PY tools/permute.py --version 2.1.0 $(cat build/lean/registers.txt) --write` reorders
   each registers-only draft's source for about two minutes of CPU; about one in nine becomes a match. Read
   what it changed (`git diff drafts/`), then `PY tools/drafts.py --version 2.1.0 grade --from <run> --write`.
8. **Into `src/`.** `PY tools/drafts.py --version 2.1.0 promote <the run's drafts that match or are
   registers-only> --write` moves a draft when every function in it matches or passes
   `tools/equiv_test.py`, and its declarations fit the tree; the others stay, each with its reason in its row.
   Then `PY tools/equiv_test.py --version 2.1.0 <the moved files that do not match> --write`: the verdict in
   `data/equivalence.csv` is what makes a registers-only function count.
9. **Readability.** `PY tools/fakematch.py --unneeded-barriers --base origin/main --write` removes the
   barriers that change no function's code. Then `PY tools/fakematch.py --base origin/main --list`: a barrier, an inlining attribute or a
   union a worker added only to match comes out, and `PY tools/fakematch.py --unset --base origin/main` lists
   every read of a variable that is not set (a text whose match proves nothing: give the callee the parameters
   it has, or send the file back); grade the file again (`drafts.py add ... --remove` if it no
   longer matches and is not verified).
10. **Declarations.** `PY tools/decl_plan.py --write --grown origin/main`, then
    `PY tools/hub_migrate.py --version 2.1.0 --plan build/decl_plans/chunk_1.tsv` moves what the new files
    copied into unit headers. This step is not only mechanical: expect files to edit by hand (`LEFT` lines),
    and send back to `drafts/` a file whose copied symbol cannot be planned, with the reason as its note.
11. **The gates**, all of them, on the finished tree: `PY tools/check.py --version 2.1.0 --update` (no
    `REGRESSION`, `DUPLICATE`, `EQUIV`, `REFS` or hand-written `RELOC` line), `PY tools/decl_check.py --strict
    --base origin/main` (PASS; compare with `decl_before.txt`), `PY tools/name_check.py --version 2.1.0 --strict`,
    `PY tools/policy_check.py --base origin/main`, `PY tools/port_sweep.py --version 2.1.0 --jobs 8`, the tool tests. A
    `REFS` line on a registers-only function is a wrong callee or global: fix it or send the file back.
12. **Pull request.** Commit (`Phase 4.x: <run>: ...`, with your session's own attribution lines), add the
    run's entry at the top of `docs/JOURNAL.md`, regenerate `docs/HANDOFF.md`'s table and the status card
    (`tools/progress.py --slice boot --handoff`, `tools/status_card.py --write`), push the branch and open
    the pull request with the template's headings. You are the coordinator of this work, so you merge it
    once the required checks are green (`gh pr merge <n> --merge`, never `--admin`).
13. **Report.** `PY tools/batch/lean/usage.py` and `coord.py stats`: matches, registers-only results and
    what became of them (matched by the permuter, verified, held and why), tokens per match against the
    earlier runs (`tools/batch/README.md`), and one recommendation for the next batch's settings. The
    numbers go in the journal entry; the owner decides whether there is a next batch.
