#pragma once

#include "base/math/UnkSegment2D.h"
#include "types.h"

// An axis-aligned box: its corners go (x0, y0), (x0, y1), (x1, y1), (x1, y0).
// An axis-aligned box: its lower and upper corners.
struct UnkBox2D {
    f32 x0;
    f32 y0;
    f32 x1;
    f32 y1;
    UnkVec2Result Corner(s32 index) const asm("fn_710010d02c");
};
