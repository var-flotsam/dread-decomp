#pragma once

#include "types.h"

// As CCooldownXBossFireBallDef_Array_ovb0072.cpp defines the element (the same type).
// A flag, a word and a flag, copied as one 9-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkFireBallDefData_ovb0072 {
    bool field_0x0;
    u8 pad_0x1[3];
    f32 field_0x4;
    bool field_0x8;
};
