#pragma once

// CChozoWarriorGlaiveSpinAttack: functions of its unit that other files call. Names are placeholders.
#include "types.h"

struct UnkGlaiveSpinAttack;  // the attack; its owner actor at +0x8
// Whether the spin attack may start (src/game/CChozoWarriorGlaiveSpinAttack/fn_7100494c9c.cpp).
bool fn_7100494c9c(UnkGlaiveSpinAttack* self, void* arg, u32 mode);
