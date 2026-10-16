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
| 3. Compiler identification | **Done (leaf functions).** Upstream Clang 9.0.1 `-O2 -mcpu=cortex-a57`: 7/10 exact. `-O2` vs `-O3` still open |
| 4–5. Vertical slice (boot to title screen) | **Not started.** Waiting on the owner's go-ahead; proposal below |
| Port | Not started (`port/` is empty) |

Function status (BotW codes): `O` 7, `m` 2, `M` 1, `L` 1,429, `U` 102,578.

## Hard rules (summary; full text in CLAUDE.md)

- No game data, assembly listings, decompiler output, keys or firmware in git. Game data stays in
  `orig/` and `build/`. The pre-commit hook enforces part of this; never bypass it.
- `src/` is our own C++. Never paste Ghidra output.
- Never download game images, keys, firmware or Nintendo SDK files.
- Ask the owner before pushing or when an input is missing.

## Repository map

```
tools/setup.py            import + hash-verify a dump into orig/<ver>/
tools/verify_dump.py      check the dump against the game's own records (NSO hashes, title ID, files.toc)
tools/nso.py              NSO parser (LZ4 segments, header hashes)
tools/nso2elf.py          NSO -> AArch64 ELF with sections/dynamic/relocations
tools/recon/              Python-only recon: eh_frame bounds, RTTI, Lua tables, build strings, reflection
tools/ghidra/             PyGhidra headless driver (analyze.py) and exporter (export.py)
tools/gen_functions_csv.py   data/functions/main_XX.csv + data/reflection_types.csv
tools/fetch_toolchains.py    candidate upstream LLVM releases, hash-pinned
tools/compiler_matrix.py     compiler x flags matrix over src/compiler_id/
tools/asmdiff.py          per-function instruction diff vs the original (relocations masked)
tools/check.py            build src/ with the project toolchain, derive O/m/M/W, catch regressions
tools/hooks/pre-commit    rejects >1 MB files, console formats/keys, orig/ and build/
data/functions/main_XX.csv   one CSV per MiB of address space (1 MB hook limit)
data/reflection_types.csv    reflected type, kind, parent (classes), sizeof, getter
data/hashes.json          confirmed dump hashes;  data/toolchains.json  pinned compiler hashes
src/compiler_id/          10 reconstructed leaf functions used to identify the compiler
include/types.h           u8..f64 without a standard library
docs/                     recon.md, compiler.md, references.md, this file
```

## Rebuilding everything from scratch (Windows)

Not in git, and must exist locally: the dump (Eden: `%APPDATA%\eden\dump\010093801237C000\`),
Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\`, JDK 21, LLVM 23 at `C:\Program Files\LLVM`, 7-Zip.

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
.venv/Scripts/python tools/fetch_toolchains.py                     # ~1 GB download
.venv/Scripts/python tools/check.py --version 2.1.0 --update
```

The output of `gen_functions_csv.py` should be identical to what's committed (only `status` is
carried over from the previous files).

## Key facts

- **Modules:** `main` (game, 19.1 MB of code), `sdk` (NintendoSDK 11.4.3, NVN, libz),
  `subsdk0` (`nn::movie`), `rtld`. `main` imports 526 SDK symbols. Their mangled names give exact
  signatures for `nn::` headers.
- **Statically linked into `main`:** NintendoWare `nn::atk` (~1,150 functions, at the end of
  `.text`), Lua 5.1 (around `0x71010a5000`–`0x71010c2000`), Dear ImGui.
- **Game code:** no RTTI and no C++ exceptions; `.eh_frame` gives exact bounds for 99% of
  functions. The linker folds identical bodies, so one address can stand for several functions.
- **Mercury reflection** is the main naming source. Every reflected type `T` has a guarded getter
  (`T::refl_GetClass`) that calls its parent's getter and a per-type helper
  (`T::refl_InitClass`). The helper stores `sizeof(T)` at descriptor `+0x18`. Parents agree with
  MEDS for 2,265 classes.
- **Memory:** 1.82 GB `.bss`. The engine reserves its memory statically, which a port will need
  to replicate or remap.
- **Internal codename:** "cazadora" (from a leftover Perforce build path).

## Decisions made (and why)

| Decision | Reason |
|---|---|
| Upstream Clang **9.0.1**, `-O2 -mcpu=cortex-a57 -fno-rtti -fno-exceptions` | Only candidate matching all three version-discriminating functions; the SDK compiler isn't public and is off-limits |
| Status codes from **zeldaret/botw** (`O m M W U L`) | Covers matching, minor/major non-matching and library code; `m` is usable by the port, `M`/`W` are not |
| `O/m/M/W` derived by `tools/check.py` from `src/` + `// NON_MATCHING(minor|major):` | No hand-edited outputs; regressions of `O` fail the check |
| `L` for `nn::atk`, SDK, runtime, Lua stdlib (incl. neighbour-inferred `?`) | Library code isn't game progress (as BotW's `L`); inferred ones may need revising |
| Function list split per MiB | Owner's 1 MB pre-commit limit |
| Extra `name_source` value `symbol` | Names from the dynamic symbol table / PLT |
| `refl_*` and `vf_<slot>` name prefixes | Mark inferred roles; Mercury's real method names are unknown |
| Lua and ImGui from upstream source (approved) | Open source (MIT); same approach as PrimeDecomp's zlib |
| Repo private, MIT licensed | Owner's choice |

## Open questions for the owner

1. **Flag-confirmation pass** (recommended): 20–30 non-leaf functions to settle `-O2` vs `-O3`,
   frame-pointer policy, `-fPIC` and inlining. Also sample Lua, ImGui and `nn::atk` separately,
   since PrimeDecomp shows that libraries often use different compiler versions or flags.
2. **Matching standard:** recommended "aim for `O`, time-box each function to 30–60 minutes,
   then `m` with a note". Reflection boilerplate must reach `O` via a shared template.
3. **`nn::` headers:** use `open-ead/nnheaders`/`nnsdk` (mid-migration; "matching decomp only"
   usage note) or write our own from the 526 mangled imports?
4. **`nn::atk`:** stub for the slice. Decompile later as its own track? (`open-ead/nnware` is
   early and unlicensed.)
5. **Exact slice via Eden's GDB stub:** port number and whether it's enabled, to record the
   functions executed from boot to the title screen.

## Proposed next steps (Phase 4–5)

1. Flag-confirmation pass (above); add objdiff config and a progress report (by status, by
   subsystem, excluding `L`).
2. Vendor Lua 5.1: build each 5.1.x patch release with the project flags, diff against `main`, pick
   the matching one, and map its functions (marked `L`). Same for Dear ImGui.
3. Reflection: write the getter and descriptor-helper templates/macros until one class matches,
   then generate all 2,346 from `data/reflection_types.csv`. About 4,900 functions; high leverage.
4. Core engine types the slice depends on: `base::global` strings, `CStrId`, containers, CRC64,
   allocators over the static arena.
5. Boot to title screen: startup, memory, file system / `files.toc` / `.pkg`, resource manager,
   Lua + GUI bindings, GUI, render front end, input, localisation, save probe. Estimated
   6,000–9,000 functions (static floor: 4,394 reachable from `nnMain`).
6. Port groundwork: an aurora-style `nn::` + NVN compatibility layer (see `docs/references.md`).

## Known issues and gotchas

- **Windows PATH:** shells started before the toolchain install may not see `cmake`/`ninja`. Use
  `.venv/Scripts/python` and absolute tool paths, as the scripts do.
- **Ghidra project lock:** only one process can open `build/ghidra/<ver>/dread.gpr` at a time
  (`analyze.py` and `export.py` can't run together).
- `llvm-readelf` isn't in the LLVM Windows build; use `llvm-readobj --elf-output-style=GNU`.
- CRLF warnings on commit are harmless (`.gitattributes` normalises to LF).
- `CRodomithonXSuckAttack`'s recovered `sizeof` (0x40) is smaller than its parent's (0x190).
  Treat it as unreliable. 42 reflected types have no identified descriptor kind.
- Subsystems ending in `?` were inferred from neighbours. They are guesses, and some `L` markings
  depend on them.
- `fn_710083ecf8` needs `fmin` + `b.lt` where upstream Clang emits `fminnm` + `b.mi`. Watch for
  this pattern; it may point to a shared math helper or a Nintendo compiler patch.
- **Leftovers for the owner to delete by hand:** `C:\Program Files\Git\tmp_bak.cpp` (a stray
  386-byte copy of `src/compiler_id/fn_7100162300.cpp`, blocked from automatic removal) and the
  empty folder `a folder named dread next to this repository` (locked by a running shell).
