// CFlockingSwarmControllerComponent unit: spreads a group's members around a circle: each takes one
// sector from a random starting angle, a random angle within it, a random distance (as a fraction of
// the group's radius) and phase, and a random height in [-25, 25]. Names and layouts are placeholders.
#include <cstdlib>
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game_s1/fn_7100078c9c.cpp defines it: v turned by `angle` about the z axis.
UnkVec3f fn_710007a390(const UnkVec3f& v, f32 angle);

struct UnkSwarmMember_l7397a0 {
    u8 pad_0x0[0x2c];
    UnkVec3f offset;  // 0x2c
    u8 pad_0x38[0x44 - 0x38];
    f32 field_0x44;  // distance, as a fraction of the radius
    f32 field_0x48;  // phase
    u8 pad_0x4c[0x6c - 0x4c];
    u32 field_0x6c;
    u32 field_0x70;
};
static_assert(__builtin_offsetof(UnkSwarmMember_l7397a0, field_0x6c) == 0x6c, "layout");

struct UnkSwarmGroup_l7397a0 {
    u8 pad_0x0[0x40];
    f32 radius;  // 0x40
    u8 pad_0x44[0xb8 - 0x44];
    UnkSwarmMember_l7397a0** members;  // 0xb8
    u32 count;                         // 0xc0

    UnkSwarmMember_l7397a0* At(u32 i) { return i < count ? members[i] : nullptr; }
};
static_assert(__builtin_offsetof(UnkSwarmGroup_l7397a0, count) == 0xc0, "layout");

struct UnkSwarmSettings_l7397a0 {
    u8 pad_0x0[0x468];
    u32 field_0x468;
};

struct UnkFlockingSwarm_l7397a0 {
    u8 pad_0x0[0x328];
    UnkSwarmSettings_l7397a0* settings;  // 0x328
};

static inline f32 Random01_l7397a0() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }
static inline f32 RandomRange_l7397a0(f32 min, f32 max) { return (max - min) * Random01_l7397a0() + min; }

void fn_71007397a0(UnkFlockingSwarm_l7397a0* self, UnkSwarmGroup_l7397a0* group) {
    const f32 kTwoPi = 6.2831855f;
    u32 n = group->count;
    if (n == 0)
        return;
    f32 halfSector = kTwoPi / ((f32)n * 2.0f);
    f32 radius = group->radius;
    f32 angle = RandomRange_l7397a0(0.0f, kTwoPi);
    for (u32 i = 0; i < n; ++i) {
        UnkSwarmMember_l7397a0* member = group->At(i);
        f32 memberAngle = angle + RandomRange_l7397a0(-halfSector, halfSector);
        f32 distance = RandomRange_l7397a0(0.0f, 1.0f);
        member->field_0x44 = distance;
        member->field_0x48 = RandomRange_l7397a0(0.0f, kTwoPi);
        UnkVec3f offset(radius * distance, 0.0f, 0.0f);
        offset = fn_710007a390(offset, memberAngle);
        offset.z = RandomRange_l7397a0(-25.0f, 25.0f);
        member->offset = offset;
        member->field_0x6c = member->field_0x70 = self->settings->field_0x468;
        angle += halfSector * 2.0f;
    }
}
