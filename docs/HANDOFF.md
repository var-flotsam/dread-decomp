# Handoff: dread-decomp

The current state of the project, for the next developer, human or AI. Read this, then `CLAUDE.md`
(rules and conventions) and `docs/PORT_PLAN.md` (the route to a running PC build and the decisions
it needs). History, batch logs and the index of matching findings are in `docs/JOURNAL.md`;
`docs/recon.md` describes the binary, `docs/compiler.md` the toolchain, `docs/references.md` the
outside projects. This page is edited in place, never appended to: session notes go to the journal.

**Last updated 2026-10-26** (hub batches 3 and 4, var-flotsam/dread-decomp#195 and #197; the TU helper fold, #196; boot hubs round 1, branch `claude/boot-hubs-1`).
Target: **Metroid Dread 2.1.0**, `main` build ID `646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.

## Start here

- **Open work:** `gh pr list` is the source of truth for what is waiting to merge; the lifter's semantic
  coverage mode (stream M) runs in its own session on a `claude/lifter-semantic-coverage-*` branch.
- **Next hub batch:** `tools/hub_migrate.py --header-redeclared` has nothing left but one deliberate
  keep (`src/game_s2/fn_71003f1b00.cpp`), and every conflicting symbol with a hand-written definition
  has one declaration (batch 4). The 250 conflicting symbols left have no hand-written definition:
  39 a lifted one, 211 none, so the callers' types and `tools/abi_facts.py` decide the signature, and
  most need a shared type in a new header. `tools/decl_check.py --list` ranks them. Work them per
  `.claude/skills/dread-hubs`: the tool first (`hub_migrate.py SYM --header H --declaration D` once a
  signature is chosen), agents on disjoint file sets for what it leaves, verified with
  `tools/baseline.py` (save a whole-tree tag first: 1,353 sources in 2 minutes).
- **Port:** `python tools/port_link.py --version 2.1.0` rebuilds `build/port/link/boot_missing.csv`, the
  boot-slice functions without a source (the lifter's worklist) and the runtime stubs still needed.
- Every number on this page names the command that produced it; rerun the command rather than trust
  the page.

## Where things stand

| Phase | State |
|---|---|
| 1. Repo, provenance, hooks | **Done.** Dump imported and verified; hashes confirmed |
| 2. Binary reconnaissance | **Done.** 104,017 functions; 7.0% named; 2,485 reflected types with `sizeof` for all 2,346 classes |
| 3. Compiler identification | **Done.** Clang 9.0.1 `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float` |
| 4.1 System headers, 4.2 Lua | **Done.** `lib/nnsdk` + `lib/musl` submodules, libc++ 9.0.1 headers; upstream Lua 5.1.4 vendored, 286 functions `L` |
| 4.3 Reflection (generated) | **Done, in maintenance.** Class getters, descriptor constructors, attribute/field functions, pointer/enum/collection types (`src/reflection/`, `src/reflection_types/`) |
| Generators: lifted, statics, clones, tables | **In maintenance.** `src/lifted/` 46,380 functions; `src/reflection_statics/`, `src/clones/`, `src/tables/`. The lifter is at its limit for byte-identical output (see the journal); no new variant rounds |
| 4.4 Core engine types and hand matching | **In progress.** String ids and pool, `UnkVector`, CRC64, allocator getter, reflected-function array; hand slices in `src/game_*/` (0.76 MB typed) |
| 4.5 Boot to title screen | **Measured, not started as a slice.** 4,441 functions reachable from `nnMain` by direct calls; see the table below |
| Port | **M0 done** (var-flotsam/dread-decomp#183): `tools/port_sweep.py` compiles all 3,808 `src/` files for x86-64 once `port/runtime/include` stands in for the SDK headers; `port/` CMake project with `dread_game`, `dread_runtime` (our own `nn::` declarations and stubs) and the banner executable. **M1 steps 1 and 2 done** (2026-10-25, var-flotsam/dread-decomp#186 and the vendored-libraries step): `tools/port_link.py` links the whole program, Lua and dlmalloc included, relocatably and lists what is missing (2,157 boot-slice functions without a source). **M1 step 3 done** (2026-10-25): `tools/port_lift.py` gives 1,777 of the slice's 2,148 `U` functions a semantic lift checked by a P-code oracle (`build/port/lifted/`, never `src/`); with them 380 slice functions are left without a source; the gaps step raises that to 1,941 lifts and 216 left. No executable on this PC (no Windows SDK or WSL) |

Numbers from `tools/progress.py` and `tools/progress.py --slice boot` on 2026-10-25 (hub batch 2,
var-flotsam/dread-decomp#187):

| Scope | Functions counted | Bytes | `O` matching | `O`+`m` usable | `U` left |
|---|---|---|---|---|---|
| Whole binary (library `L` excluded) | 102,398 | 18.77 MB | **53.90%** (84,713) | 55.40% | 17,438 functions, 8.37 MB |
| Boot slice (reachable from `nnMain`, direct calls only) | 4,308 | 1.71 MB | **24.96%** (1,889; boot hubs round 1, 2026-10-26) | 27.49% | 2,366 functions |

Of the 10.1 MB matched, about 9.4 MB is generated (reflection boilerplate, lifted `char*` code,
clones, tables) and 0.76 MB is hand-written with real types. The `U` bytes are mostly mid-sized
game logic (5.6 MB between 256 B and 2 KB), which needs real types, not generators.

Source hygiene (`tools/decl_check.py`, 2026-10-26): 11,298 `fn_`/`g_unk_` symbols declared in
1,502 hand-written files (2,355 of them the generated class-getter header); 251 with conflicting
signatures (393 on the morning of 2026-10-24, 337 after the first hub batch, 310 after the second,
308 after the TU-init symbols, 286 after the third); 1 declared in `include/` and again outside it
(76, 53, 45, 27, 6, 5; the one left is deliberate); 1,243 declared in more than one file (1,906 extra
copies). Translation units (`tools/tu_map.py`): 2,773 units from 2,760 static initialisers, 2,712
at high confidence; 11,656 `U` functions (6.0 MB) sit in a unit with a reflected class hint.

## Direction (decided 2026-10-24, owner's goal: clean, readable, playable in a few months)

1. **Measure the boot slice**, not only the whole binary: `tools/progress.py --slice boot`.
2. **Hubs first.** Pick `U` work with `tools/hubs.py` (most unmatched callers, boot slice first),
   then `--ready` functions whose callees are all matched. A matched hub with one canonical header
   unblocks dozens of typed callers (the transform hubs gave 27 callers from one header).
3. **One declaration per shared symbol**, in `include/`; `tools/decl_check.py` keeps conflicting
   signatures from growing (CI). The pilot migrated `fn_710009d50c` from 70 declarations and 12
   signatures to one header (recipe in `CONTRIBUTING.md`).
4. **Relocation targets are part of a match**: `check.py` demotes a byte-identical function that
   names the wrong callee, global or GOT slot (`RELOC` lines).
5. **Files by unit, not by address slice**: `tools/tu_map.py --function <addr>` names the unit;
   new files go under `src/<subsystem>/`; no new `src/game_<addr>/` directories.
6. **Generators are in maintenance**; the only generator work left that pays is the lifter's
   semantic mode for the port (Route B in `docs/PORT_PLAN.md`), which exists since 2026-10-25
   (`tools/port_lift.py`) and grows by closing its gaps.
7. **Logs go to `docs/JOURNAL.md`**; this page stays short. Bigger PRs (a hub cluster or about
   30 functions); the self-hosted check job replaces reading every diff (`docs/maintaining.md`).

## Workstreams

Work runs as parallel sessions, one per stream. Each stream edits only the files in its row; shared
headers under `include/` are everyone's, so a change there says so in the PR title.

| Stream | Area | Files it may edit | Branches |
|---|---|---|---|
| A. Boot slice and hubs | `tools/hubs.py --boot`: hubs, then their callers, then ready functions; canonical headers for each hub | `src/<subsystem>/` (new), the existing `src/game_*/` files it migrates, `include/` | `claude/boot-<topic>` |
| B. Core engine types | `base::` containers, string ids and pool, reflection runtime, allocators (the engine hubs) | `src/base/`, `include/base/` | `claude/core-<topic>` |
| C. Gameplay slice | After the title screen: player, world, characters along the gameplay path | `src/game/`, `include/game/` | `claude/game-<topic>` |
| D. Port layer | `port/` on `main` (Route B, decided 2026-10-24): x86-64 build, `nn::` runtime, NVN shim, audio; M0 in `docs/PORT_PLAN.md` first | `port/` | `claude/port-<topic>` |
| E. Tooling | `check.py`, `asmdiff.py`, `hubs.py`, `callgraph.py`, `decl_check.py`, `tu_map.py`, `verify.py`, CI | `tools/`, `.github/` | `claude/tools-<topic>` |
| M. Generators (maintenance) | Regenerate when needed; fix the pool bugs below; no new variant or shape rounds | `tools/gen_*.py`, `tools/refl_attrs.py`, `tools/sigmatch.py`, `src/reflection*/`, `src/clones/`, `src/lifted/`, `src/tables/`, `lib/lua/` | `claude/gen-<topic>` |

Merge rules:
- A PR is a hub cluster or about 30 functions or more, sized to finish within one session; merge
  `main` into the branch before merging. Tool and header changes other streams wait on may go alone.
- After merging `main`, take `main`'s `data/functions/*.csv`, rerun only the generator you own,
  then `tools/check.py --update` (`tools/verify.py --regen <generator>` does both). Never
  hand-merge the CSVs.
- `check.py` and `verify.py` fail when one function is defined in two files outside the Phase 3
  samples; `verify.py` runs `gen_lift.py --prune` so lifted copies drop out by themselves.
- Before the PR: `check.py --update` with no `REGRESSION`, `DUPLICATE` or hand-written `RELOC`
  lines; `decl_check.py --base origin/main` PASS; notes at the top of `docs/JOURNAL.md`.
- A stream that changes focus updates its row above in its next PR.

**Starting points for hand matching.** `tools/hubs.py --version 2.1.0 --boot` (hubs),
`--ready --boot` (functions whose callees are all matched), `--callers <addr>` (who to migrate
after a hub). `tools/gen_lift.py --version 2.1.0 --best ADDR[,ADDR...]` writes the lifter's
closest attempt to `build/lift_best/fn_<addr>.cpp` (gitignored) as a `char*` draft for
`tools/trymatch.py`. `tools/ghidra/decompile.py` prints Ghidra's view to read, never to commit.

## Hard rules (summary; full text in CLAUDE.md)

- No game data, assembly listings, decompiler output, keys or firmware in git. Game data stays in
  `orig/` and `build/`. The pre-commit hook enforces part of this; never bypass it.
- `src/` and `include/` are our own C++. Never paste Ghidra output.
- Never download game images, keys, firmware or Nintendo SDK files.
- Ask the owner before pushing or when an input is missing.
- Matching standard: aim for `O`; time-box each function to 30–60 minutes, then annotate
  `NON_MATCHING(minor|major)`. Reflection boilerplate must reach `O`.

## Repository map

```
tools/setup.py, verify_dump.py   import + hash-verify a dump into orig/<ver>/; check it against the game's records
tools/nso.py, nso2elf.py         NSO parsing; NSO -> AArch64 ELF
tools/recon/                     Python-only recon: eh_frame bounds, RTTI, Lua tables, build strings, reflection
tools/ghidra/                    PyGhidra headless driver (analyze.py), exporter (export.py), decompile.py (read only)
tools/gen_functions_csv.py       data/functions/main_XX.csv + data/reflection_types.csv
tools/fetch_toolchains.py, fetch_sources.py   upstream LLVM candidates + objdiff; Lua, libc++, dlmalloc sources (hash-pinned)
tools/compiler_matrix.py, libcheck_lua.py     Phase 3 evidence (docs/compiler.md)
tools/buildcfg.py, hostenv.py    the one place that defines compiler, flags, include paths; platform details
tools/asmdiff.py                 per-function diff vs the original; relocation targets checked (check_relocs)
tools/trymatch.py                build + diff one source, RELOC lines included
tools/check.py                   build src/, derive O/m/M/W, regressions, duplicates, relocation targets
tools/verify.py                  one-command gate: generators, check --update, progress, policy, decl_check (info)
tools/progress.py                whole-binary progress; --slice boot for the nnMain slice; --slice-from <fn>
tools/dashboard_export.py        status, daily history, phases and workstreams as CSVs for the project dashboard (build/dashboard/)
tools/callgraph.py, hubs.py      direct-call graph (Ghidra export); hubs by unmatched callers, --ready, --callers
tools/decl_check.py              fn_/g_unk_ declarations in several files or with conflicting signatures; --base enforces
tools/baseline.py                trymatch output snapshot, compare and restore for a file set (headers expand to includers)
tools/hub_migrate.py             mechanical canonical-header migration on top of baseline.py; LEFT lines are the hand work
tools/tu_map.py                  translation-unit map from .init_array + reflection groups -> data/tu_map.csv; --function
tools/objdiff_gen.py             objdiff targets + objdiff.json (gitignored) for every src/ unit
tools/shape_census.py            same-shape U families (maintenance)
tools/sigmatch.py                names vendored-library functions in main -> data/library_symbols.csv
tools/gen_reflection.py, refl_attrs.py, gen_types.py, gen_statics.py, gen_clones.py, gen_tables.py, gen_lift.py
                                 generators (maintenance); regenerate, never hand-edit their output
tools/policy_check.py, hooks/    content rules (CI and pre-commit)
.github/                         CI: policy + decl_check (hosted); check-local.yml (self-hosted check.py, inert until enabled)
src/compiler_id/, flag_id/, atk_id/   Phase 3 samples (frozen)
src/reflection/, reflection_types/, reflection_statics/, clones/, lifted/, tables/   GENERATED
src/base/                        hand-written engine code: string ids, pool, CRC64, allocator, reflected functions
src/game_*/                      hand-written game code by address slice (frozen layout; migrate by unit, no new ones)
src/lua/                         game-side Lua changes (luaB_print)
include/types.h                  u8..f64 without a standard library
include/base/                    engine headers: reflection (CClass.h, UnkReflCast.h ...), containers, string ids, clone templates
include/base/reflection/UnkClassGetters.h   GENERATED by tools/gen_class_getters.py (from data/functions/, no game data): every class getter
include/game/                    game headers shared by several files (UnkActor, UnkGameManager, transforms, TU init, graphics state ...)
tools/gen_class_getters.py, tools/port_sweep.py   class-getter header generator; host compile sweep of src/ for the port
port/                            the PC port layer: CMake project, runtime stubs (M0; see docs/PORT_PLAN.md)
lib/lua/, lib/dlmalloc/, lib/libcxx/   vendored upstream code with READMEs; lib/nnsdk, lib/musl submodules
data/                            functions/ CSVs, reflection_types.csv, tu_map.csv, library_symbols.csv, hashes, pins
docs/                            this page, PORT_PLAN.md, JOURNAL.md, recon.md, compiler.md, references.md, maintaining.md
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
.venv/Scripts/python tools/fetch_sources.py                        # Lua 5.1.x, libc++ 9.0.1 headers, dlmalloc
git submodule update --init                                       # lib/nnsdk, lib/musl
.venv/Scripts/python tools/sigmatch.py --version 2.1.0             # then rerun gen_functions_csv.py
.venv/Scripts/python tools/gen_reflection.py --version 2.1.0
.venv/Scripts/python tools/gen_types.py --version 2.1.0            # ~20 s
.venv/Scripts/python tools/gen_statics.py --version 2.1.0          # ~6 min
.venv/Scripts/python tools/gen_clones.py --version 2.1.0           # ~10 s
.venv/Scripts/python tools/gen_tables.py --version 2.1.0           # ~5 s
.venv/Scripts/python tools/gen_lift.py --version 2.1.0             # ~80 min; run after the other generators
.venv/Scripts/python tools/tu_map.py --version 2.1.0               # data/tu_map.csv, ~10 s
.venv/Scripts/python tools/gen_class_getters.py                    # include/base/reflection/UnkClassGetters.h, instant
.venv/Scripts/python tools/check.py --version 2.1.0 --update       # ~2 min, parallel
.venv/Scripts/python tools/objdiff_gen.py --version 2.1.0          # then open the repo in objdiff
.venv/Scripts/python tools/progress.py --slice boot
```

The tools also run on Linux (upstream clang+llvm 9.0.1 with a `bin/clang++.exe` symlink to
`clang++`, `llvm-cxxfilt` on PATH; results identical to Windows) and, untested on hardware, on
macOS under Rosetta 2 (`AGENTS.md`).

## Key facts

- **Modules:** `main` (game, 19.1 MB of code), `sdk` (NintendoSDK 11.4.3, NVN, libz), `subsdk0`
  (`nn::movie`), `rtld`. `main` imports 526 SDK symbols; by namespace: 152 libc, 149 `nn::audio`,
  55 `nn::os`, 37 libc++, 34 `nn::hid`, 26 `nn::nfp`, 22 `nn::fs`, 13 `nn::oe`, the rest small.
  NVN is reached through `nvnBootstrapLoader` and a function-pointer table.
- **Statically linked into `main`:** `nn::atk` (~1,900 functions from 0x71012046c0 to the end of
  `.text`, status `L`), **Lua 5.1.4** with `float` numbers built as C++ (0x71010a0880–0x71010bee20),
  dlmalloc 2.8.4 mspaces (0x71000815b0–0x7100082c38), about 1 KB of Dear ImGui.
- **Game code:** no RTTI, no C++ exceptions, unwind tables everywhere, PIC with GOT access to
  vtables and globals, no compiler-introduced SIMD. The linker folds identical bodies.
- **Translation units:** `.init_array` holds 2,760 static initialisers in address order, one per
  TU with dynamic initialisation, each emitted last in its TU. The reflection boilerplate (attribute
  helper, then the class getter) opens a TU. `data/tu_map.csv` has the resulting 2,773 units.
- **Mercury reflection** is the main naming source: every reflected type `T` has a guarded getter
  (`T::refl_GetClass`) and a descriptor constructor (`T::refl_InitClass`) storing `sizeof(T)` at
  `+0x18`. Layout in `include/base/reflection/CClass.h`. The getter's x3/x4 functions add class
  attributes and register member fields (`fn_7100096524`); an attribute is `attrs->Add<T>(KEY(), value)`.
  `UnkReflName` is `base::global::CStrId`. Two string-id types share one constructor
  (`UnkStrIdBase` with `CStrId` and `UnkStrId` on top). `UnkVector<T>` (0x28 bytes) is the engine's
  growable array. Details and matching facts: `docs/JOURNAL.md` ("Key facts" of earlier sessions
  are kept there verbatim).
- **Memory:** 1.82 GB `.bss`; the engine reserves its memory statically. **Codename:** "cazadora".
- **Game data in RomFS:** 5,754 `.bctex` textures, 478 `.bshdat` shader packages (each with its
  programs' GLSL source beside the compiled Maxwell code: 1,342 programs, vertex and fragment only;
  the port compiles the sources with glslang, `docs/PORT_PLAN.md`), 1,276
  `.bfstm` audio streams, 101 WebM videos, 72 `.pkg` packs; parsing in mercury-engine-data-structures.

## Decisions made (and why)

| Decision | Reason |
|---|---|
| Upstream Clang **9.0.1** with the flags in `tools/buildcfg.py` | Each flag is backed by a fingerprint and matching results (`docs/compiler.md`); the SDK compiler is off-limits |
| Status codes from **zeldaret/botw** (`O m M W U L`), derived by `tools/check.py` | No hand-edited outputs; `O` regressions fail the check; `L` is never overwritten |
| **A match includes its relocation targets** (2026-10-24) | `asmdiff` masks relocated fields, so a function was `O` with the wrong pool or callee; two such slips were found by hand. Generated files are reported only until their generators are fixed |
| `L` for `nn::atk`, SDK, runtime, Lua stdlib, dlmalloc | Library code isn't game progress; `nn::atk` is Nintendo middleware and stays undecompiled (the port replaces or lifts it) |
| 30–60 minute time-box per function, except reflection boilerplate | Owner-approved matching standard |
| Function list split per MiB; CSV statuses regenerated, never hand-merged | 1 MB pre-commit limit; parallel streams |
| objdiff per `src/` unit with synthesized targets; whole-binary and boot-slice progress from the CSV | No byte-identical relink is possible on Switch |
| Lua, dlmalloc, one libc++ `__sort` from upstream source (approved) | Open source; identified by diffing |
| `nn::` declarations from **open-ead/nnsdk** as a submodule | Community headers, no license: referenced, never copied; the port must not depend on them without asking |
| Generated code (reflection, statics, clones, tables, lifted) is regenerated, never edited | Thousands of instances of one template; byte-identical by construction |
| **Hub-first work order, canonical headers, files by unit** (2026-10-24) | The remaining `U` is typed game logic; per-file declarations (382 conflicting signatures) can never link; the port needs one program |
| **Generators in maintenance** (2026-10-24) | `U` under 256 B is 0.9 MB; the lifter's byte-identical mode is at its limit (journal, item 1) |
| **Handoff split into state and journal; PRs of 30+ functions; self-hosted check job** (2026-10-24) | Every session read a 28k-token log; 178 PRs in six days at a median of 10 functions and 3 minutes to merge |
| **Class getters declared once in a generated header** (`UnkClassGetters.h`, 2026-10-24) | 243 hand files declared getters locally as `void*`; the CSV knows every getter, so the header needs no game data and never drifts |
| Route to a running build: **Route B** (`docs/PORT_PLAN.md`, 2026-10-24) | One program now, lifted code as a bridge that matching replaces; the port lives in `port/` on `main` |

## Open decisions for the owner

1. ~~Route A or Route B.~~ Route B (2026-10-24): stream D starts with M0, and the lifter gets a
   semantic coverage mode for the port (stream M's one remaining generator task).
2. `tools/decl_check.py` strictness: today CI fails a PR that adds a conflicting signature or a
   local prototype of a symbol `include/` declares, and only warns about one more identical copy
   (`--strict` fails those too). The precondition is met: the 13 symbols `include/` itself declared
   twice were reconciled on 2026-10-24, so `--strict` in CI is the owner's call.
3. ~~The frozen Phase 3 sample `src/flag_id/fn_71004dd360.cpp` names a global where the original
   holds a function address.~~ Fixed 2026-10-24 (it stores a function's address; bytes unchanged).
4. ~~Register a runner and enable the self-hosted check job.~~ Done 2026-10-24 (runner `dread-pc`,
   `docs/maintaining.md`). It cannot be a *required* check while the repository is private without
   GitHub Pro: merge on green by hand until the repository is public.

## Next steps

**Stream A (boot slice and hubs).** Round 1 (2026-10-26, journal) matched the eight top hubs of
2026-10-25 and two siblings (the string-pool holder, the `CBlackboard` section lookups,
`CRntFile`'s seek, the navmesh cell's edge lookup, the sound command queue, the game manager's id
map), four agents on disjoint files, each hub in a file named for its class with one header. Next,
from `tools/hubs.py --version 2.1.0 --boot` (2026-10-26): `fn_710000542c` (264 B, 21 unmatched
callers; declared in `UnkCloneRecords.h`), `fn_7100ee1f34` (2,096 B, 19), `fn_710023b580` (564 B,
19), `fn_710001a270` (300 B, 19, the `CFilePathStrId` constructor), `fn_7100deba8c` (316 B, 18),
`fn_7100fd3804` (1,900 B, 17), `fn_7100deabe4` (32 B, 16), `fn_710007e7f8` (616 B, 16),
`fn_7100087388` (272 B, 15), `fn_710009dcd8` (44 B, 15), `fn_7100defcec` (28 B, 15); the round's
own follow-ups: `fn_7101047414` and `fn_7101047484` (byte-identical to the `CBlackboard` lookup),
`fn_7100080578` (the string pool's next piece), `UnkHashStream_0x38` is `CRntFile` (its
functions `fn_710007be8c`, `fn_710007bf9c`, `fn_710007c3f0`, `fn_710007c1a4`, `fn_710007c0e4`),
`UnkGameManager`'s `field_0x968` is an embedded `CBlackboard`. Binary-wide the largest are
`fn_71010c0270` (120 B, 354 unmatched callers, almost all `lua_bind::*`) and `fn_71006f4b0c`
(592 B, 211). 824 boot-slice `U` functions (259 KB) are "ready" (`--ready --boot`); 6,479
binary-wide (1.9 MB).

Canonical headers. Batch 1 (2026-10-24, #184): the 13 symbols `include/` declared twice, the
actor's target getter (`include/game/UnkActor.h`), the matrix getter (`UnkMatrix44.h`), the
counted-resource free (`UnkCountedResource.h`), the game manager (`UnkGameManager.h`) and every
class getter (generated `UnkClassGetters.h`). Batch 2 (2026-10-25, #187), 25 symbols: the five
`CRntString` operations (`CRntString.h`); the `CStrId` constructor from a `CRntString`, the
global empty id and the string pool (`CStrId.h`); the reflected-value builder (`UnkReflValue.h`)
and the blackboard qualified id (`include/game/UnkGameBlackboard.h`); the animation component's
class test and track progress (`include/game/UnkAnimationComponent.h`) and the GUI focus setter
(`UnkGuiDisplayObject.h`); the type-erased array copy (`include/base/UnkErasedVector.h`), the two
handle assignments (`UnkRefHandle.h`) and the render manager (`include/game/UnkRenderManager.h`);
the sphere position setter (`UnkSphere.h`), the scene toggle notifiers
(`include/engine/scene/UnkToggle.h`), the converter registration (`UnkReflConverter.h`), the
property registration (`UnkReflStatics.h`), the AI component's navigation getter
(`UnkAIComponent.h`) and the nav-mesh manager (`UnkNavMeshManager.h`). Five matched on the way:
`fn_7100216978`, `fn_71001eef58`, `fn_71001eef74`, `fn_7100ebf278`, `fn_7101177ea0` (and
`fn_71011cf2a4` as `m`). The batch ran six file-disjoint steps in parallel and five overlapping
ones in sequence (`tools/trymatch.py --obj-dir` keeps parallel objects apart).

Batch 4 (2026-10-26) gave every conflicting symbol with a hand-written definition one declaration
with the definition's types (29 symbols, 19 new headers such as `UnkTimelineComponent.h`,
`UnkCollisionRecord.h`, `UnkSpitAttack.h`, `UnkGuiHandle.h`; journal). Next, by
`tools/decl_check.py --list`: `fn_71004f3ea4` (4 signatures in 3 files), `fn_710007b430`,
`fn_71000127c8`, `fn_71004f11f8`, the GUI functions `fn_71011b6bbc`, `fn_71011d8e90`, `fn_71011d968c`
(3 each). Batch 4's own follow-ups: `fn_7100a2c13c`, `fn_7100a2cfe8` and `fn_7100a2cb7c` (whose
`UnkReflStatics.h` declaration disagrees with its definition) into `UnkTimelineComponent.h`;
`fn_71001c28f8` into `UnkSlots.h`; the 0x60-byte record's `PushBack` `fn_7100e52cf4` (5 files) into
`UnkFreedVectorPair.h`; `fn_7100125e78`, `fn_7100125b1c` onto `UnkSegment2D.h`; `UnkAttack.h` and
`src/game_5821e4/CAttack.h` into one `CAttack`; `gen_tables.py` should use `UnkRenderManager.h`'s
`fn_7100219ae0` (its video map declares it `void`). Older follow-ups:
`fn_710021693c` and `fn_7100216d30` into `UnkSphere.h`; `fn_71004fa3b8` (9 identical local
declarations); the `UnkActor` slots at +0x240 and +0x298 typed `UnkAIComponent*` and
`UnkNavigation*`; `fn_7100394bd4` (2 signatures); `fn_7100ad3588` into `UnkErasedVector.h` and
`fn_71006eb8cc` into `UnkRefHandle.h`; `fn_710001a270` and the string neighbours `fn_710007ea60`,
`fn_710007e6c4`; `InitPropertyEx`'s `on_init` parameter type in `UnkTUInit.h` (one caller casts);
the render manager's accessors into `UnkRenderManager.h`. From the port link: 182 names that
resolve to no address (`UnkIdTable::UnkIdTable()`, `UnkMapIter<...>::Advance()`, ...) need an
`fn_<addr>` name; three lifted callers pass a `float` to a `char*` definition (`fn_7100005c40`,
`fn_7100573200`, `fn_7100a1fe98`); `src/game_s7/fn_71011bc0b0.cpp` defines `fn_71011bc830` by an
`asm(".set ...")` alias to a libc++ `__sort3` instantiation that an x86-64 compile does not emit.
Leftovers that keep a second declaration on purpose: `src/game_s2/fn_71003f1b00.cpp`
(`u32`-returning `fn_710009d50c`, load-bearing for its `m`), ten callers of `fn_7100d7c990` with
their own word-typed views of the actor's translation, two callers of `fn_710001737c` with a local
count view; `src/game_s7/UnkGuiS7.h` keeps `using UnkClass = CClass` for two includers.

The mechanical part of a migration is a tool since 2026-10-25: `tools/hub_migrate.py` removes the
local declarations, includes the header and keeps only byte-identical files (`tools/baseline.py`
is the per-file evidence, parallel, with copies to restore from). Two passes so far. The first
(var-flotsam/dread-decomp#190) migrated 66 files for 15 symbols in a minute. The second (branch
`claude/tuinit-unify`) first moved the symbols both static-initialiser helper families name into
two small headers they both include: `include/base/reflection/UnkReflTypeDtors.h` (the type
objects' destructors `fn_7100096604`, `fn_7100099468`, `fn_710009891c`, `fn_710009a368`,
`fn_710010c424`; `base/UnkCloneHelpers.h` includes it too) and `include/game/UnkTUSymbols.h` (the
function records `fn_710000cf00`/`fn_710000d470`/`fn_710000d4f4`, `UnkPropertyHead` and the property
list head `g_unk_71726db788`); `unk_static_init`'s property structs point `next` at
`UnkPropertyHead` and its helpers take the head as `UnkPropertyHead*&` (pointer retypes only; the
helper bodies and their store order are untouched, because they are load-bearing). That unblocked
96 files (the class-type destructor alone was declared locally in 57), all 195 includers of the two
families byte-identical (`baseline.py`, tag `tuinit`). Batch 3 (2026-10-26) hand-migrated the 40
files the tool left (placeholder types renamed to the headers' real ones, five agents on disjoint
file sets, about 7 minutes; journal). The two helper families now share one set of property and
tunable helpers, `include/game/UnkTUProperty.h` (2026-10-26, branch `claude/tuinit-fold`): each
property initialiser in both store orders (`InitProperty`/`InitPropertyEx` link first,
`...VtableFirst` store the vtable first; the source order steers the scheduler). What the families
still hold apart is the TU header data (`InitHeader*`), the type registrations (`InitEnum`,
`InitFlagSet`, `InitClass`, `InitVector` vs `unk_static_init`'s) and two placeholder views of a
type object (`UnkReflType` vs `UnkEnumType`); folding those is the next readability step, verified
the same way (`baseline.py`, every includer of both families). Cheap follow-up: 729 calls in unit
files still spell the atexit stub `__cxa_atexit`, which `check.py` cannot resolve; `fn_7100000250`
makes each a checked relocation (the helper headers' 24 sites added 1,055 checks, 0 mismatched).

**Stream B (core engine types).** Left from the string-id and reflection work: the reflected
function array's growth (`fn_7100097f80`), the function's constructor and destructor
(`fn_710009a5b4`, `fn_710009aae8`), the allocator functions next to `operator new`
(`fn_7100083870`, `fn_71000839c0`, `fn_7100083a10`, `fn_7100083a70`), the string pool's remaining
`m` functions (insert `fn_71000801e4`, compare `fn_710007f240`, release `fn_71000803a4`). CRC64
and the string-id constructors sit next to `nnMain`, so `gen_functions_csv.py` infers `runtime?`
and marks them `L` although they match (`fn_7100000850`, the constructor from a `CRntString`,
matches since 2026-10-25 and still reads `L`); fix that inference.

**Stream E (tooling).** Done 2026-10-25: `tools/baseline.py` (trymatch snapshots for a file set,
compare, restore; headers expand to their includers; `--obj-dir` per source so agents never
collide) and `tools/hub_migrate.py` (the mechanical hub migration on top of it, with a `BROKEN`
guard for files that do not compile before the edit), plus the batch procedure in
`.claude/skills/dread-hubs`. Done 2026-10-24: every generator verifies relocation targets
(`asmdiff.diff_object(strict=True)`), the three generator target bugs are fixed (`gen_clones`'s
destructor scan now reads each string-id member's pool, `gen_statics`'s fact extractor drops
stale register state, `gen_tables` reads the video map's globals from the GOT) and `check.py`
enforces relocation targets everywhere. Next: a dynamic boot trace from Eden's GDB stub to
replace the static lower bound; directory moves driven by `data/tu_map.csv` once a unit is
complete (one rename PR per unit); `tools/gen_functions_csv.py` subsystem inference from
`tu_map.csv` instead of neighbours (68,731 functions are still "unknown"); `trymatch.py`-style
relocation checking in `tools/nearcopy.py`'s candidates if it compiles anything; `check.py`'s
definition rule counts a function pointer in a template argument (`UnkFoo<&fn_<addr>>`) as a
definition of that address, which `tools/port_link.py` already excludes.

**Stream D (port).** M0 is done (#183). M1 step 1 is done (2026-10-25, #186): `tools/port_link.py`
links the 3,804 sweep objects relocatably (`ld.lld -r`, no duplicate symbol, 60,402 undefined),
classifies what is missing, generates `--defsym` aliases (37,531 references to functions defined
under another symbol: the declaration debt and the lifter's `callee::` convention), absolute data
symbols (15,450 `g_unk_` addresses) and trap thunks, and links again to a residue of 304 (71
vtables, 49 standard library names, 2 `nn::hid` destructors, 182 names without an address).
M1 step 2 is done (2026-10-25): `lib/lua` and `lib/dlmalloc` are compiled into the link and the
build (`dread_libs`), and `fn_7100000250` (`__cxa_atexit`) jumps to the host's. On `main` after hub
batch 2 the boot slice's 4,211 game functions are 1,109 defined, 941 reached through an alias only,
4 the host's and **2,157 without a source** (1.24 MB; 2,148 `U`, 8 `L`, one `O`), listed by
callers in `build/port/link/boot_missing.csv`, the lifter's coverage worklist. The residue is 375
(the 304 above plus Lua's C library calls). M1 step 3 is done (2026-10-25): `tools/port_lift.py`
lifts the slice's `U` functions in `gen_lift`'s semantic mode and accepts a lift when Ghidra-P-code
execution of it and of the original agree (`tools/pcode_emu.py`, oracle (b); design and limits in
`docs/PORT_PLAN.md`, usage in `port/README.md`). 1,777 of 2,148 pass (839 KB); with
`tools/port_link.py --lifted build/port/lifted` the slice has 380 functions without a source. The
gaps step (exact flags, jump tables, float pass-through, four oracle fixes) brings it to 1,941
lifts (1.04 MB) and **216 slice functions without a source** (196 KB, `boot_unlifted.csv`). The
oracle covers a median 67% of an original's instructions, so the next lifter work is input
coverage and the remaining gaps: stack arguments (23), flat-mode register order (14), `x8`
results in tail position (14), other jump-table shapes (9), and the 104 functions the oracle
rejects (`tools/port_lift.py --debug ADDR` shows the first difference). Next for M1,
in order: `nn::` stubs in call order (`nn::os` 16 and `nn::oe` 1 still
needed, then `nn::fs`, `nn::hid`, `movie`, `nv`); the data loader that maps the dump's segments at
their original addresses (5,467 data symbols are referenced PC-relatively, so the image must sit
within 2 GB of `0x7100000000`). Producing an executable needs WSL or the MSVC Build Tools.

**Stream M (generators).** The bugs above and `--prune`/regeneration after merges. The lifter's
semantic mode exists (`opts={"semantic"}`, driven by `tools/port_lift.py`; byte mode is unchanged,
checked text for text on 4,000 candidates): its gaps are listed under stream D. A `src/lifted/`
concern it surfaced for the port: byte-identical lifts can rely on argument registers a call does
not name (the AArch64 bytes are the same, an x86-64 compile is not bound to them), and the
`lift_guard` helpers construct statics of their own with a constructor that has no address. Open leftovers of every generator round
are recorded in `docs/JOURNAL.md` (work log items 0–3).

## Known issues and gotchas

- **Linux:** identical results with upstream clang+llvm 9.0.1 (see Rebuilding). **Windows PATH:**
  shells started before the toolchain install may not see `cmake`/`ninja`; use `.venv/Scripts/python`
  and absolute tool paths, as the scripts do.
- **Ghidra project lock:** only one process can open `build/ghidra/<ver>/dread.gpr` at a time.
  `tools/ghidra/decompile.py` fails from a worktree under `.claude/` (Ghidra rejects a path element
  starting with a dot): run it from the main checkout.
- `llvm-readelf` isn't in the LLVM Windows build; use `llvm-readobj --elf-output-style=GNU`.
- Constructors compile to two aliased symbols (C1/C2). `check.py` counts them once (by address);
  objdiff counts both.
- **Relocation check limits:** section-relative references (switch tables, string literals),
  conditional branches and unresolved symbols are not checked (about 128,000 relocations), so a
  wrong string literal is still not caught. A callee with no address is unresolved: write the
  atexit stub as `fn_7100000250`, not `__cxa_atexit`. Every generator verifies targets itself
  (`asmdiff.diff_object(strict=True)`), so a `RELOC` line in a generated file means a generator
  regression: fix the generator, never the output.
- **`decl_check.py` is a brace-structure heuristic**, not a parser: a brace-init default argument
  drops that prototype, every `#if` branch is scanned, and only the first declarator of a
  multi-declarator prototype is recorded (none of these occur today). Attributes, `noexcept` and the
  `HIDDEN` macro are not part of a signature.
- **`tu_map.csv` confidence:** 13 initialisers register a class whose getter is in a neighbouring
  unit; 107 units are just their initialiser (their code was folded or inlined away); 38 units over
  64 KB certainly hold several TUs. Cite address ranges with unit indices, which renumber on
  regeneration.
- **`check-local.yml`** runs `check.py` on the runner `dread-pc` for every PR from this
  repository (`SELF_HOSTED_CHECK` is set; `docs/maintaining.md`). A fork's PR skips it (counts as
  passing), so fork PRs still need a local `check.py`.
- objdiff targets: call targets are verified against the original, but data references are
  mirrored from our object without verification. String-literal relocations point at an undefined
  `.rodata.str1.1` symbol in the target.
- C/C++ headers are musl 1.2.2 (`lib/musl`) and libc++ 9.0.1 (fetched). musl's `BUFSIZ` (1024) sets
  Lua's `luaL_Buffer` size.
- `CRodomithonXSuckAttack`'s recovered `sizeof` (0x40) is smaller than its parent's (0x190); treat it
  as unreliable. 42 reflected types have no identified descriptor kind.
- Subsystems ending in `?` are guesses, and some `L` markings depend on them.
- `fn_710083ecf8` needs `fmin` + `b.lt` where Clang emits `fminnm` + `b.mi`; it may be a shared
  math helper.
- `tools/gen_reflection.py` needs the project compiler (it verifies candidates by compiling them);
  its output is deterministic. Generated sources contain the string literals the original uses.
- In the Windows Bash tool, heredocs can lose backslashes; write patches to Python sources with
  the Edit tool.
- **Leftovers for the owner to delete by hand:** `C:\Program Files\Git\tmp_bak.cpp` (a stray copy
  of `src/compiler_id/fn_7100162300.cpp`) and the empty folder named `dread` next to this
  repository (locked by a running shell).
