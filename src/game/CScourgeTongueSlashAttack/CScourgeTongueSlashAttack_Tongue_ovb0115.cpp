// CScourgeTongueSlashAttack's unit (tools/tu_map.py unit 1031): the attack's start checks, its aim and its
// tongue collider. fn_710069a7e4 is the start check: the base check (fn_7100360b84), then, when asked, the
// target's +0x1b8 object (fn_7101161088), the actor's +0x218 (fn_71010cab0c) and the lazily created manager's
// list at +0x5e0 (fn_71006a22b0) must allow it before fn_710069a8e8 decides. fn_710069ae3c runs it with a
// query built from the actor's attack parameters (fn_71002c6f80) and the flat distance to the target.
// fn_710069afac updates the aim (+0x64 angle, +0x68 distance) from the CScourgeAIComponent and hands it to
// the AI (stored at +0x14748 while the CCharClassScourgeBlockAimTrack holds, else fn_710069f604).
// fn_710069b8a0 gives the tongue's collider (created on first use as a 2D polygon collection) an outline.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAttack.h"
#include "game/CEnemyMovement.h"
#include "game/CScourgeAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkXformNode.h"
#include "game/logic/collision/CPolygonCollectionShape.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern "C" float sqrtf(float);

struct UnkScourgeSlash_b0115 {
    void** vtable;
    UnkActor* actor;
    u8 pad_0x10[0x54];
    f32 angle;     // 0x64
    f32 distance;  // 0x68
};
bool fn_710069a8e8(UnkScourgeSlash_b0115* self, void* a, void* b);

bool fn_7101161088(void* object);
bool fn_71006a22b0(void* list, UnkActor* actor);

bool fn_710069a8e8(UnkScourgeSlash_b0115* self, void* a, void* b);

static inline UnkManager_71726af468* GetManager_b0115() {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    return manager;
}

bool fn_710069a7e4(UnkScourgeSlash_b0115* self, void* arg, u32 mode, bool check, void* a, void* b) {
    bool ok = fn_7100360b84(reinterpret_cast<CAttack*>(self), arg, mode);
    UnkActor* target = fn_7100d7c990(self->actor);
    void* object = target ? target->field_0x1b8 : nullptr;
    if (check) {
        void* slot = self->actor->field_0x218;
        if (ok && object) //X
            ok = !fn_7101161088(object);
        if (slot && ok) {
            if (fn_71010cab0c(slot))
                return false;
        } else if (!ok) {
            return false;
        }
        UnkManager_71726af468* manager = GetManager_b0115();
        if (!fn_71006a22b0(reinterpret_cast<u8*>(manager) + 0x5e0, self->actor))
            return false;
    } else if (!ok) {
        return false;
    }
    return fn_710069a8e8(self, a, b);
}

extern char g_unk_7101a21540[];

struct UnkScourgeAIView_ovb0115 {
    u8 pad_0x0[0x1140];
    u32 field_0x1140;
    u8 pad_0x1144[0x24];
    u8 field_0x1168[8];  // the attack parameters
};

// A list of nodes (link at +8) freed through the allocator.
struct UnkAttackNodeList_ovb0115 {
    void* vtable;
    u32 count;
    void* head;
    void* tail;
    base::UnkAllocator* allocator;

    ~UnkAttackNodeList_ovb0115() {
        vtable = g_unk_7101a21540 + 0x10;
        void* n = head;
        while (n) {
            void* next = *reinterpret_cast<void**>(static_cast<char*>(n) + 8);
            allocator->vf_0x48(n);
            n = next;
        }
        count = 0;
        head = nullptr;
        tail = nullptr;
    }
};

// The query an attack check takes: built by fn_71002c6f80 from the actor and the parameters.
struct UnkAttackQuery_ovb0115;
void fn_71002c6f80(UnkAttackQuery_ovb0115* self, UnkActor* actor, void* params);
// A counted reference released on destruction.
struct UnkAttackRef_ovb0115 {
    UnkRefObject* p;
    ~UnkAttackRef_ovb0115() {
        if (p) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkAttackQuery_ovb0115 {
    UnkAttackRef_ovb0115 ref;
    f32 distance[2];  // 0x8
    u8 pad_0x10[0x18];
    UnkAttackNodeList_ovb0115 nodes;  // 0x28
    u8 pad_0x50[0x8];

    UnkAttackQuery_ovb0115(UnkActor* actor, void* params) { fn_71002c6f80(this, actor, params); }
};

bool fn_710069ae3c(UnkScourgeSlash_b0115* self, bool check, void* a, void* b) {
    UnkScourgeAIView_ovb0115* ai = static_cast<UnkScourgeAIView_ovb0115*>(self->actor->field_0x240);
    UnkActor* target = fn_7100d7c990(self->actor);
    if (!ai || !target)
        return false;
    base::math::CVector2D flat;
    UnkAttackQuery_ovb0115 query(self->actor, ai->field_0x1168);
    UnkActor* actor = self->actor;
    f32 tx = target->field_0x44, ty = target->field_0x54, tz = target->field_0x64;
    f32 ax = actor->field_0x44, ay = actor->field_0x54, az = actor->field_0x64;
    base::math::CVector3D d = {ax - tx, ay - ty, az - tz};
    fn_710007b430(&flat, &d);
    f32 length = sqrtf(flat.x * flat.x + flat.y * flat.y);
    query.distance[0] = length;
    query.distance[1] = length;
    return fn_710069a7e4(self, &query, ai->field_0x1140, check, a, b);
}

struct UnkScourgeCharClass_ovb0115 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x65c];
    f32 field_0x664;
};

struct UnkScourgeAI_b58a4;

struct UnkScourgeAI_ovb0115 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x8];
    UnkScourgeCharClass_ovb0115* char_class;  // 0x10
    u8 pad_0x18[0x14748 - 0x18];
    f32 aim[2];  // 0x14748
};

f32 fn_710069e868(UnkScourgeCharClass_ovb0115* self);
bool fn_71006a01f8(UnkScourgeAI_ovb0115* self, f32 angle, f32 range, f32 width, const UnkVec3f* pos,
                   const UnkVec3f* forward, const UnkVec3f* up, f32* distance, bool flag);

void fn_710069afac(UnkScourgeSlash_b0115* self, bool aim) {
    UnkScourgeAI_ovb0115* ai = static_cast<UnkScourgeAI_ovb0115*>(self->actor->field_0x240);
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710069ef44()))
        return;
    UnkScourgeCharClass_ovb0115* cc = ai->char_class;
    if (!cc || !fn_710009d50c(cc->vf_0x0(), fn_710069de24()))
        return;
    f32 angle = self->angle;
    f32 distance = self->distance;
    if (fn_710069a8e8(self, &angle, &distance)) {
        self->angle = angle;
        self->distance = distance;
    }
    if (aim) {
        f32 hit = 3.40282347e+38f;
        UnkActor* actor = self->actor;
        UnkVec3f pos(actor->field_0x44, actor->field_0x54, actor->field_0x64);
        const UnkMatrix44& m2 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
        UnkVec3f forward(m2.m[2], m2.m[6], m2.m[10]);
        const UnkMatrix44& m1 = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->actor->field_0x38));
        UnkVec3f up(m1.m[1], m1.m[5], m1.m[9]);
        f32 angle0 = self->angle;
        if (fn_71006a01f8(ai, angle0, fn_710069e868(cc), cc->field_0x664, &pos, &forward, &up, &hit, false)) {
            if (hit > self->distance)
                self->distance = hit;
        } else {
            self->distance = fn_710069e868(cc);
        }
    }
    bool blocked = fn_710117b348(static_cast<UnkAnimationComponent*>(self->actor->field_0x220), fn_7100c4b798());
    f32 a = self->angle;
    f32 d = self->distance;
    if (blocked) {
        ai->aim[0] = a;
        ai->aim[1] = d;
    } else {
        fn_710069f604(reinterpret_cast<UnkScourgeAI_b58a4*>(ai), a, d);
    }
}

using base::global::CStrId;

extern const base::global::CStrId g_unk_7172704968 __attribute__((visibility("hidden")));
extern UnkMatrix44 g_unk_7101a1a680;

struct UnkTileOutlines;
struct UnkTileOutline;

struct UnkScourgeShape_ovb0115 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual s32 vf_0x18();
};

struct UnkScourgeCollider_ovb0115 {
    void** vtable;
    u8 pad_0x8[0x150];
    UnkScourgeShape_ovb0115* shape;  // 0x158

    void SetTransform(const UnkMatrix44* m) {
        reinterpret_cast<void (*)(UnkScourgeCollider_ovb0115*, const UnkMatrix44*)>(vtable[0x78 / 8])(this, m);
    }
};

using UnkCreateCollider_ovb0115 = UnkScourgeCollider_ovb0115* (*)(UnkCollisionContainer*, const CStrId*,
                                                                  const CStrId*, const CStrId*, const CStrId*, u32,
                                                                  u32, u32, void*, s32, void*, s32, void*, s32,
                                                                  bool, bool, bool);

struct UnkScourgeOwner_ovb0115 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkScourgeSlashShape_ovb0115 {
    void** vtable;
    UnkScourgeOwner_ovb0115* owner;
};

// Gives the tongue's collider (created on first use as a 2D polygon collection) the outline.
void fn_710069b8a0(UnkScourgeSlashShape_ovb0115* self, UnkTileOutline* outline) {
    UnkCollisionContainer* collision = self->owner->collision;
    if (!collision)
        return;
    UnkScourgeCollider_ovb0115* collider =
        reinterpret_cast<UnkScourgeCollider_ovb0115*>(fn_71008640a4(collision, &g_unk_7172704968));
    if (!collider) {
        const CStrId* type = reinterpret_cast<const CStrId*>(
            reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0x1f98);  // "POLYCOLLECTION2D"
        UnkCreateCollider_ovb0115 create =
            (*reinterpret_cast<UnkCreateCollider_ovb0115**>(collision))[0x168 / 8];
        collider = create(collision, &g_unk_7172704968, &g_unk_7172704968 + 1, &g_unk_7101d470e0, type, 0x2000, 0,
                          1, nullptr, -1, nullptr, -1, nullptr, -1, false, false, true);
        if (!collider)
            return;
    }
    UnkScourgeShape_ovb0115* shape = collider->shape;
    s32 kind = shape->vf_0x18();
    if (shape && kind == 16) {
        fn_7100d62d40(reinterpret_cast<UnkTileOutlines*>(shape), outline);
        collider->SetTransform(&g_unk_7101a1a680);
        fn_7100d35a98(reinterpret_cast<UnkEntry*>(collider), true);
    }
}
