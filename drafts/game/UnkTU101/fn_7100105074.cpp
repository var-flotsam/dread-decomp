// Unit 101: the falloff factor of a sound source at a position: 1 outside its range, otherwise a linear
// or exponential fade by distance to the source. Names and layouts are placeholders.
#include <math.h>
#include "types.h"

struct UnkVec3_l105074 {
    f32 x, y, z;
};

struct UnkFalloff_l105074 {
    u8 pad_0x0[0x98];
    f32 pos[3];  // 0x98
    u8 pad_0xa4[0xc0 - 0xa4];
    s32 mode;  // 0xc0
    u8 pad_0xc4[0xe4 - 0xc4];
    f32 range;  // 0xe4
    f32 limit;  // 0xe8
    u8 pad_0xec[0x11f - 0xec];
    bool enabled;  // 0x11f
};

f32 fn_7100105074(UnkFalloff_l105074* self, UnkVec3_l105074* at) {
    f32 result = 1.0f;
    if (self->enabled) {
        f32 range = self->range;
        f32 limit = self->limit;
        if (range > 0.0f && !(range < limit)) {
            f32 dx = self->pos[0] - at->x;
            f32 dy = self->pos[1] - at->y;
            f32 dz = self->pos[2] - at->z;
            f32 dist = sqrtf(dx * dx + dy * dy + dz * dz);
            range = self->range;
            limit = self->limit;
            if (range + 0.001f < limit || !(range - 0.001f <= limit)) {
                dist -= limit;
                range -= limit;
            }
            f32 t = 1.0f - dist / range;
            t = t > 0.0f ? (t < 1.0f ? t : 1.0f) : 0.0f;
            if (self->mode == 1)
                result = t;
            else if (self->mode == 0)
                result = expf(t * 4.6f) * 0.01f;
        }
    }
    return result;
}
