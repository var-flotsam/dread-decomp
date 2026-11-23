#pragma once

#include "game/UnkTU108.h"
#include "types.h"

// Six values, an id and a half-word.
struct UnkValuesIdHalf {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    base::global::CStrId id;
    u16 field_0x20;

    UnkValuesIdHalf& operator=(const UnkValuesIdHalf& other) {
        field_0x0 = other.field_0x0;
        field_0x4 = other.field_0x4;
        field_0x8 = other.field_0x8;
        field_0xc = other.field_0xc;
        field_0x10 = other.field_0x10;
        field_0x14 = other.field_0x14;
        id = other.id;
        field_0x20 = other.field_0x20;
        return *this;
    }
};
