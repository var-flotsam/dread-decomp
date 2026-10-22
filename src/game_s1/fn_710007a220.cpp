// Wraps each component of an angle vector (radians) into (-pi, pi]: subtract the multiple of
// 2 pi below it, then step back one turn if the remainder is past pi.
#include "types.h"

struct UnkVec3f {
    f32 x, y, z;
    UnkVec3f() {}
    UnkVec3f(const UnkVec3f& o) : x(o.x), y(o.y), z(o.z) {}
};

static inline f32 WrapAngle(f32 v) {
    const f32 two_pi = 6.2831855f;
    f32 a = v - __builtin_floorf(v / two_pi) * two_pi;
    if ((f64)a > 3.141592653589793)
        a += -two_pi;
    return a;
}

UnkVec3f fn_710007a220(const UnkVec3f* v) {
    f32 x = WrapAngle(v->x);
    f32 y = WrapAngle(v->y);
    f32 z = WrapAngle(v->z);
    UnkVec3f r;
    r.x = x;
    r.y = y;
    r.z = z;
    return r;
}
