#pragma once

// UnkTU2647: functions of this translation unit that other files call (tools/tu_map.py unit 2647,
// 0x710102cba4-0x710103d05c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSoundDef;
struct UnkSoundDesc;
struct UnkPathIdListPair_710103a240;
void fn_7101036e34(void* value);  // The value's destructor.
void fn_710103a240(UnkPathIdListPair_710103a240* self, const UnkPathIdListPair_710103a240* other);
void fn_7101037bc0(UnkSoundDef* self, UnkSoundDesc* out, f32* scales, u32 index);
void fn_71010372dc(void* items, const void* other_items);
extern char g_unk_7101cac748[];
