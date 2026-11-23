#pragma once

#include "game/UnkMeleeOwner.h"
#include "types.h"

struct UnkMeleeComponent {
    u8 pad_0x0[0x30];
    UnkMeleeOwner* owner;
};
