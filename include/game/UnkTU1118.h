#pragma once

// UnkTU1118: functions of this translation unit that other files call (tools/tu_map.py unit 1118,
// 0x71006fc4fc-0x71006ff6ec). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAIState_l6fe894;
struct UnkReadyObject_l6fe894;
struct UnkNavKind2_b66a4;
struct UnkNavPath_b66a4;
void fn_71006fc7c4(UnkAIState_l6fe894* self, UnkReadyObject_l6fe894* ready);
UnkNavPath_b66a4* fn_71006fce10(UnkNavKind2_b66a4* obj);
