#pragma once

#include "types.h"

// An id, a value, an array of polymorphic records (assigned by fn_7100e4e898), flags, values
// and two arrays records.
struct UnkPolyVector {
    u64 unk_0x0[5];

    UnkPolyVector& operator=(const UnkPolyVector& other);
};
