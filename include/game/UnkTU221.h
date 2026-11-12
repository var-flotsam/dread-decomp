#pragma once

// UnkTU221: functions of this translation unit that other files call (tools/tu_map.py unit 221,
// 0x710019a4e8-0x710019b5ec). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMatrix44;
struct UnkBillboardDrawable;
struct UnkStruct_710019ab0c;
void fn_710019ab0c(UnkStruct_710019ab0c* self);
void fn_710019a4e8(UnkBillboardDrawable* self, u32 a, u32 b);  // The drawable's constructor.
void fn_710019b184(UnkBillboardDrawable* self, u32 index, u32 part, const UnkMatrix44* matrix);
