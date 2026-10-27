#pragma once

// A sound object: the voice the dialogs play (game/UnkAdamDialog.cpp) and the target of the sound
// handles the callers keep (a reference with a serial). The functions on it seen so far
// (fn_7100105034, fn_7100105a80, fn_7100105ef4) are in unit 101 of tools/tu_map.py
// (0x7100104ef4-0x71001061f4), between the sound system's units 99 and 103. Only the members
// fn_7100105a80 touches are spelled out; the name and the layout are placeholders.

#include "types.h"

struct UnkSound;

// The pending callback object at +0x80: told once (vf_0) and then dropped.
class UnkSoundCallback {
public:
    virtual void vf_0(UnkSound* owner) = 0;
};

struct UnkSound {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    f32 field_0xc;
    u32 field_0x10;
    u8 pad_0x14[0x78 - 0x14];
    void* field_0x78;
    UnkSoundCallback* field_0x80;
    u8 pad_0x88[0x100 - 0x88];
    f32 field_0x100;
    f32 field_0x104;
    u8 pad_0x108[0x110 - 0x108];
    f32 field_0x110;  // a running countdown while positive
    u8 pad_0x114[0x118 - 0x114];
    bool field_0x118;  // a request is queued
    u8 pad_0x119[0x121 - 0x119];
    bool field_0x121;
    bool field_0x122;
};
static_assert(__builtin_offsetof(UnkSound, field_0x78) == 0x78, "UnkSound placeholder layout");
static_assert(__builtin_offsetof(UnkSound, field_0x100) == 0x100, "UnkSound placeholder layout");
static_assert(__builtin_offsetof(UnkSound, field_0x110) == 0x110, "UnkSound placeholder layout");
static_assert(__builtin_offsetof(UnkSound, field_0x118) == 0x118, "UnkSound placeholder layout");
static_assert(__builtin_offsetof(UnkSound, field_0x122) == 0x122, "UnkSound placeholder layout");

// Releases the pending callback object, then applies a queued request: either cancels a running
// countdown, or stores `value` (the stored one when negative) at +0x100 and `limit` (at most 10) at
// +0x104; with no limit (0 or below) the helper fn_71000ff8c4 is told through the object at +0x78
// and, for a positive `value`, a fresh countdown starts at +0x0. The callers pass 0.2 and 0.
// Defined in src/game_lo/fn_7100105a80.cpp.
void fn_7100105a80(UnkSound* self, f32 value, f32 limit);
