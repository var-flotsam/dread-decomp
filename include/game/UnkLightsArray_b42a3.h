#pragma once

#include "base/global/CRntVector.h"
#include "game/UnkLightsElem_b42a3.h"

struct UnkLightsArray_b42a3 {
    UnkLightsElem_b42a3* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};
