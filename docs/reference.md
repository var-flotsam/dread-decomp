# Project reference

Stable facts about the binary, the tree and the tools: where things are, how to rebuild from
scratch, decisions already made and the known gotchas. It changes rarely; current state and next
steps are in `docs/HANDOFF.md`, history in `docs/JOURNAL.md`. Moved here from the handoff on 2026-10-28.

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
tools/dashboard_export.py        status, history, areas, workstreams, lifts, boot distance as CSVs for the dashboard (build/dashboard/)
tools/status_card.py             README's status card (docs/status.svg) from tracked files; --write, --check, --summary (PR job summary)
tools/callgraph.py, hubs.py      direct-call graph (Ghidra export); hubs by unmatched callers, --ready, --callers
tools/decl_check.py              fn_/g_unk_ declarations in several files or with conflicting signatures; --base enforces
tools/struct_check.py            struct/class/union names defined in several files, same text or different; --list, --name, --base
tools/struct_migrate.py          move a type's identical copies into one header; a source keeps the edit only if its code is unchanged
tools/equiv_test.py              tier 2: runs a registers-only non-match beside the original; verdicts in data/equivalence.csv
tools/ref_check.py               wrong addresses in non-matching functions: what each calls and refers to, against the original
tools/permute.py                 cheap permuter: reorders a registers-only near-match's source until it matches; --write
tools/drafts.py                  drafts/: add (file a non-match, graded), grade (regrade rows), promote (move to src/ when earned)
tools/sample_review.py           the week's seeded sample of newly decompiled functions to read (docs/maintaining.md)
tools/baseline.py                trymatch output snapshot, compare and restore for a file set (headers expand to includers)
tools/hub_migrate.py             mechanical canonical-header migration on top of baseline.py; LEFT lines are the hand work
tools/rename.py                  give a function a readable name: data/names.csv, data/functions/, every hand-written use; --check (CI)
tools/tu_map.py                  translation-unit map from .init_array + reflection groups -> data/tu_map.csv; --function
tools/tu_status.py               status per translation unit (matched, done, partial, untouched) -> data/tu_status.csv; --list, --unit, --summary (PR job summary)
tools/objdiff_gen.py             objdiff targets + objdiff.json (gitignored) for every src/ unit
tools/shape_census.py            same-shape U families (maintenance)
tools/sigmatch.py                names vendored-library functions in main -> data/library_symbols.csv
tools/gen_reflection.py, refl_attrs.py, gen_types.py, gen_statics.py, gen_clones.py, gen_tables.py, gen_lift.py
                                 generators (maintenance); regenerate, never hand-edit their output
tools/policy_check.py, hooks/    content rules (CI and pre-commit)
tools/batch/                     lean/: the batch kit (COORDINATOR.md, worker.md; runs in place); tips.md; README.md
.github/                         CI: policy + decl_check, compile + tool tests (hosted); check-local.yml (self-hosted check.py, enabled: "Known issues")
src/compiler_id/, flag_id/, atk_id/   Phase 3 samples (frozen)
src/reflection/, reflection_types/, reflection_statics/, clones/, lifted/, tables/   GENERATED
src/base/                        hand-written engine code: string ids, pool, CRC64, allocator, reflected functions
src/game/<unit>/, src/base/...    hand-written files moved by unit from the address slices (tools/unit_move.py, 2026-10-28)
src/game_*/                      the address-slice files whose unit is not known yet, and their shared local headers (no new ones)
src/lua/                         game-side Lua changes (luaB_print)
include/types.h                  u8..f64 without a standard library
include/base/                    engine headers: reflection (CClass.h, UnkReflCast.h ...), containers, string ids, clone templates
include/base/reflection/UnkClassGetters.h   GENERATED by tools/gen_class_getters.py (from data/functions/, no game data): every class getter
include/game/                    game headers shared by several files (UnkActor, UnkGameManager, transforms, TU init, graphics state ...)
tools/gen_class_getters.py, tools/port_sweep.py   class-getter header generator; host compile sweep of src/ for the port
port/                            the PC port layer: CMake project, runtime stubs (M0; see docs/PORT_PLAN.md)
lib/lua/, lib/dlmalloc/, lib/libcxx/   vendored upstream code with READMEs; lib/nnsdk, lib/musl submodules
data/                            functions/ CSVs, reflection_types.csv, reflection_fields.csv, tu_map.csv, tu_status.csv, data_map.csv, library_symbols.csv, hashes, pins
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

The tools also run on Linux: `tools/fetch_toolchains.py 9.0.1` fetches the official x86_64 tarball
into `~/.cache/dread-decomp` (results identical to Windows; the hosted build job uses it), with
`.venv/bin/python` in place of `.venv/Scripts/python`.

### macOS (Apple Silicon, untested on hardware)

Clang 9.0.1 has no arm64 macOS build; the x86_64 build runs under Rosetta 2. Before relying on it,
`tools/check.py --version 2.1.0` must reproduce the PC's `O` count.

1. Install Xcode command line tools (`xcode-select --install`), Python 3.12
   (`brew install python@3.12`) and Rosetta 2 (`softwareupdate --install-rosetta --agree-to-license`).
2. Clone, `git config core.hooksPath tools/hooks`, `python3.12 -m venv .venv`, and install the
   packages above with `.venv/bin/python -m pip install ...`.
3. Keep the dump in a folder outside the repository (for example `~/dread-dump/` with `exefs/` and
   `romfs/`), then run `tools/setup.py`, `tools/verify_dump.py`, `tools/nso2elf.py`,
   `tools/fetch_toolchains.py` (into `~/Library/Caches/dread-decomp`), `tools/fetch_sources.py` and
   `tools/recon/run.py` as above.
4. If `git status` shows `data/toolchains.json` or `data/sources.json` changed, a download hash
   differs: stop and tell the owner.

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
| `L` for `nn::atk`, SDK, runtime, Lua stdlib, dlmalloc | Library code isn't game progress. Until 2026-11-11 `nn::atk` was also to stay undecompiled; see the next row |
| **`nn::atk` is decompiled, as its own library** (owner, 2026-11-11; replaces "stays undecompiled") | The goal is no translation layer and every function hand-written, so lifting it could never be the end state, and a replacement player could only be tuned by ear where a decompiled library is checked byte for byte. It follows the MusyX precedent (`docs/references.md`). The port writes `nn::audio` (149 imports) over a PC mixer. Size on 2026-11-11: 1,152 `L` functions from 0x71012046c0, 0.17 MB, median 68 bytes; game code calls 188 of them directly. Its functions keep `L` and get their own progress line, apart from the game's. Not yet done: that line, a home for the sources (the batch files under `src/atk_id/` belong there), and names for the 872 unnamed functions |
| 30–60 minute time-box per function, except reflection boilerplate | Owner-approved matching standard |
| Function list split per MiB; CSV statuses regenerated, never hand-merged | 1 MB pre-commit limit; parallel streams |
| objdiff per `src/` unit with synthesized targets; whole-binary and boot-slice progress from the CSV | No byte-identical relink is possible on Switch |
| Lua, dlmalloc, one libc++ `__sort` from upstream source (approved) | Open source; identified by diffing |
| `nn::` declarations from **open-ead/nnsdk** as a submodule | Community headers, no license: referenced, never copied; the port must not depend on them without asking |
| Generated code (reflection, statics, clones, tables, lifted) is regenerated, never edited | Thousands of instances of one template; byte-identical by construction |
| **Hub-first work order, canonical headers, files by unit** (2026-10-24) | The remaining `U` is typed game logic; per-file declarations (382 conflicting signatures) can never link; the port needs one program |
| **Generators in maintenance** (2026-10-24) | `U` under 256 B is 0.9 MB; the lifter's byte-identical mode is at its limit (journal, item 1) |
| **Handoff split into state and journal; self-hosted check job** (2026-10-24) | Every session read a 28k-token log; 178 PRs in six days at a median of 10 functions and 3 minutes to merge |
| **No minimum pull request size; no phase prefix** (owner, 2026-11-14; replaces "PRs of 30+ functions") | The audit of 2026-11-14: most PRs merged within 90 minutes and nearly every one was "Phase 4.7"; the floor did not match one-class layout batches |
| **Class getters declared once in a generated header** (`UnkClassGetters.h`, 2026-10-24) | 243 hand files declared getters locally as `void*`; the CSV knows every getter, so the header needs no game data and never drifts |
| Route to a running build: **Route B** (`docs/PORT_PLAN.md`, 2026-10-24) | One program now, lifted code as a bridge that matching replaces; the port lives in `port/` on `main` |
| **Vulkan first; shaders compiled to SPIR-V at build time** from the user's dump (owner, 2026-10-26; `docs/PORT_PLAN.md` decision 3) | NVN's object model maps onto Vulkan, the binding layout is already worked out for it, and it runs on all three systems; the sources need the NVIDIA-only `GL_NV_gpu_shader5` under OpenGL. A build step (`tools/bshdat.py --compile`) keeps a compiler out of the executable; its output stays local game-derived data |

### The rules in `CLAUDE.md`, with their reasons

`CLAUDE.md` was cut to the rules on 2026-11-14 (owner, in the audit of what slows progress): it loads into every
session and every batch worker. Each rule's reason and the owner's words are here.

| Rule | Reason |
|---|---|
| **Purpose** (owner, 2026-11-05) | A tool for the community's understanding of a game they already own: speedrunners reading the real code to find new methods, developers making mods that would otherwise be impossible. Deliberately different from the static recompilation "Metroid Dread Recomp": no translation layer in the product. The project hosts no keys, ROM or filesystem and never hands them to a user, to stay clear of piracy |
| **Completion and the proof tiers** (owner, 2026-11-11) | Decompilation is complete when every function of the game is hand-written, readable and proven equivalent, measured over all the game's code (`L` excluded). Generated code is a stepping stone. Tier 2 counts because a permuter, not model attempts, is what turns a registers-only difference into a match; `tools/permute.py` matched about one in nine on 2026-11-12 |
| **Behaviour, then readability, then bytes** (owner, 2026-11-13) | "Byte matching and readable are two different qualities but both are needed. Identical behavior but byte mismatch are okay for a later date"; "The project will not be considered to be absolutely done until the decompilation functions identically and is human readable" |
| **Faithful first** (owner, 2026-11-13) | "We want faithful to original above our invented rules." Precedent: `src/game_lo/fn_7100219588.cpp` reads two locals the original also leaves unset rather than setting them |
| **No assembly in commit messages or PR descriptions** (owner, 2026-11-14) | They are as public and as permanent as a tracked file |
| **Inferred names** (owner, 2026-11-14) | Readable names are part of readable code. A real name from RTTI, reflection or strings replaces an inferred one when evidence turns up; `tools/rename.py` keeps the map, the function list and every hand-written use in step |
| **`REFS` notes on minor functions** (owner, 2026-11-12) | A minor function that calls something else is not the same behaviour unless the note says why it is harmless |
| **Drafts for unverified work** (owner, 2026-11-13) | A non-match in `src/` claimed behaviour nobody checked; the machine test's first run found four real bugs in functions marked minor by hand. The non-matching functions already in `src/` stay |
| **Code no developer would write; no `goto` to force a match** (owner, 2026-11-14) | From zeldaret/botw's `AGENTS.md`: a match whose text no developer would write still holds the compiler's work, and a helper found once repairs every caller |
| **No unset values** (owner, 2026-11-13) | A text that reads an unset local has no behaviour of its own; the compiler may make anything of it, the original's bytes included, so its match proves nothing |
| **One declaration per shared symbol; the type ratchet** (2026-10-24, 2026-11-12, 2026-11-13) | The port links one program, and per-file declarations (382 conflicting signatures once) can never link; the agent runs of 2026-11-09 to 11-11 doubled the copies in two days |
| **A layout batch is one class** (owner, 2026-11-13) | One layout per object, settled where the object's own code is; bulk moves of copied types were declined |
| **Bulk runs: the hold and its lifting** (owner, 2026-11-11; lifted 2026-11-14) | Held after the runs of 2026-11-09 to 11-11 doubled declaration copies; lifted once CI gates blocked that damage and the 40-function test showed shared headers gave no gain. Workers take several functions each because a worker's start-up of about 100,000 tokens made the pilot cost 736,000 a match |
| **Rule freeze** (owner, 2026-11-14, until 2026-11-28) | From 2026-11-12 to 11-14 about 52 PRs merged and 32 functions matched; 16 of those PRs changed no source. Most new rules brought a tool, a gate and doc text |
| **Who merges** (owner, 2026-11-11) | The coordinator merges on green checks; `--admin` also skips the required checks, and the last-push approval it once bypassed is off |

### Toolchain (the owner's machine)

Python 3.12 in `.venv` (`lz4`, `mercury-engine-data-structures`, `capstone`, `pyelftools`, and `pyghidra` from
Ghidra's bundled wheels); JDK 21 (Temurin) with Ghidra 12.1.4 at `%LOCALAPPDATA%\Programs\ghidra_12.1.4_PUBLIC`
(headless: `support\analyzeHeadless.bat`); CMake 4.4, Ninja 1.13, LLVM/Clang + lld 23.1.3 for inspection; the
project compiler, upstream Clang 9.0.1, from `tools/fetch_toolchains.py` (`%LOCALAPPDATA%\dread-decomp\toolchains\llvm-9.0.1`;
on macOS the x86_64 build under Rosetta 2 in `~/Library/Caches/dread-decomp`, untested on hardware;
paths in `tools/hostenv.py`), flags `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float
-fno-rtti -fno-exceptions` (`tools/buildcfg.py`, evidence in `docs/compiler.md`); objdiff 3.8.2 GUI and CLI in
`%LOCALAPPDATA%\dread-decomp\tools\`. Shells opened before an install may need PATH refreshed from the registry.

## Known issues and gotchas

- **Linux:** identical results with upstream clang+llvm 9.0.1 (see Rebuilding). **Windows PATH:**
  shells started before the toolchain install may not see `cmake`/`ninja`; use `.venv/Scripts/python`
  and absolute tool paths, as the scripts do.
- **Ghidra project lock:** only one process can open `build/ghidra/<ver>/dread.gpr` at a time.
  `tools/ghidra/decompile.py` fails from a worktree under `.claude/` (Ghidra rejects a path element
  starting with a dot): run it from the main checkout.
- **Worktrees:** `tools/worktree_setup.py <path> [--new <branch>] [--oracle]` gives a worktree the untracked
  inputs it needs (`.venv`, `orig`, `build/recon`, the Ghidra export, the submodules) and grades a known match.
  By hand on Windows: a new worktree needs `orig` and `build` linked to the main checkout's.
  Git Bash's `ln -s` silently copies them (12.6 GB, 2026-10-26) unless native symlinks are on; make
  junctions instead: `cmd //c mklink /J orig <main>\orig` (and `build`).
- `llvm-readelf` isn't in the LLVM Windows build; use `llvm-readobj --elf-output-style=GNU`.
- Constructors compile to two aliased symbols (C1/C2). `check.py` counts them once (by address);
  objdiff counts both.
- **Clang 9 ignores `asm("fn_<addr>")` on a constructor or destructor** (2026-11-14): the object calls and
  defines the mangled name (`_ZN10UnkHalf_cuC1EOS_`), never `fn_<addr>`. The declaration still names the
  address, by the label or by a comment that starts with it (`~CRntString();  // fn_710007ecd4`; a
  template's `// fn_<CTOR>`, `template <> X<0>::~X();  // fn_...` for one instantiation, a macro's
  `/* fn_##p */`; a class's vtable by `struct X : Base {  // vtable g_unk_<addr>`), and
  `tools/dread/asm_labels.py` reads it for `check.py`, `trymatch.py`, `drafts.py` and
  `equiv_test.py`. A matching function's call to a constructor or destructor that the object does not
  define and nothing names fails as `RELOC ... special`; a wrong address fails as any RELOC does.
- **Relocation check limits:** the content of string literals and constant-pool entries is compared
  with the original's since 2026-11-11 (a `RELOC ... string` or `const` line). Still not checked,
  about 92,000 relocations: unresolved symbols (callees, statics and vtables whose name is not
  address-based), jump tables and lookup tables outside the constant pool, wide strings, and
  conditional branches. So a callee declared under a made-up name is not verified. A callee with no address is unresolved: write the
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
