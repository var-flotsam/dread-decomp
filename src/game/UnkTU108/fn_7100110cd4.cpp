// Unit 108: intersects the segment a-b with a plane given by a point and a normal; returns 1 and writes
// the crossing point when it lies on the segment, 2 when the segment runs in the plane, else 0.
// Names and layouts are placeholders.
#include "types.h"

struct UnkVec3_108 {
    f32 x;
    f32 y;
    f32 z;
};

static inline f32 Dot(const UnkVec3_108& a, const UnkVec3_108& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
u32 fn_7100110cd4(const UnkVec3_108* a, const UnkVec3_108* b, const UnkVec3_108* planePoint,
                  const UnkVec3_108* normal, UnkVec3_108* out) {
    UnkVec3_108 dir;
    dir.x = b->x - a->x;
    dir.y = b->y - a->y;
    dir.z = b->z - a->z;
    UnkVec3_108 toPlane;
    toPlane.x = a->x - planePoint->x;
    toPlane.y = a->y - planePoint->y;
    toPlane.z = a->z - planePoint->z;
    f32 denom = Dot(dir, *normal);
    f32 num = -Dot(toPlane, *normal);
    if (__builtin_fabsf(denom) < 0.01f)
        return (__builtin_fabsf(num) < 0.01f) << 1;
    f32 t = num / denom;
    if (!(t < 0.0f) && !(t > 1.0f)) {
        if (out) {
            out->x = a->x + dir.x * t;
            out->y = a->y + dir.y * t;
            out->z = a->z + dir.z * t;
        }
        return 1;
    }
    return 0;
}
