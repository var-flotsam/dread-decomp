#pragma once

#include "game/UnkRcRef.h"
#include "game/UnkTU341.h"

struct UnkRcRefArray {
    UnkRcRef* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
