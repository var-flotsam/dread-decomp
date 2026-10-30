// From unit 254 of tools/tu_map.py (0x71001c33cc-0x71001c383c, no reflected class): the constructor of
// an object whose +0x10 points back at itself, with an embedded part at +0x20 (fn_710021685c), two
// scales of one at +0xa0 and two counts set to 0x80 and 1. Names and layouts are placeholders.
#include "types.h"

void fn_710021685c(void* part);

struct UnkUnit254Object {
    u32 field_0x0;
    u32 field_0x4;
    s32 field_0x8;
    u8 pad_0xc[4];
    UnkUnit254Object* self;  // 0x10
    u32 field_0x18;
    u8 pad_0x1c[4];
    u8 part[0x50];  // 0x20
    bool field_0x70;
    u8 pad_0x71[7];
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    f32 scale[4];  // 0xa0
    bool field_0xb0;
    u8 pad_0xb1[3];
    bool field_0xb4;
};
static_assert(__builtin_offsetof(UnkUnit254Object, field_0xb4) == 0xb4, "UnkUnit254Object layout");

// NON_MATCHING(minor): the last two pairs of zero stores come out in the other order.
void fn_71001c33cc(UnkUnit254Object* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->self = nullptr;
    fn_710021685c(self->part);
    self->field_0x98 = 0;
    self->scale[0] = 1.0f;
    self->scale[1] = 1.0f;
    self->scale[2] = 1.0f;
    self->scale[3] = 1.0f;
    self->field_0x70 = false;
    self->field_0xb4 = false;
    self->self = self;
    self->field_0x0 = 0x80;
    self->field_0x4 = 1;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
}
