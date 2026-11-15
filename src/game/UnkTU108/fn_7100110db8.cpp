// Unit 108: tests whether the segment a-b crosses a plane (normal xyz, distance w) and writes the
// crossing point. Names and layouts are placeholders.
#include "types.h"

struct UnkVec3_108 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkPlane108 {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
};

static inline f32 Distance(const UnkPlane108* plane, const UnkVec3_108* p) {
    return plane->x * p->x + plane->y * p->y + plane->z * p->z;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
bool fn_7100110db8(const UnkVec3_108* a, const UnkVec3_108* b, const UnkPlane108* plane, UnkVec3_108* out) {
    f32 da = Distance(plane, a);
    f32 db = Distance(plane, b);
    da = plane->w + da;
    db = plane->w + db;
    bool behindA = da < 0.0f;
    bool behindB = db < 0.0f;
    if (behindA != behindB) {
        if (out) {
            f32 t = da / (da - db);
            out->x = a->x + (b->x - a->x) * t;
            out->y = a->y + (b->y - a->y) * t;
            out->z = a->z + (b->z - a->z) * t;
        }
        return true;
    }
    return false;
}
