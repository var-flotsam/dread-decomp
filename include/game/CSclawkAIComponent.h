#pragma once

// CSclawkAIComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1005,
// 0x710067c150-0x710067d7d0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSclawkAIComponent;

// Enters `state`: tells the owner's animation, picks a random wait (state 0) or starts the time stored for
// state 2, and shows the owner's part while in state 2.
void fn_710067c478(UnkSclawkAIComponent* self, u32 state);
