// Unit 108: intersects the segment a-b with a plane given by a point and a normal; returns 1 and writes
// the crossing point when it lies on the segment, 2 when the segment runs in the plane, else 0.
// Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "types.h"

using base::math::CVector3D;

u32 fn_7100110cd4(const CVector3D* a, const CVector3D* b, const CVector3D* planePoint,
                  const CVector3D* normal, CVector3D* out) {
    CVector3D dir = *b - *a;
    CVector3D toPlane = *a - *planePoint;
    f32 denom = Dot(dir, *normal);
    f32 num = -Dot(toPlane, *normal);
    if (__builtin_fabsf(denom) < 0.01f)
        return (__builtin_fabsf(num) < 0.01f) << 1;
    f32 t = num / denom;
    if (!(t < 0.0f) && !(t > 1.0f)) {
        if (out) {
            *out = *a + dir * t;
        }
        return 1;
    }
    return 0;
}
