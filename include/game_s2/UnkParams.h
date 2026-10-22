#pragma once

// The parameter block that a large family of component handlers builds on the stack and hands to
// fn_71006f4b0c (kind 5 uses the vtable g_unk_7101a25b10; other kinds have their own vtables and
// store the kind at +0x34). Only the fields the handlers touch are named.

#include "types.h"

extern void* g_unk_7101a25b10[];

struct UnkParams {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[3];
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};

void fn_71006f4b0c(void* target, UnkParams* params, u32 flag);
