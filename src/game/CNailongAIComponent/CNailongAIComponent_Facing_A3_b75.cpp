// CNailongAIComponent's unit (tools/tu_map.py): the owner's (+0x30) controller (+0x220) is sent ids 4 and 8
// (swapped by the sign of the movement object's (+0x1f0) vector, past +-0.001). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"
#include "game/UnkShakernautId.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;

// Returned in memory: the type is not trivially copyable.
struct UnkNailongVec_A3_b75 {
    f32 x, y, z;
    UnkNailongVec_A3_b75(const UnkNailongVec_A3_b75& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkNailongMover_A3_b75 {
    void** vtable;
    UnkNailongVec_A3_b75 Get() {
        using Fn = UnkNailongVec_A3_b75 (*)(UnkNailongMover_A3_b75*);
        return reinterpret_cast<Fn>(vtable[0x210 / 8])(this);
    }
};

struct UnkNailongOwner_A3_b75 {
    u8 pad_0x0[0x1f0];
    UnkNailongMover_A3_b75* mover;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkNailongAI_A3_b75 {
    u8 pad_0x0[0x30];
    UnkNailongOwner_A3_b75* owner;  // 0x30
    u8 pad_0x38[0x1483c - 0x38];
    u32 side;  // 0x1483c
};

// fn_7100620ba0: by the sign of the vector's x (dead band +-0.001) clears one of ids 4/8 (fn_7101178564),
// sets the other (fn_71011785a8) and records the side (0 left, 1 right, 2 inside the band).
void fn_7100620ba0(UnkNailongAI_A3_b75* self) {
    goto start;
    UnkShakernautId_b53 a1;
start:
    UnkNailongOwner_A3_b75* owner = self->owner;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    UnkNailongMover_A3_b75* mover = owner->mover;
    if (!ctrl || !mover)
        return;
    f32 x;
    {
        UnkNailongVec_A3_b75 v = mover->Get();
        x = v.x;
    }
    goto compare;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
    UnkShakernautId_b53 a3;
    UnkShakernautId_b53 b3;
compare:
    if (x < -0.001f) {
        a1.value = 4;
        fn_7101178564(ctrl, &a1, true, false);
        b1.value = 8;
        fn_71011785a8(ctrl, &b1, true, false);
        self->side = 0;
    } else if (x > 0.001f) {
        a2.value = 8;
        fn_7101178564(ctrl, &a2, true, false);
        b2.value = 4;
        fn_71011785a8(ctrl, &b2, true, false);
        self->side = 1;
    } else {
        a3.value = 4;
        fn_7101178564(ctrl, &a3, true, false);
        b3.value = 8;
        fn_71011785a8(ctrl, &b3, true, false);
        self->side = 2;
    }
}
