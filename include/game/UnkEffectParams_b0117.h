#pragma once

#include "types.h"

// The block handed to the owner's +0x218 (fn_71010c7e6c); the vtable tells the kinds apart.
struct UnkEffectParams_b0117 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    u8 field_0x18[13];
    u8 pad_0x25[3];
    u8 field_0x28[13];
    u8 pad_0x35[3];
    u32 field_0x38;
    u8 pad_0x3c;
    u8 field_0x3d;
    u8 field_0x3e;
    u8 field_0x3f;
    u8 pad_0x40;
    u8 field_0x41;
    u8 pad_0x42[2];
    u8 field_0x44[13];
    u8 field_0x51;
    u8 field_0x52;
};
