#pragma once

// UnkTU2517: functions of this translation unit that other files call (tools/tu_map.py unit 2517,
// 0x7100d6936c-0x7100d6968c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSegmentShape;
void fn_7100d6936c(void* shape);  // The SEGMENT2D shape's constructor.
void fn_7100d69400(UnkSegmentShape* self);
