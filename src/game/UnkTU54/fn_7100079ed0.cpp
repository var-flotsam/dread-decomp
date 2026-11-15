// Unit 54: builds the rotation matrix of a quaternion (x, y, z, w) into a matrix. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

struct UnkQuatToMat {
    f32 x, y, z, w;
};

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
void fn_7100079ed0(UnkMatrix44* mat, const UnkQuatToMat* q) {
    f32* m = mat->m;
    for (int i = 0; i < 16; i++) m[i] = 0.0f;
    m[15] = 1.0f;
    m[10] = 1.0f;
    m[5] = 1.0f;
    m[0] = 1.0f;
    f32 yy = q->y * q->y, zz = q->z * q->z, xx = q->x * q->x;
    f32 xz = q->z * q->x, xy = q->y * q->x, yz = q->y * q->z;
    f32 xw = q->x * q->w, yw = q->y * q->w, zw = q->z * q->w;
    m[0] = 1.0f - 2.0f * (yy + zz);
    m[1] = 2.0f * (xy + zw);
    m[2] = 2.0f * (xz - yw);
    m[4] = 2.0f * (xy - zw);
    m[5] = 1.0f - 2.0f * (xx + zz);
    m[6] = 2.0f * (yz + xw);
    m[8] = 2.0f * (xz + yw);
    m[9] = 2.0f * (yz - xw);
    m[10] = 1.0f - 2.0f * (xx + yy);
}
