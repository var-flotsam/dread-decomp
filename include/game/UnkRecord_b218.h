#pragma once

#include "game/UnkArray_b218.h"
#include "game/UnkPartArray_b218.h"
#include "types.h"

struct UnkRecord_b218 {
    base::global::CStrId id;
    u8 field_0x8;
    u8 pad_0x9[7];
    UnkPartArray_b218 field_0x10;
    UnkArray_b218<u32> field_0x38;
    UnkArray_b218<u8> field_0x60;
    UnkArray_b218<u32> field_0x88;

    UnkRecord_b218()
        : id(g_unk_710162e2ed, true), field_0x8(0), field_0x10(0xf), field_0x38(0xf), field_0x60(0xf), field_0x88(0xf) {}
    __attribute__((always_inline)) UnkRecord_b218(const UnkRecord_b218& other)
        : id(other.id), field_0x8(other.field_0x8), field_0x10(other.field_0x10), field_0x38(other.field_0x38),
          field_0x60(other.field_0x60), field_0x88(other.field_0x88) {}
};
