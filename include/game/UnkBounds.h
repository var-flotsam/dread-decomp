#pragma once

#include "types.h"

struct UnkBounds {
    f32 min_x;
    f32 min_y;
    f32 max_x;
    f32 max_y;

    UnkBounds() : min_x(3.4028235e38f), min_y(3.4028235e38f), max_x(-3.4028235e38f), max_y(-3.4028235e38f) {}
};
