#pragma once

#include "types.h"

struct UnkVec3_sm42 {
    f32 x, y, z;
    UnkVec3_sm42(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
    UnkVec3_sm42(const UnkVec3_sm42& o) : x(o.x), y(o.y), z(o.z) {}
};
