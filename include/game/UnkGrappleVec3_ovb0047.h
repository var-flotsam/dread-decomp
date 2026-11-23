#pragma once

#include "types.h"

struct UnkGrappleVec3_ovb0047 {
    f32 x, y, z;
    bool IsZero() const { return x == 0.0f && y == 0.0f && z == 0.0f; }
};
