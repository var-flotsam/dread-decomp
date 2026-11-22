#pragma once

#include "base/global/CRntVector.h"
#include "types.h"

// ---- fn_71002a1ff8 ----
// A copy of a growable array of 16-bit values: same settings, own storage.
struct UnkU16Array_b0228 {
    u16* data;                      // 0x0
    u32 count;                      // 0x8
    u32 capacity;                   // 0xc
    u32 step;                       // 0x10
    bool field_0x14;                // 0x14
    u32 flags;                      // 0x18
    base::UnkAllocator* allocator;  // 0x20

    UnkU16Array_b0228() = default;
    UnkU16Array_b0228(const UnkU16Array_b0228& o) {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = o.step;
        field_0x14 = o.field_0x14;
        flags = o.flags;
        allocator = o.allocator;
        u32 cap = o.capacity;
        u16* p = nullptr;
        if (cap) {
            p = static_cast<u16*>(allocator->vf_0x40((u64)cap * 2, 8, flags));
            if (data) {
                __builtin_memcpy(p, data, (u64)count * 2);
                allocator->vf_0x48(data);
            }
            data = p;
            capacity = cap;
        }
        __builtin_memcpy(p, o.data, (u64)o.count * 2);
        count = o.count;
    }
};
