#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkQueryParam {
    void** vtable;
    base::global::UnkStrId field_0x8;
    u8 pad_0x10[0x4];
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
};
