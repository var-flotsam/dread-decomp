#pragma once

// UnkTU181: functions of this translation unit that other files call (tools/tu_map.py unit 181,
// 0x710015e4b0-0x710015f0f8). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMatrix44;
void fn_710015e4b0(void* state, u32 stage, u64 location, u32 count, u32 a, const UnkMatrix44* values, u32 b);
