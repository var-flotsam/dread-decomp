#pragma once

#include "base/reflection/UnkReflTypedPtr.h"
#include "types.h"

// A reflected field (0xb0 bytes): its type at +0x10.
struct UnkTunableField {
    u8 pad_0x0[0x10];
    const base::reflection::CType* type;
    u8 pad_0x18[0x10];
    void* field_0x28;  // the accessor; without one fn_71000e74bc decides whether it can be set
    u8 pad_0x30[0x80];
};
