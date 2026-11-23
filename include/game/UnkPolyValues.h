#pragma once

#include "types.h"

struct UnkPolyValues {
    char* vtable;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;

    void Destroy() { reinterpret_cast<void (**)(UnkPolyValues*)>(vtable)[1](this); }
};
