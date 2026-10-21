"""Project build configuration shared by every tool that compiles code.

Code generation flags are the ones identified in docs/compiler.md. System headers:
  - libc++ 9.0.1 headers (tools/fetch_sources.py; matches the project compiler)
  - musl (lib/musl, open-ead/botw-lib-musl) for the C library headers
  - NintendoSDK declarations from lib/nnsdk (open-ead/nnsdk, community-written)
The target stays aarch64-none-elf (the triple all matching results were obtained with);
libc++ is told about musl and pthreads explicitly instead.
"""

from __future__ import annotations

from pathlib import Path

import fetch_sources
import fetch_toolchains

ROOT = Path(__file__).resolve().parent.parent
COMPILER = "9.0.1"

CODEGEN = ["--target=aarch64-none-elf", "-mcpu=cortex-a57", "-O3", "-fPIC",
           "-fasynchronous-unwind-tables", "-mno-implicit-float", "-ffunction-sections"]
CXX = ["-std=c++1z", "-fno-rtti", "-fno-exceptions"]


def libcxx_include() -> Path:
    return fetch_sources.source_dir("libcxx-9.0.1.src") / "include"


def system_includes() -> list[str]:
    return ["-nostdinc++", "-isystem", str(libcxx_include()), "-isystem", str(ROOT / "lib" / "musl" / "include"),
            "-D_LIBCPP_HAS_MUSL_LIBC", "-D_LIBCPP_HAS_THREAD_API_PTHREAD"]


def project_includes() -> list[str]:
    return ["-I", str(ROOT / "include"), "-I", str(ROOT / "lib" / "nnsdk" / "include"), "-DSWITCH", "-DNNSDK"]


# The game's Lua objects have no type-based alias analysis (every field store may alias a reload);
# found by tools/sigmatch.py: lcode, llex and lparser match completely only with this flag.
LUA_FLAGS = ["-fno-strict-aliasing"]


def cxx_command(src: Path, obj: Path, extra: list[str] | None = None) -> list[str]:
    """Compile a C++ source (or a C source as C++, as the game builds Lua)."""
    lang = ["-x", "c++"] if src.suffix == ".c" else []
    return [str(fetch_toolchains.clang_path(COMPILER)), *CODEGEN, *CXX, *system_includes(),
            *project_includes(), *(extra or []), *lang, "-c", str(src), "-o", str(obj)]
