#pragma once

#include "base/UnkValueType108.h"

struct UnkValue108 {
    union {
        u8 inline_data[8];
        u8* data;
    };
    UnkValueType108* type;  // 0x8
    u8 pad_0x10[0x4];
    u32 mode;  // 0x14
};
