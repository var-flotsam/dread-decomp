#pragma once

#include "game/UnkPathPointTail.h"

struct UnkPathPointEntry {
    void** vtable;
    u32 field_0x8;
    u32 field_0xc;
    UnkPathPointTail tail;
    u32 field_0x18;
    u8 pad_0x1c[0x4];

    void Destroy() { reinterpret_cast<void (*)(UnkPathPointEntry*)>(vtable[1])(this); }
};
