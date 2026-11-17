// CBombGun's unit (tools/tu_map.py): drops a bomb. It builds the bomb actor named by the bomb tweak track (or the id
// table's id at +0x980 when the owner's animation has no such track), sets up its movement from the gun's tunables,
// aims it at the owner's current target, tells the boss AI about it and starts its timeline.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTimelineComponent.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkBombAimMovement_ovb0078;
struct UnkActor;

extern const u64 g_unk_710164ac50;

void fn_7100aac004(UnkBombAimMovement_ovb0078* self, UnkWeakTarget* target);

// A counted reference to a weak handle.
struct UnkHandleRef_l830f78 {
    UnkWeakHandle* ptr;
    UnkHandleRef_l830f78() : ptr(nullptr) {}
    UnkHandleRef_l830f78(const UnkHandleRef_l830f78& other) : ptr(other.ptr) {
        if (ptr)
            ++ptr->ref_count;
    }
    ~UnkHandleRef_l830f78() {
        if (ptr && --ptr->ref_count == 0)
            ptr->vf_0x10();
    }
};

struct UnkTargets_l830f78 {
    UnkHandleRef_l830f78 target;
    UnkHandleRef_l830f78 other;
};

struct UnkTargeting_l830f78 {
    u8 pad_0x0[0x128];
    UnkTargets_l830f78 targets;  // 0x128
};

struct UnkBombGunOwner_l830f78 {
    u8 pad_0x0[0x1f0];
    UnkTargeting_l830f78* targeting;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkBombGun_l830f78 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkBombGunOwner_l830f78* owner;  // 0x18

    template <class R>
    R Call(u32 offset) {
        return reinterpret_cast<R (*)(UnkBombGun_l830f78*)>(vtable[offset / 8])(this);
    }
};

struct UnkBombParams_l830f78 {
    u64 value;
    u32 count;
    UnkBombParams_l830f78() : value(0), count(0) {}
};

struct UnkBombMovement_l830f78 {
    void** vtable;
    u8 pad_0x8[0x348 - 0x8];
    u8 request[0x3fc - 0x348];  // 0x348
    s32 field_0x3fc;
    u8 pad_0x400[0x408 - 0x400];
    f32 field_0x408;
    f32 field_0x40c;

    void Prepare(UnkBombGunOwner_l830f78* owner, UnkBombParams_l830f78* a, UnkBombParams_l830f78* b) {
        reinterpret_cast<void (*)(UnkBombMovement_l830f78*, UnkBombGunOwner_l830f78*, UnkBombParams_l830f78*,
                                  UnkBombParams_l830f78*)>(vtable[0x2c8 / 8])(this, owner, a, b);
    }
    void SetOwner(UnkBombGunOwner_l830f78* owner) {
        reinterpret_cast<void (*)(UnkBombMovement_l830f78*, UnkBombGunOwner_l830f78*)>(vtable[0x290 / 8])(this,
                                                                                                         owner);
    }
};

struct UnkBombActor_l830f78 {
    void** vtable;

    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBombActor_l830f78*)>(vtable[0])(this);
    }
    void Apply(u32 offset, UnkBombParams_l830f78* params, bool flag) {
        reinterpret_cast<void (*)(UnkBombActor_l830f78*, UnkBombParams_l830f78*, bool)>(vtable[offset / 8])(
            this, params, flag);
    }
};

struct UnkBossAI_l830f78 {
    void** vtable;

    const base::reflection::CClass* GetClass() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkBossAI_l830f78*)>(vtable[0])(this);
    }
    void OnBomb(UnkBombActor_l830f78* bomb) {
        reinterpret_cast<void (*)(UnkBossAI_l830f78*, UnkBombActor_l830f78*)>(vtable[0x7c0 / 8])(this, bomb);
    }
};

struct UnkBossActor_l830f78 {
    u8 pad_0x0[0x240];
    UnkBossAI_l830f78* ai;  // 0x240
};

struct UnkBossHolder_l830f78 {
    u8 pad_0x0[0x10];
    UnkBossActor_l830f78* actor;  // 0x10
};

struct UnkBehaviorManager_l830f78 {
    u8 pad_0x0[0x100];
    UnkBossHolder_l830f78* holder;  // 0x100
};

struct UnkTargetObject_l830f78 {
    void** vtable;

    u32 Flags() { return reinterpret_cast<u32 (*)(UnkTargetObject_l830f78*)>(vtable[0x58 / 8])(this); }
};

struct alignas(8) UnkBombDefType_l830f78 {
    u32 id;
};

UnkBombMovement_l830f78* fn_7100831358(UnkBombActor_l830f78* bomb);
void fn_7100ac7db4(UnkBombMovement_l830f78* movement, const char* name, bool flag);

// The id's text, counting the read.
static inline const char* PathText(CStrId* id) {
    ++id->data->reads;
    return reinterpret_cast<const char*>(&id->data->String());
}

// Builds the actor the id names.
static inline UnkBombActor_l830f78* Spawn(UnkActorDefFactory* factory, CStrId* id) {
    UnkBombDefType_l830f78 type;
    u32 type_id = g_unk_7172a0ba48;
    const char* text = PathText(id);
    type.id = type_id;
    return reinterpret_cast<UnkBombActor_l830f78*>(
        fn_710119438c(factory, text, id, reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
}

static inline UnkTargets_l830f78 TargetsOf(UnkBombGunOwner_l830f78* owner) {
    if (UnkTargeting_l830f78* targeting = owner->targeting)
        return targeting->targets;
    return UnkTargets_l830f78();
}

static inline UnkBehaviorManager_l830f78* BehaviorManager() {
    char* manager = g_unk_71726b2398;
    if (!manager) {
        manager = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(manager);
        g_unk_71726b2398 = manager;
    }
    return reinterpret_cast<UnkBehaviorManager_l830f78*>(manager);
}

bool fn_7100830f78(UnkBombGun_l830f78* self) {
    if (!self->owner || !self->Call<bool>(0x48))
        return false;
    UnkTrackQuery query;
    query.cls = fn_7100d2d1d8();
    query.result = nullptr;
    query.value = -1.0f;
    if (UnkAnimationComponent* animation = self->owner->animation)
        fn_710117b08c(animation, &query);
    UnkActorDefFactory* factory = g_unk_71726a0220;
    UnkBombActor_l830f78* bomb = query.result ? Spawn(factory, &query.result->sources)
                                              : Spawn(factory, &GameManagerIdTable().field_0x980);
    if (!bomb || !fn_710009d50c(bomb->GetClass(), fn_7100d70c20()))
        return false;
    if (UnkBombMovement_l830f78* movement = fn_7100831358(bomb)) {
        UnkBombParams_l830f78 first;
        UnkBombParams_l830f78 second;
        movement->Prepare(self->owner, &first, &second);
        bomb->Apply(0xd0, &first, true);
        bomb->Apply(0xd8, &second, true);
        fn_7100ac7db4(movement, "Bomb", false);
        movement->SetOwner(self->owner);
        fn_71008c1538(reinterpret_cast<UnkRequest*>(movement->request), self->Call<f32>(0x180));
        movement->field_0x3fc = self->Call<s32>(0x198);
        movement->field_0x408 = self->Call<f32>(0x1a0);
        movement->field_0x40c = self->Call<f32>(0x1a8);
        UnkTargets_l830f78 targets = TargetsOf(self->owner);
        if (UnkWeakHandle* handle = targets.target.ptr) {
            UnkWeakTarget* target = handle->target;
            if (target && (reinterpret_cast<UnkTargetObject_l830f78*>(target)->Flags() & 0x8000))
                fn_7100aac004(reinterpret_cast<UnkBombAimMovement_ovb0078*>(movement), target);
        }
        if (UnkBossHolder_l830f78* holder = BehaviorManager()->holder) {
            if (UnkBossActor_l830f78* actor = holder->actor) {
                if (UnkBossAI_l830f78* ai = actor->ai) {
                    if (fn_710009d50c(ai->GetClass(), fn_71003ab524()))
                        ai->OnBomb(bomb);
                }
            }
        }
        fn_7100d7c828(reinterpret_cast<UnkActor*>(bomb), &g_unk_710164ac50, false);
    }
    return true;
}
