#pragma once

#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkWorld;

struct UnkMeleeOwner {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
    u8 pad_0x228[0x58];
    UnkWorld* world;  // 0x280
};
