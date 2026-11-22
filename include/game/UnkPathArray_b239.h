#pragma once

#include "types.h"

struct UnkPathEntry_b239;

struct UnkPathArray_b239 {
    UnkPathEntry_b239* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 field_0x18;
    u64 field_0x20;
};
