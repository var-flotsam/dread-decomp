# Compiler identification

## Summary

**Best match: upstream Clang/LLVM 9.0.1, `-O2` (or `-O3`), `-mcpu=cortex-a57`, no fast-math.**
It reproduces 7 of 10 test functions byte-for-byte, and 88.8% of instructions across all
10. It is the only candidate that matches all three functions that discriminate between versions.

```sh
clang++ --target=aarch64-none-elf -mcpu=cortex-a57 -O2 -std=c++17 \
        -fno-rtti -fno-exceptions -ffunction-sections -c foo.cpp
```

Nintendo's own SDK compiler is not public, and these rules forbid obtaining it. Like the
BotW decomp, this project uses an upstream release. NintendoSDK 11.4.3's Clang is most likely
based on LLVM 9. The remaining differences (below) may come from Nintendo patches or from
source idioms not yet reproduced.

## Evidence from the binary

- **SDK version 11.4.3** (`SDK MW+Nintendo+NintendoSdk_nnSdk-11_4_3-Release` and matching NVN,
  libz, Atk and movie markers). See `docs/recon.md`.
- There is no `.comment` section or Clang version string; NSOs don't carry one.
- **No RTTI in game code** (only precompiled `nn::atk` has `type_info`): `-fno-rtti`.
- **No exception runtime imports** (`__cxa_throw`, personality routines): `-fno-exceptions`.
  `.eh_frame` is still present for every function, which is consistent with Clang's default
  AArch64 unwind tables.
- Leaf functions don't set up a frame record. Non-leaf frame-pointer use hasn't been tested yet.

## Method

1. `tools/fetch_toolchains.py` downloads the upstream LLVM Windows releases 4.0.1, 8.0.1,
   9.0.1, 10.0.0, 11.1.0 and 12.0.1 and pins their SHA-256 in `data/toolchains.json`. These
   span the period around SDK 11, with 4.0.1 as the BotW reference.
2. Ten small **leaf** functions (40–84 bytes, no calls or global accesses) were picked from game
   code (`0x7100100000`–`0x7100f00000`, before Lua and `nn::atk`), chosen to cover loops,
   conditional selects, float compares, bitfields, multiplies, and struct return through `x8`.
   Each is reconstructed as C++ in `src/compiler_id/fn_<addr>.cpp`.
3. `tools/compiler_matrix.py` compiles every source with 6 compilers × {`-O1`, `-O2`, `-O3`, `-Os`}
   × {plain, `-ffast-math`, `-mcpu=cortex-a57`, both} (96 configurations), and
   `tools/asmdiff.py` compares each function instruction by instruction against `main`, with
   relocated fields masked.

## Results

Instruction match per function. **M** means an exact byte match.

| Config | `71001106cc` | `7100124344` | `71001243ac` | `7100162300` | `7100273218` | `710061a210` | `7100798840` | `710083ecf8` | `710086c89c` | `7100b170bc` | Matched | Mean |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| **clang-9.0.1 -O2 a57** | **M** | **M** | 50% | **M** | **M** | **M** | **M** | 88% | 50% | **M** | **7/10** | **88.8%** |
| clang-12.0.1 -O2 a57 | **M** | **M** | 50% | **M** | **M** | 57% | **M** | 88% | 19% | **M** | 6/10 | 81.3% |
| clang-10.0.0 -O2 a57 | 9% | **M** | 50% | **M** | **M** | **M** | **M** | 88% | 50% | **M** | 6/10 | 79.7% |
| clang-11.1.0 -O2 a57 | 9% | **M** | 50% | **M** | **M** | **M** | **M** | 88% | 50% | **M** | 6/10 | 79.7% |
| clang-4.0.1 -O2 a57 | **M** | **M** | 50% | **M** | **M** | 9% | 0% | 88% | 56% | **M** | 5/10 | 70.2% |
| clang-8.0.1 -O2 a57 | **M** | **M** | 50% | **M** | **M** | 9% | 0% | 88% | 28% | **M** | 5/10 | 67.4% |
| clang-9.0.1 -O2 a57 + fast-math | 18% | **M** | 50% | **M** | **M** | **M** | **M** | 88% | 50% | **M** | 6/10 | 80.6% |
| clang-9.0.1 -Os a57 | **M** | **M** | 50% | **M** | **M** | **M** | 0% | 88% | 50% | **M** | 6/10 | 78.8% |
| clang-9.0.1 -O1 a57 | 9% | **M** | 50% | **M** | **M** | **M** | **M** | 6% | 0% | **M** | 6/10 | 66.5% |
| clang-9.0.1 -O2 (generic CPU) | 45% | **M** | 20% | 0% | **M** | **M** | 20% | 50% | 44% | **M** | 4/10 | 57.9% |

The full matrix (96 configurations) is regenerated into `build/compiler_matrix/2.1.0/results.json`.

### What discriminates

| Function | What it tests | Matches with |
|---|---|---|
| `7100798840` | float compare against constant, NaN-sensitive branch | Clang ≥ 9 (4.0.1 and 8.0.1 fail) |
| `71001106cc` | 2×2 transform returning a non-trivially-copyable vec2 through `x8` | 4.0.1, 8.0.1, 9.0.1, 12.0.1 (10 and 11 schedule it differently) |
| `710061a210` | element-wise compare loop | 9.0.1, 10.0.0, 11.1.0 (12.0.1 and older fail) |

Only **9.0.1** passes all three. Other conclusions:

- **`-mcpu=cortex-a57` is required.** The Cortex-A57 scheduling model and instruction selection
  change `7100162300`, `7100798840` and `71001106cc` (generic CPU: 4/10).
- **No `-ffast-math`.** It breaks `71001106cc` (FMA contraction and reassociation) without fixing
  anything else.
- **`-O2` and `-O3` produce identical code for all 10 leaf functions**, so leaf functions can't
  distinguish them. `-O1` and `-Os` are worse. Deciding between `-O2` and `-O3` needs functions where
  inlining or unrolling decisions differ.

## Remaining differences (with 9.0.1 -O2 a57)

| Function | Score | Difference | Likely cause |
|---|---|---|---|
| `71001243ac` | 50% | same instructions, different order and register assignment | source shape (e.g. inlined accessor or handle type) not yet reproduced; identical under every compiler |
| `710086c89c` | 50% | same instructions; pointer increment ordered before the counter decrement, registers swapped | loop shape; identical under 9, 10 and 11 |
| `710083ecf8` | 88% | original uses `fmin` (NaN-propagating) and a NaN-inclusive `b.lt`; every upstream candidate emits `fminnm` and `b.mi` | a min helper or idiom not yet found, or a Nintendo compiler patch. No tested flag reproduces both instructions together |

Three functions only matched after the source was adjusted, all while testing against 9.0.1:
an unsigned index (`7100b170bc`), a 64-bit down-counter that loads `b` before `a`
(`710061a210`), and the branch structure (`710083ecf8`). This could bias the comparison toward
9.0.1. However, the two strongest discriminators (`7100798840` and `71001106cc`) were never
changed, and the first version of `710061a210` already separated 9–11 (79%) from 12 (21%).

## Caveats and next steps

- Ten leaf functions is a small sample. The next check should be 20–30 non-leaf functions
  (calls, frame setup, inlining) to settle `-O2` versus `-O3`, frame-pointer policy, `-fPIC` and
  global-access codegen, and whether 9.0.0 differs from 9.0.1.
- If `fmin` keeps appearing where upstream emits `fminnm`, look for a common source helper
  (e.g. a `base::math` min) before assuming a compiler patch.
- Libraries compiled separately by Nintendo (`nn::atk`) may use a different toolchain. Lua and
  ImGui are presumably built with the game's flags but haven't been checked.
