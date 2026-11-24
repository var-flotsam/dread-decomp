#pragma once

// One object that 5 files viewed in part (fn_7100dec788, declared in game/UnkTU2553.h, returns it to all five; views of 456 to 632 bytes whose shared offsets agree). Names and layouts are placeholders.
#include "types.h"

struct UnkAmmo;
struct UnkPlayerSub;
struct UnkHelper;
struct UnkInventory;

struct UnkPlayer {
    u8 pad_0x0[0x1b8];
    UnkAmmo* ammo;
    UnkPlayerSub* field_0x1c0;
    u8 pad_0x1c8[0x28];
    void* field_0x1f0;
    u8 pad_0x1f8[0x10];
    UnkHelper* field_0x208;
    u8 pad_0x210[0x10];
    void* field_0x220;
    u8 pad_0x228[0x38];
    void* field_0x260;
    u8 pad_0x268[0x8];
    UnkInventory* inventory;
};
