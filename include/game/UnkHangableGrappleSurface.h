#pragma once

// The hangable grapple surface as unit 1283 (CHangableGrappleSurfaceComponent) uses it: its hang point,
// its definition and the grapple message it answers. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

// A point copied member by member (a user-written copy: fn_7100825258 returns it in memory).
struct UnkHangPoint_b50 {
    f32 x, y, z;

    UnkHangPoint_b50(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
    UnkHangPoint_b50(const UnkHangPoint_b50& other) : x(other.x), y(other.y), z(other.z) {}
    UnkHangPoint_b50& operator=(const UnkHangPoint_b50& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }
};

struct UnkHangSource_b50 {
    u8 pad_0x0[0x20];
    base::global::UnkStringData* field_0x20;
};

struct UnkHangSender_b50 {
    u8 pad_0x0[0x10];
    UnkHangSource_b50* field_0x10;
};

struct UnkHangMessage_b50 {
    UnkHangSender_b50* sender;
    u8 pad_0x8[0xc];
    UnkHangPoint_b50 point;  // 0x14
};

struct UnkHangDef_b50 {
    u8 pad_0x0[0x70];
    base::global::UnkStringData* field_0x70;  // CStrId
    f32 field_0x78;  // the length an edge must reach to hang from its middle
};

struct UnkCollisionContainer_l864654;
struct UnkCollisionShape_l864654;

struct UnkHangOwner_l825258 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer_l864654* collision;  // 0x2a0
};

struct UnkHangable_b50 {
    u8 pad_0x0[0x30];
    UnkHangOwner_l825258* owner;  // 0x30
    u8 pad_0x38[0x50];
    UnkHangPoint_b50 point;  // 0x88
    u8 pad_0x94[0x4];
    UnkHangDef_b50* def;  // 0x98
};
