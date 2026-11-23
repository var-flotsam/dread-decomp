#pragma once

#include "types.h"

// A string id's pooled data as these reads see it.
struct UnkDlgIdData_ovb0069 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};
