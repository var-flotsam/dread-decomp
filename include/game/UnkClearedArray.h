#pragma once

#include "types.h"

// Clears (destroying the elements, keeping the storage) of arrays of ids (at 0xd642f0 of an
// owner) and of ids with two values.
template <class T>
struct UnkClearedArray {
    T* data;
    u32 count;

    void Clear() {
        for (u64 i = 0; i < count; ++i)
            data[i].~T();
        count = 0;
    }
};
