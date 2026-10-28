#pragma once

// CBigkranXSpitAttack, the state machine that fn_71003f24f4 (switch state), fn_71003f268c (update),
// fn_71003f2710 (state 2) and fn_71003f28c8 (state 1) in src/game_s2/ run: its owner actor at
// +0x8, its data block at +0x28, the current state at +0x30 and the requested one at +0x34, and
// the timers and the table index of the later members. Only the members those functions touch are
// spelled out; the names and the layout are placeholders.

#include "game/UnkActor.h"
#include "types.h"

struct UnkSpitVec3 {
    f32 x;
    f32 y;
    f32 z;
};

// One entry of the pattern table: two floats and the word the timer is reloaded from.
struct UnkSpitEntry {
    f32 field_0x0;
    f32 field_0x4;
    u32 field_0x8;
};

// The attack's data: the ranges and the delay state 1 reads (from +0xec), the pattern table
// (+0x148, +0x150 entries) state 2 plays.
struct UnkSpitData {
    u8 pad_0x0[0xec];
    f32 field_0xec;
    f32 field_0xf0;
    f32 field_0xf4;
    f32 field_0xf8;
    f32 field_0xfc;
    u8 pad_0x100[0x48];
    UnkSpitEntry* field_0x148;
    u32 field_0x150;
};
static_assert(__builtin_offsetof(UnkSpitData, field_0x150) == 0x150, "UnkSpitData placeholder layout");

struct UnkSpitAttack {
    void** vtable;
    UnkActor* field_0x8;  // the owner
    u8 pad_0x10[0x18];
    UnkSpitData* field_0x28;
    s32 field_0x30;  // the current state
    s32 field_0x34;  // the requested state
    u8 pad_0x38[0x24];
    u8 field_0x5c[0x18];
    u8 field_0x74[0x20];
    u8 pad_0x94[0x40];
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;  // state 2's timer
    f32 field_0xe0;  // state 1's timer
    f32 field_0xe4;
    u32 field_0xe8;  // the table index
    UnkSpitVec3 field_0xec;
    u8 field_0xf8[8];
};
static_assert(__builtin_offsetof(UnkSpitAttack, field_0xf8) == 0xf8, "UnkSpitAttack placeholder layout");

// Switches to the state `state` when it differs and sets up what that state needs. Defined in
// src/game/CBigkranXSpitAttack/fn_71003f24f4.cpp.
void fn_71003f24f4(UnkSpitAttack* self, u32 state);

// State 1's update: whether the attack should go on to the next state. Defined in
// src/game/CBigkranXSpitAttack/fn_71003f28c8.cpp.
bool fn_71003f28c8(UnkSpitAttack* self, f32 delta);
