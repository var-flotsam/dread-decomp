# Handoff: dread-decomp

For the next developer, human or AI, picking up this project. Read this, then `CLAUDE.md`
(rules and conventions), `docs/recon.md` (what's in the binary), `docs/compiler.md`
(toolchain), and `docs/references.md` (outside projects).

**Twelfth session (2026-10-21, on the PC, branch `claude/lifter-mismatch-classes`, var-flotsam/dread-decomp#18):** lifter mismatch study
(Next steps, item 1, "Done (twelfth session)"); `gen_lift.py` and `check.py --update` were rerun,
so the numbers below are current.

**Eleventh session (2026-10-21, branch `claude/local-agent-setup-df6136`):** set up two parallel
workstreams (below), `AGENTS.md` and `docs/tasks/` for a small local model on a MacBook,
macOS toolchain support (untested on hardware) and `tools/verify.py`. No matching changes.

## Workstreams

Two machines work at once, on separate files. Each edits only the files in its row.

| Owner | Area | Files it may edit | Current branch |
|---|---|---|---|
| PC (Claude) | The lifter: the mismatch study, then loops, `ccmp` and jump tables (Next steps, item 1) | `tools/gen_lift.py`, `src/lifted/` | `claude/lifter-mismatch-classes` (var-flotsam/dread-decomp#18, created before this table; later ones `pc/<topic>`) |
| Mac (local model, `AGENTS.md`) | Guarded statics, then small single-family `gen_clones.py` templates, then reflection and Lua leftovers (`docs/tasks/`) | `tools/gen_statics.py`, `include/base/reflection/UnkReflStatics.h`, `src/reflection_statics/`; for clone cards also `tools/gen_clones.py`, `include/UnkClone.h`, `include/base/UnkCloneHelpers.h`, `src/clones/` | none yet; new branches `mac/<topic>` |

Both may change `data/functions/*.csv` (statuses only, through `check.py --update`) and their
own row here. Anything else (shared headers, `check.py`, `CLAUDE.md`) goes through the owner first.

Merge rules:
- Merge `main` into your branch often and keep PRs small.
- After merging `main`, take `main`'s `data/functions/*.csv`, rerun only the generator you own,
  then `tools/check.py --update` (`tools/verify.py --regen <generator>` does both). Never
  hand-merge the CSVs.
- `gen_clones.py` and `gen_lift.py` skip functions defined elsewhere in `src/` (`gen_statics.py`
  runs before both), so a rerun drops duplicates.
- Branch prefixes are `pc/` and `mac/`. Only the owner pushes and merges.
- A machine that changes focus updates its row above in its next PR.

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
| Guarded statics | **In progress.** `tools/gen_statics.py`: attribute keys, static string ids, typed lookups and registries built on weak function-local statics (`include/base/reflection/UnkReflStatics.h`): 765 functions, 141 KB (`src/reflection_statics/`) |
| Repeated templates | **In progress.** `tools/gen_clones.py`: hand-matched templates instantiated once per copy (`UnkClone_fn_<addr>` tags): the named registry, `UnkVector` methods per element kind, the reflection collection iterator, a deferred set, and scan kinds (facts per candidate): destructors of string-id/refcounted members, property reads and string-id writes, enum value lists, and shape-family templates (`UnkCloneHelpers.h`): 6,256 functions, 1.22 MB (`src/clones/`) |
| 4.4 Core engine types | **In progress.** String ids (`CStrId` incl. copy-assignment, `UnkStrId`), the growable array `base::UnkVector` (push, grow-at, insert, remove, clear/free, move) and its allocator interface; reflected-function initialisers now match (below) |
| 4.5+ Boot to title screen | Not started; plan below |
| Port | Not started (`port/` is empty) |

Function status (BotW codes): `O` 80,439, `m` 3, `M` 1, `L` 1,614, `U` 21,960.
**Game-code progress: 45.39% of bytes (8.52 MB) matching** (`tools/progress.py`; library code
excluded; 44.92% at the start of the tenth session, 44.45% at the start of the ninth, 39.09% at the start of the seventh). `tools/check.py` counts `src/` definitions,
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
.venv/Scripts/python tools/gen_statics.py --version 2.1.0          # ~20 s
.venv/Scripts/python tools/gen_clones.py --version 2.1.0           # ~10 s
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
     (203) and registries (51). Left (~3,200 functions, mostly one-offs): the 437 class getters in
     skipped TUs, primitive type getters (`refl_GetType<unsigned>`, 30 × 480 B: an inlined
     `CType` constructor whose kind hash and name share one string), 55 × 356 B serialisers,
     37 × 456 B and 46 pointer getters whose pointee is in a skipped TU. Add a kind: a template
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
     session (7 functions). Left there (39, one-offs): `fn_7100408c20` (33 KB), `CRntString`
     copied from a live object, other attribute builders (`fn_7100228ca0`, 5), `CStrId` values
     read from a function-local static table (`fn_7100031894`), `CName` values stored as a hash,
     enum values in other forms, field lookups by name hash. List them by failure point with a
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
   - Enum static initialisers and the other per-TU static initialisers. These define
     globals, so they need the TU's data definitions (the 160 B ones are mostly lifted already).
   - Slot `0x28`/`0x30`/`0x38`/`0x48`/`0x50` helpers of other classes (need class layouts):
     ~2,400 functions, ~390 KB; a census of `U` helpers by slot and mnemonics (see
     `class_constructors` in `gen_reflection.py`) picks the next families.
   - A shape census (mnemonic sequence per U function, grouped) was the fastest way to pick
     templates this session: the top families were reflected-function initialisers (839 × 380 B)
     and `CClass` constructors (777 × 388 B). `tools/shape_census.py` now does this (`--top`,
     `--max-size`); `--family ADDR` lists the operands that vary across a family's copies.
3. **Lua leftovers:** 23 ambiguous and the `lapi`/`lcode`/`ltable` mismatches (`ltable` number
   hashing may need another `luaconf` change).
4. Core engine types the slice depends on. Done: the inline code of `CStrId`/`UnkStrId` (copy,
   move-assign, destructors) and `UnkVector` with the allocator interface. Left: the string pool
   itself (the `fn_71000003d4` constructor and `fn_71000803a4` release), `UnkReflArray` (the
   keyed container at `CClass+0x110`, grown by `fn_7100097550`), CRC64 (`fn_7100001570`), the
   allocator getter (`fn_7100083990`) and the allocators over the static arena.
5. Boot to title screen: startup, memory, file system / `files.toc` / `.pkg`, resource manager,
   Lua + GUI bindings, GUI, render front end, input, localisation, save probe. Estimated
   6,000–9,000 functions (static floor: 4,394 reachable from `nnMain`).
6. Port groundwork: an aurora-style `nn::` + NVN compatibility layer (see `docs/references.md`).

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
