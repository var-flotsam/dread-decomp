#pragma once

#include "types.h"

// A query box (as in src/game_hi/fn_7100d56604.cpp): two corners in any order, two more words.
struct UnkQueryBox {
    f32 minX, minY, maxX, maxY;
    u32 field_0x10;
    u32 field_0x14;
    UnkQueryBox(const UnkQueryBox& o)
        : minX(o.minX), minY(o.minY), maxX(o.maxX), maxY(o.maxY), field_0x10(o.field_0x10), field_0x14(o.field_0x14) {}
};
