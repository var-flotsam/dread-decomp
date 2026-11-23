#pragma once

#include "types.h"

struct UnkEntry;

struct UnkTarget {
    void** vtable;
    u8 pad_0x8[0xa0];
    UnkEntry* field_0xa8;
    u32 field_0xb0;
    u8 pad_0xb4[0x4];
    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkTarget*)>(vtable[3])(this); }
    void vf_0x178(void* id) { reinterpret_cast<void (*)(UnkTarget*, void*)>(vtable[47])(this, id); }
};
