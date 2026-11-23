#pragma once

#include "types.h"

// The allocator the pools' storage comes from, and the five heaps (as
// src/game_s1/fn_7100153ac0.cpp declares them).
struct UnkPtrHolder {
    void** vtable;
    void* ptr;
};
