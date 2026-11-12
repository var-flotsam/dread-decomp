// Unit 54: builds a 4x4 rotation matrix from an axis and an angle (radians).
// Names and layouts are placeholders.
#include "types.h"

extern "C" f32 sqrtf(f32);
extern "C" f32 cosf(f32);
extern "C" f32 sinf(f32);

struct UnkAxis3f {
    f32 x, y, z;
};

struct UnkMatrix4f {
    f32 m[16];
};

UnkMatrix4f fn_710007966c(const UnkAxis3f* axis, f32 angle) {
    UnkMatrix4f r;
    f32 len = sqrtf(axis->x * axis->x + axis->y * axis->y + axis->z * axis->z);
    f32 inv = 1.0f / (len + 0.0001f);
    f32 x = axis->x * inv;
    f32 y = axis->y * inv;
    f32 z = inv * axis->z;
    f32 c = cosf(angle);
    f32 s = sinf(angle);
    f32 t = 1.0f - c;

    for (int i = 3; i < 15; i++) r.m[i] = 0.0f;
    f32 xy = x * y * t;
    f32 xz = x * z * t;
    f32 yz = y * z * t;
    r.m[2] = xz + y * s;
    r.m[0] = c + x * x * t;
    r.m[1] = xy - z * s;
    r.m[4] = xy + z * s;
    r.m[5] = c + y * y * t;
    r.m[6] = yz - x * s;
    r.m[8] = xz - y * s;
    r.m[9] = yz + x * s;
    r.m[10] = c + z * z * t;
    r.m[15] = 1.0f;
    return r;
}
