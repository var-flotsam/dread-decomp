#pragma once

// UnkLoadingMask: one definition of a type that several sources had each written out with the same
// text (tools/struct_migrate.py). Names and layouts are placeholders.
#include "types.h"

struct UnkLoadingMask {
    u32 bits;
    UnkLoadingMask() : bits(0) {}
    UnkLoadingMask(u32 b) : bits(b) {}
    UnkLoadingMask(const UnkLoadingMask& o) : bits(o.bits) {}
};
