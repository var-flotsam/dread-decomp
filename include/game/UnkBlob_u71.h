#pragma once

#include "types.h"

// A block that is either owned (copied on assignment) or borrowed.
struct UnkBlob_u71 {
    u32 size;
    void* data;  // 0x8
    bool owns;   // 0x10
};
