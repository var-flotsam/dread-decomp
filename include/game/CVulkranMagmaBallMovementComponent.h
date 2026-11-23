#pragma once

// CVulkranMagmaBallMovementComponent: the magma ball's movement, as far as its update, its hit test and the
// Spittail ball (which derives from it) use it. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkActor.h"
#include "game/UnkXformNode.h"
#include "types.h"

// The player's part that says whether a magma ball bounces off it.
struct UnkMagmaBallPlayerPart {
    u8 pad_0x0[0x340];
    bool armed;  // 0x340
};
UnkMagmaBallPlayerPart* fn_7100623810(UnkActor* actor);

struct UnkMagmaBallHolder {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkMagmaBallDef {
    u8 pad_0x0[0xed];
    bool no_hit;  // 0xed
};

// What the shot sent back carries (+0x3b4 of the movement).
struct UnkMagmaBallShot {
    UnkVec3f pos;  // 0x0
    UnkVec3f dir;  // 0xc
    f32 field_0x18;
    f32 field_0x1c;
    f32 field_0x20;
    f32 field_0x24;
    s32 field_0x28;
};

struct UnkVulkranMagmaBall;

struct UnkVulkranMagmaBallVtbl {
    void* slots[0x2a8 / 8];
    void (*vf_0x2a8)(UnkVulkranMagmaBall* self, UnkActor* other, void* info);
    void* slots_0x2b0[(0x338 - 0x2b0) / 8];
    void (*vf_0x338)(UnkVulkranMagmaBall* self, UnkMagmaBallShot* shot, const UnkVec3f* angles,
                     const char* kind, base::global::CFilePathStrId* path);
};

struct UnkVulkranMagmaBall {
    UnkVulkranMagmaBallVtbl* vtbl;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0xd4];
    f32 dir_x;  // 0x10c
    f32 dir_y;  // 0x110
    f32 dir_z;  // 0x114
    u8 pad_0x118[0x118];
    UnkMagmaBallHolder* holder;  // 0x230
    u8 pad_0x238[0x178];
    s32 shot_id;            // 0x3b0
    UnkMagmaBallShot shot;  // 0x3b4
    u8 pad_0x3e0[0x1f0];
    UnkMagmaBallDef* def;  // 0x5d0
};
static_assert(__builtin_offsetof(UnkVulkranMagmaBall, shot) == 0x3b4, "layout");
static_assert(__builtin_offsetof(UnkVulkranMagmaBall, def) == 0x5d0, "layout");
static_assert(sizeof(UnkVulkranMagmaBall) == 0x5d8, "layout");

// The kind of shot the ball sends back.
extern const char g_unk_710164a808[] __attribute__((visibility("hidden")));

// The hit test: bounces off the player when its part is armed, else the projectile's own test.
bool fn_71006dfa88(UnkVulkranMagmaBall* self, UnkActor* other, void* a2, void* a3, void* a4, void* a5);
