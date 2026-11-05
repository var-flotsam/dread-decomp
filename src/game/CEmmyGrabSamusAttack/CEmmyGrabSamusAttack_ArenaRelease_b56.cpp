// The release of an 8-byte inline arena in the CEmmyGrabSamusAttack unit, the same code as
// src/base/UnkUnit25/UnkInlineArena_b42.cpp's Release (fn_710009fef0): when the nesting count drops
// to zero, the cursor rewinds to the storage start if the pointer lies below it. Names and layout
// are placeholders.
#include "types.h"

namespace {

struct UnkInlineArena8_b56a3 {
    u64 field_0x0;
    u8 storage[8];
    u8* cursor;
    u32 depth;
};

}  // namespace

struct UnkInlineArena8Ref_b56a3;

void fn_7100510dd4(UnkInlineArena8Ref_b56a3* ref, u8* p) {
    UnkInlineArena8_b56a3* self = reinterpret_cast<UnkInlineArena8_b56a3*>(ref);
    u32 d = self->depth;
    if (d == 1 && self->storage <= p && p < self->cursor)
        self->cursor = self->storage;
    self->depth = d - 1;
}
