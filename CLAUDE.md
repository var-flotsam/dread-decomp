# dread-decomp

Start with `docs/HANDOFF.md` for current state and next steps (one screen; only the merging session edits it),
`docs/PORT_PLAN.md` for the route to a running PC build, `docs/reference.md` for stable facts (repository map,
rebuilding, decisions, gotchas), and `docs/JOURNAL.md` for history and the index of matching findings (long;
search it, don't read it whole). Contributors working from a fork: `CONTRIBUTING.md`;
the maintainer: `docs/maintaining.md` (publication, branch rules, merging, the self-hosted check job). Outside
projects are listed in `docs/references.md`. Add any newly used or referenced project there.

A clean-room-style decompilation of Metroid Dread (Nintendo Switch), aiming for a
native PC port later. It is modelled on PrimeDecomp and zeldaret/botw.

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
| `data/` | yes | metadata only: `hashes.json`, `functions/main_XX.csv`, `reflection_types.csv`, `tu_map.csv`, `library_symbols.csv`, `toolchains.json`, `sources.json`; never code bytes |
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
python tools/trymatch.py --version <ver> <file.cpp> -v # build and diff one source (RELOC lines included)
python tools/hubs.py --version <ver> --boot | --ready --boot | --callers <fn>   # what to match next
python tools/gen_lift.py --version <ver> --best ADDR,...   # closest lifted draft -> build/lift_best/ (hand-matching start)
python tools/tu_map.py --function <addr|name>          # the unit a function belongs to, and its file name
python tools/decl_check.py --base origin/main          # one declaration per shared symbol (CI)
python tools/hub_migrate.py --version <ver> --plan PLAN.tsv   # mechanical header migration; .claude/skills/dread-hubs
python tools/ghidra/decompile.py --version <ver> <addr|name>  # read Ghidra's view (never commit it)
python tools/progress.py --slice boot [--handoff]      # progress; --handoff rewrites docs/HANDOFF.md's table
python tools/port_link.py --version <ver> [--lifted build/port/lifted]   # port link and boot_missing.csv
port/docker/run.sh python3 tools/port_link.py --version <ver> --stdlib libcxx9 --objects build/port/linux/sweep --out build/port/linux/link --executable  # Linux executable (port/README.md)
```

Target version: **2.1.0** (update v327680). The `main` NSO build ID is
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`. Title ID is `010093801237C000`.

Remote: https://github.com/var-flotsam/dread-decomp (private until the owner publishes it; MIT
license; see `docs/maintaining.md`). The pre-2026-10-18
history, with the original PRs, is kept in the private var-flotsam/dread-decomp-archive (rollback
only; don't push there). Ask the user before pushing. (var-flotsam/MPT-Decomp is a separate project, a Metroid
Prime Trilogy recomp. Never push this repo there.)

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
  Only use a real name when it comes from RTTI, reflection, strings, or
  mercury-engine-data-structures. Record that source in the `name_source` column of `data/functions/main_XX.csv`.
- The function list is `data/functions/main_XX.csv` (one file per MiB of address space,
  to stay under the 1 MB hook limit), generated by `tools/gen_functions_csv.py`. Columns are
  `address,size,name,name_source,subsystem,status`. `name_source` is one of
  `symbol|library|rtti|reflection|string|unknown` (`library` = matched against vendored
  upstream code by `tools/sigmatch.py`). `status` uses the zeldaret/botw codes: `O` matching,
  `m` non-matching (minor), `M` non-matching (major), `W` work in progress, `U` not
  decompiled, `L` library. `tools/check.py --update` derives `O/m/M/W` from `src/`; only those
  survive regeneration.
- Mark a non-matching function with `// NON_MATCHING(minor): <reason>` or
  `// NON_MATCHING(major): <reason>` on the line(s) directly above its definition. Use minor only
  when the behaviour is identical.
- Matching standard: aim for `O` on every function, but time-box each one to about 30-60
  minutes. If it still differs, keep the closest version and annotate it with
  `NON_MATCHING(minor)` (same behaviour; usable by the port) or `NON_MATCHING(major)`, with the
  remaining difference spelled out. Exception: reflection boilerplate (`refl_GetClass`,
  `refl_InitClass`) must reach `O`, through a shared template or macro.
- Build and check: `python tools/check.py --version <ver>` (Clang 9.0.1 with the flags in
  `tools/buildcfg.py`, see `docs/compiler.md`). It must pass before committing `src/` changes: no
  `REGRESSION`, `DUPLICATE` or hand-written `RELOC` lines. A function is `O` only when its bytes match
  and every callee, global and GOT slot it names is the original's. Subsystems ending in `?` were
  inferred from neighbours.
- Inferred-role names use a `refl_` prefix (e.g. `CActor::refl_GetClass`) or `vf_<slot>` for
  vtable slots. These are not Mercury's real method names.
- **One declaration per shared symbol.** A function or global used by more than one file is declared
  once, in a header under `include/`, with one signature; a `.cpp` declares only what it alone uses.
  `python tools/decl_check.py --base origin/main` must pass (CI runs it); `--symbol <name>` shows every
  copy. Migrating a hub to its header: `tools/hub_migrate.py` does the mechanical part and lists the
  files it leaves; the hand recipe is `CONTRIBUTING.md`, "Migrating a hub", verified with
  `tools/baseline.py` (procedure for a batch: `.claude/skills/dread-hubs`).
- **Files by unit.** New hand-written sources go under `src/<subsystem>/`, named for the class or
  translation unit (`python tools/tu_map.py --function <addr>`); no new address-named directories.
- **What to match next:** `python tools/hubs.py --version <ver> --boot` (hubs along the boot path by
  unmatched callers), then `--ready`. Progress is reported for the whole binary and the boot slice.
- **Logs go to `docs/JOURNAL.md`** (newest first, with reusable findings added to its index).
  Feature branches leave `docs/HANDOFF.md` alone; the session that merges a round updates it and
  regenerates its numbers with `tools/progress.py --slice boot --handoff`.
- Commit after each numbered step, with commit messages prefixed by phase and step
  (e.g. `Phase 2.1: ...`). A pull request is a hub cluster or about 30 functions or more.

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
