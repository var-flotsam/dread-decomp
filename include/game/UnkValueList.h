#pragma once

#include "types.h"

// A list of values: the first, the second (0 when there is only one) and the rest.
struct UnkValueList {
    u32 count = 0;
    u32 kind = 0x7fffffff;
    f32 second = 0.0f;
    u32 first = 0;
    const u32* rest = nullptr;

    void Set(u32 n, const u32* values) {
        count = n;
        kind = 2;
        second = n ? reinterpret_cast<const f32*>(values)[1] : 0.0f;
        first = values[0];
        rest = n ? values + 1 : nullptr;
    }
};
