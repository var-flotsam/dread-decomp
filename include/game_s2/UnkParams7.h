#pragma once

// The larger parameter block (kind 7, vtable g_unk_7101a265a8) that some component handlers build on
// the stack for fn_71006f4b0c. Only the fields the handlers touch are named.

#include "types.h"

extern void* g_unk_7101a265a8[];

struct __attribute__((packed)) UnkParams7 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    f32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    void* field_0x38;
    u8 field_0x40;
    u8 pad_0x41[7];
    void* field_0x48;
    u8 field_0x50;
    u8 pad_0x51[3];
    u32 field_0x54;
    u8 field_0x58;
    u8 field_0x59;
    u8 field_0x5a;
    u8 field_0x5b;
    u8 field_0x5c;
    u8 field_0x5d;
    u8 field_0x5e;
    u8 pad_0x5f[5];
    u8 field_0x64;
    u8 pad_0x65[3];
    u8 field_0x68[16];
    u8 field_0x78;
    u8 pad_0x79[7];
    u8 field_0x80[16];
    u8 field_0x90;
    u8 pad_0x91[7];
    u8 field_0x98;
};

void fn_71006f4b0c(void* target, UnkParams7* params, u32 flag);
