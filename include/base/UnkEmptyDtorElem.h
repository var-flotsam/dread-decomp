#pragma once

// fn_71002874dc is an empty function that several element types call as their out-of-line
// destructor (a 4-byte key or element: src/game_lo/fn_71002b6094.cpp, src/game_s2/fn_71002b26fc.cpp,
// src/game_s2/fn_710035f70c.cpp). It reads no argument, so it takes any object.

#include "types.h"
void fn_71002874dc(void* self);
