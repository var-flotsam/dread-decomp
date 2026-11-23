#pragma once

#include "game/UnkModel_u207.h"
#include "game/UnkTU207.h"

struct UnkController_u207 {
    u8 pad_0x0[8];
    UnkModel_u207* model;                    // 0x8
    base::global::CRntVector<UnkEntry_u207> entries;  // 0x10
};
