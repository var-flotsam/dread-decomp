#pragma once

#include "base/global/UnkTwoStepStrId.h"
#include "types.h"

struct UnkTarget;

struct UnkReset {
    u8 pad_0x0[0x18];
    void* field_0x18;
    u32 field_0x20;
    u32 field_0x24;
    u8 pad_0x28[0x10];
    UnkTarget* field_0x38;
    UnkStrIdTwoStep field_0x40;
    u8 pad_0x48[0x18];
    u32 field_0x60;
};
