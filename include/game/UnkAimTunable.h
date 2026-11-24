#pragma once

// One object that 3 files viewed in part (the tunable global g_unk_71726b1438, which all three read; views of 188, 188 and 148 bytes whose shared offsets agree). Names and layouts are placeholders.
#include "types.h"

struct __attribute__((packed)) UnkAimTunable {
    void** vtable;
    u8 pad_0x8[0x1c];
    u64 field_0x24;
    u32 field_0x2c;
    u8 pad_0x30[0x4];
    u64 field_0x34;
    u32 field_0x3c;
    u8 pad_0x40[0x4];
    u64 field_0x44;
    u64 field_0x4c;
    u64 field_0x54;
    u8 pad_0x5c[0x8];
    u64 field_0x64;
    u64 field_0x6c;
    u64 field_0x74;
    u64 field_0x7c;
    u64 field_0x84;
    u64 field_0x8c;
    u8 pad_0x94[0x14];
    f32 field_0xa8;
    u8 pad_0xac[0xc];
    f32 field_0xb8;
};
