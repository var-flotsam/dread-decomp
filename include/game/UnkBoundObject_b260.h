#pragma once

#include "base/global/CStrId.h"
#include "types.h"

struct UnkBoundObject_b260 {
    u8 unk_0x0[0x30];
    base::global::CStrId key;   // 0x30
    u8 unk_0x38[0x90];
    void* owner;  // 0xc8
    u8 unk_0xd0[0xe5];
    u8 detached;  // 0x1b5
};
