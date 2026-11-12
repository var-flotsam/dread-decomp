#pragma once

// UnkTU376: functions of this translation unit that other files call (tools/tu_map.py unit 376,
// 0x71002b57bc-0x71002b679c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkObject_0x100;
void fn_71002b57bc(UnkObject_0x100* self);  // The payload's constructor.

// The payload's default constructor or its assignment (the pair fn_71002b57bc, fn_71002b5c90).
UnkObject_0x100* fn_71002b5c90(UnkObject_0x100* self, const UnkObject_0x100* other);
