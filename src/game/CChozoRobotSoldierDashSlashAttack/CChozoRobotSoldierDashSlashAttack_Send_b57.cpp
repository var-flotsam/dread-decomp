// CChozoRobotSoldierDashSlashAttack's unit (tools/tu_map.py): while the flag at +0x44 is set, asks the
// owner's (+0x8) sub-object at +0x220 about one hash (fn_710117ae38) and, if it answers, sends it a zero
// under another (refl_EmitU32, base/reflection/UnkReflStatics.h); with the flag clear it clears +0x40.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

// The blend setting (as in src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp).
struct UnkTrackBlend;
extern UnkTrackBlend g_unk_7172769770;

struct UnkCrsDashOwner_b57 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* field_0x220;
};

struct UnkCrsDash_b57 {
    u8 pad_0x0[0x8];
    UnkCrsDashOwner_b57* owner;
    u8 pad_0x10[0x30];
    u32 field_0x40;
    bool field_0x44;
};

void fn_7100468c94(UnkCrsDash_b57* self) {
    UnkAnimationComponent* target = self->owner->field_0x220;
    if (!target) {
        return;
    }
    if (!self->field_0x44) {
        self->field_0x40 = 0;
        return;
    }
    if (fn_710117ae38(target, 0xfb7d4bc21bc9379a, false)) {
        base::reflection::refl_EmitU32(self->owner->field_0x220, 0x1303e8318358324f, 0xf4061ff25a2163d6, 0,
                                       &g_unk_710164b958, &g_unk_7172769770);
    }
}
