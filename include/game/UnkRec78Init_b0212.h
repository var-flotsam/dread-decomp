#pragma once

#include "game/GUI/UnkMissionLogRecords.h"

struct UnkRec78Init_b0212 {
    u64 field_0x0;
    base::global::CRntVector<u8> items_0x8{0xf};
    u64 field_0x30 = 0;
    f32 scale_x = 1.0f;
    f32 scale_y = 1.0f;
    u64 field_0x40 = 0;
    u32 field_0x48;
    u8 field_0x4c = 0;
    base::global::CRntVector<u8> items_0x50{0xf};
};
