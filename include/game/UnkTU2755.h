#pragma once

// UnkTU2755: functions of this translation unit that other files call (tools/tu_map.py unit 2755,
// 0x71011f6044-0x71011f688c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGuiItemState;
struct UnkGuiPos;
UnkGuiPos fn_71011f6240(const UnkGuiItemState* state, u32 h, u32 v);
