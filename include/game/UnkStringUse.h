#pragma once

#include "types.h"

// The string data's use count, a union member like ref_count (no type-based alias info, so the
// id is reloaded after the increment; see fn_7100ae9a10).
// The string data's use count, a union member like ref_count (no type-based alias info, so
// the id is reloaded after the increment).
struct UnkStringUse {
    u8 unk_0x0[0x44];
    union {
        u32 use_count;
    };
};
