#pragma once

#include "game/UnkAcidBlobDef_ovb0135.h"
#include "game/UnkAcidBlobOwner_ovb0135.h"
#include "game/UnkAcidBlobTarget_ovb0135.h"

struct UnkAcidBlob_ovb0135 {
    u8 pad_0x0[0x30];
    UnkAcidBlobOwner_ovb0135* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x118];
    UnkAcidBlobTarget_ovb0135* target;  // 0x230
    u8 pad_0x238[0x280];
    f32 vel_x;  // 0x4b8
    f32 vel_y;  // 0x4bc
    f32 vel_z;  // 0x4c0
    u8 pad_0x4c4[0xad];
    bool landed;  // 0x571
    u8 pad_0x572[0x5e];
    UnkAcidBlobDef_ovb0135* def;  // 0x5d0
    u8 pad_0x5d8[0x10];
    s32 size_step;  // 0x5e8
};
