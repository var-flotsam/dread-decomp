#pragma once

#include "game/UnkTU58.h"
#include "types.h"

// The sound engine's heaps: a sub-allocator over a static block, the engine's own heap object over
// the same memory at +0x40, and a hook run after every allocation (with the heap; it may replace
// the result at +0x48).
struct UnkAtkHeap {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 heap[8];                       // 0x40, nn::atk's
    void* last;                       // 0x48
    void (*hook)(void* arg, UnkAtkHeap* heap);
    void* hook_arg;
    u64 field_0x60;
    u64 field_0x68;
};
