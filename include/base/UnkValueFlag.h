#pragma once

#include "types.h"

// A value and a flag, and two values, assigned member by member.
struct UnkValueFlag {
    u32 value;
    bool flag;

    // Copies the five data bytes (not the padding).
    UnkValueFlag& operator=(const UnkValueFlag& other) {
        __builtin_memcpy(this, &other, 5);
        return *this;
    }
};
