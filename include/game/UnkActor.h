#pragma once

// An actor as the hand-written AI sources (game_s2, game_s7, game_hi) see it: a transform at
// +0x38 (the UnkXform of game/UnkXformNode.h, whose translation column the callers read directly
// as the three floats at +0x44, +0x54 and +0x64), component and object slots from +0x1b8 to
// +0x2e8, and the target slot at +0x290 that fn_7100d7c990 below follows. Only the members the
// hand-written callers share are spelled out; the names and the layout are placeholders.

#include "types.h"

struct UnkActor {
    u8 pad_0x0[0x38];
    u8 field_0x38[0xc];  // the transform; fn_710002614c returns its matrix
    f32 field_0x44;      // translation x
    u8 pad_0x48[0xc];
    f32 field_0x54;      // translation y
    u8 pad_0x58[0xc];
    f32 field_0x64;      // translation z
    u8 pad_0x68[0x150];
    void* field_0x1b8;   // tested with fn_7101161088
    u8 pad_0x1c0[0x18];
    void* field_0x1d8;
    u8 pad_0x1e0[0x18];
    void* field_0x1f8;   // answers vf_0x2d8 (a tracked entry) and fn_7100979a60 (a position)
    u8 pad_0x200[0x8];
    void* field_0x208;   // queried with fn_71009e12d0, fn_71009e8590, fn_71009e75e8
    u8 pad_0x210[0x8];
    void* field_0x218;
    void* field_0x220;   // the character class: tracks are looked up in it (fn_71011771e0, fn_710117b348)
    void* field_0x228;   // what shape queries run against (fn_71011735e8)
    u8 pad_0x230[0x10];
    void* field_0x240;   // the AI component (its class is tested against the *AIComponent getters)
    u8 pad_0x248[0x48];
    void* field_0x290;   // the target slot: its +0x40 is the UnkWeakHandle (game_s2/UnkWeakHandle.h) of the target
    void* field_0x298;
    u8 pad_0x2a0[0x10];
    void* field_0x2b0;
    u8 pad_0x2b8[0x30];
    void* field_0x2e8;
    u8 pad_0x2f0[0x8c];
    f32 field_0x37c;     // a radius (fn_710049eb04 adds the two actors')
};
static_assert(sizeof(UnkActor) == 0x380, "UnkActor placeholder layout");

// The actor an actor is acting against: the target that its slot at +0x290 holds through a weak
// handle (null when the handle is empty), else the player (fn_7100dec788 of the game manager
// g_unk_7101d47310), unless the actor is the player itself. The definition is generated (src/lifted/).
UnkActor* fn_7100d7c990(UnkActor* actor);
