#pragma once

// CMagnetSlidingBlockComponent: functions and globals of this translation unit that other files use (tools/tu_map.py unit 1412,
// 0x710091d7d0-0x7100921efc). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "game/UnkTUProperty.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkMagnetSlidingBlockRail_ovb0097;
struct UnkMagnetSlidingBlock_b0086;
struct CMagnetSlidingBlockComponent;
void CMagnetSlidingBlockComponent_SendMaterialColorChange(CMagnetSlidingBlockComponent* self, f32 speed);
extern void* g_unk_7101a82960[];
extern void* g_unk_7101a82c58[];
extern UnkProperty g_unk_7172737530;
UnkMagnetSlidingBlockRail_ovb0097* fn_710091f6bc(void* object);
void CMagnetSlidingBlockComponent_SetCollidersAndSpeed(UnkMagnetSlidingBlock_b0086* self, bool on);
extern base::global::CStrId g_unk_71727375d8 __attribute__((visibility("hidden")));
