// Unit 108: tests whether the segment a-b crosses a plane (normal xyz, distance w) and writes the
// crossing point. Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector3D;

struct UnkPlane108 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

static inline f32 Distance(const UnkPlane108* plane, const CVector3D* p) {
    return plane->x * p->x + plane->y * p->y + plane->z * p->z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers (tools/equiv_test.py is
// to be run again on this text).
bool fn_7100110db8(const CVector3D* a, const CVector3D* b, const UnkPlane108* plane, CVector3D* out) {
    f32 da = Distance(plane, a);
    f32 db = Distance(plane, b);
    da = plane->w + da;
    db = plane->w + db;
    bool behindA = da < 0.0f;
    bool behindB = db < 0.0f;
    if (behindA != behindB) {
        if (out) {
            f32 t = da / (da - db);
            *out = *a + (*b - *a) * t;
        }
        return true;
    }
    return false;
}
