#pragma once

#include "game/UnkSwarmArea_b66a4.h"

struct UnkSwarmHold_b66a4 {
    void** vtable;
    u8 pad_0x8[0x390 - 0x8];
    u32 mode;  // 0x390
    u8 pad_0x394[0x56c - 0x394];
    f32 time;  // 0x56c
    UnkSwarmArea_b66a4* Area(u32 slot) {
        return reinterpret_cast<UnkSwarmArea_b66a4* (*)(UnkSwarmHold_b66a4*)>(vtable[slot / 8])(this);
    }
};
