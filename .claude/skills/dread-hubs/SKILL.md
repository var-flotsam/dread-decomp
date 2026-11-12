---
name: dread-hubs
description: Run a canonical-header batch in var-flotsam/dread-decomp (one declaration per shared fn_/g_unk_ symbol under include/) mechanically with tools/hub_migrate.py first, then by hand on the files it leaves, verified with tools/baseline.py; safe for several agents in one checkout.
---

# Canonical-header batches

Goal: every `fn_<addr>` / `g_unk_<addr>` that more than one file uses is declared once, in a header
under `include/`, with one real-typed signature (CLAUDE.md, "One declaration per shared symbol").
`python tools/decl_check.py` measures the debt (`--list`, `--symbol NAME`, `--definition-types`). CI runs
`--strict --base origin/main`, which fails a pull request that adds a conflicting signature, a local copy
of a symbol `include/` declares, one more identical copy of any shared symbol, or a definition whose types
are newly not its declaration's.

Run Python as `.venv/Scripts/python` from the repository root (Git Bash; forward slashes).

## 1. Mechanical pass first (minutes, no model)

```sh
python tools/hub_migrate.py --version 2.1.0 --header-redeclared      # every symbol with one include/ header
python tools/hub_migrate.py --version 2.1.0 fn_<addr> [fn_<addr>...]  # chosen symbols
python tools/hub_migrate.py --version 2.1.0 fn_<addr> --header include/base/X.h --declaration "T fn_<addr>(...);"
python tools/hub_migrate.py --version 2.1.0 --plan batch.tsv           # many chosen signatures at once
```

`python tools/decl_plan.py --write` writes such plans itself (`build/decl_plans/chunk_N.tsv`): for every
symbol in more than one file it picks the unit header (`tools/tu_map.py`, `tools/data_map.py`) and the
signature, and counts the symbols it leaves out, by reason. `--grown origin/main` plans only what a branch
added, which is the step between a batch of new files and its pull request.

For a batch, write the chosen signatures as a plan (`SYMBOL<TAB>include/x.h<TAB>declaration`, one per
line; the declaration may be left out when the header declares the symbol already) and run it once:
one baseline, one rebuild and a few narrowing rounds instead of one run per symbol, with the same
result. With a plan the tool also lists the generated sources that include a header it edits and
rebuilds the smallest two per header (`--generated-sample N`): a header type that clashes with a
generated declaration (reflected type globals are `CType` there) shows there instead of only in
`check.py`.

For each symbol it deletes every local declaration, adds the `#include`, rebuilds each touched file
and every hand-written includer of a touched header with `tools/trymatch.py`, and keeps only the
files whose output is byte-identical to before; the rest are put back and printed as
`LEFT <file> <first compile error or differing line>`. Those lines are the hand worklist. A file that
does not compile before the edit is `BROKEN` and never touched: fix that first.

## 2. Hand pass on the LEFT files

Recipe: CONTRIBUTING.md, "Migrating a hub to its canonical header". The usual causes and fixes:

- `no matching function for call` / `assigning to 'void *'`: the file's placeholder type differs
  from the header's. Rename the placeholder to the real type (`struct UnkFoo;` becomes the header's
  type, a `void*` member becomes `T*`), or cast at the call only when the object really is something
  else. Retype a getter that feeds the call and run `decl_check.py --symbol` on it: retype its other
  declarations too, or list them as follow-ups.
- `reference to 'X' is ambiguous`: the file defines its own `struct X` with the header's name.
  Delete the local struct if the header's layout covers the fields the file touches (check the
  offsets), else rename the local one.
- Output changed but compiled: a wrong-typed declaration was load-bearing (a `bool` vs `u32`
  return folds differently). Keep the local declaration, annotate why, and list the file.

Verification is `tools/baseline.py`, never by hand:

```sh
python tools/baseline.py save    --tag <step> --symbol fn_<addr> [--includers include/base/X.h]
# edit
python tools/baseline.py compare --tag <step>          # UNCHANGED / CHANGED with the differing lines
python tools/baseline.py restore --tag <step>          # puts back every file compare found changed
```

`save` also copies each source, so `restore` is exact. A `.h` in the selection expands to its
hand-written includers; objects go to `build/baseline/obj/`, so parallel agents never collide.

## 3. Rules for several agents in one checkout

- Before the agents start, the orchestrator saves `baseline.py save --tag <batch>-full --all` (every
  hand-written source, two minutes) and compares it once they are done: the whole batch's evidence,
  includers of every edited header included.
- Each agent owns the files that declare its symbols plus the headers it edits; file sets of
  parallel steps must be disjoint (`decl_check.py --symbol` lists them; overlapping steps run in
  sequence). Edit a header under `include/` with a minimal insertion, re-reading it just before.
  New headers are free. `baseline.py restore` always names the files (only the agent's own): another
  agent's mid-edit file can show as CHANGED in a compare; check it again later, never restore it.
- With `hub_migrate.py --declaration`, the tool adds the prototype after its baseline; never add the
  declaration by hand first (files with a conflicting local copy would no longer compile before the run).
- Agents never run `check.py`, `verify.py`, a generator, or a git command that changes the tree or
  the index. They report `files_changed`, headers included by generated code
  (`src/clones`, `src/reflection*`, `src/lifted`, `src/tables`) and a commit message.
- The orchestrator reruns the generators named (`gen_clones.py`, `gen_statics.py`, `gen_tables.py`,
  `gen_lift.py --prune`), runs `check.py --version 2.1.0 --update` once (no `REGRESSION`, `DUPLICATE`,
  `EQUIV`, `REFS` or `RELOC` line; an `EQUIV` line means a header change altered the code of a function
  that was verified equivalent: run `equiv_test.py --write` on its file again), `decl_check.py --strict
  --base origin/main` (PASS), `name_check.py --version 2.1.0 --strict`, `policy_check.py --base origin/main` and
  `port_sweep.py --version 2.1.0 --jobs 8`, and commits per step with its session's own attribution lines. After any
  `decl_plan.py --write` and `hub_migrate.py` run, the full `check.py` is not optional: the migration
  skips files that already fail to compile.
- The orchestrator is the coordinator of the batch: it opens the pull request (the template's headings)
  and merges it once the required checks are green, with `gh pr merge <n> --merge`, never `--admin`.

## 4. What to pick

- `tools/decl_check.py --list`: conflicting signatures first (they can never link), then symbols
  with many identical copies (one header declaration removes them all mechanically).
- `tools/hubs.py --version 2.1.0 --boot`: unmatched hubs by unmatched callers; a small `U` hub
  (under ~150 B) is worth matching first so the canonical signature comes from a definition
  (`.claude/skills/dread-match`).
- New sources go under `src/<subsystem>/` named by `tools/tu_map.py --function <addr>`; never a new
  address-named directory. Class getters are already in the generated `UnkClassGetters.h`.

Hard rules stay: no game data, listings or decompiler output in tracked files (`tools/ghidra/decompile.py`
is for reading); placeholder names unless reflection, strings or an existing header give the real one.