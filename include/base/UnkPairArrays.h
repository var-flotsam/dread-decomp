#pragma once

// A pair of parallel arrays of 8-byte elements (keys, values) that share a count, a capacity and
// a growth step, allocated from an allocator with the given flags. Names are placeholders. The
// layout is embedded at +0xf0 of the object behind fn_71002d2740's owner and is also built on its own
// (fn_71002d4e50).

#include "base/UnkVector.h"
#include "types.h"

namespace base {

struct UnkPairElem {
    u32 lo;
    u32 hi;
};

struct UnkPairArrays {
    u64* keys;
    u64* values;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    UnkAllocator* allocator;

    // Grows both arrays (the old contents are copied bitwise) when the capacity has to change.
    void Grow(u32 new_capacity) {
        u64 size = (u64)new_capacity << 3;
        u64* new_keys = static_cast<u64*>(allocator->vf_0x40(size, 8, flags));
        u64* new_values = static_cast<u64*>(allocator->vf_0x40(size, 8, flags));
        if (keys) {
            __builtin_memcpy(new_keys, keys, (u64)count << 3);
            allocator->vf_0x48(keys);
        }
        u64* old_values = values;
        if (old_values) {
            for (u32 i = 0; i < count; i++)
                __builtin_memcpy(&new_values[i], &old_values[i], sizeof(u64));
            allocator->vf_0x48(values);
        }
        keys = new_keys;
        values = new_values;
    }
};

}  // namespace base
