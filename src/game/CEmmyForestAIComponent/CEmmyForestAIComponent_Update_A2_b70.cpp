// CEmmyForestAIComponent's unit (tools/tu_map.py): an update. When the owner (+0x30) has a controller
// (+0x220), sends it id 0x40 through fn_71011785a8 if the game manager's fn_7100df5740 holds, else through
// fn_7101178564 (as CShakernautAIComponent_Flip_b53.cpp); then the base update fn_710054d16c.
// Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"
#include "game/UnkShakernautId.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;

struct UnkEmmyForestOwner_A2_b70 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkEmmyForestAI_A2_b70 {
    u8 pad_0x0[0x30];
    UnkEmmyForestOwner_A2_b70* owner;  // 0x30
};

void fn_710054d16c(UnkEmmyForestAI_A2_b70* self, f32 dt);

void fn_71004f2c60(UnkEmmyForestAI_A2_b70* self, f32 dt) {
    goto start;
    UnkShakernautId_b53 set_id;
    UnkShakernautId_b53 clear_id;
start:
    if (self->owner->ctrl) {
        bool on = fn_7100df5740(g_unk_7101d47310);
        UnkShakernautCtrl_b53* ctrl = self->owner->ctrl;
        if (on) {
            set_id.value = 0x40;
            fn_71011785a8(ctrl, &set_id, true, false);
        } else {
            clear_id.value = 0x40;
            fn_7101178564(ctrl, &clear_id, true, false);
        }
    }
    fn_710054d16c(self, dt);
}
