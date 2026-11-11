#pragma once

// UnkTU358: functions of this translation unit that other files call (tools/tu_map.py unit 358,
// 0x71002871cc-0x710028739c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkNavPathNode;
void fn_71002872f4(UnkNavPathNode* self);  // The node's destructor (destroys the payload).
