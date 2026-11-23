#pragma once

#include "game/UnkShotWeakRefData_b0011.h"
#include "types.h"

struct UnkShotWeakRef_b0011;

// A weak reference (the manager's +0x1210 holds them): alive while the target's generation is this
// one's. Copied as a block.
struct UnkShotWeakRef_b0011 {
    UnkShotWeakRefData_b0011 ref;

    UnkShotWeakRef_b0011(const UnkShotWeakRef_b0011& other) : ref(other.ref) {}
};
