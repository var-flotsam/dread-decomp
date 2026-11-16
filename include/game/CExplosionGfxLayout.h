#pragma once

// CExplosionGfx (tools/tu_map.py unit 2552), as far as its hand-written sources know it. The field names come
// from its reflected members (src/game/CExplosionGfx/fn_7100ddf434.cpp); the layout is a placeholder.
#include "types.h"

struct CExplosionGfx {
    u8 pad_0x0[0xb8];
    f32 fMainExplosionScale;  // +0xb8
    f32 vRandomScale[2];      // +0xbc
    f32 fRadius;              // +0xc4
    f32 fRadiusAmplitude;     // +0xc8
    f32 fRandomArcAmplitude;  // +0xcc
    u32 uAmount;              // +0xd0
    f32 fLength;              // +0xd4
    f32 vRandomDelay[2];      // +0xd8
};
