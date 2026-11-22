#pragma once

#include "game/UnkRange.h"
#include "types.h"

struct UnkRanges;

struct UnkRanges {
    u8 pad_0x0[0x8];
    UnkRange ranges[3];  // 0x8
};
