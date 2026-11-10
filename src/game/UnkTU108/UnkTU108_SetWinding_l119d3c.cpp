// Unit 108: sets a polygon's winding direction (state 1 or 2), reversing the point list when the
// signed area disagrees with the requested direction. Names and layouts are placeholders.
#include "types.h"

struct UnkPolyPoint {
    f32 x;
    f32 y;
    f32 field_0x8;
};

struct UnkPolygon_b61 {
    UnkPolyPoint* points;
    u32 count;
    u8 pad_0xc[0x20];
    u8 field_0x2c;
    u8 pad_0x2d[0x13];
    u32 state;
};

void fn_7100118c78(UnkPolygon_b61* self);

void fn_7100119d3c(UnkPolygon_b61* self, bool flag) {
    u32 state = self->state;
    if (flag) {
        if (state == 2)
            return;
    } else if (state == 1) {
        return;
    }
    if (self->field_0x2c) {
        f32 area = 0.0f;
        for (u64 i = 0; i != self->count; ++i) {
            u64 next = i < self->count - 1 ? i + 1 : 0;
            UnkPolyPoint a = self->points[i];
            UnkPolyPoint b = self->points[next];
            area += (b.x - a.x) * 0.5f * (a.y + b.y);
        }
        if ((area >= 0.0f) ^ flag)
            fn_7100118c78(self);
    }
    self->state = flag ? 2 : 1;
}
