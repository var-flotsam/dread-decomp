#pragma once

// The result of a collision test (the collision world's dispatchers, the shape tester fn_7100d51368, the pair
// handlers behind fn_7100d541c0): bit 0 a hit, bit 1 stop. 4 bytes, returned in memory: its copy is
// user-written. The callers merge results by or-ing the bits. Names are placeholders.
#include "types.h"

struct UnkColliderResult {
    u32 bits;
    UnkColliderResult() : bits(0) {}
    UnkColliderResult(u32 b) : bits(b) {}
    UnkColliderResult(const UnkColliderResult& o) { bits = o.bits; }
};
