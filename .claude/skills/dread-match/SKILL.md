---
name: dread-match
description: Hand-match Metroid Dread functions in var-flotsam/dread-decomp one at a time in a compile-and-diff loop, and place each result by the proof tiers (match, verified equivalent, draft), with the checks a pull request must pass.
---

# dread-match

Hand-matching for `var-flotsam/dread-decomp`: one session, one function at a time, a pull request per
cluster. `CLAUDE.md` loads by itself and its hard rules apply; this is the working procedure under them.
(A batch of worker agents is a different thing: `tools/batch/lean/`, and it needs the owner's go-ahead.)

Run Python as `.venv/Scripts/python` from the repository root (Git Bash; forward slashes). Long commands
(`tools/verify.py`, `tools/check.py`: 8 to 13 minutes) go in the background; don't edit `src/` or `include/`
while one runs.

## Inputs (ask once if missing)

- What to match: a unit, a class, or a list from `python tools/hubs.py --version 2.1.0 --ready --boot`
  (functions on the boot path whose callees are done).
- The branch: `claude/<topic>-<YYYYMMDD>`, from `origin/main`.

## Hard rules (short form)

- No game data in git: nothing from `orig/` or `build/`, no disassembly, no `trymatch -v` or Ghidra output
  in files, commits or pull request text. Scratch work goes in `build/` (gitignored).
- `src/` and `include/` are your own C++. Placeholder names (`fn_<addr>`, `g_unk_<addr>`, `field_0x18`,
  `UnkFoo`) unless RTTI, reflection, strings or mercury-engine-data-structures give the real one.
- Never `--no-verify`. Never edit a generated directory (`src/reflection*/`, `src/clones/`, `src/lifted/`,
  `src/tables/`): change its generator.

## Where the function goes

- `python tools/tu_map.py --function <addr>` prints the unit and the folder for a new source
  (`src/<subsystem>/<Class>/`). Name the file for the class or unit and what the function does. No
  address-named directories.
- **Declarations: one per shared symbol.** Before declaring a callee or global:
  `python tools/decl_check.py --symbol fn_<addr>`. If a header under `include/` declares it, include that
  header. If other sources declare it, it belongs in a unit header: add it there and remove the copies
  (`.claude/skills/dread-hubs`), don't add another copy. Only a symbol this file alone uses is declared in the
  file. Where a source defines the callee, declare it with the definition's types.
- A global reached through a GOT slot is declared as its target (`g_unk_<target address>`), never named
  after the slot.

## Loop

```sh
python tools/trymatch.py --version 2.1.0 <file.cpp>       # score per function, RELOC and KIND lines
python tools/trymatch.py --version 2.1.0 <file.cpp> -v    # side by side; read only
python tools/gen_lift.py --version 2.1.0 --best <addr>    # the closest lifted draft, as a starting point to rewrite
python tools/nearcopy.py --version 2.1.0 <LO> <HI>        # U functions that are near copies of matched ones
python tools/ghidra/decompile.py --version 2.1.0 <addr>   # Ghidra's view; read only, never copied
```

Fix in the order the `KIND` line gives: calls, control flow, conditions, operations, registers. A `RELOC`
line means a callee, global or GOT slot is not the original's: fix that before anything else, whatever the
score. Change one thing per compile. A near copy of a matched function still has its own callees and
globals: declare the copy's, not the original's.

What usually moves a function, all of it ordinary source: the type and signedness of a local or a field
(`u64` loop indices), a parameter by reference or by value, the order of declarations (stack order follows
it) and of statements, a comparison written the other way round, an inline helper the original clearly had.

**Readable first.** Don't add a barrier (`asm("" : "+r"(x))`), `volatile`, an empty `if`, an inlining
attribute, a union around a member, an untyped store or an offset cast only to change what the compiler
emits. One of these usually stands for a missing helper, a wrong field type or two variables merged into one:
look for that. `python tools/fakematch.py --base origin/main --list` shows what the branch added. Readability
outranks a match: if the function matches only with one of them, the version without it is the one to keep.

## When it does not match (about 30 to 60 minutes a function)

What the last `KIND` line says decides where the function goes.

1. **`registers`** (the same instructions in another order or with other registers):
   - `python tools/permute.py --version 2.1.0 <file.cpp>:<addr> --write` tries source reorderings for a
     couple of minutes of CPU. About one in nine becomes a match; read what it changed.
   - Still registers-only: put `// NON_MATCHING(minor): <what differs>` above the definition and run
     `python tools/equiv_test.py --version 2.1.0 <file.cpp> --write`. A `pass` makes it *verified equivalent*:
     it stays in `src/` and counts as done (the row in `data/equivalence.csv` is the proof; `check.py` fails
     it when the code changes).
2. **Anything else, or no `pass`:** it does not count. Keep it in `src/` as `NON_MATCHING(minor)` only if you
   are sure the behaviour is identical, or as `NON_MATCHING(major)` with the difference spelled out, when the
   port needs the function. Otherwise it is a draft:
   `python tools/drafts.py --version 2.1.0 add <file.cpp> <addr> --from <topic> --note "<what differs>" --remove`
   files it under `drafts/` with a graded row, where nothing builds it and the work is not lost.
3. A `NON_MATCHING(minor)` function that calls or refers to something the original does not (or lacks
   something it has) names it in its note, by address or import name, with why it is harmless.
   `python tools/ref_check.py --version 2.1.0 <file.cpp>` lists what differs; `check.py` fails an unnamed one.

Reflection boilerplate (`refl_GetClass`, `refl_InitClass`) is the exception: it must match, through the
shared template.

## Before the pull request

Every 15 to 30 functions, or a hub cluster:

```sh
git fetch origin main && git merge origin/main
python tools/verify.py --version 2.1.0                    # generators, check --update, progress, policy, decl_check
python tools/decl_check.py --strict --base origin/main    # no new copy, conflict or type difference
python tools/name_check.py --version 2.1.0 --strict
python tools/port_sweep.py --version 2.1.0 --jobs 8                       # the sources still compile for x86-64
python -m unittest discover -s tools/tests -t .           # when a tool changed
```

`verify.py` must end in PASS: no `REGRESSION`, `DUPLICATE`, `EQUIV`, `REFS` or hand-written `RELOC` line. On
a conflict in `data/functions/`, take `origin/main`'s files and let `check.py --update` derive the statuses
again; never merge them by hand. Then:

- Commit with a `Phase 4.x:` prefix. Add an entry at the top of `docs/JOURNAL.md` (what matched, what did
  not and why, anything reusable). Leave `docs/HANDOFF.md` alone unless you are the session that merges.
- `git push -u origin <branch>`, then open the pull request with the template's headings (What this changes
  / Progress / Checklist); give matching and done before and after from `tools/progress.py`.
- **Who merges:** the coordinator of the work. If this session is, merge once the required checks are green
  with `gh pr merge <n> --merge`, never `--admin`; then regenerate the handoff table and the status card
  (`tools/progress.py --slice boot --handoff`, `tools/status_card.py --write`) on the next branch. A sweeping
  change of `main` is the owner's to merge.

## Report

Functions matched, verified equivalent, filed as drafts (with the common reasons), progress before and
after, and the pull request links, in one short message.
