#pragma once

#include "game/UnkGuiAnchor.h"
#include "types.h"

// The state fn_71011f61bc reads: its angle at +0x10 and scale at +0x1c (src/game/UnkGuiItemState.cpp
// lays it out in full).
struct UnkGuiItemState {
    UnkGuiAnchor anchor;
    f32 angle;
    f32 width;
    f32 height;
    f32 scale_x;
    f32 scale_y;
};
