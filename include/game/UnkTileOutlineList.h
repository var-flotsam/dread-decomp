#pragma once

#include "types.h"

// The polygon list it builds at +0xa8 (constructed by fn_7100123eb4, as src/game/CBreakableTileGroup.cpp's
// scratch list).
// A scratch outline list fn_7100ca1180 builds and destroys (fn_7100123eb4, fn_7100123fb0).
struct UnkTileOutlineList {
    u8 data[0x40];
};
