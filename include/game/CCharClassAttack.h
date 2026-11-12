#pragma once

// CCharClassAttack: functions of this translation unit that other files call (tools/tu_map.py unit 445,
// 0x7100362e00-0x7100364820). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkAttackTarget_A2_b68;
struct UnkStruct_71003641fc;
void fn_71003641fc(UnkStruct_71003641fc* self);
void* fn_71003646b0(UnkAttackTarget_A2_b68* target, u32 index);  // The target's entry at `index`, or null when it has none.
