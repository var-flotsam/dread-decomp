// Unit 101: resets an effect record's three float triples to their stored defaults and attaches an owner.
// Names and layouts are placeholders.
#include "types.h"

extern s32 g_unk_7108b6c470 __attribute__((visibility("hidden")));

struct UnkTriple_l10519c {
    f32 x, y, z;
    UnkTriple_l10519c() = default;
    explicit UnkTriple_l10519c(f32 v) : x(v), y(v), z(v) {}
};

struct UnkReset_l10519c {
    UnkTriple_l10519c a;  // 0x0
    u8 pad_0xc[0x28 - 0xc];
    f32 one_x;  // 0x28
    f32 one_y;  // 0x2c
    f32 one_z;  // 0x30
    u8 pad_0x34[0x3c - 0x34];
    UnkTriple_l10519c b;  // 0x3c
    u8 pad_0x48[0x64 - 0x48];
    UnkTriple_l10519c c;  // 0x64
    u8 pad_0x70[0x78 - 0x70];
    void* owner;  // 0x78
    void* owner_next;  // 0x80
    u8 pad_0x88[0xc4 - 0x88];
    u32 field_0xc4;
    u8 pad_0xc8[0xe0 - 0xc8];
    f32 def_c;  // 0xe0
    u8 pad_0xe4[0xec - 0xe4];
    f32 def_a;  // 0xec
    u8 pad_0xf0[0xf4 - 0xf0];
    f32 def_b;  // 0xf4
    u8 pad_0xf8[0x120 - 0xf8];
    bool flag;  // 0x120
};

void fn_710010519c(UnkReset_l10519c* self, void* owner, u32 value, bool flag) {
    g_unk_7108b6c470++;
    self->flag = flag;
    UnkTriple_l10519c a(self->def_a);
    UnkTriple_l10519c b(self->def_b);
    UnkTriple_l10519c c(self->def_c);
    self->owner = owner;
    self->owner_next = nullptr;
    self->a = a;
    self->one_z = 1.0f;
    self->field_0xc4 = value;
    self->b = b;
    self->c = c;
    self->one_x = 1.0f;
    self->one_y = 1.0f;
}
