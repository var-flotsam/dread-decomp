// tools/tu_map.py unit 259 (no class hint): clears the counts of a large owner (fn_71001d0ae4):
// eight 64 KiB buffers (stride 0x10008, counts from +0xa0128), three more counts and two words near
// the start. Names and layouts are placeholders.
#include "types.h"

struct UnkBuffer259_b67a1 {
    u32 count;
    u8 pad_0x4[0x4];
    u8 data[0x10000];
};

struct UnkOwner259Clear_b67a1 {
    u8 pad_0x0[0xf8];
    u32 field_0xf8;
    u8 pad_0xfc[0x24];
    u32 field_0x120;
    u8 pad_0x124[0xa0128 - 0x124];
    UnkBuffer259_b67a1 buffers[8];  // 0xa0128
    u8 pad_0x120168[0x1948168 - 0x120168];
    u32 field_0x1948168;
    u8 pad_0x194816c[0x3178170 - 0x194816c];
    u32 field_0x3178170;
    u8 pad_0x3178174[0x317a978 - 0x3178174];
    u32 field_0x317a978;
};
static_assert(__builtin_offsetof(UnkOwner259Clear_b67a1, field_0x317a978) == 0x317a978, "layout");

void fn_71001d0ae4(UnkOwner259Clear_b67a1* self) {
    for (int i = 0; i < 4; ++i)
        self->buffers[i].count = 0;
    self->field_0x1948168 = 0;
    self->field_0x3178170 = 0;
    self->field_0x317a978 = 0;
    for (int i = 4; i < 8; ++i)
        self->buffers[i].count = 0;
    self->field_0xf8 = 0;
    self->field_0x120 = 0;
}
