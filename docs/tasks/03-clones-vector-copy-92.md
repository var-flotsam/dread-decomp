# 03 Clone template: the 92-byte memcpy family

## Goal

Match one small family of repeated template copies and add it to `tools/gen_clones.py`.
The copies differ only in an element size (2, 4, 8 or 16 bytes), so one template with a sized
element type covers them all.

Measured family: 25 functions of 92 bytes (23 instructions), 2,300 bytes, all `U`. It is the
largest family under 200 bytes in `tools/shape_census.py --max-size 200` except
`0x71002b9908` (`UnkVector::EraseAt`, known hard: skip it).

## Files you may edit

- `include/base/UnkCloneHelpers.h` (add one template and its comment)
- `tools/gen_clones.py` (add one entry to `KINDS`)
- `build/scratch/` (your test sources; gitignored)
- `data/functions/*.csv` (only through `verify.py`)

Do not edit `include/base/UnkVector.h` (shared core type). If the template must be an
`UnkVector` member, stop and write a note for the owner.

## Example address

`0x71000d15fc`. What is known:
- One call, to `memcpy`. A size compare with an early exit (`cmp` + `b.ls`) comes first.
- The varying operands are the element-size shifts: `lsl x10, x8, #3` with a mask, and
  `lsl x2, x8, #3` for the `memcpy` length (16 copies with `#3`, 6 with `#2`, 2 with `#4`,
  1 with `#1`), and the source offset `add x1, x0, #8`.
- `gen_clones.elem_size()` reads `lsl x1, x8, #k`, not `lsl x2`. You need your own reader
  for this family (a small function next to `elem_size`, or a parameter).
- Element types for sized trivially copyable data: `UnkCloneElem<Tag, Size>` (`include/UnkClone.h`).

## Steps

1. Confirm the family and its varying operands:
   ```sh
   $PY tools/shape_census.py --version 2.1.0 --max-size 200 --top 3
   $PY tools/shape_census.py --version 2.1.0 --family 71000d15fc
   ```
2. See the original next to a stub: `build/scratch/c92.cpp` with `void fn_71000d15fc() {}`:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/c92.cpp -v | head -40
   ```
3. Write a plain `fn_71000d15fc` first, then the template in `UnkCloneHelpers.h`, instantiated
   with `UnkCloneElem<UnkClone_fn_71000d15fc, 8>`. Get `MATCH` for the example, then for one
   copy with element size 4 (find one with `--family`; the census prints counts, use
   `gen_clones.instructions()` in a one-off script to pick an address, printing only addresses).
4. Add a `KINDS` entry like the existing ones: reference `0x71000D15FC`, header
   `base/UnkCloneHelpers.h`, `exact=False` (shapes vary by the shift), and an `instantiate`
   that reads the element size and returns `None` when it cannot.
5. Run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen gen_clones
   ```

## Done when

- `verify.py` prints `RESULT: PASS`; O went up by close to 25.
- Committed: `Phase 4.3: gen_clones <kind> (<N> functions)`.

## Next family

Rerun `--max-size 200 --top 12` and take the next family (for example `0x71000de484`,
20 × 112 B, where only one load/store width varies). Same steps, same time-box.

## Time-box

60 minutes per family.
