#pragma once

#include "types.h"

// Whether the word at 0x24 of the argument's object is 2.
struct UnkStateObject {
    u8 unk_0x0[0x24];
    s32 field_0x24;
};
