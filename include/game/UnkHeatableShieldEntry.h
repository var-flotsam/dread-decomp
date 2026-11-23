#pragma once

// One 0xd0-byte entry of CHeatableShieldComponent's array (include/game/UnkHeatableShieldEntries.h); its fuller
// view is src/game/CHeatableShieldComponent/CHeatableShieldEntry_Reset_A5_b71.cpp's. Names and layouts are
// placeholders.
#include "types.h"

struct UnkHeatableShieldEntry {
    u8 data[0xc8];
    void* owner;  // 0xc8
};
