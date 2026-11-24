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
    f32 field_0x8;  // the cell size fn_7100e95480 keeps (guarded by field_0x98)
    u8 unk_0xc[0x64];
    u64 cell_size_guard;
    u8 unk_0x78[0x20];
    u64 field_0x98;  // field_0x8's guard
    u8 unk_0xa0[0x60];
    UnkMinimapDrawList lists[4];
};
