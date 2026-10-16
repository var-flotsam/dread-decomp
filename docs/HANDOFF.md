# Handoff: dread-decomp

For the next developer, human or AI, picking up this project. Read this, then `CLAUDE.md`
(rules and conventions), `docs/recon.md` (what's in the binary), `docs/compiler.md`
(toolchain), and `docs/references.md` (outside projects).

Snapshot as of 2026-10-15. Target: **Metroid Dread 2.1.0**, `main` build ID
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.

## Where things stand

| Phase | State |
|---|---|
| 1. Repo, provenance, hooks | **Done.** Dump imported and verified; hashes confirmed |
| 2. Binary reconnaissance | **Done.** 104,017 functions; 7.0% named; 2,485 reflected types with `sizeof` for all 2,346 classes |
| 3. Compiler identification | **Done.** Clang 9.0.x `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float`. Confirmed on 20 non-leaf game functions (18 exact), `nn::atk` (5/6) and upstream Lua 5.1.4 (81/82) |
| Tooling for Phase 4 | **Done.** `tools/check.py` (build + status), objdiff (`tools/objdiff_gen.py`), whole-binary progress (`tools/progress.py`) |
| 4–5. Vertical slice (boot to title screen) | **Not started.** Waiting on the owner's go-ahead; plan below |
| Port | Not started (`port/` is empty) |

Function status (BotW codes): `O` 25, `m` 4, `M` 1, `L` 1,429, `U` 102,558. The five matching
`nn::atk` functions keep `L`, so they aren't counted as game progress.

## Hard rules (summary; full text in CLAUDE.md)

- No game data, assembly listings, decompiler output, keys or firmware in git. Game data stays in
  `orig/` and `build/`. The pre-commit hook enforces part of this; never bypass it.
- `src/` is our own C++. Never paste Ghidra output.
- Never download game images, keys, firmware or Nintendo SDK files.
- Ask the owner before pushing or when an input is missing.
- Matching standard: aim for `O`; time-box each function to 30–60 minutes, then annotate
  `NON_MATCHING(minor|major)`. Reflection boilerplate must reach `O`.

## Repository map

```
tools/setup.py            import + hash-verify a dump into orig/<ver>/
tools/verify_dump.py      check the dump against the game's own records
tools/nso.py, nso2elf.py  NSO parsing; NSO -> AArch64 ELF
tools/recon/              Python-only recon: eh_frame bounds, RTTI, Lua tables, build strings, reflection
tools/ghidra/             PyGhidra headless driver (analyze.py) and exporter (export.py)
tools/gen_functions_csv.py   data/functions/main_XX.csv + data/reflection_types.csv
tools/fetch_toolchains.py    upstream LLVM candidates + objdiff, hash-pinned (data/toolchains.json)
tools/fetch_sources.py       upstream Lua 5.1.x sources, hash-pinned (data/sources.json)
tools/compiler_matrix.py     compiler x flag matrix: --set leaf | nonleaf | atk
tools/libcheck_lua.py        build upstream Lua with the game flags and diff it against main
tools/asmdiff.py          per-function instruction diff vs the original (relocations masked)
tools/check.py            build src/ with the project toolchain; derive O/m/M/W; catch regressions
tools/objdiff_gen.py      objdiff target objects + objdiff.json (gitignored) for every src/ unit
tools/progress.py         whole-binary progress from data/functions/ (excludes L)
tools/hooks/pre-commit    rejects >1 MB files, console formats/keys, orig/ and build/
src/compiler_id/          10 leaf functions;  src/flag_id/  20 calling functions;  src/atk_id/  6 nn::atk
include/types.h           u8..f64 without a standard library
lib/nnsdk/                open-ead/nnsdk submodule: community NintendoSDK declarations (no license; referenced, not copied)
lib/musl/                 open-ead/botw-lib-musl submodule: musl 1.2.2 C headers (MIT)
tools/buildcfg.py         the one place that defines compiler, flags and include paths
data/                     functions/ CSVs, reflection_types.csv, hashes.json, toolchains.json, sources.json
docs/                     recon.md, compiler.md, references.md, this file
```

## Rebuilding everything from scratch (Windows)

Not in git, and must exist locally: the dump (Eden: `%APPDATA%\eden\dump\010093801237C000\`),
Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\`, JDK 21, LLVM 23 at `C:\Program Files\LLVM`, 7-Zip.
Downloaded toolchains and sources go to `%LOCALAPPDATA%\dread-decomp\`.

```sh
git config core.hooksPath tools/hooks
py -3.12 -m venv .venv
.venv/Scripts/python -m pip install lz4 mercury-engine-data-structures capstone pyelftools
.venv/Scripts/python -m pip install --no-index --find-links "%LOCALAPPDATA%/Programs/ghidra_12.1.4_PUBLIC/Ghidra/Features/PyGhidra/pypkg/dist" pyghidra==3.1.0
.venv/Scripts/python tools/setup.py --version 2.1.0 --exefs <dump>/exefs --romfs <dump>/romfs
.venv/Scripts/python tools/verify_dump.py --version 2.1.0
.venv/Scripts/python tools/nso2elf.py orig/2.1.0/exefs/main orig/2.1.0/main.elf
.venv/Scripts/python tools/recon/run.py --version 2.1.0
.venv/Scripts/python tools/ghidra/analyze.py --version 2.1.0      # ~8 min, 16 GB heap
.venv/Scripts/python tools/gen_functions_csv.py --version 2.1.0
.venv/Scripts/python tools/fetch_toolchains.py                     # ~1.5 GB of LLVM releases + objdiff
.venv/Scripts/python tools/fetch_sources.py                        # Lua 5.1.x
.venv/Scripts/python tools/check.py --version 2.1.0 --update
.venv/Scripts/python tools/objdiff_gen.py --version 2.1.0          # then open the repo in objdiff
.venv/Scripts/python tools/progress.py
```

The output of `gen_functions_csv.py` is reproducible (only `status` is carried over).

## Key facts

- **Modules:** `main` (game, 19.1 MB of code), `sdk` (NintendoSDK 11.4.3, NVN, libz), `subsdk0`
  (`nn::movie`), `rtld`. `main` imports 526 SDK symbols. Their mangled names give exact signatures
  for `nn::` headers.
- **Statically linked into `main`:** `nn::atk` (~1,150 functions, end of `.text`, same toolchain as
  the game), **Lua 5.1.4** with `float` numbers, built as C++ (around `0x71010a5000`–`0x71010c2000`;
  `print` replaced), and about 1 KB of Dear ImGui.
- **Game code:** no RTTI, no C++ exceptions, unwind tables everywhere, PIC with GOT access to
  vtables and globals, no compiler-introduced SIMD. The linker folds identical bodies.
- **Mercury reflection** is the main naming source. Every reflected type `T` has a guarded getter
  (`T::refl_GetClass`) built from `"T"`, the parent's getter, and two type-specific function
  pointers. The per-type helper (`T::refl_InitClass`) stores `sizeof(T)` at descriptor `+0x18`.
  Parents agree with MEDS for 2,265 classes. Our reconstruction of the `CActor` getter is 98.4%;
  only the static's offset from its guard (set by the other statics in the TU) differs.
- **Memory:** 1.82 GB `.bss`. The engine reserves its memory statically.
- **Internal codename:** "cazadora" (from a leftover Perforce build path).

## Decisions made (and why)

| Decision | Reason |
|---|---|
| Upstream Clang **9.0.1**, `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float -fno-rtti -fno-exceptions` | Each flag is backed by a fingerprint in the binary and by matching results (`docs/compiler.md`). The SDK compiler is off-limits |
| Status codes from **zeldaret/botw** (`O m M W U L`) | Covers matching, minor/major non-matching and library code; `m` is usable by the port |
| `O/m/M/W` derived by `tools/check.py` from `src/` + `// NON_MATCHING(minor|major):` | No hand-edited outputs; `O` regressions fail the check; `L` is never overwritten |
| `L` for `nn::atk`, SDK, runtime, Lua stdlib (incl. neighbour-inferred `?`) | Library code isn't game progress (BotW's `L`) |
| 30–60 minute time-box per function, except reflection boilerplate | Owner-approved matching standard |
| Function list split per MiB | Owner's 1 MB pre-commit limit |
| objdiff per `src/` unit with synthesized target objects; whole-binary progress from the CSV | No byte-identical relink is possible on Switch (BotW does the same) |
| Lua and ImGui from upstream source (approved) | Open source (MIT); Lua 5.1.4 confirmed by diffing |
| Repo private, MIT licensed | Owner's choice |

## Open questions for the owner

1. **`nn::` headers:** use `open-ead/nnheaders`/`nnsdk` (mid-migration; "matching decomp only"
   usage note) or write our own from the 526 mangled imports?
2. **`nn::atk`:** stub for the slice; decompile later as its own track? (`open-ead/nnware`'s atk is
   early and unlicensed.)
3. **Exact slice via Eden's GDB stub:** port number and whether it's enabled, to record the
   functions executed from boot to the title screen.

## Proposed next steps (Phase 4–5)

1. **Vendor Lua 5.1.4** into `lib/lua/` (with the `float` `luaconf.h` change),
   and add it to `check.py`/objdiff. Then signature-match the rest of Lua (VM, parser, GC: roughly
   400 functions) against `main` to name them and mark them `L`. `tools/libcheck_lua.py` already
   builds and maps the standard libraries.
2. **Reflection:** write the getter and descriptor-helper templates until one whole translation
   unit's statics line up (the guard/descriptor offsets), then generate all 2,346 classes from
   `data/reflection_types.csv`. About 4,900 functions; high leverage.
3. Core engine types the slice depends on: `base::global` strings (refcounted, with a global
   string pool; the getter shows the destructor), `CStrId`, containers, CRC64, allocators over the
   static arena.
4. Boot to title screen: startup, memory, file system / `files.toc` / `.pkg`, resource manager,
   Lua + GUI bindings, GUI, render front end, input, localisation, save probe. Estimated
   6,000–9,000 functions (static floor: 4,394 reachable from `nnMain`).
5. Port groundwork: an aurora-style `nn::` + NVN compatibility layer (see `docs/references.md`).

## Known issues and gotchas

- **Windows PATH:** shells started before the toolchain install may not see `cmake`/`ninja`. Use
  `.venv/Scripts/python` and absolute tool paths, as the scripts do.
- **Ghidra project lock:** only one process can open `build/ghidra/<ver>/dread.gpr` at a time.
- `llvm-readelf` isn't in the LLVM Windows build; use `llvm-readobj --elf-output-style=GNU`.
- CRLF warnings on commit are harmless (`.gitattributes` normalises to LF).
- Constructors compile to two aliased symbols (C1/C2). `check.py` counts them once (by address);
  objdiff counts both.
- objdiff targets: call targets are verified against the original, but data references
  (GOT/ADRP) are mirrored from our object without verification. String-literal relocations
  point at an undefined `.rodata.str1.1` symbol in the target.
- C/C++ headers are musl 1.2.2 (`lib/musl`) and libc++ 9.0.1 (fetched). musl's `BUFSIZ` (1024) sets
  Lua's `luaL_Buffer` size; confirm it against Lua's string-buffer code.
- `CRodomithonXSuckAttack`'s recovered `sizeof` (0x40) is smaller than its parent's (0x190); treat it
  as unreliable. 42 reflected types have no identified descriptor kind.
- Subsystems ending in `?` are guesses, and some `L` markings depend on them.
- `fn_710083ecf8` needs `fmin` + `b.lt` where Clang emits `fminnm` + `b.mi`. Watch for the pattern;
  it may be a shared math helper.
- **Leftovers for the owner to delete by hand:** `C:\Program Files\Git\tmp_bak.cpp` (a stray
  386-byte copy of `src/compiler_id/fn_7100162300.cpp`, blocked from automatic removal) and the
  empty folder `a folder named dread next to this repository` (locked by a running shell).
