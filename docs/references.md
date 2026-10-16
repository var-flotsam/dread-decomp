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
| [open-ead/nnheaders](https://github.com/open-ead/nnheaders) | User-written NintendoSDK headers (from reverse engineering and games shipped with symbols) | Candidate for our `nn::` declarations. **Mid-migration** to nnsdk/nnware per its README; no PRs during migration. |
| [open-ead/nnsdk](https://github.com/open-ead/nnsdk) | NintendoSDK headers, plus decompiled SDK modules that some games link statically | Says "usage … outside of matching decompilation is not supported". Fine for the decomp; check before using in the port. |
| [open-ead/nnware](https://github.com/open-ead/nnware) | Decompiled NintendoWare modules: `atk`, `font`, `g3d`, `ui2d`, `vfx` | Has `lib/atk`, but it is very early (12 files, 1 `.cpp`). **The repo declares no license**, so treat it as reference only and don't copy from it. Our `nn::atk` is 11.4.3. |
| [open-ead/sead](https://github.com/open-ead/sead), [open-ead/agl](https://github.com/open-ead/agl), [open-ead/EventFlow](https://github.com/open-ead/EventFlow) | Decompiled Nintendo EPD engine libraries | Not used by Dread (Mercury Engine is MercurySteam's), but the pattern of a shared library decomp repo fits `lib/mercury/` if Samus Returns is ever decompiled. |
| [open-ead/botw-lib-musl](https://github.com/open-ead/botw-lib-musl) | musl libc fork used as BotW's C library headers | Possible source of libc headers for building `src/`. |
| [AxioDL/musyx](https://github.com/AxioDL/musyx) (CC0-1.0) | Decompiled MusyX audio middleware, used by PrimeDecomp and Metaforce | Precedent for decompiling audio middleware as its own project (cf. `nn::atk`). |
| [encounter/decomp-toolkit](https://github.com/encounter/decomp-toolkit) (Apache-2.0) | GameCube/Wii project tooling used by PrimeDecomp | Not applicable to NSO executables. |

## Tools and libraries we use

| Project | Version | Used for | License |
|---|---|---|---|
| [randovania/mercury-engine-data-structures](https://github.com/randovania/mercury-engine-data-structures) (MEDS) | 0.36.2 (PyPI) | Dread type names and hierarchy, property names, `files.toc` / `.pkg` parsing, asset-ID hashing | MIT |
| [NationalSecurityAgency/ghidra](https://github.com/NationalSecurityAgency/ghidra) | 12.1.4 (release zip, SHA-256 checked) | Headless auto-analysis and exports | Apache-2.0 |
| PyGhidra | 3.1.0 (wheel bundled with Ghidra, installed offline) | Driving Ghidra from Python | Apache-2.0 |
| [llvm/llvm-project](https://github.com/llvm/llvm-project) | 23.1.3 (winget; `llvm-readobj`, `llvm-objdump`, `llvm-cxxfilt`) | Inspection and demangling | Apache-2.0 with LLVM exception |
| LLVM releases 7.1.0+ ([GitHub releases](https://github.com/llvm/llvm-project/releases)) and 4.0.1, 6.0.1, 7.0.1, 9.0.0 ([releases.llvm.org](https://releases.llvm.org/)): 4.0.1, 6.0.1, 7.0.1, 8.0.1, 9.0.0, 9.0.1, 10.0.0, 11.1.0, 12.0.1 | pinned in `data/toolchains.json` | Compiler candidates; **9.0.1 is the project toolchain** (9.0.0 is identical) | Apache-2.0 with LLVM exception (4.0.1–7.0.1: UIUC) |
| [encounter/objdiff](https://github.com/encounter/objdiff) | 3.8.2 (GUI + CLI, SHA-256 checked against GitHub's published digests) | Per-unit diff GUI and progress reports (`tools/objdiff_gen.py` writes target objects and `objdiff.json`) | Apache-2.0 |
| [Lua 5.1.x sources](https://www.lua.org/ftp/) | 5.1, 5.1.1–5.1.5 (pinned in `data/sources.json`, kept outside the repo) | Identifying the embedded Lua: **5.1.4**, float `lua_Number`, built as C++ (`tools/libcheck_lua.py`) | MIT |
| [capstone-engine/capstone](https://github.com/capstone-engine/capstone) | 5.0.9 (PyPI) | AArch64 disassembly in tools | BSD |
| [eliben/pyelftools](https://github.com/eliben/pyelftools) | 0.33 | Reading compiled objects | Public domain |
| [python-lz4/python-lz4](https://github.com/python-lz4/python-lz4) | 4.4.5 | NSO segment decompression | BSD |
| Python 3.12, Temurin JDK 21, CMake 4.4, Ninja 1.13, 7-Zip | winget | Toolchain | various |
| Eden (yuzu fork) | 0.2.1, supplied by the user | Dumping ExeFS/RomFS; boot log confirmed the 2.1.0 build ID | GPL-3.0 (not distributed here) |

## Planned (approved, not yet vendored into the repo)

| Project | Why | License |
|---|---|---|
| [Lua 5.1.4](https://www.lua.org/ftp/lua-5.1.4.tar.gz) | Identified: 81 of 82 standard-library functions match upstream 5.1.4 built with the game flags. Vendor it (with the `float` `luaconf.h` change) and mark its functions `L` | MIT |
| [ocornut/imgui](https://github.com/ocornut/imgui) | Only about 1.2 KB of Dear ImGui survives linking (`0x7100001fb8`). Version not identified; low priority | MIT |

## Tried and unavailable

- **LibHac / hactoolnet** (`Thealexbarney/LibHac`): the GitHub repo returned 404 on 2026-10-15. It wasn't needed, because Eden's dump was used directly.
- **No-Intro DAT-o-MATIC** (datomatic.no-intro.org): its search doesn't return results to scripts, so the XCI/NSP hashes couldn't be checked externally. The dump was verified internally instead (`tools/verify_dump.py`).
