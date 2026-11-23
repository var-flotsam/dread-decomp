#pragma once

#include "types.h"

// A float with a flag saying it is set (the float's bits in the low word, the flag in the high).
struct UnkTrackOptional {
    u64 bits;
};
