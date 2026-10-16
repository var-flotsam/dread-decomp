# Compiler identification

## Summary

**Game code: upstream Clang/LLVM 9.0.x with**

```sh
clang++ --target=aarch64-none-elf -mcpu=cortex-a57 -O3 -fPIC \
        -fasynchronous-unwind-tables -mno-implicit-float \
        -std=c++17 -fno-rtti -fno-exceptions -ffunction-sections -c foo.cpp
```

| Sample | Size | Exact matches | Instructions matching |
|---|---|---|---|
| Leaf functions (`src/compiler_id/`) | 10 | 7 | 88.8% |
| Non-leaf game functions (`src/flag_id/`) | 20 | **18** | **99.2%** |
| `nn::atk` virtual functions (`src/atk_id/`) | 6 | 5 | 96.1% |
| **Upstream Lua 5.1.4** library functions, built from unmodified source with these flags | 82 | **81** | 98.8% |

The only Lua mismatch is `luaB_print`, which MercurySteam replaced. Nintendo's SDK compiler is
not public, and this project's rules forbid obtaining it. Like the BotW decomp, we use an upstream
release. 9.0.0 and 9.0.1 produce identical code on every sample; `tools/check.py` uses 9.0.1.
`-ffunction-sections` is ours (it keeps one function per section for diffing); the original's
section layout is not observable after linking.

## Evidence for each flag

| Flag | Evidence |
|---|---|
| Clang 9.0.x | Only 9.0.x matches all version-discriminating functions. With the final flags on the non-leaf set: 9.0.x 18/20, 8.0.1 11, 4.0.1 5, 10.0.0 and 11.1.0 2, 12.0.1 0 |
| `-mcpu=cortex-a57` | Changes scheduling and instruction selection; without it the leaf set drops from 7/10 to 4/10 |
| `-O3` | `-O3`'s aggressive tail duplication copies the epilogue into each return path (as in the original); `-O2` branches to one epilogue. Non-leaf 18 vs 16, Lua 81 vs 66 |
| `-fPIC` (`-fPIE` is equivalent here) | 29,098 `adrp`+`ldr` loads go through 10,660 GOT slots; vtables and global objects are reached via the GOT. Non-leaf 3/20 to 7/20 with PIC alone; 12/20 to 17/20 alongside unwind tables and no vectorisation |
| `-fasynchronous-unwind-tables` | Every function has `.eh_frame`. The CFI directives keep frame setup together at the start of each function, as in the original (non-leaf 7 to 15 with PIC; Lua 16 to 81). `aarch64-none-elf` doesn't emit them by default |
| `-mno-implicit-float` | Game code has no vector FP arithmetic, no `q`-register struct copies and no `movi` zeroing in 13 MB. This flag stops all three (both vectorisers refuse to run under it). `-fno-vectorize -fno-slp-vectorize` alone leaves `q`-register copies. `-mstrict-align` alone still zeroes with `movi`+`stp q`. Non-leaf 17 to 18 |
| `-fno-rtti`, `-fno-exceptions` | No `type_info` for game classes; no exception-runtime imports |
| Frame pointer kept | 39,285 of 42,106 non-leaf functions set `x29`; `-fomit-frame-pointer` drops the non-leaf set to 2/20 |
| No `-fdata-sections` | Statics are addressed relative to each other (reflection getter: descriptor at guard + 0x18) |
| Strict aliasing (default) | With `-fno-strict-aliasing` all of `src/` still builds, but `fn_71005a998c` (flag_id) stops matching. Keep the default |
| `-fmath-errno` (default) | `sqrtf` is inlined as `fsqrt` with a fallback call when the result is NaN |
| C++ for Lua | Lua compiled as C++ matches 81/82; as C 63/82 (the `float` overloads of `<cmath>` matter) |

## Libraries

- **`nn::atk`** (NintendoWare 11.4.3, shipped precompiled by Nintendo) is consistent with the same
  toolchain. 9.0.x and 10.0.0 both reach 5/6; 4.0.1–8.0.1 reach 3/6. The one non-match is a
  10-byte struct copy whose order no tested compiler reproduces.
- **Lua 5.1.4** with `lua_Number = float` (patched `luaconf.h`), compiled as C++ with the game
  flags: 81 of 82 standard-library functions are byte-identical to upstream. Earlier 5.1.x releases
  match fewer (5.1.5: 80, 5.1.3: 77, 5.1.2: 73, 5.1 and 5.1.1: 70). `luaB_print` was replaced by the
  game. Run `tools/libcheck_lua.py`.
- **Dear ImGui:** only one 1,232-byte function at `0x7100001fb8` (it references `imgui.ini`,
  `imgui_log.txt` and `##Overlay`) and a few neighbours survive linking. The version is not
  identified; low priority.

## Method

1. `tools/fetch_toolchains.py` downloads upstream LLVM 4.0.1, 6.0.1, 7.0.1, 8.0.1, 9.0.0, 9.0.1,
   10.0.0, 11.1.0 and 12.0.1 (SHA-256 pinned in `data/toolchains.json`).
2. Our own C++ reconstructions in `src/compiler_id/` (10 leaf), `src/flag_id/` (20 calling
   functions: constructors, deleting-destructor shape, copy assignment, virtual calls, loops with
   calls, GOT globals, string literals, the `sqrtf` errno path, struct return through `x8`, and the
   `CActor` reflection getter) and `src/atk_id/` (6 `nn::atk` functions).
3. `tools/compiler_matrix.py --set leaf|nonleaf|atk` compiles each set under every compiler and
   flag combination and scores it with `tools/asmdiff.py` (relocated fields masked).
4. The binary itself was checked for each candidate flag's fingerprint (GOT use, frame records,
   vector instructions, zeroing idioms) before adopting it.

Full matrices: `build/compiler_matrix/2.1.0/<set>/results.json`, `build/libcheck/lua/2.1.0/results.json`.

## Remaining differences (final flags)

| Function | Status | Difference |
|---|---|---|
| `710083ecf8` | `M` | original uses `fmin` and a NaN-inclusive `b.lt`; Clang emits `fminnm` and `b.mi` (differs only for NaN) |
| `71001243ac`, `710086c89c` | `m` | same instructions; scheduling and register assignment |
| `710011ec5c` | `m` | one temporary in `w8` instead of `w9` |
| `7100227448` (`CActor` reflection getter) | `m` | descriptor at guard + 0x8 instead of + 0x18; depends on the other statics in the real translation unit, which Phase 4 must reproduce |
| `7101215464` (`nn::atk`) | `m` | 10-byte struct copy order |

Several sources were adjusted after seeing the original, to restore source shape (an unsigned
index, a 64-bit down-counter, operand order, a 16-byte sub-struct, a forwarded argument, a
truncating cast). Those changes were checked against all candidate compilers, and the version
ranking didn't change.

## History

The first pass (10 leaf functions only) chose `-O2` and no PIC, because leaf functions can't
tell `-O2` from `-O3` and never touch the GOT. The non-leaf pass corrected that. Treat flags inferred
from leaf functions alone as provisional.
