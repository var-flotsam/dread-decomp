#pragma once

// The door shield's ids among the statics of CDoorLifeComponent's unit (tools/tu_map.py unit 1376): the two
// flags set or cleared on the actor while the shield opens or closes, and its pair of animations. Used by the
// shield (src/game/CDoorShieldLifeComponent) and the door's update (src/game/UnkTU1376). Names and layouts are
// placeholders.
#include "types.h"

struct UnkDoorShieldFlags_ovb0001 {
    u8 pad_0x0[0x30];
    u64 name;  // 0x30
};

// The shield's open and close animations, side by side.
struct UnkDoorShieldAnimations {
    u64 open;
    u64 close;
};

extern UnkDoorShieldFlags_ovb0001* g_unk_7172731790 __attribute__((visibility("hidden")));
extern UnkDoorShieldFlags_ovb0001* g_unk_7172731798 __attribute__((visibility("hidden")));
extern const UnkDoorShieldAnimations g_unk_71727317a0 __attribute__((visibility("hidden")));
