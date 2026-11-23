#pragma once

#include "game/UnkOBoxPlacement.h"

struct UnkOBoxShape2D {
    u8 pad_0x0[0xa8];
    UnkOBoxPlacement placement;  // 0xa8
};
