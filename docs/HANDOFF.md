# Handoff: dread-decomp

The current state of the project, for the next developer, human or AI. Read this, then `CLAUDE.md`
(rules and conventions) and `docs/PORT_PLAN.md` (the route to a running PC build and the decisions
it needs). History, batch logs and the index of matching findings are in `docs/JOURNAL.md`;
`docs/recon.md` describes the binary, `docs/compiler.md` the toolchain, `docs/references.md` the
outside projects. This page is edited in place, never appended to: session notes go to the journal.

**Last updated 2026-10-27** (port: the runtime's import names on Windows, branch `claude/port-windows-imports`; lifter: arguments the byte-identical lifts dropped, branch `claude/lifter-passthrough`; port: the Windows wide-text helpers, #229; the Windows compile check, #225; port M1: boot-match round 2 and the boot past the game's main function, branch `claude/port-m1-game-main`; the boot-match batch and the boot past the system start-up, #223; port M1 step 4: runtime stubs and the data loader, #201, #203, #206; hub batches 3 and 4, var-flotsam/dread-decomp#195 and #197; the TU helper fold, #196; the shader packages, #198; boot hubs round 1, #199; port M2's shader build step and NVN binding layer, #202 and #204, and its drawing part, branch `claude/port-nvn-draw`; hub batch 5, #207; hub batch 6, branch `claude/hub-batch-6`; boot hubs round 2, branch `claude/boot-hubs-2`).
Target: **Metroid Dread 2.1.0**, `main` build ID `646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.

## Start here

- **Open work:** `gh pr list` is the source of truth for what is waiting to merge; the lifter (stream M:
  `abi_facts.py`, the semantic mode, its oracle and worklist) runs in its own session on
  `claude/lifter-*` branches (after `claude/lifter-passthrough`: the semantic gaps, the oracle, the
  wider worklist).
- **Next hub batch:** `tools/hub_migrate.py --header-redeclared` has nothing left but one deliberate
  keep (`src/game_s2/fn_71003f1b00.cpp`), and every conflicting symbol with a hand-written definition
  has one declaration (batch 4). The 156 conflicting symbols left (batch 6, 2026-10-26) have no
  hand-written definition, so the callers' types, `tools/abi_facts.py` and Ghidra's view decide the
  signature, and most need a shared type in a new header. `tools/decl_check.py --list` ranks them. Work them per
  `.claude/skills/dread-hubs`: the tool first (`hub_migrate.py --plan batch.tsv` with every chosen
  signature, one build for all), agents on disjoint file sets for what it leaves, verified with
  `tools/baseline.py` (`save --tag <batch>-full --all` before the agents start: two minutes).
- **Next boot hubs:** `tools/hubs.py --version 2.1.0 --boot` (round 3's list and round 2's
  follow-ups are under stream A below). Rounds 1 and 2 ran four and five agents, one per hub group
  on disjoint files (grouped by `tools/tu_map.py` unit), each with the brief "match it (`dread-match`
  loop, time-boxed), one header, migrate the declaring files"; the whole-tree baseline before,
  then the orchestrator settles the declarations the agents left, runs `check.py` once, and has an
  adversarial review check the headers and every `NON_MATCHING(minor)` claim.
- **Port:** `python tools/port_link.py --version 2.1.0` rebuilds `build/port/link/boot_missing.csv`, the
  boot-slice functions without a source (the lifter's worklist), the runtime stubs still needed and the
  data loader's tables; `python tools/test_port_loader.py --version 2.1.0` checks the loader on the dump.
  Graphics (M2): `cmake --build build/port/cmake --target shaders dread_nvn` compiles the dump's
  shaders to SPIR-V with `shaders.manifest` and the NVN shim (`port/nvn/README.md`), which now
  implements every NVN function the game calls; `nvn_startup_check` draws every program on lavapipe.
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
| Generators: lifted, statics, clones, tables | **In maintenance.** `src/lifted/` 46,443 functions (2026-10-27, with argument pass-through); `src/reflection_statics/`, `src/clones/`, `src/tables/`. The lifter is at its limit for byte-identical output (see the journal); no new variant rounds |
| 4.4 Core engine types and hand matching | **In progress.** String ids and pool, `UnkVector`, CRC64, allocator getter, reflected-function array; hand slices in `src/game_*/` (0.76 MB typed) |
| 4.5 Boot to title screen | **Measured, not started as a slice.** 4,441 functions reachable from `nnMain` by direct calls; see the table below |
| Port | **M0 done** (var-flotsam/dread-decomp#183): `tools/port_sweep.py` compiles all 3,808 `src/` files for x86-64 once `port/runtime/include` stands in for the SDK headers; `port/` CMake project with `dread_game`, `dread_runtime` (our own `nn::` declarations and stubs) and the banner executable. **M1 steps 1 and 2 done** (2026-10-25, var-flotsam/dread-decomp#186 and the vendored-libraries step): `tools/port_link.py` links the whole program, Lua and dlmalloc included, relocatably and lists what is missing (2,157 boot-slice functions without a source). **M1 step 3 done** (2026-10-25): `tools/port_lift.py` gives 1,777 of the slice's 2,148 `U` functions a semantic lift checked by a P-code oracle (`build/port/lifted/`, never `src/`); with them 380 slice functions are left without a source; the gaps step raises that to 1,941 lifts and 216 left. **M2 started** (2026-10-26, #202, #204): the `shaders` build step (all 1,342 stages to SPIR-V, checked against NVN's reflection) and `port/nvn/`, the NVN shim's binding layer over Vulkan, compile-checked. **The Linux executable** (2026-10-26, #214, Docker): on 2026-10-27 the boot runs `nninitStartup`, every static initialiser, `nnMain`, the system start-up and the game's main function `fn_7100000980` into the start-up `fn_7100015c70`, where it reaches functions without a source (127 by direct calls from the boot's run-time roots, more through pointers). **Windows: compile only** (2026-10-27): all 3,979 sources compile with llvm-mingw (`port_sweep.py --target x86_64-w64-mingw32`); Linux stays the only running target until the title screen |

Numbers from `tools/progress.py` and `tools/progress.py --slice boot` on 2026-10-27 (the lifter's
pass-through regeneration, branch `claude/lifter-passthrough`, after #224 and #225):

| Scope | Functions counted | Bytes | `O` matching | `O`+`m` usable | `U` left |
|---|---|---|---|---|---|
| Whole binary (library `L` excluded) | 102,398 | 18.77 MB | **54.54%** (85,005; lifter pass-through, 2026-10-27) | 56.91% | 17,007 functions, 8.09 MB |
| Boot slice (reachable from `nnMain`, direct calls only) | 4,308 | 1.71 MB | **29.41%** (2,025; lifter pass-through, 2026-10-27) | 39.31% | 2,126 functions |

Of the 10.1 MB matched, about 9.4 MB is generated (reflection boilerplate, lifted `char*` code,
clones, tables) and 0.76 MB is hand-written with real types. The `U` bytes are mostly mid-sized
game logic (5.6 MB between 256 B and 2 KB), which needs real types, not generators.

Source hygiene (`tools/decl_check.py`, 2026-10-26, after hub batch 6): 11,347 `fn_`/`g_unk_` symbols
declared in 1,545 hand-written files (2,355 of them the generated class-getter header); 156 with
conflicting signatures (393 on the morning of 2026-10-24, 337 after the first hub batch, 310 after the
second, 308 after the TU-init symbols, 286 after the third, 249 after the fourth, 220 after the fifth
and boot hubs round 2); 1 declared in `include/` and again outside it (76, 53, 45, 27, 6, 5; the one
left is deliberate); 1,145 declared in more than one file (1,574 extra copies). Translation units (`tools/tu_map.py`): 2,773 units from 2,760 static initialisers, 2,712
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
- **Game data in RomFS:** 5,754 `.bctex` textures, 478 `.bshdat` shader packages (671 programs,
  each a vertex and a fragment stage whose debug section carries its GLSL source; parsed and
  extracted by `tools/bshdat.py`, compiled with glslang, `docs/PORT_PLAN.md`), 1,276
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
| **Vulkan first; shaders compiled to SPIR-V at build time** from the user's dump (owner, 2026-10-26; `docs/PORT_PLAN.md` decision 3) | NVN's object model maps onto Vulkan, the binding layout is already worked out for it, and it runs on all three systems; the sources need the NVIDIA-only `GL_NV_gpu_shader5` under OpenGL. A build step (`tools/bshdat.py --compile`) keeps a compiler out of the executable; its output stays local game-derived data |

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
5. ~~Graphics backend and when shaders compile.~~ Vulkan first, SPIR-V at build time (2026-10-26).

## Next steps

**Stream A (boot slice and hubs).** Round 1 (2026-10-26, #199) matched the eight top hubs of
2026-10-25 and two siblings; round 2 (2026-10-26, journal) took the next 18 boot hubs and round 1's
follow-ups: 14 `O`, 10 `NON_MATCHING(minor)`, one dropped, each in a file named for its class or
unit with one header (`CRntFile`, `CFilePathStrId`, `CType`, `CGameManager`, `CBlackboard`,
`CGameBlackboard`, the music, model and rumble managers, the file device, the registry). Next,
from `tools/hubs.py --version 2.1.0 --boot` (2026-10-26, after round 2): `fn_7100ee1f34` (2,096 B,
19 unmatched callers; the `CSubAreaManager` unit's 0xae8-byte object, dropped in round 2, layout
notes in the journal), `fn_71000ead80` (416 B, 13), `fn_71000ea448` (2,032 B, 12),
`fn_7100076acc` (544 B, 12), `fn_710007c1ac` (476 B, 12, the `CRntFile` unit), `fn_710009d52c`
(168 B, 12, `CType`'s unit), `fn_7100087850` (160 B, 11, the registry's lookup), `fn_71011a2fbc`
(20 B, 11), `fn_710027a33c` (204 B, 11), `fn_7101196fd8` (48 B, 11), `fn_7100decab0` (428 B, 10,
the game manager's target getter, declared in `UnkGameManager.h`). Round 2's own follow-ups: a
second pass on its low-scoring minors (`fn_710023b580` 10.6%, `fn_710009d7a8` 16.4%,
`fn_7100deba8c` 16.5%, `fn_710007c714` 23.8%, `fn_7100fd3804` 24.8%, `fn_710007e7f8` 43.5%);
the music manager singleton `g_unk_71726bc560` still has three local types (five files, two of
them with their own view of the 0xf18-byte object); `UnkGameManager` lacks the map at +0x1a70 that
`fn_7100e039e4` reads through its own view; `src/game_aca57c/fn_7100ea56f0.cpp`'s `UnkSuffix` is a
`CRntString`; `fn_710008b88c` has no header. Binary-wide the largest are `fn_71010c0270` (120 B,
354 unmatched callers, almost all `lua_bind::*`) and `fn_71006f4b0c` (592 B, 211). 895 boot-slice
`U` functions (300 KB) are "ready" (`--ready --boot`).

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
`UnkCollisionRecord.h`, `UnkSpitAttack.h`, `UnkGuiHandle.h`; journal). Batch 5 (2026-10-26, journal)
chose signatures for 34 symbols without one (Ghidra, `abi_facts.py`, the callers), among them the
deferred set's add and remove, the GUI root (`UnkGuiRoot.h`: layout, global, five root functions),
the display-object container, the matrix product and scale, the segment's normal and constructor,
the timeline component's lookups, reflection type globals, the suit properties, behaviour-tree
nodes, and moved `CAttack.h` to `include/game/`. Batch 6 (2026-10-26, journal): about 70 symbols and batch
5's review follow-ups, among them every `ITEM_` property (48, `UnkItemProperties.h`, with
`ItemQualifiedId`/`ItemSource` instead of `SuitAs`), the reflected type globals
(`UnkReflTypeGlobals.h`; `g_unk_7101d5e2e0` had 57 copies), the action-node base constructor
`fn_7100393e68` (27 nodes on `UnkBehaviorTreeNodeBase`), the boss AI component, the sphere's radius
setter (16 files), the weak-handle unlink, the GUI event, child lookup and key list, and
`UnkGuiFocus`/`CAttack.h`'s handle types onto `UnkWeakHandle.h`; `UnkReflStatics.h` includes the small
`UnkTimelineCalls.h`. Next, by `tools/decl_check.py --list`: `g_unk_71726bc570` (the stats global, 3
views), `g_unk_71727bd078` (a tunable), `fn_7100ee1f34`, `g_unk_7101d47360` and `g_unk_71726bc560`
(managers; `fn_7100ee1f34` is also a boot hub), `fn_7100010e58`, `fn_7100017390`, `fn_71002aeb9c`,
`fn_71003ac8f4`, `fn_7100ad61f8`, `g_unk_7172794d40` (2 signatures each). Batch 6's own follow-ups
(its review): `src/game_5821e4/CAttackDtor.h`'s `UnkAttackOwner` is a second view of
`UnkWeakHandle` (and `fn_710063904c.cpp`'s `fn_71010cb2fc(void*, void*)` is why
`UnkAttackListenerRaw::Destroy` is a template); the three `src/game_s7` callers of `fn_71010c4250` cast
their `UnkProp` (`src/game_s7/UnkPropS7.h`) to `UnkComponentProp`: move it into the header; two action
nodes (`fn_710037a410`, `fn_710037ca84`) cast to the node base instead of deriving; the lifted
`fn_7100394bd4` is defined without parameters, the header takes `void*` (port aliasing by address);
`UnkStrIdTable` and `CBlackboard.h`'s `UnkPropertyArrays` are one parallel-array template with two
strides; the GUI callers cast their own views to `UnkGuiDisplayObject*` (derive them instead).
`src/game_aca57c/fn_7100b2c6a0.cpp` calls `fn_71011d968c` through an untyped function pointer and
`fn_71011cde44` typed (its minor functions went from 98.2% to 96.5% for that; an asm-labelled second
declaration would have kept the bytes but hides from `decl_check`).
Older follow-ups:
`fn_710021693c` and `fn_7100216d30` into `UnkSphere.h`; `fn_71004fa3b8` (9 identical local
declarations); the `UnkActor` slots at +0x240 and +0x298 typed `UnkAIComponent*` and
`UnkNavigation*`; `fn_7100394bd4` (2 signatures); `fn_7100ad3588` into `UnkErasedVector.h` and
`fn_71006eb8cc` into `UnkRefHandle.h`; `fn_710001a270` and the string neighbours `fn_710007ea60`,
`fn_710007e6c4`; `InitPropertyEx`'s `on_init` parameter type in `UnkTUInit.h` (one caller casts);
the render manager's accessors into `UnkRenderManager.h`. From the port link: 182 names that
resolve to no address (`UnkIdTable::UnkIdTable()`, `UnkMapIter<...>::Advance()`, ...) need an
`fn_<addr>` name; `src/game_s7/fn_71011bc0b0.cpp` defines `fn_71011bc830` by an
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
replace the static lower bound; `check.py`'s resolver could map the mangled constructor and
destructor names of address-bound members (`UnkStringPoolHolder`'s C1/D1) to their addresses, so the
66 includers' references are checked; directory moves driven by `data/tu_map.csv` once a unit is
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
oracle covers a median 67% of an original's instructions. The lifter's second gaps round
(2026-10-27, branch `claude/lifter-semantic-gaps`) handles the gaps listed then: stack arguments,
`x8` results in tail position, jump tables through register copies, shrink-wrapped prologues,
frames sized by shifted immediates, float widths, and half and byte loads converted to float. On
the boot slice with the run-time roots (2,366 `U` functions, 1.40 MB at the time) it gets 2,295
accepted lifts (97%). The oracle's inputs and a fresh `build/port/lifted` follow (stream M);
`tools/port_lift.py --debug ADDR` shows a rejected function's first difference. M1 step 4 (2026-10-26, var-flotsam/dread-decomp#201, #203 and #206): the runtime defines every `nn::os` and
`nn::oe` import the slice reaches (port_link's residue lists none); `dread_game` and the sweep
build with `-fno-strict-aliasing`; the data loader (`port/runtime/include/dread_port_loader.h`,
`port/README.md` "The data loader") maps the dump's main NSO at `0x7100000000` before the game's
static initialisers and translates the 128,576 code pointers in `.data` through the function table
`port_link.py` now writes (`image_tables.s`); `python tools/test_port_loader.py --version 2.1.0`
runs it on the dump in 2 s (26 checks, no SDK). `nn::fs` is in too (all 22 imports; `rom` is the
dump's `romfs/`, saves go to `$DREAD_SAVE`). So is the rest of the start-up `fn_71001325d0` (`nn::mem`,
`nv`, `nn::account`, `nn::util`, `nn::time`, `nn::diag`). `nn::hid` too (one handheld pad set by
`dread_port_set_npad_state`, no vibration devices). Every SDK import of the slice is in the runtime
(`nn::prepo` drops reports, `movie` has no tracks yet); `tools/port_imports.py` checks stubs against
the dump's mangled import names. `tools/port_addresses.py` places the vtables and
named functions the link had no address for from matched code (residue 439 to 226: 191 host names,
9 vtables and 26 names, 12 of them a placeholder name the sources reuse for different classes:
rename them, `port/README.md` "Addresses from matched code"). **The port links and runs on Linux in Docker** (`port/README.md`,
"Linux in Docker"; WSL 2 and Docker installed by the owner on 2026-10-26). The boot-match batch
(2026-10-27, branch `claude/port-m1-boot-match`) hand-matched 194 of the 213 boot-slice functions
that had neither a source nor a lift (112 `O`, 82 `m`, each `m` re-checked by a skeptic; 19 of 2
to 16 KB left, `build/port/linux/link/boot_unlifted.csv`), gave the 40 declarations it made
conflicting one header each, and matched `nninitStartup`. The boot now maps the dump, runs
`nninitStartup` (the memory heap and the default allocator, as the SDK does before the static
initialisers), every static initialiser, `nnMain` and the whole system start-up, and traps at the
game's main function `fn_7100000980` (764 B, `L` in `data/functions/` but the game's). Crashes
print their address and return addresses (`port/runtime/src/fault.cpp`). Boot-match round 2
(2026-10-27, branch `claude/port-m1-game-main`, journal) matched `fn_7100000980` and the 141
functions the boot then reached by direct calls from its run-time roots (`nnMain`'s three callbacks
and main's four), 95 `O` and 47 `m`. It also stubbed `nn::nfp` (26 of 26). The boot now runs into
the start-up `fn_7100015c70`; under `DREAD_TRAP=log` six functions without a source come first,
then a null dereference in lifted code after one of them returned 0. Next for M1: round 3,
the 127 functions on the same roots' static list (74 KB, the scratch script of round 2 over the new
`traps.s`) and the ones reached through pointers (`fn_7100075730`, `fn_710008a460`,
`fn_710002f0dc`, `fn_71000666b0`, found by running), the same workflow; then follow the traps.
Two blockers come from the lifted code itself, not from missing functions:
- **Dropped arguments.** Byte-identical lifts dropped arguments at their call sites (277 boot-path
  callers) and in their definitions (164 on the boot path), and `tools/abi_facts.py` under-counted
  216 functions (capstone's alias bug). The lifter's pass-through regeneration (2026-10-27) leaves
  103 functions, 10 on the boot path (`build/port/passthrough.json`); their semantic lifts carry
  `"overrides"` in port_lift's manifest, which port_link has to honour (stream M). Until it does,
  write a trapped path's callers by hand, as unit 167's wrappers were
  (`src/engine/UnkGraphicsApi.cpp`).
- **Split statics.** Function-local statics of header-inline functions are executable symbols on
  the port, while lifted code uses the image's. Bind them to the image with `--defsym` from
  `tools/port_addresses.py`'s harvest.

`nn::audio` (6 reached through lifted code) still needs stubs, and also the game globals that a
matched file defines itself while other code reaches them by address (`port/README.md`, "Game data
defined by the sources").
M2 groundwork (2026-10-26, branch `claude/port-bshdat`): `tools/bshdat.py --version 2.1.0
--compile` parses the 478 shader packages, extracts the 671 programs' 1,342 GLSL sources and the
compiler's reflection to `build/shaders/2.1.0/`, and compiles 1,341 stages to OpenGL and Vulkan
SPIR-V with glslang 16.6.0 after two rewrites (block bindings from the reflection,
`GL_NV_explicit_typecast`); FXAA's fragment stage needs a rewrite rule. Decided 2026-10-26: Vulkan
first, the SPIR-V built from the user's dump by the port's build (`bshdat.py --compile`'s Vulkan
mode), no compiler in the executable. Done since (#202, #204): the build step is the port's
`shaders` target (`tools/bshdat.py --install`: the FXAA fixup makes all 1,342 stages compile; each
module is checked against NVN's reflection; `shaders.manifest` beside the executable), and
`port/nvn/` (`dread_nvn`) holds the manifest reader and the binding layer: NVN's per-stage slots on
one Vulkan descriptor set per stage, stages recognised by their control section (the only part
`nvnProgramSetShaders` gets by pointer). Variant selection is the engine's own (`fn_71001b0f24` on
`CProgramResource`, `port/nvn/README.md`): the shim needs nothing for it, but the engine's loader
and selector (`fn_71001b0050`, `fn_71001b0f24`, `fn_71001b0f7c`, the program switch `fn_71001cc608`)
are `U` and in port_lift's boot slice. The game calls 150 of its 523 NVN functions.
Since then (#217 and the Vulkan check): the binding layer runs in the Linux image
(`nvn_manifest_check`) and against lavapipe with the validation layers (`nvn_vulkan_check`,
`port/docker/vulkan/Dockerfile`, the owner's go-ahead): all 671 programs' modules, layouts,
descriptor sets and pipelines, no validation message, after two fixes it found (inactive fragment
inputs the vertex stage does not write are dropped by the build step; the device needs
`maintenance4`). Then the start-up path of the shim itself: the entry layer (`nvnBootstrapLoader`,
`nvnDeviceGetProcAddress`, a named trap for every function not written), the device over Vulkan,
memory pools with a GPU address space and buffers (the binding layer's address resolution), queues
and syncs, command buffers, the state objects and the window: 106 functions, 74 of the 146 the game
calls (`data/nvn_procs.csv` from `tools/nvn_procs.py`, which also names every `g_unk_716e29xxxx` of
the matched start-up). `nvn_startup_check` replays that start-up `fn_71001587f0` and three frames
against the shim on lavapipe with validation: all checks pass, no message. With the texture part
(textures as Vulkan images, the texture and sampler pools, samplers; formats from
mercury-engine-data-structures and by use, targets from the game's own textures) the replay reaches
no unimplemented function: 143 functions, 108 of the 146 the game calls. With the drawing part
(2026-10-27): all 146 (182 in all). Programs, pipelines, Vulkan command buffers, render targets,
clears, barriers, uniform updates, vertex streams and draws, each number's meaning taken from the
engine's own reflected enumerations and the wrapper that converts them (`port/nvn/README.md`,
"Drawing"; `include/nvn_shim/draw.h`). `nvn_startup_check` with the shaders draws all 671 programs
in full and depth-only on lavapipe with no validation message. Next for M2: the alpha test (a discard
the build step adds to the fragment stages, with a push constant), a Vulkan surface for the window,
asset texel upload (the console's tiled layout) and the sampler enumerations; and, once the boot
reaches `fn_71001587f0`, `dread_nvn` joins `tools/port_link.py --executable` (agreed with the M1
session).
Windows (decided 2026-10-27: Linux is the only running target until the title screen; Windows is
compile-checked alongside, #225): `tools/port_sweep.py --target
x86_64-w64-mingw32` compiles with llvm-mingw 20261006 (`tools/fetch_toolchains.py
llvm-mingw-20261006`, Itanium ABI, libc++ 9.0.1 over MinGW-w64's headers) and scans for what LLP64
and a 16-bit `wchar_t` change silently. All 3,979 sources compile and the scan finds nothing: the
game's wide text goes through the `char32_t` helpers in `include/base/Char32.h` (the original calls
where `wchar_t` is 32 bits). Keep both at zero: rerun it when a PR adds hand-written code. Next
Windows steps, after the title screen on Linux: the runtime's LLP64-mangled import names
(`tools/port_imports.py --windows`: the runtime compiles; 41 of its 200 imports are lost by name,
each with one `long long` counterpart, for `image_tables.s` to list under the console's name), Win64/COFF
versions of `port_link.py`'s `traps.s`, `residue.s` and `image_tables.s`, the fault handler as
structured exception handling, and a Windows link (`port/README.md`, "Windows (compile only)").

**Stream M (generators).** The bugs above and `--prune`/regeneration after merges. The lifter's
semantic mode (`opts={"semantic"}`, driven by `tools/port_lift.py`) gives the port oracle-checked
lifts of `U` functions; its gaps are listed under stream D. Byte mode now names, at each call, the
argument registers the callee reads that the caller still holds (2026-10-27, journal "Lifter:
arguments lost in byte-mode lifts"; regenerated, 46,443 lifted, 73 more `O`, none lost). Byte-identical
lifts had relied on registers their C never named (AArch64 keeps them; an x86-64 compile does not):
before, 3,220 `src/lifted` definitions read more than they declared and 8,362 made a call that
dropped one; after, **103** functions lose an argument (36 definitions, 95 callers; 10 on the boot
path), listed by `tools/port_lift.py --passthrough` in `build/port/passthrough.json`. port_lift
lifts those in semantic mode and marks each accepted lift `"overrides": "src/lifted/lifted_XXX.cpp"`
in its manifest, for `tools/port_link.py` to link instead of the `src/lifted` copy (the M1 session's
change, branch `claude/port-lift-overrides`). 93 of the 103 have an accepted lift, the 10
boot-path ones included. `tools/abi_facts.py` corrects capstone 5.0.7's register access
(`cmp`/`cmn`/`tst`, shifts and extends by immediate, `fmov #imm`), sees the cases of a switch, and
serves every tool from one table computed in address order (`build/port/abi_facts.pickle`).
port_lift's worklist is now a slice (2026-10-27): `--slice boot` (the default) is every `U`
function reachable by direct calls from `nnMain`, from the seven run-time roots, from the four
graphics functions and from the four the boot reaches only through pointers (2,226 functions,
1.26 MB); `--slice all` is every `U` function but the SDK imports (16,002, 8.08 MB);
`--worklist build/port/link/boot_missing.csv` gives the old one. Next for the lifter: the semantic
gaps, the oracle's inputs, and both slices run (branches `claude/lifter-*`). Open leftovers of
every generator round are recorded in `docs/JOURNAL.md` (work log items 0–3).

## Known issues and gotchas

- **Linux:** identical results with upstream clang+llvm 9.0.1 (see Rebuilding). **Windows PATH:**
  shells started before the toolchain install may not see `cmake`/`ninja`; use `.venv/Scripts/python`
  and absolute tool paths, as the scripts do.
- **Ghidra project lock:** only one process can open `build/ghidra/<ver>/dread.gpr` at a time.
  `tools/ghidra/decompile.py` fails from a worktree under `.claude/` (Ghidra rejects a path element
  starting with a dot): run it from the main checkout.
- **Worktrees on Windows:** a new worktree needs `orig` and `build` linked to the main checkout's.
  Git Bash's `ln -s` silently copies them (12.6 GB, 2026-10-26) unless native symlinks are on; make
  junctions instead: `cmd //c mklink /J orig <main>\orig` (and `build`).
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
