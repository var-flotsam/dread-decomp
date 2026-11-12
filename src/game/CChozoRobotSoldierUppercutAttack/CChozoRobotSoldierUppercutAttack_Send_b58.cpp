// CChozoRobotSoldierUppercutAttack's unit (tools/tu_map.py): when the owner's (+0x8) component at +0x240 is
// a CChozoRobotSoldierAIComponent that accepts the argument (fn_71004745a8), once (flag at +0x40): unless
// the owner's sub-object at +0x220 knows the hash the member at +0x28 gives, sends it a zero under a fixed
// hash (refl_EmitU32) and sets the flag (fn_710046aa10's body). Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/CChozoRobotSoldierAIComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkCrsUpOwner_b58 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkCrsUpHashed_b58 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkCrsUpProvider_b58 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual UnkCrsUpHashed_b58** vf_0x18(UnkCrsUpOwner_b58* owner);
};

struct UnkCrsUp_b58 {
    u8 pad_0x0[0x8];
    UnkCrsUpOwner_b58* owner;
    u8 pad_0x10[0x18];
    UnkCrsUpProvider_b58* field_0x28;
    u8 pad_0x30[0x10];
    bool field_0x40;
};

void fn_710046ab68(UnkCrsUp_b58* self, void* arg) {
    base::reflection::UnkReflObject* component = self->owner->field_0x240;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710046ea2c()) || !fn_71004745a8(component, arg))
        return;
    if (self->field_0x40)
        return;
    UnkCrsUpOwner_b58* owner = self->owner;
    UnkAnimationComponent* target = owner->field_0x220;
    if (!target)
        return;
    if (fn_710117ae38(target, (*self->field_0x28->vf_0x18(owner))->hash, false))
        return;
    base::reflection::refl_EmitU32(self->owner->field_0x220, 0xde6f5a5d963ca497, 0xf4061ff25a2163d6, 0,
                                   &g_unk_710164b958, &g_unk_7172769740);
    self->field_0x40 = true;
}
