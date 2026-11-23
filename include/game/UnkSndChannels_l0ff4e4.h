#pragma once

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "game/UnkSndChannelFade_l0ff4e4.h"
#include "game/UnkTU99.h"

struct UnkSndChannels_l0ff4e4 {
    u8 pad_0x0[0xf0];
    u32 mask;                            // 0xf0
    UnkSndChannelFade_l0ff4e4 fades[8];  // 0xf4
    u8 pad_0x194[0x3f0 - 0x194];
    base::global::CRntVector<UnkInterp_7100105f94*> voices_a;  // 0x3f0
    base::global::CRntVector<UnkInterp_7100105f94*> voices_b;  // 0x418
    u8 pad_0x440[0x400568 - 0x440];
    nn::os::MutexType mutex;  // 0x400568
};
