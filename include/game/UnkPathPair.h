#pragma once

#include "types.h"

// An 8-byte element of two words, copied member by member.
struct UnkPathPair {
    u32 value[2];

    // Each word is copied bitwise.
    void CopyFrom(const UnkPathPair& other) {
        __builtin_memcpy(&value[0], &other.value[0], sizeof(u32));
        value[1] = other.value[1];
    }
    UnkPathPair(const UnkPathPair& other) { CopyFrom(other); }
    UnkPathPair(UnkPathPair&& other) { CopyFrom(other); }
};
