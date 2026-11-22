#pragma once

#include "types.h"

struct UnkInsertTarget;

struct UnkInsertOwner {
    u8 unk_0x0[0x8];
    UnkInsertTarget* target;  // the object itself is at 8
    u8 unk_0x10[0x20];
    bool dirty;
};
