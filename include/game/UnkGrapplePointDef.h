#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkGrapplePointDef {
    void** vtable;
    u8 pad_0x8[0x14];
    u32 field_0x1c;
    u8 pad_0x20[0x38];
    bool field_0x58;
    u8 pad_0x59[0x7];
    base::global::UnkStrId field_0x60;
    base::global::UnkStrId field_0x68;
};
