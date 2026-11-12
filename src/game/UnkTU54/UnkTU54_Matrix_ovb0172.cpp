// Unit 54 of tools/tu_map.py (the matrix helpers of src/game_s1/fn_7100078c9c.cpp): a transform's
// rotation columns scaled to unit length (fn_71000774fc), the matrix to quaternion conversion
// (fn_71000772fc) and the angle unwrap of a vector of angles towards a target (fn_710007a050).
// Names and layouts are placeholders.
#include <math.h>

#include "base/UnkMatrix44.h"
#include "game/UnkTU54.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkQuatRet_b0172 {
    f32 x, y, z, w;
    UnkQuatRet_b0172() : x(0.0f), y(0.0f), z(0.0f), w(0.0f) {}
    UnkQuatRet_b0172(const UnkQuatRet_b0172& o) : x(o.x), y(o.y), z(o.z), w(o.w) {}
};

UnkQuatRet_b0172 fn_71000772fc(const UnkMatrix44* mat) {
    UnkQuatRet_b0172 q;
    const f32* m = mat->m;
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

void fn_71000774fc(UnkXform* x) {
    f32* a = &x->m[0][0];
    f32 inv = 1.0f / sqrtf(a[0] * a[0] + a[4] * a[4] + a[8] * a[8]);
    a[0] = a[0] * inv;
    a[4] = inv * a[4];
    a[8] = inv * a[8];
    inv = 1.0f / sqrtf(a[1] * a[1] + a[5] * a[5] + a[9] * a[9]);
    a[1] = a[1] * inv;
    a[5] = inv * a[5];
    a[9] = inv * a[9];
    inv = 1.0f / sqrtf(a[2] * a[2] + a[6] * a[6] + a[10] * a[10]);
    a[2] = a[2] * inv;
    a[6] = inv * a[6];
    a[10] = inv * a[10];
}

static inline void Unwrap(const f32* t, f32* a) {
    f32 cur = *a;
    f32 d = fabsf(*t - cur);
    f32 up = cur + 6.2831855f;
    f32 d_up = fabsf(*t - up);
    f32 down = cur + -6.2831855f;
    f32 d_down = fabsf(*t - down);
    f32 best;
    if (d_down < d && d_down < d_up)
        best = down;
    else if (d_up < d && d_up < d_down)
        best = up;
    else
        return;
    *a = best;
}

void fn_710007a050(const UnkVec3f* target, UnkVec3f* angle) {
    Unwrap(&target->x, &angle->x);
    Unwrap(&target->y, &angle->y);
    f32 tz = target->z;
    f32 z = angle->z;
    f32 d = fabsf(tz - z);
    f32 up = z + 6.2831855f;
    f32 d_up = fabsf(tz - up);
    f32 down = z + -6.2831855f;
    f32 d_down = fabsf(tz - down);
    if (d_down < d && d_down < d_up)
        angle->z = down;
    else if (d_up < d && d_up < d_down)
        angle->y = angle->y + 6.2831855f;
}
