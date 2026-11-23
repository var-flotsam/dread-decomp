#pragma once

#include "types.h"

// A point of a display object's anchor state (the block at +0x78) by horizontal and vertical
// anchor, as src/game/GUI/CMapMenuController/CMapMenuController_BindWidgets.cpp declares it.
// A 2D position returned in memory (its user-provided copy keeps it out of registers).
// A point of a display object's anchor state, as fn_71011f6240 returns it.
// A 2D position returned in memory (src/game/UnkGuiAnchor.cpp).
// From src/game/UnkGuiAnchor.cpp (the same definitions).
struct UnkGuiPos {
    f32 x;
    f32 y;

    UnkGuiPos(f32 x_, f32 y_) : x(x_), y(y_) {}
    UnkGuiPos(const UnkGuiPos& other) : x(other.x), y(other.y) {}
    UnkGuiPos& operator=(const UnkGuiPos& other) = default;
};
