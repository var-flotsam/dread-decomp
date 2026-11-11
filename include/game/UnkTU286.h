#pragma once

// UnkTU286: functions of this translation unit that other files call (tools/tu_map.py unit 286,
// 0x71002094dc-0x710020a18c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFadeColor;
void fn_7100209968(void* node, f32 duration, const UnkFadeColor* to, const UnkFadeColor* from);
void fn_7100209fec(void* node, bool visible);
