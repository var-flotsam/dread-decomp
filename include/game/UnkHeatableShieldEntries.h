#pragma once

#include "types.h"

struct UnkHeatableShieldEntry;

struct UnkHeatableShieldEntries {
    u8 pad_0x0[0x30];
    void* owner;
    u8 pad_0x38[0x8];
    UnkHeatableShieldEntry* data;
    u8 pad_0x48[0x8];
    u32 count;
};
