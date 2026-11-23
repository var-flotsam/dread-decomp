#pragma once

#include "game/UnkTUInit.h"

// The unit's data (its statics, merged into one block).
struct UnkRotationalPlatformTU {
    UnkTUHeader header;
    u64 field_0x18;
    CStrId player_detector_right;  // 0x20
    CStrId player_detector_left;   // 0x28
    CStrId collision_opened;       // 0x30
    CStrId collision_closed;       // 0x38
    CStrId initial;                // 0x40
    CStrId finished;               // 0x48
    UnkReflType type;              // 0x50
};
