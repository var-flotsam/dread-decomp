#pragma once

#include "types.h"

// A vector with a fixed capacity, its elements stored in place after the count.
template <class T, u32 N>
struct UnkFixedVector_l808234 {
    u32 count;
    u8 storage[sizeof(T) * N];

    UnkFixedVector_l808234() : count(0) {}

    T* items() { return reinterpret_cast<T*>(storage); }

    // The element at `i`, default-constructing the ones up to it when the vector is shorter.
    T& At(u32 i) {
        u32 n = i + 1;
        if (n > count) {
            for (u32 j = count; j < n; ++j)
                new (&items()[j]) T();
            count = n;
        }
        return items()[i];
    }

    void Clear() {
        for (u32 i = 0; i < count; ++i)
            items()[i].~T();
        count = 0;
    }

    ~UnkFixedVector_l808234() { Clear(); }
};
