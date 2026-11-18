#pragma once

// UnkTU1428: functions of this translation unit that other files call (tools/tu_map.py unit 1428,
// 0x710093b200-0x71009506b0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkMorphBall_eb64;
struct UnkEventOwner_m_ovb0137;
struct UnkEnergyWaveCompB_b49;
struct UnkEnergyWaveObject_b49;
void fn_710094f8f0(UnkEnergyWaveCompB_b49* comp, UnkEnergyWaveObject_b49* object);
void* fn_710094c9ac(UnkEventOwner_m_ovb0137* owner);
void fn_710094b508(void* object);
void fn_710093ed64(UnkMorphBall_eb64* self);
void fn_710093efdc(UnkMorphBall_eb64* self, u32 a, u32 b, u32 c);
