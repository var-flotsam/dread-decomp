#pragma once

#include "types.h"

struct UnkSlomoSettings {
    u8 pad_0x0[0x10];
    u8 body[0x34];
    // A union member: no type-based alias info, so the settings pointer is reloaded after the increment.
    union {
        s32 ref_count;  // 0x44
    };
};
