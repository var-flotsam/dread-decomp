#pragma once

#include "types.h"

struct UnkChild;

struct UnkParent {
    u8 pad_0x0[0xc8];
    UnkChild** children;
    u8 pad_0xd0[8];
    u32 count;
};
