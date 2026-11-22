#pragma once

#include "game/UnkOwner_af41.h"

struct UnkSelf_af41 {
    u8 pad_0x0[0x30];
    UnkOwner_af41* owner;
    u8 pad_0x38[0x3ec];
    u8 has_pos;  // 0x424
    u8 pad_0x425[3];
    f32 pos[3];  // 0x428
};
