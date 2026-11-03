// CProjectileMovement unit (tools/tu_map.py unit 1623): whether, with the linked object's flag
// (+0x119) set, what `hit` refers to is a collision component whose actor has one of two
// abilities (looked up by type id as in src/game_aca57c/fn_7100df5308.cpp) in state 6. Names and
// layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"

using base::reflection::CClass;

struct UnkAbility;
struct UnkAbilitySet;
UnkAbility* fn_71003012f8(UnkAbilitySet* set, u64 id);
u64 fn_7100327d14();
u64 fn_710033b6a4();

struct UnkHitAbilityState_b46a3 {
    u8 unk_0x0[0x10];
    s32 state;
};

// An ability as the state test reads it: its state at 0xb4, or while that is 7 the state of its
// object at 0x20 (7 when there is none).
struct UnkHitAbility_b46a3 {
    u8 unk_0x0[0x20];
    UnkHitAbilityState_b46a3* sub;
    u8 unk_0x28[0x8c];
    s32 state;

    s32 GetState() const {
        if (state == 7)
            return sub ? sub->state : 7;
        return state;
    }
};

struct UnkHitActor_b46a3 {
    u8 unk_0x0[0x2e0];
    UnkAbilitySet* abilities;  // 0x2e0
};

struct UnkHitComponent_b46a3 {
    virtual const CClass* vf_0x0();
    u8 unk_0x8[0x28];
    UnkHitActor_b46a3* actor;  // 0x30
};

struct UnkHitTarget_b46a3 {
    u8 unk_0x0[0x10];
    UnkHitComponent_b46a3* component;  // 0x10
};

struct UnkProjectileLink5_b46a3 {
    u8 pad_0x0[0x119];
    bool check_abilities;  // 0x119
};

struct UnkProjectileMovement5_b46a3 {
    u8 pad_0x0[0x5c8];
    UnkProjectileLink5_b46a3* link;  // 0x5c8
};

static inline bool InState6(UnkAbility* a) {
    return a && reinterpret_cast<UnkHitAbility_b46a3*>(a)->GetState() == 6;
}

bool fn_7100ac3ff4(UnkProjectileMovement5_b46a3* self, UnkWeakHandle** hit) {
    if (!self->link->check_abilities)
        return false;
    UnkWeakHandle* handle = *hit;
    if (!handle || !handle->target)
        return false;
    UnkHitComponent_b46a3* component = reinterpret_cast<UnkHitTarget_b46a3*>(handle->target)->component;
    if (!component)
        return false;
    if (!fn_710009d50c(component->vf_0x0(), fn_710085fc44()))
        return false;
    UnkHitActor_b46a3* actor = component->actor;
    if (!actor || !actor->abilities)
        return false;
    UnkAbilitySet* abilities = actor->abilities;
    UnkAbility* a = fn_71003012f8(abilities, fn_710033b6a4());
    UnkAbility* b = fn_71003012f8(abilities, fn_7100327d14());
    if (InState6(a))
        return true;
    return InState6(b);
}
