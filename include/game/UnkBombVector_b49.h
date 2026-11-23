#pragma once

#include "base/global/CRntVector.h"
#include "game/UnkBombValue_b49.h"

struct UnkBombVector_b49 {
    UnkBombValue_b49* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool grows;
    u32 flags;
    base::UnkAllocator* allocator;
};
