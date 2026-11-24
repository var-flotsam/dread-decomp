// Node routine that calls fn_710041a198, fn_71004917d0, fn_71004923f4, fn_71004930c8, sets its vtable and initialises or clears fields.
#include <new>

#include "base/global/CStrId.h"
#include "base/global/UnkTwoStepStrId.h"
#include "game/CCharClassChozoWarriorAIComponent.h"
#include "game/CCharClassChozoWarriorDeflectorShieldAttack.h"
#include "game/CCharClassChozoWarriorGlaiveSpinAttack.h"
#include "game/CCharClassChozoWarriorGlaiveWalljumpAttack.h"
#include "game/UnkBossAIComponent.h"
#include "game/UnkChozoWarriorCharClass.h"
#include "types.h"

using base::global::UnkStrId;

void fn_7100497584(UnkChozoWarriorCharClass* self) {
    fn_710041a198(self);
    self->vtable = &g_unk_7101a3cab0[2];
    fn_71004923f4(self->oChozoWarriorGlaiveSpinAttackDef);
    fn_71004930c8(self->oChozoWarriorGlaiveWalljumpAttackDef);
    fn_71004917d0(self->oChozoWarriorDeflectorShieldAttackDef);
    self->bUsesShield = 1;
    self->field_0x4fc = 257;
    self->field_0x409 = 0;
}
