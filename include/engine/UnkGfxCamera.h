#pragma once

#include "base/UnkMatrix44.h"

// The camera at +0x317aa20 as the draw modes use it.
struct UnkGfxCamera {
    u8 unk_0x0[0x20];
    UnkMatrix44 projection;  // 0x20
    u8 unk_0x60[0x8];
    void* field_0x68[2];  // indexed by field_0x78
    u32 field_0x78;
    u8 unk_0x7c[0x4];
    void* field_0x80;
};
