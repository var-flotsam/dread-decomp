#pragma once

// One object that 2 files viewed in part (fn_7100976588 returns it to both; views of 0x68 and 0x160 bytes whose shared offsets agree). Names and layouts are placeholders.
#include "types.h"

struct UnkShapeInner_o0309;

struct UnkShape {
    void** vtable;
    u8 pad_0x8[0x38];
    f32 field_0x40;
    u8 pad_0x44[0x4];
    f32 field_0x48;
    u8 pad_0x4c[0x14];
    u8 field_0x60;
    u8 pad_0x61[0xf7];
    UnkShapeInner_o0309* field_0x158;

    void vf_0x98() { reinterpret_cast<void (*)(UnkShape*)>(vtable[19])(this); }
};
