#pragma once

#include "types.h"

// A plane: its normal and its distance term.
struct UnkPlane {
    f32 x;
    f32 y;
    f32 z;
    f32 d;
};
