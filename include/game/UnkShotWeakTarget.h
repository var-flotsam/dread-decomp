#pragma once

#include "types.h"

// A weak reference: alive while the target's generation is this one's.
// A weak reference, as src/game/CShotManager.cpp has it.
struct UnkShotWeakTarget {
    void* field_0x0;
    s64 generation;
};
