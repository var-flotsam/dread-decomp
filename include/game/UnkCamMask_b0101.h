#pragma once

#include "types.h"

// A bit mask passed by value (copied into a temporary), and its two guarded constants (as in
// src/game/UnkLoadingScreen.cpp).
// A bit mask passed by value (as src/game/CAnimatedCameraCtrl/CAnimatedCameraCtrl_Lifecycle_ovb0101.cpp).
struct UnkCamMask_b0101 {
    u32 bits;
    UnkCamMask_b0101() : bits(0) {}
    UnkCamMask_b0101(u32 b) : bits(b) {}
    UnkCamMask_b0101(const UnkCamMask_b0101& o) : bits(o.bits) {}
};
