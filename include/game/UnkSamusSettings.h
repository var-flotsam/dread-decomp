#pragma once

#include "types.h"

// The settings object of src/game/CSamusMovement.cpp (reached through the GOT); +0x79 is read here.
// A settings object (reached through the GOT): the legs' override is off while its +0x78 is clear.
// The Samus settings object of src/game/CSamusMovement.cpp (reached through the GOT).
struct UnkSamusSettings {
    u8 pad_0x0[0x24];
    f32 field_0x24;
    f32 field_0x28;
    u8 pad_0x2c[0x8];
    u32 field_0x34;
    u32 field_0x38;
    u8 pad_0x3c[0x34];
    f32 field_0x70;
    f32 field_0x74;
    bool field_0x78;
    u8 pad_0x79[0x3];
};
