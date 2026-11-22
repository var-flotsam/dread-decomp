#pragma once

#include "game/CBreakableTileGroup.h"
#include "types.h"

struct UnkTileGroup;

// One tile of a group. `state` is 0 (intact), 1 (hit) or 2 (broken).
struct UnkQueryObj {
    u32 field_0x0;  // the tile's index in its group
    u32 field_0x4;  // its type (the enum registered next to fn_7100ca0118's clone: 7 is "WEIGHT")
    u32 field_0x8;
    u32 field_0xc;
    f32 field_0x10;  // the respawn time (fn_7100ca6af8 of the type), unless field_0x3c is set
    u32 pad_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    base::global::CStrId field_0x20;  // a render layer, switched with the state when it is not empty
    f32 field_0x28;                   // x
    f32 field_0x2c;                   // y
    UnkTileGroup* field_0x30;
    s32 state;  // 0x38
    bool field_0x3c;
    u8 pad_0x3d[0xb];
    u64 field_0x48;  // a render object, 0xffffffff when there is none
    u8 pad_0x50[0x58];
};
