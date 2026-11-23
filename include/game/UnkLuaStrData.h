#pragma once

#include "types.h"

struct UnkLuaStrData {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};
