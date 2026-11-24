// CCharClassSteamJetComponent unit: reads the jet's enabled flag from the current scenario's blackboard
// section; when it is set and the jet's actor is linked, starts the actor's effects, enables the other linked
// actor and turns its collision on; when it is clear, notifies the jet's actor. Names and layouts are
// placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CBlackboard.h"
#include "game/CCharClassShootActivatorComponent.h"
#include "game/CCharClassSteamJetComponent.h"
#include "game/CNavMeshItemComponent.h"
#include "game/CScenario.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU341.h"
#include "game/UnkTU372.h"
#include "game/UnkTU437.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

#include "../../../src/game_s2/UnkSpitSend.h"

using base::global::CStrId;

void* fn_710035379c(void* owner);
void fn_71007e66bc(UnkWaterTarget* target);
extern u64 g_unk_710164af18;

struct UnkSteamJetCollider_l899cb8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual u32 vf_0x58();
    virtual void vf_0x60(u32 flags);
};

// The collider named `name` in `colliders`, or null.
static inline UnkSteamJetCollider_l899cb8* ColliderNamed(UnkCollisionContainer* colliders, const CStrId& name) {
    return reinterpret_cast<UnkSteamJetCollider_l899cb8*>(fn_71008640a4(colliders, &name));
}

struct UnkSteamJetWeakSpot_l899cb8 {
    u8 pad_0x0[0x3d];
    bool field_0x3d;
};

struct UnkSteamJetOwner_l899cb8 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* colliders;  // 0x2a0
};

struct UnkSteamJetAnimation_l899cb8 {
    u8 pad_0x0[0x2c8];
    s32 field_0x2c8;
};

struct UnkSteamJetActor_l899cb8 {
    u8 pad_0x0[0x220];
    UnkSteamJetAnimation_l899cb8* animation;  // 0x220
};

struct UnkSteamJetEndTarget_l899cb8 : base::UnkRefCounted {
    UnkSteamJetActor_l899cb8* actor;  // 0x10
};

// A link to an actor whose handle is fetched from `source` on first use.
struct UnkSteamJetEnd_l899cb8 {
    u8 pad_0x0[8];
    u8 source[0x28];  // 0x8
    s32 state;        // 0x30
    u8 pad_0x34[4];
    base::UnkRefHandle target;  // 0x38

    UnkSteamJetEndTarget_l899cb8* Target() {
        if (state == 1) {
            {
                base::UnkRefPtrMember ref = fn_7100353be8(source);
                fn_710031d208(&target, reinterpret_cast<const base::UnkRefHandle*>(&ref));
            }
            state = 2;
        }
        return static_cast<UnkSteamJetEndTarget_l899cb8*>(target.ptr);
    }

    // The linked actor, or null.
    UnkSteamJetActor_l899cb8* Actor() {
        UnkSteamJetEndTarget_l899cb8* t = Target();
        return t ? t->actor : nullptr;
    }
};

struct UnkSteamJetValue_l899cb8 {
    CStrId id;
    u64 field_0x8;
};

struct UnkSteamJet_l899cb8 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkSteamJetOwner_l899cb8* owner;  // 0x30
    u8 pad_0x38[0xf4 - 0x38];
    bool enabled;  // 0xf4
    u8 pad_0xf5[0xf8 - 0xf5];
    UnkSteamJetEnd_l899cb8 jet;    // 0xf8
    UnkSteamJetEnd_l899cb8 other;  // 0x138
    u8 pad_0x178[0x1c8 - 0x178];
    UnkSteamJetValue_l899cb8* values;  // 0x1c8
    u64* keys;                         // 0x1d0
    u32 count;                         // 0x1d8

    // The value stored under `property`'s name (one past the end when there is none).
    UnkSteamJetValue_l899cb8* Find(const UnkProperty& property) {
        u64 index = 0;
        u32 n = count;
        if (n) {
            u64* ids = keys;
            u64 key = (u64)property.name.data;
            for (; index != n; ++index) {
                if (ids[index] == key)
                    break;
            }
        }
        return &values[index];
    }

    void vf_0x170(f32 value) {
        reinterpret_cast<void (*)(UnkSteamJet_l899cb8*, f32)>(vtable[0x170 / 8])(this, value);
    }
};
static_assert(__builtin_offsetof(UnkSteamJet_l899cb8, other) == 0x138, "UnkSteamJet layout");
static_assert(__builtin_offsetof(UnkSteamJet_l899cb8, count) == 0x1d8, "UnkSteamJet layout");

void fn_7100899cb8(UnkSteamJet_l899cb8* self, CBlackboard* blackboard) {
    CBlackboard::CSection* section = fn_71010472e0(blackboard, fn_71011a2fbc(g_unk_7101d47310->field_0x608));
    if (!section)
        return;
    // The request sent to the jet's animation; the original gives both their own stack slots.
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg type_id;
    if (fn_710006d11c(section, &self->Find(g_unk_717272c420)->id, &self->enabled)) {
        if (self->jet.Target() && self->jet.Target()->actor && self->enabled) {
            fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164af18, false);
            self->vf_0x170(1.0f);
            if (UnkSteamJetAnimation_l899cb8* animation = self->jet.Target()->actor->animation) {
                animation->field_0x2c8 = 1;
                mode.id = 0;
                type_id.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
                CAnimationComponent_SendActionToTrack(animation, 0xf4f16aa254fe9ff2, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_717276974c, &type_id);
            }
            if (self->other.Target() && self->other.Target()->actor)
                fn_7100232864(reinterpret_cast<UnkEntityState*>(self->other.Target()->actor), false);
            if (UnkWaterTarget* water = fn_710089a1b0(self->jet.Actor()))
                fn_71007e66bc(water);
            if (auto* weak_spot = static_cast<UnkSteamJetWeakSpot_l899cb8*>(fn_710035379c(self->owner)))
                weak_spot->field_0x3d = false;
            if (UnkCollisionContainer* colliders = self->owner->colliders) {
                if (UnkSteamJetCollider_l899cb8* collider = ColliderNamed(colliders, CStrId("collision", true)))
                    collider->vf_0x60(collider->vf_0x58() | 0x1000);
            }
        }
    }
    if (!self->enabled) {
        if (void* target = fn_71002b3b84(reinterpret_cast<UnkWeakTarget*>(self->jet.Actor())))
            fn_71009c0f1c(target, 0x13ae275a1bcd6558);
    }
}
