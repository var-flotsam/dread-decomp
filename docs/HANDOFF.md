# Handoff: dread-decomp

For the next developer, human or AI, picking up this project. Read this, then `CLAUDE.md`
(rules and conventions), `docs/recon.md` (what's in the binary), `docs/compiler.md`
(toolchain), and `docs/references.md` (outside projects).

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
| Small functions | **In progress.** `tools/gen_lift.py` lifts functions of up to 512 bytes from their instructions and keeps byte-identical ones: 44,529 functions up to 2 KB, 1.78 MB (`src/lifted/`; sret functions, stack locals, a goto-based flat mode for crossing branches and loops) |
| Guarded statics | **In progress.** `tools/gen_statics.py`: attribute keys, static string ids, typed lookups and registries built on weak function-local statics (`include/base/reflection/UnkReflStatics.h`): 756 functions, 139 KB (`src/reflection_statics/`) |
| Repeated templates | **In progress.** `tools/gen_clones.py`: hand-matched templates instantiated once per copy (`UnkClone_fn_<addr>` tags): the named registry, `UnkVector` methods per element kind, the reflection collection iterator, a deferred set, and scan kinds (facts per candidate): destructors of string-id/refcounted members, property reads and string-id writes, enum value lists: 2,300 functions, 628 KB (`src/clones/`) |
| 4.4 Core engine types | **In progress.** String ids (`CStrId` incl. copy-assignment, `UnkStrId`), the growable array `base::UnkVector` (push, grow-at, insert, remove, clear/free, move) and its allocator interface; reflected-function initialisers now match (below) |
| 4.5+ Boot to title screen | Not started; plan below |
| Port | Not started (`port/` is empty) |

Function status (BotW codes): `O` 77,637, `m` 3, `M` 1, `L` 1,614, `U` 24,762.
**Game-code progress: 42.74% of bytes (8.02 MB) matching** (`tools/progress.py`; library code
excluded; 39.09% at the start of this session). `tools/check.py` counts `src/` definitions,
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
tools/gen_lift.py         lifter: candidates <= 512 B not defined elsewhere in src/; 9 variants; verify
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
.venv/Scripts/python tools/gen_lift.py --version 2.1.0             # ~15 min; run after the other generators
.venv/Scripts/python tools/check.py --version 2.1.0 --update       # ~35 s, parallel
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
1. **Extend the lifter (`tools/gen_lift.py`), largest payoff first.** It lifts 47,431 of 75,661
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
   - Mismatches now dominate (~60,700 lifted, 44,549 match). Only ~1,600 (165 KB) differ in
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
     562 others. Left there (~45, one-offs): an inlined `CFilePathStrId` constructor (8; and
     `fn_7100408c20`, 33 KB, for `CStrId`), `CRntString` copied from a live object, other
     attribute builders (`fn_7100228ca0`), field lookups by name hash, guarded statics.
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
   - Enum static initialisers and the other per-TU static initialisers. These define
     globals, so they need the TU's data definitions (the 160 B ones are mostly lifted already).
   - Slot `0x28`/`0x30`/`0x38`/`0x48`/`0x50` helpers of other classes (need class layouts):
     ~2,400 functions, ~390 KB; a census of `U` helpers by slot and mnemonics (see
     `class_constructors` in `gen_reflection.py`) picks the next families.
   - A shape census (mnemonic sequence per U function, grouped) was the fastest way to pick
     templates this session: the top families were reflected-function initialisers (839 × 380 B)
     and `CClass` constructors (777 × 388 B).
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
