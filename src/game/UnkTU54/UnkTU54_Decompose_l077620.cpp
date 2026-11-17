// Unit 54: splits a transform matrix into its translation, its rotation as a quaternion and the
// lengths of its rotation columns (the scale). Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "base/math/CVector4D.h"
#include "base/math/CVectorOps.h"
#include "base/math/UnkOneVector3.h"
#include "types.h"

using base::math::CVector3D;
using base::math::CVector4D;

struct UnkQuatRet_b0172 {
    f32 x, y, z, w;
    UnkQuatRet_b0172() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    UnkQuatRet_b0172(const UnkQuatRet_b0172& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

UnkQuatRet_b0172 fn_71000772fc(const UnkMatrix44* mat);

// fn_71000772fc's conversion, which the original expands in place for a local matrix.
static inline UnkQuatRet_b0172 ToQuaternion(const f32* m) {
    UnkQuatRet_b0172 q;
    f32 m00 = m[0], m11 = m[5];
    f32 trace = m00 + m11;
    f32 m22 = m[10];
    trace = trace + m22;
    if (trace > 0.0f) {
        f32 s = sqrtf(trace + 1.0f);
        s = s + s;
        q.w = s * 0.25f;
        q.x = (m[9] - m[6]) / s;
        q.y = (m[2] - m[8]) / s;
        q.z = (m[4] - m[1]) / s;
    } else if (m00 > m11 && m00 > m22) {
        f32 s = sqrtf(m00 + 1.0f - m11 - m22);
        s = s + s;
        q.w = (m[9] - m[6]) / s;
        q.x = s * 0.25f;
        q.y = (m[1] + m[4]) / s;
        q.z = (m[2] + m[8]) / s;
    } else if (m11 > m22) {
        f32 s = sqrtf(m11 + 1.0f - m00 - m22);
        s = s + s;
        q.w = (m[2] - m[8]) / s;
        q.x = (m[1] + m[4]) / s;
        q.y = s * 0.25f;
        q.z = (m[6] + m[9]) / s;
    } else {
        f32 s = sqrtf(m22 + 1.0f - m00 - m11);
        s = s + s;
        q.w = (m[4] - m[1]) / s;
        q.x = (m[2] + m[8]) / s;
        q.y = (m[6] + m[9]) / s;
        q.z = s * 0.25f;
    }
    return q;
}

static inline bool IsNear(f32 a, f32 b) {
    return fabsf(a - b) < 0.001f;
}

static inline bool IsUnitScale(const CVector3D& scale) {
    const CVector3D& one = g_unk_71016477a8;
    return IsNear(scale.x, one.x) && IsNear(scale.y, one.y) && IsNear(scale.z, one.z);
}

// The rotation of a matrix whose columns are scaled: the columns are made unit length first.
static inline UnkQuatRet_b0172 UnscaledRotation(const UnkMatrix44* mat) {
    UnkMatrix44 unit = *mat;
    f32* m = unit.m;
    for (int col = 0; col < 3; col++) {
        f32 inv = 1.0f / Length(CVector3D{m[col], m[col + 4], m[col + 8]});
        m[col] = m[col] * inv;
        m[col + 4] = m[col + 4] * inv;
        m[col + 8] = m[col + 8] * inv;
    }
    return ToQuaternion(m);
}

// The quaternion's assignment (CVector4D's, out of line).
static inline void SetRotation(CVector4D* dst, const UnkQuatRet_b0172& q) {
    fn_710007b518(dst, reinterpret_cast<const CVector4D*>(&q));
}

void fn_7100077620(const UnkMatrix44* mat, CVector3D* translation, CVector4D* rotation, CVector3D* scale) {
    const f32* m = mat->m;
    *translation = CVector3D{m[3], m[7], m[11]};
    f32 sx = Length(CVector3D{m[0], m[4], m[8]});
    f32 sy = Length(CVector3D{m[1], m[5], m[9]});
    f32 sz = Length(CVector3D{m[2], m[6], m[10]});
    scale->x = sx;
    scale->y = sy;
    scale->z = sz;
    if (IsUnitScale(*scale)) {
        SetRotation(rotation, fn_71000772fc(mat));
    } else {
        SetRotation(rotation, UnscaledRotation(mat));
    }
}
