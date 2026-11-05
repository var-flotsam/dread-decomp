// CCaterzillaSpawnPointComponent unit (tools/tu_map.py): one step of a chain segment's counters (the
// segment layout of CCaterzillaSpawnPointComponent_b47.cpp; next at +0x420). Bit 0 of the flags counts
// one up at +0x3c0; a pending count at +0x3c4 (and its mirror at +0x3cc) counts down together with the
// next segment's, and when it runs out the head/tail marks (+0x3dd, +0x3de, +0x430) move along the
// chain. Names and layouts are placeholders.
#include "types.h"

struct UnkCaterzillaSegment_b52 {
    u8 pad_0x0[0x3c0];
    u32 field_0x3c0;
    u32 field_0x3c4;
    u32 field_0x3c8;
    u32 field_0x3cc;
    u8 pad_0x3d0[0xd];
    u8 field_0x3dd;
    u8 field_0x3de;
    u8 pad_0x3df[0x420 - 0x3df];
    // A union member: no type-based alias info, so the original reloads it after every byte store.
    union {
        UnkCaterzillaSegment_b52* next;
    };
    u8 pad_0x428[8];
    u8 field_0x430;
};
static_assert(__builtin_offsetof(UnkCaterzillaSegment_b52, field_0x430) == 0x430, "segment layout");

static inline __attribute__((always_inline)) void CountDown_b52(UnkCaterzillaSegment_b52* self) {
    if (self->field_0x3c0 == 0)
        return;
    --self->field_0x3c0;
    if (self->next && self->next->field_0x3c0)
        --self->next->field_0x3c0;
    if (self->field_0x3c0)
        return;
    if (self->next) {
        if (self->field_0x3de) {
            self->field_0x430 = 0;
            self->field_0x3de = 0;
            self->next->field_0x3de = 1;
            self->next->field_0x3dd = 1;
        }
        return;
    }
    if (!self->field_0x3dd && self->field_0x3cc == self->field_0x3c4)
        self->field_0x3dd = 1;
}

void fn_7100a16abc(UnkCaterzillaSegment_b52* self, u32 flags) {
    if (flags & 1)
        ++self->field_0x3c0;
    if (self->next && self->field_0x3c4) {
        --self->field_0x3c4;
        --self->field_0x3cc;
        if (self->next->field_0x3c4)
            --self->next->field_0x3c4;
        if (self->field_0x3c4 == 0) {
            if (self->next->field_0x3dd && !self->field_0x3de) {
                self->field_0x3de = 1;
                self->next->field_0x3de = 0;
                self->next->field_0x3dd = 0;
                self->next->field_0x430 = 0;
            }
            self->field_0x430 = 1;
            return;
        }
    }
    CountDown_b52(self);
}
