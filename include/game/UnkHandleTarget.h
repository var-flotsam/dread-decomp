#pragma once

#include "types.h"

// A handle to a playing rumble (as in src/game/CAbilityEnergyWave/fn_71003053c4.cpp): current while the target's
// serial is the handle's.
// A handle to a playing vibration: current while the target's serial is the handle's.
// A handle to a playing rumble: current while the target's serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};
