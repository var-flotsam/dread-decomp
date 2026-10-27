# port/

The PC port layer (Route B in [`docs/PORT_PLAN.md`](../docs/PORT_PLAN.md)): the `nn::` runtime
that the game's SDK imports map to, the x86-64 build that compiles the matched and generated
sources under `src/` into one program, and later the NVN graphics shim and audio. Milestone M0,
this directory's first state, is the skeleton: the build configures, the runtime builds, every
source under `src/` compiles for x86-64, and the list of what does not port is known (nothing but
the SDK headers; see [What M0 found](#what-m0-found-2026-10-24)).

Rules that apply here as everywhere: no game data in git (the port reads the user's own dump at
build time), our own code only, and nothing copied from `lib/nnsdk`. The port does not depend on
that submodule: `runtime/include/nn/` is written from the game's own uses in `src/` and the
imported symbol names recorded in `data/functions/`.

## Layout

| Path | Contents |
|---|---|
| `CMakeLists.txt` | the `dread_port` project: `dread_game`, `dread_libs`, `dread_runtime`, `dread`, `sweep`, `link_report`, `loader_test` |
| `sample_duplicates.txt` | the sources `dread_game` and the whole-program link both leave out (frozen samples that define a function another source defines), with the reason |
| `main.cpp` | the executable's entry point; at M0 a banner |
| `runtime/include/nn/` | the port's `nn::` declarations, one header per namespace, each with a note on what the boot path needs from it |
| `runtime/include/dread_port.h` | the runtime's own surface: `dread_port_missing_function`, the trap behind every function without a source, `dread_port_mismatched_call`, the trap behind a call the link refused to alias, and what `main` and the window call (`dread_port_set_host_arguments`, `dread_port_request_exit`) |
| `runtime/src/` | stub implementations; every stub carries a `TODO(M<n>)` naming the boot-path function that needs it; `missing.cpp` holds the two traps |
| `nvn/` | the NVN shim over Vulkan (M2): the shader manifest reader and the binding layer, `dread_nvn`; the binding design and the engine's variant selection in [`nvn/README.md`](nvn/README.md) |

| `runtime/include/dread_port_loader.h`, `runtime/src/loader.cpp`, `runtime/src/image.cpp`, `runtime/src/host_memory.h` | the data loader: maps the dump's main NSO at its original addresses before the game's static initialisers ([The data loader](#the-data-loader-m1)) |
| `tests/loader_test.cpp`, `../tools/test_port_loader.py` | the loader's test: the core built into a library with clang and lld only, run on the user's dump |
| `../tools/port_sweep.py` | the compile-only sweep that lists what in `src/` does not port |
| `../tools/port_link.py` | the whole-program relocatable link and the report of what is missing ([The link (M1)](#the-link-m1)) |
| `../tools/port_lift.py` | semantic lifts of the boot slice's `U` functions into `build/port/lifted/`, checked by the P-code oracle ([Semantic lifts](#semantic-lifts-m1-step-3)) |
| `../tools/bshdat.py` | the `.bshdat` shader packages: GLSL sources, reflection, the study's SPIR-V in `build/shaders/`; `--install` is the build step behind the `shaders` target ([Shaders](#shaders-m2)) |

### Targets

- `dread_game` (OBJECT library): every `src/**/*.cpp` except the files in `sample_duplicates.txt`
  and anything matching `DREAD_SWEEP_EXCLUDE`. The four listed are the frozen Phase 3 samples that
  define a function another source also defines ([What M0 found](#what-m0-found-2026-10-24));
  `tools/port_link.py` reads the same list, so the build and the whole-program link leave out the
  same files. The other 32 samples under `src/compiler_id`, `src/flag_id` and `src/atk_id` are
  compiled like everything else. Compiled
  with `include/` and the runtime's headers, at the configuration's optimisation level (`-O2 -g`
  by default), `-fPIC`, `-fno-rtti -fno-exceptions`, `-fno-strict-aliasing` (the code type-puns as the original did), warnings off (the code is held to the
  original's bytes, not to a newer compiler's warnings). An object library rather than a static
  one so that the whole-program link of M1 keeps every function, referenced or not.
- `dread_libs` (OBJECT library): the vendored libraries `main` links statically, all of `lib/lua`
  and dlmalloc through `lib/dlmalloc/mspace.c`, compiled as C++ with `-fno-strict-aliasing`, as the
  matching build compiles them (`tools/buildcfg.py`'s `LUA_FLAGS`). Their functions are status `L`;
  the port needs them all the same, since the game calls Lua and allocates through the mspaces.
- `dread_runtime` (STATIC library): `runtime/src/*.cpp`, the stubs behind `runtime/include/nn/`.
- `dread`: `main.cpp` linked with `dread_runtime`. `DREAD_LINK_GAME=ON` adds `dread_game` and
  `dread_libs`, which fail to link until every function the boot path reaches has a source (M1).
  Built only when `DREAD_LINK` is ON.
- `dread_nvn` (STATIC library): `nvn/`, the NVN shim over Vulkan; only when `DREAD_VULKAN_INCLUDE` is set.
- `sweep`: runs `tools/port_sweep.py` with this build's compiler, target and standard library.
- `link_report`: runs `tools/port_link.py` the same way, over the sweep's objects (run `sweep` first).
- `shaders` (in `ALL`; `dread` depends on it): `tools/bshdat.py --install` compiles the game's shaders to SPIR-V from the dump into `DREAD_SHADER_DIR` ([Shaders](#shaders-m2)). On when the dump's shader folder exists (`DREAD_SHADERS`).

### Cache variables

| Variable | Default | Meaning |
|---|---|---|
| `CMAKE_BUILD_TYPE` | `RelWithDebInfo` with a single-configuration generator | `-O2 -g -DNDEBUG`: the sweep's optimisation level (`-O2`) with debug info. Left empty it would mean `-O0`, which is not what the sweep measured |
| `DREAD_LINK` | ON; OFF when `CMAKE_TRY_COMPILE_TARGET_TYPE` is `STATIC_LIBRARY` or `CMAKE_CROSSCOMPILING` | build the executable (needs a linker and a C/C++ runtime for the target) |
| `DREAD_LINK_GAME` | OFF | link `dread_game` into `dread` (M1) |
| `DREAD_LIBCXX_INCLUDE`, `DREAD_MUSL_INCLUDE` | on a Windows host, the libc++ 9.0.1 headers under `%LOCALAPPDATA%\dread-decomp\sources` and `lib/musl/include` when both exist (what `tools/buildcfg.py` uses); empty elsewhere | compile against these headers with `-nostdinc++` (compile only, nothing links against them). Empty means the host's own libc++ and libc, which is what a real host build uses |
| `DREAD_SWEEP_EXCLUDE` | empty | semicolon-separated CMake regexes over repository-relative paths; sources the sweep marks as not yet portable are left out of `dread_game` until fixed |
| `DREAD_GAME_VERSION` | `2.1.0` | passed to the sweep |
| `DREAD_PYTHON` | the repository's `.venv`, else `find_package(Python3)` | runs the sweep, the link report and the shader build step. A checkout without `.venv` (a worktree, a fresh clone) must set it to an interpreter with `pyelftools`: `tools/port_link.py` imports `tools/asmdiff.py`, which needs it (the sweep itself needs only the standard library) |
| `DREAD_SHADERS` | ON when `orig/<version>/romfs/system/shd/` exists at the first configure (a dump added later needs `-DDREAD_SHADERS=ON`) | build the `shaders` target (needs glslang: `tools/fetch_toolchains.py glslang-16.6.0`) |
| `DREAD_SHADER_DIR` | `<build dir>/shaders` | where `tools/bshdat.py --install` writes the SPIR-V and `shaders.manifest` |
| `DREAD_VULKAN_INCLUDE` | the newest `vulkan-headers-*` in the tool cache (`tools/fetch_toolchains.py vulkan-headers-1.4.328`), else `$VULKAN_SDK/include` | the Vulkan headers `dread_nvn` compiles against; empty: `dread_nvn` is not built |

## Configuring

### Windows, compile only (no Windows SDK)

The development PC has LLVM 23 but no Windows SDK and no MSVC, so nothing can be linked for
Windows there. The build still configures and compiles with the matching build's headers
(libc++ 9.0.1 from `tools/fetch_sources.py`, musl from `lib/musl`) for a Linux target triple:

```bat
cmake -S port -B build/port/cmake -G Ninja ^
  -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe" ^
  -DCMAKE_CXX_COMPILER_TARGET=x86_64-linux-gnu ^
  -DCMAKE_TRY_COMPILE_TARGET_TYPE=STATIC_LIBRARY -DDREAD_LINK=OFF
cmake --build build/port/cmake --target dread_runtime
cmake --build build/port/cmake --target dread_game
```

Linking a Windows executable needs one of: the MSVC Build Tools (then target
`x86_64-pc-windows-msvc`, lld-link, and the host's standard library: leave
`DREAD_LIBCXX_INCLUDE` and `DREAD_MUSL_INCLUDE` empty), MinGW-w64 (an `x86_64-w64-mingw32`
toolchain), or WSL (a Linux build, below). A Windows target is LLP64 (`long` is 32-bit):
`include/types.h` takes `u64` and `s64` from `long long` under `_WIN64` (merged with
var-flotsam/dread-decomp#184), and its `static_assert` checks the widths on every target. Since
the data loader (below) the game links only with the Itanium C++ ABI, which rules out
`x86_64-pc-windows-msvc` for `dread_game`: on Windows that leaves MinGW-w64 or WSL.

### Linux

```sh
cmake -S port -B build/port/cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build/port/cmake
./build/port/cmake/dread
```

Clang with libc++ or libstdc++; the code is C++17 with `-fno-exceptions -fno-rtti`. The matched
code uses Clang builtins and attributes throughout, so Clang is the compiler, not GCC.

### Linux in Docker (any host with Docker, including this PC)

```sh
docker build -t dread-port-linux port/docker                         # Ubuntu 24.04, Clang 18, lld, CMake, Ninja, Python
port/docker/run.sh python3 tools/fetch_sources.py libcxx-9.0.1.src    # once: libc++ 9.0.1's headers into build/cache
port/docker/run.sh python3 tools/port_sweep.py --version 2.1.0 --stdlib libcxx9 --objects build/port/linux/sweep
port/docker/run.sh python3 tools/port_addresses.py --version 2.1.0    # optional: vtables and named functions (needs build/check)
port/docker/run.sh python3 tools/port_link.py --version 2.1.0 --stdlib libcxx9 --objects build/port/linux/sweep \
    --out build/port/linux/link --lifted build/port/lifted --executable
port/docker/run.sh build/port/linux/link/dread                        # maps orig/2.1.0, runs the game until the first trap
```

`port/docker/run.sh` mounts the checkout at `/src`, `orig/` (read-only) and `build/` from where
they really are (a worktree's junctions are resolved; Docker does not follow them inside a bind
mount) and keeps the tools' download cache in `build/cache`. Nothing of the game goes into the
image. The headers are libc++ 9.0.1's over glibc (`--stdlib libcxx9`): the game's sources
instantiate libc++ 9's internal `std::__sort3` and its siblings (`lib/libcxx/sort.h`), which
libc++ 18 renamed, and the host's libstdc++ lacks them; the library linked is the host's libc++,
whose ABI is unchanged. All 3,830 sources compile so. `port_link.py --executable` then links
`build/port/linux/link/dread`:

- **At a fixed base above the image** (`--image-base`, default `0x7173000000`, just past the
  `.bss`): within 2 GB of all the PC-relative data, and above it because Linux starts the `brk`
  heap a random distance after the executable; below the image the heap landed inside it
  (`0x7116e6c000`) and the loader could not map. No PIE, but the position-independent start
  files (`Scrt1.o`, `crtbeginS.o`): the plain `crtbegin.o` uses 32-bit absolute relocations.
- **The residue gets traps** (`residue.s`): each name still undefined that the host does not
  define becomes a thunk that reports its name, a vtable 64 slots of one, a data name 0x100 zero
  bytes. On `main`: 24 functions, 9 vtables, 3 data names.

**First run (2026-10-26).** The loader maps the dump at `0x7100000000` and applies its 146,684
relocations (115,825 of the 128,576 code pointers translate to the port's functions, with the
1,941 semantic lifts linked), every static initialiser of the game runs, and `main` calls
`nnMain` (`src/engine/nnMain.cpp`), which walks the host arguments (`src/engine/HostArguments.cpp`)
and enters the main loop: the system start-up `fn_71001325d0` (`src/engine/SystemStartup.cpp`)
initialises the SDK heap, `nv` and `nn::fs`'s allocator, and the first trap is the engine's
aligned allocation `fn_7100083ba0` (`L` in `data/functions/`, but the game's: the default
allocator's slot 0x40). Each trap names the next function the boot needs: that is M1's work list.

### macOS

As Linux, with Apple Clang or Homebrew LLVM (`-DCMAKE_CXX_COMPILER=$(brew --prefix llvm)/bin/clang++`).
Untested on hardware, like the matching toolchain there.

## The sweep

```sh
python tools/port_sweep.py --version 2.1.0                 # host clang++, x86_64-linux-gnu on Windows
python tools/port_sweep.py --version 2.1.0 --no-runtime    # without port/runtime/include: the raw list
python tools/port_sweep.py --version 2.1.0 --only '^src/game_s7/' --jobs 16
```

It compiles every `src/**/*.cpp` on its own with the project's include setup
(`tools/buildcfg.py`: the libc++ 9.0.1 and musl headers, `include/`) and the host's code
generation flags (`-std=c++1z -O2 -fno-strict-aliasing -fno-rtti -fno-exceptions -fPIC -c`, none of the AArch64
flags), sorts the errors into categories (the message with file, line and quoted names
stripped, so one problem in a hundred generated files is one line), counts failures per top-level
`src/` directory, and lists separately the files that fail only because they include an SDK
header (`<nn/...>`): those are the port's own declarations to write. Objects go to
`build/port/sweep/`, the report to stdout and `build/port/sweep.json`; the exit status is always 0.
`lib/nnsdk` is never on its include path; `port/runtime/include` is, unless `--no-runtime`.

With `llvm-nm` next to the compiler (or on `PATH`) the sweep then reads the objects back and
reports definitions: a strong symbol defined in more than one object (a link error), and a
`fn_<addr>` function defined in more than one object under different signatures (two copies of
one function, which no linker notices). It takes the own name of each demangled symbol, so a
function address inside a template argument does not count as a definition. `tools/check.py`
reports duplicates too but leaves the Phase 3 sample directories out of that rule; the per-file
exclusions in `port/CMakeLists.txt` come from this report.

## What M0 found (2026-10-24)

Raw sweep (`--no-runtime`; Clang 23.1.3, `x86_64-linux-gnu`, libc++ 9.0.1 and musl headers):
**3,808 files, 3,799 compiled, 9 failed**, all nine in one category, `sdk header not found`:
`<nn/os.h>` (7 files, all through `include/base/UnkDefaultAllocator.h`),
`<nn/os/os_TickTypes.h>` (1) and `<nn/oe.h>` (1; the same file includes `<nn/vi.h>`). Per
directory: `src/base` 3 of 23, `src/game_s1` 5 of 219, `src/game_s7` 1 of 184, every other
directory 0, including `src/lifted` (287 files), `src/clones` (7), `src/reflection` (2,112),
`src/reflection_statics`, `src/reflection_types`, `src/tables` and the rest of `src/game_*`.
No inline assembly, no AArch64-only builtin, no failing layout assertion anywhere.

With the runtime's headers on the include path (the default): **3,808 of 3,808 compile**. The
declarations those nine files needed, and so the whole of the runtime's typed surface today, are:

| Header | Declares | Needed by |
|---|---|---|
| `nn/nn_common.h` | `nn::Result`, `nn::TimeSpan` (a plain `nanoseconds` field, as the game reads it) | every other header |
| `nn/os/os_TickTypes.h` | `nn::os::Tick`, `SystemTickFrequency` (the runtime's own rate, 19.2 MHz like the console; no game code depends on the value), `GetSystemTick`, `ConvertToTimeSpan`, `ConvertToTick` | `src/game_s1/fn_7100080c40.cpp` (frame timers) |
| `nn/os.h` | `nn::os::MutexType` (32 bytes, 8-aligned: fixed by the game's object layouts), `InitializeMutex`, `FinalizeMutex`, `LockMutex`, `UnlockMutex` (declared from the import names: the engine reaches them through four-byte wrappers, so `src/` itself names only `MutexType`), opaque `ThreadType`, `GetCurrentThread` | `include/base/UnkDefaultAllocator.h` and its seven includers |
| `nn/oe.h` | `PerformanceMode` (no enumerators: the game passes the value 1), `PerformanceConfiguration`, `Initialize`, `SetPerformanceConfiguration`, `GetProgramTotalActiveTime` | `src/game_s7/fn_71001587f0.cpp`, `src/game_s1/fn_7100080c40.cpp` |
| `nn/vi.h` | opaque `Display` and `Layer`, `Initialize`, `OpenDefaultDisplay`, `CreateLayer`, `GetNativeWindow` | `src/game_s7/fn_71001587f0.cpp` (graphics start-up) |
| `nn/fs.h`, `nn/hid.h`, `nn/audio.h`, `nn/time.h`, `nn/account.h`, `nn/prepo.h`, `nn/nfp.h`, `nn/diag.h`, `nn/mem.h` | nothing yet: each lists `main`'s imports from that namespace and what the boot path needs | M1 onwards |

Definitions (the sweep's `llvm-nm` pass over the 3,808 objects): one symbol is defined in two
objects, `fn_7100227448()` (`CActor::refl_GetClass`) in `src/flag_id/fn_7100227448.cpp` and
`src/reflection/CActor.cpp`; three more functions are defined twice under different signatures,
`fn_71005a2dd8`, `fn_71006a3390` and `fn_71006de790`, as a constructor in `src/flag_id` and with
a placeholder `char*` signature in `src/lifted`. All four are frozen Phase 3 samples, and those
four files are the only ones `dread_game` leaves out, so every function it compiles has one
definition. The other 32 samples (10 in `src/compiler_id`, 16 in `src/flag_id`, 6 in
`src/atk_id`; 22 `O`, 3 `m`, 1 `M` and 6 `L` in `data/functions/`) are the only definition of
their function and are compiled. `tools/check.py` derives their statuses from them but exempts
the sample directories from its DUPLICATE rule, which is why the sweep checks definitions itself.
No function outside the samples is defined twice.

What the sweep cannot see is what the link report below measures: the objects are compiled, not
linked, so every `fn_<addr>` callee without a source and every `g_unk_<addr>` data symbol and
vtable (`.data` and `.rodata` come from the user's dump at build time) only show up when the
objects are linked together. Two caveats on the numbers:
the sweep and the CMake build compile at the same optimisation level and with the same PIC
setting (`-O2`, `-fPIC`; the CMake default build type is `RelWithDebInfo`), but both have only
been run with the libc++ 9.0.1 and musl headers on the Windows PC. The host standard library a
Linux or WSL build links against (`--system-stdlib`, the CMake default there) has not been tried.
The CMake build on the Windows PC configures and builds `dread_runtime` and `dread_game`; it
cannot produce a Windows executable until a linker and a C runtime are installed (above).

## The link (M1)

```sh
python tools/port_link.py --version 2.1.0             # build/port/sweep -> build/port/link/
cmake --build build/port/cmake --target link_report   # the same, with the build's compiler and target
```

M1's first step is to link everything the sweep compiled into one program and see what is missing.
`tools/port_link.py` does that as a **relocatable link**: `ld.lld -r` over the sweep objects (every
`src/` source minus `sample_duplicates.txt`) and, since M1's second step, the 30 objects of the
vendored libraries (`lib/lua`, `lib/dlmalloc`; the tool compiles them as `dread_libs` does,
`--no-libraries` leaves them out) into `build/port/link/dread_game.o`, which keeps
every undefined symbol as it is. A shared-object link with unresolved symbols ignored is not an
option: `include/base/UnkCloneHelpers.h`, `include/base/UnkStaticStrId.h`,
`include/base/global/CRntString.h` and `include/game/UnkTUInit.h` declare globals with
`visibility("hidden")`, and lld refuses an undefined hidden symbol in a shared object. The link
takes under a second, reports no duplicate symbol (a duplicate fails the run: it is a real problem),
and leaves 60,402 undefined symbols next to 131,638 defined symbol-table entries (105,394 distinct
names; local names such as `.L.str` repeat across objects). The tool resolves symbols to addresses
exactly as `tools/check.py` does (a definition by `fn_<addr>` anywhere in the mangled symbol, which
is how a template instantiation tagged `UnkRefl_fn_<addr>` counts as that function; a reference by
its demangled name without parameters, `fn_`/`g_unk_`/`unk_<addr>` in any namespace, else a name
unique in `data/functions/`), with one guard: a symbol whose only mention of an address is a
function pointer (`&fn_<addr>` in a template argument) defines nothing at that address, so it is
neither an alias target nor coverage. It classifies each undefined symbol and generates the inputs
that close the link:

- **Function aliases** (`aliases.rsp`, one `--defsym=<reference>=<definition>` each). Most
  references name a function that *is* defined, under another symbol: hand-written files declare
  callees with their own parameter lists (the declaration debt `tools/decl_check.py` tracks),
  `gen_lift` declares every callee as `callee::fn_<addr>(...)` with one overload per argument list
  and uses `h::`, `noret::`, `lift_guard::`, `fp::`, `fret::`; `gen_clones`, `gen_statics` and the
  hand files use namespaces of their own. In `-r` mode a `--defsym` to a defined symbol gives the
  alias that symbol's section and offset (verified for ordinary and COMDAT weak definitions), so
  the alias costs nothing. The relocations say how each reference is used: a PLT32 is a call, a GOT
  or absolute entry only takes the address (the function tables of `src/reflection_types` and
  `src/reflection`), and for the calls the two parameter lists are compared by x86-64 argument
  class, since a `float` or `double` by value travels in a different register than an integer or a
  pointer. The sub-classes are in the report; `class_mismatch` is the only unsafe one, and a called
  alias with that verdict is not emitted: the call gets a thunk to `dread_port_mismatched_call`
  instead, so it fails at the call with the address rather than computing with the wrong register
  (`--alias-mismatch` emits the alias anyway, for experiments).
- **Absolute data symbols** (`aliases.rsp`, `--defsym=<symbol>=0x<address>`), for every
  `g_unk_<addr>`/`unk_<addr>` reference, plain or namespaced. Nothing in `src/` defines game data:
  `.data`, `.rodata` and `.bss` come from the user's dump, which M1's data loader maps at the
  original addresses at build time, and an absolute symbol is exactly what a reference to such a
  mapping resolves to. Caveat for the loader and the final link: the hidden declarations above are
  referenced PC-relatively, and the report counts those relocations (`R_X86_64_PC32`: 20,178
  against 5,467 symbols, 5,315 addresses, in this run; `classes.data.pc_relative` in `link.json`),
  so the program image has to be linked within 2 GB of the game's addresses (`0x7100000000` and
  up), or those declarations lose `hidden`; everything else goes through the GOT and does not care.
- **Traps and jumps** (`traps.s`, assembled to `traps.o`): for every undefined function symbol
  whose address has no definition at all, a thunk `movabsq $0x<addr>, %rdi; jmp
  dread_port_missing_function`, so a boot that reaches unported code stops with the address instead
  of running off (`runtime/src/missing.cpp`). Imports of C library names (`<EXTERNAL>::strlen` and
  the like, referenced through their 4-byte thunk's `fn_<addr>`) get `jmp strlen` instead, and so
  do the status-`L` functions reached by their `fn_<addr>` name whose `data/functions/` name is the
  host's: `operator new`, `operator delete`, `operator delete[]` and `free` jump to the host's
  `_Znwm`, `_ZdlPv`, `_ZdaPv` and `free`, and `fn_7100000250`, which `include/base/UnkAtExit.h`
  identifies as `main`'s `__cxa_atexit`, jumps to the host's `__cxa_atexit` (the tool's
  `HOST_BY_ADDRESS`; every static initialiser registers its destructors through it). The game's
  own allocator, dlmalloc's `mspace_*`, is not among them: it is compiled in from `lib/dlmalloc`. Imports of `nn::` names get a
  `--defsym` to the runtime's function when `runtime/` defines one, a trap otherwise; those are the
  "runtime stubs needed".
- **A second relocatable link**, `dread_whole.o` = `dread_game.o` + the runtime's objects
  (compiled by the tool with the sweep's flags, so it does not depend on the CMake build) +
  `traps.o` + `@aliases.rsp`. What is still undefined after it, the residue, is what the port has to
  provide outside this mechanism. lld took the 53,244 `--defsym` in one response file; no chunking
  was needed.

The boot slice (`tools/callgraph.py`, every function reachable from `nnMain` by direct calls) is
reported by coverage (defined; defined but reached through an alias only; host, the standard
library's by address; none), and `boot_missing.csv` lists the functions without a definition by
callers in the slice: the lifter's coverage worklist and the matching queue. `link.json` holds
everything the report prints with the full lists (aliases with their ABI verdict, missing functions
with status and size, imports, the host-library functions reached by address). Every figure below
is printed by the tool or stored in `link.json`; none is computed by hand.

### What the first link found (2026-10-24)

Clang 23.1.3 and lld for `x86_64-linux-gnu`, the sweep's objects with `port/runtime/include` on
the include path: 3,804 objects, 131,638 defined symbol-table entries (105,394 distinct names) and
60,402 undefined symbols; 84,966 addresses have a function definition (71,965 under their own
`fn_<addr>` name, 17,638 as a tagged template instantiation; no address is mentioned only as a
function pointer), 530 global functions resolve to no address (anchors, pads, the users of
statics), 4 vtables are defined.

| Class | Symbols | Addresses | Detail |
|---|---|---|---|
| Function alias | 37,531 | 35,176 | 27,188 only take the address; 10,343 are calls: `same` 7,548, `same_with_this` 1,132, `ref_more_args` 459 (all safe), `ref_fewer_args` 1,185 (check), `unmangled_definition` 13, `class_mismatch` 6 (unsafe: three declarations of `fn_7100a2cb7c` pass a `float` where the definition takes `bool`, and three lifted callers pass a `float` to a definition that takes `char*`; these six are trapped, not aliased). 141 aliases (85 addresses) choose among several distinct definitions (C1/C2 constructors, tagged helpers); the alias takes the one whose signature fits best |
| Missing function | 6,741 | 5,905 | status `U` 5,849 addresses, 2,626,352 bytes (4,871 of the symbols are calls); status `L` 56 addresses, 13,568 bytes (26 Lua API functions and dlmalloc's `mspace_*`: `lib/lua` and `lib/dlmalloc` are vendored and not compiled into the port yet; `fn_`-named library functions); no `O`, `m`, `M` or `W` |
| Import | 117 | 92 | C library 19 addresses (`jmp` thunks); `nn::os` 21 (the runtime defines 9), `nn::nfp` 21, `movie` 8, `nn::audio` 6, `nn::hid` 4, `nn::mem` 4, `nn::time` 3, `nn::diag` 2, `nn::oe` 2 (runtime 1), `nn::fs` 1, `nn::util` 1 |
| Data | 15,709 | 15,450 | 4,993 namespaced references; none defined in the link; 5,467 symbols (5,315 addresses) referenced PC-relatively, 20,178 relocations |
| Vtable | 71 | | `vtable for CAttack` and 70 more: classes with a virtual function nothing defines |
| Standard library | 39 | 4 | by name: C 21, C++ 10 (`__cxa_*`, `operator new/delete`, `std::__shared_weak_count`); by address: 8 references to the 4 status-`L` functions whose name is the host's (`operator new` 0x7100083b60, `operator delete` 0x7100083c00, `operator delete[]` 0x7100083c30, `free` 0x7100132340), each a `jmp` to the host's symbol |
| `nn::` by name | 12 | | 10 defined by the runtime, 2 not (`nn::hid::VibrationPlayer` and `VibrationNodeConnection` destructors) |
| Named without address | 182 | | `UnkIdTable::UnkIdTable()`, `UnkMapIter<...>::Advance()`, ...: they need an `fn_<addr>` or a `data/functions/` name first |

Boot slice: 4,441 functions reachable from `nnMain`, 230 of them imports, 4,211 game functions.

| Status | Functions | Defined | Aliased only | Host | None | Bytes of none |
|---|---|---|---|---|---|---|
| `O` | 1,875 | 975 | 899 | 0 | 1 | 384 |
| `m` | 49 | 31 | 18 | 0 | 0 | 0 |
| `M` | 1 | 1 | 0 | 0 | 0 | 0 |
| `U` | 2,153 | 0 | 0 | 0 | 2,153 | 1,237,420 |
| `L` | 133 | 0 | 2 | 3 | 128 | 49,176 |
| all | 4,211 | 1,007 | 919 | 3 | 2,282 | 1,286,980 |

The one `O` function without a definition is `fn_71011bc830`: `src/game_s7/fn_71011bc0b0.cpp`
defines it with an `asm(".set ...")` alias to a libc++ `__sort3` instantiation that the x86-64
compile does not emit, so the alias defines nothing there. The three `L` functions the host
provides are `operator new`, `operator delete` and `operator delete[]`. The imports the slice
reaches: C library 90, `nn::os` 30 (runtime 11), `movie` 24, `nn::oe` 20 (runtime 4), `nn::hid`
12, `nn::prepo` 12, `nn::account` 8, `nn::fs` 8, `nv` 6 (the NVN bootstrap), host C++ library 6,
`nn::mem` 4, `nn::time` 4, `nn::util` 4, `nn::diag` 2. The worklist's head is an `L` function with
470 callers in the slice (`fn_7100000250`), then `U` functions with 80, 56 and 36 callers, then
another `L` function with 34 (`fn_7100000850`); 103 of the slice's 128 missing `L` functions are
Lua's.

Link inputs: 53,244 `--defsym` (37,525 function aliases, 15,709 data symbols, 10 imports to the
runtime), 6,814 trap thunks (6,808 for functions without a definition, 6 for the refused
`class_mismatch` calls) and 48 jumps to the host's library (40 import thunks, 8 by address). The
second link succeeds in about 3 s and leaves a **residue of 304** undefined symbols: 71 vtables,
49 standard library names (C 31: the libc the thunks jump to and the C runtime; C++ 18:
`operator new/delete`, which the by-address thunks jump to as well, `__cxa_*`,
`std::recursive_mutex` and `steady_clock` from the runtime's own stubs), 2 `nn::hid` destructors
the runtime does not define yet, and the 182 names without an address. That residue is the port's
remaining work outside the lifter: the vtables are the data loader's and the matching queue's
(a vtable is defined once every virtual function of the class is), the standard library is the
host's at the final link, the named functions need addresses, and the runtime stubs come in call
order along the boot path.

### With the vendored libraries (2026-10-25)

M1's second step compiles `lib/lua` and `lib/dlmalloc` into the link (`dread_libs`) and sends
`fn_7100000250` to the host's `__cxa_atexit`. Measured on `main` after hub batch 2 (3,812 sources,
3,808 sweep objects, all compiling), first without the libraries (`--no-libraries`), then with them:

| Boot slice, functions without a definition | `--no-libraries` | With the libraries |
|---|---|---|
| status `U` | 2,148 (1,236,964 bytes) | 2,148 (1,236,964 bytes) |
| status `L` | 127 (48,876 bytes) | 8 (1,196 bytes) |
| status `O` | 1 | 1 |
| all | 2,276 (1,286,224 bytes) | **2,157 (1,238,544 bytes)** |

With them, 1,109 of the slice's 4,211 game functions are defined, 941 are reached through an alias
only and 4 are the host's. Link 1 finds no duplicate symbol, and 369 more addresses have a
definition: the Lua and dlmalloc functions, which resolve by their unique names in
`data/functions/`. Outside the slice, 23 status-`L` addresses (2,232 bytes) are still
missing. Sixteen are `nn::atk` middleware. Five are functions next to `nnMain` that the CSV marks `L`
by its `runtime?` inference, among them the `CStrId` destructor `fn_710000051c`. Two sit in Lua's
range without a name (`fn_71010a4740`, `fn_71010a4900`). The slice's 8 are `fn_7100000790`
(14 callers), `fn_7100083ba0` (3), `fn_71000005d0`, `fn_7100000610`, `fn_71010a4900`,
`fn_71010a58e0`, `fn_71010a5a70` and `nnMain` itself, the entry the port's `main` will call.

The residue of the second link grows from 304 to 375, by C library names only: Lua's `stdio`,
locale and time functions, which the host's libc provides at the final link. `C_LIBRARY` lists them,
so none of them lands under "named without address". The link inputs are 53,203 `--defsym`, 6,723
traps and 51 jumps to the host's library.

## Semantic lifts (M1 step 3)

```sh
python tools/port_lift.py --version 2.1.0                      # build/port/link/boot_missing.csv -> build/port/lifted/
python tools/port_link.py --version 2.1.0 --lifted build/port/lifted
python tools/port_lift.py --version 2.1.0 --debug 7100129910   # every attempt and the first disagreeing input
python tools/port_lift.py --version 2.1.0 --top 300 --mutants 200 --out build/port/lifted_t300   # test the oracle
```

The boot slice's `U` functions get a source that behaves like the original without matching it.
`tools/port_lift.py` lifts them with `tools/gen_lift.py`'s lifters in semantic mode, compiles every
variant for AArch64 with the project compiler and `-fno-strict-aliasing -fwrapv
-fno-delete-null-pointer-checks`, and runs each against the original, in variant order, as Ghidra
P-code (`tools/pcode_emu.py`, oracle (b) of `docs/PORT_PLAN.md`). The first variant whose calls
(target and the argument registers the callee reads, `tools/abi_facts.py`), writes outside its own
frame and result agree with the original's on every conclusive input is compiled for x86-64 with
the sweep's compiler and flags:

| Path | Contents |
|---|---|
| `build/port/lifted/src/fn_<addr>.cpp` | the lift, opening with `DREAD_PORT_LIFTED(<addr>)` (empty unless a boot defines it: oracle (c)) |
| `build/port/lifted/obj/fn_<addr>.o` | its x86-64 object, which `port_link.py --lifted` links |
| `build/port/lifted/manifest.json` | the generator version and its sources' blob hashes, and every worklist function's outcome, oracle inputs and coverage |
| `build/port/lifted_work/` | the AArch64 attempts and their laid-out objects |
| `build/port/pcode_cache.pickle`, `build/port/abi_facts.pickle` | P-code per instruction word and the calling-convention facts, kept between runs (game-derived: `build/` only) |

None of it is tracked and none of it counts as matching (open decision 2; `tools/check.py` never
sees it). `port_link.py --lifted` leaves out a lift whose address the sources now define and says
when the manifest's generator sources differ from the checked-out ones.

**Result on `main` after hub batch 2 (2026-10-25).** Of 2,148 `U` functions (1,236,964 bytes):

| Outcome | Functions | Bytes |
|---|---|---|
| accepted | 1,777 | 839,396 |
| rejected by the oracle | 157 | 123 KB |
| inconclusive (time-outs, a return outside the function) | 23 | 6 KB |
| lifter gaps: `adds`/`ands` carry or overflow conditions, jump tables (`adr`), other conditions, flat-mode register order, float widths, stack arguments, ... | 191 | 262 KB |

After the follow-up step (exact flags and flag locals, jump tables, float pass-through, incoming
argument registers, four oracle fixes), the same worklist gives:

| Outcome | Functions | Bytes |
|---|---|---|
| accepted | **1,941** | 1,042,620 |
| rejected by the oracle | 104 | 79,236 |
| inconclusive | 24 | 6,256 |
| lifter gaps (stack arguments, flat-mode register order, `x8` results in tail position, other jump-table shapes, ...) | 79 | 108,852 |

and the link leaves **216 slice functions (195,924 bytes) without a source**, headed by
`fn_7100000790` (`L`, 14 callers), `fn_7100076acc` (12, rejected by the oracle) and `fn_71011a2fbc`
(12, an `x8` result in a tail call). The mutation test then rejects 39% of the faults, 76% on
completely covered functions.

The link with them (`port_link.py --lifted`): of the slice's 4,211 game functions, 1,777 are
lifted and **380 (399,148 bytes) still have no source**, down from 2,157 (`boot_unlifted.csv`,
headed by `fn_710023b580`, 19 callers, an `adds` carry condition, and
`fn_7100defcec`, 16, which the oracle rejects: the original faults on one input where the lift returns). The residue grows from 375 to 378 by `atoi`, `roundf` and `wcscmp`, the host's.

The oracle's reach is the limit to keep in mind: its inputs run a median of 67% of an original's
instructions, and of 300 faults injected into accepted lifts it rejects 45%, 79% on the functions
it covers completely. What it found along the way were real lifter bugs that byte identity had
hidden, all fixed in semantic mode: a compare reading a register its own `subs` overwrote, a
post-indexed load reading the incremented base, a 64-bit read narrowed to the type of a write
elsewhere in the function, callee-saved registers spilled into the locals and dropped as if they
were the prologue's saves.

## Addresses from matched code (M1)

```sh
python tools/port_addresses.py --version 2.1.0   # build/check objects -> build/port/addresses.json, about a minute
python tools/port_link.py --version 2.1.0        # uses it when present (--no-addresses to compare)
```

The link's residue kept two kinds of names nothing gives an address: vtables of classes whose
virtual functions are not all defined (`vtable for CAttack`), and functions named other than
`fn_<addr>` (`base::UnkSubAllocator::UnkSubAllocator()`). The game's own code knows them: a
function whose bytes match the original loads the vtable or calls the function, and the
original instruction encodes the address. `tools/port_addresses.py` reads `tools/check.py`'s
AArch64 objects, diffs each as `check.py` does, and asks its relocation check about every
symbol it cannot resolve as if it were at 0: the reported mismatch is the original's target
(calls, page-and-offset pairs and GOT slots; not a lone half). On `main`: 89,299 byte-matching
functions, 9,297 symbols placed (67 vtables). `port_link.py` then defines a vtable as an
absolute symbol at the game's own (the data loader makes its slots x86-64 code; the Itanium
layout is the same) and treats a placed name like any `fn_<addr>` reference (an alias of the
definition there, or a trap). The residue drops from 439 to **226**: 191 host library names, 9
vtables and 26 names, all of them either unused by matched code or one of the 12 symbols whose
uses disagree. Those are naming problems in the sources, left out on purpose and listed with
the functions behind each address in `addresses.json`: a placeholder class name reused for
different classes (`UnkCallbackSet` and its vtable at `0x7101a33fa0` and `0x7101a20950`,
`UnkAttackListener`, `UnkElem50`, `unk_any_props::UnkPropAny` and `UnkPropList` with five
vtables each, `UnkBase_0x30`, `UnkElem40`, `UnkElem80`), and `base::global::CRntString::~CRntString()`,
251 uses at `0x710007ecd4` and 2, in `fn_710026ead0` itself, at `0x710026ead0`.

## The runtime (M1)

Every SDK import the boot slice reaches is defined in `runtime/`: `nn::os` 30, `nn::oe` 20,
`movie` 24, `nn::hid` 12, `nn::prepo` 12, `nn::fs` 8, `nn::account` 8, `nv` 6, `nn::mem` 4,
`nn::time` 4, `nn::util` 4, `nn::diag` 2 (counting the GOT slots; `port_link.py` aliases each import's four-byte thunk to the
runtime's function by name), and so is every one a source calls from outside the slice. Each
definition's `TODO(M<n>)` names its callers; `runtime/include/nn/os.h` and `oe.h` say where each
layout comes from.

- **Threads** (`CreateThread`, `StartThread`, `WaitThread`, `DestroyThread`, `SetThreadCoreMask`,
  `SetThreadNamePointer`) run on host threads (`std::thread`); the game's stack, priority and core
  mask are kept but unused. `GetCurrentThread` returns the block `CreateThread` was given, as the
  engine's thread wrapper `fn_7100066744` expects, and a runtime block on any other thread.
  `ThreadType` is 0x1c0 bytes and `LightEventType` 12 (that wrapper lays out two light events at
  +0x1c0 and +0x1cc); `EventType`, `SystemEventType` and `MessageQueueType` stay opaque until a
  source embeds one.
- **Events, light events, system events, message queues** are host objects (a mutex and a
  condition variable) whose pointer the runtime stores in the game's object with a tag word: a call
  on an object nobody initialised stops with the call's name. System events have no producer in the
  game: the runtime's own stubs create and signal them (`InitializeSystemEventForPort`).
- **`nn::oe`** reports the docked operation mode, ignores performance and boost settings, and
  delivers one notification, the exit request (4), after `dread_port_request_exit()`; the
  notification loop `fn_71001327c0` sets the game's quit flag on it.
- **`GetHostArgc`/`GetHostArgv`** return what `main` passed to `dread_port_set_host_arguments`.
- **`nn::fs`** (all 22 imports): each mount is a host directory. `rom` is the dump's
  `romfs/` under `dread_port_orig_directory()` (`$DREAD_ORIG`, default `orig/2.1.0`), read-only;
  the save mount (`savedata`, from `fn_7100fc3510`) is `$DREAD_SAVE` (default `build/save`),
  written in place, so `CommitSaveData` has nothing to do (a crash keeps a write the console
  would roll back). Failures use the console's encoding, which the engine's file object
  (`fn_7101235090`) decodes itself: module 2, description from bit 9 (1 path not found, 2
  already exists, 7 target locked, 8 directory not empty; the port's own host failure is 6000).
  Directory entries keep the game's 0x310-byte layout (`fn_710002e6e0`). Files are `std::FILE`
  streams and directories `std::filesystem`; on this PC that is compile-checked only (no C++
  runtime to run it with).
- **The rest of the system start-up** `fn_71001325d0`: `nn::mem::StandardAllocator` (the game's
  one, at `0x710ab70000` in the mapped `.bss`) allocates on the host's heap with a header per
  block, not in the 32 MB region the game names; `nv` keeps the allocator callbacks and the 8 MB
  it is given for the NVN shim (`GetGraphicsSetupForPort`); `nn::account` has one local user,
  always open, with a fixed id (so one save directory); `nn::util::GenerateUuid` is a random
  version-4 UUID, `TSNPrintf`/`SNPrintf` are `vsnprintf`; `nn::time` reads the host's local
  time into the game's 0xd0-byte `ClockSnapshot`; `nn::diag::detail::AbortImpl` (both forms)
  prints what it was given and aborts. Not yet: `nn::util::DecompressGzip` (needs an inflater;
  its one caller is outside the slice) and `ReferSymbol`.
- **`nn::hid`** (all 34 functions; the parameter lists from the imports' mangled names, which
  the dump's dynamic symbols carry): one handheld controller (id 0x20), always connected, whose
  buttons and sticks the port's window sets with `dread_port_set_npad_state`; the input poll
  `fn_710006deac` wants exactly one connected. No vibration devices, so the rumble graph is built
  and connected but never reaches a motor. The vibration classes keep their layouts in
  `include/base/input/UnkHidVibration.h` (the matching build uses them too; their members are
  declared there, `nn/hid.h` completes the small types); their vptr points at a table of traps
  that names the class and slot, since the port's nodes have no virtual functions of their own.
- **`nn::prepo`**: play reports accepted and dropped; nothing is sent anywhere.
- **`movie`** (all 55 functions): `Buffer` is complete (0x40 bytes, as the player's arrays
  step); every source has no tracks until the port has a video decoder: `SetDataSource`
  accepts any file or stream, `GetTrackCount` answers zero, so the player's track reader
  `fn_71002233c4` reports each track missing, and the rest returns `kStatusNoMedia`. The game's
  extractor and decoder subclasses write their own vptr; their vtables in `.data` name the
  library's functions, which the loader resolves by name.
- **`nn::oe::GetDesiredLanguage`** answers `$DREAD_LANGUAGE` (a code the game knows: `en-US`,
  `fr`, `ja`, ...), else `en-US`.
- **Checking a signature** (`tools/port_imports.py --version 2.1.0`): the dump's dynamic symbols
  name every import with its parameter types; the tool compares them with the names
  `image_tables.s` can resolve, per namespace (`--list NS` prints the rest). A stub whose
  mangled name matches has the console's parameter types. On `main` with this change: 285 of
  547 imports resolve; every implemented namespace completely, except `nn::hid`'s one data
  symbol. Left: `nn::audio` 149, `nn::nfp` 26, `nn::os` 13 (memory blocks, multi-waits),
  `nn::util` 11 (math tables and gzip), the C++ ABI's and some C names.
- **Imports outside `nn::`** (`nv`, `movie`) resolve to the runtime by name like `nn::` ones, and
  an overloaded name (`AbortImpl`) goes to the overload whose parameters fit the caller's
  reference best (`port_link.py`'s `runtime_overload`, the alias ranking).

## The data loader (M1)

```sh
python tools/port_link.py --version 2.1.0              # also writes build/port/link/image_tables.s
python tools/test_port_loader.py --version 2.1.0       # the loader on the dump, in this process (2 s)
cmake --build build/port/cmake --target loader_test    # the same with the build's compiler
```

The game's `.rodata`, `.data` and `.bss` are not in git or in the executable: the runtime maps them
from the user's dump when the port starts (`runtime/include/dread_port_loader.h` has the design).
`dread_port_map_game_image` (`runtime/src/image.cpp`) runs as a priority-101 constructor, before
every unprioritised static initialiser and so before the game's own, reads
`$DREAD_ORIG/exefs/main` (default `orig/2.1.0/exefs/main` under the working directory), checks the
module ID against 2.1.0's and calls the core, `dread_port_load_image` (`runtime/src/loader.cpp`):

- **At the original addresses.** The image is mapped at `0x7100000000`, `.text` through the end of
  the `.bss` (`0x72a19000` bytes, 1.82 GB of it `.bss`), so every absolute `g_unk_<addr>` symbol
  `port_link.py` defines now points at the game's bytes, and every pointer inside `.data` stays
  valid. The PC-relative references (5,467 symbols, `0x71015af379` to `0x7172a112e8`) need the
  port's code within 2 GB of all of them: about 214 MiB below `0x7100000000` or 235 MiB above the
  `.bss`, so the final executable is linked at a fixed base there (`-no-pie` with a text-segment
  address on Linux, `/FIXED /BASE` on Windows), never position-independent at a random address.
- **Relocations as the console applies them**, except code pointers. Of 2.1.0's 146,684 dynamic
  relocations (145,813 relative, 489 PLT, 364 absolute, 18 GOT), 128,576 relative ones point into
  `.text`: vtable slots and callbacks. Each is translated to the port's x86-64 function for that
  address through `dread_port_functions`, a table `port_link.py` writes into `image_tables.s`
  from the link's own definitions (91,166 addresses on `main` today: the own `fn_<addr>` symbol
  first, else the alias target, the runtime function or the link's trap or jump thunk), or, for an
  address with no entry, to a 32-byte trap thunk the loader writes that reports the address
  (`dread_port_missing_function`). The symbol relocations are all imports; they resolve by name
  through `dread_port_imports` (the runtime's `nn::` functions by mangled name, the C library names
  `main` imports, `__cxa_pure_virtual` and the C++ ABI's type-info vtables), else a thunk to
  `dread_port_missing_import` with the name.
  On `main` with the loader the import table has 144 names, and link 2's residue is 413: the 21
  more host names the tables reference (the C library names `main` imports, the C++ ABI's, `mmap`)
  are all standard library.
- **The data references** the link defines (`dread_port_data_references`, the 15,453 `g_unk_`
  addresses) must each lie inside the image, or the load fails before anything is mapped.
- **Protection**: `.text` and `.rodata` read-only once relocated (they hold no relocation slot),
  `.data` and `.bss` read-write, the thunks executable and read-only. The `.text` bytes are mapped
  for reading only: the port never executes AArch64 code.

The tables are generated from our sources' symbols, not from game data; the loader reads the
relocations from the dump at run time. Besides its fixed base, the final link needs the Itanium C++
ABI (Linux, macOS, MinGW; not MSVC), because the game's vtables in `.data` are used as they are
(after translation they are valid x86-64 vtables, which also makes the 71 vtables nothing defines
in the link the game's own, at addresses still to be named).

**The test** (`tools/test_port_loader.py`): the core is freestanding (no allocation, no standard
library; the host's memory services are passed in, `runtime/src/host_memory.h`), so it builds with
nothing but clang and lld: on Windows a DLL linked by `lld-link /noentry /nodefaultlib` against a
`kernel32.lib` that `llvm-dlltool` makes from five names, no Windows SDK or C runtime; elsewhere a
shared object. Python loads it with `ctypes`, maps the dump in its own process and checks the
refusals (not an NSO, wrong module ID, unsorted table, data reference outside the image), every
byte of `.text`, `.rodata` and `.data` against `tools/nso.py`'s decompression with the relocations
applied in Python, `.bss` samples, every relocated slot (half of the code targets in the table, the
rest trap thunks carrying their address; an import thunk carrying its name), calls through a
thunk and a translated pointer, the protection (Windows `VirtualQuery`) and that a second load
fails on the taken range. On 2.1.0: 26 of 26 checks in about 2 s; a mutation that skips every 64th
code pointer is caught (9,264 wrong slots).

## Shaders (M2)

```sh
python tools/fetch_toolchains.py glslang-16.6.0       # once, by name (not in the default fetch): Khronos glslang
cmake --build build/port/cmake --target shaders       # the build step: SPIR-V + shaders.manifest, about a minute
python tools/bshdat.py --version 2.1.0 --compile      # the study: every mode, build/shaders/2.1.0/compile_report.csv
```

`tools/bshdat.py` parses the game's 478 `.bshdat` packages (layout in its docstring) and writes,
per package, `p<key>.vert` and `p<key>.frag` for each program (the GLSL its debug section carries,
includes inlined, the engine's variant `#define`s after `#version`) and `manifest.json` (variant
keys and defines, includes, section offsets, the compiler's reflection of each stage, the
package's name tables) to `build/shaders/<version>/`.

**The build step** (decision 3: Vulkan, SPIR-V at build time). The `shaders` target (in `ALL`, and
a dependency of `dread`) runs `tools/bshdat.py --install ${DREAD_SHADER_DIR}`, by default
`<build dir>/shaders/` beside the executable. It compiles every stage once, for Vulkan, with the
loader's rewrites (`shim_rewrite`: each uniform block gets the binding NVN's compiler gave it,
from the reflection; `GL_NV_explicit_typecast` is enabled; the per-stage `FIXUPS`), one descriptor
set per stage (vertex 0, fragment 1) and the combined samplers from binding 16. Each module is then
read back (`spirv_resources`) and checked against the stage's NVN reflection: every active block
and sampler at the binding the shim expects, and only the `Shader` capability. The result is
`<package>/p<key>.<stage>.spv` and `shaders.manifest`, a line-per-record table (format in
`install()`) of packages, variant keys and defines, per-stage blocks and samplers with their NVN
slot and Vulkan binding, inputs and outputs, each stage's control key (how the NVN shim
recognises the stage `nvnProgramSetShaders` receives; [`nvn/README.md`](nvn/README.md)), and the
package's attribute and uniform name tables with their engine IDs. The manifest is written last and only when every stage passed, so a failed
build reruns. On 2.1.0 all 1,342 stages of the 671 programs compile and match (31 MB of SPIR-V).
The target is on when the dump's `romfs/system/shd/` exists (`DREAD_SHADERS`), and reruns when the dump's packages, the tool or the glslang binary change; the SPIR-V is
derived from game data, so it stays under `build/` and is never installed or distributed.

The one source fix (`FIXUPS`): `pp_fxaa_nx`'s fragment stage is FXAA 3.11, whose `FxaaTexOff`
passes its `off` parameter to `textureLodOffset`; GLSL requires that offset to be a constant
expression (NVIDIA's compiler inlined the function and folded it). Every call passes a literal, so
`FxaaTexOff` becomes a macro and each call gets its constant offset (`ConstOffset` in the SPIR-V):
the same texels as the original. A fixup is a regular expression over the construct (never the
shipped text, which is game data); one that does not match exactly once fails the stage.

`--compile` is the study behind this: glslang on every source as extracted and in each rewrite
mode, OpenGL and Vulkan (`.gl.spv`, `.vk.spv` beside the sources), `compile_report.csv` and a
summary; 1,342 of 1,342 stages compile in the shim modes. The binding model is recorded in
`docs/JOURNAL.md` (2026-10-26). Everything under `build/` is game data and stays local.
