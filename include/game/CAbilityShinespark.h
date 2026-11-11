#pragma once

// CAbilityShinespark: functions of this translation unit that other files call (tools/tu_map.py unit 433,
// 0x71003255a0-0x7100333544). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDirOut_b56;
struct UnkShinespark_b56;
u64 fn_7100327d14();
void fn_7100331c7c(void* self, const void* other);
u32 fn_710032f678(UnkShinespark_b56* self, const UnkDirOut_b56* dir);  // The state for the direction.
