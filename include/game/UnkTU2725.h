#pragma once

// UnkTU2725: functions of this translation unit that other files call (tools/tu_map.py unit 2725,
// 0x7101186780-0x71011868fc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMusicTrigger;
void fn_71011867d4(void* self);  // The music trigger base's destructor.
void fn_7101186780(UnkMusicTrigger* self);
void fn_71011867ec(char* self);
