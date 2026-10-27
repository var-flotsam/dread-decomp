// Rotation angles of an affine transform (UnkXform, include/game/UnkXformNode.h), from the transform
// math of translation unit 54 of data/tu_map.csv (next to fn_7100076cec's scale and
// fn_7100079b18's cached angles): the basis columns are normalised when none is degenerate, then
// the angles come from the third row (with the gimbal-lock cases at +-90 degrees). fn_7100076acc
// gives one solution, fn_7100077038 both (the second with y mirrored about 180 degrees; in gimbal
// lock both take the given z).
#include "game/UnkXformNode.h"

extern "C" f32 sqrtf(f32);
extern "C" f32 asinf(f32);
extern "C" f32 cosf(f32);
extern "C" f32 atan2f(f32 y, f32 x);

static inline f32 ColumnLength(const UnkXform* x, int c) {
    f32 a = x->m[0][c];
    f32 b = x->m[1][c];
    f32 d = x->m[2][c];
    return sqrtf(a * a + b * b + d * d);
}

// NON_MATCHING(minor): register allocation: m21 and m22 swap registers (the original spills m21
// across the square roots, ours m22), and m01/m02 are loaded after the first column's sum.
UnkVec3f fn_7100076acc(const UnkXform* x) {
    f32 m22 = x->m[2][2];
    f32 m02 = x->m[0][2];
    f32 m00 = x->m[0][0];
    f32 m10 = x->m[1][0];
    f32 m20 = x->m[2][0];
    f32 m01 = x->m[0][1];
    f32 m21 = x->m[2][1];
    f32 sx = ColumnLength(x, 0);
    f32 sy = ColumnLength(x, 1);
    f32 sz = ColumnLength(x, 2);
    if (sx > 0.0001f && sy > 0.0001f && sz > 0.0001f) {
        f32 ix = 1.0f / sx;
        m00 *= ix;
        m10 *= ix;
        m20 *= ix;
        f32 iy = 1.0f / sy;
        m01 *= iy;
        m21 *= iy;
        f32 iz = 1.0f / sz;
        m02 *= iz;
        m22 *= iz;
    }
    UnkVec3f r(0.0f, 0.0f, 0.0f);
    f32 ax, ay, az;
    if (__builtin_fabsf(m20) != 1.0f) {
        f32 a = asinf(m20);
        ay = -a;
        f32 c = cosf(a);
        ax = atan2f(m21 / c, m22 / c);
        az = atan2f(m10 / c, m00 / c);
    } else {
        if (m20 == -1.0f) {
            ax = atan2f(m01, m02) + 3.1415927f;
            ay = 1.5707964f;
        } else {
            ax = atan2f(-m01, -m02) + -3.1415927f;
            ay = -1.5707964f;
        }
        az = 3.1415927f;
    }
    r.x = ax;
    r.y = ay;
    r.z = az;
    return r;
}

// Whether `v` is within 1e-7 of `target`.
static inline bool NearlyEqual(f32 v, f32 target) {
    return v - 1e-7f <= target && v + 1e-7f >= target;
}

// NON_MATCHING(minor): scheduling: the first row's loads pair m01 with m02 (the original m00 with
// m01) and come before the other loads.
void fn_7100077038(const UnkXform* x, UnkVec3f* a, UnkVec3f* b, f32 z) {
    f32 m02 = x->m[0][2];
    f32 m21 = x->m[2][1];
    f32 m00 = x->m[0][0];
    f32 m10 = x->m[1][0];
    f32 m20 = x->m[2][0];
    f32 m01 = x->m[0][1];
    f32 m22 = x->m[2][2];
    f32 sx = ColumnLength(x, 0);
    f32 sy = ColumnLength(x, 1);
    f32 sz = ColumnLength(x, 2);
    if (sx > 0.0001f && sy > 0.0001f && sz > 0.0001f) {
        f32 ix = 1.0f / sx;
        m00 *= ix;
        m10 *= ix;
        m20 *= ix;
        f32 iy = 1.0f / sy;
        m01 *= iy;
        m21 *= iy;
        f32 iz = 1.0f / sz;
        m02 *= iz;
        m22 *= iz;
    }
    f32 ax, ay, az, bx, by, bz;
    if (!NearlyEqual(__builtin_fabsf(m20), 1.0f)) {
        f32 s = asinf(m20);
        ay = -s;
        by = s + 3.14159265358979323846;
        f32 c1 = cosf(s);
        f32 c2 = cosf(by);
        ax = atan2f(m21 / c1, m22 / c1);
        az = atan2f(m10 / c1, m00 / c1);
        bx = atan2f(m21 / c2, m22 / c2);
        bz = atan2f(m10 / c2, m00 / c2);
    } else {
        az = z;
        if (NearlyEqual(m20, -1.0f)) {
            ax = atan2f(m01, m02) + z;
            ay = 1.5707964f;
        } else {
            ax = atan2f(-m01, -m02) - z;
            ay = -1.5707964f;
        }
        bx = ax;
        by = ay;
        bz = z;
    }
    a->x = ax;
    a->y = ay;
    a->z = az;
    b->x = bx;
    b->y = by;
    b->z = bz;
}
