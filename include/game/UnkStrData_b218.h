#pragma once

#include "types.h"

struct UnkStrData_b218 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
    u64 hash() const { return *reinterpret_cast<const u64*>(&hash_lo); }
};
