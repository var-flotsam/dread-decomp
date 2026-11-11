#pragma once

// UnkTU307: functions of this translation unit that other files call (tools/tu_map.py unit 307,
// 0x7100215dac-0x710021685c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkBounded;
void fn_7100215f60(UnkBounded* self, const void* point);  // Grows the bounds to hold a point.
