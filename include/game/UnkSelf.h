#pragma once

#include "types.h"
#include "base/math/CVector.h"

struct UnkOwner;
struct UnkFlagObject;
struct UnkTarget;
struct UnkActor_71003b4718;
struct UnkNavMesh;
struct UnkWeakHandle;
struct UnkFacing;

struct UnkSelf {
    u8 pad_0x0[0x18];
    UnkFacing* field_0x18;
    u8 pad_0x20[0x8];
    UnkActor_71003b4718* field_0x28;
    UnkOwner* field_0x30;
    u8 field_0x38[8];
    base::math::CVector3D field_0x40;
    u8 pad_0x50[0x8];
    UnkNavMesh* field_0x58;
    u8 field_0x60[0x18];
    UnkWeakHandle* field_0x78;
    u8 pad_0x80[0x50];
    f32 field_0xd0;
    u8 pad_0xd4[0x14];
    u64 field_0xe8;
    u64 field_0xf0;
    u8 pad_0xf8[0x5a4];
    u32 field_0x69c;
    u8 pad_0x6a0[0x1];
    u8 field_0x6a1;
    u8 pad_0x6a2[0x13e76];
    s32 field_0x14518;
    u8 pad_0x1451c[0x3ec];
    UnkFlagObject* field_0x14908;
    u8 pad_0x14910[0xa8];
    UnkTarget* field_0x149b8;
    u8 pad_0x149c0[0x1e4];
    f32 field_0x14ba4;
};
