# dread-decomp

A full, human-readable decompilation of Metroid Dread (Nintendo Switch) and a true source port, modelled on
PrimeDecomp and zeldaret/botw. This file is the rules, briefly. Why each rule exists, with the owner's words and
dates, is in `docs/reference.md`, "Decisions made"; read it when a rule seems to conflict with the work.

Read next: `docs/HANDOFF.md` (current state and next steps; one screen; only the merging session edits it),
`docs/PORT_PLAN.md` (the route to a running PC build), `docs/reference.md` (repository map, rebuilding,
toolchain, decisions, gotchas), `docs/JOURNAL.md` (history and the index of matching findings; search it, don't
read it whole). Forks: `CONTRIBUTING.md`. The maintainer: `docs/maintaining.md`. Outside projects:
`docs/references.md`; add any newly used or referenced project there.

## Purpose and completion

- **Goal.** Anyone builds a native port from their own files (keys, ROM, filesystem); the project hosts none of
  them. A full source port with no translation layer in the product: the semantic lifts the port runs today are
  scaffolding, reported in `docs/HANDOFF.md` as "lifted code still in the port", and must shrink to zero.
- **The metric is readable, hand-typed code** (`tools/progress.py --origin`). Generated code, `src/lifted`
  included, verifies correctness and does not count; it must eventually be replaced.
- **Proof tiers.** 1, *matching*: a byte match, tried first for every function. 2, *verified equivalent*: the
  difference is registers or instruction order only (`trymatch.py`'s `KIND` says registers) and
  `tools/equiv_test.py` passes (verdicts in `data/equivalence.csv`; `check.py` fails a stale passing verdict);
  it counts as done and owes its match to a permuter (`tools/permute.py`), not to model attempts.
  3, *unverified*: anything else, including a `NON_MATCHING` function without a current passing verdict. It
  does not count.
- **Order: behaviour, then readability, then bytes.** Among texts that behave as the original does, keep the
  readable one even when it does not match: a function that matches only with a union around a member, an
  empty barrier or an inlining attribute is committed readable, as tier 2. A text that reads better but behaves
  otherwise is never kept ("faithful first", below).
- **Complete** = byte-matching and readable; **done** = matching or verified equivalent (the interim figure).
  `tools/progress.py --quality` reports the two qualities apart. A template family (the reflection boilerplate)
  counts as hand-written once the template reads as source: no `fn_<addr>` or `Unk` names in its interface,
  each use names the class and its members with no addresses, one line per field. Today's do not yet.
- `nn::atk` is decompiled too, as its own library (`docs/reference.md`).

## Hard rules (never break these)

- **No game data in git.** Never commit, copy into tracked files, or print in full any game file, asset,
  assembly listing, decompiler output dump, key, or firmware. Game data lives only in `orig/` and `build/`,
  which are both gitignored.
- **Our own code only.** Source under `src/` and `include/` is our own C++ reconstruction. Never paste Ghidra
  or other decompiler output into it. Read any disassembly or decompiler output you need; copy none of it.
- **No downloads of protected material.** Never download game images, keys, firmware, or Nintendo SDK files.
  Use only what the user supplies locally.
- **No distribution.** Never produce or publish release binaries that contain game code.
- **Ask, don't guess.** If a step needs an input the user hasn't provided (paths, versions, tools), stop and ask.
- Short excerpts (a few instructions or one function) may be shown in conversation to discuss a match. Never
  write them into tracked files, commit messages or pull request descriptions, including the output of
  `tools/ghidra/decompile.py`. Describe a difference in words (the `KIND` line, the registers, the callee by
  address).
- The pre-commit hook (`tools/hooks/pre-commit`) rejects files over 1 MB,
  `.nso/.nro/.nca/.nsp/.xci/.nsz/.xcz/.keys/.elf`, and anything under `orig/` or `build/`. Never bypass it with
  `--no-verify`.

## Layout

| Path | Tracked | Contents |
|---|---|---|
| `orig/<version>/exefs/`, `orig/<version>/romfs/`, `orig/<version>/main.elf` | no | the user's dump (`tools/setup.py`) and the decompressed main NSO |
| `build/` | no | Ghidra projects and exports, compiled objects, diff reports |
| `tools/` | yes | Python tooling; `tools/ghidra/` headless Ghidra scripts; `tools/hooks/` git hooks; `tools/dread/` shared helpers |
| `src/`, `include/` | yes | reconstructed C++17 game code; shared declarations and layouts in `include/` (one per symbol) |
| `drafts/` | yes | hand-written functions that neither match nor are verified (`drafts/INDEX.csv`); not built or counted |
| `lib/` | yes | vendored Lua 5.1.4, dlmalloc 2.8.4, one libc++ sort; submodules `lib/nnsdk`, `lib/musl` (headers only) |
| `data/` | yes | metadata only, never code bytes: `functions/main_XX.csv`, `names.csv`, `tu_map.csv`, `tu_status.csv`, `equivalence.csv`, reflection tables, toolchain pins |
| `docs/` | yes | `HANDOFF.md`, `PORT_PLAN.md`, `reference.md`, `commands.md` (generated), `JOURNAL.md`, `recon.md`, `compiler.md`, `maintaining.md`, `references.md` |
| `port/` | yes | PC port layer (Route B): CMake project, the port's own `nn::` runtime; `port/README.md` |

## Commands

Every tool's usage is in `docs/commands.md` (generated by `tools/gen_commands.py`; CI fails when it is stale).
`python -m tools <tool> [args]` runs any of them under the repository's venv. A new clone's full sequence and the
toolchain are in `docs/reference.md`. Machine-specific paths go in `CLAUDE.local.md` (gitignored).

```sh
git config core.hooksPath tools/hooks && git submodule update --init   # once per clone
python tools/check.py --version <ver> [--update]       # build src/, derive statuses; must pass before committing src/
python tools/verify.py --version <ver> [--regen gen_x] # generators + check --update + progress + policy + decl_check
python tools/trymatch.py --version <ver> <file.cpp> -v # build and diff one source (RELOC lines; KIND: what to fix first)
python tools/hubs.py --version <ver> --boot | --ready --boot | --callers <fn>   # what to match next
python tools/gen_lift.py --version <ver> --best ADDR,...   # closest lifted draft -> build/lift_best/ (hand-matching start)
python tools/tu_map.py --function <addr|name>          # the unit a function belongs to, and its file name
python tools/tu_status.py [--list partial | --unit <U>] # status per translation unit
python tools/rename.py <addr> <Name> --reason "..." [--dry-run]   # give a function a readable name (data/names.csv)
python tools/data_map.py --address <addr>              # the unit a global belongs to
python tools/gen_fields.py --class <Class>             # a reflected class's members by offset, bases included
python tools/decl_check.py --strict --base origin/main # one declaration per shared symbol (CI)
python tools/struct_check.py --base origin/main        # no type name with a second, different definition (CI)
python tools/fakematch.py --base origin/main [--list]  # fake-match forms a branch adds; review before a PR
python tools/equiv_test.py --version <ver> <file.cpp> [--write]   # tier 2: is a registers-only non-match equivalent?
python tools/ref_check.py --version <ver> [<file.cpp>]  # wrong callees and globals in functions that do not match
python tools/permute.py --version <ver> <file.cpp>[:ADDR] [--write]   # free try at a registers-only near-match
python tools/drafts.py --version <ver> add|grade|promote ...   # drafts/: file a non-match, regrade, move to src/
python -m unittest discover -s tools/tests -t .       # tool tests, no game data (CI)
python tools/hub_migrate.py --version <ver> --plan PLAN.tsv   # mechanical header migration; .claude/skills/dread-hubs
python tools/ghidra/decompile.py --version <ver> <addr|name>  # read Ghidra's view (never commit it)
python tools/progress.py --slice boot [--handoff]      # progress; --handoff rewrites docs/HANDOFF.md's table
python tools/progress.py --quality                     # byte-matching and readable, as two qualities
python tools/port_link.py --version <ver> [--lifted build/port/lifted]   # port link and boot_missing.csv
port/docker/run.sh python3 tools/port_link.py --version <ver> --stdlib libcxx9 --objects build/port/linux/sweep --out build/port/linux/link --executable  # Linux executable (port/README.md)
```

Target version: **2.1.0** (update v327680); `main` NSO build ID `646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`;
title ID `010093801237C000`. Project compiler: upstream Clang 9.0.1 with the flags in `tools/buildcfg.py`
(`docs/compiler.md`).

## Pushing and merging

- Remote: https://github.com/var-flotsam/dread-decomp (public, MIT). Pushing a `claude/*` feature branch and
  opening, editing or assigning its pull request needs no asking; any other branch or repository needs the
  user's OK. Never push to var-flotsam/dread-decomp-archive (rollback only) or var-flotsam/MPT-Decomp (another
  project).
- **The coordinator of a piece of work merges it** once `policy`, `compile` and `check` are green, with
  `gh pr merge <n> --merge`. Never `--admin`: a pull request GitHub will not merge without it is not ready; say
  why and stop. The owner merges major, sweeping revisions of `main`; when unsure whether a change is
  sweeping, ask. A function is `O` only when its bytes match and `check.py` passes.
- `.claude/settings.json` allowlists the everyday commands; a user-level or `.claude/settings.local.json` rule
  overrides it.

## Conventions

- **Rule freeze (owner, 2026-11-14, until 2026-11-28).** No new rule, gate or CI check unless a defect has
  actually reached `main`; then the smallest fix that stops it. Measure functions matched per day instead.
- Python 3 (standard library where possible) for tools, CMake + Ninja for builds, C++17 for game code. Tool
  outputs are always regenerated, never hand-edited.
- **Names.** Unsure names keep a placeholder: `fn_<addr>`, `UnkStruct_0x40`, `field_0x18`, `unk_<addr>`. Real
  names come from RTTI, reflection, strings or mercury-engine-data-structures (the `name_source` column); for
  `nn::atk` also from its Wii U predecessor and games shipped with symbols, read, never copied, each listed in
  `docs/references.md`. A name inferred from what a function does goes in `data/names.csv` through
  `tools/rename.py` only, never by hand; a rename changes no code (`check.py --update` shows no REGRESSION; CI
  runs `rename.py --check`). Rename a class's functions together, free functions by unit. `refl_` and
  `vf_<slot>` mark inferred roles, not Mercury's names.
- **The function list** is `data/functions/main_XX.csv` (one per MiB), generated by `tools/gen_functions_csv.py`:
  `address,size,name,name_source,subsystem,status`. `name_source` is `symbol|library|rtti|reflection|string|
  inferred|unknown`. Status codes are zeldaret/botw's: `O` matching, `m` minor, `M` major, `W` in progress, `U`
  not decompiled, `L` library. `check.py --update` derives `O/m/M/W` from `src/`. A subsystem ending in `?` was
  inferred from neighbours.
- **Non-matching functions.** `// NON_MATCHING(minor): <reason>` (same behaviour only) or
  `// NON_MATCHING(major): <reason>` directly above the definition. A minor one that refers to something the
  original does not, or lacks something it has, names it by address or import with why it is harmless
  (`check.py` fails it otherwise: `REFS` lines).
- **Matching standard.** Aim for `O`; time-box each function to about 30 to 60 minutes. Then a registers-only
  difference gets `permute.py` and `equiv_test.py` and stays in `src/` as minor when it passes; anything else
  new goes to `drafts/` (`tools/drafts.py add`) unless the port needs it, in which case keep the closest version
  marked minor or major with the difference spelled out. Reflection boilerplate (`refl_GetClass`,
  `refl_InitClass`) must reach `O` through a shared template or macro.
- **Check before committing `src/`:** `python tools/check.py --version <ver>`: no `REGRESSION`, `DUPLICATE` or
  hand-written `RELOC` line. Every callee, global and GOT slot a matched function names must be the original's.
- **Readable source.** Code no developer would write is an optimization not undone yet: look for the inlined
  helper (written once for every caller), the `switch` turned into a table or chain, the unrolled loop, the
  duplicated tail. A barrier, offset cast, `volatile` or empty `if` usually stands for a missing helper, a wrong
  field type or two variables merged into one. Empty barriers (`asm("" : "+r"(x))`, `asm volatile("" ::: "memory")`)
  only when nothing else reproduces the ordering, with a one-line comment directly above saying what they
  preserve; inline asm with instructions is never allowed. A `goto` only where the original plausibly had one,
  with a comment (the existing ones stay until their functions are next worked on). `tools/fakematch.py` lists
  these forms; `--unneeded-barriers` finds barriers that change nothing.
- **No unset values.** A local never set is never read, passed or returned. An argument register the original
  leaves unloaded is an `UnkNoArg` parameter (`include/base/UnkNoArg.h`) passed `UnkNoArg()`; a parameter the
  callee lacks comes out of its declaration. A variable the original itself leaves unset on a path stays, with
  "unset in the original" in a comment at its declaration. `fakematch.py --unset --strict` runs in CI.
- **Faithful first.** When a rule here would make the source do something the original does not, keep what the
  original does and say so in a comment (precedent: `src/game_lo/fn_7100219588.cpp`).
- **One declaration per shared symbol**, in a header under `include/`, with the types its definition has; a
  `.cpp` declares only what it alone uses. `decl_check.py --strict --base origin/main` and
  `struct_check.py --base origin/main` (no type name gains a second, different definition) run in CI. Migrating a
  hub: `tools/hub_migrate.py`, then `CONTRIBUTING.md`, "Migrating a hub", verified with `tools/baseline.py`
  (`.claude/skills/dread-hubs`).
- **Files by unit.** New sources go under `src/<subsystem>/`, named for the class or unit
  (`tools/tu_map.py --function <addr>`); no new address-named directories.
- **What to match next:** `tools/hubs.py --version <ver> --boot`, then `--ready`.
- **A layout batch is one class:** its sources, its one layout in a header of its own (unit headers stay
  declarations only), one pull request. Recipe: `include/game/UnkXParasiteAI.h` (journal, 2026-11-13).
- **Bulk agent runs** on the lean kit need no go-ahead of their own when they keep the shape in
  `tools/batch/README.md` ("Batches") and pass every gate; each worker takes several functions of one unit.
- **Logs go to `docs/JOURNAL.md`**, newest first, reusable findings in its index. Feature branches leave
  `docs/HANDOFF.md` and `data/tu_status.csv` alone; the merging session regenerates them
  (`tools/progress.py --slice boot --handoff`, `tools/status_card.py --write`, `tools/tu_status.py --write`).
- **Commits and pull requests.** Commit messages say what changed, with no phase prefix. A pull request is one
  coherent piece of work (a class, a hub cluster, a tool change), with no minimum size.
- **Generated sources** (`src/reflection/`, `src/reflection_types/`, `src/reflection_statics/`, `src/clones/`,
  `src/tables/`, `src/lifted/`, `include/base/reflection/UnkClassGetters.h`, `docs/commands.md`; listed in
  `.gitattributes`) are regenerated by their `tools/gen_*.py`, never hand-edited. A `RELOC` line in generated code
  is a generator regression: fix the generator. Include `UnkClassGetters.h` for a class getter instead of
  declaring one.

## Status

Phases 1 to 3 (dump, recon, compiler) are done. Phase 4, a clean, readable tree that runs on PC by Route B, is in
progress: `docs/HANDOFF.md` and `docs/PORT_PLAN.md`.
