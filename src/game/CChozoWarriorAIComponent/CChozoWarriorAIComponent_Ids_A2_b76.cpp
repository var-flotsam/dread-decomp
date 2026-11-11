// CChozoWarriorAIComponent's unit (tools/tu_map.py): sends ids 0x10 and 0x20 to the owner's (+0x30) controller
// (+0x220) through fn_71011785a8 / fn_7101178564, choosing which goes through which by the flag at +0x14999 and a
// query on the owner's +0x228 object. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};

bool fn_7101173914(char* p0, char* p1);
extern char g_unk_71016499d8[];

struct UnkCwOwner_A2_b76 {
    u8 pad_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
    char* other;                  // 0x228
};

struct UnkCwAI_A2_b76 {
    u8 pad_0x0[0x30];
    UnkCwOwner_A2_b76* owner;  // 0x30
    u8 pad_0x38[0x14999 - 0x38];
    u8 flag;  // 0x14999
};

void fn_710049d094(UnkCwAI_A2_b76* self) {
    goto start;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
    UnkShakernautId_b53 b3;
start:
    UnkCwOwner_A2_b76* owner = self->owner;
    if (!owner)
        return;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    char* other = owner->other;
    if (!ctrl || !other)
        return;
    if (self->flag && fn_7101173914(other, g_unk_71016499d8)) {
        a1.value = 0x10;
        fn_7101178564(ctrl, &a1, true, false);
        b1.value = 0x20;
        fn_71011785a8(ctrl, &b1, true, false);
        return;
    }
    a2.value = 0x10;
    fn_71011785a8(ctrl, &a2, true, false);
    if (self->flag) {
        b2.value = 0x20;
        fn_7101178564(ctrl, &b2, true, false);
    } else {
        b3.value = 0x20;
        fn_71011785a8(ctrl, &b3, true, false);
    }
}
