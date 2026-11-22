#pragma once

#include "types.h"

struct UnkRecord58;

struct UnkRecordArray {
    u8 unk_0x0[0x8];
    UnkRecord58* data;
    u32 count;
};
