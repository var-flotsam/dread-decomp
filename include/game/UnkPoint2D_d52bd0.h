#pragma once

#include "types.h"

// A 2D point returned through x8 (not trivially copyable).
// A 2D point, copied member by member.
struct UnkPoint2D_d52bd0 {
    f32 x, y;

    UnkPoint2D_d52bd0(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkPoint2D_d52bd0(const UnkPoint2D_d52bd0& o) : x(o.x), y(o.y) {}
};
