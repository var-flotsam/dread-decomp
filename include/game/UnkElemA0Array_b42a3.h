#pragma once

#include "game/UnkTU2200.h"
#include "types.h"

struct UnkElemA0_b42a3;

struct UnkElemA0Array_b42a3 {
    UnkElemA0_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
