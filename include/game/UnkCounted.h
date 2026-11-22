#pragma once

#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCounted {
    void** vtable;
    u32 count;
    u32 pad_0xc;
    UnkWeakTarget* field_0x10;
    void vf_0x10() { reinterpret_cast<void (*)(UnkCounted*)>(vtable[2])(this); }
};
