#pragma once

// UnkTU883: functions of this translation unit that other files call (tools/tu_map.py unit 883,
// 0x71005ba094-0x71005c49d0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkCallback;
struct UnkZiplineRailPart_b49;
void fn_71005c41bc(void*, UnkCallback*);  // Registers.
UnkZiplineRailPart_b49* fn_71005c3084(void* object);
void fn_71005c4828(void*, void**);  // Unregisters.
