#pragma once

#include "base/UnkHandleCallbackSet.h"

struct UnkValuesCallbacks {
    u64 unk_0x0;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    UnkHandleCallbackSet set;

    __attribute__((always_inline)) UnkValuesCallbacks& operator=(const UnkValuesCallbacks& other) {
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        set = other.set;
        return *this;
    }
};
