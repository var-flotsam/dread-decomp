#pragma once

#include "types.h"

struct UnkFootstepMovement;

struct UnkFootstepPlayer {
    u8 pad_0x0[0x208];
    UnkFootstepMovement* movement;
};
