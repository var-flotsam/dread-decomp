// CChozoCommanderSentenceSphereMovementComponent's unit (tools/tu_map.py): applying the sphere's radius
// (+0x5dc, over its base radius +0x5e0) to the owner's model part (fn_7101152b90) and its collider
// (fn_71008640a4, a sphere: fn_7100d618f8), then lifting the owner so the sphere's bottom stays above the
// navigation mesh point under it (the movement's vf_0x2d8, as
// src/game/CAutsharpAIComponent/CAutsharpAIComponent_NotAcross_A2_b71.cpp reads it). Names and layouts
// are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::math::CVector2D;

struct UnkComponent;
struct UnkObject;
UnkComponent* fn_7101152b90(UnkObject*, u64);
extern __attribute__((visibility("hidden"))) base::global::CStrId g_unk_71726ca0e0;

struct UnkSphereShape_ovb0135 {
    void** vtable;
    s32 vf_0x18() { return reinterpret_cast<s32 (*)(UnkSphereShape_ovb0135*)>(vtable[0x18 / 8])(this); }
    void vf_0x78() { reinterpret_cast<void (*)(UnkSphereShape_ovb0135*)>(vtable[0x78 / 8])(this); }
};
void fn_7100d618f8(UnkSphereShape_ovb0135* self, f32 radius);

struct UnkSphereCollider_ovb0135 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkSphereShape_ovb0135* shape;  // 0x158

    void vf_0x48() { reinterpret_cast<void (*)(UnkSphereCollider_ovb0135*)>(vtable[0x48 / 8])(this); }
};

struct UnkSpherePart_ovb0135 {
    void** vtable;
    void vf_0x20(const UnkVec3f* scale) {
        reinterpret_cast<void (*)(UnkSpherePart_ovb0135*, const UnkVec3f*)>(vtable[0x20 / 8])(this, scale);
    }
};

struct UnkNavMeshHit;
struct UnkElem_7100286624;
void fn_710028760c(UnkElem_7100286624* self);

struct UnkNavMeshHit {
    u16 cell;
    u8 side;
    CVector2D point;

    ~UnkNavMeshHit() { fn_710028760c(reinterpret_cast<UnkElem_7100286624*>(this)); }

    bool IsValid() const { return cell != 0xffff && side != 0xff; }
};

struct UnkSphereMovementNav_ovb0135 {
    void** vtable;
    UnkNavMeshHit vf_0x2d8() {
        return reinterpret_cast<UnkNavMeshHit (*)(UnkSphereMovementNav_ovb0135*)>(vtable[0x2d8 / 8])(this);
    }
};

struct UnkSphereOwner_ovb0135 {
    void** vtable;
    u8 pad_0x8[0x3c];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x218];
    UnkObject* model;  // 0x280
    u8 pad_0x288[0x18];
    UnkCollisionContainer* collision;  // 0x2a0

    void vf_0xd0(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkSphereOwner_ovb0135*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};
static_assert(__builtin_offsetof(UnkSphereOwner_ovb0135, collision) == 0x2a0, "layout");

struct UnkSphereTargetActor_ovb0135 {
    u8 pad_0x0[0x1f8];
    UnkSphereMovementNav_ovb0135* movement;  // 0x1f8
};
struct UnkSphereTarget_ovb0135 {
    u8 pad_0x0[0x10];
    UnkSphereTargetActor_ovb0135* actor;  // 0x10
};

struct UnkSentenceSphereMovement_ovb0135 {
    u8 pad_0x0[0x30];
    UnkSphereOwner_ovb0135* owner;  // 0x30
    u8 pad_0x38[0x1f8];
    UnkSphereTarget_ovb0135* target;  // 0x230
    u8 pad_0x238[0x3a4];
    f32 radius;       // 0x5dc
    f32 base_radius;  // 0x5e0
};
static_assert(__builtin_offsetof(UnkSentenceSphereMovement_ovb0135, base_radius) == 0x5e0, "layout");

void fn_710045b8e0(UnkSentenceSphereMovement_ovb0135* self) {
    if (UnkObject* model = self->owner->model) {
        f32 radius = self->radius;
        f32 base_radius = self->base_radius;
        if (auto* part = reinterpret_cast<UnkSpherePart_ovb0135*>(fn_7101152b90(model, 0xeadba676664dfe2cull))) {
            f32 s = radius / base_radius;
            UnkVec3f scale(s, s, s);
            part->vf_0x20(&scale);
        }
    }
    if (UnkCollisionContainer* collision = self->owner->collision) {
        auto* collider = reinterpret_cast<UnkSphereCollider_ovb0135*>(fn_71008640a4(collision, &g_unk_71726ca0e0));
        if (collider) {
            UnkSphereShape_ovb0135* shape = collider->shape;
            if (shape && shape->vf_0x18() == 2) {
                fn_7100d618f8(shape, self->radius);
                shape->vf_0x78();
                collider->vf_0x48();
            }
        }
    }
    UnkSphereTarget_ovb0135* target = self->target;
    if (!target || !target->actor || !target->actor->movement)
        return;
    UnkNavMeshHit hit = target->actor->movement->vf_0x2d8();
    if (hit.IsValid()) {
        UnkSphereOwner_ovb0135* owner = self->owner;
        UnkVec3f at(owner->x, owner->y, owner->z);
        UnkVec3f pos = at;
        f32 bottom = pos.y - self->radius + -25.0f;
        f32 ground = hit.point.y;
        if (bottom < ground) {
            pos.y = (ground - bottom) + pos.y;
            owner->vf_0xd0(&pos, true);
        }
    }
}

