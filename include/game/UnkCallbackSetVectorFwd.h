#pragma once

// The callback-set array of game/UnkCallbackSetVector.h as the files that only pass it on see it,
// and the array's out-of-line Reserve (fn_71011143d8, defined in src/game_s7/fn_71011143d8.cpp).

#include "types.h"

struct UnkCallbackSetVector;

void fn_71011143d8(UnkCallbackSetVector* v, u32 n);
