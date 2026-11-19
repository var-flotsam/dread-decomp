// Unit 54: the rotation matrix about an axis (normalised here) by an angle in radians.
// Names and layouts are placeholders.
#include <math.h>

#include "game/UnkTU54.h"
#include "types.h"

// The vector scaled to unit length (kept finite for a zero vector).
inline UnkVec3f Normalized(const UnkVec3f& v) {
    f32 inv = 1.0f / (sqrtf(v.x * v.x + v.y * v.y + v.z * v.z) + 0.0001f);
    return UnkVec3f(v.x * inv, v.y * inv, inv * v.z);
}

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
UnkMatrix44 fn_710007966c(const void* value, f32 angle) {
    UnkMatrix44 r;
    UnkVec3f axis = Normalized(*static_cast<const UnkVec3f*>(value));
    f32 x = axis.x;
    f32 y = axis.y;
    f32 z = axis.z;
    f32 c = cosf(angle);
    f32 s = sinf(angle);
    f32 t = 1.0f - c;

    r.m[3] = 0.0f;
    r.m[4] = 0.0f;
    r.m[5] = 0.0f;
    r.m[6] = 0.0f;
    r.m[7] = 0.0f;
    r.m[8] = 0.0f;
    r.m[9] = 0.0f;
    r.m[10] = 0.0f;
    r.m[11] = 0.0f;
    r.m[12] = 0.0f;
    r.m[13] = 0.0f;
    r.m[14] = 0.0f;
    f32 xz = x * z * t;
    f32 xy = x * y * t;
    r.m[0] = c + x * x * t;
    r.m[1] = xy - z * s;
    r.m[2] = xz + y * s;
    r.m[4] = xy + z * s;
    r.m[5] = c + y * y * t;
    r.m[6] = y * z * t - x * s;
    r.m[8] = xz - y * s;
    r.m[9] = y * z * t + x * s;
    r.m[10] = c + z * z * t;
    r.m[15] = 1.0f;
    return r;
}
