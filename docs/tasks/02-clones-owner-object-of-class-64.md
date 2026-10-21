# 02 Clone template: owner's object is of a class (64 bytes)

## Goal

Add one template to `include/base/UnkCloneHelpers.h` and one kind to `tools/gen_clones.py`
for a family of 16 copies of the same 64-byte function (1,024 bytes, all `U`). Each copy
reads an owner pointer from `self`, reads an object at owner `+0x220`, and returns `false`
if it is null, else the result of `fn_710117b348(object, <class getter>())`.

Copies differ only in: the owner offset in `self` (`0x30` in 15 copies, `0x8` in 1) and the
class getter called (one per copy).

## Files you may edit

- `include/base/UnkCloneHelpers.h` (add the template and its comment)
- `tools/gen_clones.py` (add one `KINDS` entry and a small `instantiate` function)
- `build/scratch/` (test sources; gitignored)
- `data/functions/*.csv` (only through `verify.py`)

## Example addresses

`0x7100551548` (owner offset `0x30`, class getter `0x7100c50848`) and `0x71006869e8`
(owner offset `0x8`, class getter `0x7100c0ccc8`). This source matches both, verified with
`trymatch.py` (`2/2 functions match`):

```cpp
#include "base/reflection/CClass.h"
using base::reflection::CClass;

template <class G>
struct UnkTraits;

template <class G>
bool refl_HasOwnerObjectOfClass(void* self) {
    char* owner = *reinterpret_cast<char**>(static_cast<char*>(self) + UnkTraits<G>::owner_offset);
    void* obj = *reinterpret_cast<void**>(owner + 0x220);
    if (!obj)
        return false;
    return UnkTraits<G>::is_of(obj, UnkTraits<G>::type());
}

const CClass* fn_7100c50848();
bool fn_710117b348(void* obj, const CClass* cls);
struct UnkClone_fn_7100551548;
template <>
struct UnkTraits<UnkClone_fn_7100551548> {
    static constexpr u32 owner_offset = 0x30;
    static const CClass* type() { return fn_7100c50848(); }
    static bool is_of(void* o, const CClass* c) { return fn_710117b348(o, c); }
};
template bool refl_HasOwnerObjectOfClass<UnkClone_fn_7100551548>(void*);
```

## Steps

1. Confirm the family (counts only):
   ```sh
   $PY tools/shape_census.py --version 2.1.0 --family 7100551548 | head -4
   ```
2. Put the source above in `build/scratch/c64.cpp`; expect `1/1 functions match`:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/c64.cpp | tail -2
   ```
3. Move the template into `UnkCloneHelpers.h`, with the tag parameter named `Tag` as in the
   rest of that file. Rename `UnkTraits` to `UnkCloneOwnerClassTraits` and declare it in the
   same header (per-tag traits structs like `UnkCloneDtorTraits` in `include/UnkClone.h` are
   the model). Keep the `0x220` constant in the template; it does not vary. Rerun step 2
   against the header to confirm it still matches.
4. In `tools/gen_clones.py` add a `KINDS` entry: reference `0x7100551548`, header
   `"base/UnkCloneHelpers.h"`, `exact=False`. Its `instantiate(tag, ins)` reads from the
   instructions (pairs of mnemonic and operands):
   - instruction 3, `ldr x8, [x0, #<off>]` or `ldr x8, [x0]`: the owner offset;
   - instruction 6, `bl #<addr>`: the class getter.
   It returns the declarations, the traits specialisation and the explicit instantiation as
   one string, or `None` if either operand is not in that form. Look at a nearby kind that
   reads a `bl` target from `ins` and copy its pattern.
5. Run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen gen_clones
   ```

## Done when

- `verify.py` prints `RESULT: PASS`; O went up by about 16.
- `git status --short` lists only the two files above, `src/clones/` and `data/functions/`.
- Committed: `Phase 4.3: gen_clones owner object of class (<N> functions)`.

## Time-box

45 minutes.
