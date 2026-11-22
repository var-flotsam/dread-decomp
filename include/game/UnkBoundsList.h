#pragma once

#include "types.h"

struct UnkBoundsItem;

struct UnkBoundsList {
    u8 unk_0x0[0x34];
    u32 count;
    UnkBoundsItem* items;
};
