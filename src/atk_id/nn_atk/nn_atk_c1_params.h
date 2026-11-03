// Slice-1 helpers for nn::atk's optional parameters (the layout src/atk_id/nn_atk_b34a3.cpp describes):
// a flag word, then one 4-byte slot for each of its low bits that is set, in bit order.
#pragma once
#include "types.h"

struct UnkAtkC1Params {
    u32 flags;
    u32 slots[1];

    bool Has(u32 bit) const { return static_cast<u64>(flags) >> bit & 1; }
    const u8* Slot(u32 bit) const {
        u64 count = 0;
        for (u32 i = 0; i < bit; ++i)
            if (Has(i))
                ++count;
        return reinterpret_cast<const u8*>(reinterpret_cast<const u32*>(this) + count + 1);
    }
};

// A reference: a 4-byte header and a signed offset from the reference itself.
struct UnkAtkC1Ref {
    u32 field_0x0;
    s32 offset;
    const char* Target() const { return reinterpret_cast<const char*>(this) + offset; }
};
