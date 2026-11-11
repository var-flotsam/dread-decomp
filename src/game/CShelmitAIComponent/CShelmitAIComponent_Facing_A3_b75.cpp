// CShelmitAIComponent's unit (tools/tu_map.py), fn_71006ba194: by the sign of the owner's (+0x30) transform
// angle y (+0x38, derived lazily), unless the controller (+0x220) already has flag 3 (or 2 when negative,
// fn_7101178524), sets id 4 and clears 8 (or the other way round) as CShakernautAIComponent_Flip_b53.cpp sends
// them. Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
// As src/game/CAnimationComponent/CAnimationComponent_Flags_b47a3.cpp defines it.
struct UnkAnimFlagsHolder_b47a3;

struct UnkShelmitOwner_A3_b75 {
    u8 pad_0x0[0x38];
    UnkXform xform;  // 0x38
    u8 pad_0x94[0x220 - 0x94];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkShelmitAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_A3_b75* owner;  // 0x30
};

void fn_71006ba194(UnkShelmitAI_A3_b75* self) {
    UnkShelmitOwner_A3_b75* owner = self->owner;
    if (!owner)
        return;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    if (!ctrl)
        return;
    f32 y = owner->xform.GetAngle().y;
    goto compare;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
compare:
    if (y < 0.0f) {
        if (fn_7101178524(reinterpret_cast<UnkAnimFlagsHolder_b47a3*>(ctrl), 2))
            return;
        a1.value = 8;
        fn_71011785a8(ctrl, &a1, true, false);
        b1.value = 4;
        fn_7101178564(ctrl, &b1, true, false);
    } else {
        if (fn_7101178524(reinterpret_cast<UnkAnimFlagsHolder_b47a3*>(ctrl), 3))
            return;
        a2.value = 4;
        fn_71011785a8(ctrl, &a2, true, false);
        b2.value = 8;
        fn_7101178564(ctrl, &b2, true, false);
    }
}
