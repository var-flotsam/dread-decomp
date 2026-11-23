#pragma once

#include "base/color/CColor4F.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkPair_de1c40.h"
#include "game/UnkWords2_de1c40.h"
#include "game/UnkWords4_de1c40.h"
#include "types.h"

struct UnkExplosionGfx_de1c40 {
    void* vtable;  // 0x0
    u64 field_0x8;  // 0x8
    u64 field_0x10;  // 0x10
    base::color::CColor4F color_0x18;  // 0x18
    base::color::CColor4F color_0x28;  // 0x28
    base::color::CColor4F color_0x38;  // 0x38
    f32 field_0x48;  // 0x48
    base::math::UnkVector4D oConstantColor0TransitionInit;  // 0x4c
    base::math::UnkVector4D oConstantColor0TransitionEnd;  // 0x5c
    base::math::UnkVector4D oConstantColor1TransitionInit;  // 0x6c
    base::math::UnkVector4D oConstantColor1TransitionEnd;  // 0x7c
    f32 fConstantColor0TransitionTime;  // 0x8c
    f32 fConstantColor1TransitionTime;  // 0x90
    s32 eConstant0TransitionMode;  // 0x94
    s32 eConstant1TransitionMode;  // 0x98
    base::global::CFilePathStrId sParticlesPath;  // 0xa0
    base::global::CFilePathStrId sAdditionalParticlesPath;  // 0xa8
    base::global::CFilePathStrId sMeshPath;  // 0xb0
    f32 fMainExplosionScale;  // 0xb8
    UnkPair_de1c40 vRandomScale;  // 0xbc
    f32 fRadius;  // 0xc4
    f32 fRadiusAmplitude;  // 0xc8
    f32 fRandomArcAmplitude;  // 0xcc
    u32 uAmount;  // 0xd0
    f32 fLength;  // 0xd4
    UnkPair_de1c40 vRandomDelay;  // 0xd8
    f32 values_0xe0[81];  // 0xe0
    UnkWords2_de1c40 words_0x228;  // 0x228
    UnkWords4_de1c40 words_0x238;  // 0x238
    UnkWords4_de1c40 words_0x258;  // 0x258
};
