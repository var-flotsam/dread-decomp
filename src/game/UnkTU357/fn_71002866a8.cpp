// Unit 357 (tools/tu_map.py), the navigation query's unit: resets the whole query, releasing the shared
// object at +0x58 and destroying the inline list of 12-byte hits at +0x94 (count at +0x90).
// Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU362.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector2D;

struct UnkNavQueryHit_2866a8 {
    u8 data[0xc];
};

struct UnkNavQuery_2866a8 {
    u8 pad_0x0[0x10];
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u8 pad_0x30[0x10];
    void* field_0x40;
    void* field_0x48;
    u64 field_0x50;
    UnkRefObject* field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u64 field_0x70;
    u16 field_0x78;
    u8 field_0x7a;
    u8 pad_0x7b;
    u32 field_0x7c;
    bool field_0x80;
    u8 pad_0x81[3];
    CVector2D point;  // 0x84
    u16 cell;         // 0x8c
    u8 pad_0x8e[2];
    u32 count;                          // 0x90
    UnkNavQueryHit_2866a8 hits[16];     // 0x94
    f32 field_0x154;
    f32 field_0x158;
    f32 field_0x15c;
    f32 field_0x160;
    f32 field_0x164;
    f32 field_0x168;
    f32 field_0x16c;
    f32 field_0x170;
    u8 pad_0x174[4];
    u64 field_0x178;
    u64 field_0x180;
    u64 field_0x188;
    u64 field_0x190;
    u64 field_0x198;
    u8 field_0x1a0;
    u8 pad_0x1a1[3];
    f32 field_0x1a4;
    f32 field_0x1a8;
    u8 pad_0x1ac[4];
    u8 field_0x1b0;
};
static_assert(__builtin_offsetof(UnkNavQuery_2866a8, field_0x154) == 0x154, "UnkNavQuery_2866a8 layout");
static_assert(__builtin_offsetof(UnkNavQuery_2866a8, field_0x1b0) == 0x1b0, "UnkNavQuery_2866a8 layout");

void fn_71002866a8(UnkNavQuery_2866a8* self) {
    self->field_0x50 = 0;
    ResetRef(&self->field_0x58);
    self->field_0x60 = 0;
    self->field_0x68 = 0;
    self->field_0x154 = 0.0f;
    self->field_0x158 = 0.0f;
    self->field_0x15c = 0.0f;
    self->field_0x160 = 0.0f;
    self->field_0x164 = -1.0f;
    self->field_0x168 = 0.0f;
    self->field_0x170 = 0.0f;
    self->field_0x16c = 1.0f;
    self->field_0x70 = 0;
    self->field_0x78 = 0xffff;
    self->field_0x40 = nullptr;
    self->field_0x7c = 0;
    self->field_0x7a = 0xff;
    self->field_0x20 = 0;
    self->field_0x198 = 0;
    self->field_0x48 = nullptr;
    self->field_0x18 = 0;
    self->field_0x190 = 0;
    self->field_0x28 = 0;
    self->field_0x10 = 0;
    // The result: no hit, no cell, and the hits destroyed.
    self->field_0x80 = false;
    self->point.x = 0.0f;
    self->point.y = 0.0f;
    self->cell = 0xffff;
    for (u64 i = 0; i < self->count; ++i)
        fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(&self->hits[i]));
    self->count = 0;
    self->field_0x1a0 = 0;
    self->field_0x1a4 = 0.0f;
    self->field_0x1b0 = 0;
    self->field_0x180 = 0;
    self->field_0x1a8 = 0.0f;
    self->field_0x188 = 0;
    self->field_0x178 = 0;
}
