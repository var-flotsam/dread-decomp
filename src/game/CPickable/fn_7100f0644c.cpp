// CPickable unit: a constructor. Base constructor, vtable, cleared fields, a copy of the empty string id,
// then the object's own vtable slot 3. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "game/CGameObject.h"
#include "game/CPickable.h"
#include "types.h"

struct UnkPickable_lf0644c {
    void** vtable;
    u8 pad_0x8[8];
    u64 field_0x10;
    u64 field_0x18;
    u8 pad_0x20[0x10];
    u32 field_0x30;
    u8 field_0x34;
    u8 pad_0x35[3];
    u64 field_0x38;
    u64 field_0x40;
    u64 field_0x48;
    u64 field_0x50;
    u64 field_0x58;
    u8 field_0x60;
    u8 pad_0x61[3];
    u8 field_0x64[18];
    u8 field_0x76;
    u8 pad_0x77;
    u64 field_0x78;
    u64 field_0x80;
    u8 field_0x88[13];
    u8 pad_0x95[3];
    u32 field_0x98;
    u8 field_0x9c;
    u8 pad_0x9d[3];
    f32 field_0xa0;
    f32 field_0xa4;
    u8 field_0xa8;
    u8 field_0xa9;
    u8 field_0xaa;
    u8 pad_0xab[5];
    u64 field_0xb0;
    base::global::CStrId field_0xb8;
    u8 field_0xc0[13];
    u8 pad_0xcd[3];
    u8 field_0xd0[13];
    u8 pad_0xdd[3];
    u64 field_0xe0;
    s64 field_0xe8;
};

static void Clear(u8* p, int n) {
    for (int i = 0; i < n; i++) p[i] = 0;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100f0644c(UnkPickable_lf0644c* self) {
    fn_7101043904(self);
    self->vtable = g_unk_7101caa410 + 2;
    self->field_0x30 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->field_0x38 = 0;
    self->field_0x40 = 0;
    self->field_0x76 = 1;
    self->field_0x34 = 0;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    Clear(self->field_0x64, 18);
    self->field_0x98 = 0;
    self->field_0x9c = 0;
    Clear(self->field_0x88, 13);
    self->field_0xa0 = 1.0f;
    self->field_0xa4 = 1.0f;
    self->field_0xa8 = 0;
    self->field_0xa9 = 1;
    self->field_0x48 = 0;
    self->field_0x50 = 0;
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    self->field_0xaa = 0;
    self->field_0xb0 = 0;
    new (&self->field_0xb8) base::global::CStrId(g_unk_7101d470e0);
    Clear(self->field_0xc0, 13);
    Clear(self->field_0xd0, 13);
    self->field_0xe0 = 0;
    self->field_0xe8 = -1;
    reinterpret_cast<void (*)(UnkPickable_lf0644c*)>(self->vtable[3])(self);
}
