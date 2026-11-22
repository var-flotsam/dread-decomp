#pragma once

#include "game/UnkDrawParent_b260.h"
#include "game/UnkRenderer_b260.h"
#include "types.h"

struct UnkDrawer_b260 {
    u8 unk_0x0[8];
    UnkDrawParent_b260* parent;  // 0x8
    UnkRenderer_b260* renderer;  // 0x10
    u8 unk_0x18[8];
    u32 field_0x20;
};
