// CObsydomithonAIComponent's unit (tools/tu_map.py): when the owner (+0x30) has a controller (+0x220) and a
// movement object (+0x1f0), takes the x of the object's vector (virtual slot 0x210) and sends ids 4 and 8
// (swapped by sign, past +-0.001) through fn_71011785a8 and fn_7101178564 (as
// CShakernautAIComponent_Flip_b53.cpp). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};

// Returned in memory: the type is not trivially copyable.
struct UnkObsyVec_A4_b71 {
    f32 x, y, z;
    UnkObsyVec_A4_b71(const UnkObsyVec_A4_b71& o) : x(o.x), y(o.y), z(o.z) {}
};

struct UnkObsyMover_A4_b71 {
    void** vtable;
    UnkObsyVec_A4_b71 Get() {
        using Fn = UnkObsyVec_A4_b71 (*)(UnkObsyMover_A4_b71*);
        return reinterpret_cast<Fn>(vtable[0x210 / 8])(this);
    }
};

struct UnkObsyOwner_A4_b71 {
    u8 pad_0x0[0x1f0];
    UnkObsyMover_A4_b71* mover;  // 0x1f0
    u8 pad_0x1f8[0x220 - 0x1f8];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
};

struct UnkObsyAI_A4_b71 {
    u8 pad_0x0[0x30];
    UnkObsyOwner_A4_b71* owner;  // 0x30
};

void fn_710062cc94(UnkObsyAI_A4_b71* self) {
    UnkObsyOwner_A4_b71* owner = self->owner;
    UnkShakernautCtrl_b53* ctrl = owner->ctrl;
    UnkObsyMover_A4_b71* mover = owner->mover;
    if (!ctrl || !mover)
        return;
    UnkObsyVec_A4_b71 v = mover->Get();
    if (v.x < -0.001f) {
        UnkShakernautId_b53 a;
        a.value = 8;
        fn_71011785a8(ctrl, &a, false, false);
        UnkShakernautId_b53 b;
        b.value = 4;
        fn_7101178564(ctrl, &b, true, false);
    } else if (v.x > 0.001f) {
        UnkShakernautId_b53 a;
        a.value = 4;
        fn_71011785a8(ctrl, &a, false, false);
        UnkShakernautId_b53 b;
        b.value = 8;
        fn_7101178564(ctrl, &b, true, false);
    } else {
        UnkShakernautId_b53 a;
        a.value = 8;
        fn_71011785a8(ctrl, &a, false, false);
        UnkShakernautId_b53 b;
        b.value = 4;
        fn_7101178564(ctrl, &b, true, false);
    }
}
