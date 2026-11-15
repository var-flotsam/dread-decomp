// Unit 54: the matrix of three angles (x, y, z) as a rotation, identity when all are tiny. Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkAnglesVec3 {
    f32 x, y, z;
};

UnkMatrix44 fn_7100078fc8(const UnkAnglesVec3* angles) {
    UnkMatrix44 r;
    f32 a = angles->x, b = angles->y, c = angles->z;
    f32 m0, m1, m2, m4, m5, m6, m8, m9, m10;
    if (fabsf(a) > 0.0001f || fabsf(b) > 0.0001f || fabsf(c) > 0.0001f) {
        f32 sc = sinf(c);
        f32 cc = cosf(c);
        f32 sb = sinf(b);
        f32 cb = cosf(b);
        f32 sa = sinf(a);
        f32 ca = cosf(a);
        f32 sbsa = sb * sa;
        m1 = cc * sbsa - sc * ca;
        m6 = sc * (sb * ca) - cc * sa;
        m0 = cc * cb;
        m2 = sc * sa + cc * (sb * ca);
        m4 = sc * cb;
        m5 = sc * sbsa + cc * ca;
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
    r.m[0] = m0;
    r.m[1] = m1;
    r.m[2] = m2;
    r.m[4] = m4;
    r.m[5] = m5;
    r.m[6] = m6;
    r.m[8] = m8;
    r.m[9] = m9;
    r.m[10] = m10;
    r.m[3] = 0.0f;
    r.m[7] = 0.0f;
    r.m[11] = 0.0f;
    r.m[12] = 0.0f;
    r.m[13] = 0.0f;
    r.m[14] = 0.0f;
    r.m[15] = 1.0f;
    return r;
}
