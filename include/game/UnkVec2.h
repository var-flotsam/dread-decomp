#pragma once

#include "types.h"

// Resets the bounds to empty, then grows them by each point of each list (projected by
// fn_710007b430).
struct UnkVec2 {
    f32 x, y;
    UnkVec2(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkVec2(const UnkVec2& other) : x(other.x), y(other.y) {}
};
