#pragma once

#include "game/UnkBossAIComponent.h"
#include "types.h"

// The Chozo Warrior's char class (CCharClassChozoWarriorAIComponent), as its constructor fn_7100497584
// builds it: the boss AI base, then its three attack definitions and a flag. The Chozo Warrior X's
// char class builds on it (fn_710048da20). Name and layout are placeholders.
struct UnkChozoWarriorCharClass : UnkBossAIComponent {
    u8 oChozoWarriorGlaiveSpinAttackDef[0xf0];  // the glaive spin attack's part (fn_71004923f4)
    u8 oChozoWarriorGlaiveWalljumpAttackDef[0xf0];  // the glaive walljump attack's part (fn_71004930c8)
    u8 oChozoWarriorDeflectorShieldAttackDef[0xf0];  // the deflector shield attack's part (fn_71004917d0)
    u8 bUsesShield;
};
