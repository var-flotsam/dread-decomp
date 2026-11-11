#pragma once

// UnkTU2734: functions of this translation unit that other files call (tools/tu_map.py unit 2734,
// 0x710119d484-0x710119ea2c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSweepEntry_b49;
struct UnkSweepList_b49;
struct UnkSweepNode_b49;
struct UnkUnusedArg_b49;
bool fn_710119d5f0(UnkUnusedArg_b49, UnkSweepEntry_b49* entry, bool flag);
UnkSweepNode_b49* fn_710119d7e4(UnkSweepList_b49* self, UnkSweepNode_b49* node);
