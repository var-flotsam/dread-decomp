#pragma once

// Samus's movement as CSamusMovement's unit (tools/tu_map.py unit 1501) and the ground state's parameters
// (src/game/CSamusGroundMovementState/CParams/CParams_Default_ovb0138.cpp) see it. Only the members those
// touch are spelled out; src/game/CSamusMovement.cpp has another view of the same object (UnkSamusMovement).
// Names and layout are placeholders.
#include "types.h"

struct UnkAnimationComponent;
struct UnkInventory;

// The run information the movement holds at +0x5458.
struct UnkRunInfo_l9ea070 {
    u8 pad_0x0[0xc0];
    f32 field_0xc0;  // the speed floor on landing
    u32 field_0xc4;
};

// The owning actor.
struct UnkActor_l9ea070 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;  // 0x220
    u8 pad_0x228[0x48];
    UnkInventory* inventory;  // 0x270
    u8 pad_0x278[0x30];
    void* gun;          // 0x2a8
    void* field_0x2b0;  // 0x2b0
};

struct UnkGroundMovement_ovb0138 {
    u8 pad_0x0[0x30];
    UnkActor_l9ea070* actor;  // 0x30
    u8 pad_0x38[0x1c8];
    f32 field_0x200;  // 0x200
    u8 pad_0x204[0x3a8 - 0x204];
    bool field_0x3a8;  // 0x3a8
    u8 pad_0x3a9[0x7c8 - 0x3a9];
    u32 field_0x7c8;  // 0x7c8
    u8 pad_0x7cc[0x2290 - 0x7cc];
    u32 field_0x2290;  // 0x2290
    u8 pad_0x2294[0x4];
    f32 field_0x2298;  // 0x2298
    u8 pad_0x229c[0x6c];
    u8 field_0x2308;  // 0x2308
    u8 pad_0x2309[0x1b];
    u8 field_0x2324;  // 0x2324
    u8 field_0x2325;  // 0x2325
    u8 pad_0x2326[0x2];
    u64 field_0x2328;  // 0x2328
    u8 pad_0x2330[0xc];
    u32 field_0x233c;  // 0x233c
    u8 pad_0x2340[0x3118];
    UnkRunInfo_l9ea070* info;  // 0x5458
};
static_assert(__builtin_offsetof(UnkGroundMovement_ovb0138, field_0x3a8) == 0x3a8, "layout");
static_assert(__builtin_offsetof(UnkGroundMovement_ovb0138, field_0x7c8) == 0x7c8, "layout");
static_assert(__builtin_offsetof(UnkGroundMovement_ovb0138, field_0x2324) == 0x2324, "layout");
static_assert(__builtin_offsetof(UnkGroundMovement_ovb0138, info) == 0x5458, "layout");
