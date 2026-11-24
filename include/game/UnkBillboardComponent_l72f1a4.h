#pragma once

#include "base/global/CFilePathStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

struct UnkItemBase_71011a91c4;

struct UnkBillboardComponent_l72f1a4 {
    void* vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    u32 iPoolSize;
    f32 field_0x5c;
    f32 field_0x60;
    u8 pad_0x64[0x4];
    base::global::CFilePathStrId sPath;
    base::global::CFilePathStrId sBgPath;
    u32 field_0x78;
    u32 field_0x7c;
    f32 field_0x80;
    f32 field_0x84;
    f32 field_0x88;
    u32 field_0x8c;
    u32 field_0x90;
    u32 field_0x94;
    u32 field_0x98;
    f32 field_0x9c;
    u32 field_0xa0;
    u8 pad_0xa4[0x4];
    base::global::CStrId sBillboardDefaultLoop;
    base::global::CStrId sBillboardMotionDefaultLoop;
    f32 field_0xb8;
    u8 pad_0xbc[0x4];
    base::global::CStrId sGroupMotionDefaultLoop;
    f32 field_0xc8;
    bool bUseRotation;
    u8 pad_0xcd[0x3];
    base::global::CStrId field_0xd0;
    base::global::CStrId field_0xd8;
    base::global::CRntVector<UnkItemBase_71011a91c4*> field_0xe0;
    base::global::CRntVector<UnkItemBase_71011a91c4*> field_0x108;
    base::global::CStrId sSoundLoopLot;
    f32 fSoundLoopLotAmountPct;
    u8 pad_0x13c[0x4];
    base::global::CFilePathStrId sSoundLoopLit;
    f32 fSoundLoopAttMinDist;
    f32 fSoundLoopAttMaxDist;
    base::global::CStrId sSoundImpactBase;
    u32 iSoundImpactCount;
    f32 fSoundImpactRadiusMinAtt;
    f32 fSoundImpactRadiusMaxAtt;
};
