#pragma once

// CBombMovement: functions of this translation unit that other files call (tools/tu_map.py unit 1610,
// 0x7100aa9170-0x7100aac8c8). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkFanBombList_ovb0122;
struct UnkPbMovementBase_ovb0015;
UnkFanBombList_ovb0122* fn_7100aa9ecc();
void fn_7100aa9f3c(UnkPbMovementBase_ovb0015*);  // Derived classes call it as their base's destructor.
