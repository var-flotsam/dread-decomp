#pragma once

// UnkTU365: functions of this translation unit that other files call (tools/tu_map.py unit 365,
// 0x7100288e2c-0x71002ac2f4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAssignedSlot;
struct UnkBreakablePart;
struct UnkBreakableTarget;
struct UnkTracker;
void fn_71002a5d1c(UnkAssignedSlot* self, const UnkAssignedSlot* other);
UnkBreakableTarget** fn_71002a76a4(UnkBreakablePart* part);
UnkTracker* fn_710028aca8(void* link);
