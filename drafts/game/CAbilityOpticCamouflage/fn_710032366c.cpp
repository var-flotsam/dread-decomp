// CAbilityOpticCamouflage (unit 432): whether the cloak can be used now: not during an animation that ignores
// it, a grab, a near EMMY, a blocked game state, a charged hyper beam, an active lava pool or the autofocus aim
// camera. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CCharClassLavaPoolComponent.h"
#include "game/CGrabComponent.h"
#include "game/CGunComponent.h"
#include "game/CHyperBeamGun.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU811.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkListedItem;
struct UnkMagmaHandle_b50;

struct UnkMagmaList_b50 {
    UnkMagmaHandle_b50** data;
    u32 count;
};

f32 fn_710054cfbc(UnkEmmyAIComponent_b46a1* self);

// The cloak's tunable: whether EMMYs nearby are ignored.
struct UnkOpticTunable_l32366c {
    u8 pad_0x0[0x3c];
    bool field_0x3c;
};
extern UnkOpticTunable_l32366c g_unk_71726affc0;

// A lava pool component: its class at vtable slot 0, a property query at slot 0x188.
struct UnkLavaPool_l32366c {
    void** vtable;

    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkLavaPool_l32366c*)>(vtable[0])(this);
    }
    bool vf_0x188(const CStrId* id, void* arg) {
        return reinterpret_cast<bool (*)(UnkLavaPool_l32366c*, const CStrId*, void*)>(vtable[0x188 / 8])(this, id,
                                                                                                         arg);
    }
};

// An entry of the EMMY and lava pool lists: the component at +0x10.
struct UnkListEntry_l32366c {
    u8 pad_0x0[0x10];
    UnkReflObject* component;
};

struct UnkSurface_l32366c {
    u8 pad_0x0[0x44];
    f32 field_0x44;
};

struct UnkFloorHolder_l32366c {
    u8 pad_0x0[0x10];
    UnkSurface_l32366c* surface;
};

struct UnkFall_l32366c {
    u8 pad_0x0[0x54];
    f32 field_0x54;
};

struct UnkOwner_l32366c {
    u8 pad_0x0[0x1c0];
    UnkFall_l32366c* field_0x1c0;
    u8 pad_0x1c8[0x220 - 0x1c8];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x260 - 0x228];
    void* grab;  // 0x260
    u8 pad_0x268[0x2a8 - 0x268];
    UnkGunComponent* gun;  // 0x2a8
};

struct UnkAbilityHolder_l32366c {
    u8 pad_0x0[0x30];
    UnkOwner_l32366c* owner;
    u8 pad_0x38[0xe8 - 0x38];
    UnkFloorHolder_l32366c* field_0xe8;
};

struct UnkAbility_l32366c {
    u8 pad_0x0[0xc0];
    UnkAbilityHolder_l32366c* holder;
};

template <class T>
static inline T* Cast(UnkReflObject* obj, const base::reflection::CClass* (*getter)()) {
    if (!obj)
        return nullptr;
    return fn_710009d50c(obj->vf_0x0(), getter()) ? reinterpret_cast<T*>(obj) : nullptr;
}

static inline UnkReflObject* Component(UnkListEntry_l32366c* entry) {
    return entry ? entry->component : nullptr;
}

static inline bool EmmyNearby() {
    base::global::CRntVector<UnkListedItem*>* emmys = fn_7100552d7c();
    for (UnkListedItem* item : *emmys) {
        UnkEmmyAIComponent_b46a1* emmy = Cast<UnkEmmyAIComponent_b46a1>(
            Component(reinterpret_cast<UnkListEntry_l32366c*>(item)), fn_710053b934);
        if (fn_71005436e8(emmy) && fn_710054cfbc(emmy) < 0.1f)
            return true;
    }
    return false;
}

static inline bool Allowed(UnkAbility_l32366c* self, UnkOwner_l32366c* owner) {
    if (UnkAnimationComponent* animation = owner->animation)
        if (fn_710117b348(animation, fn_7100c32708()))
            return false;
    if (self->holder && self->holder->field_0xe8) {
        UnkFall_l32366c* fall = owner->field_0x1c0;
        UnkSurface_l32366c* surface = self->holder->field_0xe8->surface;
        if (fall && surface && surface->field_0x44 <= 0.0f && fall->field_0x54 <= 1.0f)
            return false;
    }
    bool ok = owner->grab ? !fn_710113d83c(owner->grab) : true;
    if (ok && !g_unk_71726affc0.field_0x3c && EmmyNearby())
        return false;
    if (ok)
        ok = !fn_7100df5a4c(g_unk_7101d47310);
    return ok;
}

bool fn_710032366c(UnkAbility_l32366c* self) {
    UnkOwner_l32366c* owner = self->holder->owner;
    bool ok = Allowed(self, owner);
    if (UnkGunComponent* gun = owner->gun) {
        if (*fn_71008407fc(gun) == GameManagerIdTable().field_0x1848) {
            void* beam = Cast<UnkReflObject>(fn_71008407d8(gun), fn_71008424e8);
            ok = ok & !fn_7100845fac(beam);
        }
    }
    if (!ok)
        return false;
    UnkMagmaList_b50* pools = fn_71008fbdb4();
    for (u32 i = 0; i < pools->count; ++i) {
        UnkLavaPool_l32366c* pool = reinterpret_cast<UnkLavaPool_l32366c*>(Cast<UnkReflObject>(
            Component(reinterpret_cast<UnkListEntry_l32366c*>(pools->data[i])), fn_71008fb79c));
        if (fn_710024fbc8(pool) && pool->vf_0x188(&g_unk_7101d470e0, nullptr))
            return false;
    }
    void* cameras = fn_7100defcec(g_unk_7101d47310);
    if (!cameras)
        return true;
    UnkCameraCtrl* ctrl = fn_71010e2080(cameras);
    if (!ctrl)
        return true;
    return !fn_710009d50c(ctrl->vf_0x0(), fn_7100720464());
}
