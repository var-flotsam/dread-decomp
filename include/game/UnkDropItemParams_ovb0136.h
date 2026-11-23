#pragma once

#include "game/SDropProbabilities.h"
#include "game/UnkDropItemName_ovb0136.h"
#include "game/UnkDropItemVec3_ovb0136.h"
#include "types.h"

struct UnkDropItemParams_ovb0136;

// The drop spawn settings (0x90 bytes; as CDropComponent passes them to its spawn), built by
// fn_71007c7ddc.
struct UnkDropItemParams_ovb0136 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10[7];
    u8 pad_0x2c[8];
    u32 field_0x34[4];
    u8 pad_0x44[0xc];
    u32 field_0x50[3];
    u8 pad_0x5c[4];
    UnkDropItemName_ovb0136 name;      // 0x60
    UnkDropItemVec3_ovb0136 field_0x68;
    UnkDropItemVec3_ovb0136 field_0x74;
    u32 item;                           // 0x80
    u32 kind;                           // 0x84
    u8 field_0x88;
    u8 field_0x89;
    u8 field_0x8a;

    UnkDropItemParams_ovb0136() { fn_71007c7ddc(this); }
};
