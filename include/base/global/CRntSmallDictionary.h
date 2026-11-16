#pragma once

// The engine's small dictionary, base::global::CRntSmallDictionary (the name reflection gives it): the
// values and the keys in two parallel arrays of the same capacity, searched in order. Only what the
// reflection descriptors use is written; its growth, copies and removal are still by address.

#include "base/global/CRntVector.h"
#include "types.h"

namespace base::global {

template <class K, class V>
struct CRntSmallDictionary {
    using key_type = K;
    using value_type = V;

    V* values = nullptr;
    K* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;  // capacity grows by this many entries
    u32 flags;      // passed to the allocator
    UnkAllocator* allocator = fn_7100083990();

    explicit CRntSmallDictionary(u32 flags) : flags(flags) {}

    // Takes over other's arrays, leaving it empty.
    CRntSmallDictionary(CRntSmallDictionary&& other)
        : values(other.values), keys(other.keys), count(other.count), capacity(other.capacity),
          step(other.step), flags(other.flags), allocator(other.allocator) {
        other.count = 0;
        other.values = nullptr;
        other.keys = nullptr;
    }
};
static_assert(sizeof(CRntSmallDictionary<u64, u64>) == 0x28, "CRntSmallDictionary layout");

}  // namespace base::global
