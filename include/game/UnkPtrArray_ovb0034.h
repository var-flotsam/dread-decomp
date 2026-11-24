#pragma once

#include "base/global/CRntVector.h"

// An array of pointers that owns its storage (0x28 bytes, laid out as base::global::CRntVector).
struct UnkPtrArray_ovb0034 {
    void** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::IAllocator* allocator;

    UnkPtrArray_ovb0034(UnkPtrArray_ovb0034&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    ~UnkPtrArray_ovb0034() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
