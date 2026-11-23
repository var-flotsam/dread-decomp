#pragma once

#include "game/UnkXformNode.h"

// A mode of the movement (the pointers at +0x2d1c): its kind and, for kind 6, a position at +0x19c.
struct UnkMode_b152 {
    void** vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
    u8 pad_0x14[0x188];
    UnkVec3f field_0x19c;
};
