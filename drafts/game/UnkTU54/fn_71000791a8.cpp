// Unit 54: the matrix of a transform from its translation, rotation angles and scale (translation
// times rotation times scale). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

static inline UnkMatrix44 Translation(UnkVec3f t) {
    UnkMatrix44 r;
    for (int i = 0; i < 16; i++) r.m[i] = 0.0f;
    r.m[15] = 1.0f;
    r.m[10] = 1.0f;
    r.m[5] = 1.0f;
    r.m[0] = 1.0f;
    r.m[3] = t.x;
    r.m[7] = t.y;
    r.m[11] = t.z;
    return r;
}

static inline UnkMatrix44 Rotation(const UnkVec3f& angle) {
    f32 a = angle.x, b = angle.y, c = angle.z;
    f32 m0, m1, m2, m4, m5, m6, m8, m9, m10;
    if (fabsf(a) > 0.0001f || fabsf(b) > 0.0001f || fabsf(c) > 0.0001f) {
        f32 sc = sinf(c);
        f32 cc = cosf(c);
        f32 sb = sinf(b);
        f32 cb = cosf(b);
        f32 sa = sinf(a);
        f32 ca = cosf(a);
        f32 sbsa = sb * sa;
        f32 sbca = sb * ca;
        m0 = cc * cb;
        m1 = cc * sbsa - sc * ca;
        m2 = sc * sa + cc * sbca;
        m4 = sc * cb;
        m5 = sc * sbsa + cc * ca;
        m6 = sc * sbca - cc * sa;
        m8 = -sb;
        m9 = cb * sa;
        m10 = cb * ca;
    } else {
        m0 = 1.0f;
        m1 = 0.0f;
        m2 = 0.0f;
        m4 = 0.0f;
        m5 = 1.0f;
        m6 = 0.0f;
        m8 = 0.0f;
        m9 = 0.0f;
        m10 = 1.0f;
    }
    UnkMatrix44 r;
    r.m[0] = m0;
    r.m[1] = m1;
    r.m[2] = m2;
    r.m[3] = 0.0f;
    r.m[4] = m4;
    r.m[5] = m5;
    r.m[6] = m6;
    r.m[7] = 0.0f;
    r.m[8] = m8;
    r.m[9] = m9;
    r.m[10] = m10;
    r.m[11] = 0.0f;
    r.m[12] = 0.0f;
    r.m[13] = 0.0f;
    r.m[14] = 0.0f;
    r.m[15] = 1.0f;
    return r;
}

static inline UnkMatrix44 Scaling(const UnkVec3f& s) {
    UnkMatrix44 r;
    for (int i = 0; i < 16; i++) r.m[i] = 0.0f;
    r.m[0] = s.x;
    r.m[5] = s.y;
    r.m[10] = s.z;
    r.m[15] = 1.0f;
    return r;
}

UnkMatrix44 fn_71000791a8(const UnkVec3f& translation, const UnkVec3f& angle, const UnkVec3f& scale) {
    return fn_71000127c8(fn_71000127c8(Translation(translation), Rotation(angle)), Scaling(scale));
}
