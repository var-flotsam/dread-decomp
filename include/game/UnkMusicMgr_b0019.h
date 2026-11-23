#pragma once

#include "game/UnkEntryArray_b0019.h"

struct UnkMusicMgr_b0019 {
    u8 pad_0x0[0xaa];
    bool enabled;  // 0xaa
    u8 pad_0xab[0x1];
    s32 hold_count;  // 0xac
    u8 pad_0xb0[0x8];
    UnkEntryArray_b0019 entries;  // 0xb8
    u8 pad_0xd0[0x180];
    u32 slot_count;  // 0x250
};
