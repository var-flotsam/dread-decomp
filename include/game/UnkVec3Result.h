#pragma once

#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable (as in
// src/game_lo/fn_71007455dc.cpp).
// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};
