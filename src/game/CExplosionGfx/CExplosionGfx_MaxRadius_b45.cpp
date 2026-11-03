// CExplosionGfx's unit (tools/tu_map.py): the largest of three floats at +0xd4, never below zero.
// Names and layouts are placeholders.
#include "types.h"

struct UnkExplosionGfx_b45a3 {
    u8 pad_0x0[0xd4];
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
};

static inline f32 Max(f32 a, f32 b) { return a > b ? a : b; }

f32 fn_7100de1388(const UnkExplosionGfx_b45a3* self) {
    f32 m = Max(self->field_0xd4, Max(self->field_0xd8, self->field_0xdc));
    return m < 0.0f ? 0.0f : m;
}
