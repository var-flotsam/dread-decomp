#pragma once

// UnkTU958: functions and globals of this translation unit that other files use (tools/tu_map.py unit 958,
// 0x7100639cf8-0x710063ce2c). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkActor;
struct UnkOffsetScratch_b65a4;
struct UnkOffsetVec_b65a4;
void fn_710063c670(void* part);
bool fn_710063b1b0(u8* registry, void* owner, u32 flags, UnkOffsetVec_b65a4* pos, UnkOffsetScratch_b65a4* scratch);
extern void* g_unk_7101a58cd8[];
void fn_710063a748(u8* part, UnkActor* actor);
