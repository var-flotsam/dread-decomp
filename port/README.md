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
| `CMakeLists.txt` | the `dread_port` project: `dread_game`, `dread_runtime`, `dread`, `sweep` |
| `main.cpp` | the executable's entry point; at M0 a banner |
| `runtime/include/nn/` | the port's `nn::` declarations, one header per namespace, each with a note on what the boot path needs from it |
| `runtime/src/` | stub implementations; every stub carries a `TODO(M<n>)` naming the boot-path function that needs it |
| `../tools/port_sweep.py` | the compile-only sweep that lists what in `src/` does not port |

### Targets

- `dread_game` (OBJECT library): every `src/**/*.cpp` except the Phase 3 sample directories
  (`src/compiler_id`, `src/flag_id`, `src/atk_id`, which define functions other files define
  too) and anything matching `DREAD_SWEEP_EXCLUDE`. Compiled with `include/` and the runtime's
  headers, `-fno-rtti -fno-exceptions`, warnings off (the code is held to the original's bytes,
  not to a newer compiler's warnings). An object library rather than a static one so that the
  whole-program link of M1 keeps every function, referenced or not.
- `dread_runtime` (STATIC library): `runtime/src/*.cpp`, the stubs behind `runtime/include/nn/`.
- `dread`: `main.cpp` linked with `dread_runtime`. `DREAD_LINK_GAME=ON` adds `dread_game`, which
  fails to link until every function the boot path reaches has a source (M1). Built only when
  `DREAD_LINK` is ON.
- `sweep`: runs `tools/port_sweep.py` with this build's compiler, target and standard library.

### Cache variables

| Variable | Default | Meaning |
|---|---|---|
| `DREAD_LINK` | ON; OFF when `CMAKE_TRY_COMPILE_TARGET_TYPE` is `STATIC_LIBRARY` or `CMAKE_CROSSCOMPILING` | build the executable (needs a linker and a C/C++ runtime for the target) |
| `DREAD_LINK_GAME` | OFF | link `dread_game` into `dread` (M1) |
| `DREAD_LIBCXX_INCLUDE`, `DREAD_MUSL_INCLUDE` | on a Windows host, the libc++ 9.0.1 headers under `%LOCALAPPDATA%\dread-decomp\sources` and `lib/musl/include` when both exist (what `tools/buildcfg.py` uses); empty elsewhere | compile against these headers with `-nostdinc++` (compile only, nothing links against them). Empty means the host's own libc++ and libc, which is what a real host build uses |
| `DREAD_SWEEP_EXCLUDE` | empty | semicolon-separated CMake regexes over repository-relative paths; sources the sweep marks as not yet portable are left out of `dread_game` until fixed |
| `DREAD_GAME_VERSION` | `2.1.0` | passed to the sweep |
| `DREAD_PYTHON` | the repository's `.venv`, else `find_package(Python3)` | runs the sweep |

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
toolchain), or WSL (a Linux build, below). On a Windows target `include/types.h` must change too:
it defines `u64` as `unsigned long`, which is 32-bit there (LLP64), and its `static_assert` says
so. That LP64 assumption is the one thing in `include/` known not to port to Windows itself; the
Linux triple sidesteps it.

### Linux

```sh
cmake -S port -B build/port/cmake -G Ninja -DCMAKE_CXX_COMPILER=clang++
cmake --build build/port/cmake
./build/port/cmake/dread
```

Clang with libc++ or libstdc++; the code is C++17 with `-fno-exceptions -fno-rtti`. The matched
code uses Clang builtins and attributes throughout, so Clang is the compiler, not GCC.

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
generation flags (`-std=c++1z -O2 -fno-rtti -fno-exceptions -fPIC -c`, none of the AArch64
flags), sorts the errors into categories (the message with file, line and quoted names
stripped, so one problem in a hundred generated files is one line), counts failures per top-level
`src/` directory, and lists separately the files that fail only because they include an SDK
header (`<nn/...>`): those are the port's own declarations to write. Objects go to
`build/port/sweep/`, the report to stdout and `build/port/sweep.json`; the exit status is always 0.
`lib/nnsdk` is never on its include path; `port/runtime/include` is, unless `--no-runtime`.

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
| `nn/os/os_TickTypes.h` | `nn::os::Tick`, `SystemTickFrequency` (19.2 MHz), `GetSystemTick`, `ConvertToTimeSpan`, `ConvertToTick` | `src/game_s1/fn_7100080c40.cpp` (frame timers) |
| `nn/os.h` | `nn::os::MutexType` (32 bytes, 8-aligned: fixed by the game's object layouts), `InitializeMutex`, `FinalizeMutex`, `LockMutex`, `UnlockMutex`, opaque `ThreadType`, `GetCurrentThread` | `include/base/UnkDefaultAllocator.h` and its seven includers |
| `nn/oe.h` | `PerformanceMode`, `PerformanceConfiguration`, `Initialize`, `SetPerformanceConfiguration`, `GetProgramTotalActiveTime` | `src/game_s7/fn_71001587f0.cpp`, `src/game_s1/fn_7100080c40.cpp` |
| `nn/vi.h` | opaque `Display` and `Layer`, `Initialize`, `OpenDefaultDisplay`, `CreateLayer`, `GetNativeWindow` | `src/game_s7/fn_71001587f0.cpp` (graphics start-up) |
| `nn/fs.h`, `nn/hid.h`, `nn/audio.h`, `nn/time.h`, `nn/account.h`, `nn/prepo.h`, `nn/nfp.h`, `nn/diag.h`, `nn/mem.h` | nothing yet: each lists `main`'s imports from that namespace and what the boot path needs | M1 onwards |

What the sweep cannot see, and M1 inherits: the objects are compiled, not linked. Every
`fn_<addr>` callee without a source, every `g_unk_<addr>` data symbol and vtable (`.data` and
`.rodata` come from the user's dump at build time), and functions defined in more than one file
(`tools/check.py` reports those) are the next list. The CMake build on the Windows PC configures
and builds `dread_runtime` and `dread_game`; it cannot produce a Windows executable until a linker
and a C runtime are installed (above).
