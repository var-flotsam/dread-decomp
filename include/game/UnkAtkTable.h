#pragma once

#include "game/UnkAtkEntry.h"

struct UnkAtkTable {
    u8 pad_0x0[0x10];
    u16 count;  // 0x10
    u16 pad_0x12;
    UnkAtkEntry entries[1];  // 0x14
};
