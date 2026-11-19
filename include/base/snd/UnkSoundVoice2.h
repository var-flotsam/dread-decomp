#pragma once

// A sound voice record of the voice pool (unit 99's lists of live voices and unit 101's reset). Names
// and layout are placeholders; the offsets are the ones src/base/snd/UnkSoundVoiceList_b56.cpp and
// fn_710010519c read and write.
#include "types.h"

// Three floats, set to one value or copied (the voice's three parameter triples).
struct UnkSoundTriple {
    f32 x, y, z;
    UnkSoundTriple() = default;
    explicit UnkSoundTriple(f32 v) : x(v), y(v), z(v) {}
};

struct UnkSoundVoice2_b56 {
    UnkSoundTriple a;  // 0x0
    u8 pad_0xc[0x28 - 0xc];
    f32 one_x;  // 0x28
    f32 one_y;  // 0x2c
    f32 one_z;  // 0x30
    u8 pad_0x34[0x3c - 0x34];
    UnkSoundTriple b;  // 0x3c
    u8 pad_0x48[0x64 - 0x48];
    UnkSoundTriple c;  // 0x64
    u8 pad_0x70[0x78 - 0x70];
    void* owner;       // 0x78
    void* owner_next;  // 0x80
    u8 pad_0x88[0xb4 - 0x88];
    u32 key;      // 0xb4
    s32 group;    // 0xb8
    s32 channel;  // 0xbc
    u8 pad_0xc0[0x4];
    u32 id;  // 0xc4, the sequence number fn_710010519c gives it
    u8 pad_0xc8[0xe0 - 0xc8];
    f32 def_c;  // 0xe0
    u8 pad_0xe4[0xec - 0xe4];
    f32 def_a;  // 0xec
    u8 pad_0xf0[0xf4 - 0xf0];
    f32 def_b;  // 0xf4
    u8 pad_0xf8[0x120 - 0xf8];
    bool flag;  // 0x120
};
