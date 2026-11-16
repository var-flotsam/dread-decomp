#pragma once

// CAIAttackComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2705,
// 0x71011336b0-0x7101135a60). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkCommanderPlayer_b67a2;
// Starts the attack `entry` on the component (`name` overrides its id when set); false when it does not start.
bool fn_7101134420(UnkCommanderPlayer_b67a2* player, void* entry, bool flag, u32 arg3, const base::global::CStrId* name,
                   f32 arg5);
