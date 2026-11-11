// CMagnetSlidingBlockWithCollisionsComponent's unit (tools/tu_map.py): whether the block can be at time
// `t` of its path: the point there (fn_710091ad8c of the owner's (+0x30) CMagnetMovablePlatformComponent,
// found with fn_71009224dc) is free when the shape of the collision object at +0x4f0 (its +0x158) placed
// there (fn_7100078cd8) hits nothing in the collision tracker of the game manager's +0x608
// (fn_7100d5b484, mask 8) with a query that ignores the owner (vtable g_unk_7101a82e88). Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU365.h"
#include "game/UnkXformNode.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

struct UnkRefl_fn_71009224dc;
template <>
base::reflection::UnkReflObject* base::reflection::refl_FindTyped<UnkRefl_fn_71009224dc>(void*);

struct UnkMinimapPathPoint {
    CVector3D v;
    ~UnkMinimapPathPoint() {}
};
struct UnkMagnetMovablePlatform_b52;
UnkMinimapPathPoint fn_710091ad8c(UnkMagnetMovablePlatform_b52* self, f32 t);

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);

struct UnkTree;
struct UnkShape;
struct UnkMtx44;

extern void* g_unk_7101a82e88[];

struct UnkSlideHitObj_ovb0133 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkSlideHitRef_ovb0133 {
    UnkSlideHitObj_ovb0133* ptr;

    UnkSlideHitRef_ovb0133() : ptr(nullptr) {}
    ~UnkSlideHitRef_ovb0133() {
        if (UnkSlideHitObj_ovb0133* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkSlideHitVec_ovb0133 {
    f32 x, y, z;
    UnkSlideHitVec_ovb0133(f32 x_, f32 y_, f32 z_) : x(x_), y(y_), z(z_) {}
};

struct UnkSlideCollisionHit_ovb0133 {
    UnkSlideHitRef_ovb0133 object;
    UnkSlideHitRef_ovb0133 field_0x8;
    s32 id;
    UnkSlideHitVec_ovb0133 point;
    UnkSlideHitVec_ovb0133 field_0x20;
    UnkSlideHitVec_ovb0133 normal;
    u32 field_0x38;
    u32 flags;
    u32 field_0x40;

    UnkSlideCollisionHit_ovb0133()
        : id(-1), point(0.0f, 0.0f, 0.0f), field_0x20(0.0f, 0.0f, 0.0f), normal(0.0f, 0.0f, 0.0f), field_0x38(0),
          flags(0), field_0x40(0) {}
};

// The hit query, from a point and ignoring an actor, with a second copy of both.
struct UnkSlideQuery_ovb0133 {
    void** vtable;
    UnkSlideCollisionHit_ovb0133 hit;
    bool field_0x50;
    u32 field_0x54;
    UnkVec3f origin;
    void* ignored;  // 0x68
    bool field_0x70;
    u8 pad_0x71[7];
    UnkVec3f origin2;  // 0x78
    void* ignored2;  // 0x88

    UnkSlideQuery_ovb0133(const UnkVec3f& o, void* actor) {
        UnkVec3f p = o;
        field_0x50 = false;
        origin = p;
        ignored = actor;
        vtable = g_unk_7101a82e88 + 2;
        field_0x70 = false;
        origin2 = p;
        ignored2 = actor;
    }
    ~UnkSlideQuery_ovb0133() { vtable = g_unk_7101a20ed8 + 2; }
};

struct UnkSlideOwner_ovb0133 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;

    UnkVec3f Position() const { return UnkVec3f(x, y, z); }
};

struct UnkSlideShaped_ovb0133 {
    u8 pad_0x0[0x158];
    UnkShape* shape;
};

struct UnkSlidingWithCollisions_ovb0133 {
    u8 pad_0x0[0x30];
    UnkSlideOwner_ovb0133* owner;
    u8 pad_0x38[0x4f0 - 0x38];
    UnkWeakHandle* collision_object;  // 0x4f0
};

bool fn_71009225b8(UnkSlidingWithCollisions_ovb0133* self, f32 t) {
    auto* platform = reinterpret_cast<UnkMagnetMovablePlatform_b52*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71009224dc>(self->owner));
    UnkMinimapPathPoint point = fn_710091ad8c(platform, t);
    UnkTree* tree = static_cast<UnkTree*>(g_unk_7101d47310->field_0x608->field_0x820);
    UnkSlideQuery_ovb0133 query(self->owner->Position(), self->owner);
    UnkShape* shape = reinterpret_cast<UnkSlideShaped_ovb0133*>(self->collision_object->target)->shape;
    UnkMatrix44_7100078c9c xf = fn_7100078cd8(reinterpret_cast<const UnkVec3f&>(point.v));
    return !fn_7100d5b484(tree, 8, shape, reinterpret_cast<const UnkMtx44*>(&xf), &query, 1);
}
