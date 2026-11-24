#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkRec {
    u8 pad_0x0[8];
    base::global::CStrId field_0x8;
    u32 field_0x10;
    base::global::CStrId sFunction;
    u64 field_0x20;
};
