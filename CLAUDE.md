# dread-decomp

Start with `docs/HANDOFF.md` for current state and next steps. Outside projects are listed in
`docs/references.md`. Add any newly used or referenced project there.

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
| `src/`, `include/` | yes | reconstructed C++17 game code |
| `lib/` | yes | vendored upstream Lua 5.1.4 (`lib/lua`); submodules `lib/nnsdk`, `lib/musl` (headers only) |
| `data/` | yes | metadata only: `hashes.json`, `functions/main_XX.csv`, `reflection_types.csv`, `library_symbols.csv`, `toolchains.json`, `sources.json`; never code bytes |
| `docs/` | yes | `recon.md`, `compiler.md`, notes |
| `port/` | yes | PC port layer (empty until later phases) |

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
python tools/gen_types.py --version <ver>        # regenerate src/reflection_types/ (non-class type getters)
python tools/gen_lift.py --version <ver>         # regenerate src/lifted/ (small functions; run after the other generators, ~4 min)
python tools/check.py --version <ver> [--update] # build src/, derive statuses, catch regressions
python tools/objdiff_gen.py --version <ver>      # objdiff targets + objdiff.json
python tools/progress.py                         # whole-binary progress (excludes L)
python tools/asmdiff.py --version <ver> <obj> -v # diff one object against the original
python tools/trymatch.py --version <ver> <file.cpp> [-v] [-- flags]  # build + diff one source
python tools/ghidra/decompile.py --version <ver> <addr|name>...    # read Ghidra's pseudocode (never commit it)
```

Target version: **2.1.0** (update v327680). The `main` NSO build ID is
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`. Title ID is `010093801237C000`.

Remote: https://github.com/var-flotsam/dread-decomp (private, MIT license).
Ask the user before pushing. (var-flotsam/MPT-Decomp is a separate project, a Metroid
Prime Trilogy recomp. Never push this repo there.)

## Local environment (Windows 11)

The user supplies these. Don't copy anything from them into the repo.

| What | Where |
|---|---|
| ExeFS/RomFS dump (2.1.0, from Eden) | (machine-specific: `CLAUDE.local.md`) |
| Keys | (machine-specific: `CLAUDE.local.md`; never read into tracked files or printed) |
| Game files (base + 2.1.0 update) | (machine-specific: `CLAUDE.local.md`) |
| Emulator (Eden, a yuzu fork) | (machine-specific: `CLAUDE.local.md`) |

Toolchain:
- Python 3.12, with packages in the repo venv `.venv` (`.venv/Scripts/python`).
  Python deps are `lz4`, `mercury-engine-data-structures`, `capstone`, `pyelftools`, and
  `pyghidra` (installed from Ghidra's bundled wheels, without network access).
- JDK 21 (Temurin), with Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\ghidra_12.1.4_PUBLIC`
  (headless: `support\analyzeHeadless.bat`).
- CMake 4.4, Ninja 1.13, LLVM/Clang + lld 23.1.3 (`C:\Program Files\LLVM\bin`) for inspection.
- Project compiler: upstream Clang 9.0.1 from `tools/fetch_toolchains.py`
  (`%LOCALAPPDATA%\dread-decomp\toolchains\llvm-9.0.1`). Flags live in `tools/buildcfg.py`:
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
  `tools/buildcfg.py`, see `docs/compiler.md`). It must pass before committing `src/` changes. Subsystems ending in `?` were inferred from neighbours.
- Inferred-role names use a `refl_` prefix (e.g. `CActor::refl_GetClass`) or `vf_<slot>` for
  vtable slots. These are not Mercury's real method names.
- Commit after each numbered step, with commit messages prefixed by phase and step
  (e.g. `Phase 2.1: ...`).

## Status

- Phase 1: done. The 2.1.0 dump has been imported. Its hashes are confirmed:
  `tools/verify_dump.py` passed (all NSO segment hashes match their headers, and all
  9774 RomFS files plus 62206 pack entries match files.toc sizes). There are no public
  reference hashes for this dump; No-Intro search and the web turned up nothing.
- Phase 2: done. See `docs/recon.md`.
- Phase 3: done, including the non-leaf and library flag pass. See `docs/compiler.md`.
- Phase 4: in progress (owner-approved). Current state, numbers and next steps: `docs/HANDOFF.md`.
- Generated sources (`src/reflection/`, `src/reflection_types/`, `src/lifted/`) are outputs of
  `tools/gen_reflection.py`, `tools/gen_types.py` and `tools/gen_lift.py`: regenerate, never hand-edit.
