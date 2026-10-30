// From unit 224 of tools/tu_map.py (0x710019c69c-0x710019dcac, no reflected class): sets a layer's
// colour (+0x150) and keeps it as bytes too (+0x1b0, red to alpha), as unit 223's fade layer does
// (src/game/UnkUnit223/fn_710019c4fc.cpp). Names and layouts are placeholders.
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
