#pragma once

// CCharClassTweakPlayRateTrack: functions and globals of this translation unit that other files use (tools/tu_map.py unit 2163,
// 0x7100c5eb80-0x7100c61e68). One declaration per symbol: include this instead of declaring one.
// Names and layouts are placeholders.
#include "types.h"

// The tunable g_unk_7172790a08, as far as its readers and writers go.
struct UnkGlobal2 {
    u8 pad_0x0[0x40];
    u32 field_0x40;
    u8 pad_0x44[0x38];
    f32 field_0x7c;
};
extern UnkGlobal2 g_unk_7172790a08;
