#pragma once

#include "types.h"

template <u32 Size>
struct UnkDefIterValue {
    u64 unk_0x0;
    union {
        u8 value[Size];
        void* ref;
    };
    s32 state;

    __attribute__((always_inline)) void* Get() { return state == 2 ? ref : value; }
};
