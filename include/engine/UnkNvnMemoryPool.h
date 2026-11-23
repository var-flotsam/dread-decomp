#pragma once

#include "types.h"

// A memory pool with its storage (the type fn_71001587f0 passes as UnkPool).
struct UnkNvnMemoryPool {
    u64 field_0x0;
    u8 pool[0x100];  // the NVN memory pool
    void* memory;
    u64 size;
    s32 flags;
    bool initialised;
};
