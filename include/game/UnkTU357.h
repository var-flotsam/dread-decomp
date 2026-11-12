#pragma once

// UnkTU357: functions of this translation unit that other files call (tools/tu_map.py unit 357,
// 0x710028659c-0x71002871cc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkWeakTarget;
struct UnkActor_7100286798;
struct UnkNavQuery;
struct UnkStruct_7100286624;
struct UnkStruct_7100286798;
void fn_710028659c(UnkNavQuery* self);
void fn_7100286624(UnkStruct_7100286624* self);
void fn_7100286798(UnkStruct_7100286798* self, UnkActor_7100286798* actor);
UnkWeakTarget* fn_7100286ad4(void* owner);
