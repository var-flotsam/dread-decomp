// tools/tu_map.py unit 224 (no class hint): a constructor over the base built by fn_710019e76c
// (fn_710019c6c0): sets the vtable unk_7101a1e278, a run of unit and 0.1 floats, three empty arrays (flags 12),
// two small ints (6, 7), a unit vector of four, an id of -1, an identity 4x4 matrix at +0x164 (zeroed, then the
// diagonal set from the last element) and a count of 3. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "game/UnkTU224.h"
#include "game/UnkTU226.h"
#include "types.h"

struct UnkIdentity44_224_b69a1 {
    f32 m[16];

    UnkIdentity44_224_b69a1() {
        __builtin_memset(m, 0, sizeof(m));
        m[15] = 1.0f;
        m[10] = 1.0f;
        m[5] = 1.0f;
        m[0] = 1.0f;
    }
};

struct UnkObject224_b69a1 {
    void* vtable;
    u8 base_0x8[0x18];
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    u64 field_0x50;
    u64 field_0x58;
    u32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    f32 field_0x6c;
    f32 field_0x70;
    f32 field_0x74;
    f32 field_0x78;
    u32 pad_0x7c;
    u64 field_0x80;
    u64 field_0x88;
    u64 field_0x90;
    u64 field_0x98;
    base::UnkVector<void*> v0;  // 0xa0
    base::UnkVector<void*> v1;  // 0xc8
    base::UnkVector<void*> v2;  // 0xf0
    u64 field_0x118;
    u64 field_0x120;
    u32 field_0x128;
    u64 field_0x130;
    u64 field_0x138;
    u32 field_0x140;
    u32 field_0x144;
    u32 field_0x148;
    u32 field_0x14c;
    f32 field_0x150;
    f32 field_0x154;
    f32 field_0x158;
    f32 field_0x15c;
    s32 field_0x160;
    UnkIdentity44_224_b69a1 m;  // 0x164
    u32 field_0x1a4;
    u64 field_0x1a8;
    s32 field_0x1b0;
    bool field_0x1b4;
    u32 field_0x1b8;
    f32 field_0x1bc;
};
static_assert(__builtin_offsetof(UnkObject224_b69a1, v0) == 0xa0, "layout");
static_assert(__builtin_offsetof(UnkObject224_b69a1, field_0x1bc) == 0x1bc, "layout");

void fn_710019c6c0(UnkObject224_b69a1* self) {
    fn_710019e76c(self);
    self->vtable = reinterpret_cast<char*>(g_unk_7101a1e278) + 0x10;
    self->field_0x20 = 0;
    self->field_0x28 = 0;
    self->field_0x30 = 0;
    self->field_0x38 = 0;
    self->field_0x3c = 1.0f;
    self->field_0x40 = 1.0f;
    self->field_0x44 = 1.0f;
    self->field_0x48 = 0.1f;
    self->field_0x4c = 0.1f;
    self->field_0x50 = 0;
    self->field_0x58 = 0;
    self->field_0x60 = 0;
    self->field_0x64 = 1.0f;
    self->field_0x68 = 1.0f;
    self->field_0x6c = 1.0f;
    self->field_0x70 = 1.0f;
    self->field_0x74 = 1.0f;
    self->field_0x78 = 1.0f;
    self->field_0x80 = 0;
    self->field_0x88 = 0;
    self->field_0x90 = 0;
    self->field_0x98 = 0;
    new (&self->v0) base::UnkVector<void*>(12);
    new (&self->v1) base::UnkVector<void*>(12);
    new (&self->v2) base::UnkVector<void*>(12);
    self->field_0x128 = 0;
    self->field_0x130 = 0;
    self->field_0x138 = 0;
    self->field_0x140 = 0;
    self->field_0x144 = 6;
    self->field_0x148 = 7;
    self->field_0x14c = 0;
    self->field_0x150 = 1.0f;
    self->field_0x154 = 1.0f;
    self->field_0x158 = 1.0f;
    self->field_0x15c = 1.0f;
    self->field_0x160 = -1;
    new (&self->m) UnkIdentity44_224_b69a1();
    self->field_0x1a4 = 3;
    self->field_0x1a8 = 0;
    self->field_0x1b0 = -1;
    self->field_0x1b4 = false;
    self->field_0x1b8 = 0;
    self->field_0x1bc = 1.0f;
    self->field_0x118 = 0;
    self->field_0x120 = 0;
}
