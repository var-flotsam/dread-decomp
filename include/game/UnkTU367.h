#pragma once

// UnkTU367: functions of this translation unit that other files call (tools/tu_map.py unit 367,
// 0x71002af630-0x71002b106c). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkDoorLock;
struct UnkNavMeshCell;
struct UnkWeakTarget;
UnkDoorLock* fn_71002af860(UnkWeakTarget*);
u32 fn_71002b009c(UnkNavMeshCell* cell);  // A word of the cell's +0x68 object, or 0.
void fn_71002af630(UnkNavMeshCell* self);  // A cell's constructor.
void fn_71002af6a0(UnkNavMeshCell* self);
