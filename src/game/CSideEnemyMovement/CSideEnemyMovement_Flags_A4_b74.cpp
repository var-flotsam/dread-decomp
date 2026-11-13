// CSideEnemyMovement unit (tools/tu_map.py, unit 1471): when the owner (+0x30) has a controller (+0x220), swaps
// the two flag bits named by the config at +0x1c20 (+0x160, +0x164): whichever is set (fn_7101178524) is
// cleared (fn_7101178564) and the other set (fn_71011785a8), as CShakernautAIComponent_Flip_b53.cpp sends them.
// Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"
#include "game/UnkShakernautId.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
// As src/game/CAnimationComponent/CAnimationComponent_Flags_b47a3.cpp defines it.
struct UnkAnimFlagsHolder_b47a3;

struct UnkSideEnemyOwner_A4_b74 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkSideEnemyConfig_A4_b74 {
    u8 pad_0x0[0x160];
    u32 bit_a;
    u32 bit_b;
};

struct UnkSideEnemyMovement_A4_b74 {
    u8 pad_0x0[0x30];
    UnkSideEnemyOwner_A4_b74* owner;
    u8 pad_0x38[0x1c20 - 0x38];
    UnkSideEnemyConfig_A4_b74* config;  // 0x1c20
};

void fn_71009b6b6c(UnkSideEnemyMovement_A4_b74* self) {
    goto start;
    UnkShakernautId_b53 set_a;
    UnkShakernautId_b53 clear_b;
    UnkShakernautId_b53 set_b;
    UnkShakernautId_b53 clear_a;
start:
    if (!self->owner)
        return;
    UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
    if (!ctrl)
        return;
    if (fn_7101178524(reinterpret_cast<UnkAnimFlagsHolder_b47a3*>(ctrl), self->config->bit_a)) {
        set_a.value = 1u << self->config->bit_a;
        fn_71011785a8(ctrl, &set_a, true, false);
        clear_b.value = 1u << self->config->bit_b;
        fn_7101178564(ctrl, &clear_b, true, false);
    } else {
        set_b.value = 1u << self->config->bit_b;
        fn_71011785a8(ctrl, &set_b, true, false);
        clear_a.value = 1u << self->config->bit_a;
        fn_7101178564(ctrl, &clear_a, true, false);
    }
}
