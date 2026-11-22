#pragma once

#include "types.h"

// Declared as src/game/CBillboardComponent/CBillboardComponent_b40.cpp does: the destructor of
// that unit's 0x148-byte entries.
struct UnkEntry_bbd {
    u8 data[0x148];
    ~UnkEntry_bbd() asm("fn_7100745b24");
};
