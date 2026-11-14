# dread-decomp

Start with `docs/HANDOFF.md` for current state and next steps (one screen; only the merging session edits it),
`docs/PORT_PLAN.md` for the route to a running PC build, `docs/reference.md` for stable facts (repository map,
rebuilding, decisions, gotchas), and `docs/JOURNAL.md` for history and the index of matching findings (long;
search it, don't read it whole). Contributors working from a fork: `CONTRIBUTING.md`;
the maintainer: `docs/maintaining.md` (publication, branch rules, merging, the self-hosted check job). Outside
projects are listed in `docs/references.md`. Add any newly used or referenced project there.

A full, human-readable decompilation of Metroid Dread (Nintendo Switch) and a true source port,
modelled on PrimeDecomp and zeldaret/botw.

## Purpose (owner, 2026-11-05)

The project is complete when anyone can build a native port for the desired operating systems from
their own files (keys, ROM, filesystem). The project hosts none of those files and never hands them
to a user, to stay clear of piracy. It is deliberately different from the static recompilation
"Metroid Dread Recomp": a full source port with no translation layer in the product. The semantic
lifts the port runs today are scaffolding that hand matching replaces; `docs/HANDOFF.md` reports
them as "lifted code still in the port", a number that must shrink to zero. The point is to give the
community a tool that deepens their understanding of a game they already own: speedrunners reading
the real code to find new methods, developers making mods that would otherwise be impossible.
Readable, hand-typed code is the completion metric (`tools/progress.py --origin`); bytes matched by
generated code verify correctness but do not count toward the goal. The rule that follows: read any
disassembly or decompiler output you need, copy none of it into tracked files.

**Completion (owner, 2026-11-11).** There are two measures. Decompilation is complete when every function
of the game is hand-written, human-readable and proven equivalent to the original; it is measured over all
of the game's code (`L` excluded). Generated code, `src/lifted` included, is a stepping stone and must
eventually be replaced. The proof has tiers:
1. *Matching.* A byte match, tried first for every function and still the direction for all of them.
2. *Verified equivalent.* The difference from the original is in register choice or instruction order only
   (`trymatch.py`'s `KIND` line says registers), and a machine test runs the original and ours on the same
   inputs with the same writes and calls. It counts as done; a permuter, not model attempts, turns it into
   a match later.
3. *Unverified.* Anything else that does not match. It does not count.

Readability outranks a match: when a function matches only with a union around a member, an empty barrier
or an inlining attribute, commit the readable version as tier 2 instead.

**The order of the two (owner, 2026-11-13).** Behaviour comes first, then readability, then bytes. Among texts
that behave as the original does, the readable one is kept even when it does not match (above). A text that
reads better but behaves otherwise is not kept: "faithful first" (Conventions) wins. The precedent is
`src/game_lo/fn_7100219588.cpp`, which reads two locals the original program also leaves unset rather than
setting them.

**Complete** (owner, 2026-11-13: "The project will not be considered to be absolutely done until the
decompilation functions identically and is human readable"; "byte matching and readable are two different
qualities but both are needed. Identical behavior but byte mismatch are okay for a later date"). A function
is complete when it is byte-matching and readable. *Done* (matching, or verified equivalent) is the interim
figure: a verified-equivalent function still owes its match. `tools/progress.py --quality` reports the two
qualities apart.

The machine test is `tools/equiv_test.py` (since 2026-11-12): its verdicts are in
`data/equivalence.csv`, and `tools/check.py` fails a passing verdict that is not about the code in the tree any
more. A `NON_MATCHING` function without a current passing verdict is tier 3. A general permuter does not exist;
`tools/permute.py` tries the cheap reorderings (it matched about one registers-only function in nine on
2026-11-12). A family
stamped out of one template or macro (the reflection boilerplate) counts as hand-written once the template
reads as source: no `fn_<addr>` or `Unk` names in its interface, each use names the class and its members
with no addresses, and a field takes one line. Today's reflection templates do not meet that yet. The port
is complete when no lifted code is left in it. `nn::atk` is decompiled too, as its own library
(`docs/reference.md`, "Decisions made").

## Hard rules (never break these)

- **No game data in git.** Never commit, copy into tracked files, or print in full any
  game file, asset, assembly listing, decompiler output dump, key, or firmware.
  Game data lives only in `orig/` and `build/`, which are both gitignored.
- **Our own code only.** Source under `src/` and `include/` is our own C++
  reconstruction. Never paste Ghidra or other decompiler output into it.
- **No downloads of protected material.** Never download game images, keys, firmware,
  or Nintendo SDK files. Use only what the user supplies locally.
- **No distribution.** Never produce or publish release binaries that contain game code.
- **Ask, don't guess.** If a step needs an input the user hasn't provided (paths,
  versions, tools), stop and ask.
- Short excerpts (a few instructions or one function) may be shown in conversation
  to discuss a match. Never write them into tracked files. That includes the output of
  `tools/ghidra/decompile.py`, which is for reading only.
- **None in commit messages or pull request descriptions either (owner, 2026-11-14).** Assembly,
  disassembly and decompiler output stay out of both: they are as public and as permanent as a tracked
  file. Describe a difference in words (`trymatch.py`'s `KIND` line, the registers, the callee by address).

The pre-commit hook (`tools/hooks/pre-commit`) enforces part of this. It rejects
files over 1 MB, `.nso/.nro/.nca/.nsp/.xci/.nsz/.xcz/.keys/.elf`, and anything under
`orig/` or `build/`. Never bypass it with `--no-verify`.

## Layout

| Path | Tracked | Contents |
|---|---|---|
| `orig/<version>/exefs/`, `orig/<version>/romfs/` | no | user's dump, copied by `tools/setup.py` |
| `orig/<version>/main.elf` | no | decompressed main NSO |
| `build/` | no | Ghidra projects and exports, compiled objects, diff reports |
| `tools/` | yes | Python tooling; `tools/ghidra/` holds headless Ghidra scripts; `tools/hooks/` holds git hooks |
| `src/`, `include/` | yes | reconstructed C++17 game code; shared declarations and layouts live in `include/` (one per symbol) |
| `lib/` | yes | vendored upstream Lua 5.1.4 (`lib/lua`), dlmalloc 2.8.4, one libc++ sort; submodules `lib/nnsdk`, `lib/musl` (headers only) |
| `data/` | yes | metadata only: `hashes.json`, `functions/main_XX.csv`, `names.csv`, `reflection_types.csv`, `reflection_fields.csv`, `tu_map.csv`, `tu_status.csv`, `data_map.csv`, `equivalence.csv`, `library_symbols.csv`, `toolchains.json`, `sources.json`; never code bytes |
| `docs/` | yes | `HANDOFF.md` (state), `PORT_PLAN.md`, `reference.md` (stable facts), `commands.md` (generated), `JOURNAL.md` (history), `recon.md`, `compiler.md`, `maintaining.md`, `references.md` |
| `port/` | yes | PC port layer (Route B): CMake project (`dread_game`, `dread_libs`, `dread_runtime`, `dread`, `sweep`, `link_report`), the port's own `nn::` runtime under `runtime/`, `sample_duplicates.txt`; `port/README.md` has the layout, the sweep and the link |

## Setup and commands

Every tool's purpose and usage lines are in `docs/commands.md`, generated from the docstrings by
`tools/gen_commands.py` (CI fails when it is stale); `python tools/<tool>.py --help` has the rest.
A new clone's full sequence is `docs/reference.md`, "Rebuilding everything from scratch".
`python -m tools <tool> [args]` runs any of them under the repository's venv from any Python (`python -m
tools` lists them). Shared helpers (the function list loader, repository paths) are in `tools/dread/`.
The ones used every day:

```sh
git config core.hooksPath tools/hooks && git submodule update --init   # once per clone
python tools/check.py --version <ver> [--update]       # build src/, derive statuses; must pass before committing src/
python tools/verify.py --version <ver> [--regen gen_x] # generators + check --update + progress + policy + decl_check
python tools/trymatch.py --version <ver> <file.cpp> -v # build and diff one source (RELOC lines; KIND: what to fix first)
python tools/hubs.py --version <ver> --boot | --ready --boot | --callers <fn>   # what to match next
python tools/gen_lift.py --version <ver> --best ADDR,...   # closest lifted draft -> build/lift_best/ (hand-matching start)
python tools/tu_map.py --function <addr|name>          # the unit a function belongs to, and its file name
python tools/tu_status.py [--list partial | --unit <U>] # status per translation unit: units by state, one unit's functions
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

Target version: **2.1.0** (update v327680). The `main` NSO build ID is
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`. Title ID is `010093801237C000`.

Remote: https://github.com/var-flotsam/dread-decomp (public; MIT
license; see `docs/maintaining.md`). The pre-2026-10-18
history, with the original PRs, is kept in the private var-flotsam/dread-decomp-archive (rollback
only; don't push there). (var-flotsam/MPT-Decomp is a separate project, a Metroid
Prime Trilogy recomp. Never push this repo there.)

**Pushing and merging.**
- Pushing a `claude/*` feature branch to this repository and opening, editing or assigning its pull
  request is allowed without asking. Any other branch, and every other repository, still needs the user's OK.
- **Who merges (owner, 2026-11-11).** The coordinator of a piece of work merges its pull requests: the
  session that coordinates that work, once the required checks (`policy`, `compile`, `check`) are green,
  with `gh pr merge <n> --merge`. Never pass `--admin`: the bypass also skips the required checks, and the
  ruleset no longer needs it (the last-push approval, which one account can never give, is off since
  2026-11-11). A pull request GitHub will not merge without it is not ready: say why and stop. The owner
  merges major, sweeping revisions of `main`. A session that is not the coordinator of the work does not
  merge it; when it is unclear whether a change is sweeping, ask the owner. The gates don't change:
  `check.py` must pass and a function is `O` only when its bytes match.
- `.claude/settings.json` allowlists these commands so they don't prompt; a user-level or
  `.claude/settings.local.json` deny or ask rule overrides it.

## Local environment

Each contributor supplies their own dump, keys and tools. Machine-specific paths (where the
dump, keys and emulator live) go in `CLAUDE.local.md`, which is gitignored and read by Claude
Code alongside this file. Never copy anything from those locations into the repo.

Toolchain:
- Python 3.12, with packages in the repo venv `.venv` (`.venv/Scripts/python`).
  Python deps are `lz4`, `mercury-engine-data-structures`, `capstone`, `pyelftools`, and
  `pyghidra` (installed from Ghidra's bundled wheels, without network access).
- JDK 21 (Temurin), with Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\ghidra_12.1.4_PUBLIC`
  (headless: `support\analyzeHeadless.bat`).
- CMake 4.4, Ninja 1.13, LLVM/Clang + lld 23.1.3 (`C:\Program Files\LLVM\bin`) for inspection.
- Project compiler: upstream Clang 9.0.1 from `tools/fetch_toolchains.py`
  (`%LOCALAPPDATA%\dread-decomp\toolchains\llvm-9.0.1`; on macOS the x86_64 build under Rosetta 2 in
  `~/Library/Caches/dread-decomp`, untested on hardware; paths in `tools/hostenv.py`). Flags live in `tools/buildcfg.py`:
  `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float -fno-rtti
  -fno-exceptions` (evidence in `docs/compiler.md`).
- objdiff 3.8.2 GUI and CLI in `%LOCALAPPDATA%\dread-decomp\tools\`.
- Shells opened before the install may need PATH refreshed from the registry.

## Conventions

- Python 3 (standard library where possible) for tooling, CMake + Ninja for builds,
  C++17 for game code.
- Tool scripts must be reproducible. Outputs are always regenerated, never hand-edited.
- Names you aren't sure of keep an address-based placeholder: `fn_<addr>` (e.g.
  `fn_7100123456`), `UnkStruct_0x40` (size suffix), `field_0x18`, `unk_<addr>` for data.
  A real name comes from RTTI, reflection, strings, or mercury-engine-data-structures; its source is the
  `name_source` column of `data/functions/main_XX.csv`.
  For `nn::atk` only (owner, 2026-11-11), names may also be read from the decompilation of its Wii U
  predecessor and from games that shipped with symbols: reading references, never copied, each listed in
  `docs/references.md`.
- **Inferred names (owner, 2026-11-14).** A function the binary does not name may get a name worked out by
  reading what it does. It is recorded in `data/names.csv` (address, qualified name, `inferred`, a one-line
  reason; `tools/dread/names.py`), which `tools/gen_functions_csv.py` applies, so it survives regeneration;
  a real name replaces it when evidence turns up. Rename only with `tools/rename.py`, never by hand: it writes
  the map and the function list and rewrites every hand-written use (comments keep the address). A rename
  keeps the function's scope and changes no code: `check.py --update` shows no REGRESSION and only the
  renamed rows' names change in `data/functions/`; CI's `policy` job runs `rename.py --check`. Rename a
  class's functions together, with its layout batch where there is one; free functions by unit.
- The function list is `data/functions/main_XX.csv` (one file per MiB of address space,
  to stay under the 1 MB hook limit), generated by `tools/gen_functions_csv.py`. Columns are
  `address,size,name,name_source,subsystem,status`. `name_source` is one of
  `symbol|library|rtti|reflection|string|inferred|unknown` (`library` = matched against vendored
  upstream code by `tools/sigmatch.py`; `inferred` = from `data/names.csv`). `status` uses the
  zeldaret/botw codes: `O` matching, `m` non-matching (minor), `M` non-matching (major), `W` work in progress, `U` not
  decompiled, `L` library. `tools/check.py --update` derives `O/m/M/W` from `src/`; only those
  survive regeneration.
- Mark a non-matching function with `// NON_MATCHING(minor): <reason>` or
  `// NON_MATCHING(major): <reason>` on the line(s) directly above its definition. Use minor only
  when the behaviour is identical. A minor function that calls or refers to something the original
  function does not (or lacks something it has) names it in the note, by address or import name, with
  why it is harmless; `tools/check.py` fails one that does not (`REFS` lines, `tools/ref_check.py`; owner,
  2026-11-12).
- Matching standard: aim for `O` on every function, but time-box each one to about 30-60
  minutes. If it still differs: a registers-only difference gets the free try (`tools/permute.py`) and
  the machine test (`tools/equiv_test.py`), and stays in `src/` as `NON_MATCHING(minor)` when it passes.
  Anything else that is new goes to `drafts/` (`tools/drafts.py add`), not to `src/`, unless the port needs
  the function: then keep the closest version annotated `NON_MATCHING(minor)` (same behaviour) or
  `NON_MATCHING(major)`, with the remaining difference spelled out (owner, 2026-11-13; the non-matching
  functions already in `src/` stay). Exception: reflection boilerplate (`refl_GetClass`,
  `refl_InitClass`) must reach `O`, through a shared template or macro.
- Build and check: `python tools/check.py --version <ver>` (Clang 9.0.1 with the flags in
  `tools/buildcfg.py`, see `docs/compiler.md`). It must pass before committing `src/` changes: no
  `REGRESSION`, `DUPLICATE` or hand-written `RELOC` lines. A function is `O` only when its bytes match
  and every callee, global and GOT slot it names is the original's. Subsystems ending in `?` were
  inferred from neighbours.
- **Empty compiler barriers.** Inline asm with instructions in it is never allowed. An empty barrier is, when it
  is the only way to reproduce the original's ordering: the register form `asm("" : "+r"(x))` or the memory
  form `asm volatile("" ::: "memory")`. Neither emits an instruction. Put a one-line comment directly above
  it saying what it preserves (e.g. `// reads data70 again after the calls, as the original does`).
  A barrier, an offset cast, `volatile` or an empty `if` usually stands for a missing helper, a wrong
  field type or two variables merged into one: look for that first. `tools/fakematch.py` lists them, and its
  `--unneeded-barriers` finds the barriers that change no function's code when taken out.
- **Code no developer would write is an optimization not undone yet (owner, 2026-11-14; from zeldaret/botw's
  `AGENTS.md`).** When the text that matches does not look like something a developer would plausibly have
  written, the compiler's work is still in it: a helper it inlined (the same run of code in several
  functions is one helper, written once), a `switch` it turned into a lookup table or a chain of tests, a
  loop it unrolled, a tail it duplicated. Find the source form before keeping the match. It is the same
  search the barrier rule asks for, and a helper found once repairs every caller. When no such form is found
  in the time box, the rules above decide where the function goes; the note says what was tried.
- **No `goto` to force a match (owner, 2026-11-14; same source).** A `goto` is written only where the
  original source plausibly had one (a shared clean-up exit, for instance), with a comment saying so. One
  that only steers block order stands for a missing helper, loop or early return. The `goto`s already in
  `src/` stay until their functions are next worked on.
- **No unset values (owner, 2026-11-13).** A local that is never set is not read: not passed to a callee,
  not returned. Such a text has no behaviour of its own, so the compiler may make anything of it, the
  original's bytes included, and its match proves nothing. What the original does at those places has an
  honest form. A register it leaves unloaded at a call is an argument that carries nothing: the parameter
  is `UnkNoArg` (`include/base/UnkNoArg.h`, an empty class by value) and the call passes `UnkNoArg()`.
  A parameter the callee does not have comes out of its declaration. A variable the original program
  itself leaves unset on a path (an out-parameter's local written on one branch only) stays, with a
  comment at its declaration saying so in the words "unset in the original" (on its line, or in the comment
  directly above it and the declarations under that). `python tools/fakematch.py --unset` lists every read;
  with `--strict` it fails on one that is not marked so, and CI's `compile` job runs it on the whole tree.
- **Faithful first** (owner, 2026-11-13: "We want faithful to original above our invented rules"). When a
  rule of this file would make the source do something the original program does not, keep what the
  original does and say so in a comment.
- Inferred-role names use a `refl_` prefix (e.g. `CActor::refl_GetClass`) or `vf_<slot>` for
  vtable slots. These are not Mercury's real method names.
- **One declaration per shared symbol.** A function or global used by more than one file is declared
  once, in a header under `include/`, with one signature; a `.cpp` declares only what it alone uses.
  `python tools/decl_check.py --strict --base origin/main` must pass (CI runs it; since 2026-11-12 one more
  identical local copy fails too, and so does a definition whose types are newly not its declaration's:
  declare a callee with the types its definition has); `--symbol <name>` shows every copy. Types have
  the same ratchet since 2026-11-13: a struct, class or union name does not gain a second, different
  definition in another file (`python tools/struct_check.py --base origin/main`, CI). Migrating a hub to its header: `tools/hub_migrate.py` does the mechanical part and lists the
  files it leaves; the hand recipe is `CONTRIBUTING.md`, "Migrating a hub", verified with
  `tools/baseline.py` (procedure for a batch: `.claude/skills/dread-hubs`).
- **Files by unit.** New hand-written sources go under `src/<subsystem>/`, named for the class or
  translation unit (`python tools/tu_map.py --function <addr>`); no new address-named directories.
- **What to match next:** `python tools/hubs.py --version <ver> --boot` (hubs along the boot path by
  unmatched callers), then `--ready`. Progress is reported for the whole binary and the boot slice.
- **Logs go to `docs/JOURNAL.md`** (newest first, with reusable findings added to its index).
  Feature branches leave `docs/HANDOFF.md` alone; the session that merges a round updates it and
  regenerates its numbers with `tools/progress.py --slice boot --handoff`, then the README's status
  card with `tools/status_card.py --write` (each pull request shows the card in its `policy` job summary)
  and the per-unit table `data/tu_status.csv` with `tools/tu_status.py --write` (feature branches leave it
  alone too; the same job summary lists the units a pull request moves).
- Commit messages say what changed; no phase prefix. A pull request is one coherent piece of work (a class, a hub
  cluster, a tool change); there is no minimum size, since checks and merging take minutes.
- **A layout batch is one class (owner, 2026-11-13).** Work that settles how sources describe an object is
  done a class at a time: that class's sources, its one layout in a header of its own (unit headers stay
  declarations only), one pull request. The recipe is the X parasite AI component's
  (`include/game/UnkXParasiteAI.h`; journal, 2026-11-13).
- **Bulk agent runs (owner, 2026-11-14).** The hold of 2026-11-11 is lifted: a batch on the lean kit runs without
  a go-ahead of its own when it keeps the shape in `tools/batch/README.md` ("Batches") and passes every gate;
  each worker takes several functions of one unit.

## Status

Phases 1-3 are done (`docs/recon.md`, `docs/compiler.md`; the 2.1.0 dump passed `tools/verify_dump.py`).
Phase 4 is in progress (owner-approved): since 2026-10-24 the goal is a clean, readable tree that runs
on PC within months, by Route B. Current state and next steps: `docs/HANDOFF.md`; the route and its open
decisions: `docs/PORT_PLAN.md`.

- Generated sources (`src/reflection/`, `src/reflection_types/`, `src/reflection_statics/`, `src/clones/`,
  `src/tables/`, `src/lifted/`, the header `include/base/reflection/UnkClassGetters.h` and
  `docs/commands.md`) are outputs of the `tools/gen_*.py` scripts (listed in `.gitattributes`):
  regenerate, never hand-edit. Every generator verifies relocation targets, so a `RELOC` line in
  generated code is a generator regression: fix the generator, never its output.
- Class getters (`T::refl_GetClass`) are declared for every reflected class in
  `include/base/reflection/UnkClassGetters.h`; include it instead of declaring one locally.
