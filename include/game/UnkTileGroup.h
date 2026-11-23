#pragma once

#include "types.h"

struct UnkQueryObj;
struct UnkTileCollider;

// The group: its origin (+0x18), its size in cells (+0x24, +0x28), its tiles (+0x40, +0x48), the
// tile index of each cell (+0x58), a render parameter (+0xc8) and its collider (+0xd0).
struct UnkTileGroup {
    u8 pad_0x0[0x10];
    u32 field_0x10;  // the group's id
    bool field_0x14;
    u8 pad_0x15[0x3];
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    s32 field_0x24;
    s32 field_0x28;
    u8 pad_0x2c[0x14];
    u32 field_0x40;
    u8 pad_0x44[0x4];
    UnkQueryObj* field_0x48;
    u8 pad_0x50[0x8];
    u32* field_0x58;
    u8 pad_0x60[0x68];
    u32 field_0xc8;
    u8 pad_0xcc[0x4];
    UnkTileCollider* field_0xd0;
};
