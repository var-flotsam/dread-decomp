#pragma once

#include "base/global/CRntVector.h"

// A table of 0x60-byte records holding two arrays (base::global::CRntVector's layout, freed when
// destroyed) and four words.
struct UnkFreedArray {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) ~UnkFreedArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
