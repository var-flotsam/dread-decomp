#pragma once

#include "game/UnkTU2577.h"
#include "types.h"

struct UnkItem_b190;

struct UnkNamedObject_b190 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    base::global::CStrId name;  // 0x8
    UnkItem_b190** items;  // 0x10
    u32 count;  // 0x18
    u8 pad_0x1c[0x1c];
};
