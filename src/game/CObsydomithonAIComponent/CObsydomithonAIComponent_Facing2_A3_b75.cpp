// CObsydomithonAIComponent's unit (tools/tu_map.py): the owner's (+0x30) controller (+0x220) is sent ids 4 and 8
// (swapped by the sign of the movement object's (+0x1f0) vector, past +-0.001). Names and layouts are placeholders.
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};
void fn_71011785a8(UnkShakernautCtrl_b53*, const UnkShakernautId_b53*, bool, bool);
void fn_7101178564(UnkShakernautCtrl_b53* self, const UnkShakernautId_b53* id, bool a, bool b);

// Returned in memory: the type is not trivially copyable.
struct UnkObsyVec_A3_b75 {
    f32 x, y, z;
    UnkObsyVec_A3_b75(const UnkObsyVec_A3_b75& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkObsyMover_A3_b75 {
    void** vtable;
    UnkObsyVec_A3_b75 Get() {
        using Fn = UnkObsyVec_A3_b75 (*)(UnkObsyMover_A3_b75*);
        return reinterpret_cast<Fn>(vtable[0x210 / 8])(this);
    }
};

struct UnkObsyOwner_A3_b75 {
    u8 pad_0x0[0x1f0];
    UnkObsyMover_A3_b75* mover;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkObsyAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkObsyOwner_A3_b75* owner;  // 0x30
};

// fn_710062cf68: as fn_710062cc94 (src/game/CObsydomithonAIComponent/CObsydomithonAIComponent_Facing_A4_b71.cpp),
// but the vector is projected on the x axis, the second id is sent with both flags set, and nothing is sent
// inside the dead band.
void fn_710062cf68(UnkObsyAI_A3_b75* self) {
    goto start;
    UnkShakernautId_b53 a1;
start:
    UnkObsyOwner_A3_b75* owner = self->owner;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    UnkObsyMover_A3_b75* mover = owner->mover;
    if (!ctrl || !mover)
        return;
    f32 d;
    {
        UnkObsyVec_A3_b75 v = mover->Get();
        d = v.x * 1.0f + v.y * 0.0f + v.z * 0.0f;
    }
    goto compare;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
compare:
    if (d > 0.001f) {
        a1.value = 4;
        fn_71011785a8(ctrl, &a1, false, false);
        b1.value = 8;
        fn_7101178564(ctrl, &b1, true, true);
    } else if (d < -0.001f) {
        a2.value = 8;
        fn_71011785a8(ctrl, &a2, false, false);
        b2.value = 4;
        fn_7101178564(ctrl, &b2, true, true);
    }
}
