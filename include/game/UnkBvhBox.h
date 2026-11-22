#pragma once

#include "types.h"

// A shape placed in a tree query (as in src/game_hi/fn_7100d5bfbc.cpp): its cached 2D bounds,
// refreshed through vf_0x98 when marked dirty.
struct UnkBvhBox {
    f32 minX, minY, maxX, maxY;
};
