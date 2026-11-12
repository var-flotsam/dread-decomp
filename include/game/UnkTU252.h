#pragma once

// UnkTU252: functions of this translation unit that other files call (tools/tu_map.py unit 252,
// 0x71001bdeac-0x71001c071c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSceneObjectSub;
struct UnkColor_ovb0092;
struct UnkLockonModel_ovb0092;
struct UnkSlot252;
struct UnkSlotArray252;
struct UnkSource_710020c780;
void fn_71001bfea4(UnkLockonModel_ovb0092* model, u32 index, const UnkColor_ovb0092* color);
void fn_71001bff28(UnkLockonModel_ovb0092* model, u32 index, UnkColor_ovb0092* color);
void fn_71001bffc4(UnkSource_710020c780* source, f32 dt);
UnkSlot252* fn_71001c04bc(UnkSlotArray252* array, u32 index);
void fn_71001beee4(UnkSceneObjectSub* target, bool);
void* fn_71001be020(UnkSceneObjectSub* target, const u64* key);
