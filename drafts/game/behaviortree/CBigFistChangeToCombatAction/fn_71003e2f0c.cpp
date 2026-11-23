// behaviortree::CBigFistChangeToCombatAction unit: the action's update. Unless the event hook already fired
// (+0x60) or the owner ignores combat, it stops three animation layers, then, in a combat, flee, patrol or
// graze track, picks the turn animation for where the target stands and asks the AI (+0x30) to play it;
// once it plays, hooks fn_71003e35a0 to the owner's character class. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkVec3fOps.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

struct UnkBigFistChangeToCombatAction;
struct UnkBigFistEvent;
bool fn_71003e35a0(UnkBigFistChangeToCombatAction* self, const UnkBigFistEvent* ev);

// Plays an animation on the AI by id and kind; false when it could not start.
bool fn_7101110964(UnkReflObject* ai, CStrId* anim, const u64* kind, const u32* flags, f32 blend, f32 start);

// The turn animations: forward-left/right and the like, by entry (0 to 5).
extern CStrId g_unk_71726c09a8[6] __attribute__((visibility("hidden")));

struct UnkBigFistCombatLink_l3e2f0c {
    u8 pad_0x0[0x8];
    void* context;
    bool (*callback)(UnkBigFistChangeToCombatAction* self, const UnkBigFistEvent* ev);
};

struct UnkBigFistChangeToCombatAction_l3e2f0c {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x10];
    UnkReflObject* ai;                     // 0x30
    UnkAnimationComponent* animation;      // 0x38
    UnkBigFistCombatLink_l3e2f0c link;     // 0x40
    UnkWeakHandle* handle;                 // 0x58
    bool triggered;                        // 0x60
};

static inline const UnkMatrix44& Transform_l3e2f0c(UnkActor* actor) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
}

// The transform's third column: where the actor faces.
static inline UnkVec3f Facing_l3e2f0c(const UnkMatrix44& m) {
    return UnkVec3f(m.m[2], m.m[6], m.m[10]);
}

static inline UnkVec3f Pos_l3e2f0c(const UnkActor* actor) {
    return UnkVec3f(actor->field_0x44, actor->field_0x54, actor->field_0x64);
}

template <const CClass* (*GetClass)()>
static inline UnkReflObject* AiAs_l3e2f0c(UnkActor* owner) {
    UnkReflObject* ai = static_cast<UnkReflObject*>(owner->field_0x240);
    if (!ai)
        return nullptr;
    return fn_710009d50c(ai->vf_0x0(), GetClass()) ? ai : nullptr;
}

u32 fn_71003e2f0c(UnkBigFistChangeToCombatAction_l3e2f0c* self) {
    if (self->triggered)
        return 5;
    if (fn_710117b348(self->animation, fn_7100c2a3d8()) || fn_71010cab0c(self->owner->field_0x218))
        return 2;

    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(self->animation), 0xc, false, false, -1.0f);
    fn_7101178e08(reinterpret_cast<UnkActorTarget*>(self->animation), 6, false, false, -1.0f);
    fn_710117662c(reinterpret_cast<UnkActorTarget*>(self->animation), 0xb, 0, false, false, -1.0f);

    if (fn_710117b348(self->animation, fn_7100c1181c()))
        return 5;
    if (!fn_710117b348(self->animation, fn_7100c19f48()) && !fn_710117b348(self->animation, fn_7100c4d378()) &&
        !fn_710117b348(self->animation, fn_7100c219b8()))
        return 5;

    CStrId anim(g_unk_710162e2ed, true);
    UnkActor* target = fn_7100d7c990(self->owner);
    UnkVec3f offset = Pos_l3e2f0c(target) - Pos_l3e2f0c(self->owner);
    f32 ahead = Dot(offset, Facing_l3e2f0c(Transform_l3e2f0c(self->owner)));

    if (fn_710117b348(self->animation, fn_7100c219b8())) {
        anim = ahead > 0.0f ? g_unk_71726c09a8[5] : g_unk_71726c09a8[4];
    } else {
        UnkReflObject* goliath = AiAs_l3e2f0c<fn_71005908ac>(self->owner);
        UnkReflObject* goliath_x = AiAs_l3e2f0c<fn_7100587ee4>(self->owner);
        if (goliath || goliath_x)
            anim = ahead > 0.0f ? g_unk_71726c09a8[1] : g_unk_71726c09a8[0];
        else if (ahead > 0.0f)
            anim = Dot(Facing_l3e2f0c(Transform_l3e2f0c(self->owner)), UnkVec3f(1.0f, 0.0f, 0.0f)) > 0.0f ? g_unk_71726c09a8[2]
                                                                                           : g_unk_71726c09a8[3];
        else
            anim = g_unk_71726c09a8[0];
    }

    UnkReflObject* ai = self->ai;
    u64 kind = base::reflection::refl_GetStrIdTable<0x7100031894>().ids[0x10c0 / 8].data->Key();
    u32 flags = 0;
    if (!fn_7101110964(ai, &anim, &kind, &flags, -1.0f, 0.0f))
        return 6;

    UnkBigFistCombatLink_l3e2f0c* link = &self->link;
    link->context = self;
    link->callback = fn_71003e35a0;
    UnkWeakTarget* track = static_cast<UnkWeakTarget*>(self->owner->field_0x220);
    if (UnkWeakHandle* handle = self->handle) {
        if (UnkWeakTarget* old = handle->target)
            fn_710117b3f0(old, link);
        ResetRef(&self->handle);
    }
    if (track) {
        fn_7100316018(track, &self->handle);
        fn_710117b3e8(self->handle ? self->handle->target : nullptr, link);
    }
    fn_710111094c(self->ai, 1);
    return 2;
}
