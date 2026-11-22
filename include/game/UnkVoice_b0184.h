#pragma once

#include "types.h"

// A voice: the group bit it answers to is 1 << shift.
struct UnkVoice_b0184 {
    u8 pad_0x0[0xb0];
    u32 shift;  // 0xb0
};
