#pragma once

#include "types.h"

struct UnkDropItemVec3_ovb0136 {
    f32 x, y, z;
    UnkDropItemVec3_ovb0136& operator=(const UnkDropItemVec3_ovb0136& other) {
        u64 xy;
        u32 w;
        __builtin_memcpy(&xy, &other, 8);
        __builtin_memcpy(&w, &other.z, 4);
        __builtin_memcpy(this, &xy, 8);
        __builtin_memcpy(&z, &w, 4);
        return *this;
    }
};
