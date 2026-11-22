#pragma once

#include "base/global/CRntVector.h"

template <class T>
struct UnkCopiedVector {
    T* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    __attribute__((always_inline)) void Reallocate(u32 new_capacity) {
        T* p = static_cast<T*>(allocator->vf_0x40((u64)new_capacity * sizeof(T), 8, flags));
        if (data) {
            T* src = data;
            T* dst = p;
            for (u32 i = 0; i < count; ++i, ++src, ++dst)
                new (dst) T(*src);
            allocator->vf_0x48(data);
        }
        data = p;
        capacity = new_capacity;
    }

    __attribute__((always_inline)) UnkCopiedVector& operator=(const UnkCopiedVector& other) {
        if (this != &other) {
            count = 0;
            u32 reserve = other.capacity;
            if (capacity < reserve)
                Reallocate(reserve);
            for (const T *it = other.data, *end = it + other.count; it != end; ++it) {
                u32 n = count;
                u32 cap = capacity;
                u32 new_capacity = cap;
                if (cap <= n && step != 0)
                    new_capacity = (n / step + 1) * step;
                if ((s32)n >= (s32)new_capacity)
                    for (;;) {
                    }
                if (cap < new_capacity)
                    Reallocate(new_capacity);
                new (&data[count]) T(*it);
                ++count;
            }
        }
        return *this;
    }
};
