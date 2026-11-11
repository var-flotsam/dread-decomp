#pragma once

// UnkTU2557: functions of this translation unit that other files call (tools/tu_map.py unit 2557,
// 0x7100e0f028-0x7100e140c0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkLoadingScreen;
void fn_7100e0f89c(UnkLoadingScreen* self);  // Constructor.
void fn_7100e0f028();
void fn_7100e1389c() __attribute__((visibility("hidden")));
void fn_7100e138a8() __attribute__((visibility("hidden")));
