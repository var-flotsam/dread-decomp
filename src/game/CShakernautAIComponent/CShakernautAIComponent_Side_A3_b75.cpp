// CShakernautAIComponent's unit (tools/tu_map.py), fn_71006ad858: from the x of the orientation state's
// direction (+0x147f8 +0x18, as src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp lays it
// out) clears one of ids 4/8 on the actor's controller (fn_7101178564), sets the other (fn_71011785a8) and
// records the side (+0xc: 1 past +0.001, 0 below it, 2 otherwise). Names and layouts are placeholders.
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
void fn_71011785a8(UnkShakernautCtrl_b53*, const UnkShakernautId_b53*, bool, bool);
void fn_7101178564(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b);

struct UnkShakernautSideState_A3_b75 {
    u8 unk_0x0[0xc];
    u32 side;  // 0xc
    u8 unk_0x10[8];
    f32 dir_x;  // 0x18
};

struct UnkShakernautActor_A3_b75 {
    u8 unk_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;
};

struct UnkShakernautAIComponent_A3_b75 {
    u8 unk_0x0[0x30];
    UnkShakernautActor_A3_b75* actor;
    u8 unk_0x38[0x147f8 - 0x38];
    UnkShakernautSideState_A3_b75 state;
};

void fn_71006ad858(UnkShakernautAIComponent_A3_b75* self) {
    goto start;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
    UnkShakernautId_b53 a3;
    UnkShakernautId_b53 b3;
start:
    UnkShakernautCtrl_b53* ctrl = self->actor->ctrl;
    if (!ctrl)
        return;
    UnkShakernautSideState_A3_b75* state = &self->state;
    f32 x = state->dir_x;
    if (x > 0.001f) {
        a1.value = 8;
        fn_7101178564(ctrl, &a1, true, false);
        b1.value = 4;
        fn_71011785a8(ctrl, &b1, true, false);
        state->side = 1;
    } else if (x < 0.001f) {
        a2.value = 4;
        fn_7101178564(ctrl, &a2, true, false);
        b2.value = 8;
        fn_71011785a8(ctrl, &b2, true, false);
        state->side = 0;
    } else {
        a3.value = 8;
        fn_7101178564(ctrl, &a3, true, false);
        b3.value = 4;
        fn_71011785a8(ctrl, &b3, true, false);
        state->side = 2;
    }
}
