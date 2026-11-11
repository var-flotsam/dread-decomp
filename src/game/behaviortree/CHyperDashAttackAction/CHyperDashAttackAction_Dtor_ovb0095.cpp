// behaviortree::CHyperDashAttackAction's unit (tools/tu_map.py unit 662): the destructor (listener, string id,
// string, then the base's fn_7100393ea0) and the lookup of the CChozoCommanderGroundAttack the action works
// with. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntString.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU2702.h"
#include "game/behaviortree/CBaseAction.h"
#include "game/behaviortree/CHyperDashAttackAction.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

// A listener holding a weak handle to what it listens to; it unregisters on destruction.
struct UnkHyperDashListener_ovb0095 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkWeakHandle* link;  // 0x18

    ~UnkHyperDashListener_ovb0095() {
        vtable = &g_unk_7101a22d48[2];
        if (UnkWeakHandle* l = link) {
            if (UnkWeakTarget* target = l->target) {
                fn_710117b3f0(target, this);
                l = link;
            }
            if (l && --l->ref_count == 0)
                l->vf_0x10();
        }
    }
};

struct UnkHyperDashAction_ovb0095 {
    void** vtable;
    u8 pad_0x8[0x28];
    base::global::CRntString field_0x30;
    base::UnkStrIdMember field_0x60;
    u8 pad_0x68[0x20];
    UnkHyperDashListener_ovb0095 listener;  // 0x88
};
static_assert(__builtin_offsetof(UnkHyperDashAction_ovb0095, listener) == 0x88, "layout");

// behaviortree::CHyperDashAttackAction's destructor: its listener (+0x88), the string id at +0x60 and
// the string at +0x30, then the base's destructor fn_7100393ea0.
void fn_710044b4cc(UnkHyperDashAction_ovb0095* self) {
    self->vtable = &g_unk_7101a36838[2];
    self->listener.~UnkHyperDashListener_ovb0095();
    self->field_0x60.~UnkStrIdMemberOf();
    fn_710007ecd4(&self->field_0x30);
    fn_7100393ea0(self);
}

using base::reflection::UnkReflObject;

struct UnkScorpiusAI_b69a4;

struct UnkHyperDashActor_ovb0095 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkHyperDashAI_ovb0095 {
    u8 pad_0x0[0x1148];
    UnkReflObject* attack;  // 0x1148
};

struct UnkHyperDashNode_ovb0095 {
    u8 pad_0x0[0x18];
    UnkHyperDashActor_ovb0095* actor;  // 0x18
    u8 pad_0x20[0x10];
    const char* attack_name;  // 0x30
};

static inline UnkReflObject* AsGroundAttack(UnkReflObject* attack) {
    if (!attack)
        return nullptr;
    return fn_710009d50c(attack->vf_0x0(), fn_710042b82c()) ? attack : nullptr;
}

// The CChozoCommanderGroundAttack the action works with: the owner's CChozoCommanderAIComponent attack
// named at +0x30 (fn_7101109df4), or its current one (+0x1148) when no name is set.
UnkReflObject* fn_7100449868(UnkHyperDashNode_ovb0095* self) {
    UnkReflObject* ai = nullptr;
    if (self->actor) {
        if (UnkReflObject* component = self->actor->ai)
            ai = fn_710009d50c(component->vf_0x0(), fn_710044e708()) ? component : nullptr;
    }
    if (self->attack_name && self->attack_name[0])
        return AsGroundAttack(static_cast<UnkReflObject*>(
            fn_7101109df4(reinterpret_cast<UnkScorpiusAI_b69a4*>(ai), &self->attack_name)));
    if (!ai)
        return nullptr;
    return AsGroundAttack(reinterpret_cast<UnkHyperDashAI_ovb0095*>(ai)->attack);
}
