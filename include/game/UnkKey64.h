#pragma once

#include "types.h"

// A 64-bit key compared through its own operator (a plain u64 index loads it earlier).
struct UnkKey64 {
    u64 value;
    bool operator!=(const UnkKey64& o) const { return value != o.value; }
};
