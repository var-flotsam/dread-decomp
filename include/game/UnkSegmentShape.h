#pragma once

#include "types.h"

// A collision shape (0xc0 bytes) built from a segment and its destructor, and the collision query
// (declared as src/game/CLogicDroppedItemManager.cpp does).
// A collision shape (0xc0 bytes) built from a segment (fn_7100d693ac) and destroyed by
// fn_7100d69400.
struct UnkSegmentShape {
    u8 data[0xc0];
};
