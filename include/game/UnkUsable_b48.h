#pragma once

#include "game/UnkUsableValue_b48.h"

struct UnkUsable_b48 {
    u8 pad_0x0[0x4d0];
    UnkUsableValue_b48* values;
    u64* keys;
    u32 count;
};
