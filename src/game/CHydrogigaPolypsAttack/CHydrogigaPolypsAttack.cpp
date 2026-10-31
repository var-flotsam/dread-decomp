// A routine of CHydrogigaPolypsAttack (fn_71005b548c): after fn_71005b2c58 it takes a timer from
// its settings at +0x60 (the value at +0xf0 less one), picks a count in [+0xf4, +0xf8) at random
// (the lower bound when the range is empty) and resets the index at +0x70. Names and layouts are
// placeholders.
#include "types.h"

extern "C" int rand();

void fn_71005b2c58(void* self);

struct UnkPolypsSettings {
    u8 pad_0x0[0xf0];
    f32 field_0xf0;
    s32 field_0xf4;
    s32 field_0xf8;
};

struct UnkPolypsAttack {
    u8 pad_0x0[0x60];
    UnkPolypsSettings* field_0x60;
    f32 field_0x68;
    s32 field_0x6c;
    s32 field_0x70;
};

void fn_71005b548c(UnkPolypsAttack* self) {
    fn_71005b2c58(self);
    UnkPolypsSettings* settings = self->field_0x60;
    self->field_0x68 = settings->field_0xf0 - 1.0f;
    s32 min = settings->field_0xf4;
    u32 range = settings->field_0xf8 - min;
    s32 count = min;
    if (range != 0)
        count = (u32)(rand() & 0x7fff) % range + min;
    self->field_0x6c = count;
    self->field_0x70 = -1;
}
