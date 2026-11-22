#pragma once

#include "game/UnkTU1102.h"
#include "game/UnkTgmHolder_b205.h"

struct UnkTgmRef_b205 : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkTgmHolder_b205* target;
};
