#pragma once

#include "game/UnkTU83.h"

// base::global::CRntVector's layout; the elements of the u32 and u8 arrays are copied inline.
template <class T>
struct UnkArray_b218 {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkArray_b218(u32 f)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(f), allocator(fn_7100083990()) {}

    __attribute__((always_inline)) UnkArray_b218(const UnkArray_b218& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14),
          flags(other.flags), allocator(other.allocator) {
        u32 n = other.capacity;
        if (capacity < n) {
            T* p = static_cast<T*>(allocator->vf_0x40((u64)n * sizeof(T), 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * sizeof(T));
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = n;
        }
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(T));
        count = other.count;
    }
};
