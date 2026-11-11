You are the coordinator of a lean decompilation test run for dread-decomp: 50 functions, one worker agent per
function, to measure the token cost per matched function. Run unsupervised to the end. Your own context is a
cost too: don't read files you don't need, don't echo outputs back, keep every message short.

Setup (once; stop and report if a step fails):
1. In <repos>\dread-decomp run `git fetch -q origin`. Create a worktree:
   `git worktree add ../dread-decomp-lean -b claude/lean-test-<YYYYMMDD> origin/main`. Then in
   <repos>\dread-decomp-lean (cmd): `mklink /J .venv ..\dread-decomp\.venv` and
   `mklink /J orig ..\dread-decomp\orig`, and copy `..\dread-decomp\build\lean` and `..\dread-decomp\build\pilot`
   (pilot.py only is needed) into its build\. Also copy the latest
   `..\dread-decomp-overnight\build\overnight\tips.md` over build\lean\tips.md if it exists.
   Copy `..\dread-decomp\build\recon` into its build\ too, and run `git submodule update --init` in the
   worktree (real checkouts; never junction lib/musl or lib/nnsdk). Check one compile before the first wave:
   `.venv/Scripts/python build/lean/try.py src/atk_id/nn_atk_Shutdown_pilot.cpp` must print MATCH.
   From here on work only in the dread-decomp-lean worktree. Never touch dread-decomp-overnight or
   dread-decomp except to read the two files above.
2. `.venv/Scripts/python build/lean/prepare.py --sample 50`

Loop:
3. `.venv/Scripts/python build/lean/coord.py wave 8` prints up to 8 JSON jobs {addr, model, prompt}. In ONE
   message, start one Agent per job (subagent_type general-purpose, model = the job's model, prompt = the
   job's prompt verbatim, description "lean <addr>"), all in the foreground so the wave returns together.
   Don't inspect their work. Then run the next wave. Stop when it prints QUEUE EMPTY.
   If a wave is interrupted, run `coord.py reset-running` before the next one.

Finish:
4. `.venv/Scripts/python build/lean/coord.py commit` (re-grades every match, checks declarations, one commit).
   If it lists declaration problems, fix them in those files (copy the listed signature, or include the
   header) and rerun; drop a file you can't fix.
5. `.venv/Scripts/python tools/check.py --version 2.1.0 --update` (must report no REGRESSION, DUPLICATE or
   hand-written RELOC), then `.venv/Scripts/python tools/decl_check.py --base origin/main` (must PASS). Commit
   data/functions with "Phase 4.7: lean run statuses". Push the branch and open a PR titled
   "Phase 4.7: lean test run (<n> functions)". Do not merge it.
6. `.venv/Scripts/python build/lean/usage.py` and `.venv/Scripts/python build/lean/coord.py stats`. Write
   the results to build/lean/REPORT.md: matched per tier (fresh small / overnight retry / large), per model,
   the usage.py table (per job and per match, vs the overnight run), and which rules wasted the most
   (e.g. opus retries that failed). End with one recommendation for the full run's settings.
   Reply with that summary and the PR link.

Rules: CLAUDE.md's hard rules apply. Commits end with "Co-Authored-By: Claude Opus 5.5 <noreply@anthropic.com>";
the PR body ends with "🤖 Generated with [Claude Code](https://claude.com/claude-code)". Never use --no-verify.
