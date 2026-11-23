#pragma once

#include "game/logic/collision/CShape.h"

// An array-like global: three zero words, a growth step of 10, flags 0xe and the default
// allocator.
struct UnkArray_71729ce018 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;
    base::UnkAllocator* allocator;
};
