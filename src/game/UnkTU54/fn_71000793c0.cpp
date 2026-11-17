// Unit 54: the matrix of a translation followed by a rotation from three angles (the rotation is
// identity when all angles are tiny). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkXformNode.h"
#include "types.h"

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
UnkMatrix44 fn_71000793c0(const UnkVec3f& translation, const UnkVec3f& angle) {
    UnkVec3f t = translation;
    UnkMatrix44 move;
    for (int i = 0; i < 16; i++) move.m[i] = 0.0f;
    move.m[15] = 1.0f;
    move.m[10] = 1.0f;
    move.m[5] = 1.0f;
    move.m[0] = 1.0f;
    move.m[3] = t.x;
    move.m[7] = t.y;
    move.m[11] = t.z;

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
    UnkMatrix44 rotation;
    rotation.m[15] = 1.0f;
    rotation.m[0] = m0;
    rotation.m[1] = m1;
    rotation.m[2] = m2;
    rotation.m[3] = 0.0f;
    rotation.m[4] = m4;
    rotation.m[5] = m5;
    rotation.m[6] = m6;
    rotation.m[7] = 0.0f;
    rotation.m[8] = m8;
    rotation.m[9] = m9;
    rotation.m[10] = m10;
    rotation.m[11] = 0.0f;
    rotation.m[12] = 0.0f;
    rotation.m[13] = 0.0f;
    rotation.m[14] = 0.0f;
    return fn_71000127c8(move, rotation);
}
