#pragma once

#include "game/CCameraFX_Shake.h"
#include "game/UnkRandShakeCurve_ovb0131.h"
#include "types.h"

struct UnkRandShake_ovb0131 {
    u8 pad_0x0[0x34];
    f32 duration;  // 0x34
    u8 pad_0x38[0x18];
    f32 time;  // 0x50
    bool stopped;  // 0x54
    u8 pad_0x55[0x3];
    UnkRandShakeCurve_ovb0131* curve;  // 0x58
    f32 amplitude;  // 0x60
    f32 amplitude_w;  // 0x64
    base::math::CVector3D offset;  // 0x68
    f32 offset_w;  // 0x74
};
