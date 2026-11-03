// Unit 259 of tools/tu_map.py (the graphics system's unit): sets a colour from its four float
// components (stored at +0x40-0x4c) and its packed 8-bit RGBA form at +0x50 (red in the top byte).
// Names and layouts are placeholders.
#include "types.h"

struct UnkColorHolder_b46a1 {
    u8 pad_0x0[0x40];
    f32 r;
    f32 g;
    f32 b;
    f32 a;
    u32 rgba;
};

void fn_71001d1e78(UnkColorHolder_b46a1* self, f32 r, f32 g, f32 b, f32 a) {
    self->r = r;
    self->g = g;
    self->b = b;
    self->a = a;
    self->rgba = (u32)(r * 255.0f) << 24 | (u32)(g * 255.0f) << 16 | (u32)(b * 255.0f) << 8 | (u32)(a * 255.0f);
}
