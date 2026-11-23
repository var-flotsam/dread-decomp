#pragma once

#include "base/global/CRntFile.h"
#include "types.h"

struct UnkWriter_b0202 {
    u32 count;
    u8 entries_0x4[0xc80];
    u8 pad_0xc84[4];
    base::global::CRntFile* file;  // 0xc88
};
