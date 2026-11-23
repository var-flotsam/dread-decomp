#pragma once

#include "game/UnkQuadRender_ovb0259.h"

struct UnkQuadNode_ovb0259 {
    u8 pad_0x0[0x18];
    u8 node[0x160];  // 0x18
    UnkQuadRender_ovb0259* render;  // 0x178
};
