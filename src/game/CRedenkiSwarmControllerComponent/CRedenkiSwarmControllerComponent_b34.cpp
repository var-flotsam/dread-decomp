#include "types.h"

struct UnkVec3 {
    f32 x, y, z;
};

// A weight that falls off with the squared distance between `a` and `b`: 1000 / (d^2 + 1).
f32 fn_710065b01c(const UnkVec3& a, const UnkVec3& b) {
    UnkVec3 va = a;
    UnkVec3 vb = b;
    f32 dx = va.x - vb.x;
    f32 dy = va.y - vb.y;
    f32 d2 = dx * dx + dy * dy;
    f32 dz = va.z - vb.z;
    return 1000.0f / (d2 + dz * dz + 1.0f);
}
