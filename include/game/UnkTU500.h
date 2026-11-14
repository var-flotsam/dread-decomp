#pragma once

// UnkTU500: functions of this translation unit that other files call (tools/tu_map.py unit 500,
// 0x7100394820-0x7100395f24, behaviortree::CBehavior's). One declaration per function: include this instead
// of declaring one. Names and layouts are placeholders.
#include "game/CBlackboard.h"
#include "types.h"

bool fn_7100395b34(CBlackboard::CSection* section, const void* name, f32* out);
