// Unit 308: widens two min/max ranges by a point's distance (beyond a radius) and caches the fixed-point (x10000) maximum.
// Names and layouts are placeholders.
#include <math.h>

#include "types.h"

struct UnkRangeF {
    f32 min;
    f32 max;

    void Include(f32 v) {
        if (!(min <= max)) {
            min = v;
            max = v;
        } else if (min > v) {
            min = v;
        } else if (max < v) {
            max = v;
        }
    }
};

struct UnkRangeOwner {
    u8 pad_0x0[8];
    f32 radius;
    u8 pad_0xc[0xc];
    f32 center[3];
    UnkRangeF near;
    UnkRangeF far;
    u8 pad_0x34[4];
    s64 scaled;
    u8 pad_0x40[0xc];
    u8 isZero;
};

void fn_7100216b00(UnkRangeOwner* self, const f32* p, f32 margin) {
    f32 px = p[0], py = p[1];
    f32 cx = self->center[0], cy = self->center[1];
    f32 pz = p[2];
    f32 cz = self->center[2];
    f32 dx = fabsf(px - cx);
    f32 dy = fabsf(py - cy);
    f32 dz = fabsf(pz - cz);
    dy = dy + margin;
    dx = dx + margin;
    dz = dz + margin;
    f32 d = sqrtf(dx * dx + dy * dy + dz * dz);
    f32 over = d - self->radius;
    f32 v = !(over < 0.0f) ? over : 0.0f;
    self->near.Include(v);
    self->far.Include(v);
    f32 top = self->near.max;
    s64 fixed = (s64)(top * 10000.0f);
    self->scaled = fixed;
    self->radius = top;
    self->isZero = fixed == 0;
}
