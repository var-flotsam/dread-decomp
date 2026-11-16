// Unit 250 (tools/tu_map.py): the constructor of an object whose +0x10 points back at itself, with a
// sphere at +0x20 (fn_710021685c) and four growing arrays from +0xa8. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU308.h"
#include "types.h"

struct UnkTU250Object {
    u32 field_0x0;
    u32 field_0x4;
    s32 field_0x8;
    u8 pad_0xc[4];
    UnkTU250Object* self;  // 0x10
    u32 field_0x18;
    u8 pad_0x1c[4];
    u8 sphere[0x50];  // 0x20
    bool field_0x70;
    u8 pad_0x71[7];
    u64 field_0x78;
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    u32 field_0xa0;
    u8 pad_0xa4[4];
    base::global::CRntVector<void*> items0;  // 0xa8
    base::global::CRntVector<void*> items1;  // 0xd0
    base::global::CRntVector<void*> items2;  // 0xf8
    base::global::CRntVector<void*> items3;  // 0x120
};
static_assert(__builtin_offsetof(UnkTU250Object, items3) == 0x120, "UnkTU250Object layout");

// The second argument is unused (its caller passes null).
void fn_71001bcbec(UnkTU250Object* self, void*) {
    self->field_0x0 = 0;
    self->field_0x4 = 0;
    self->field_0x8 = -1;
    self->field_0x18 = 0;
    self->self = nullptr;
    fn_710021685c(self->sphere);
    self->field_0x70 = false;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 0;
    self->field_0xa0 = 0;
    new (&self->items0) base::global::CRntVector<void*>(0xf);
    new (&self->items1) base::global::CRntVector<void*>(0xf);
    new (&self->items2) base::global::CRntVector<void*>(0xf);
    new (&self->items3) base::global::CRntVector<void*>(0xf);
    self->field_0x0 = 0x400;
    self->field_0x4 = 1;
    self->self = self;
}
