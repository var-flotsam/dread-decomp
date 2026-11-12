#pragma once

// CBehavior: functions and globals of this translation unit that other files use (tools/tu_map.py unit 500,
// 0x7100394820-0x7100395f24). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

struct UnkInlineString_A2_b72;
struct UnkStringEqualsCondition_A2_b72;
struct UnkSelf;
struct UnkStruct_0x68;
void fn_7100394bd0(UnkStruct_0x68* self);
bool fn_7100394e2c(UnkSelf*, void*, f32*);  // Returns whether the property is set.
void fn_71003954fc(UnkSelf* behavior, void* record, f32 value);
extern base::global::CStrId g_unk_71726b7cb8;
bool fn_7100395044(UnkStringEqualsCondition_A2_b72* self, base::global::CStrId* id, UnkInlineString_A2_b72* out);
