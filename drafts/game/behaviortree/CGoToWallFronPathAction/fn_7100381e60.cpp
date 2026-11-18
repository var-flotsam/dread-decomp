// behaviortree::CGoToWallFronPathAction's unit (tools/tu_map.py unit 479): the node's start. After the base's
// fn_7100394bd0, the path of the actor linked to the owner's AI (+0x1350) is read: a segment from its
// second-to-last point to 1000 units past its last point along the owner's facing is cast into the scenario's
// collision tree, and on a hit the owner's navigation is sent to go to the hit point. Names and layouts are
// placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game/behaviortree/CBehavior.h"
#include "game/behaviortree/CGoToWallFronPathAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::math::CVector2D;
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];
extern void* g_unk_7101a25b60[];

struct UnkBox;

// ---- the path ----

struct UnkPathNode_l381e60 {
    void** vtable;

    const UnkVec3f& vf_0x18() {
        return reinterpret_cast<const UnkVec3f& (*)(UnkPathNode_l381e60*)>(vtable[0x18 / 8])(this);
    }
};

struct UnkPath_l381e60 {
    void** vtable;

    s32 vf_0x28() { return reinterpret_cast<s32 (*)(UnkPath_l381e60*)>(vtable[0x28 / 8])(this); }
    UnkPathNode_l381e60* vf_0x30(s32 index) {
        return reinterpret_cast<UnkPathNode_l381e60* (*)(UnkPath_l381e60*, s32)>(vtable[0x30 / 8])(this, index);
    }

    const UnkVec3f& Point(s32 index) { return vf_0x30(index)->vf_0x18(); }
};

struct UnkPathSource_l381e60 {
    void** vtable;

    UnkPath_l381e60* vf_0x30(u32 index) {
        return reinterpret_cast<UnkPath_l381e60* (*)(UnkPathSource_l381e60*, u32)>(vtable[0x30 / 8])(this, index);
    }
};

// What fn_7100382150 returns, as this function reads it.
struct UnkPathHolder_l381e60 {
    u8 pad_0x0[0x30];
    UnkPathSource_l381e60 source;  // 0x30
};

// ---- the collision-tree segment query ----

struct UnkHitVec_l381e60 {
    f32 x, y, z;
    UnkHitVec_l381e60(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkCollisionHit_l381e60 {
    base::UnkRefPtrMember object;
    base::UnkRefPtrMember field_0x8;
    s32 id;
    UnkVec3f point;
    UnkHitVec_l381e60 field_0x20;
    UnkHitVec_l381e60 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkCollisionHit_l381e60()
        : object{nullptr}, field_0x8{nullptr}, id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f),
          normal(0.0f, 0.0f, 0.0f), field_0x38(0), flags(0), field_0x40(0) {}
};

struct UnkSegmentHitQuery_l381e60 {
    void** vtable;
    UnkCollisionHit_l381e60 hit;
    bool field_0x50;
    u32 field_0x54;
    UnkVec3f origin;
    u64 field_0x68;

    explicit UnkSegmentHitQuery_l381e60(const UnkVec3f& o) {
        vtable = g_unk_7101a20fa8 + 2;
        field_0x50 = false;
        origin.x = o.x;
        origin.y = o.y;
        origin.z = o.z;
        field_0x68 = 0;
    }
    ~UnkSegmentHitQuery_l381e60() { vtable = g_unk_7101a20ed8 + 2; }
};
static_assert(__builtin_offsetof(UnkSegmentHitQuery_l381e60, field_0x68) == 0x68, "layout");

struct UnkHitSegment_l381e60 {
    CVector2D start;
    CVector2D end;
    CVector2D delta;

    UnkHitSegment_l381e60(const CVector2D& s, const CVector2D& e) {
        start.x = s.x;
        start.y = s.y;
        end.x = e.x;
        end.y = e.y;
        delta.x = e.x - s.x;
        delta.y = e.y - s.y;
    }
};

// ---- the navigation request ----

// CAIGotoPointNavigationState::SPointTarget: a point to go to.
struct UnkPointTarget_l381e60 {
    void** vtable;
    UnkVec3f point;

    explicit UnkPointTarget_l381e60(const UnkVec3f& p) {
        vtable = g_unk_7101a25b60 + 2;
        point = p;
    }
};

// The go-to request handed to the navigation (fn_71006f4b0c); its target is a reflected value.
struct UnkGoToParams_l381e60 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x3];
    f32 field_0x18;
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    bool field_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue target;  // 0x70
    bool field_0x88;

    UnkGoToParams_l381e60() {
        __builtin_memset(field_0x8, 0, 13);
        field_0x30 = false;
        __builtin_memset(field_0x20, 0, 16);
        field_0x3e = false;
        vtable = g_unk_7101a25b38 + 2;
        target = base::reflection::refl_EmptyValue();
        field_0x18 = 1.0f;
        field_0x3c = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x1c = true;
        field_0x34 = 1;
        field_0x3f = true;
        field_0x40 = true;
        __builtin_memset(field_0x48, 0, 16);
        field_0x88 = true;
    }
    ~UnkGoToParams_l381e60() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&target);
    }

    void SetTarget(const base::reflection::CClass* type, const void* object) {
        fn_71000a1c60(&target);
        fn_71000a18d4(&target, type, object, false);
    }
};
static_assert(__builtin_offsetof(UnkGoToParams_l381e60, field_0x88) == 0x88, "layout");

// ---- the action ----

struct UnkGoToLink_l381e60 {
    u8 pad_0x0[0x10];
    UnkActor* actor;  // 0x10
};

struct UnkGoToAI_l381e60 {
    u8 pad_0x0[0x1350];
    u8 breakable_part[8];  // 0x1350
};

struct UnkGoToActor_l381e60 {
    u8 pad_0x0[0x38];
    UnkMatrix44 transform;  // 0x38
    u8 pad_0x78[0x1c8];
    UnkGoToAI_l381e60* ai;  // 0x240
};
static_assert(__builtin_offsetof(UnkGoToActor_l381e60, ai) == 0x240, "layout");

struct UnkGoToWallFrontPathAction_l381e60 {
    u8 pad_0x0[0x18];
    UnkGoToActor_l381e60* actor;  // 0x18
};

void fn_7100381e60(UnkGoToWallFrontPathAction_l381e60* self) {
    fn_7100394bd0(reinterpret_cast<UnkStruct_0x68*>(self));
    auto* link = reinterpret_cast<UnkGoToLink_l381e60*>(
        *fn_71002a76a4(reinterpret_cast<UnkBreakablePart*>(self->actor->ai->breakable_part)));
    if (!link || !link->actor)
        return;

    auto* holder = reinterpret_cast<UnkPathHolder_l381e60*>(fn_7100382150(link->actor));
    UnkPath_l381e60* path = holder->source.vf_0x30(0);
    const UnkVec3f& last = path->Point(path->vf_0x28() - 1);
    f32 last_x = last.x;
    f32 last_y = last.y;
    UnkVec3f from = path->Point(path->vf_0x28() - 2);

    // From the second-to-last point to 1000 units past the last one along the owner's facing.
    const UnkMatrix44& m = fn_710002614c(&self->actor->transform);
    CVector2D start = {from.x, from.y};
    CVector2D facing = {m.m[2], m.m[6]};
    CVector2D reach = {facing.x * 1000.0f, facing.y * 1000.0f};
    CVector2D end = {last_x + reach.x, last_y + reach.y};
    UnkHitSegment_l381e60 segment(start, end);
    UnkSegmentHitQuery_l381e60 query(from);
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    if (fn_7100d5c518(tree, 1, reinterpret_cast<const UnkBox*>(&segment), &query, 2) && self->actor &&
        self->actor->ai) {
        UnkVec3f point = query.hit.point;
        if (UnkNavigation* navigation = fn_71010fccf8(reinterpret_cast<UnkAIComponent*>(self->actor->ai))) {
            UnkGoToParams_l381e60 params;
            UnkPointTarget_l381e60 target(point);
            params.SetTarget(fn_7100701c54(), &target);
            fn_71006f4b0c(navigation, &params, 1);
        }
    }
}
