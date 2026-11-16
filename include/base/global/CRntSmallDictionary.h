#pragma once

// The engine's small dictionary, base::global::CRntSmallDictionary (the name reflection gives it): the
// values and the keys in two parallel arrays of the same capacity, searched in order. Only what the
// reflection descriptors use is written; its growth, copies and erase are still by address.

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

    V* End() { return &values[count]; }
    const V* End() const { return &values[count]; }

    // The value under key, or End().
    V* Find(const K& key) {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
    const V* Find(const K& key) const {
        for (u32 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }

    // Destroys the keys, then the values, keeping the arrays.
    void Clear() {
        if (keys) {
            for (u64 i = 0; i < count; ++i)
                keys[i].~K();
        }
        if (values) {
            for (u64 i = 0; i < count; ++i)
                values[i].~V();
        }
        count = 0;
    }

    // Takes over other's arrays, leaving it empty; the arrays it held are not freed. The fields are taken
    // in the original's order.
    CRntSmallDictionary& operator=(CRntSmallDictionary&& other) {
        if (this != &other) {
            Clear();
            allocator = other.allocator;
            keys = other.keys;
            values = other.values;
            capacity = other.capacity;
            count = other.count;
            flags = other.flags;
            step = other.step;
            other.count = 0;
            other.values = nullptr;
            other.keys = nullptr;
        }
        return *this;
    }

    // Equal when the counts are and every key of this one is in other with an equal value.
    bool operator==(const CRntSmallDictionary& other) const {
        if (count != other.count)
            return false;
        for (u32 i = 0; i < count; ++i) {
            const V* v = other.Find(keys[i]);
            if (v == other.End() || !(*v == values[i]))
                return false;
        }
        return true;
    }
};
static_assert(sizeof(CRntSmallDictionary<u64, u64>) == 0x28, "CRntSmallDictionary layout");

}  // namespace base::global
