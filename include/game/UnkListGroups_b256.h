#pragma once

#include "game/UnkListGroup_b256.h"

struct UnkListGroups_b256 {
    u8 pad_0x0[0x10];
    u32 count;                // 0x10
    UnkListGroup_b256* head;  // 0x18
};
