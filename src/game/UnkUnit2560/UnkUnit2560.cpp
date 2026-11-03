// From unit 2560 of tools/tu_map.py (0x7100e1bc3c-0x7100e1dbbc, no reflected class): fn_7100e1bc3c tells the
// object at +0x10 (its +0x18, fn_71001f3224 with 0) and then resets each 0x190-byte element of the array at
// +0x8 (count at +0x4; fn_7100744d90). Names and layouts are placeholders.
#include "types.h"

struct UnkUnit2560Element {
    u8 data[0x190];
};

struct UnkUnit2560Owner {
    u8 pad_0x0[0x18];
    u8 field_0x18[1];
};

struct UnkUnit2560 {
    u32 pad_0x0;
    u32 count;                    // 0x4
    UnkUnit2560Element* data;     // 0x8
    UnkUnit2560Owner* owner;      // 0x10
};

void fn_71001f3224(void* object, u32 value);
void fn_7100744d90(UnkUnit2560Element* element);

void fn_7100e1bc3c(UnkUnit2560* self) {
    if (self->owner)
        fn_71001f3224(self->owner->field_0x18, 0);
    for (u64 i = 0; i < self->count; ++i)
        fn_7100744d90(&self->data[i]);
}
