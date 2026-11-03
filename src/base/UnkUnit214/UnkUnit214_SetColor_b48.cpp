// Unit 214 (tools/tu_map.py): sets the colour at +0xe0 (fn_7100030728, clamped) and keeps its
// 8-bit form at +0x138 (each component times 255, truncated). Names and layouts are placeholders.
#include "base/color/CColor4F.h"

using base::color::CColor4F;

struct UnkUnit214Owner {
    u8 pad_0x0[0xe0];
    CColor4F color;    // 0xe0
    u8 pad_0xf0[0x48];
    u8 rgba[4];        // 0x138
};

void fn_7100196338(UnkUnit214Owner* self, const CColor4F* color) {
    fn_7100030728(&self->color, color);
    self->rgba[0] = (s32)(self->color.r * 255.0f);
    self->rgba[1] = (s32)(self->color.g * 255.0f);
    self->rgba[2] = (s32)(self->color.b * 255.0f);
    self->rgba[3] = (s32)(self->color.a * 255.0f);
}
