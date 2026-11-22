#pragma once

#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

struct UnkTuneStr_b0019;

struct UnkMusicEntry_b0019 {
    u8 pad_0x0[0x28];
    UnkPathVec_b0019 paths;  // 0x28
    u8 pad_0x50[0x8];
    UnkTuneStr_b0019* name;  // 0x58
};
