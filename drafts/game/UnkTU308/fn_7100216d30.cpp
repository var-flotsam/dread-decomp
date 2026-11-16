// Unit 308: scales a stored base value by a factor and caches the fixed-point (x10000) form.
// Names and layouts are placeholders.
#include "types.h"

struct UnkScaleRecord {
    u8 pad_0x0[8];
    f32 scale;
    u8 pad_0xc[0x1c];
    f32 field_0x28;
    u8 pad_0x2c[4];
    f32 base;
    u8 pad_0x34[4];
    s64 scaled;
    u8 pad_0x40[0xc];
    u8 isZero;
};

void fn_7100216d30(UnkScaleRecord* self, f32 factor) {
    f32 v = self->base * factor;
    self->field_0x28 = v;
    self->scale = v;
    s64 fixed = (s64)(v * 10000.0f);
    self->scaled = fixed;
    self->isZero = fixed == 0;
}
