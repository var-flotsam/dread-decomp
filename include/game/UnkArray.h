#pragma once

#include "base/global/CRntVector.h"

template <class T>
struct UnkArray {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) UnkArray() : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(0xf) {
        allocator = GetDefaultAllocator();
    }

    T* begin() { return data; }
    T* end() { return data + count; }

    ~UnkArray() {
        if (data) {
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};
