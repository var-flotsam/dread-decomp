// CAIAttackComponent unit (tools/tu_map.py unit 2705): picks the force-damage mode for a hit on a target before
// handing it on (fn_7101136314): the owner's ModifyForceDamage track decides it, else a hit the owner's AI turns
// away or a frozen owner may still force it; on results 1 and 10 tells the AI and flags +0x2d0.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCollisionComponent.h"
#include "game/CFrozenComponent.h"
#include "game/CLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkRefl_fn_710113569c;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_710113569c>(void*);

// The record the ModifyForceDamage track resolves: the mode it forces.
struct UnkForceDamageRecord_l135434 {
    u8 pad_0x0[0x18];
    s32 mode;
};

struct UnkDamageInfo_l135434 {
    u8 pad_0x0[0x20];
    u8 field_0x20[0x8];
};

struct UnkAttackAI_l135434 {
    virtual void vf_0x0();
    bool vf_0x1e0(const void* what, const CStrId* id) {
        return reinterpret_cast<bool (*)(UnkAttackAI_l135434*, const void*, const CStrId*)>(
            (*reinterpret_cast<void***>(this))[0x1e0 / 8])(this, what, id);
    }
    void vf_0x4f0() {
        reinterpret_cast<void (*)(UnkAttackAI_l135434*)>((*reinterpret_cast<void***>(this))[0x4f0 / 8])(this);
    }
    u8 pad_0x8[0x8];
    UnkReflObject* field_0x10;
};

struct UnkAttackOwner_l135434 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkAttackAI_l135434* ai;
    u8 pad_0x248[0x2a0 - 0x248];
    UnkCollisionContainer* collision;
};

struct UnkTargetPart_l135434 {
    u8 pad_0x0[0xad0];
    u8 field_0xad0[0x8];
};

struct UnkAttackTarget_l135434 {
    u8 pad_0x0[0x1b8];
    void* life;
    u8 pad_0x1c0[0x1d8 - 0x1c0];
    UnkTargetPart_l135434* field_0x1d8;
    u8 pad_0x1e0[0x208 - 0x1e0];
    void* field_0x208;
    void* field_0x210;
    u8 pad_0x218[0x220 - 0x218];
    UnkAnimationComponent* field_0x220;
};

struct UnkAIAttackComponent_l135434 {
    u8 pad_0x0[0x30];
    UnkAttackOwner_l135434* owner;
    u8 pad_0x38[0x2d0 - 0x38];
    bool field_0x2d0;
};

s32 fn_7101136314(UnkAIAttackComponent_l135434* self, UnkAttackTarget_l135434* target, void* arg,
                  UnkDamageInfo_l135434* info, s32 mode);
bool fn_7100870280(u8* part);
bool fn_7100870258(u8* part);

// Whether force damage reaches the target: the owner has collision, and the target either has no
// +0x208 or ignores force damage through its track, in which case its part at +0x1d8 must not refuse it.
inline bool TakesForceDamage(UnkAIAttackComponent_l135434* self, UnkAttackTarget_l135434* target) {
    if (!self->owner->collision)
        return false;
    if (target->field_0x208) {
        UnkAnimationComponent* anim = target->field_0x220;
        if (!anim || !fn_710117b348(anim, fn_7100c321f8()))
            return true;
    }
    if (!target->field_0x210 || !target->field_0x1d8)
        return false;
    u8* part = target->field_0x1d8->field_0xad0;
    return !fn_7100870280(part) && !fn_7100870258(part);
}

// The force-damage mode of a hit: the one asked for, else what the owner's ModifyForceDamage track forces, else
// 1 when the target is alive and takes force damage after a hit the AI turns away, or any hit while frozen.
static s32 ForceDamageMode(UnkAIAttackComponent_l135434* self, UnkAttackTarget_l135434* target,
                           UnkDamageInfo_l135434* info, s32 mode) {
    if (mode == 2)
        return mode;
    UnkTrackQuery query = {fn_7100c3fe78(), nullptr, -1.0f};
    if (UnkAnimationComponent* anim = self->owner->field_0x220) {
        fn_710117b08c(anim, &query);
        if (query.result && TakesForceDamage(self, target))
            mode = reinterpret_cast<UnkForceDamageRecord_l135434*>(query.result)->mode;
    }
    if (mode != 0)
        return mode;
    UnkAttackOwner_l135434* owner = self->owner;
    UnkAttackAI_l135434* ai = owner->ai;
    if (!ai)
        return 0;
    UnkReflObject* ai_object = ai->field_0x10;
    if (ai_object) {
        bool is_ai = fn_710009d50c(ai_object->vf_0x0(), fn_7100408b2c());
        owner = self->owner;
        if (!is_ai)
            ai_object = nullptr;
    }
    const CStrId* id = owner->collision ? fn_7100864018(owner->collision, info) : &g_unk_7101d470e0;
    if (!ai_object || self->owner->ai->vf_0x1e0(info->field_0x20, id)) {
        UnkReflObject* frozen = base::reflection::refl_FindTyped<UnkRefl_fn_710113569c>(self->owner);
        if (!frozen || !fn_71007f9f20(frozen) || fn_71007f9fd0(reinterpret_cast<UnkFrostbiteComponent*>(frozen)))
            return 0;
    }
    return target->life && fn_7101161088(target->life) && TakesForceDamage(self, target);
}

s32 fn_7101135434(UnkAIAttackComponent_l135434* self, UnkAttackTarget_l135434* target, void* arg,
                  UnkDamageInfo_l135434* info, s32 mode) {
    s32 result = fn_7101136314(self, target, arg, info, ForceDamageMode(self, target, info, mode));
    if (result == 10 || result == 1) {
        if (fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner)) == reinterpret_cast<UnkActor*>(target)) {
            if (UnkAttackAI_l135434* ai = self->owner->ai)
                ai->vf_0x4f0();
        }
        self->field_0x2d0 = true;
    }
    return result;
}
