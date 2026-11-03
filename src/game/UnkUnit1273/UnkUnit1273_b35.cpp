// A function in an unnamed unit (tu_map unit 1273, 0x710080f9e0-0x710080fc8c): equality of two keyed
// values. Both need the same kind (+0x14); kind 1 compares the ids at +0x10, kind 0 also the pointers
// at +8, and other kinds never compare equal. Names and layouts are placeholders.
#include "types.h"

// Named as src/game/CMaterialFXComponent/fn_7100926874.cpp declares the callee (the id at +8 is a
// CStrId there).
struct UnkKey {
    u64 field_0x0;
    void* field_0x8;
    u32 field_0x10;
    u32 kind;
};

bool fn_710080f9e0(const UnkKey* a, const UnkKey* b) {
    if (a->kind != b->kind)
        return false;
    if (a->kind != 1) {
        if (a->kind != 0 || a->field_0x8 != b->field_0x8)
            return false;
    }
    return a->field_0x10 == b->field_0x10;
}
