// CChozoWarriorXGlaiveSpinAttack's unit (the name from reflection): the attack can start when the base test
// fn_7100494c9c passes and the owner (+0x8) still has life (+0x1d0, +0x54 > 0); when the owner's AI
// component (+0x240) is a CChozoWarriorXAIComponent, its actor's (+0x30) y must also be at most 50 above the
// height at +0x149a4. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkActor.h"
#include "types.h"

using base::reflection::CClass;

struct UnkGlaiveLife_A2_b70 {
    u8 pad_0x0[0x54];
    f32 life;
};

struct UnkGlaiveAI_A2_b70 {
    virtual const CClass* vf_0x0();
    u8 pad_0x8[0x28];
    UnkActor* actor;  // 0x30
    u8 pad_0x38[0x149a4 - 0x38];
    f32 height;  // 0x149a4
};

struct UnkGlaiveOwner_A2_b70 {
    u8 pad_0x0[0x1d0];
    UnkGlaiveLife_A2_b70* life;  // 0x1d0
    u8 pad_0x1d8[0x68];
    UnkGlaiveAI_A2_b70* ai;  // 0x240
};

struct UnkGlaiveSpinAttack_A2_b70 {
    u8 pad_0x0[0x8];
    UnkGlaiveOwner_A2_b70* owner;
};

bool fn_7100494c9c(UnkGlaiveSpinAttack_A2_b70* self);

bool fn_7100481914(UnkGlaiveSpinAttack_A2_b70* self) {
    if (!fn_7100494c9c(self))
        return false;
    UnkGlaiveOwner_A2_b70* owner = self->owner;
    if (owner->life && !(owner->life->life > 0.0f))
        return false;
    UnkGlaiveAI_A2_b70* ai = owner->ai;
    if (!ai || !fn_710009d50c(ai->vf_0x0(), fn_710048db44()))
        return true;
    return !(ai->actor->field_0x54 > ai->height + 50.0f);
}
