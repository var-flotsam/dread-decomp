#pragma once

#include "types.h"

struct UnkOwner;
struct UnkFlagObject;
struct UnkTarget;

struct UnkSelf {
    u8 pad_0x0[0x30];
    UnkOwner* field_0x30;
    u8 pad_0x38[0x664];
    u32 field_0x69c;
    u8 pad_0x6a0;
    u8 field_0x6a1;
    u8 pad_0x6a2[0x13e76];
    s32 field_0x14518;
    u8 pad_0x1451c[0x3ec];
    UnkFlagObject* field_0x14908;
    u8 pad_0x14910[0xa8];
    UnkTarget* field_0x149b8;
    u8 pad_0x149c0[0x1e4];
    f32 field_0x14ba4;
};
