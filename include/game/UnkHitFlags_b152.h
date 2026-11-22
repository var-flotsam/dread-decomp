#pragma once

#include "types.h"

struct UnkHitFlags_b152 {
    u32 bits;
    UnkHitFlags_b152(u32 b) : bits(b) {}
    UnkHitFlags_b152(const UnkHitFlags_b152& o) : bits(o.bits) {}
};
