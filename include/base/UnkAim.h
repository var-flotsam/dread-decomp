#pragma once

#include "types.h"

// Moves the angle (radians) by a full turn either way when that brings it closer to `target`
// (fn_710007a170). The angle is the first member of a pair (src/game/CCentralUnitCannonAIComponent/
// fn_71003fd0b0.cpp names the pair UnkAim).
struct UnkAim {
    f32 angle;
    f32 current;
};
