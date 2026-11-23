#pragma once

#include "types.h"

struct UnkGoliathIdData_b55 {
    u8 pad_0x0[0x10];
    const char* text;  // a CRntString starts here
    u8 pad_0x18[0x2c];
    union {
        s32 count;  // a union member: the original reloads around its stores
    };
};
