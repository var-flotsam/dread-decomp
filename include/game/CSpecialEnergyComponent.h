#pragma once

// CSpecialEnergyComponent: functions of this translation unit that other files call (tools/tu_map.py unit 1535,
// 0x7100a1db40-0x7100a208b4). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkSpecialEnergyComponent;
struct UnkTarget429;
struct UnkLifeComponent;
void fn_7100a1fa98(UnkLifeComponent* life, bool a, bool b, f32 value);
f32 fn_7100a1ff18(UnkLifeComponent* life);
bool fn_7100a1f84c(UnkSpecialEnergyComponent* self);
void fn_7100a1feac(UnkTarget429* self, f32 amount);
namespace base {
namespace reflection {
struct UnkTunable;
}  // namespace reflection
}  // namespace base
extern base::reflection::UnkTunable g_unk_7172745770;  // CTunableSpecialEnergyComponent
