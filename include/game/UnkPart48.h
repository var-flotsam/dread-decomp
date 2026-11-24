#pragma once

#include "game/UnkTU83.h"
#include "types.h"

// base::global::CRntVector's layout, copied out of line.
struct UnkPart48 {
    union {
        void* data;
    };
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    // base::global::CRntVector's defaults.
    __attribute__((always_inline)) UnkPart48()
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf),
          allocator(GetDefaultAllocator()) {}
    UnkPart48(const UnkPart48& other);
    __attribute__((always_inline)) UnkPart48(UnkPart48&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    __attribute__((always_inline)) ~UnkPart48() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
