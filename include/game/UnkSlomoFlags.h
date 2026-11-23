#pragma once

#include "types.h"

// The slow-motion flags as bytes of the game manager's word at +0xc4.
// The bytes of the game manager's slow motion state (+0xc4).
struct UnkSlomoFlags {
    bool enabled;  // 0xc4
    u8 unk_0xc5;
    bool paused;  // 0xc6
};
