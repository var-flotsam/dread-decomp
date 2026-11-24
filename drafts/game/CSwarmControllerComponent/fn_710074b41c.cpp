// CSwarmControllerComponent's unit: counts the live members of the active groups that lie within a
// distance of a point. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "game/UnkTU1159.h"
#include "types.h"

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec3Result {
    f32 x;
    f32 y;
    f32 z;
    UnkVec3Result(const UnkVec3Result& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkStruct_71007455dc;
UnkVec3Result GetPositionFromParentAndOffset(UnkStruct_71007455dc* self);

struct UnkGroup_l74b41c {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0x93];
    UnkSwarmMember_ovb6** members;  // 0xb8
    u32 count;                      // 0xc0
    u8 pad_0xc4[0x84];
};
static_assert(sizeof(UnkGroup_l74b41c) == 0x148, "layout");

struct UnkGroups_l74b41c {
    u8 pad_0x0[0x50];
    UnkGroup_l74b41c* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110
};

struct UnkGroupsOwner_l74b41c {
    u8 pad_0x0[0x10];
    UnkGroups_l74b41c* groups;  // 0x10
};

struct UnkSwarmCount_l74b41c {
    u8 pad_0x0[0xe8];
    UnkGroupsOwner_l74b41c* def;  // 0xe8
};

s32 fn_710074b41c(UnkSwarmCount_l74b41c* self, const base::math::CVector3D* point, f32 distance) {
    f32 distance_sq = distance * distance;
    s32 total = 0;
    for (s32 i = 0; i < self->def->groups->count; ++i) {
        UnkGroups_l74b41c* groups = self->def->groups;
        if (i >= groups->count || !groups->groups)
            continue;
        UnkGroup_l74b41c* group = &groups->groups[i];
        if (!group->active)
            continue;
        for (u32 j = 0; j < group->count; ++j) {
            UnkSwarmMember_ovb6* member = group->members[j];
            if (!member || !fn_7100744e1c(member))
                continue;
            UnkVec3Result position = GetPositionFromParentAndOffset(reinterpret_cast<UnkStruct_71007455dc*>(member));
            f32 dx = point->x - position.x;
            f32 dy = point->y - position.y;
            f32 dz = point->z - position.z;
            if (dx * dx + dy * dy + dz * dz < distance_sq)
                ++total;
        }
    }
    return total;
}
