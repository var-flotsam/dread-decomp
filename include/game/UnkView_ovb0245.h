#pragma once

#include "game/UnkXformNode.h"

struct UnkView_ovb0245 {
    u8 pad_0x0[0x30];
    UnkVec3f position;  // 0x30
    UnkVec3f scale;     // 0x3c
    u8 pad_0x48[0x11c];
    UnkMatrix44 matrix;  // 0x164
    u8 pad_0x1a4[8];
    u8 matrix_valid;  // 0x1ac
};
