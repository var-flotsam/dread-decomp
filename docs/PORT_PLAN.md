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
| Shaders | 478 `.bshdat` files, precompiled NVIDIA Maxwell shader binaries in Mercury's container | A Maxwell-to-SPIR-V translator. Switch emulators have them; check licenses before using one (`docs/references.md`). Writing one from scratch is months of work on its own |
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
   and shader translation second, audio third.
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

This is aggressive. M1 is realistic with focus. M2 depends on shader translation, which is either
a large dependency or a large project. "Playable in a few months" requires M2 to go well; say so
when planning, and re-estimate after M1.

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
   a pinned generator version, like `build/`).
3. Graphics backend (Vulkan is the natural target for an NVN shim) and the shader translator
   (write, or depend on an emulator's; license check first).
4. `nn::atk`: lift it for the port (keeps "not decompiled") or replace it.
5. Pull-request size and review: with the self-hosted check job (`.github/workflows/check-local.yml`)
   the owner can merge on green instead of reading each diff.
