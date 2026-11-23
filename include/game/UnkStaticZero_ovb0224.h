#pragma once

#include "types.h"

// A zero whose constructor isn't constexpr, so the statics are guarded.
struct UnkStaticZero_ovb0224 {
    u32 value;
    UnkStaticZero_ovb0224() : value(0) {}
};
