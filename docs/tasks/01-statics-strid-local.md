# 01 Guarded statics: local static string ids

## Goal

Add a kind `strid_local` to `tools/gen_statics.py` for an ordinary (non-template) function
that builds a string id once from a literal and returns it. The guard and the string id are
local (reached with `adrp` + `add`), not through the GOT like the existing `strid` kind.

Measured family: 13 functions of 128 bytes (32 instructions), 1,664 bytes. The lifter rejects
them (`insn: ldarb`).

## Files you may edit

- `tools/gen_statics.py`
- `data/functions/*.csv` (only through `verify.py`)
- `src/reflection_statics/` is rewritten by the generator; never edit it.

## Example address

`0x7100304140`, name string `"EnergyWave"`. This source is verified to match byte for byte:

```cpp
#include "base/reflection/UnkReflStatics.h"
const base::global::CStrId& fn_7100304140() {
    static base::global::CStrId s_id("EnergyWave", true);
    return s_id;
}
```

## Steps

1. See the family (counts and varying operands only):
   ```sh
   $PY tools/shape_census.py --version 2.1.0 --family 7100304140 | head -3
   ```
2. Put the source above in `build/scratch/strid_local.cpp` and confirm it:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/strid_local.cpp | tail -2
   ```
   Expect `1/1 functions match`.
3. In `tools/gen_statics.py`:
   - Add `"strid_local": 0x7100304140,` to `REFERENCES`, with a one-line comment.
   - `facts()` already finds `name` for this shape (the literal passed to the string-id
     constructor). Do not change `facts()`.
   - In `emit()`, before the other kinds, return a plain function for `kind == "strid_local"`
     (no tag, no template). Use `refl_attrs.c_string(f["name"])` for the literal. Return
     `None` when `"name"` is missing. Keep the line format of the other kinds.
   - Update the module docstring's list of kinds in a few words.
4. Run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen gen_statics
   ```
   The `gen_statics` line shows `candidates: {...}` with a count for `strid_local`.

## Done when

- `verify.py` prints `RESULT: PASS` and O went up (about +13).
- `git status --short` lists only `tools/gen_statics.py`, `src/reflection_statics/` and
  `data/functions/` files.
- Committed: `Phase 4.3: gen_statics local static string ids (<N> functions)`.

## Next (same card, if time is left)

Other sizes of the same idea may exist. Count `U` functions that call the string-id
constructor `0x71000003d4` and have one `ldarb`; do not list them, just count. Note the count
in `docs/tasks/notes/01-statics-strid-local.md`.

## Time-box

45 minutes.
