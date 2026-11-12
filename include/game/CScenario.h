#pragma once

// CScenario: functions of this translation unit that other files call (tools/tu_map.py unit 2735,
// 0x710119ea2c-0x71011a4eb4). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkGameManager.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkGameManagerSub_0x608;

void fn_71011a2a24(UnkGameManagerSub_0x608* scenario, void* object);  // Takes an object off the scenario's lists.
void fn_71011a2770(UnkGameManagerSub_0x608* points, void* object);
// Reads the string id under the property `name` names (refl_GetKeyedValue for CStrId).
void fn_71011a2fd0(UnkGameManagerSub_0x608* self, const base::global::CStrId& id, bool arg2);
base::global::CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
