#pragma once

#include "types.h"

// Kind 1: the word is the object itself; otherwise it is the id the scene finds it by.
// An item of a group (src/game/UnkSceneGroupItem.cpp passes the flag on).
struct UnkSceneGroupItem {
    u64 field_0x0;
    u8 pad_0x8[0x8];
    u32 field_0x10;
};
