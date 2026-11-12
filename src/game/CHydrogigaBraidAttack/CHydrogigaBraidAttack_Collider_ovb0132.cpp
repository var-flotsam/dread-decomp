// CHydrogigaBraidAttack's unit (tools/tu_map.py unit 875): the braid's collider: created at the start
// (fn_71005b39d8, which also hooks fn_71005b4608 up to the tracked object) and stretched each update
// (fn_71005b3cf8). Names and layouts are placeholders.
#include <utility>

#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CHydrogigaAttack.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCollisionRecord.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"

using base::global::CStrId;

void fn_71005b4608();

extern CStrId g_unk_71726eaa10 __attribute__((visibility("hidden")));

using UnkBraidCreateCollider_ovb0132 = UnkCollisionObject* (*)(UnkCollisionContainer*, const CStrId*, const CStrId*,
                                                               const CStrId*, const CStrId*, u32, u32, u32, void*,
                                                               s32, void*, s32, void*, s32, bool, bool, bool);

struct UnkBraidLink_ovb0132 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkBraidOwnerView2_ovb0132 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // +0x2a0
};

struct UnkBraidStart_ovb0132 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // +0x8
    u8 pad_0x10[0x58];
    bool field_0x68;
    u8 pad_0x69[0x7];
    UnkBraidLink_ovb0132 link;  // +0x70
    UnkWeakHandle* handle;      // +0x88
};

// The attack's start: after the base start, hooks fn_71005b4608 up to the object the owner tracks
// and creates the braid's collider (g_unk_71726eaa10).
void fn_71005b39d8(UnkBraidStart_ovb0132* self) {
    fn_71005b2c58(self);
    self->field_0x68 = false;
    UnkActor* owner = self->owner;
    if (UnkWeakTarget* track = static_cast<UnkWeakTarget*>(owner->field_0x220)) {
        UnkBraidLink_ovb0132* link = &self->link;
        link->context = self;
        link->callback = fn_71005b4608;
        if (UnkWeakHandle* handle = self->handle) {
            if (UnkWeakTarget* target = handle->target)
                fn_710117b3f0(target, link);
            if (UnkWeakHandle* p = self->handle) {
                if (--p->ref_count == 0)
                    p->vf_0x10();
                self->handle = nullptr;
            }
        }
        asm("" : "+r"(self));  // as src/game/CChozoCommanderHypersparkAttack/CHypersparkAttack_Start_A2_b73.cpp
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
        owner = self->owner;
    }
    UnkCollisionContainer* collision = reinterpret_cast<UnkBraidOwnerView2_ovb0132*>(owner)->collision;
    if (!collision)
        return;
    const CStrId* type =
        reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x628);
    UnkBraidCreateCollider_ovb0132 create = (*reinterpret_cast<UnkBraidCreateCollider_ovb0132**>(collision))[0x168 / 8];
    create(collision, &g_unk_71726eaa10, &g_unk_71726eaa10, &g_unk_7101d470e0, type, 0x180, 0, 1, nullptr, -1,
           nullptr, -1, nullptr, -1, false, false, true);
}

using base::global::CStrId;
using base::math::CVector2D;
using base::math::CVector3D;

typedef u32 __attribute__((may_alias)) a_u32_ovb0132;

// A 2D point copied word by word.
struct UnkVec2Bits_ovb0132 {
    f32 x, y;
    UnkVec2Bits_ovb0132(const CVector2D& o) {
        ((a_u32_ovb0132*)this)[0] = ((const a_u32_ovb0132*)&o)[0];
        ((a_u32_ovb0132*)this)[1] = ((const a_u32_ovb0132*)&o)[1];
    }
};

struct UnkEntity;

extern CStrId g_unk_71726eaa10 __attribute__((visibility("hidden")));

// A collider's shape; kind 4 (vf_0x18) is a box whose extent is at +0xa8.
struct UnkBraidShape_ovb0132 {
    void** vtable;
    u8 pad_0x8[0xa0];
    f32 left;    // +0xa8
    f32 bottom;  // +0xac
    f32 right;   // +0xb0
    f32 top;     // +0xb4

    s32 Kind() { return reinterpret_cast<s32 (*)(UnkBraidShape_ovb0132*)>(vtable[0x18 / 8])(this); }
    void Update() { reinterpret_cast<void (*)(UnkBraidShape_ovb0132*)>(vtable[0x80 / 8])(this); }
};

struct UnkBraidCollider_ovb0132 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkBraidShape_ovb0132* shape;  // +0x158

    void Refresh() { reinterpret_cast<void (*)(UnkBraidCollider_ovb0132*)>(vtable[0x48 / 8])(this); }
};

struct UnkBraidOwnerView_ovb0132 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // +0x2a0
};

struct UnkBraidAttack_ovb0132 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // +0x8
};

void fn_71005b2fe0(UnkBraidAttack_ovb0132* self);

// After fn_71005b2fe0, stretches the owner's braid collider (g_unk_71726eaa10) between the owner and
// the node 0xd8e6f43e2dc84f93 of its shape, up to 200 above the owner.
void fn_71005b3cf8(UnkBraidAttack_ovb0132* self) {
    fn_71005b2fe0(self);
    UnkCollisionContainer* collision = reinterpret_cast<UnkBraidOwnerView_ovb0132*>(self->owner)->collision;
    if (!collision)
        return;
    UnkBraidCollider_ovb0132* collider =
        reinterpret_cast<UnkBraidCollider_ovb0132*>(fn_71008640a4(collision, &g_unk_71726eaa10));
    if (!collider)
        return;
    u64 key;
    UnkMatrix44 m = {};
    m.m[15] = 1.0f;
    m.m[10] = 1.0f;
    m.m[5] = 1.0f;
    m.m[0] = 1.0f;
    UnkEntity* entity = static_cast<UnkEntity*>(self->owner->field_0x228);
    key = 0xd8e6f43e2dc84f93;
    if (!entity || !fn_71011735e8(entity, &key, &m, 0))
        return;
    CVector2D p;
    UnkActor* owner = self->owner;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    CVector3D pos = {x, y, z};
    f32 node_x = m.m[3];
    fn_710007b430(&p, &pos);
    UnkVec2Bits_ovb0132 q(p);
    f32 top = q.y + 200.0f;
    if (node_x < self->owner->field_0x44)
        std::swap(p.x, node_x);
    f32 left = p.x;
    f32 bottom = q.y;
    f32 right = node_x;
    UnkBraidShape_ovb0132* shape = collider->shape;
    UnkBraidShape_ovb0132* box = shape->Kind() == 4 ? shape : nullptr;
    box->left = left;
    box->bottom = bottom;
    box->right = right;
    box->top = top;
    box->Update();
    collider->Refresh();
}
