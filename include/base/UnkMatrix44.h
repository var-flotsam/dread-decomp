#pragma once

// A 4x4 matrix of f32, row-major (the transform matrix of UnkXform, game/UnkXformNode.h).
#include "types.h"

struct UnkMatrix44 {
    f32 m[16];
};
