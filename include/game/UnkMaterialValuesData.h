#pragma once

#include "types.h"

// The element's plain part (copied as a whole).
struct UnkMaterialValuesData {
    u8 field_0x10;
    s32 field_0x14;  // the number of values
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    void* field_0x30;  // the values
};
