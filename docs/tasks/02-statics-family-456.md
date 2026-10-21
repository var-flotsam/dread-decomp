# 02 Guarded statics: the 456-byte family

## Goal

Match the largest family of guarded-static functions not yet done, and add it to
`tools/gen_statics.py` as a new kind. This card is harder than `01`: the template is not known
yet. Do `01` first.

Measured family: 37 functions of 456 bytes (114 instructions), 16,872 bytes, all `U`. It is
the largest multi-copy family among the 2,483 `U` functions with an `ldarb` (guard check);
the lifter rejects them (`insn: ldarb`).

## Files you may edit

- `include/base/reflection/UnkReflStatics.h` (add one template and its comment)
- `tools/gen_statics.py` (add one kind)
- `build/scratch/` (your test sources; gitignored)
- `data/functions/*.csv` (only through `verify.py`)

## Example address

`0x71004f0648`. What is known (from its calls and varying operands):
- One guard (`ldarb`), acquired and released with `__cxa_guard_acquire`/`__cxa_guard_release`
  (`0x7101238da0`/`0x7101238db0`), and a destructor registered with `0x7100000250` (atexit).
- Guard and object are reached through the GOT at adjacent slots (`+0xd20`, `+0xd28` in the
  example): a weak static, so a template like the others in `UnkReflStatics.h`.
- It calls the string-pool release `0x71000803a4`, `0x7100083990` (12 bytes), two helpers
  next to it (`0x71004f1558`, `0x71004f1748`) and one virtual function (`blr`).
- Across the 37 copies only the GOT slots differ (instructions 32-36, 81-82, 100-101).
  So the code does not depend on the template argument except through its static.

## Steps

1. See which operands vary:
   ```sh
   $PY tools/shape_census.py --version 2.1.0 --family 71004f0648 | head -12
   ```
2. See the original next to an empty stub. Write `build/scratch/f456.cpp` with
   `void fn_71004f0648() {}` and run:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/f456.cpp -v | head -120
   ```
   Read it; do not save it anywhere tracked.
3. Write the function in your scratch file: first as a plain function named `fn_71004f0648`,
   declaring the callees as `fn_<addr>` with guessed signatures. Iterate with `trymatch.py`
   until the only differences are the static's GOT accesses.
4. Turn it into a template `refl_<Something><G>()` in `UnkReflStatics.h` with
   `UnkReflStatic<G>` constants, like `refl_GetStaticStrId`. Instantiate it with the tag
   `UnkRefl_fn_71004f0648` in the scratch file and get `MATCH`.
5. Add the kind: the template's comment in the header, a `REFERENCES` entry
   (`0x71004F0648`), any constants in `facts()`, and its lines in `emit()`.
6. Run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen gen_statics
   ```

## Done when

- `verify.py` prints `RESULT: PASS`; O went up by close to 37.
- Committed: `Phase 4.3: gen_statics <kind> (<N> functions)`.
- If only some copies match, that is fine; say how many in the commit message.

## If it does not match

After the time-box, keep nothing in the header or generator. Write
`docs/tasks/notes/02-statics-family-456.md`: how many instructions still differ, which
callees you identified, and the closest scratch source in your own words (a short C++ outline
is fine; no disassembly). Commit the note alone.

## Time-box

90 minutes. Stop at 3 attempts in a row with no fewer differing instructions.
