#pragma once

#include "types.h"

// A set of flags (a u32) that starts empty; the empty set is a shared constant.
struct UnkEnergyWaveFlags_ovb22 {
    u32 bits;
    UnkEnergyWaveFlags_ovb22() : bits(0) {}
    UnkEnergyWaveFlags_ovb22(const UnkEnergyWaveFlags_ovb22& o) : bits(o.bits) {}
};
