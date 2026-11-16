// Unit 958 (tools/tu_map.py): fn_710063ab04 says whether any entity in the list at +0x40, other than the one
// to ignore, is frozen or answers fn_71011057fc and lies within twice the radius at +0x37c of the point.
// Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "game/CFrozenComponent.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkRobotSoldierAI_A2_b69;

struct UnkNearRow_l63ab04 {
    f32 m[4];
};

struct UnkNearEntity_l63ab04 {
    u8 pad_0x0[0x38];
    UnkNearRow_l63ab04 transform[3];  // 0x38
    u8 pad_0x68[0x240 - 0x68];
    UnkRobotSoldierAI_A2_b69* ai;  // 0x240
    u8 pad_0x248[0x2c8 - 0x248];
    UnkFrostbiteComponent* frostbite;  // 0x2c8
};

struct UnkNearItem_l63ab04 {
    u8 pad_0x0[0x10];
    UnkNearEntity_l63ab04* entity;  // 0x10
};

struct UnkNearNode_l63ab04 {
    UnkNearItem_l63ab04* item;
    UnkNearNode_l63ab04* next;
};

struct UnkNearParams_l63ab04 {
    u8 pad_0x0[0x37c];
    f32 radius;  // 0x37c
};

struct UnkNearOwner_l63ab04 {
    u8 pad_0x0[0x10];
    UnkNearParams_l63ab04* params;  // 0x10
};

struct UnkNearSelf_l63ab04 {
    u8 pad_0x0[0x40];
    UnkNearNode_l63ab04* entities;  // 0x40
    u8 pad_0x48[0xa0 - 0x48];
    UnkNearOwner_l63ab04* owner;  // 0xa0
};

using base::math::CVector3D;

static inline CVector3D GetPosition(const UnkNearEntity_l63ab04* entity) {
    CVector3D p;
    p.x = entity->transform[0].m[3];
    p.y = entity->transform[1].m[3];
    p.z = entity->transform[2].m[3];
    return p;
}

static inline UnkNearEntity_l63ab04* GetEntity(UnkNearItem_l63ab04* item) {
    return item ? item->entity : nullptr;
}

static inline UnkFrostbiteComponent* GetFrostbite(UnkNearEntity_l63ab04* entity) {
    return entity ? entity->frostbite : nullptr;
}

bool fn_710063ab04(UnkNearSelf_l63ab04* self, const CVector3D* pos, UnkNearEntity_l63ab04* ignore) {
    UnkNearNode_l63ab04* node = self->entities;
    if (!node)
        return false;
    f32 diameter = self->owner->params->radius * 2.0f;
    f32 max_dist_sq = diameter * diameter;
    for (; node; node = node->next) {
        UnkNearEntity_l63ab04* entity = GetEntity(node->item);
        if (ignore && entity == ignore)
            continue;
        bool frozen = false;
        UnkFrostbiteComponent* frostbite = GetFrostbite(entity);
        if (frostbite)
            frozen = fn_71007f9f20(frostbite) || fn_71007f9fd0(frostbite);
        UnkRobotSoldierAI_A2_b69* ai = entity->ai;
        bool active = ai ? fn_71011057fc(ai) : false;
        if (!(frozen | active))
            continue;
        if (DistanceSq(*pos, GetPosition(entity)) < max_dist_sq)
            return true;
    }
    return false;
}
