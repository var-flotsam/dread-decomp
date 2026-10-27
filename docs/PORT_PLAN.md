# Port plan: from matching source to a running PC build

Written 2026-10-24 for the owner's goal: a clean, readable project that is **playable on PC within
a few months**. This page says what a running build needs, which of two routes can get there in
that time, what each milestone is gated on, and which decisions are the owner's. Current numbers
are in `docs/HANDOFF.md`; the day-to-day rules are in `CLAUDE.md`.

## Where the matching work stands against this goal

| Measure (2026-10-24) | Value |
|---|---|
| Game bytes matching (`O`), whole binary | 53.8% of 18.8 MB |
| Of that, generated (reflection boilerplate, lifted small functions, clones, tables) | 9.4 MB |
| Of that, hand-written with real types (`src/game_*`, `src/base`) | 0.76 MB |
| Boot path, 4,394 functions statically reachable from `nnMain`: game bytes matched | 27% of 1.66 MB |
| `U` bytes between 256 B and 2 KB (needs real types; generators are exhausted) | 5.6 MB |

Two facts follow. First, the headline percentage does not measure distance to a running build:
most of it is boilerplate and `char*` register transliteration that a port cannot call by name.
Second, at the observed hand-matching rate (about 0.76 MB of typed code so far, with many parallel
sessions), typing the 8.4 MB that is left, plus retyping 3.1 MB of lifted code, is a multi-year
effort. **A matching-only route cannot reach "playable" in months.**

## What a running build needs

The game is `main` (19.1 MB of code) plus the Switch SDK. Everything the SDK does has to be
provided on PC. From the import table of `main` and the RomFS:

| Surface | Evidence | PC replacement |
|---|---|---|
| Graphics: NVN | Loaded through `nvnBootstrapLoader` + `nvnDeviceGetProcAddress` (a function-pointer table, 600+ `nvn*` names in strings) | An NVN shim over Vulkan (NVN's object model is the closest to Vulkan's). This is the largest single piece |
| Shaders | 478 `.bshdat` packages: 671 programs, each a vertex and a fragment stage compiled by NVN's compiler, whose debug section carries the stage's **GLSL source** and includes (`#version 440 core`/`450`). `tools/bshdat.py` (2026-10-26) parses the format, extracts the 1,342 sources to `build/shaders/` and decodes each stage's reflection (blocks, samplers, inputs, outputs and their bindings) | glslang 16.6.0 compiles 1,341 of 1,342 stages (670 programs) to OpenGL and Vulkan SPIR-V after two rewrites, applied when the port's build compiles them (decision 3: Vulkan, build time): uniform blocks declared `layout(location = N)` get the binding NVN's compiler assigned (the stage's active blocks in declaration order, from the reflection), and `GL_NV_explicit_typecast` is enabled for C-style casts. The last stage (FXAA, a non-constant texel offset) needed a fixup, now applied by the build. No 64-bit handles in any source (SPIR-V needs only `Shader`): bindless is the shim's API side. Since 2026-10-26 the port's `shaders` target compiles all 1,342 stages and checks each against NVN's reflection, and `port/nvn/` maps NVN's per-stage block and texture slots to one Vulkan set per stage, samplers from binding 16 (`port/nvn/README.md`). Variant selection is the engine's own code on the packages (`fn_71001b0f24`), and so are the uniform name tables' engine IDs (`fn_71001b0050`, `fn_71001b0f7c`): nothing for the shim, lifts for M1 |
| Audio renderer | 149 `nn::audio` imports (the low-level renderer that `nn::atk` drives); `nn::atk` 11.4.3 is statically linked (about 1,150 functions, status `L`, not decompiled by decision) | Either implement `nn::audio` over a PC mixer and keep `nn::atk` as **lifted** code, or replace `nn::atk` with an open player for `bfsar`/`bfstm`/`bfgrp` (1,276 streams, 73 groups, one archive). Lifting `nn::atk` is the shorter path and keeps the decision not to hand-decompile it |
| OS, threads, time, memory | 55 `nn::os`, 6 `nn::mem`, 5 `nn::time`; the game reserves a 1.82 GB `.bss` and sub-allocates with dlmalloc mspaces (`lib/dlmalloc`) | `std::thread`, mutexes and timers; a reserved virtual range for the `.bss` |
| File system | 22 `nn::fs`; RomFS with `files.toc` and 72 `.pkg` packs (parsing already in MEDS) | Plain files from the user's own dump |
| Input | 34 `nn::hid` | SDL game controller and keyboard |
| Video | WebM through `nn::movie` (`subsdk0`), 101 files | libwebm/libvpx, or skip cutscenes at first |
| Console services | `nn::nfp` (amiibo, 26), `nn::prepo` (6), `nn::account` (4), `nn::oe` (13), `nn::vi` (4), `nn::diag` (2) | Stubs; `nn::vi` becomes a window |
| C and C++ runtime | 152 libc imports, 37 libc++ | Host libc and libc++ (the game's own code is `-fno-exceptions -fno-rtti`) |
| Game data (`.data`/`.rodata`: vtables, constant tables, string literals) | Game data, never in git | Extracted at build time from the user's dump into `build/`, as PrimeDecomp-based ports require the original disc. Vtables and relocations are metadata and can be tracked as a map; the bytes are not |

## Two routes

### Route A: finish matching, then port

Every function the game runs is hand-typed and matched, then the port layer is built against that
source. This is the cleanest end state and the slowest path. It is what the project has been doing.
Estimated time to a title screen: more than a year at the current rate, even with the hub-first
work order. **Not compatible with the timeline.**

### Route B (recommended): one program now, matching replaces lifting over time

Build the whole of `main` as one PC program immediately:

1. **Every function has a source.** Hand-matched, typed code where it exists (`src/game_*`,
   `src/base`, generated reflection, clones, tables). For everything else, a **lifted** translation
   of its instructions, as `tools/gen_lift.py` already produces for 47,000 small functions, but
   accepted on a *semantic* bar instead of the byte-identical one: it must compile and reproduce
   the original's behaviour, not its bytes. The byte-identical lifts are kept as they are (they are
   also matches). The semantic lifts live outside `src/` (for example `port/lifted/`, generated,
   gitignored or tracked by decision), never count toward matching progress, and are replaced,
   function by function, as hand matching lands.
2. **The lifter grows for coverage, not for matches.** Its known gaps (`ccmp`, jump tables, loops
   and block layout, large frames, `ldarb` guards) are exactly what blocks the remaining 17,500
   functions. Under the semantic bar, "lifts and compiles" is enough, so each gap closed covers
   thousands of functions at once. This is the one place where generator work still pays, and it
   is the heart of this route.
3. **A correctness oracle for semantic lifts.** Byte identity is no longer the check, so the lifter
   needs another one: (a) differential execution of a lifted function against the original in the
   emulator on recorded inputs (Eden has a GDB stub; traces stay in `build/`), (b) cross-checking
   the lift's data-flow against Ghidra's P-code *as a test oracle only* (read in `build/`, never
   committed; the rules already allow reading decompiler output), and (c) the game itself: a boot
   that reaches the title screen exercises thousands of them. Start with (c) and (b); add (a) for
   functions that misbehave.
4. **The port layer** (`port/`) provides the surface in the table above, in the order the boot path
   needs it: `nn::os`/`nn::fs`/`nn::hid`/stubs first (the title screen needs them), the NVN shim
   and the shader pipeline (`.bshdat` parser, glslang) second, audio third.
5. **Canonical headers make the hand-written code linkable.** Every shared callee and type is
   declared once under `include/` (enforced by `tools/decl_check.py`). Lifted code calls hand code
   and the reverse through the same `fn_<addr>` names, so the two mix freely.

What this buys: a build that *runs* months earlier, with readable code appearing where it matters
(hubs, boot, gameplay) while the long tail runs as lifted code. What it costs: the lifter and the
oracle are tool work (weeks), lifted code is ugly and must be clearly fenced off from the matched
tree, and a semantic lift can be wrong in ways a byte match cannot. The percentage of hand-typed
code becomes the honest readability metric; the bytes-matched number keeps measuring verification.

**Decision (owner, 2026-10-24): Route B.** The port lives in `port/` on `main`, not on a
long-lived branch: it must track every header and matched function as they land, and a side
branch would diverge within days. Port work goes through short-lived branches and pull requests
like everything else, and a port change never edits `src/` matching code in the same PR.

## Milestones (Route B), each with a gate

| # | Milestone | Gate (what must be true) | Realistic span |
|---|---|---|---|
| M0 | Tooling and rules in place | `tools/progress.py --slice boot`, `tools/hubs.py`, `tools/decl_check.py`, relocation check in `check.py`, `data/tu_map.csv`; handoff split; `port/` skeleton builds an empty executable on PC; `src/` compiles for x86-64 (compile only) and the list of non-portable constructs is known | 1 to 2 weeks |
| M1 | Whole-program link of the boot slice | Every function reachable from `nnMain` has a source (hand or lifted); the program links on PC with `nn::` stubs; unreferenced functions trap with their address; the lifter's semantic mode exists with oracle (b) and (c) | by the end of month 1 (lifter work is the risk) |
| M2 | Title screen on PC | Boot runs through `nnMain`, loads the RomFS, draws the GUI: the NVN shim, shader translation for the GUI shaders, input, a window | month 2 to 3 (graphics is the biggest unknown) |
| M3 | A level loads and plays | Gameplay slice (player, world, characters, physics) runs lifted or matched; collision and animation are correct | month 3 to 4 |
| M4 | Playable | Audio, saves, cutscenes or a skip; stability over a full session | month 4 and later |

**M0 status (2026-10-24).** The port skeleton is in: `port/CMakeLists.txt` (`dread_game`,
`dread_runtime`, `dread`, `sweep`), `port/runtime/` with the `nn::` headers the three
SDK-including sources need (`nn::os` tick and mutex, `nn::oe`, `nn::vi`) and empty skeletons for
the other namespaces, and `tools/port_sweep.py`, the compile-only sweep (`port/README.md`). With
Clang 23 for `x86_64-linux-gnu` and the libc++ 9.0.1 and musl headers, the sweep compiles 3,799 of
the 3,808 `src/` files without the runtime and all 3,808 with it. The only category was
`sdk header not found` (`<nn/os.h>` through `include/base/UnkDefaultAllocator.h` in 7 files,
`<nn/os/os_TickTypes.h>` and `<nn/oe.h>`/`<nn/vi.h>` in one each); nothing in `src/lifted`,
`src/clones` or `src/reflection` failed, and no inline assembly or AArch64-only builtin turned up.
The build configures on the Windows PC and `dread_runtime` and `dread_game` compile there, but it
cannot link a Windows executable yet: the PC has LLVM 23 without a Windows SDK or MSVC (the MSVC
Build Tools, MinGW-w64 or WSL are the options), and a Windows target needs `include/types.h` to
take `u64`/`s64` from `long long` under `_WIN64` (LLP64), which a separate change does (`Phase
4.5: types.h: 64-bit Windows hosts get u64/s64 from long long`). The sweep also reads the objects
back with `llvm-nm`: four frozen Phase 3 samples define a function another source defines
(`fn_7100227448`, also in `src/reflection/CActor.cpp`, and three `src/lifted` pairs) and are the
only files `dread_game` leaves out; the other 32 samples (22 `O`) are compiled, and no other
function is defined twice. What the sweep cannot see is M1's list: callees without a source, and
data symbols and vtables. The other M0 items (the slice, hub and declaration tools,
`data/tu_map.csv`) are tracked in `docs/HANDOFF.md`.

**M1 status (2026-10-24).** Step 1, the whole-program link report, is in: `tools/port_link.py`
(`port/README.md`, "The link (M1)") links the sweep's 3,804 objects with `ld.lld -r` (no duplicate
symbol; 60,402 undefined), classifies every undefined symbol, generates the aliases and traps that
close the link, and links again. Headline numbers: the boot slice has 4,211 game functions, of
which 1,007 are defined, 919 defined but reached only through an alias, 3 are the host's
(`operator new`/`delete`/`delete[]`, status `L`, reached by address), and **2,282 have no
definition at all, 1,286,980 bytes**: 2,153 `U` (1,237,420 bytes), 128 `L` (49,176 bytes), one
`O` (an `asm` alias to a libc++ internal the x86-64 compile does not emit). That is the lifter's
coverage work, listed by callers in `build/port/link/boot_missing.csv`. Aliases: 37,531 for 35,176
addresses (hand declarations with other parameter lists, `gen_lift`'s `callee::` convention, tagged
template instantiations); 27,188 only take the address, and of the 10,343 calls 9,139 are safe by
x86-64 argument class, 1,185 pass fewer arguments than the definition reads (check), 13 land on an
unmangled definition and 6 are unsafe (a `float` against a `bool`): those six are trapped at the
call rather than aliased. Data: 15,709 references to 15,450 `g_unk_` addresses, all defined as
absolute addresses until the data loader maps the dump; 5,467 of the symbols are referenced
PC-relatively (20,178 relocations, the `hidden` declarations), which ties the final image to within
2 GB of `0x7100000000`.
Imports the slice reaches, by namespace: C library 90, `nn::os` 30 (the runtime defines 11),
`movie` 24, `nn::oe` 20 (4), `nn::hid` 12, `nn::prepo` 12, `nn::account` 8, `nn::fs` 8, `nv` 6,
`nn::mem` 4, `nn::time` 4, `nn::util` 4, `nn::diag` 2. Vtables undefined: 71. Names that resolve to
no address: 182. The second link's residue is 304 symbols (those vtables and names, 49 standard
library names, 2 `nn::hid` destructors). Next for M1: the lifter's coverage mode over
`boot_missing.csv` (the 2,153 `U` functions of the slice first, by callers), the runtime stubs in
call order along the boot path (`nn::os` and `nn::oe` first, then `nn::fs`, `nn::hid`, `movie`,
`nv`), and the data loader that maps the dump's segments at their original addresses and replaces
the absolute symbols.

**M1 step 2 (2026-10-25).** The vendored libraries are in the link and the build (`dread_libs`:
`lib/lua` and `lib/dlmalloc` compiled as C++ with the matching build's flags), and `fn_7100000250`
(`__cxa_atexit`, per `include/base/UnkAtExit.h`) jumps to the host's. On `main` after hub batch 2 the
boot slice's functions without a definition drop from 2,276 to **2,157 (1,238,544 bytes)**: 2,148
`U`, 8 `L` (127 before), one `O`. The residue grows from 304 to 375 by Lua's C library calls only.
Next: the lifter's coverage mode over `boot_missing.csv`, the runtime stubs and the data loader, as
above.

**M1 step 3 (2026-10-25): the lifter's semantic coverage mode.** Design, as built:

- *Approach.* `tools/port_lift.py` runs `gen_lift`'s lifters with `opts={"semantic"}` over the `U`
  rows of `boot_missing.csv`. Semantic mode changes only what byte identity used to vouch for: a
  direct call names every argument register its callee reads (`tools/abi_facts.py`, liveness over
  the callee's instructions), a callee that returns through `x8` gets its result placed by C++17
  copy elision, string literals stay the game's addresses, `ldarb` guards and exclusive
  load/store pairs are lifted as atomics instead of `lift_guard` helpers (whose constructor has no
  address), every condition follows the flags exactly (carry and overflow after `adds`/`cmn`;
  since the follow-up step), division and shifts follow AArch64, and flat mode keeps the values a
  pending compare or a writeback reads, keeps NZCV in locals for a condition in another block,
  turns Clang's compressed jump tables into a `switch`, never narrows a 64-bit read to the type of
  the last write in address order, treats an argument register unwritten in address order as the
  incoming argument, and lifts spills of callee-saved registers into the locals (only the
  prologue's slots are frame bookkeeping).
  Byte mode is untouched: on 4,000 sampled candidates every attempt is identical, text for text.
- *Oracle (b).* `tools/pcode_emu.py` runs the original instructions and each compiled variant
  (AArch64, the project compiler, `-fno-strict-aliasing -fwrapv -fno-delete-null-pointer-checks`)
  as raw P-code from Ghidra's AArch64 specification (`tools/ghidra/pcode.py`, an in-memory parse,
  no project lock), so the lifter never decides what an instruction means. Inputs are seeded:
  registers and memory words drawn from five profiles (zero-, pointer- and small-integer-heavy,
  and the function's own constants), more seeds while they reach new instructions. Callees are not
  run: each call is recorded with the argument registers the callee reads (only the low 32 bits of
  one it reads as `w<n>`), returns a value derived from its position, clobbers the caller-saved
  registers and writes a pattern through stack pointer arguments. A variant is accepted when its
  calls, its writes outside its own frame and its result agree with the original's on every
  conclusive input. `--mutants N` measures the oracle itself by injecting one-point faults into
  accepted lifts.
- *Oracle (c).* Not runnable on this PC (no executable). Every generated x86-64 source opens its
  function with `DREAD_PORT_LIFTED(<address>)`, empty by default, so a boot can log which lifts
  ran once one exists.
- *Layout (open decision 2, default kept).* `build/port/lifted/`: `src/fn_<addr>.cpp`,
  `obj/fn_<addr>.o` (x86-64, `tools/port_sweep.py`'s compiler and flags) and `manifest.json` (the
  generator version, the blob hashes of its five sources, every function's outcome, inputs and
  coverage). Nothing enters `src/`; `tools/check.py` and `tools/progress.py` never see a semantic
  lift. `tools/port_link.py --lifted build/port/lifted` links them (leaving out any address the
  sources now define) and reports the boot slice with and without them.
- *Result.* On `main` after hub batch 2: of the slice's 2,148 `U` functions, 1,966 lift, 1,957
  compile and **1,777 pass the oracle (839,396 bytes)**; 157 are rejected by it, 23 are
  inconclusive, the rest are lifter gaps (`adds`/`ands` carry conditions 42, jump tables 37, stack
  arguments 21, ...). With them the slice's functions without a source drop from 2,157 to **380
  (399,148 bytes)** (`tools/port_link.py --lifted`); the residue grows by three C library names.
  The run takes about 40 minutes on the PC (8 oracle processes).
- *Follow-up (2026-10-25, the gaps step).* Exact flags (carry and overflow after `adds`/`cmn`, NZCV
  kept in flat-mode locals across blocks), compressed jump tables as a `switch`, float arguments
  passed on as `f64`, argument registers unwritten in address order taken as the incoming
  arguments, and four oracle fixes (Ghidra's vector registers are 32 bytes apart, not 16; a float
  left in `v0` by a call is not an argument; an input where only the original faults compares
  what came before the fault; C++ runtime imports have signatures): **1,941 accepted (1,042,620
  bytes)**, 104 rejected by the oracle, 24 inconclusive, 79 lifter gaps (stack arguments 23,
  flat-mode register order 14, `x8` results in tail position 14, other jump-table shapes 9, ...).
  The slice's functions without a source drop from 380 to **216 (195,924 bytes)**; the residue
  is 380 (two more host C library names).
- *Limits.* Seeded inputs reach a median of 67% of an original's instructions (476 of the 1,941
  accepted lifts are covered completely, 684 below half). The mutation test (300 one-point faults)
  is rejected 39% of the time overall and 76% on completely covered functions, so a lift means
  "agrees wherever the inputs went", not proved; the manifest keeps each lift's coverage. Floating-point results compare bit for bit on
  AArch64; x86-64 without FMA rounds fused multiply-adds differently, and out-of-range float to
  integer conversions differ. A function pointer argument whose callee is unknown is compared by
  `x0` and the registers set for it; a float only passed on goes as `f64`, which keeps its bits.

**M1 step 4 (2026-10-26): runtime stubs, the data loader, no strict aliasing.** Every `nn::os` and
`nn::oe` import the slice reaches is in `port/runtime/` (threads on host threads, events and
message queues over host objects, the docked operation mode, the exit request as the one
notification), so `tools/port_link.py`'s residue lists none; the next namespaces in call order are
`nn::fs` (the start-up `fn_71001325d0` mounts the RomFS next), `nn::account`, `nn::time`, `nv`,
then `nn::hid` and `movie`. The data loader (`port/runtime/include/dread_port_loader.h`, usage in
`port/README.md`) maps the dump's main NSO at `0x7100000000` before the game's static
initialisers, applies its 146,684 relocations and translates the 128,576 code pointers in `.data`
(vtables, callbacks) to the port's functions through a table `port_link.py` generates from the
link's definitions, with a trap thunk for an address the port does not define;
`tools/test_port_loader.py` runs it on the dump without an SDK. What it fixes for the final link: a
fixed base within 2 GB of the PC-relative data (about 214 MiB free below `0x7100000000`, 235 MiB
above the `.bss`), and the Itanium C++ ABI (the game's vtables are used as they are), so on Windows
MinGW-w64 or WSL, not MSVC. `dread_game` and the sweep compile with `-fno-strict-aliasing`.

This is aggressive. M1 is realistic with focus. M2 was budgeted around shader translation as a
large dependency or a large project; since the `.bshdat` files turned out to carry their GLSL
sources (2026-10-25), that risk is gone and M2's unknown is the NVN shim itself (the object
model and the function-pointer table; the binding semantics are known since 2026-10-26, when
`tools/bshdat.py` compiled 1,341 of the 1,342 stages to SPIR-V). "Playable in a few
months" still requires M2 to go well; say so when planning, and re-estimate after M1.

**M2 status (2026-10-27).** Graphics groundwork, pieces 1 to 3 (var-flotsam/dread-decomp#202,
#204; design in `port/nvn/README.md`):

| Step | State |
|---|---|
| Shaders to SPIR-V at build time (decision 3) | **Done.** The `shaders` target runs `tools/bshdat.py --install` over the dump: 1,342 of 1,342 stages (671 programs) compile, with the FXAA fixup; each module's bindings, block sizes and capabilities are checked against NVN's reflection; `shaders.manifest` (format 3) beside the executable, 31 MB of SPIR-V under `build/` |
| Binding model | **Done, compile-checked.** `port/nvn/` (`dread_nvn`): uniform-buffer slot `b` of a stage is binding `b` of the stage's set (vertex 0, fragment 1), texture slot `t` is binding `16 + t0` element `t - t0`, a texture handle is a texture pool index and a sampler pool index (combined image-sampler); placeholders for what NVN found unused |
| Recognising a stage | **Done.** `nvnProgramSetShaders` gets `{code address, control pointer}` per stage (`fn_710015d760`); the manifest keys stages by FNV-1a 64 over the first 2,048 control bytes, which separates all 1,010 distinct control sections |
| Variant selection | **Understood; no shim work.** The engine's `CProgramResource` loader (`fn_71001b0050`) and selector (`fn_71001b0f24`) choose and build programs from the original `.bshdat` files; they are `U` and go on the lifter's graphics worklist |
| Entry layer, device, memory, queue, command buffers, state objects, window | **Done for the start-up path** (2026-10-26). 106 functions, 74 of the 146 the game calls (`data/nvn_procs.csv`, `tools/nvn_procs.py`); the rest trap by name. `nvn_startup_check` replays the matched start-up `fn_71001587f0` and three frames against them on lavapipe: all checks pass, no validation message; it stops at the 11 texture functions of the render targets |
| Textures, texture and sampler pools, samplers | **Done for the start-up and the asset formats** (2026-10-26): 143 functions, 108 of the 146 the game calls; the start-up replay reaches no unimplemented function. Formats from the engine's own converter and reflected `EComponents` enum (sRGB included; mercury-engine-data-structures agrees for linear files) and by use (depth, a fast-cleared colour target); targets 2D and cube from the game's own textures; seven render-target formats still unknown (they fail by name). Asset texel upload (untiling) and the sampler enumerations are open |
| Programs, vertex state, pipelines, draws, clears, render targets, present | **Done but for the alpha test and a surface** (2026-10-27): all 146 functions the game calls (182 in all). Numbers from the engine's reflected enumerations and the wrappers that convert them (`include/nvn_shim/draw.h`); depth range and window origin from the game's own shaders. `nvn_startup_check` draws all 671 programs, full and depth-only, on lavapipe: 1,342 draws, no validation message. Open: the alpha test (a discard added by the build step), a Vulkan surface for the window |
| Against Vulkan | **Done for the binding layer** (owner's go-ahead for a Vulkan loader in the Linux image, 2026-10-26). `nvn_vulkan_check` on lavapipe with the validation layers: all 671 programs' modules, set layouts, descriptor sets (written by the binding layer) and full and vertex-only pipelines, 0 validation errors or warnings. It found two interface rules the shim must keep: drop fragment inputs NVN found inactive and the vertex stage does not write (now in the build step, 32 inputs), and enable `maintenance4` (2 programs) |

## What this changes for the streams

- **Matching work order:** hubs first (`tools/hubs.py`, boot slice first), then "ready" functions
  whose callees are all matched, because each hub matched with a canonical header unblocks dozens
  of typed callers and replaces that many lifted ones. Choose by what the running build calls.
- **Generators:** `gen_reflection`, `gen_clones`, `gen_statics`, `gen_tables` go into maintenance
  (regenerate when needed). `gen_lift` becomes the coverage tool of Route B and gets the only new
  generator work.
- **New stream, port layer:** `port/` with its own CMake build for x86-64, the `nn::` runtime, the
  NVN shim, audio. It starts now; it does not wait for matching.
- **Readability:** new files go under `src/<subsystem>/` named by the owning class or translation
  unit (`tools/tu_map.py --function ADDR`), shared declarations in `include/`, no new address-named
  directories. Existing `src/game_s7/`-style directories are moved when a unit is complete.

## Open decisions

1. ~~Route B or Route A.~~ Route B, decided 2026-10-24 (above).
2. Where semantic lifts live and whether they are tracked (they are our tool's output, no game
   bytes, but large; the 1 MB file limit and reviewability argue for generated-and-gitignored with
   a pinned generator version, like `build/`). Since M1 step 3 they are generated into
   `build/port/lifted/` with the generator's version and source hashes in its manifest; tracking
   them (for example as `port/lifted/`) stays the owner's call.
3. ~~Graphics backend and when shaders are compiled.~~ **Decided 2026-10-26 by the owner: Vulkan
   first, SPIR-V compiled at build time.** The port's build runs `tools/bshdat.py --install`
   (the `shaders` target, since #202) over the user's own dump (extraction, the two binding
   rewrites, the FXAA fixup, the checks against NVN's reflection) and installs the SPIR-V and
   `shaders.manifest` under `build/` beside the executable: derived from game data, so it is never
   committed or distributed, like everything else the tools generate. The executable carries no
   shader compiler; a load-time compile with a cache is not planned. OpenGL is not a target (1,298
   stages enable the NVIDIA-only `GL_NV_gpu_shader5`, and macOS stops at GL 4.1).
   ~~The shader translator
   (write, or depend on an emulator's; license check first).~~ Settled 2026-10-25: the shipped
   `.bshdat` files contain the GLSL sources, so the shaders are compiled with glslang and nothing
   is taken from an emulator (the surface table above). The `.bshdat` layout is decoded
   (`tools/bshdat.py`, 2026-10-26), and the sources use no bindless handles: each stage binds at
   most five uniform blocks and samplers at fixed slots (declared 0 to 12, arrays reaching 13), so
   NVN's texture handles become combined image-sampler descriptors, one descriptor set per stage.
4. `nn::atk`: lift it for the port (keeps "not decompiled") or replace it.
5. Pull-request size and review: with the self-hosted check job (`.github/workflows/check-local.yml`)
   the owner can merge on green instead of reading each diff.
