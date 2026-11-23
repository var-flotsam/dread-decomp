#pragma once

#include "game/UnkMinimapDrawList.h"

// The unit's statics, which the compiler merged into one block (addressed from one base): the map
// cell size at 0x71729cdaa4 (a guarded function-local static of fn_7100e91d68), its guard at
// 0x71729cdb10 and the four draw lists at 0x71729cdba0. Declared from the 8-aligned word before
// the cell size so the guard and the lists keep their natural alignment.
// The unit's statics, which the compiler merged into one block (addressed from one base), as
// src/game/CMinimapManager.cpp declares it.
struct UnkMinimapStatics {
    u8 unk_0x0[0x4];
    f32 cell_size;
    u8 unk_0x8[0x68];
    u64 cell_size_guard;
    u8 unk_0x78[0x88];
    UnkMinimapDrawList lists[4];
};
