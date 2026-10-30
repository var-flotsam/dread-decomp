// From unit 224 of tools/tu_map.py (0x710019c69c-0x710019dcac, no reflected class): sets a layer's
// colour (+0x150) and keeps it as bytes too (+0x1b0, red to alpha), as unit 223's fade layer does
// (src/game/UnkUnit223/fn_710019c4fc.cpp); and its blend mode (fn_710019dbf4). Names and layouts are
// placeholders.
#include "base/color/CColor4F.h"

struct UnkColorLayer224 {
    u8 pad_0x0[0x150];
    base::color::CColor4F color;  // 0x150
    u8 pad_0x160[0x50];
    u8 bytes[4];  // 0x1b0
};
static_assert(__builtin_offsetof(UnkColorLayer224, bytes) == 0x1b0, "UnkColorLayer224 layout");

void fn_710019db08(UnkColorLayer224* self, const base::color::CColor4F* color) {
    fn_7100030728(&self->color, color);
    self->bytes[0] = (u8)(s32)(self->color.r * 255.0f);
    self->bytes[1] = (u8)(s32)(self->color.g * 255.0f);
    self->bytes[2] = (u8)(s32)(self->color.b * 255.0f);
    self->bytes[3] = (u8)(s32)(self->color.a * 255.0f);
}

// The layer's blend mode (+0x1a4) from its kind (+0x148, 1 or another) and whether it is additive
// (+0x1af): 0 or 1 when it is, 2 or 3 when not.
struct UnkBlendLayer224 {
    u8 pad_0x0[0x148];
    s32 kind;  // 0x148
    u8 pad_0x14c[0x58];
    u32 mode;  // 0x1a4
    u8 pad_0x1a8[0x7];
    bool additive;  // 0x1af
};
static_assert(__builtin_offsetof(UnkBlendLayer224, additive) == 0x1af, "UnkBlendLayer224 layout");

template <class Self>
static inline void SetMode(Self* self) {
    u32 mode;
    if (self->additive)
        mode = self->kind != 1;
    else
        mode = self->kind != 1 ? 3 : 2;
    self->mode = mode;
}

void fn_710019dbf4(UnkBlendLayer224* self) { SetMode(self); }
// Copies in two neighbouring units, on their own layouts (the same member names).
struct UnkBlendLayer_710019639c {
    u8 pad_0x0[0xd8];
    s32 kind;  // 0xd8
    u8 pad_0xdc[0x54];
    u32 mode;  // 0x130
    u8 pad_0x134[0x2];
    bool additive;  // 0x136
};
static_assert(__builtin_offsetof(UnkBlendLayer_710019639c, additive) == 0x136, "UnkBlendLayer_710019639c layout");

struct UnkBlendLayer_710019c284 {
    u8 pad_0x0[0xf0];
    s32 kind;  // 0xf0
    u8 pad_0xf4[0x54];
    u32 mode;  // 0x148
    u8 pad_0x14c[0x35];
    bool additive;  // 0x181
};
static_assert(__builtin_offsetof(UnkBlendLayer_710019c284, additive) == 0x181, "UnkBlendLayer_710019c284 layout");

void fn_710019639c(UnkBlendLayer_710019639c* self) { SetMode(self); }
void fn_710019c284(UnkBlendLayer_710019c284* self) { SetMode(self); }
