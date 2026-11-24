#pragma once

#include "game/UnkPtrArray_ovb0034.h"

struct UnkPtrArrays_ovb0034 {
    UnkPtrArray_ovb0034* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;
};
