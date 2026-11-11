#pragma once

// time: functions of this translation unit that other files call (tools/tu_map.py unit 45,
// 0x710006b6a4-0x710006d2a0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSoundHolder;
void fn_710006cd9c(UnkSoundHolder* holder);  // Constructor.
void fn_710006b6a4(void* self);
void fn_710006cea8(UnkSoundHolder* holder, bool value);
