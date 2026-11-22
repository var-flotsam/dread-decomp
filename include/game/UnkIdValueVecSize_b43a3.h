#pragma once

#include "types.h"

// The value is 24 bytes in 0x7101cab338's class, 16 in the others'.
template <char* Vtable>
struct UnkIdValueVecSize_b43a3 {
    static constexpr u64 value = 16;
};
