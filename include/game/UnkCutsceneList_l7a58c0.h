#pragma once

#include "game/UnkTU2661.h"

struct UnkCutsceneList_l7a58c0 {
    u8 pad_0x0[0x130];
    u32 count;  // 0x130
    u8 pad_0x134[0x44];
    UnkCutsceneEntry_A5_b65* current;  // 0x178
};
