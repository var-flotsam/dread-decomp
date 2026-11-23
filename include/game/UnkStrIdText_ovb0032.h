#pragma once

#include "base/global/CRntString.h"
#include "types.h"

// A string id's data as the name read sees it: the text at +0x10 and a use count at +0x44 (a union
// member: the original reloads the id after the increment).
struct UnkStrIdText_ovb0032 {
    u8 pad_0x0[0x10];
    base::global::CRntString str;
    u8 pad_0x40[0x4];
    union {
        s32 uses;  // 0x44
    };
};
