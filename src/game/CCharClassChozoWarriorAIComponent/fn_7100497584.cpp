// Node routine that calls fn_710041a198, fn_71004917d0, fn_71004923f4, fn_71004930c8, sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCharClassChozoWarriorAIComponent.h"
#include "game/CCharClassChozoWarriorDeflectorShieldAttack.h"
#include "game/CCharClassChozoWarriorGlaiveSpinAttack.h"
#include "game/CCharClassChozoWarriorGlaiveWalljumpAttack.h"
#include "game/UnkBossAIComponent.h"
#include "types.h"

using base::global::UnkStrId;

struct UnkNode : UnkBossAIComponent {
    u8 oChozoWarriorGlaiveSpinAttackDef[0xf0];  // the glaive spin attack's part (fn_71004923f4)
    u8 oChozoWarriorGlaiveWalljumpAttackDef[0xf0];  // the glaive walljump attack's part (fn_71004930c8)
    u8 oChozoWarriorDeflectorShieldAttackDef[0xf0];  // the deflector shield attack's part (fn_71004917d0)
    u8 bUsesShield;
};

void fn_7100497584(UnkNode* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a3cab0[2];
    fn_71004923f4(self->oChozoWarriorGlaiveSpinAttackDef);
    fn_71004930c8(self->oChozoWarriorGlaiveWalljumpAttackDef);
    fn_71004917d0(self->oChozoWarriorDeflectorShieldAttackDef);
    self->bUsesShield = 1;
    self->field_0x4fc = 257;
    self->field_0x409 = 0;
}
