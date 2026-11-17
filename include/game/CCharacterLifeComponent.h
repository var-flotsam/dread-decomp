#pragma once

// CCharacterLifeComponent: functions of this translation unit that other files call (tools/tu_map.py unit 2713,
// 0x7101158410-0x71011593e0). One declaration per function: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

struct UnkEnemyLifeDtor_ovb0043;
void fn_7101158c4c(UnkEnemyLifeDtor_ovb0043* self);  // Derived classes call it as their base's destructor.

// Adds `fraction` of the maximum life (fn_71011603a4) and tells the owner; with `flag`, a change is also
// sent to the owner's animation. Whether the life changed.
struct UnkCharacterLifeComponent;
bool fn_7101158d78(UnkCharacterLifeComponent* self, f32 fraction, bool flag);
