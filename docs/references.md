# External references

Every outside project this repo uses, copies conventions from, or has evaluated. Last checked
2026-10-15. Licenses are as reported by GitHub or the package metadata at that time. Re-check
before vendoring anything.

## Projects we model ourselves on

| Project | What we take from it | Notes |
|---|---|---|
| [PrimeDecomp/prime](https://github.com/PrimeDecomp/prime) | Overall shape of a matching decomp; per-library compiler versions and flags; Matching / NonMatching / Equivalent object statuses; progress tracking | Its `configure.py` pins different Metrowerks versions per library (Dolphin SDK `GC/1.2.5n`, game `GC/1.3.2`) and per-object inline settings. It decompiles the Dolphin SDK, the C library and MetroTRK in-repo (`extern/sdk`), pulls MusyX as a submodule, and uses upstream zlib 1.1.3 source. Progress is published on [decomp.dev](https://decomp.dev). |
| [zeldaret/botw](https://github.com/zeldaret/botw) | Switch-specific model: upstream Clang (4.0.1) instead of the SDK compiler; a function CSV; **status codes `O`/`m`/`M`/`W`/`U`/`L`**; per-function (not whole-binary) matching; `lib/` layout for Nintendo libraries | Libraries are shared `open-ead` submodules (below). Havok is header stubs only (`lib/hkStubs`). The C library comes from a musl fork. |
| [open-ead/nx-decomp-tools](https://github.com/open-ead/nx-decomp-tools) | Definition of the status codes (`viking/src/functions.rs`, `Status` enum) | BotW's checking and diff tooling ("viking"). Not used directly; our `tools/check.py` and `tools/asmdiff.py` cover the same ground. |
| [AxioDL/metaforce](https://github.com/AxioDL/metaforce) | The decomp-to-native-port path | A Metroid Prime engine reimplementation that its README says "was recently rewritten on top of the Metroid Prime matching decompilation". It uses aurora for the console APIs. |
| [encounter/aurora](https://github.com/encounter/aurora) (MIT) | Model for the port layer | "A source-level GameCube & Wii compatibility layer intended for use with game decompilation projects": GX over D3D12/Vulkan/Metal, PAD over SDL. The Dread equivalent would be an `nn::` + NVN layer. [encounter/borealis](https://github.com/encounter/borealis) (MIT) adds shared modules for aurora-based ports. |

## Shared Switch libraries (evaluated, not vendored)

| Project | Contents | Status for us |
|---|---|---|
| [open-ead/nnsdk](https://github.com/open-ead/nnsdk) | **Used: git submodule at `lib/nnsdk`.** NintendoSDK declaration headers (user-written from reverse engineering and games shipped with symbols), plus decompiled SDK modules some games link statically | The repo declares **no license**, so we reference it as a submodule and never copy it. Its README says use outside matching decomp is unsupported: don't make the port depend on it without asking the maintainers. Successor of nnheaders |
| [open-ead/botw-lib-musl](https://github.com/open-ead/botw-lib-musl) | **Used: git submodule at `lib/musl`.** Prebuilt musl 1.2.2 for AArch64; we use its headers | musl is MIT (`COPYRIGHT`) |
| [switchbrew wiki, HID services](https://switchbrew.org/wiki/HID_services) and [switchbrew/libnx](https://github.com/switchbrew/libnx) `HidNpadButton` (ISC) | Documentation: the Npad button bits (A 0, B 1, X 2, Y 3, stick clicks 4-5, L R ZL ZR 6-9, Plus 10, Minus 11, d-pad 12-15) | Read only, for the names of `DREAD_INPUT` in `port/runtime/src/hid.cpp`; nothing copied, not a dependency. |
| [open-ead/nnheaders](https://github.com/open-ead/nnheaders) | Original community NintendoSDK headers | Frozen mid-migration (no new PRs); superseded by nnsdk/nnware. Not used. |
| [open-ead/nnware](https://github.com/open-ead/nnware) | Decompiled NintendoWare modules: `atk`, `font`, `g3d`, `ui2d`, `vfx` | Has `lib/atk`, but it is very early (12 files, 1 `.cpp`). **The repo declares no license**, so treat it as reference only and don't copy from it. Our `nn::atk` is 11.4.3. |
| [nw4f/Snd](https://github.com/nw4f/Snd) | Decompilation of the Snd library of NintendoWare for Cafe (Wii U), the predecessor of `nn::atk`. Allowed by the owner on 2026-11-11 as a reading reference for names and structure when `nn::atk` is decompiled; not yet used | **The repo declares no license** (checked 2026-11-11): reference only, copy nothing. |
| [open-ead/sead](https://github.com/open-ead/sead), [open-ead/agl](https://github.com/open-ead/agl), [open-ead/EventFlow](https://github.com/open-ead/EventFlow) | Decompiled Nintendo EPD engine libraries | Not used by Dread (Mercury Engine is MercurySteam's), but the pattern of a shared library decomp repo fits `lib/mercury/` if Samus Returns is ever decompiled. |
| [AxioDL/musyx](https://github.com/AxioDL/musyx) (CC0-1.0) | Decompiled MusyX audio middleware, used by PrimeDecomp and Metaforce | Precedent for decompiling audio middleware as its own project (cf. `nn::atk`). |
| [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit) (Apache-2.0) | GameCube/Wii project tooling used by PrimeDecomp | Not applicable to NSO executables. |

## Surveyed on 2026-11-14 (reference only, nothing copied)

| Project | What we take from it | Notes |
|---|---|---|
| [zeldaret/botw `AGENTS.md`](https://github.com/zeldaret/botw/blob/master/AGENTS.md) | Three rules, reworded in `CLAUDE.md`: undo the optimizations in code no developer would write, no `goto` to force a match, no assembly in commit messages or pull request descriptions | Its other rules were weighed and left (journal, 2026-11-14) |
| [MonsterDruide1/OdysseyDecomp](https://github.com/MonsterDruide1/OdysseyDecomp) | The idea of tracking status per object file (`data/file_list.yml`) | Super Mario Odyssey; upstream Clang, BotW's tooling |
| [shibbo/3DWDecomp](https://github.com/shibbo/3DWDecomp) | Nothing; a comparison point for agent-written Switch code | Builds with the NintendoSDK's compiler, which this project never obtains |
| [GRAnimated/MinecraftLCE](https://github.com/GRAnimated/MinecraftLCE) | Nothing; comparison only | Minecraft: Nintendo Switch Edition |
| [tangosdev/sm64ds-decomp](https://github.com/tangosdev/sm64ds-decomp) | Nothing; it counts functions linked into its PC port apart from functions matched, as our lifted-code figure does | Its README carries an instruction aimed at AI readers; ignore it |
| [Chris Lewis's Snowboard Kids posts](https://blog.chrislewis.au/decompiling-a-nintendo-64-game-in-84-days/), [the long tail](https://blog.chrislewis.au/the-long-tail-of-llm-assisted-decompilation/) and [macabeus/mizuchi](https://github.com/macabeus/mizuchi) with [its benchmark](https://gambiconf.substack.com/p/can-llms-really-do-matching-decompilation) | Worker practice: similar matched functions as references, an attempt cap, no permuter inside the worker's loop | N64 and GBA projects, other compilers |
| [gminustwo/msr2ds](https://github.com/gminustwo/msr2ds/wiki) | Nothing | A recompiled PC port of Samus Returns; GPL, so never into this repository |

## Tools and libraries we use

| Project | Version | Used for | License |
|---|---|---|---|
| [randovania/mercury-engine-data-structures](https://github.com/randovania/mercury-engine-data-structures) (MEDS) | 0.36.2 (PyPI) | Dread type names and hierarchy, property names, `files.toc` / `.pkg` parsing, asset-ID hashing | MIT |
| [NationalSecurityAgency/ghidra](https://github.com/NationalSecurityAgency/ghidra) | 12.1.4 (release zip, SHA-256 checked) | Headless auto-analysis and exports; its AArch64 SLEIGH specification gives the raw P-code the semantic-lift oracle executes (`tools/ghidra/pcode.py`, `tools/pcode_emu.py`) | Apache-2.0 |
| PyGhidra | 3.1.0 (wheel bundled with Ghidra, installed offline) | Driving Ghidra from Python | Apache-2.0 |
| [llvm/llvm-project](https://github.com/llvm/llvm-project) | 23.1.3 (winget; `llvm-readobj`, `llvm-objdump`, `llvm-cxxfilt`) | Inspection and demangling | Apache-2.0 with LLVM exception |
| LLVM releases 7.1.0+ ([GitHub releases](https://github.com/llvm/llvm-project/releases)) and 4.0.1, 6.0.1, 7.0.1, 9.0.0 ([releases.llvm.org](https://releases.llvm.org/)): 4.0.1, 6.0.1, 7.0.1, 8.0.1, 9.0.0, 9.0.1, 10.0.0, 11.1.0, 12.0.1 | pinned in `data/toolchains.json` | Compiler candidates; **9.0.1 is the project toolchain** (9.0.0 is identical) | Apache-2.0 with LLVM exception (4.0.1–7.0.1: UIUC) |
| [KhronosGroup/glslang](https://github.com/KhronosGroup/glslang) | 16.6.0 (release archive, `bin/` only, SHA-256 checked against GitHub's published digests; `tools/fetch_toolchains.py glslang-16.6.0`) | Compiling the GLSL sources `tools/bshdat.py` extracts from the `.bshdat` packages to OpenGL and Vulkan SPIR-V (`--compile`) | BSD-3-Clause and other permissive licences (its `LICENSE.txt`) |
| [encounter/objdiff](https://github.com/encounter/objdiff) | 3.8.2 (GUI + CLI, SHA-256 checked against GitHub's published digests) | Per-unit diff GUI and progress reports (`tools/objdiff_gen.py` writes target objects and `objdiff.json`) | Apache-2.0 |
| [Lua 5.1.x sources](https://www.lua.org/ftp/) | 5.1, 5.1.1–5.1.5 (pinned in `data/sources.json`, kept outside the repo) | Identifying the embedded Lua: **5.1.4**, float `lua_Number`, built as C++ (`tools/libcheck_lua.py`) | MIT |
| [libc++ 9.0.1 headers](https://github.com/llvm/llvm-project/releases/tag/llvmorg-9.0.1) (`libcxx-9.0.1.src`) | pinned in `data/sources.json`, kept outside the repo | C++ standard library headers for the build (Nintendo's SDK also ships libc++) | Apache-2.0 with LLVM exception |
| [libc++ `std::__sort`](https://github.com/llvm/llvm-project/blob/llvmorg-9.0.1/libcxx/include/algorithm) (`libcxx/include/algorithm`, `llvmorg-9.0.1`) | adapted copy vendored in `lib/libcxx/` (owner-approved, 2026-10-22; changes in its README) | Body of the game's `std::__sort` instantiation (`fn_710117c430`) | Apache-2.0 with LLVM exception |
| [dlmalloc 2.8.4](https://gee.cs.oswego.edu/pub/misc/malloc-2.8.4.c) (Doug Lea), fetched from the [DenizThatMenace/dlmalloc](https://github.com/DenizThatMenace/dlmalloc) mirror of every release at commit `c48293b6` (the author's site is blocked here) | vendored unmodified in `lib/dlmalloc/` (owner-approved, 2026-10-22), sha256 pinned in `data/sources.json` | The game's sub-allocator heaps: mspaces built without locks or system memory (`lib/dlmalloc/README.md`); named by `tools/sigmatch.py --library dlmalloc` | Public domain (CC0) |
| [webmproject/libvpx](https://github.com/webmproject/libvpx) | 1.14.1 (tag archive pinned in `data/sources.json`, kept outside the repo; `tools/fetch_sources.py libvpx-1.14.1`, built by `tools/build_libvpx.py` as a static VP9-decoder-only library) | The port's movie runtime decodes the game's VP9 WebM videos (title background, cutscenes) with it (`port/runtime/src/movie.cpp`); the WebM container is read by the runtime's own `webm.cpp` | BSD-3-Clause (plus its PATENTS grant) |
| [capstone-engine/capstone](https://github.com/capstone-engine/capstone) | 5.0.9 (PyPI) | AArch64 disassembly in tools; register liveness for `tools/abi_facts.py` | BSD |
| [eliben/pyelftools](https://github.com/eliben/pyelftools) | 0.33 | Reading compiled objects | Public domain |
| [python-lz4/python-lz4](https://github.com/python-lz4/python-lz4) | 4.4.5 | NSO segment decompression | BSD |
| Python 3.12, Temurin JDK 21, CMake 4.4, Ninja 1.13, 7-Zip | winget | Toolchain | various |
| [obra/superpowers](https://github.com/obra/superpowers) | 7.0.1 (commit `2ce8aa15`), two skills adapted, not the plugin | `.claude/skills/systematic-debugging` (root cause before any fix; a section on this repository's runs) and `.claude/skills/writing-skills` (testing a skill on a subagent before relying on it); notice in `.claude/skills/LICENSE.superpowers`. The plugin's session-start hook and its other skills were left out: the hook loads into every session, and the planning, TDD, subagent and worktree skills duplicate this repository's own process | MIT |
| Eden (yuzu fork) | 0.2.1, supplied by the user | Dumping ExeFS/RomFS; boot log confirmed the 2.1.0 build ID | GPL-3.0 (not distributed here) |

## Other Dread projects (evaluated, reference only)

| Project | What we take from it | License |
|---|---|---|
| [yozander2009-stack/Metroid-Dread-Recomp](https://github.com/yozander2009-stack/Metroid-Dread-Recomp) | A static recompilation of the same 2.1.0 `main` on SwitchRecomp, playable on Windows and Linux. Read on 2026-10-28 (head `c5dfcf0`) for facts about the game on PC, mapped to our addresses (ours = theirs - 0x8005000 + 0x7100000000; journal, "What the Dread recomp found"); `tools/recomp_trace.py` turns a local build of it into a boot trace. No code taken | SwitchRecomp parts Apache-2.0 with a NOTICE that asks for visible credit; its Vulkan renderer (from [scarletnx/ScarletNX](https://github.com/scarletnx/ScarletNX)) GPL-3.0, so never into this repository; the front end's files carry no licence header |
| SwitchRecomp (MZ Labs) | The translator and runtime under the recomp above; not used directly | Apache-2.0 |
| [theboy181/switch-ptchtxt-mods](https://github.com/theboy181/switch-ptchtxt-mods) | Cited by the recomp for the FXAA and 60 FPS cutscene addresses; not used | not checked |

## Model-assisted decompilation (read for method, no code taken)

Read on 2026-11-13 and 2026-11-14 for how other projects run a model on matching work. Two of their lessons are
in `tools/batch/lean/` since 2026-11-14: reference matches in a worker's context (`similar.py`) and a capped
attempt budget (`try.py`).

| Project | What we take from it | License |
|---|---|---|
| Snowboard Kids 2 decompilation, as written up by Chris Lewis: [Using Coding Agents to Decompile Nintendo 64 Games](https://blog.chrislewis.au/using-coding-agents-to-decompile-nintendo-64-games/), [The Unexpected Effectiveness of One-Shot Decompilation with Claude](https://blog.chrislewis.au/the-unexpected-effectiveness-of-one-shot-decompilation-with-claude/), [The Long Tail of LLM-Assisted Decompilation](https://blog.chrislewis.au/the-long-tail-of-llm-assisted-decompilation/) | The practice of giving the model similar functions that already match, with their source, before it writes | blog posts; no code read |
| Mizuchi, a pipeline and benchmark for matching decompilation with models: [Can LLMs Really Do Matching Decompilation? I Tested 60 Functions to Find Out](https://gambiconf.substack.com/p/can-llms-really-do-matching-decompilation) | Its figures on attempts: about half the matches on the first attempt, and odds of about a quarter after the third. The reason the attempt budget is a cap of 3 | article; no code read |
| Super Smash Bros. Melee decompilation's search for similar functions ("decomp-search") | The same practice as the first row, as a tool of that project; known from the survey only, its repository was not read | not checked |

## Planned (approved, not yet vendored into the repo)

| Project | Why | License |
|---|---|---|
| [Lua 5.1.4](https://www.lua.org/ftp/lua-5.1.4.tar.gz) | Identified: 81 of 82 standard-library functions match upstream 5.1.4 built with the game flags. Vendor it (with the `float` `luaconf.h` change) and mark its functions `L` | MIT |
| [ocornut/imgui](https://github.com/ocornut/imgui) | Only about 1.2 KB of Dear ImGui survives linking (`0x7100001fb8`). Version not identified; low priority | MIT |

## Tried and unavailable

- **LibHac / hactoolnet** (`Thealexbarney/LibHac`): the GitHub repo returned 404 on 2026-10-15. It wasn't needed, because Eden's dump was used directly.
- **No-Intro DAT-o-MATIC** (datomatic.no-intro.org): its search doesn't return results to scripts, so the XCI/NSP hashes couldn't be checked externally. The dump was verified internally instead (`tools/verify_dump.py`).
