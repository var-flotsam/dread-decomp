// CSwarmControllerComponent's unit: hands every active group (with something pending) whose
// position lies within the billboard component's radius (plus an extra) of a point, in the plane,
// to a callback. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkTU1161.h"
#include "game/UnkVec3Result.h"

using base::reflection::UnkReflObject;

struct UnkParent_71007455dc;

struct UnkBillboard_l74d808 {
    virtual const base::reflection::CType* vf_0x0();
    u8 pad_0x8[0x54];
    f32 radius;  // 0x5c
};
static_assert(__builtin_offsetof(UnkBillboard_l74d808, radius) == 0x5c, "layout");

struct UnkGroup_l74d808 {
    u8 pad_0x0[0x24];
    bool active;  // 0x24
    u8 pad_0x25[0xbb];
    u32 pending;  // 0xe0
    u8 pad_0xe4[0x64];
};
static_assert(sizeof(UnkGroup_l74d808) == 0x148, "layout");

struct UnkGroups_l74d808 {
    u8 pad_0x0[0x10];
    UnkBillboard_l74d808* billboard;  // 0x10
    u8 pad_0x18[0x38];
    UnkGroup_l74d808* groups;  // 0x50
    u8 pad_0x58[0xb8];
    s32 count;  // 0x110

    UnkGroup_l74d808* Get(s64 i) {
        if (i < count && groups)
            return &groups[i];
        return nullptr;
    }
};

struct UnkGroupsOwner_l74d808 {
    u8 pad_0x0[0x10];
    UnkGroups_l74d808* groups;  // 0x10
};

struct UnkSwarmNear_l74d808 {
    u8 pad_0x0[0xe8];
    UnkGroupsOwner_l74d808* def;  // 0xe8
};

struct UnkGroupCallback_l74d808 {
    void* context;
    void (*function)(void* context, UnkGroup_l74d808* group);
};

// The squared distance, in the plane, from a group's position to a point.
static inline f32 GroupPlanarDistanceSq(UnkGroup_l74d808* group, const base::math::CVector2D* point) {
    UnkVec3Result position = fn_7100745e24(reinterpret_cast<UnkParent_71007455dc*>(group));
    base::math::CVector2D flat;
    fn_710007b430(&flat, reinterpret_cast<const base::math::CVector3D*>(&position));
    f32 dx = flat.x - point->x;
    f32 dy = flat.y - point->y;
    return dx * dx + dy * dy;
}

void fn_710074d808(UnkSwarmNear_l74d808* self, f32 extra, const base::math::CVector2D* point,
                   UnkGroupCallback_l74d808* callback) {
    UnkReflObject* object = reinterpret_cast<UnkReflObject*>(self->def->groups->billboard);
    UnkBillboard_l74d808* billboard =
        fn_710009d50c(object->vf_0x0(), fn_710072be34()) ? reinterpret_cast<UnkBillboard_l74d808*>(object) : nullptr;
    f32 reach = billboard->radius + extra;
    f32 reach_sq = reach * reach;
    for (s32 i = 0; i < self->def->groups->count; ++i) {
        UnkGroup_l74d808* group = self->def->groups->Get(i);
        if (group->pending == 0 || !group->active)
            continue;
        if (GroupPlanarDistanceSq(group, point) < reach_sq)
            callback->function(callback->context, group);
    }
}
