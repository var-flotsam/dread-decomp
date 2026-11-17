#pragma once

// UnkTU2604: functions of this translation unit that other files call (tools/tu_map.py unit 2604,
// 0x7100f8af0c-0x7100f92a44). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkStrIdToIdMap.h"
#include "types.h"
void fn_7100f8da4c(void*);
// Erases the node `key` names from the map; returns the node before it in the list (null when none).
UnkStrIdToIdNode* fn_7100f8ede4(UnkStrIdToIdMap* self, const base::global::CStrId* key);
