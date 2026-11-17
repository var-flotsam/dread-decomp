#pragma once

// CDrop: functions of this translation unit that other files call (tools/tu_map.py unit 1222,
// 0x71007bf170-0x71007c0710). One declaration per function: include this instead of declaring one.
// Names are placeholders; the drop's layout and its table's constructor (fn_71007bf550) are in
// game/CDropLayout.h.
#include "types.h"
struct CDrop;
// The drop's constructor and destructor bodies (src/game/CDrop/CDrop_Members_ovb0105.cpp).
void fn_71007bf3f0(CDrop* self);
void fn_71007bfa70(CDrop* self);
void* fn_71007c04f0(void* object);
