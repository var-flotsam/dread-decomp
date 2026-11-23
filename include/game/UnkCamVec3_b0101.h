#pragma once

#include "types.h"

struct UnkCamVec3_b0101 {
    f32 x, y, z;
    UnkCamVec3_b0101(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkCamVec3_b0101(const UnkCamVec3_b0101& o) : x(o.x), y(o.y), z(o.z) {}
    UnkCamVec3_b0101& operator=(const UnkCamVec3_b0101& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};
