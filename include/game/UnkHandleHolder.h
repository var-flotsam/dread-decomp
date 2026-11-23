#pragma once

#include "types.h"

struct UnkHandleHolder {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};
