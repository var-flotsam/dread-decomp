#pragma once

#include "types.h"

struct UnkRegistryNode_b0248;
class UnkRegistryHandler_b0248;

struct UnkRegistry_b0248 {
    u8 pad_0x0[0x18];
    UnkRegistryNode_b0248** buckets;  // 0x18
    u32 count;                        // 0x20
    u32 capacity;                     // 0x24
    u8 pad_0x28[0x38];
    void* context;                    // 0x60
    void (*on_error)(void* context, UnkRegistryHandler_b0248* handler, u32 result);  // 0x68
};
