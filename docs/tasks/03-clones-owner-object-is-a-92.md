# 03 Clone template: act on the owner's object if it is a class (92 bytes)

## Goal

Add one template (two entry forms) to `include/base/UnkCloneHelpers.h` and one kind to
`tools/gen_clones.py` for a family of 14 copies of the same 92-byte function (1,288 bytes,
all `U`). Each copy reads an owner pointer, reads an object at an owner offset, returns if it
is null, asks `fn_710009d50c(object->vf_0x0(), <class getter>())` (is-a), and if true
tail-calls an action on the object. Do card `02` first: this one uses the same approach.

Copies differ in:
- where the owner comes from: `[x0, #8]` (3 copies: one argument) or `[x1]` (11 copies: the
  object is the second argument, the first is unused);
- the object offset in the owner: `0x240` (13 copies) or `0x1b8` (1);
- the class getter and the action (one each per copy).

## Files you may edit

- `include/base/UnkCloneHelpers.h` (add the templates and their comment)
- `tools/gen_clones.py` (add one `KINDS` entry and a small `instantiate` function)
- `build/scratch/` (test sources; gitignored)
- `data/functions/*.csv` (only through `verify.py`)

## Example addresses

Verified with `trymatch.py` (`3/3 functions match`): `0x7100427fb4` (one argument, owner
offset `0x8`, object offset `0x240`, getter `0x710044e708`, action `0x7100453dcc`),
`0x7100b97a4c` (second argument, owner offset `0`, `0x240`, `0x71003f6484`, `0x71003f6d90`) and
`0x7100b9f49c` (second argument, `0`, `0x1b8`, `0x71008bd0f4`, `0x71008be4e0`).

```cpp
#include "base/reflection/CClass.h"
using base::reflection::CClass;
using base::reflection::UnkReflObject;

template <class G>
struct UnkTraits;

template <class G>
void refl_IfOwnerObjectIsA(void* self) {
    char* owner = *reinterpret_cast<char**>(static_cast<char*>(self) + UnkTraits<G>::owner_offset);
    UnkReflObject* obj = *reinterpret_cast<UnkReflObject**>(owner + UnkTraits<G>::object_offset);
    if (!obj)
        return;
    if (UnkTraits<G>::is_a(obj->vf_0x0(), UnkTraits<G>::type()))
        UnkTraits<G>::action(obj);
}

// The same with the object as the second argument (the first is unused).
template <class G>
void refl_IfOwnerObjectIsA1(void*, void* self) {
    refl_IfOwnerObjectIsA<G>(self);
}

const CClass* fn_71003f6484();
bool fn_710009d50c(const CClass* cls, const CClass* base);
void fn_71003f6d90(UnkReflObject* obj);
struct UnkClone_fn_7100b97a4c;
template <>
struct UnkTraits<UnkClone_fn_7100b97a4c> {
    static constexpr u32 owner_offset = 0x0;
    static constexpr u32 object_offset = 0x240;
    static const CClass* type() { return fn_71003f6484(); }
    static bool is_a(const CClass* c, const CClass* b) { return fn_710009d50c(c, b); }
    static void action(UnkReflObject* o) { fn_71003f6d90(o); }
};
template void refl_IfOwnerObjectIsA1<UnkClone_fn_7100b97a4c>(void*, void*);
```

For a one-argument copy, instantiate `refl_IfOwnerObjectIsA<Tag>(void*)` with
`owner_offset = 0x8` instead.

## Steps

1. Confirm the family (counts only):
   ```sh
   $PY tools/shape_census.py --version 2.1.0 --family 7100427fb4 | head -6
   ```
2. Put the source above in `build/scratch/c92.cpp`; expect `1/1 functions match`. Add the
   `0x7100427fb4` instantiation and expect `2/2`:
   ```sh
   $PY tools/trymatch.py --version 2.1.0 build/scratch/c92.cpp | tail -3
   ```
3. Move both templates into `UnkCloneHelpers.h` (tag parameter `Tag`; traits struct
   `UnkCloneOwnerIsATraits`, declared in the same header). Rerun step 2 against the header.
4. In `tools/gen_clones.py` add a `KINDS` entry: reference `0x7100427fb4`, header
   `"base/UnkCloneHelpers.h"`, `exact=False`. Its `instantiate(tag, ins)` reads:
   - instruction 3: `ldr x8, [x0, #<off>]` (one argument) or `ldr x8, [x1]` (second argument);
   - instruction 4: `ldr x19, [x8, #<off>]`: the object offset;
   - instruction 11: `bl #<addr>`: the class getter;
   - instruction 19: `b #<addr>`: the action.
   It picks `refl_IfOwnerObjectIsA` or `refl_IfOwnerObjectIsA1` from instruction 3 and
   returns `None` for any other form.
5. Run the gate:
   ```sh
   $PY tools/verify.py --version 2.1.0 --regen gen_clones
   ```

## Done when

- `verify.py` prints `RESULT: PASS`; O went up by about 14.
- Committed: `Phase 4.3: gen_clones owner object is-a action (<N> functions)`.

## Time-box

45 minutes.
