#pragma once

#include "types.h"

// Two values copied one by one (as plain words: the original reloads the count after each
// element).
struct UnkValuePair {
    u32 values[2];

    UnkValuePair(const UnkValuePair& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
    }
};
