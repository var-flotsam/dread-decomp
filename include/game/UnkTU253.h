#pragma once

// UnkTU253: functions of this translation unit that other files call (tools/tu_map.py unit 253,
// 0x71001c071c-0x71001c33cc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkModelData;
struct UnkResetOwner;
struct UnkSource_710020c780;
void fn_71001c0968(UnkModelData* data, u32 flag);
void fn_71001c3040(void* object, u64 hash, u32 a, const f32* values, u32 count, f32 f);
void fn_71001c3270(UnkResetOwner* self);
bool fn_71001c32bc(UnkSource_710020c780* source, const u64* key, u32 flag, f32* out);
