#pragma once

#include "game/UnkListEntry_b256.h"

struct UnkListGroup_b256 {
    u8 pad_0x0[8];
    u32 count;                  // 0x8
    UnkListEntry_b256* head;    // 0x10
    u8 pad_0x18[0x58];
    void* key;                  // 0x70
    UnkListGroup_b256* next;    // 0x78
};
