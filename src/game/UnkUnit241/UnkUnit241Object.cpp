// From unit 241 of tools/tu_map.py (0x71001b3ebc-0x71001b49dc, no reflected class): the constructor
// of an object whose +0x10 points back at itself, with a sphere at +0x20 (game/UnkSphere.h,
// fn_710021685c), an array at +0x80 and the value 30 at +0xc0; the same shape as unit 254's
// (src/game/UnkUnit254/fn_71001c33cc.cpp). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

void fn_710021685c(void* self);  // UnkSphere's constructor

struct UnkUnit241Object {
    u32 field_0x0;
    u32 field_0x4;
    s32 field_0x8;
    u8 pad_0xc[4];
    UnkUnit241Object* self;  // 0x10
    u32 field_0x18;
    u8 pad_0x1c[4];
    u8 sphere[0x50];  // 0x20
    bool field_0x70;
    u8 pad_0x71[7];
    u64 field_0x78;
    base::UnkVector<void*> items;  // 0x80
    u32 field_0xa8;
    u8 pad_0xac[4];
    u64 field_0xb0;
    u64 field_0xb8;
    f32 field_0xc0;
    u32 field_0xc4;
    u32 field_0xc8;
};
static_assert(__builtin_offsetof(UnkUnit241Object, field_0xc8) == 0xc8, "UnkUnit241Object layout");

void fn_71001b3ebc(UnkUnit241Object* self) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->self = nullptr;
    fn_710021685c(self->sphere);
    self->field_0x70 = false;
    self->field_0x78 = 0;
    new (&self->items) base::UnkVector<void*>(0xf);
    self->field_0xa8 = 0;
    self->field_0xb0 = 0;
    self->field_0xb8 = 0;
    self->field_0xc0 = 30.0f;
    self->field_0xc4 = 0;
    self->field_0xc8 = 0;
    self->field_0x0 = 4;
    self->field_0x4 = 8;
    self->self = self;
}
