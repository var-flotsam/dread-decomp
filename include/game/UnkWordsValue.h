#pragma once

#include "types.h"

struct UnkWordsValue {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;

    UnkWordsValue(const UnkWordsValue& other) {
        field_0x0 = other.field_0x0;
        field_0x8 = other.field_0x8;
        field_0x10 = other.field_0x10;
    }
};
