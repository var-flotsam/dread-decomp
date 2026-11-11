#pragma once

// UnkTU230: functions of this translation unit that other files call (tools/tu_map.py unit 230,
// 0x71001a9f3c-0x71001aa20c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkGfxStrId;
bool fn_71001aa0b4(void* program, u32 stage, u32 id, u64* location);
void* fn_71001aa028(void* program);
void fn_71001aa0cc(void* program, u32 stage, const UnkGfxStrId* name, u64* location);
