#pragma once

#include "types.h"

// An item definition: a counted object with its name at +0x10.
struct UnkDropItemDef_ovb0136 {
    u8 pad_0x0[0x10];
    const char* name;
    u8 pad_0x18[0x2c];
    // A union member: the definition pointer is reloaded after the increment.
    union {
        u32 ref_count;  // 0x44
    };
};
