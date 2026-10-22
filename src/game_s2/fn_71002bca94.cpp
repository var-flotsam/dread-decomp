// How close the second object is, from 1 at the near limit (at 0x20) down to 0 at the far limit
// (at 0x24): the distance between the first object's position (values at 0x44, 0x54, 0x64) and
// the second's, clamped to the limits and scaled. Returns 0 without a position object or with limits
// that aren't ordered and non-negative.
#include "types.h"

struct UnkPositioned {
    u8 pad_0x0[0x44];
    f32 field_0x44;
    u8 pad_0x48[0xc];
    f32 field_0x54;
    u8 pad_0x58[0xc];
    f32 field_0x64;
};

struct UnkLimits {
    u8 pad_0x0[0x20];
    f32 field_0x20;
    f32 field_0x24;
};

UnkPositioned* fn_7100d7c990(void* object);

f32 fn_71002bca94(UnkLimits* self, void* first, UnkPositioned* second) {
    f32 result = 0.0f;
    UnkPositioned* target = fn_7100d7c990(first);
    if (target) {
        f32 tx = target->field_0x44;
        f32 ty = target->field_0x54;
        f32 tz = target->field_0x64;
        f32 sx = second->field_0x44;
        f32 sy = second->field_0x54;
        f32 sz = second->field_0x64;
        f32 dx = tx - sx;
        f32 dy = ty - sy;
        f32 dz = tz - sz;
        f32 distance = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
        f32 near = self->field_0x20;
        if (near >= 0.0f) {
            f32 far = self->field_0x24;
            if (far > 0.0f && near < far) {
                f32 upper = far <= distance ? far : distance;
                f32 clamped = (__builtin_isless(near, distance) || __builtin_isunordered(near, distance)) ? upper : near;
                result = 1.0f - (clamped - near) / (far - near);
            }
        }
    }
    return result;
}
