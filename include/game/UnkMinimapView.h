#pragma once

#include "game/UnkTU287.h"

// The map's on-screen viewport: a node at 0x18 (fn_710020a5a0 scales it, fn_710020a4c4 places it), a
// dirty flag and the view whose rectangle (position, size) fn_71001c36c0 applies.
// The view (the same definition as src/game/CMinimapManager.cpp's).
struct UnkMinimapView {
    u8 unk_0x0[0x90];
    base::math::CVector2D pos;
    base::math::CVector2D size;
};
