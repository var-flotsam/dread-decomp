#pragma once

// A sphere positioned in the world, embedded in transform nodes (+0x80 of UnkXformNode) and in
// the skinned-model owner (+0x20, src/game_s1/fn_71001c1b70.cpp): a radius, an offset in the
// owner's space, a position, and the position quantised to tenths. Its unit (tu_map 308) is
// src/game/UnkSphere.cpp (the overlap test fn_7100216c2c is the frozen sample
// src/flag_id/fn_7100216c2c.cpp); fn_710021685c constructs it, fn_710021693c sets the radius and
// fn_71002169c4 the offset.
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkSphere {
    void* vtable;
    f32 radius;        // 0x8, also kept at 0x28, and times 10000 as an s64 at 0x38
    UnkVec3f offset;   // 0xc: a point in the owner's space
    UnkVec3f pos;      // 0x18
    f32 field_0x24;    // lower bound of the range [0x24, 0x28] fn_71002169e0 widens
    f32 field_0x28;    // the radius again; upper bound of that range
    f32 field_0x2c;    // lower bound of the second range [0x2c, 0x30]
    f32 base_radius;   // 0x30: the radius before fn_7100216d30's scale
    u8 pad_0x34[4];
    s64 radius_fixed;  // 0x38: the radius times 10000, truncated
    s32 cell[3];       // 0x40: pos times ten, truncated
    bool field_0x4c;   // the quantised radius is zero
    bool field_0x4d;
    u8 pad_0x4e[2];
};
static_assert(sizeof(UnkSphere) == 0x50, "UnkSphere layout");

// Sets the position and its quantised copy. Defined in src/game/UnkSphere.cpp.
void fn_7100216978(UnkSphere* self, const UnkVec3f* position);
// Sets the radius (and its copies and quantised form). Not decompiled yet.
void fn_710021693c(UnkSphere* self, f32 radius);
// Sets the radius (and its copies and quantised form) to the base radius times `scale`.
void fn_7100216d30(UnkSphere* self, f32 scale);
// Grows both ranges to take in the point's distance beyond the radius.
void fn_71002169e0(UnkSphere* self, const UnkVec3f* point);
// Whether `other` lies inside this sphere's range on every axis.
bool fn_7100216ca4(UnkSphere* self, const UnkSphere* other);

// The sphere embedded at +0x80 of a transform node (inside UnkXformNode::pad_0x74).
inline UnkSphere* UnkXformNodeSphere(UnkXformNode* node) {
    return reinterpret_cast<UnkSphere*>(reinterpret_cast<char*>(node) + 0x80);
}
