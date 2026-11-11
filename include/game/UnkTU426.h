#pragma once

// UnkTU426: functions of this translation unit that other files call (tools/tu_map.py unit 426,
// 0x71002f650c-0x71002f8194). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkNode;
struct UnkPathIds;
struct UnkResourceHandle;
struct UnkResourceManager;
void fn_71002f6ac0(void* member);
UnkResourceHandle fn_71002f7048(UnkResourceManager* manager, const UnkPathIds& ids);
void fn_71002f75ac(void* self, void* arg, u32 flags);
UnkNode** fn_71002f7a2c(void* buckets, u32 index);
