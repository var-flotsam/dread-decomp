#pragma once

#include "types.h"

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkXformStaticZero32 {
    u32 value;
    UnkXformStaticZero32() : value(0) {}
};
