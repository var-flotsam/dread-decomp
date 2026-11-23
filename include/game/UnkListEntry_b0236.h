#pragma once

#include "base/reflection/UnkReflTypedPtr.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

struct UnkListEntry_b0236 {
    base::reflection::UnkReflValue value;       // 0x0
    base::reflection::UnkReflTypedPtr typed;    // 0x18
    u32 kind;                 // 0x28
    u32 pad_0x2c;
    base::reflection::UnkReflValue field_0x30;
    base::reflection::UnkReflValue field_0x48;
    u64 field_0x60;
};
