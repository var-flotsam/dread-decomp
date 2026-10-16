# dread-decomp

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
  to discuss a match. Never write them into tracked files.

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
| `data/` | yes | metadata only: `hashes.json`, `functions.csv`; never code bytes |
| `docs/` | yes | `recon.md`, `compiler.md`, notes |
| `port/` | yes | PC port layer (empty until later phases) |

## Setup

```sh
git config core.hooksPath tools/hooks          # once per clone
python tools/setup.py --version <ver> --exefs <dir> --romfs <dir>
python tools/setup.py --version <ver> --confirm   # after checking printed hashes
python tools/setup.py --version <ver> --verify-only
```

Target version: **2.1.0** (update v327680). The `main` NSO build ID is
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`. Title ID is `010093801237C000`.

Remote: https://github.com/var-flotsam/dread-decomp (private, no license chosen yet).
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
  Python deps are `lz4` and `mercury-engine-data-structures`.
- JDK 21 (Temurin), with Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\ghidra_12.1.4_PUBLIC`
  (headless: `support\analyzeHeadless.bat`).
- CMake 4.4, Ninja 1.13, LLVM/Clang + lld 23.1.3 (`C:\Program Files\LLVM\bin`).
- Shells opened before the install may need PATH refreshed from the registry.

## Conventions

- Python 3 (standard library where possible) for tooling, CMake + Ninja for builds,
  C++17 for game code.
- Tool scripts must be reproducible. Outputs are always regenerated, never hand-edited.
- Names you aren't sure of keep an address-based placeholder: `fn_<addr>` (e.g.
  `fn_7100123456`), `UnkStruct_0x40` (size suffix), `field_0x18`, `unk_<addr>` for data.
  Only use a real name when it comes from RTTI, reflection, strings, or
  mercury-engine-data-structures. Record that source in `data/functions.csv`.
- `data/functions.csv` columns: `address,size,name,name_source,subsystem,status`.
  `name_source` is one of `rtti|reflection|string|unknown`. `status` is one of
  `none|nonmatching|matching`.
- Commit after each numbered step, with commit messages prefixed by phase and step
  (e.g. `Phase 2.1: ...`).

## Status

- Phase 1: done. The 2.1.0 dump has been imported. `data/hashes.json` is recorded but
  **unconfirmed** until the user checks it (`--confirm`).
- Phase 2 (recon) and Phase 3 (compiler ID) are next.
