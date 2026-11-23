#pragma once

#include "base/global/CRntVector.h"
#include "game/UnkTU212.h"

// A copy of a node array: the same capacity from the same allocator.
struct UnkNodeArrayCopy {
    UnkNamedNode** data;
    u32 count;
    base::UnkAllocator* allocator;

    explicit UnkNodeArrayCopy(const base::global::CRntVector<UnkNamedNode*>& other) {
        allocator = other.allocator;
        data = other.capacity ? static_cast<UnkNamedNode**>(
                                    allocator->vf_0x40((u64)other.capacity * sizeof(UnkNamedNode*), 8, other.flags))
                              : nullptr;
        __builtin_memcpy(data, other.data, (u64)other.count * sizeof(UnkNamedNode*));
        count = other.count;
    }
    UnkNamedNode** begin() { return data; }
    UnkNamedNode** end() { return data + count; }
    ~UnkNodeArrayCopy() {
        if (data)
            allocator->vf_0x48(data);
    }
};
