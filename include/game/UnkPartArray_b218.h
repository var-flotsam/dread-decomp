#pragma once

#include "game/UnkTU83.h"

// The array copied out of line.
struct UnkPartArray_b218 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkPartArray_b218(u32 f)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(f), allocator(GetDefaultAllocator()) {}
    UnkPartArray_b218(const UnkPartArray_b218& other) {
        fn_71000cbae4(reinterpret_cast<UnkPart48*>(this), reinterpret_cast<const UnkPart48*>(&other));
    }
};
