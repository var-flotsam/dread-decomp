#pragma once

#include "types.h"

// Returned in memory: the vector type has a user-written copy constructor.
struct UnkVec3_b42 {
    f32 x, y, z;
    UnkVec3_b42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_b42(const UnkVec3_b42& o) : x(o.x), y(o.y), z(o.z) {}
};
