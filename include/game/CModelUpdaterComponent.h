#pragma once

// CModelUpdaterComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2719,
// 0x710116ed2c-0x7101174bd0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEnergyWaveFxComp_b49;
struct UnkFlagTarget;
struct UnkGrappleAnswer_b50;
void fn_7101172764(UnkFlagTarget* target, const char* name);
void fn_7101172780(UnkFlagTarget* target, const char* name);
void fn_71011738f8(UnkEnergyWaveFxComp_b49* comp, void* id, u32 flags);
void fn_710117364c(void* target, const u64* id, UnkGrappleAnswer_b50* out);
bool fn_7101173964(void* self);
