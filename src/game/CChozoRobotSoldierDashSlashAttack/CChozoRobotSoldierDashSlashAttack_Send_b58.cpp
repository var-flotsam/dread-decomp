// CChozoRobotSoldierDashSlashAttack's unit (tools/tu_map.py): when the owner's (+0x8) component at +0x240 is
// a CChozoRobotSoldierAIComponent that accepts the argument (fn_71004745a8) and the owner's sub-object at
// +0x220 exists: while the flag at +0x44 is set, asks the sub-object about one hash (fn_710117ae38) and,
// if it answers, sends it a zero under another (refl_EmitU32); with the flag clear it clears +0x40.
// Names are placeholders.
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

// The blend setting (as in src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp).
struct UnkTrackBlend;

bool fn_71004745a8(base::reflection::UnkReflObject* component, void* arg);

struct UnkCrsDashOwner_b58 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkCrsDash_b58 {
    u8 pad_0x0[0x8];
    UnkCrsDashOwner_b58* owner;
    u8 pad_0x10[0x30];
    u32 field_0x40;
    bool field_0x44;
};

void fn_7100468de0(UnkCrsDash_b58* self, void* arg) {
    base::reflection::UnkReflObject* component = self->owner->field_0x240;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_710046ea2c()) || !fn_71004745a8(component, arg))
        return;
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target)
        return;
    if (!self->field_0x44) {
        self->field_0x40 = 0;
        return;
    }
    if (fn_710117ae38(target, 0xfb7d4bc21bc9379a, false)) {
        base::reflection::refl_EmitU32(self->owner->field_0x220, 0x79e25f7bf69fa4f7, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769770);
    }
}
