#pragma once

#include "types.h"

struct UnkDeleteSlotButton {
    char* vtable;
    u8 base_0x8[0x2a0];
    void* field_0x2a8;  // a callback: unused word, the object, the function
    void* object;
    void (*fn)();
    s32 slot;  // 0x2c0
};
