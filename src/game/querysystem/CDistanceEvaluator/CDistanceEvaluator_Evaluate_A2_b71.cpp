// querysystem::CDistanceEvaluator's unit (tools/tu_map.py unit 380): the score of the distance between
// two transforms (translation at +0x44/+0x54/+0x64), 1 at the range's minimum (+0x20) falling linearly
// to 0 at its maximum (+0x24); 0 for an invalid range. Names are placeholders.
#include "types.h"

struct UnkDistanceTransform_A2_b71 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkDistanceEvaluator_A2_b71 {
    u8 pad_0x0[0x20];
    f32 min;  // 0x20
    f32 max;  // 0x24
};

f32 fn_71002bba34(UnkDistanceEvaluator_A2_b71* self, UnkDistanceTransform_A2_b71* a,
                  UnkDistanceTransform_A2_b71* b) {
    if (!(self->min >= 0.0f) || !(self->max > 0.0f && self->min < self->max))
        return 0.0f;
    f32 ax = a->x, ay = a->y, az = a->z;
    f32 bx = b->x, by = b->y, bz = b->z;
    f32 dx = ax - bx;
    f32 dy = ay - by;
    f32 dz = az - bz;
    f32 d = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    f32 min = self->min;
    f32 max = self->max;
    f32 c = d <= min ? min : (d >= max ? max : d);
    return 1.0f - (c - min) / (max - min);
}
