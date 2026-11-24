// CGoliathXBurstProjectionBombMovement's unit (tools/tu_map.py unit 840): starts the movement: after the base
// start, takes four values from the target's CGoliathXAIComponent, hands back the old effect, shows the bomb or
// tracker effect at the owner's position, records that position and, when tracking, picks the tracker damage id
// before refreshing the request from it. Names and layouts are placeholders.
#include "../../game_5821e4/UnkStaticInit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/UnkOneVector3.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CFXComponent.h"
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/CWeaponMovement.h"
#include "game/UnkActor.h"
#include "game/UnkFxObjectVtable_ovb0041.h"
#include "game/UnkFxObject_ovb0041.h"
#include "game/UnkModelManager.h"
#include "game/UnkStatics_710058c890.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2739.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::math::CVector3D;
using base::reflection::CClass;
using namespace unk_static_init;

// The effect object the model manager hands out, as its virtual calls see it (as in
// src/game/CMeleeComponent/CMeleeComponent_Checks_ovb0041.cpp).
struct UnkFxObject_ovb0041;

// The target's CGoliathXAIComponent: four values the bomb takes over.
struct UnkGoliathXAI_l58a6e4 {
    void** vtable;
    u8 pad_0x8[0x14c68];
    f32 field_0x14c70;
    f32 field_0x14c74;
    f32 field_0x14c78;
    f32 field_0x14c7c;

    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkGoliathXAI_l58a6e4*)>(vtable[0])(this); }
};

struct UnkGoliathXActor_l58a6e4 {
    u8 pad_0x0[0x240];
    UnkGoliathXAI_l58a6e4* ai;  // 0x240
};

struct UnkBombTarget_l58a6e4 {
    u8 pad_0x0[0x10];
    UnkGoliathXActor_l58a6e4* actor;  // 0x10
};

struct UnkGoliathBombMovement_l58a6e4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x44];
    f32 field_0x7c;
    u8 pad_0x80[0x1b0];
    UnkBombTarget_l58a6e4* target;  // 0x230
    u8 pad_0x238[0x110];
    u8 request[0xb8];  // 0x348
    CStrId damage_id;  // 0x400
    u8 pad_0x408[0x108];
    UnkFxObject_ovb0041* fx;  // 0x510
    u8 pad_0x518[0x878];
    UnkVec3f start;  // 0xd90
    u8 pad_0xd9c[0x10];
    f32 field_0xdac;
    f32 field_0xdb0;
    f32 field_0xdb4;
    f32 field_0xdb8;
};
static_assert(__builtin_offsetof(UnkGoliathBombMovement_l58a6e4, field_0xdac) == 0xdac,
              "UnkGoliathBombMovement_l58a6e4 layout");

// A vector copied component by component.
static inline CVector3D Copy(const CVector3D& v) {
    CVector3D copy;
    copy.x = v.x;
    copy.y = v.y;
    copy.z = v.z;
    return copy;
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

void CGoliathXBurstProjectionBombMovement_SpawnBombOrTrackerFxAtOwner(UnkGoliathBombMovement_l58a6e4* self) {
    fn_7100ac77c0();
    if (UnkBombTarget_l58a6e4* target = self->target) {
        if (UnkGoliathXActor_l58a6e4* actor = target->actor) {
            UnkGoliathXAI_l58a6e4* ai = actor->ai;
            if (ai && fn_710009d50c(ai->vf_0x0(), fn_7100587ee4())) {
                self->field_0xdac = ai->field_0x14c70;
                self->field_0xdb0 = ai->field_0x14c74;
                self->field_0xdb4 = ai->field_0x14c78;
                self->field_0xdb8 = ai->field_0x14c7c;
            }
        }
    }

    if (self->fx) {
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(self->fx), false);
        fn_71011a91c4(ModelManager(), self->fx, 0);
    }
    self->fx = nullptr;

    if (self->owner->field_0x280) {
        const UnkFilePathStrIdBase& source = self->field_0x7c > 0.0f ? g_unk_71726e5cb8 : g_unk_71726e5cb0;
        CFilePathStrId path(reinterpret_cast<const CFilePathStrId&>(source));
        UnkFxObject_ovb0041* fx = fn_71003b7db0(ModelManager(), &path, false, true);
        self->fx = fx;
        if (fx) {
            UnkActor* owner = self->owner;
            UnkVec3f position(owner->field_0x44, owner->field_0x54, owner->field_0x64);
            fn_7100146d84(fx, &position);
            const UnkVec3f* zero = reinterpret_cast<const UnkVec3f*>(&g_unk_7101647790);
            fn_7100146d98(self->fx, zero);
            self->fx->vtable->set_scale(self->fx, reinterpret_cast<const UnkVec3f*>(&g_unk_71016477a8));
            fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(self->fx), true);
            self->fx->vtable->set_loop(self->fx, false);
            self->fx->vtable->set_speed(self->fx, 1.0f);
            fn_710115217c(self->owner->field_0x280, self->fx, 0, 4, &CStrId::Empty(), Copy(g_unk_7101647790),
                          Copy(g_unk_7101647790), nullptr);
        }
    }

    UnkActor* owner = self->owner;
    self->start = UnkVec3f(owner->field_0x44, owner->field_0x54, owner->field_0x64);
    if (self->field_0x7c > 0.0f)
        self->damage_id = static_cast<const CStrId&>(g_unk_71726e5cc0.field_0x20);
    fn_71008c1538(reinterpret_cast<UnkRequest*>(self->request), fn_7100789f5c(&self->damage_id));
}
