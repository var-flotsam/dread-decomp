// CCaterzillaSpawnPointComponent unit (tools/tu_map.py): resets `count` segments of a chain (next at
// +0x420), each taking its rest value (+0x3c8) as current and previous and clearing its state; when the
// chain ends early, the last segment is marked as the tail. Names and layouts are placeholders.
#include "types.h"

struct UnkCaterzillaSegment {
    u8 pad_0x0[0x3c0];
    u32 field_0x3c0;
    u32 field_0x3c4;
    u32 field_0x3c8;
    u32 field_0x3cc;
    bool field_0x3d0;
    u8 pad_0x3d1[7];
    u32 field_0x3d8;
    bool field_0x3dc;
    u8 pad_0x3dd;
    bool field_0x3de;
    u8 pad_0x3df[0x420 - 0x3df];
    UnkCaterzillaSegment* next;
    u8 pad_0x428[4];
    u32 field_0x42c;
    bool field_0x430;
};
static_assert(__builtin_offsetof(UnkCaterzillaSegment, field_0x430) == 0x430, "segment layout");

void fn_7100a1782c(UnkCaterzillaSegment* self, s32 count) {
    if (count < 1)
        return;
    u32 rest = self->field_0x3c8;
    self->field_0x3c4 = rest;
    self->field_0x3c0 = rest;
    self->field_0x3cc = 0;
    self->field_0x3d0 = false;
    *reinterpret_cast<u64*>(&self->field_0x3d8) = 0;
    self->field_0x42c = 0;
    self->field_0x430 = false;
    if (self->next) {
        fn_7100a1782c(self->next, count - 1);
    } else {
        self->field_0x3de = true;
        self->field_0x3d0 = true;
        self->field_0x3dc = true;
    }
}
