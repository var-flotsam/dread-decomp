#pragma once

#include "types.h"

// The effect restart of src/game/CEmmyGrabSamusAttack/CEmmyGrabSamusAttack_Tick_b54.cpp in red: the level
// (+0x6d4) drops by 0.8 per second, not below zero.
// The colour types of fn_710050f26c (src/game/CEmmyGrabSamusAttack/fn_710050f26c.cpp), named the same so
// the call resolves to that definition.
// The colour types of fn_710050f26c, as CEmmyGrabSamusAttack_Tick_b54.cpp names them.
struct UnkRGB_710050f26c {
    f32 r;
    f32 g;
    f32 b;
};
