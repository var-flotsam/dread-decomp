#pragma once

// CScenario: functions of this translation unit that other files call (tools/tu_map.py unit 2735,
// 0x710119ea2c-0x71011a4eb4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkGameManagerSub_0x608;

void fn_71011a2a24(UnkGameManagerSub_0x608* scenario, void* object);  // Takes an object off the scenario's lists.
void fn_71011a2770(UnkGameManagerSub_0x608* points, void* object);
