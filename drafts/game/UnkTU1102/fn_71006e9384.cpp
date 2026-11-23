// Unit 1102 (CXParasiteAIComponent): enters the state `mode` asks for. Leaving for 2 lets go of the grabbed
// object; 3 notes the owner's position in the blackboard, 4 takes a target and locks on to the grabbed
// object's partner, 5 locks on to the partner of the object at +0x147a0, 7 drops the lock. Names and
// layouts are placeholders.
#include "base/math/CVector.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/UnkActor.h"
#include "game/UnkTU1102.h"
#include "game/UnkTU341.h"
#include "game/UnkTypedValue.h"
#include "game/UnkXParasiteAI.h"
#include "game/behaviortree/CBehavior.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

// As src/game/CXParasiteAIComponent/CXParasiteAIComponent_Flee_A3_b75.cpp defines it.
struct UnkXParasiteMode_A3_b75 {
    u32 kind;
    u32 unk_0x4;
    u64 unk_0x8;
};

void fn_71006e9fcc(UnkXParasiteAI* self, UnkActor* target);

// The blackboard properties: the owner's position (a CVector3D) and a flag (a bool).
extern u8 g_unk_717270b8b8[] __attribute__((visibility("hidden")));
extern u8 g_unk_717270b8f0[] __attribute__((visibility("hidden")));

// The weak handle of the partner of what `link` leads to, or null.
static inline UnkWeakHandle* PartnerHandle(UnkWeakHandle* link) {
    if (!link || !link->target)
        return nullptr;
    UnkXParasitePartnerLink* partner_link = Grabbed(link)->partner_link;
    if (!partner_link)
        return nullptr;
    UnkWeakTarget* partner = reinterpret_cast<UnkWeakTarget*>(partner_link->partner);
    if (!partner)
        return nullptr;
    return GetWeakHandle(partner);
}

// The owner's position.
static inline CVector3D Translation(const UnkActor* owner) {
    return {owner->field_0x44, owner->field_0x54, owner->field_0x64};
}

static inline const base::reflection::CType* TypeOf(const CVector3D*) { return fn_71000246e4(); }
static inline const base::reflection::CType* TypeOf(const bool*) { return fn_710000d228(); }

// Stores `value` under `name` in the blackboard's section of the behaviour, unless the blackboard is locked.
template <class T>
static inline void SetProperty(CBlackboard* blackboard, u8* name, const T& value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, g_unk_71726b7cb8)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = TypeOf(&value);
            v.value = const_cast<T*>(&value);
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(name), &v);
        }
    }
}

void fn_71006e9384(UnkXParasiteAI* self, const UnkXParasiteMode_A3_b75* mode) {
    if (self->state == 7)
        return;
    const u32 kind = mode->kind;
    self->field_0x1470c = -1.0f;
    self->field_0x146fc = 0;
    self->field_0x146e9 = false;
    self->state = kind;
    self->field_0x146e4 = 0x7fffffff;
    if (self->state == 0x7fffffff) {
        self->state = 1;
        return;
    } else if (self->state == 2) {
        if (self->link && self->link->target) {
            fn_7100232c68(self->link->target);
            ResetRef(&self->link);
        }
    }
    if (self->state == 3) {
        UnkActor* owner = self->owner;
        CVector3D pos = Translation(owner);
        SetProperty(&self->blackboard, g_unk_717270b8b8, pos);
    }
    if (self->state == 4) {
        fn_71006e9fcc(self, nullptr);
        if (self->link && self->link->target && Grabbed(self->link)->partner_link &&
            Grabbed(self->link)->partner_link->partner) {
            UnkWeakHandle* handle =
                GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(Grabbed(self->link)->partner_link->partner));
            AssignRef(&self->field_0x147b0, handle);
        }
        SetProperty(&self->blackboard, g_unk_717270b8f0, true);
    }
    if (self->state == 5) {
        UnkWeakHandle* other = static_cast<UnkWeakHandle*>(self->field_0x147a0);
        if (other && other->target && Grabbed(other)->partner_link && Grabbed(other)->partner_link->partner) {
            UnkWeakHandle* handle =
                GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(Grabbed(other)->partner_link->partner));
            AssignRef(&self->field_0x147b0, handle);
        }
    }
    if (self->state == 7)
        ResetRef(&self->field_0x147b0);
}
