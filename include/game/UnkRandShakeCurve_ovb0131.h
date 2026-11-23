#pragma once

#include "types.h"

struct UnkRandShakeCurve_ovb0131 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x44 - 0x18];
    union {
        u32 use_count;
    };
};
