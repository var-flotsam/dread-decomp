#pragma once

// UnkTU1124: functions of this translation unit that other files call (tools/tu_map.py unit 1124,
// 0x710070368c-0x7100703cf4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkUnit1124Object;
void fn_7100703814(UnkUnit1124Object* self, u32 id);
u64 fn_71007038f4(void* self, void* params, bool reset);  // Its first argument is the handler, its second the block.
