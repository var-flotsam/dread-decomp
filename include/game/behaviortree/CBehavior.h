#pragma once

// CBehavior: functions of this translation unit that other files call (tools/tu_map.py unit 500,
// 0x7100394820-0x7100395f24). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
bool fn_7100394e2c(UnkSelf*, void*, f32*);  // Returns whether the property is set.
void fn_71003954fc(UnkSelf* behavior, void* record, f32 value);
