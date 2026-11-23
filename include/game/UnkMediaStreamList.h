#pragma once

#include "game/UnkMediaStream.h"
#include "types.h"

struct UnkMediaStreamList {
    u8 pad_0x0[0x30];
    s32 kind;  // 0x30
    u8 pad_0x34[0x2c];
    UnkMediaStream** items;  // 0x60
    u32 count;               // 0x68
};
