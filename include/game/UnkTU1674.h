#pragma once

// UnkTU1674: functions of this translation unit that other files call (tools/tu_map.py unit 1674,
// 0x7100b1df48-0x7100b1e70c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGuiEventData_ovb0182;
void DispatchEventToHandlersForKey(void* slots, UnkGuiEventData_ovb0182* event);
