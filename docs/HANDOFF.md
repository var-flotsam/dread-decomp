# Handoff: dread-decomp

For the next developer, human or AI, picking up this project. Read this, then `CLAUDE.md`
(rules and conventions), `docs/recon.md` (what's in the binary), `docs/compiler.md`
(toolchain), and `docs/references.md` (outside projects).

**Fourteenth session (2026-10-21, Linux cloud, branch `claude/project-thread-slp42f`):** probe of the
ten largest `U` functions (Next steps, "Giant functions"); new generator `tools/gen_tables.py`
(`src/tables/`, stream 8): the strid table, the loading screen's video map (`video_map` kind;
`include/base/global/CFilePathStrId.h` now keeps its pointer in `UnkFilePathStrIdBase`, no regressions) and the
boss rush setup (`boss_rush`, `m`: register allocation only).
`gen_tables.py` and `check.py --update` were rerun.

**Thirteenth session (2026-10-21, Linux cloud, branch `claude/project-thread-m3b90j`):** core engine
types (Next steps, item 4): the string pool, CRC64 and the allocator getter in `src/base/`
(var-flotsam/dread-decomp#20, #22 and the string heap PR); no generator was rerun.

**Twelfth session (2026-10-21, on the PC, branch `claude/lifter-mismatch-classes`, var-flotsam/dread-decomp#18):** lifter mismatch study
(Next steps, item 1, "Done (twelfth session)"); `gen_lift.py` and `check.py --update` were rerun,
so the numbers below are current.

**Eleventh session (2026-10-21, branch `claude/local-agent-setup-df6136`):** set up two parallel
workstreams (below), `AGENTS.md` and `docs/tasks/` for a small local model on a MacBook,
macOS toolchain support (untested on hardware) and `tools/verify.py`. No matching changes.

## Workstreams

**Thirteenth session (2026-10-21, cloud):** work now runs as parallel cloud threads, one per
stream below. The Mac workstream is retired (not enough memory); `AGENTS.md` and `docs/tasks/`
remain as reference only. Each stream edits only the files in its row.

| Stream | Area | Files it may edit | Branches |
|---|---|---|---|
| 1. Lifter | Next steps, item 1: loops and block layout, `ccmp`, jump tables, register moves | `tools/gen_lift.py`, `src/lifted/` | `cloud/lift-<topic>` |
| 2. Shape families | Next steps, item 2 (clone families, all sizes); `docs/tasks/` (gave guarded statics to stream 6) | `tools/gen_clones.py`, `include/UnkClone.h`, `include/base/UnkCloneHelpers.h` and new template headers, `src/clones/` | `cloud/clones-<topic>` |
| 3. Core engine types | Next steps, item 4: string pool, `UnkReflArray`, CRC64, allocators (hand-written) | `src/base/`, `include/base/global/`, new `include/base/*.h` | `cloud/core-<topic>` |
| 4. Reflection leftovers | Next steps, item 2, "Left" | `tools/gen_reflection.py`, `tools/refl_attrs.py`, `tools/gen_types.py`, `src/reflection/`, `src/reflection_types/`, `include/base/reflection/` (not `UnkReflStatics.h`) | `cloud/refl-<topic>` |
| 5. Lua leftovers | Next steps, item 3 (library code, no game progress) | `lib/lua/`, `tools/sigmatch.py`, `tools/libcheck_lua.py`, `data/library_symbols.csv` | `cloud/lua-<topic>` |
| 6. Guarded statics | Next steps, item 1, guarded function-local statics (taken over from stream 2, which gives up these files) | `tools/gen_statics.py`, `include/base/reflection/UnkReflStatics.h`, `src/reflection_statics/` | `cloud/statics-<topic>` |
| 7. Game logic by hand | One-off `U` game functions (not in shape families; not engine/, script/, reflection, Lua) decompiled by hand, one file per function. Split by address at 0x71008cde78 (the median `U` game-function address on 2026-10-21): below it `src/game_lo/`, from it up `src/game_hi/` | `src/game_lo/`, `src/game_hi/` | `claude/project-thread-<id>` |
| 8. Table functions | The largest `U` functions that only walk a fixed table of string literals (probe of the top 10 by size, 2026-10-21; see "Giant functions" below) | `tools/gen_tables.py`, `src/tables/` | `claude/project-thread-<id>` |

**Starting points for hand matching.** `python tools/gen_lift.py --version 2.1.0 --best ADDR[,ADDR...]`
(or `--best @file`, one address per line) lifts each function in every variant and rewrite round
and writes the closest attempt to `build/lift_best/fn_<addr>.cpp` (gitignored; the first line gives
its instruction match), ready for `tools/trymatch.py`. About 1 s per function plus one compile per
variant. The attempt is `char*` arithmetic over registers; give it real types, then move the
matched function into your own files. Never commit `build/lift_best/`.

All may change `data/functions/*.csv` (statuses only, through `check.py --update`). Anything
else (shared headers, `check.py`, `CLAUDE.md`) goes through the owner first.

Merge rules:
- Keep PRs small, sized to finish within one session; merge `main` into your branch before merging.
- After merging `main`, take `main`'s `data/functions/*.csv`, rerun only the generator you own,
  then `tools/check.py --update` (`tools/verify.py --regen <generator>` does both). Never
  hand-merge the CSVs.
- `check.py` and `verify.py` fail when one function is defined in two files outside the Phase 3
  samples (`src/compiler_id/`, `src/flag_id/`, `src/atk_id/`), e.g. a new clone that `src/lifted/`
  also defines. `verify.py` runs `gen_lift.py --prune` (seconds) unless `--regen` names
  `gen_lift`, so lifted copies drop out by themselves; any stream may commit that change to
  `src/lifted/`. Only stream 1 runs the full lift (hours in the cloud).
- `gen_clones.py` and `gen_lift.py` skip functions defined elsewhere in `src/` (`gen_statics.py`
  runs before both), so a rerun drops duplicates.
- A stream that changes focus updates its row above in its next PR.

**Tenth session (2026-10-20, branch `claude/lifter-float-0b96b0`):** lifter fixes (Next steps,
item 1, "Done (tenth session)"); `gen_lift.py` and `check.py --update` were rerun, so the
numbers below are current.

**Ninth session (2026-10-20, on the PC, branches `claude/sync-local-repo-4f8846`, merged in
var-flotsam/dread-decomp#13, and `claude/shape-round-6`):** the shape-family pass, rounds 5 and 6
(Next steps, item 2, "Done (ninth session)"). Numbers below are current;
`gen_lift.py` was not rerun (it skips what the other generators define).

**Seventh session (2026-10-19, branch `phase-4-3-remaining-reflection`):** the full regeneration
ran (the last steps, `gen_lift.py` and `check.py --update`, in a Linux cloud session with upstream
Clang 9.0.1, which reproduced the Windows results exactly). Numbers below are current. What was
done: Next steps, item 2.

Snapshot as of 2026-10-18 (fifth Phase 4 session; branches `phase-4-4-clones` (var-flotsam/dread-decomp#18, open) and `phase-4-4-skipped-getters` on top of it (not pushed); earlier work merged in var-flotsam/dread-decomp#13–#17). Target: **Metroid Dread 2.1.0**, `main` build ID
`646761F643AFEBB379EDD5E6A5151AF2CEF93DC1`.

## Where things stand

| Phase | State |
|---|---|
| 1. Repo, provenance, hooks | **Done.** Dump imported and verified; hashes confirmed |
| 2. Binary reconnaissance | **Done.** 104,017 functions; 7.0% named; 2,485 reflected types with `sizeof` for all 2,346 classes |
| 3. Compiler identification | **Done.** Clang 9.0.x `-O3 -mcpu=cortex-a57 -fPIC -fasynchronous-unwind-tables -mno-implicit-float`. Confirmed on 20 non-leaf game functions (18 exact), `nn::atk` (5/6) and upstream Lua 5.1.4 (81/82) |
| Tooling for Phase 4 | **Done.** `tools/check.py` (build + status), objdiff (`tools/objdiff_gen.py`), whole-binary progress (`tools/progress.py`) |
| 4.1 System headers | **Done.** `lib/nnsdk` + `lib/musl` submodules, libc++ 9.0.1 headers, `tools/buildcfg.py` |
| 4.2 Lua | **Done.** Upstream 5.1.4 vendored in `lib/lua/` (2 marked config changes); 286 Lua functions located in `main` by `tools/sigmatch.py` (status `L`) |
| 4.3 Reflection | **In progress.** all 2,354 located class getters (444 in TUs with other statics, through padding); all 1,010 other `CClass` descriptor constructors (skipped TUs and getter-less types); 1,911 of 1,957 class attribute/member-field functions, 478 other registration functions plus 54 in their class getter's TU, and all 1,006 out-of-line reflected-function initialisers (`src/reflection/`, `tools/gen_reflection.py`, `tools/refl_attrs.py`). Pointer types: 993 standalone getter/constructor pairs plus 108 generated in their pointee's TU; all 238 `CEnumType` constructors; 178 standalone `CCollectionType` constructors plus those in their element class's TU (`src/reflection_types/`, `tools/gen_types.py`) |
| Small functions | **In progress.** `tools/gen_lift.py` lifts functions of up to 512 bytes from their instructions and keeps byte-identical ones: 46,219 functions, 2.02 MB (`src/lifted/`; second-round store-order, may_alias and memcpy rewrites; sret functions, stack locals, a goto-based flat mode for crossing branches and loops) |
| Guarded statics | **In progress.** `tools/gen_statics.py`: attribute keys, static string ids (weak and internal), typed lookups, registries and the tables built from them, type-id senders and singleton parts, on function-local statics (`include/base/reflection/UnkReflStatics.h`), and game code with its own statics lifted through `gen_lift.py`: 879 functions, 175 KB (`src/reflection_statics/`) |
| Repeated templates | **In progress.** `tools/gen_clones.py`: hand-matched templates instantiated once per copy (`UnkClone_fn_<addr>` tags): the named registry, `UnkVector` methods per element kind, the reflection collection iterator, a deferred set, and scan kinds (facts per candidate): destructors of string-id/refcounted members, property reads and string-id writes, enum value lists, and shape-family templates (`UnkCloneHelpers.h`): 6,256 functions, 1.22 MB (`src/clones/`) |
| 4.4 Core engine types | **In progress.** String ids (`CStrId` incl. copy-assignment, `UnkStrId`), the growable array `base::UnkVector` (push, grow-at, insert, remove, clear/free, move) and its allocator interface; reflected-function initialisers now match (below) |
| 4.5+ Boot to title screen | Not started; plan below |
| Port | Not started (`port/` is empty) |

Function status (BotW codes): `O` 81,478, `m` 3, `M` 1, `L` 1,614, `U` 20,921.
**Game-code progress: 46.16% of bytes (8.67 MB) matching** (`tools/progress.py`; library code
excluded; 45.39% at the start of the twelfth session, 44.92% at the start of the tenth session, 44.45% at the start of the ninth, 39.09% at the start of the seventh). `tools/check.py` counts `src/` definitions,
which differ slightly (matching `nn::atk` functions keep `L`; constructor aliases).

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
tools/trymatch.py         build one source (src/ or scratch) with the project flags and diff it
tools/ghidra/decompile.py print Ghidra's decompilation of functions, to read (never commit it)
tools/check.py            build src/ with the project toolchain; derive O/m/M/W; catch regressions
tools/verify.py           one-command gate: --regen generators, check.py --update, progress, policy; ~10 lines
tools/hostenv.py          platform details: download cache folder, .exe names, llvm-cxxfilt
AGENTS.md, docs/tasks/    short guide and task cards for a small local model (Mac workstream)
tools/objdiff_gen.py      objdiff target objects + objdiff.json (gitignored) for every src/ unit
tools/progress.py         whole-binary progress from data/functions/ (excludes L)
tools/hooks/pre-commit    rejects >1 MB files, console formats/keys, orig/ and build/
tools/policy_check.py     the same rules plus listing/key-file detection, for the whole tree or a PR (CI)
.github/                  CI (policy job), CODEOWNERS, PR template, main ruleset (rulesets/main.json)
README.md, CONTRIBUTING.md, docs/maintaining.md   public-facing docs: fork procedure, rules, maintainer guide
src/compiler_id/          10 leaf functions;  src/flag_id/  20 calling functions;  src/atk_id/  6 nn::atk
src/reflection/           GENERATED by tools/gen_reflection.py: one file per original TU of class getters
                          (skipped TUs: helpers, attribute functions, constructors only), plus
                          registration_NN, function_inits_NN and class_descriptors_NN chunks
src/lifted/               GENERATED by tools/gen_lift.py: small functions lifted from their instructions
src/reflection_statics/   GENERATED by tools/gen_statics.py: functions around weak function-local statics
src/clones/               GENERATED by tools/gen_clones.py: explicit instantiations for repeated functions
src/tables/               GENERATED by tools/gen_tables.py: giant functions that walk a fixed table of string literals
tools/gen_tables.py       table generator: recovers the entries with a small register simulator, emits and verifies
tools/gen_clones.py       clone generator: kinds (verified reference, probe, or scan) -> instantiations, verified
include/UnkClone.h        placeholder tag and element types for clones (bytes, owning pointer, handle)
include/base/UnkCollectionIter.h   collection iterator and its cached (index, element) value; begin/end/erase
include/base/UnkDestructors.h      destructor templates over traits (vtable, base) and member descriptors
include/base/UnkNamedRegistry.h, UnkDeferredSet.h, UnkIdPair.h   smaller templates found as clone families
tools/gen_statics.py      guarded-static templates (UnkReflStatics.h) matched by shape against references
tools/gen_lift.py         lifter: candidates <= 512 B not defined elsewhere in src/; 9 variants, ROUND2 rewrites; verify
                          (--sample N / --only ADDRS: outcome table by size band, in build/, ~1 min)
tools/shape_census.py     families of undefined U functions by size + mnemonics; --family ADDR: varying operands
include/types.h           u8..f64 without a standard library
include/base/reflection/CClass.h   CType/CClass layout, getter macro, descriptor constructor template
lib/lua/                  vendored upstream Lua 5.1.4 (MIT) + README listing the game's changes
tools/sigmatch.py         names vendored-library functions in main -> data/library_symbols.csv
tools/gen_reflection.py   regenerates src/reflection/ from main (skipped TUs: build/reflection_tus.json)
tools/gen_types.py        regenerates src/reflection_types/: non-class type getters + descriptor constructors
include/base/reflection/CPointerType.h  CPointerType, its constructor template and refl_GetPointerType<G>
include/base/reflection/CEnumType.h     CEnumType and its constructor template
tools/refl_attrs.py       reconstructs attribute/member-field functions; keeps only byte-identical ones
include/base/global/CStrId.h            base::global::CStrId and UnkStrId (pooled, refcounted string ids)
include/base/UnkVector.h                base::UnkVector<T> (growable array) and base::UnkAllocator
src/base/reflection/      hand-written reflection code (out-of-line function initialisers)
include/base/reflection/UnkReflAttributes.h   attribute keys/values, AddField, tunable registry
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
.venv/Scripts/python tools/fetch_sources.py                        # Lua 5.1.x + libc++ 9.0.1 headers
git submodule update --init                                       # lib/nnsdk, lib/musl
.venv/Scripts/python tools/sigmatch.py --version 2.1.0             # then rerun gen_functions_csv.py
.venv/Scripts/python tools/gen_reflection.py --version 2.1.0
.venv/Scripts/python tools/gen_types.py --version 2.1.0            # ~20 s
.venv/Scripts/python tools/gen_statics.py --version 2.1.0          # ~6 min (own statics are lifted)
.venv/Scripts/python tools/gen_clones.py --version 2.1.0           # ~10 s
.venv/Scripts/python tools/gen_tables.py --version 2.1.0           # ~5 s
.venv/Scripts/python tools/gen_lift.py --version 2.1.0             # ~80 min; run after the other generators
.venv/Scripts/python tools/check.py --version 2.1.0 --update       # ~35 s, parallel
.venv/Scripts/python tools/objdiff_gen.py --version 2.1.0          # then open the repo in objdiff
.venv/Scripts/python tools/progress.py
```

The output of `gen_functions_csv.py` is reproducible (only `status` is carried over).

**macOS (Apple Silicon), untested on hardware.** `tools/fetch_toolchains.py` fetches only the
project compiler, from the official `clang+llvm-9.0.1-x86_64-apple-darwin.tar.xz` (pinned in
`data/toolchains.json` as `9.0.1-macos`; there is no arm64 LLVM 9 build, so it runs under
Rosetta 2), and the arm64 objdiff builds; everything goes to `~/Library/Caches/dread-decomp`
(`tools/hostenv.py`). `llvm-cxxfilt` comes from PATH, Homebrew's `llvm` or the fetched
toolchain. Ghidra and `gen_functions_csv.py` aren't needed there (the CSVs are tracked).
Steps: `AGENTS.md`. Before any work on the Mac, `check.py` must give the PC's O count.

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
  pointers. The descriptor constructor (`T::refl_InitClass`, an instantiation of
  `CClass`'s constructor template) stores `sizeof(T)` at `+0x18`. Parents agree with MEDS for
  2,265 classes. Layout reconstructed in `include/base/reflection/CClass.h`: `CType` (name, kind
  hash, size, flags (bit 0 = abstract), nine function slots, create, parent, two containers) and
  `CClass` (+ destroy, two containers; 0x138 bytes). Within a TU, Clang lays out all guards first,
  then all descriptors; that is how original TUs are recovered.
- **Per-type reflection helpers** (the functions the descriptor points to; typed in
  `CClass.h` as `UnkFn_0x..`, roles inferred from code shape): `0x28` unknown (varies);
  `0x30`/`0x38` copy/move-construct `(dst, src)`; `0x40` destroy in place (complete dtor);
  `0x48`/`0x50` copy/move-assign, returning `dst` or null; `0x58` compare (`a == b` or `false`);
  `0x60` returns -1; `0x68` the object's dynamic class. A reflected object's vtable has its
  class getter in slot 0 and its destructors after it (8 types, CAIAction and relatives, have
  the destructor first and the getter in slot 3). The `0x68` call is not a tail call because
  the virtual returns a pointer and the helper a reference.
- **The getter's x3/x4 functions are not create/destroy** (as first assumed): both receive the
  descriptor. x3 (`CType+0x70`) adds class attributes to the lazily created container at
  `CType+0x80` (`fn_710009dd6c`); x4 (`CClass+0xe0`) registers member fields
  (`fn_7100096524`: name, type, offset, -1, -1; returns the field, whose attributes are at +0x38).
  Most are empty. An attribute is `attrs->Add<T>(KEY(), value)`: the key getters are guarded statics
  named by their strings ("Description", "DefaultValue", "MinValue", ...), the value goes to
  `fn_71000934b8` as {type descriptor, pointer}. Enum values are typed by a global descriptor or an
  enum type getter. Matching facts: `AddField` takes the name `CStrId` **by value** (Clang 9 gives
  indirect by-value argument temporaries no lifetime markers, so the slot isn't shared); `Add` takes
  the value by const reference and passes the {type, ptr} pair as a temporary. The CTunable* classes'
  field functions start with a run-once global flag and end by registering with a lazily created
  global registry (`fn_71000839a0(0x58, 0x3f)` + `fn_710013ac14`).
- **`UnkReflName` is `base::global::CStrId`**: `fn_71000003d4` builds the values that
  `refl_GetType<base::global::CStrId>` (`fn_7100009658`) describes.
- **Two string-id types share that constructor.** A reflected function's name
  (`UnkReflFunction+0x30`) is destroyed differently from the `CStrId` passed to `fn_7100097550`
  or `AddField`, in the same function (`fn_71004f01a8`). This is modelled as `UnkStrIdBase`
  (pointer + constructor, trivial destructor) with `CStrId` and `UnkStrId` on top, each with its
  own destructor. Both move-assignments are the same inline code (`UnkStrIdBase::MoveFrom`),
  and the refcount is a union member so that the decrement reloads `data` and the count. Which
  of the two is the engine's real `CStrId` is unknown.
- **`UnkVector<T>`** (0x28 bytes: data, count, capacity, growth step, a bool, allocation flags,
  allocator) is the container at `CType+0x90` and a reflected function's parameter-type list
  (`+0x8`). It grows to the next multiple of the step through allocator slots `0x40`/`0x48`, and
  copies elements bitwise (the original reloads `count` after the store).
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
| `nn::` declarations from **open-ead/nnsdk** as a submodule (owner-approved) | Community headers; no license, so referenced, never copied. The port must not depend on them without asking the maintainers |
| **`nn::atk` not decompiled**; stays `L` | Nintendo middleware, not game code: legal exposure for little gain. The port replaces it with an open audio backend (BotW treats Havok the same way) |
| No Eden/GDB tracing for now | The static call graph gives a sound minimum for the slice; tracing can be added later |
| Getter/constructor code generated from the binary | Thousands of instances of one template; generated sources are outputs, regenerate them |
| Explicit instantiation of the descriptor constructor in each getter TU | Keeps it out of line as in the original (implicit instantiation inlines it); a `noinline` attribute would match equally |
| Repo private, MIT licensed | Owner's choice |
| Generate TU-independent functions outside their TU's getters (skipped-TU files, `registration_NN`, `function_inits_NN`, `class_descriptors_NN`, `src/lifted/`) | Their bytes don't depend on the TU's statics; only the getters do. They can move into their TU's file once it is generated |
| Lifted functions use `char*` parameters and casts, not real types | Byte-identical and usable as a reference; real signatures come when the subsystem is decompiled by hand |
| **Padding statics** for TUs with unreconstructed statics (owner-approved; replaces "no dummy statics") | LLVM's AArch64 GlobalMerge packs a TU's internal statics into one block sorted by size, and getters address descriptors from their guards, so the other statics shift the descriptors. The pad is internal `char` arrays of the same total size (pieces under a descriptor's size) plus an anchor used with them, in a small unmapped function. Each padded file is verified; it falls back to the skipped-TU form otherwise |
| Repeated functions are instantiations of one template with a tag type per address (`gen_clones.py`) | The original has many identical, unfolded instantiations; one hand-matched template covers them all and keeps the address in the symbol name. Element types are placeholders chosen for identical code generation (`UnkClone.h`) |

## Open questions for the owner

None blocking. The owner asked for best judgement on `nn::atk` and tracing (decisions above).

## Next steps

0. **Clone generator (`tools/gen_clones.py`), done this session.** Exact-clone families (same
   instructions up to branch targets) were 1.3 MB of `U`. To find more, group `U` functions by
   `gen_clones.instructions()` (branch targets made relative) and sort by total bytes. Three kinds of entry:
   a verified reference (exact or shape match), a **probe** (no reference: the instantiation is
   compiled and its mnemonics searched for, so a template matched for one element type finds its
   copies for others) and a **scan** (facts read per candidate; `dtor`). Matching notes:
   - Union members model the original's missing alias info in several places (refcounts,
     `UnkVector`'s count in the registry, owning/handle element pointers, `UnkIterValue::Ref`).
   - `UnkVector`: trivially copyable elements are copied with `memcpy`/`memset`, others moved one by
     one (source walked by pointer, destination by its own pointer, 32-bit counter); `Insert`'s
     append path is `PushBack`'s shape (count re-read before `data` is stored). Trivially copyable
     8-byte elements match as `UnkCloneElem<Tag, 8>` (bytes) where a pointer element doesn't.
   - Left: `UnkVector::EraseAt` (fn_71002b9908, 28 × 192 B: loop counter and source offset in
     separate registers), ~400 destructors with other member kinds (vectors, nested objects),
     the CStrId-valued iterator family (fn_71000c936c, fn_71000cea70) and a long tail of
     families under 8 KB each (re-run the census for current numbers). The remaining call-free vtable
     functions (509) are varied virtual dispatchers, not one template.
1. **Extend the lifter (`tools/gen_lift.py`), largest payoff first.**
   - Done (thirteenth session, cloud): lifting now runs in parallel processes (a full run is
     ~105 min on 4 cores); output is one file per 64 KiB with per-function declarations, so
     `--prune` drops functions defined elsewhere in seconds; `--best` writes the closest attempt of
     any function for hand matching. Inlined guarded-static helpers (`GUARDS`: string pool,
     singleton, type id; replicas in `HEADER`, namespace `lift_guard`) lift as one statement:
     +126 `O`. Negative results: of 800 unlifted candidates none match in any round, and a
     one-field-struct ("field") rewrite for struct-path TBAA added 0 of 578 mismatches. Next, from
     the statics thread: a fourth helper (guard GOT 0x7101cba0c0, a pool getter followed by
     `bl 0x710007fdb4`), tbz up to four instructions after the ldarb, and the DSO handle
     (0x7101a19000) lifted as `""` instead of an extern.
   - Done (twelfth session), mismatch study and second-round rewrites (+1,039 `O`, ~146 KB;
     45.39% to 46.16%). Lifted-but-not-matching functions were classified with a scratch script
     (best attempt per function vs the original; each normalisation step, in order, removes part
     of the differing instructions, and the function's class is the step that removes most).
     Sample A: `--sample 600 --seed 1 --new` (450 mismatches, 172 KB); sample B: 600 new
     candidates of 256 B-1 KB, seed 2 (396 mismatches, 184 KB). Counts / bytes, A then B:

     | Class | Before (A) | Before (B) | After (A) | After (B) | Example |
     |---|---|---|---|---|---|
     | scheduling (same instructions, other order, incl. `ldp`/`stp` pairing) | 170 / 65 KB | 148 / 70 KB | 158 / 64 KB | 148 / 70 KB | fn_7100025bd0 |
     | registers (same instructions, other registers) | 120 / 61 KB | 143 / 67 KB | 111 / 62 KB | 136 / 64 KB | fn_710006f230 |
     | other (several kinds at once; mostly extra `mov`s and missing reloads) | 143 / 45 KB | 102 / 46 KB | 132 / 39 KB | 104 / 47 KB | fn_7100020a68 |
     | extra or missing instructions | 12 / 1 KB | 2 / 1 KB | 11 / 1 KB | 1 / 0 KB | fn_710002e468 |
     | constants materialised differently | 4 / 0 KB | 0 | 4 / 0 KB | 0 | fn_710090ca38 |
     | stack layout (sp offsets, frame) | 1 / 0 KB | 1 / 0 KB | 1 / 0 KB | 1 / 0 KB | fn_7100861e38 |
     | block or branch layout | 0 (365 insns) | 0 (440 insns) | 0 | 0 | fn_7100105a80 |
     | now matching | - | - | 33 / 5 KB | 6 / 2 KB | |

     Three causes inside "scheduling" and "registers" were fixable from the lifter's output, so
     functions that compile without matching get one more round per `ROUND2` rewrite (a round
     never loses a function that matched): **alias** (the biggest: the original reloads values
     after stores of another type, e.g. a pointer re-read after a refcount store, so every access
     goes through a `may_alias` typedef `a_u32` etc.; `-fno-strict-aliasing` alone gave 17 of the
     450), **sort** (field stores of one base in ascending offset order: the scheduler and store
     pairing moved them; as the only mode it loses 31 per 1,000, hence a second round) and
     **copy** (a load run then a store run of the same values becomes `__builtin_memcpy`). Left:
     - Large functions barely move (6 of 396 in sample B): most are flat-mode loops whose block
       layout and induction variables differ, not fixable by rewriting statements.
     - "registers" is mostly a knock-on of ordering (one moved statement renames the rest); the
       remaining scheduling differences are around calls and GOT loads (`adrp`/`ldr` placement).
     - "other": we emit ~3x the original's register `mov`s (values copied into callee-saved
       registers across calls), and the original uses more `add`s and `subs` where we use
       `sub` + `cmp`. Before the alias round the original also had ~25% more loads than us in
       these functions; after it the counts are about even.
     - The full run takes ~80 min on the PC (~40 min first round, ~12 min per rewrite round).
   - Done (tenth session), three cheap fixes (+83 functions, ~45 KB): `MAX_SIZE` is 16 KB (was
     2 KB); a stack load (`ldr x, [sp, #k]`) no longer crashes the GOT-load check (~2,000 more
     functions lift, few match: the `char stk[]` layout differs); code ending in `bl` is a call to a
     `[[noreturn]]` callee (`namespace noret`).
   - Done (tenth session), scalar floats (+471 functions, ~42 KB): `fmov` (immediates, register
     bit casts, mov/movk constants as hex literals), arithmetic incl. fused forms, `fcsel`,
     `fccmp` chains, conversions, `ldp`/`stp` of s/d, float call results (a call is retyped to
     return a float when s0/d0 is read after it: `namespace fret`/`dret`) and float tail calls.
     Clang 9 forms that matter: `fmin`/`fmax` are `!(a >= c) ? a : c`; `fcsel` between 1.0 and 0.0
     is `(float)(cond)`; after `tst w<n>, #1` only a `bool` parameter keeps `fcsel ne`; negated
     float conditions invert the cc (`a != b`, not `!(a == b)`); float accesses to hidden
     globals use an aligned declaration (`namespace h4`/`h8`) so the page offset folds.
     Left: of ~5,400 float functions, ~3,600 now lift but ~90% still differ in scheduling and
     register order (e.g. `fcvt` order for vararg calls, commuted `fadd` operands). Others are
     blocked by integer gaps: guarded statics (`ldarb`), `ccmp`/`ccmn`, jump tables (`adr`).
     Near-miss ideas: a `bool`-parameter variant for integer `tbz`/`csel` on bit 0 (the same
     flip as `fcsel`), aligned declarations for integer global accesses. It lifts 47,431 of 75,661
   candidates (<= 512 B) and 39,598 of those match. Rejection reasons (count / bytes, at 128 B;
   rerun `build/scratch`-style tallies by subclassing `Lifter` and recording why `step` fails):
   - **Guarded function-local statics** (`ldarb`): `tools/gen_statics.py` covers attribute keys
     (61), static string ids (386 from a literal, 55 copied from a global), typed lookups by name
     (203), registries (51), tables built from a registry (`refl_RegistryMapInit`, 37) and internal
     static string ids (plain functions, 13), type-id senders (`refl_SendTypeId`, 11) and
     parts of one singleton (`refl_GetSingletonPart`, 5), typed lookups whose class is internal to
     another TU (`refl_FindTypedHidden`, 5) and attribute keys with a u16 at +0x14 or a global value
     type (`refl_GetAttrKeyFlags`, `key_global`; 17), and game code parsed rather than shape-matched:
     a u32 sent under a hash (`refl_EmitU32`, `parse_emit_u32`; 12 of 15 parsed match). Game code with
     its own static (a string id or file-path id from a literal, or a zeroed word) goes through
     `gen_lift.py`'s Lifter, patched in-process by `own_patch` (`src/reflection_statics/own_*.cpp`):
     14 of 281 candidates up to 1 KB match. Most of the rest lift but differ in the stack frame
     around the guard (the original spills x0/x1 below its locals) or in general lifter gaps. Left (stream 6 census, 2026-10-21): 2,483 `U` functions call
     `__cxa_guard_acquire` (2.28 MB); 2.15 MB of that is one-offs (game code with a static
     inside), and only ~130 KB is in same-shape families of two or more. Older notes: the 437
     class getters in skipped TUs, primitive type getters (`refl_GetType<unsigned>`, 30 × 480 B:
     an inlined `CType` constructor whose kind hash and name share one string), 55 × 356 B
     serialisers and 46 pointer getters whose pointee is in a skipped TU. Add a kind: a template
     in `UnkReflStatics.h`, a reference address in `REFERENCES`, its constants in `facts()`.
   - Stack locals are handled (one `char` array over the gap between save slots, or over the
     bytes accessed; `STACK_LAYOUTS`), but only ~22% of those functions match. Parameters first
     read as `w<n>` are `u32` (this fixed many `mov w2, w1` vs `mov x2, x1` mismatches).
   - Mismatches now dominate (60,768 lifted, 46,219 match after the twelfth session). Only ~1,600 (165 KB) differ in
     register names alone; most differ in instruction selection or scheduling (typed struct
     accesses in the original vs our `char*` arithmetic), so the generic lifter is near its
     limit. Further gains need real types per subsystem (hand decompilation), not variants. Remaining unlifted (~4.6 MB): guarded statics (`ldarb`, 1.2 MB),
     `ccmp`, float arithmetic, early returns of values set on only one path.
   - **`CCollectionType`** (`include/base/reflection/CCollectionType.h`): the descriptors are
     globals built by static initialisers, so only constructors are generated: 178 standalone
     (`gen_types.py`; an element descriptor loaded through the GOT is a global, not a local
     class) and the rest in their element class's TU (`gen_reflection.py`, "local items"; 175 of
     181 local pointer/collection types match). The static initialisers
     that build the descriptors (e.g. `fn_71000e2800`, 14 KB) are still `U`.
   - sret (`x8` read first) and crossing branches/loops are handled (UnkResult built in place;
     `FlatLifter` with gotos), but only ~25% of flat-mode lifts match; the rest differ in
     register allocation or block layout.
   - Missing instructions: `tst`, `subs`, `csel`, `madd`, `udiv`, `mvn`, shifted/extended
     register operands (`add x0, x1, x2, lsl #3`), register-indexed addressing, loops
     (backward branches), float arithmetic.
   - **Mismatches** (~7,800 lifted but not byte-identical): register allocation differences
     (callee-saved registers assigned in another order, ~3,200), store scheduling in large
     constructors (~1,200, mostly descriptor constructors of other kinds), conditional-branch
     layout. A variant that changes declaration order of temporaries may help the first.
   - Raising `MAX_SIZE` beyond 512 adds little (most large functions fail to lift); per-variant
     scratch TUs are capped at 8,000 functions (`SCRATCH_CHUNK`, ELF section-index limit).
2. **Remaining reflection.** Done this session: pointer types whose pointee getter is in the
   same TU (generated into that TU; constructor uses the constant address), the `CPointerType`
   flag at `+0xe0` (true for `std::unique_ptr`), helpers/attribute functions/descriptor
   constructors of skipped TUs (no getters, no dummy statics), registration functions outside
   class getters (mostly container accessors passed by static initialisers) and every
   out-of-line reflected-function initialiser (`refl_attrs.parse_initialiser`). Left:
   - Done (sixth session): class getters in TUs with **other statics**, through padding (see
     Decisions; `layout_gap`/`padding_lines` in `gen_reflection.py`); the 54 container accessors
     (912 B), generated in their element class getter's TU.
   - Done (seventh session): **every located class getter** (2,354 of 2,355). The shape check
     ignores the descriptor's offset from a same-TU parent's (`add xN, x2, #imm`); six base library
     getters inline the name's constructor (`CStrId(UnkStrIdInline{}, ...)` in `CStrId.h`, the
     same body as `fn_71000003d4`, which matches out of line but is marked `L` by neighbour
     inference) and are verified by compiling them, like padded TUs. `tools/recon/reflection.py`
     names class/enum getters MEDS doesn't list (`CGun`, `GUI::CBossRushReport`, ...) and ignores
     functions over 640 B: those have a getter inlined (the "2 non-contiguous TUs",
     `CSubAreaManager`, were such a misdetection; seven `CAbility*` types had shared one wrong
     getter). Primitive `CType` getters (`refl_GetType<float>`, `fn_7100008500`;
     `UnkReflPrimitiveType.h`), pointer getters with the constructor inlined and `CFlagsetType`
     constructors (`gen_types.py`). Registration functions: `CRntString`, `CFilePathStrId`, `CName`
     and `CVector4D` values, function/parameter attributes through `UnkReflFunctionVtbl`, the field
     attribute container at `+0x70`, and a lambda-wrapped form for the stack-slot order
     (`refl_attrs.verify` retries alternatives): 1,911 of 1,957 attribute/field functions, 508 of
     562 others. The inlined string-id constructor (`CFilePathStrId(UnkStrIdInline{}, ...)`, its
     pool created on first use by a guarded static in a cold tail) matches since the thirteenth
     session (7 functions), as do `CName` values stored as a hash (`UnkNameHash`) and copies of
     any global `CStrId` (2), ids copied from a string-id table built on first use
     (`refl_GetStrIdTable`, 3) and enum defaults kept in guarded weak statics (`refl_GetEnumZero`,
     `refl_GetEnumNot8`, 4), the `CRntString` inside the string `g_unk_7101d470e0` points to
     (`refl_GlobalRntString`, 3) and string-list values (`UnkReflStringList` built from an
     initializer list by `fn_7100228ca0`, 4), the same from a string-id table
     (`refl_RntStringOf`, 1) and calls on a field found by its name's hash, setting a
     getter-backed field's function index (`refl_FindField`, 4) (all in `UnkReflAttributes.h`).
     Left there (13, one-offs): `fn_7100408c20` (33 KB), `CVector4D` copied from a constant,
     `fn_71004ded68`, `fn_7100beaf3c`, `fn_71000961f4`, `fn_710044e7fc`, `fn_7100b8bb94` (a u16
     global or'd into a version), and 6 registration functions outside class getters. The 4 that
     parsed but differed (`fn_71004ade14`, `fn_71007a2fb4`, `fn_7100add284`, `fn_7100db69a8`) match by
     hand in `src/game_s7/`: a type getter defined in the class's own file is called but its known
     return passed as a hidden constant, written `(fn_getter(), &g_unk_<addr>)` with the global
     declared `visibility("hidden")`. `refl_attrs` could emit that form itself. List them by failure point with a
     copy of `refl_attrs.parse` whose `return None`s raise their line number.
   - Descriptor helpers of **collection iterator values** (`UnkIterValue<T, K>`, K = index or key):
     move-construct, destroy, copy-assign and compare as `gen_clones` kinds (word, owner,
     string-id and keyed elements; a scan kind for elements with a vtable and string-id members,
     `UnkDtorElem`). Left: keyed move/copy-assign (written in `UnkStrIdMemberOf`, differ in
     scheduling), elements with vectors or out-of-line destructors, and an element destroy keyed by
     a string id that compiles to a 4-byte thunk when it shares a file with an identical one.
   - Done (eighth session): two `gen_clones` scan kinds. `pointee_typed` covers the reflected
     pointer helper that returns the pointee with its dynamic type (`UnkReflTypedPtr.h`, 328
     functions). Its getter is called once per branch, which only matches when the null branch
     calls a second, linker-folded copy of the getter (`folded::fn_<addr>`). `iter_value_destroy_dtor`
     covers iterator-value destroys whose element has an out-of-line destructor (50). The
     helper-slot census (`class_constructors` traits whose status is `U`) now gives 2,355
     functions, 374 KB, in small families (the largest is 5 KB).
   - Done (eighth session): scan kind `iter_element_value` (162 functions): an iterator value's
     element returned as a reflected value (`refl_GetIterElementValue`, `UnkReflValue.h`). The
     element stands in as `UnkCloneBlock` (u64 words; a byte array of the same size forms the
     element address differently and doesn't match).
     Leaf copies load the element type's descriptor through the GOT (143 more).
   - Done (eighth session): scan kinds `field_set_value` (scalar property writes,
     `refl_SetFieldValue`, 187), and `call_and_cast_arg` / `call_and_cast_field` (overrides that
     call their base, then cache a pointer cast to a class with the is-kind-of test
     `fn_710009d50c`, `UnkReflCast.h`, 163).
   - Done (eighth session): setters that clamp an f32 and notify a target by a 64-bit id
     (`refl_SetFieldMinNotify`, 73) or update a lazily created singleton (`refl_SetFieldApply`, 75);
     collection-iterator copy-assignment with out-of-line element destructors (`iter_assign_dtor`,
     52; `UnkCollectionIter` now takes the index type). Deleting destructors free through
     `__builtin_operator_delete` like compiler-generated ones, so stores before the free are
     dropped (40 more `dtor` matches).
   - Done (eighth session): more destructor shapes in `UnkDestructors.h`: no base call
     (`refl_DtorNoBase`, 49 more `dtor`), linked lists freed through their allocator (`list_dtor`,
     57), and classes with a secondary base linked into a refcounted owner's list (`linked_dtor`,
     87); `refl_SetDynamicValue` (`UnkReflCast.h`, 49 identical copies, a reference kind).
     Capstone prints immediates below 10 in decimal, so operand regexes need `(0x...|\d)`.
   - Done (eighth session): `hash_map_clear` (`UnkHashMap.h`, 44), `list_deleting_dtor` (57),
     `iter_element_dyn_value` (44), `iter_value_destroy_strid_elem` (34), and 14 reference kinds for
     families of identical copies (`UnkCloneHelpers.h`, `UnkSharedRecord.h`, `refl_IterSync`; 188).
     Census of identical-copy families (`U`, same normalised instructions): ~0.8 MB in 1,345
     families. Tried and left: the state machine at 0x71002fab10 (44 copies; the original keeps
     `next` in two registers), keyed-array Reserve 0x71001358e0, 0x71002b9908, 0x71000204d4 and
     0x7100287de8 (98%, one swapped pair), 0x71000df388, 0x7100241b30, 0x7100122160, 0x71001f3be4.
   - Done (eighth session, round 2): scan kinds `id_record_assign` (28), `strid_member_ctor` (25),
     `vector_move_poly` (25), `iter_value_destroy_strid2` (24; instantiates `UnkIterValue::Reset`,
     which the original doesn't inline), `iter_value_copy_assign_elem` (19), and 23 more reference
     kinds in `UnkCloneHelpers.h` (~280 functions). `gen_clones.py` now also retries `W` candidates.
     Left: 0x710013208c, 0x71004a84c8 (96%, an inverted branch pair), 0x71001f3a64, 0x71000cbef4,
     0x710035480c (the state-machine shape again), 0x7100020898, 0x7100085930.
   - Done (eighth session, round 3): 32 scan kinds over shape families (593 functions), among them
     `derived_copy_ctor` (68), `hash_key_fields` (55), `callback_base_dtor` (34),
     `callback_list_destroy` (28), `vector_free_poly` (26), the string-id value vector
     (`UnkStrIdValueVec.h`), pool lists, per-TU type initialisers (`type_static_init`), cast-and-register
     and owner destructors. Templates are in `UnkCloneHelpers.h`, `UnkStrIdValueVec.h`, `UnkCache.h`,
     `UnkDestructors.h` and `UnkHashMap.h`. Left: 0x710088af9c (a state machine's update, 15%),
     0x7100020eb0 (string-id hash erase, 44%: block layout), 0x71004f0648 and 0x71000d6310 (block
     layout), and the large per-TU static initialisers 0x71003a8a80, 0x71002d4770, 0x710021c330.
   - Done (eighth session, round 4): `keyed_set_assign` (21), `keyed_set_iter_value_copy` (11),
     `vt_iter_end`/`vt_iter_begin` (11 each), `vt_iter_sync` (8), `owner_vec_insert` (5). Close but
     not matching: 0x7100f5ff2c (`refl_StrIdValueInsert` shape, 4 instructions of register choice)
     and 0x7100f5f5ac (an erase whose index/pointer round trip through an exact division by 24
     Clang 9 folds away). Skipped: 0x710035d230 (static initialiser), 0x710065fb20 (state machine).
     Not yet tried (the run hit its usage limit): 0x71000c3cc0 0x71000c3c14 0x71000d6914
     0x71000d60e4 0x71000d6de0 0x710006d11c 0x710009abf8 0x710008fa30 0x71002464fc 0x7100245c4c
     0x71002e19d0 0x71002e1904 0x71002d3f88 0x71002fa6cc 0x71002ca39c 0x7100300380 0x7100241ddc
     0x7100239cf4 0x710023c3e0 0x71001f30c0 0x7100b8eec8 0x710098e094 0x7100bf7624 0x71003d3cf8
     0x7100e02988 0x7100c79a80 0x7100536450 0x71006cc484 0x71009ba4ac 0x71004f0ab8. About 600 KB
     is left in shape families of five or more `U` members.
   - Done (ninth session, 2026-10-20, round 5, on the PC): most of the round-4 list (~490
     functions). Kinds: `variant_assign`/`variant_assign_pair` (a string id + value or pair
     variant, `UnkStrIdValueVariant`, 36), `keyed_value_array_dtor` (10), `keyed_value_get` (a
     keyed reflected-value lookup per value type, 10), `iter_assign_block` (iterator assignment
     over block elements, 160, of which ~130 matched other kinds before), `vector_clear_poly`
     (25), `value_holder_deleting_dtor` (38), `derived_copy_ctor_48` (26), `strid_copy_ctor`
     (three vtables, 22), `iter_value_destroy_poly_vec_key16` (4), `keyed_pair_iter_assign` (20),
     `set_matrix` (8), `value_map_insert` (a chained hash map of string ids to values, 11) and
     `shared_copy_ctor` (a 0xd0-byte class with an atomically counted pointer, 14).
     The `dtor` scan now reads member offsets below 10 (capstone prints `#8`; 54 more) and treats
     a second vtable store as an inlined base destructor (`refl_DtorInlineBase`, 27 more).
     `gen_statics.py` has a second attribute-key reference for keys whose u32 is 1 (`key_one`, 9).
     Lessons: struct-path TBAA lets Clang skip reloads that the original does after stores to a
     node's links; store through a raw `char*` offset instead. Clang keeps the null check of a
     node only in the form `if (n) new (n) T(...)`. Left: 0x71000d60e4 (the variant's move
     constructor, 63%: one branch schedules the pool load early), 0x71000d6de0 (the map's erase,
     block layout), 0x7100300380 (find by string id, then cast), 0x71003d3cf8 and 0x7100c79a80
     (game code around guarded statics), 0x7100536450 (a TU's static string-id destructors). Not
     yet tried: 0x7100245c4c 0x7100241ddc 0x7100e02988 0x71006cc484 0x71009ba4ac
     0x71004f0ab8. Tooling note: in the Windows Bash tool, heredocs can lose backslashes; write
     patches to Python sources with the Edit tool.
   - Done (ninth session, round 6): a new census that skips everything already in `src/` (486
     functions). Iterator values and iterators: `index_pair_iter_assign` (16),
     `iter_value_copy_assign_index` (14), `iter_value_from_ref` (72), `iter_value_move_ool` and
     `iter_value_move_ool_strid` (an element with an out-of-line move constructor,
     `UnkCloneMoveElem`; 17 + 26, the latter the round-5 leftover 0x71000d60e4),
     `iter_value_destroy_key16` (14), `iter_value_destroy_refptr` (12). Keyed arrays:
     `keyed_find` (26), `keyed_find_then` (20), `keyed_find_assign` (15; the original doesn't
     tail-call the assignment, so the template uses `disable_tail_calls`). Vectors:
     `vector_free_dtor` (10), `vector_clear_dtor` (10). Classes: `call_with_owner_id` (15),
     `strid_dtor_at` (19), `ref_handle_copy_ctor` (20), `value_holder_dtor` (38), `zeroed_ctor`
     (49), `dtor_call_clear` (33). Small helpers: `add_f32` (25) and `pair_counts` (35; most copies
     return the record through x8). Left: 0x71000d15fc (an erase by `memcpy`, 25 copies: the
     original masks the scaled index, as if the loop were idiom-recognised; no source form found),
     0x71000d6a20 (a list walk to the i-th node, 23 copies, 67%: the counter's form),
     0x71000de484 (20), 0x710000dc80 and 0x7100415e18 (property reads returning a reflected value
     through x8), 0x7100142df0 (clamped colour setters), 0x71003e2ea8. In this census, families of
     five or more `U` members still hold ~500 KB; under 100 bytes, ~30 families of 10 or more.
   - Done (2026-10-21, cloud, stream 2, round 7): 13 kinds, +180 functions. `strid_value_insert`
     (`UnkStrIdValueVec.h`; each branch constructs the element itself, 10 × 1 KB), and a new header
     `UnkCloneCalls.h` for small call-shaped families: `set_flag_notify` (17), `set_flag_virtual`
     (13), `call_with_owner_child` (16), `ctor_then_init` (14), `cast_then_call` (14),
     `find_by_id_call` (13, the key's argument register varies), `call_arg_return_one` (14),
     `elem_at` (12), `copy_ctor3` (12; byte copies keep the vtable stores), `link_slot_assign`
     (12; the two state resets are a range check and a switch on 1, 2 read through the raw
     offset) and `get_lookup_f32` (11); `derived_copy_ctor` takes u8/u16 fields (+10).
     `gen_clones.defined_in_src` ignores trailing comments (a commented declaration in
     `src/base/` hid a clone). Left: 0x7100f5ff2c (the insert that calls its push-back, 4
     registers in the shared tail), 0x71008eb574 (clamped colour, 16 × 100 B: the zero register
     and loads are scheduled first), 0x710000e08c (an exact division by 3 Clang folds away),
     0x7100339a50 (a resource load around two string-id types, 9 × 760 B).
   - Done (2026-10-21, cloud, stream 2, round 8): +96 functions. `UnkCloneCalls.h` kinds
     `set_flag_notify_b`, `release_ref`, `copy_ctor2`, `set_bool`, `set_flag_call` (10 each or
     fewer), the reflected properties three pointers away `set_deep_u32`/`get_deep_u32`, `get_false`
     and `keyed_set` (parallel key/value arrays of one width). `call_and_cast_field` also takes a
     descriptor passed as a direct (hidden) address after its getter is called for initialisation
     (+10).
   - Done (2026-10-21, cloud, stream 2, round 9): +56 functions. `value_map_clear` (`UnkHashMap.h`),
     `keyed_array_copy` (the key stored as bytes so the count reloads), `slist_insert_before` (new
     `UnkSList.h`; the count is a union member), `callback_list_dtor` (`UnkCallbackList`'s
     destructor and deleting destructor), `strid_vec_dtor`, and `iter_value_destroy_poly_vec_key16`
     now reads its key and state offsets (`UnkClonePolyVecPad`; +8).
   - Done (2026-10-21, cloud, stream 2, round 10): +29 functions in a new `UnkCloneRecords.h`:
     `strid_pair_record_assign` (8), `keyed_vec_array_clear` (7; the vector loop counts in u32),
     `path_record_copy_ctor` (8; string ids and a file path copied in place) and
     `callback_entry_dtor` (6; `UnkCallbackListShort` plus a string id, with or without a key).
     Looked at and left: 0x7100769fc0 (a per-TU static initialiser). Not yet tried: 0x71000df484,
     0x71000df5ec, 0x71005b4d50, 0x7100af30a0, 0x71011d437c, 0x71000d8428, 0x7100240168,
     0x7100431cec, 0x71000d78ec, 0x71000d5888, 0x71000d1fec.
   - Done (2026-10-21, cloud, stream 2, round 11): +26 functions in `UnkCloneRecords.h`:
     `strid_vec_record_copy_ctor` (6; a string-id vector with an out-of-line Reserve, element
     stored bitwise), `add_function_holder` (7; the first libc++ `std::function` in `include/`,
     copied into a heap holder whose vtable is stored before the members), `keyed_move_assign` (6)
     and `keyed_move_array_clear` (7). Close but left: 0x71000df5ec (a keyed array shrink; the key
     loop compares a 64-bit counter in 32 bits, which no counter type reproduced) and 0x7100240168
     (`UnkSList` set-at: two instructions of scheduling in the inlined push-back; the loop counts
     up, `for (u32 j = 0; j < idx; ++j)`). Skipped: 0x71011d437c (a reflected float setter).
   - Done (2026-10-21, cloud, stream 2, round 12): `strid_vec_variant_reset` (12; a string id and
     byte vector or a word pair, with or without a key). Close but left: 0x71000d15fc (25 × 92 B,
     a vector erase by memmove; the original forms the element offset as `lsl` + `and` mask, which
     Clang 9 folds into the add in every form tried), 0x71002444f4 (the same variant's move
     constructor, state 2 holding pointers; scheduling and a tail-merged load) and 0x7100f5f0f0
     (a keyed record assigned through owned or referenced values; the original duplicates the
     key store per source state). Skipped: 0x7100eab448 (reflected setters that notify by name).
   - Enum static initialisers and the other per-TU static initialisers. These define
     globals, so they need the TU's data definitions (the 160 B ones are mostly lifted already).
   - Slot `0x28`/`0x30`/`0x38`/`0x48`/`0x50` helpers of other classes (need class layouts):
     ~2,400 functions, ~390 KB; a census of `U` helpers by slot and mnemonics (see
     `class_constructors` in `gen_reflection.py`) picks the next families.
   - A shape census (mnemonic sequence per U function, grouped) was the fastest way to pick
     templates this session: the top families were reflected-function initialisers (839 × 380 B)
     and `CClass` constructors (777 × 388 B). `tools/shape_census.py` now does this (`--top`,
     `--max-size`); `--family ADDR` lists the operands that vary across a family's copies.
3. **Lua leftovers:** mostly done (thirteenth session, cloud; continued in the fourteenth). The
   game's Lua needs `-fno-strict-aliasing` (`buildcfg.LUA_FLAGS`), a 64-bit `twoto`, `(float)strtod`
   and a 16-byte aligned jump buffer, and its binary-chunk loader is the older byte-swapping one
   (`lundump.c`: `S->swap`, a swapping `LoadMem`, no `nCcalls` check, `LoadChar` stored as is);
   all in `lib/lua/README.md`. `sigmatch.py` names look-alikes (the `math_*` wrappers, `lua_error`,
   `luaL_register`) from the `luaL_Reg` tables, by order for the rest, and now near-matches by
   mnemonic sequence (mutual best, at least 0.8, 128 bytes or more). 368 of 466 compiled Lua
   functions are named. `LoadFunction` and `luaU_undump` match exactly; `luaB_print` is game code
   (`src/lua/fn_71010a6440.cpp`, `O`: output through a function pointer, not `fputs`). Still
   differing by a few instruction-selection choices: `lua_getinfo` (one pre-indexed load) and
   `luaV_execute` (six hunks: `GETARG_Bx` index, `RKB`/`RKC` extends, a pre-indexed load); neither
   `GETARG_*` macro variants nor loop rewrites fixed them. The 55 other unnamed functions in the
   Lua address range are game code (the game dropped most of `io`/`os`/`package`/`debug`), not
   library look-alikes, and the 8-byte hook getters and the allocator group fold into other code.
   `libcheck_lua.py` could not be rerun (lua.org is not reachable from the cloud sandbox).
4. Core engine types the slice depends on. Done: the inline code of `CStrId`/`UnkStrId` (copy,
   move-assign, destructors) and `UnkVector` with the allocator interface. Done (thirteenth
   session, cloud, `src/base/`): CRC64 (`fn_7100001570`, `O`: reflected table CRC-64, init ~0,
   no final xor; the index must be written `(crc ^ byte) & 0xff`), the string-id constructor
   out of line (`fn_71000003d4`, `O`, a wrapper around the inline `UnkStrIdBase` constructor),
   the allocator getter (`fn_7100083990`, `O`, a hidden global) and the string release
   (`fn_71000803a4`, `m`: 3 instructions of block layout in the inlined set lookup). CRC64 and
   the constructor sit next to `nnMain`, so `gen_functions_csv.py` infers `runtime?` and marks
   them `L`; they match but don't count toward progress until that inference is fixed. Then the
   rest of the pool (`include/base/global/UnkStringPool.h` has the set and node layouts): erase
   (`fn_7100080a00`, `O`), insert (`fn_71000801e4`, `m`: register allocation around the heap's
   GOT address) and the buffer compare (`fn_710007f240`, `m`: a select the original speculates).
   The heap behind the string copies (`g_unk_7101d58d40`, `include/base/global/UnkStringHeap.h`)
   has six size classes (16 to 512-byte blocks carved from one allocation each): class setup
   (`fn_7100083c48`), clear (`fn_7100083d90`) and allocate (`fn_7100084030`) are `O`. Adding a function to
   a class (`fn_7100097550`, `UnkReflArray` at `CClass+0x110`) is `O` in
   `src/base/reflection/fn_7100097550.cpp`: the key search, the inlined move of the 0xb8-byte
   `UnkReflFunction` (full layout there, as local views checked against the headers) and the
   class's `_EditorOnly` attribute read with a default (`Get<T>(key, def)`; a volatile byte read
   keeps the default in memory as the original does). Left: the array's growth
   (`fn_7100097f80`), the function's constructor and destructor (`fn_710009a5b4`,
   `fn_710009aae8`), and the `U` functions next to `operator new`, probably more allocator code (`fn_7100083870`,
   `fn_71000839c0`, `fn_7100083a10`, `fn_7100083a70`).
5. Boot to title screen: startup, memory, file system / `files.toc` / `.pkg`, resource manager,
   Lua + GUI bindings, GUI, render front end, input, localisation, save probe. Estimated
   6,000–9,000 functions (static floor: 4,394 reachable from `nnMain`).
6. Port groundwork: an aurora-style `nn::` + NVN compatibility layer (see `docs/references.md`).

### Giant functions (probe, 2026-10-21, cloud thread "Biggest functions probe")

The ten largest `U` functions held 458 KB (2.4 points). What they are:

| Address | Size | What it is | State |
|---|---|---|---|
| 0x71000407f0 | 96 KB | Implicit destructor of the 1,198-id table below (every id's destructor inlined) | Not generated: structure and block layout match, register allocation doesn't (the pool sits in x8 or x0 in different ~100-id stretches). Tried 9 source shapes of the id destructor |
| 0x7100031894 | 61 KB | Constructor of a struct of 1,198 string ids built from literals with precomputed CRC64 hashes (called from ~700 functions; a guarded static, `fn_7100065ca0`) | **O**, `tools/gen_tables.py` |
| 0x7100057eb8 | 56 KB | Passes each of the same 1,198 literals with an empty 0x30-byte temporary to `fn_7100065b14` (a string escaper) | **O**, `tools/gen_tables.py` |
| 0x71000406dc | 276 B | The ids' constructor (string, length, hash, flag) | **O**, `tools/gen_tables.py` |
| 0x7100db8dec | 63 KB | Boss rush setup: twelve stages, each four to seven `UnkStrId` assignments, flags, a CRC64 and two `UnkVector<UnkItemAmount>` arrays filled by 227 inlined pushes in all (item pointers are GOT entries) | **m**, `tools/gen_tables.py` (`boss_rush`). The calls and blocks are the original's; from the third stage on, register allocation differs (it spills the hoisted array field addresses and rematerialises constants the original keeps), and in the later stages the id release is laid out out of line. Ids had to be `UnkStrId`: with `CStrId` the moved-from temporary's destructor leaves a second store |
| 0x7100ef378c | 52 KB | Loading screen setup: two widgets, then 79 inlined hash-map inserts of (enum key, `CFilePathStrId` video path, `CStrId` label, five flags) | **O**, `tools/gen_tables.py` (`video_map`). Keys 49-71 use `InsertCopy` (the copy taken inside the insert): the original's register allocation differs in that stretch and only that source shape reproduces it, so it is a shape match, not a known source difference. Needed `CFilePathStrId`'s pointer in a base struct with a defaulted copy (as `CStrId`'s `UnkStrIdBase`), which sets the compare order in its copy constructor |
| 0x7100408c20 | 34 KB | Tunable registration (field name, editor description, `refl_GetType<float>` ...) | Reflection-attribute style (stream 4's `refl_attrs.py` shapes); 94 chunk shapes |
| 0x7100c66c60 | 30 KB | Float-heavy game logic (collision, `sqrtf`) | Hand only; not regular (211 shapes in 244 calls) |
| 0x7100b66018 | 28 KB | Inventory GUI (beam list composition), 18 guarded statics | Irregular; hand only |
| 0x7100ae39e0 | 21 KB | Reflection registration of a save-data class (fields with descriptions) | Stream 4 |
| 0x7100e72680 | 17 KB | `CMinimapDef` reflection registration | Stream 4 |

**Above 2 KB, outside the six hand slices (`src/game_s7/`, 2026-10-21).** 559 `U` functions above
2 KB held 2.18 MB. Most call-table candidates are singletons; the static initialisers were dropped
(their globals are merged by GlobalMerge, and the original's post-increment folding can't be
reproduced through an `extern char[]`). Done: the loading-screen tips registration
`fn_7100e17d14` (**O**, 60 calls through an `std::initializer_list` pair), and libc++'s
`std::sort` helpers instantiated for a ref-counted handle list (`fn_710117d4d0.cpp`): `__sort3`,
`__sort4`, `__sort5` **O** and `__insertion_sort_incomplete` **m**, each mapped to its address
through an `alias` declaration. The comparator takes its handles by value and has external
linkage (an anonymous namespace changes the load order). `__sort` itself (`fn_710117c430`,
4 KB) is **m**: the original halves the length unsigned and inlines `__insertion_sort_3`, unlike
libc++ 9.0.1, so its body is an adapted copy of libc++'s in `lib/libcxx/sort.h` (owner-approved,
with LLVM's license); it has the same operations in another block layout. Left for hand work:
`fn_7101144a5c`, `fn_7100e68f44`, `fn_7100dd3624`.

Static initialisers above 2 KB (31, 152 KB) are this stream's too (the coordinator's call,
2026-10-22). Sources are written with slice 3's helpers (`src/game_5821e4/UnkStaticInit.h`) plus
`src/game_s7/UnkStaticInitS7.h` (header-id variants, named values, conversions with a type object
on one side, buffer params, empty ids, resource types, (id, value) pair lists). 26 match and 3
are `m` (register allocation or store scheduling only). Findings: a unit whose u32 sits right before its ids (merged
globals) takes a packed, 4-byte-aligned struct; each vector of ids built from a list must sit in
its own immediately called lambda, because the original inlines one initialiser function per
global and the inliner then shares one stack array between lists of the same length (so lists of
one length in a unit need one element type: `UnkIdElem` when any has an empty id); resource-type
source-extension arrays are filled element by element (an initialised array becomes a constant
copy), declared in reverse order; and the lift drops stack stores of strings kept in callee-saved
registers, so take those from the original's code. Three the lifter rejects (an inlined guarded
static: a class name id built on first use, `InitClassIdFrom`) were converted from the original's
code directly. Still `U`: `fn_7100eb4510` (an inlined vector reserve with a virtual allocator) and
`fn_71001e0820` (merged globals: the original folds one id's address into a post-indexed store).

Other functions above 2 KB (2026-10-22), all **O**: `lua_bind::GainAllItemsAndPowerUps`
(`fn_710107a880`, 37 item grants through a lazily built qualified id, `fn_7100ae9a10`), a
property-bag serializer (`fn_7100bda920`), a shape-pair collision dispatch (`fn_7100d541c0`) and
a 64-hit query (`fn_7100aaac80`). The property bag (`src/game_s7/UnkPropBagS7.h`: two vectors and
a lock, keys from the id table `fn_7100031894` builds, setters `fn_710033187c` float,
`fn_710050f4ac` bool, `fn_710050e4dc` string, `fn_7100aa3b9c` u32, destructor `fn_710050e8e4`)
is used by 121 `U` functions (121 KB, most under 2 KB). Findings: the lock is a member with its
own constructor (calling `fn_710007be8c` from the bag's constructor body moves it after the second
vector); a call whose `x0` is left as it is (the dispatch's handlers) takes an empty struct by
value first; a local array of 64 elements with default member initialisers is fully unrolled, and
a counter declared after it lands below it on the stack (declare it first). Tried and left: the
other collision dispatch `fn_7100d51368` (4 KB, inlined geometry per case) and constructors that
inline a vector reserve (`fn_7100ee1f34`).
Also **O**: a component cache (`fn_7100d78a30`, 7.3 KB, 42 slots), where each slot has its own
weak inline `static const CStrId` kind id (`Kind_LIFE()` and so on, reached through the GOT); the
comparison must take both ids by reference (`SameId(Kind(), comp->owner->id)`) or the owner load
is hoisted. `fn_710092f204` (melee hit-effect path from 15 local statics) stays **m**: the
original tests kind 5 before kind 2 and duplicates each item id's out-of-line first-use build.
Property handlers, all **O**: `fn_7101129240`, `fn_71010d3508`, `fn_710111dc08` (11 KB). Each
compares a property's name with keys from the id table and reads the value from the property's
stream (`fn_710007cf30`) or writes it there (`fn_710007cb4c`), falling back to
`fn_71010c4250`; helpers are in `src/game_s7/UnkPropS7.h`. Findings: the id table needs a sized
type (with the empty `UnkReflSingleton`, Clang decides no store can reach a key and drops the
reloads); the key's use count is a union member but the property name's is not, while the
property's name pointer is; the stream functions return an integer (a pointer compare changes
the branch weights and the block layout), and handlers return `u32` (with `bool`, the base
handler's result is masked on return). More callers of these stream functions are under 2 KB
(`fn_7101171fcc`, `fn_71010e4ab8`, `fn_71011608b4`, `fn_71010c4250` itself).
Also **O**: a state-machine setup that creates 18 states and adds them to a keyed list
(`fn_71010e2278`, 5.1 KB), and a GUI panel setup that keeps handles to 18 children found by name
hash (`fn_71011dfeb0`, 5.3 KB). Both use the shared handles in `src/game_s7/UnkHandleS7.h`
(an object's slot at vf_0x28, a handle created on first use). Findings: in the keyed list's add,
the name is copied as raw bytes and the values are union members, or the count isn't reloaded;
a temporary `CFilePathStrId` built inside the call's argument (not a named local) gives the
original's join block; one callee needed its float parameters declared before the pointer to get
the argument copies in order. The child hashes are CRC-64s computed at compile time; strings for
some are in the binary (a reverse lookup over all strings finds them), the rest stay constants.
Related `U` GUI setups that use the same child lookup (`fn_71011d98ec`): `fn_7100b39950` (12 KB),
`fn_7100b49c88`, `fn_7100b4d4a4`, `fn_7100b92e9c`.
The boss rush results screens (`fn_7100b94320`, `fn_7100b921e0`) are **m**; shared declarations are
in `src/game_s7/UnkGuiS7.h`. The button callbacks are `std::function<void()>` built from
`std::bind(&C::method, this)`; libc++'s small buffer holds the bind object. Finding for compositions
looked up through the registry (`fn_71011b69d8`, also the menu setups in `src/game_aca57c/`): take
a reference to the slot after assigning it (`self->composition = ...Get(); auto& c =
self->composition;`), not before. The original keeps the slot's address in a register, set by
pre-indexed stores, and only this order gives that. What's left: `fn_7100b94320` addresses five
loads from the object rather than through that register, and in `fn_7100b921e0` a colour copy is
only partly sunk below a branch.
Music volume overrides by stream id (`src/game_s7/fn_71010182fc.cpp`): the setter
`fn_7101019158` (4.5 KB) is **O** and the getter `fn_71010182fc` (3.7 KB) is **m**. Each is a
73-case switch on the id's CRC-64 (signed). Clang's compare tree comes out the same when the cases
are written in the order of their bodies in the binary. The getter's unmatched ids branch to a
`ret` shared with a case in the middle; ours go to the last one.
Three more long repeated functions are **O** in `src/game_s7/`. `fn_7100fc4b34` (8.4 KB) reads
about 50 tuning values from two property sets: each value is reset, then read by its definition's
qualified id from a section keyed in the id table, through a `Get<Reader>` template. The four
locals of the last two flags have to be function-scope so they get separate stack slots.
`fn_71011415f4` (3.4 KB) clears a component's flag set and re-adds 30 ids. The id is copied with
`memcpy`, then retained, as in the state-list `Add`. `fn_7100d92158` (7.4 KB) registers 29
descriptors: a temporary entry is built by `fn_710002003c`, passed to `fn_710002002c`, and its ids
are released in two steps. Three of the descriptors get a factory singleton created on first use.
Five more libc++ sort families are in `src/game_s7/`, mapped by `alias` as in `fn_710117d4d0.cpp`.
`fn_710031e030.cpp` (entries of two handles and plain data, by a float) is **O** in all six. Its
entry has the implicit copy: the plain members are copied as one block. `fn_7100566930.cpp` and
`fn_71003cd6e0.cpp` sort 0x48-byte entries (two weak handles assigned with `fn_71002a5d1c`, then
13 words), whose copy is member by member with a self-check. Their `__insertion_sort_3` and
`__insertion_sort_incomplete` are **O** and the rest **m** (register choice). The second has an
out-of-line comparator (`fn_71003cd4fc`, **O**) that returns a word: the callers test all of `w0`
(`cbz`), not bit 0 as they would for a `bool`. `fn_710072a110.cpp` sorts object pointers nearest
first: all five **O** (through a class iterator, see below). `fn_710010e470.cpp` sorts boxes
by the centre on one axis: all five **m** (see its note). Other findings:
- The game's `swap` is always inlined, so the element needs an `always_inline` ADL `swap`.
- `__sort` switches to insertion sort below 30 elements for trivially copyable types and below 6
  otherwise. `lib/libcxx/sort.h` now computes this the way libc++ does.
- Temporaries made in one comparison get separate stack slots.
- Writing an expression term by term keeps the original's order.
- With 8-byte elements sorted through raw pointers, our insertion loops walk an index where the
  original walks a pointer. The fix is the class iterator below.
- Some game comparators have internal linkage (an anonymous namespace). Then every
  instantiation is internal, and `std::__sort` inlines its one call to `__insertion_sort_3`. This
  made `fn_7100338620`'s `__sort` **O** with plain `std::__sort`, without `lib/libcxx/sort.h`.
  - Map the instantiations with `extern "C"` aliases: a declaration that uses the internal type
    is itself internal and gets dropped.
  - Write each alias as `void fn_X()` on its own line inside an `extern "C" { }` block, so the
    `NON_MATCHING` annotation attaches. `check.py` doesn't read `extern "C" void fn_X()`, so
    those functions show up as **W**.
- When an internal instantiation's comparator is empty, the game's helpers take no comparator
  argument (dead-argument elimination). An alias counts as taking the address, which blocks that.
  `fn_7100d4de00.cpp` therefore gets the instantiations emitted from an unmapped anchor function
  that calls `std::sort`, and names them with `asm(".set ...")`. That made its
  `__insertion_sort_incomplete` **O**.

Three more families: `fn_7100354e10` and `fn_710081ccd0` (0x48-byte entries, nearest first with
z taken as 0) and `fn_71010dd1d0` (entry pointers by depth, then by tile group). The insertion
sorts of the first two and all of `fn_71010dd1d0` are **O**; the rest are **m**.
- The distance with z taken as 0 matched when each position is copied into a vector with z = 0
  and a comparator member takes the full 3D distance to it. Writing out the 2D terms plus `z * z`
  gives the right values in the wrong load order.
- Clang adds the inline hint only for an explicit `inline`: a member defined in its class gets the
  default threshold (250 instead of 325). `fn_71010dd1d0`'s comparator is inlined everywhere, as
  in the game, only when `operator()` is declared `inline`.
- `fn_71010dd1d0`'s helpers had their comparator dropped (internal, empty), so it uses the
  anchor and `.set` names. Its `__sort3` and `__sort5` stay in `src/game_hi/`: `check.py` masks
  call targets, so they don't need names here.

`fn_7100abbd40` is the same code as `fn_7100354e10`'s family. `fn_71001e2e30` sorts object pointers
by a float in a copied info block, with an internal comparator: all five **O**.

**Sort through a class iterator.** Many of the game's pointer containers have their own iterator
class, and libc++ then instantiates the helpers on it rather than on a raw pointer.
`src/game_s7/UnkIter.h` is a minimal random-access iterator over a plain array. Sorting through it
fixes three differences at once:
- the insertion loops walk a pointer, not an index;
- `__sort4`/`__sort5` inline the way the game's do;
- the halving in `__sort` is unsigned, even with plain `std::__sort`.

With it, `fn_710072a110`, `fn_71010dd1d0` and `fn_71001e2e30` became fully **O**. So did the clone
families `fn_71000a0660`/`fn_71000a5e00` (by name, `strcmp`) and
`fn_71002c5c00`/`fn_71007f6100`/`fn_71007f6b90`/`fn_71007f7620` (by a float; the float is in a
union, so the original reloads it after each element store). Try the iterator first on any sort
family. It also made `fn_7100213380` (0x40-byte polymorphic boxes, below) fully **O**. It doesn't help the 0x48-byte handle
families (`fn_7100566930`, `fn_71003cd6e0`, `fn_7100354e10`, `fn_710081ccd0`), `fn_710010e470` or
`fn_7100338620`.

Also claimed by stream 7 (2026-10-22, coordinator-approved), although each `__sort` is under
2 KB: the sort families `fn_7100256810`, `fn_7100742750`, `fn_7100743340`, `fn_7100744000`,
`fn_7101072a80`, `fn_710109b9f0` and `fn_71011bc0b0`, with their helpers. Stream 7 also takes any
further libc++ sort family it finds, after noting it here. Hand slices, leave these to stream 7.
Further families claimed the same way: `fn_71002aa690` (0x30-byte elements) and `fn_7100c75e40`
(0x28-byte elements), with their helpers. Both are done on plain pointers (the class iterator made
them worse), with comparator types of external linkage, since each keeps `__insertion_sort_3` out
of line:
- `fn_71002aa690`: nearest first with z taken as 0, like `fn_7100354e10`. Four **O**. The two
  insertion sorts are **m**: the held entry's store-back is addressed from another register. The
  position's x and z had to be union members; y as well made it worse.
- `fn_7100c75e40`: by a key, then nearest first in 2D. `__sort`, `__sort4` and `__sort5` are **O**.
  `__sort3` and the insertion sorts are **m**: the original loads the first entry before the
  branch that picks the swap.

All seven are done (2026-10-22), each through `UnkIter`: every function is **O** except
`fn_7100744000`'s own `__sort` (**m**, below).
- `fn_7100742750` compares with a plain function pointer. With a comparator type that isn't
  internal, `std::__sort` keeps `__insertion_sort_3` out of line (`fn_7100742ee0`), as this copy
  does; an internal comparator inlines it, as the other six do. So whether the game's `__sort`
  calls `__insertion_sort_3` tells you whether the comparator was internal.
- `fn_7100256810` (an s32 field), `fn_71011bc0b0` (an s32 key with a fallback field) and
  `fn_7100744000` (a float key, then an id) needed the compared field to be a union member, so
  that it is read again after each element store. `fn_7100744000` also needed a swap that assigns
  the two members in turn, and its `__sort` is still one instruction off (the original advances
  the pointer and then loads; ours loads with a pre-indexed address).
- `fn_7101072a80`, `fn_710109b9f0` and `fn_71011bc0b0` have empty comparators, so they use the
  anchor and `.set` names.

More instantiations found by their recursion (each 2 KB, or with 2 KB helpers):
- `fn_7100134020`: all five **m**. 0x160-byte entries (three pooled string ids and a string with
  its own storage) by name. One difference is left: the original reuses the compared id's pointer
  when a swap moves the entry. That needs the alias analysis to know the swap's temporary isn't the
  entry, which it does only for pointer arguments, but sorting raw pointers pairs stores the
  original keeps apart. See the file's note.
- `fn_7100213380`: done, all five **O**. 0x40-byte polymorphic boxes by the centre on an axis. A
  copy rebuilds the box from its corners and stores them again as whole tenths. Three details
  mattered: one inline virtual keeps the vtable out of the file (the copy's calls stay indirect),
  the assignment is user-written (so the tenths and the last word are copied separately), and the
  elements are sorted through `UnkIter`.
- `fn_7100212a80` (not a sort: it builds a box tree and calls `fn_7100213380`)
- `fn_7100248e50`
- `fn_710030ed9c`
- `fn_7100336ca0`: all five **m** (weak handles assigned by `fn_71003384e0`, nearest first).
  The class iterator fixed the handle reload and the sizes. The comparisons still load the target
  and the positions in another order, whatever the distance's shape.

Copies of slice 2's `fn_71002dacc0` (a pair of empty string ids) outside slice 2's range,
`fn_7100a8e07c` and `fn_7100ef1970`, are stream 7's (coordinator's call, 2026-10-22): both **O**
in `src/game_s7/fn_7100a8e07c.cpp`.

**Copy-assignment of property records** (stream 7, 2026-10-22; `fn_710043439c` and `fn_71004c8f58`
are under 2 KB in slice 2's range but share the types, so stream 7 took them too, with no open PR
touching them). `fn_7100442780`, `fn_710043439c`, `fn_710048b740`, `fn_7100510548` and
`fn_71004c8f58` are all **O**: records holding 0x70-byte entries (a property with an any-like
value assigned by `fn_710009efb8`, then a 0x40-byte block). Types are in
`src/game_s7/UnkAnyPropsS7.h`. Each operator= is defaulted out of line and aliased to the address,
because a wrapper calling it doesn't inline it. With a defaulted operator=, the layout alone decides
the code:
- runs of trivially copyable fields become one memcpy (loads, then stores);
- fields copied one by one, interleaved, are wrapper types with a user operator= (`UnkVal<T>`,
  or a struct with its own operator=; field order inside it can even be reversed);
- an 8-byte gap that is never copied is a polymorphic member's vptr;
- the weak handle's operator= needs `always_inline`.
The memcpy of the last array entry is scheduled with whatever plain fields follow it, so the
load order there tells you whether the next field is plain or wrapped (`fn_7100510548`).
`fn_7100fbdfd0` (slice 6's range) also calls `fn_710009efb8` once; it is not part of this family.
The records' copy constructors are **O** in the same files: `fn_7100441cfc`, `fn_710048acac`,
`fn_710050f824`, and in slice 2's range `fn_7100433c5c` and `fn_71004c8ad4` (stream 7's, same
terms). Each is defaulted and aliased like the operator=. What the constructors needed:
- the entries are separate members (an array is copied by a loop) and derive from the property,
  each level storing its vptr;
- the records derive from a base (vtable 0x7101cd1b38) and sometimes a middle class, whose vptr
  store survives only if an untyped copy follows it (`fn_710048acac`'s two-u32 field);
- the block, `UnkPropRef`'s values, `UnkVal` and `UnkTail` have user copy constructors, so they
  are copied one by one instead of joining a memcpy run.

**Collection-type registration** (stream 7): `fn_71000e2800` (14 KB, **O**, no callers in code)
adds 105 type getters' results to their one-byte lists (`fn_710009ec00`), builds 60 `CClass`
globals from their names (`CollectionTypeMapper<...>` value types, `CRntVector<...>` and so on;
the constructors take four or six arguments), registers their destructors and 45 conversions.
Every global and callback is reached through the GOT, so all are plain `extern`s. The source was
written mechanically from the call sequence, with the helpers in `UnkStaticInitS7.h`.

**Exact copies of matched functions** (stream 7, claimed 2026-10-22): a whole-binary scan for `U`
functions whose instructions equal an `O` function's (local branches normalised, calls and page
addresses identical) found 115 (27 KB); 114 are written as copies of their originals' source in
`src/game_s7/` (one file per original file, named after its first copy; each lists its copies),
and `fn_71004a84c8` stays with stream 2. Several were "tried and left" by the clone rounds above
(0x7100287de8, 0x71000df388, 0x7100020898, 0x71000d1fec, 0x71002444f4); the hand-matched originals
now cover them. Rerun the scan after each batch of new matches: new `O` functions bring new copies.
A looser pass (calls may target different functions with the same position in each body, and
data page addresses may differ) found 35 more `U` copies of 17 originals, also claimed by stream 7
on 2026-10-22. They are written the same way, with each copy's own callees and data declared; the
largest family is `fn_7100d94830`'s erase-by-key (9 copies).
A third pass (also masking the offsets of loads and stores whose base came from a page address)
found 56 more `U` copies of 30 originals, claimed by stream 7 on 2026-10-22; the largest family is
`fn_7100d2ac00` (8 copies, in `fn_7100a15260.cpp`). Copy files keep the original's names, strings
and comments (asmdiff masks data), and their first line says so.
The two copies of `fn_71002430e8` (a generated clone, `refl_StrIdHashErase`) that release their
keys through CFilePathStrId's pool, `fn_71000741d4` and `fn_71011aa6a0`, are stream 7's (**O**).
Stream 7 probed the id table's destructor `fn_71000407f0` (96 KB, see "Giant functions") on
2026-10-22 and left it: built as a defaulted destructor of the generated table it reloads the pool
at the same ids as the original (every 99), but the pool's register is swapped in each stretch. The
original's first stretch keeps the pool in x8 (its first load still reads `this` from x0, before
the copy to x19); ours copies `this` first and keeps the pool in x0, and a 3-id table is all x0.
Ten more shapes of the id destructor (an early return, no local pool, a cached empty pointer,
`__builtin_expect` on each test, `always_inline`) don't change that.

**Render resource types** (stream 7): `fn_71001b1dd0` (**m**, 2.5 KB) registers nine render
resource types through a record with an out-of-line constructor and destructor (`fn_71000e9a7c`,
`fn_71000e8dd0`). Findings: this TU's file-path ids decrement and test in two steps and assign as
destroy-then-move; the vector growth moves elements bitwise (`memcpy`, so `count` is reloaded) with
`src`/`dst` pointers and a `u32` counter; the file's statics are one merged block
(`g_unk_716e5543a0`). Left: the last copy loop's count stays in memory in ours (one instruction).

**Copies of stream 2's shapes** (stream 7, claimed 2026-10-22): the static-id guard copies of
`fn_7100464520` (`fn_71005b9d70`, `fn_71006e0f90`, `fn_7100898e80`, in `fn_71005b9d70.cpp`) and the
copies of the compare `fn_71004a84c8` (`fn_71005d9dec`, `fn_71005ee098`, in `fn_71005d9dec.cpp`;
each member of the compared value is inline or, in mode 2, a pointer; the loop index must be `u64`).
`fn_71004a84c8` itself stays with stream 2. Also claimed (2026-10-22): the three copies of each of
stream 2's owned-list array functions (`UnkOwnedList.h`) at 0x71005da4f8-0x71005db4c0,
0x71005e3778-0x71005e4740 and 0x71005ee7a4-0x71005ef76c (clear, compare, free, move-assign, erase,
grow-at, begin, end), in `fn_71005da4f8.cpp`.

**Inventory string and projectile models** (stream 7): `fn_7101147868` (**O**, 4.9 KB) appends a
"SamusInventory" record whose value is 26 digits, one per item definition (1 when the inventory's
map holds an amount above zero under the item's qualified id). Findings: the lazy id check must be
a pointer compare (`data == &pool->empty`; the integer compare other files use inverts the branch
here); the record is built as a temporary inside the append call; and its constructor
(`fn_7100ec4240`, declared with an `asm` label) takes the two ids by value, so the caller destroys
the copies after the append and keeps their final stores. `fn_7101144a5c` (11.8 KB, next to it)
builds a local map from the 26 area-and-item names and walks it; still `U`.
`fn_7100ed4cb8` (**O**) loads the nine beam projectile models through the manager's pool; the
model pointer must be declared before the path id (stack order).

**Hash lists by height** (stream 7, 2026-10-22): `fn_71003f2a8c` and `fn_71006c26fc` clear a
list of 64-bit name hashes and push three to five constants, chosen by the height difference of two
positions against 300. `fn_71006c26fc` is **O**; `fn_71003f2a8c` is **m** (one store's index
register). Both are in `src/game_s7/fn_71003f2a8c.cpp`, using a local copy of the `UnkVector` layout
whose PushBack copies the old elements one by one through a pointer. Two details mattered:
- The list must be a reference parameter. Only then can the step be loaded before the first push's
  capacity test (the original selects the step when the capacity is 0).
- Each branch pushes its own last hash and returns. A shared tail lets Clang sink the pushes into
  one with a phi of the constant.

**Flag lists** (stream 7): `fn_7100eed9ec` (**O**) and `fn_7100600c98` (**m**) build a local list
of `u32` flags on the stack (`src/game_s7/UnkFlagListS7.h`: a linked list with a vtable,
allocator and flags 0x13) and hand it to `fn_71008724f8`. The list's fields are union members:
the original reloads them after every node store. `alignas(16)` puts the list at sp+0x10.
`fn_7100600c98`'s remaining difference is that the first call's flag statics share the list's stack slot.

**Link groups** (stream 7): `fn_7100de7d88` (**O**) is the defaulted destructor of 13 link groups,
each three pointer arrays plus one field. It needs 13 separate members: an array of
non-trivially-destructible members becomes a loop. `fn_7100b34b44` (**m**, registers swap from the
second push) clears a pointer list and pushes 14 addresses inside the same object.

## Known issues and gotchas

- **Linux:** the tools also run on Linux with upstream clang+llvm 9.0.1 (the Linux release tarball, with a `bin/clang++.exe` symlink to `clang++` at the path `fetch_toolchains.clang_path` expects) and `llvm-cxxfilt` on PATH. Results are identical to Windows.
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
- `tools/gen_reflection.py` now needs the project compiler (it verifies attribute-function
  candidates by compiling them); its output is still deterministic.
- Generated sources contain the string literals the original code uses (field names, editor
  descriptions), as the getters already contain type names.
- **Leftovers for the owner to delete by hand:** `C:\Program Files\Git\tmp_bak.cpp` (a stray
  386-byte copy of `src/compiler_id/fn_7100162300.cpp`, blocked from automatic removal) and the
  empty folder named `dread` next to this repository (locked by a running shell).
