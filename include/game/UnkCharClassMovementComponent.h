#pragma once

#include "types.h"

struct UnkCharClassMovementComponent {
    void** vtable;
    u8 pad_0x8[0x10];
    u64 field_0x18;
    u8 pad_0x20[0x38];
    bool field_0x58;
    u8 pad_0x59[3];
    u32 field_0x5c;
};
