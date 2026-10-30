// From unit 223 of tools/tu_map.py (0x710019bd2c-0x710019c69c, no reflected class): sets a fade
// layer's colour (+0xf8) and keeps it as bytes too (+0x17c, red to alpha). Unit 224 has the same for
// another layer (src/game/UnkUnit224/fn_710019db08.cpp). Names and layouts are placeholders.
#include "base/color/CColor4F.h"

struct UnkFadeLayer;

struct UnkFadeLayerColor {
    u8 pad_0x0[0xf8];
    base::color::CColor4F color;  // 0xf8
    u8 pad_0x108[0x74];
    u8 bytes[4];  // 0x17c
};
static_assert(__builtin_offsetof(UnkFadeLayerColor, bytes) == 0x17c, "UnkFadeLayerColor layout");

void fn_710019c4fc(UnkFadeLayer* layer, const base::color::CColor4F* color) {
    auto* self = reinterpret_cast<UnkFadeLayerColor*>(layer);
    fn_7100030728(&self->color, color);
    self->bytes[0] = (u8)(s32)(self->color.r * 255.0f);
    self->bytes[1] = (u8)(s32)(self->color.g * 255.0f);
    self->bytes[2] = (u8)(s32)(self->color.b * 255.0f);
    self->bytes[3] = (u8)(s32)(self->color.a * 255.0f);
}
