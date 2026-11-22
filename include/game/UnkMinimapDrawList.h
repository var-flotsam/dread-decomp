#pragma once

#include "types.h"

// A draw list (0x28 bytes): its entries and their count, reset at the start of each pass.
struct UnkMinimapDrawList {
    void* data;
    u32 count;
    u8 unk_0xc[0x1c];
};
