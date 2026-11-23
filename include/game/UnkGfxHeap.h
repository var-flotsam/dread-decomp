#pragma once

#include "types.h"

// A heap of texture memory: its NVN memory pool first, the pool's base address at +0x100.
struct UnkGfxHeap {
    u8 pool[0x100];
    u64 base;  // 0x100
    u8 pad_0x108[0x18];
    void* space;  // 0x120
};
