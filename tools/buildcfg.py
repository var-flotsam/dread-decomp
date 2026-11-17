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


# Some of the game's units were built without type-based alias analysis too: their code reloads a field
# after a store of another type. A source of such a unit says so in its first lines with
# `// build-flags: no-strict-aliasing`; data/unit_flags.csv lists the units and the evidence for each.
SOURCE_FLAGS = {"no-strict-aliasing": ["-fno-strict-aliasing"]}
SOURCE_FLAGS_LINES = 40


def source_flags(src: Path) -> list[str]:
    """The flags a source's `// build-flags:` line asks for (unknown names are an error)."""
    try:
        with open(src, encoding="utf-8", errors="replace") as f:
            head = [next(f, "") for _ in range(SOURCE_FLAGS_LINES)]
    except OSError:
        return []
    out: list[str] = []
    for line in head:
        line = line.strip()
        if line.startswith("// build-flags:"):
            for name in line.split(":", 1)[1].split():
                if name not in SOURCE_FLAGS:
                    raise ValueError(f"{src}: unknown build flag {name!r} (known: {', '.join(SOURCE_FLAGS)})")
                out += SOURCE_FLAGS[name]
    return out


def cxx_command(src: Path, obj: Path, extra: list[str] | None = None) -> list[str]:
    """Compile a C++ source (or a C source as C++, as the game builds Lua), with its `// build-flags:`."""
    lang = ["-x", "c++"] if src.suffix == ".c" else []
    return [str(fetch_toolchains.clang_path(COMPILER)), *CODEGEN, *CXX, *system_includes(),
            *project_includes(), *source_flags(src), *(extra or []), *lang, "-c", str(src), "-o", str(obj)]
