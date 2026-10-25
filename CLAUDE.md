# dread-decomp

Start with `docs/HANDOFF.md` for current state and next steps (short; edited in place, never appended to),
`docs/PORT_PLAN.md` for the route to a running PC build, and `docs/JOURNAL.md` for history and the index of
matching findings (long; search it, don't read it whole). Contributors working from a fork: `CONTRIBUTING.md`;
the maintainer: `docs/maintaining.md` (publication, branch rules, merging, the self-hosted check job). Outside
projects are listed in `docs/references.md`. Add any newly used or referenced project there.
Small local models used `AGENTS.md` and `docs/tasks/` (retired; reference only).

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
| `docs/` | yes | `HANDOFF.md` (state), `PORT_PLAN.md`, `JOURNAL.md` (history), `recon.md`, `compiler.md`, `maintaining.md`, `references.md` |
| `port/` | yes | PC port layer (Route B): CMake project (`dread_game`, `dread_libs`, `dread_runtime`, `dread`, `sweep`, `link_report`), the port's own `nn::` runtime under `runtime/`, `sample_duplicates.txt`; `port/README.md` has the layout, the sweep and the link |

## Setup

```sh
git config core.hooksPath tools/hooks          # once per clone
git submodule update --init                    # lib/nnsdk, lib/musl (needed to build)
python tools/setup.py --version <ver> --exefs <dir> --romfs <dir>
python tools/setup.py --version <ver> --confirm   # after checking printed hashes
python tools/setup.py --version <ver> --verify-only
python tools/verify_dump.py --version <ver>   # NSO segment hashes, title ID, RomFS vs files.toc
python tools/nso2elf.py orig/<ver>/exefs/main orig/<ver>/main.elf
python tools/recon/run.py --version <ver>        # Python-only recon -> build/recon/<ver>/
python tools/ghidra/analyze.py --version <ver>   # Ghidra import/analysis/export (~8 min)
python tools/gen_functions_csv.py --version <ver>
python tools/fetch_toolchains.py                 # candidate upstream LLVM releases + objdiff
python tools/fetch_sources.py                    # upstream Lua 5.1.x sources
python tools/compiler_matrix.py --version <ver> --set leaf|nonleaf|atk
python tools/libcheck_lua.py --version <ver>     # upstream Lua vs the embedded copy
python tools/sigmatch.py --version <ver> [--library lua]  # name vendored-library functions -> data/library_symbols.csv; then rerun gen_functions_csv.py
python tools/gen_reflection.py --version <ver>   # regenerate src/reflection/ (uses tools/refl_attrs.py)
python tools/gen_class_getters.py                # regenerate include/base/reflection/UnkClassGetters.h from data/functions/ (no game data)
python tools/gen_types.py --version <ver>        # regenerate src/reflection_types/ (non-class type getters)
python tools/gen_statics.py --version <ver>      # regenerate src/reflection_statics/ (guarded function-local statics)
python tools/gen_clones.py --version <ver>       # regenerate src/clones/ (repeated template instantiations; ~10 s)
python tools/gen_lift.py --version <ver>         # regenerate src/lifted/ (small functions; run after the other generators, ~15 min)
python tools/gen_lift.py --version <ver> --best ADDR,...|@file  # closest lifted attempt per function -> build/lift_best/ (hand-matching start)
python tools/gen_lift.py --prune                 # drop lifted copies of functions now defined elsewhere (seconds; verify.py runs it)
python tools/gen_lift.py --version <ver> --sample 500 [--seed S] [--only ADDR,...|@file] [--new] [--examples K]  # outcome table only (build/), ~1 min
python tools/nearcopy.py --version <ver> LO HI | --refs U O   # U functions closest to an O one (hand-matching starts; .claude/skills/dread-match)
python tools/shape_census.py --version <ver> [--min 5] [--max-size B] [--top N] [--family ADDR]  # same-shape U families (maintenance)
python tools/tu_map.py --version <ver>           # translation-unit map from .init_array -> data/tu_map.csv (~10 s)
python tools/tu_map.py --function <addr|name>    # the unit a function belongs to and a file name for it; --report, --top N
python tools/check.py --version <ver> [--update] [--no-reloc]  # build src/, derive statuses, regressions, duplicates, relocation targets (RELOC lines)
python tools/verify.py --version <ver> [--regen gen_statics,...]  # generators + check --update + progress + policy + decl_check (info), ~10 lines
python tools/objdiff_gen.py --version <ver>      # objdiff targets + objdiff.json
python tools/progress.py [--slice boot | --slice-from <fn>]  # whole-binary progress (excludes L); the nnMain boot slice
python tools/dashboard_export.py [--ref HEAD] [--days 30]  # status CSVs for the project dashboard -> build/dashboard/ (refresh: upload them)
python tools/hubs.py --version <ver> [--boot] [--top N]      # U functions by unmatched callers: what to match next
python tools/hubs.py --version <ver> --ready [--boot]        # U functions whose callees are all matched; --callers <fn> lists callers
python tools/decl_check.py [--list | --symbol <name> | --json] [--base <ref> [--strict]]  # fn_/g_unk_ declared in several files or with conflicting signatures; --base enforces (CI)
python tools/hub_migrate.py --version <ver> SYM... | --header-redeclared [--dry-run]  # mechanical hub migration: drop local declarations, include the one header, keep byte-identical files; LEFT lines are the hand work
python tools/baseline.py save|compare|restore --tag T [FILES | --symbol SYM | --includers HDR]  # trymatch output snapshot and compare for a file set (the migration evidence; .claude/skills/dread-hubs)
python tools/asmdiff.py --version <ver> <obj> -v # diff one object against the original (RELOC lines included)
python tools/trymatch.py --version <ver> <file.cpp> [-v] [--no-reloc] [-- flags]  # build + diff one source
python tools/ghidra/decompile.py --version <ver> <addr|name>...    # read Ghidra's pseudocode (never commit it)
python tools/policy_check.py [--base <ref>]      # content rules (also run by CI on every PR)
python tools/port_sweep.py --version <ver>       # port: compile every src/ file for x86-64 (compile only) -> build/port/sweep/
python tools/port_link.py --version <ver>        # port: whole-program relocatable link of the sweep objects, undefined-symbol report, boot_missing.csv -> build/port/link/
python tools/port_lift.py --version <ver> [--top N | --only ADDR,...] [--debug ADDR] [--mutants N]  # port: semantic lifts of boot_missing.csv's U functions, checked by the P-code oracle -> build/port/lifted/ (never src/)
python tools/port_link.py --version <ver> --lifted build/port/lifted   # the link with the semantic lifts: boot slice with and without them
python tools/abi_facts.py --version <ver> ADDR...  # argument registers a function reads, whether callers use its result (semantic lifts, oracle)
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
- **Logs go to `docs/JOURNAL.md`** (newest first, with reusable findings added to its index);
  `docs/HANDOFF.md` is edited where the state changed, never appended to.
- Commit after each numbered step, with commit messages prefixed by phase and step
  (e.g. `Phase 2.1: ...`). A pull request is a hub cluster or about 30 functions or more.

## Status

- Phase 1: done. The 2.1.0 dump has been imported. Its hashes are confirmed:
  `tools/verify_dump.py` passed (all NSO segment hashes match their headers, and all
  9774 RomFS files plus 62206 pack entries match files.toc sizes). There are no public
  reference hashes for this dump; No-Intro search and the web turned up nothing.
- Phase 2: done. See `docs/recon.md`.
- Phase 3: done, including the non-leaf and library flag pass. See `docs/compiler.md`.
- Phase 4: in progress (owner-approved). Since 2026-10-24 the goal is a clean, readable tree that
  runs on PC within months: hub-first matching along the boot path, canonical headers, generators in
  maintenance. Current state, numbers and next steps: `docs/HANDOFF.md`; the route to a running build
  and its open decisions: `docs/PORT_PLAN.md`.
- Generated sources (`src/reflection/`, `src/reflection_types/`, `src/reflection_statics/`, `src/clones/`,
  `src/tables/`, `src/lifted/`, and the header `include/base/reflection/UnkClassGetters.h`) are outputs
  of `tools/gen_reflection.py`, `tools/gen_types.py`, `tools/gen_statics.py`, `tools/gen_clones.py`,
  `tools/gen_tables.py`, `tools/gen_lift.py` and `tools/gen_class_getters.py`: regenerate, never
  hand-edit. Every generator verifies relocation targets, so a `RELOC` line in generated code is a
  generator regression: fix the generator, never its output.
- Class getters (`T::refl_GetClass`) are declared for every reflected class in
  `include/base/reflection/UnkClassGetters.h`; include it instead of declaring one locally.
