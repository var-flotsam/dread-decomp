#pragma once

#include "types.h"

struct UnkValues2Memberwise {
    u32 a;
    u32 b;

    UnkValues2Memberwise& operator=(const UnkValues2Memberwise& other) {
        a = other.a;
        b = other.b;
        return *this;
    }
};
