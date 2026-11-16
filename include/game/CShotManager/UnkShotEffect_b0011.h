#pragma once

// An explosion effect slot of CShotManager::CTunableShotManager (0xfc bytes; the manager keeps a
// base::global::CRntVector of them at +0x30). Its copy takes the first six words one by one and the rest as a
// block. Names and layout are placeholders.
#include "types.h"

struct UnkShotEffectState_b0011 {
    bool live;  // 0x18
    u8 pad_0x19[0x7];
    f32 field_0x20;
    f32 field_0x24;
    u32 type;     // 0x28
    u32 variant;  // 0x2c
    u8 pad_0x30[0xcc];
};

struct UnkShotEffect_b0011 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    UnkShotEffectState_b0011 state;  // 0x18

    UnkShotEffect_b0011(const UnkShotEffect_b0011& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          state(other.state) {}
};
static_assert(sizeof(UnkShotEffect_b0011) == 0xfc, "UnkShotEffect_b0011 layout");

// Starts (a = 1) or stops (a = 0) the effect.
void fn_7100ddec50(UnkShotEffect_b0011* self, u32 a, u32 b);
