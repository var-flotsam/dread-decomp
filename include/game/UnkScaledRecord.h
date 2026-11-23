#pragma once

#include "types.h"

struct UnkScaledRecord {
    void* vtable;
    f32 scale;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 vec[3];
    f32 field_0x24;
    f32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    u32 pad_0x34;
    s64 scaled;
    s32 fixed[3];
    u8 isZero;
    u8 field_0x4d;
};
