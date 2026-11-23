#pragma once

#include "types.h"

// A resolved value (0x30 bytes), as src/base/reflection/UnkReflResolve.cpp defines it, built here from a
// reflected value.
// Two words assigned only from another object.
struct UnkResolvedPair {
    u64 a;
    u64 b;

    UnkResolvedPair& operator=(const UnkResolvedPair& o) {
        if (this != &o) {
            a = o.a;
            b = o.b;
        }
        return *this;
    }
};
