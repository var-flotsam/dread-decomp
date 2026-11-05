// CTakumakuAIComponent's unit (tools/tu_map.py): when the owner (+0x30) has a controller (+0x220), takes the
// x of the owner's transform (+0x38) applied to (1, 0, 0) and, past +-0.001, sends ids 4 and 8 (swapped by
// sign) through fn_71011785a8 and fn_7101178564 (as CShakernautAIComponent_Flip_b53.cpp).
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
void fn_71011785a8(UnkShakernautCtrl_b53*, const UnkShakernautId_b53*, bool, bool);
void fn_7101178564(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b);

struct UnkTakumakuOwner_A4_b71 {
    u8 pad_0x0[0x38];
    UnkMatrix44 xform;  // 0x38
    u8 pad_0x78[0x220 - 0x78];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkTakumakuAI_A4_b71 {
    u8 pad_0x0[0x30];
    UnkTakumakuOwner_A4_b71* owner;  // 0x30
};

void fn_71006d5364(UnkTakumakuAI_A4_b71* self) {
    goto start;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
start:
    UnkTakumakuOwner_A4_b71* owner = self->owner;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    if (!ctrl)
        return;
    const UnkMatrix44& m = fn_710002614c(&owner->xform);
    f32 cx = m.m[2], cy = m.m[6], cz = m.m[10];
    f32 x = cx * 1.0f + cy * 0.0f + cz * 0.0f;
    if (x > 0.001f) {
        a1.value = 4;
        fn_71011785a8(ctrl, &a1, false, false);
        b1.value = 8;
        fn_7101178564(ctrl, &b1, true, true);
    } else if (x < -0.001f) {
        a2.value = 8;
        fn_71011785a8(ctrl, &a2, false, false);
        b2.value = 4;
        fn_7101178564(ctrl, &b2, true, true);
    }
}
