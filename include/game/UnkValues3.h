#pragma once

#include "types.h"

// Copy-assignment of a record of two arrays and four values: the first array (of three
// values per element) appends through its out-of-line PushBack; the second (of half-words)
// is copied as one block.
struct UnkValues3 {
    u32 values[3];

    UnkValues3(const UnkValues3& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }
};
