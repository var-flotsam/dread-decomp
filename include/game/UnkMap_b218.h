#pragma once

#include "game/UnkTU83.h"

template <class Node>
struct UnkMap_b218 {
    base::IAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    Node** buckets;                 // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40
};
