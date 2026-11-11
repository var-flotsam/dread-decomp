// CShakernautAIComponent's unit (tools/tu_map.py), fn_71006b1ddc: turns the actor (+0x30) toward `dir`: hands
// its +0x298 the facing query of src/game/CTakumakuDashAttack/CTakumakuDashAttack_Start_b58.cpp with `dir`,
// then, when the actor has a controller (+0x220), clears one of ids 4/8 (fn_7101178564), sets the other
// (fn_71011785a8) and records the side (+0x147f8 +0xc: 1 when dir.x is past +0.001), and stores `dir` as the
// state's direction (+0x18, fn_710007b444). Names and layouts are placeholders.
#include "game/CAnimationComponent.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

// As src/game/CShakernautAIComponent/CShakernautAIComponent_Flip_b53.cpp declares them.
struct UnkShakernautCtrl_b53;
struct UnkShakernautId_b53 {
    u32 value;
    u32 unk_0x4;
};

// As src/game/CBatalloonAIComponent/fn_71003de7b4.cpp declares them.
struct UnkVec3 {
    u32 a;
    u32 b;
    u32 c;
};
void fn_710007b444(void* dst, const UnkVec3* value);
// fn_71006f4b0c is declared in game_s2/UnkParams.h.

struct UnkShakernautQuery_A3_b75 {
    void** vtable;
    u32 dir[3];
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u64 field_0x20;
    u64 field_0x28;
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;

    UnkShakernautQuery_A3_b75() {
        vtable = g_unk_7101a24b58 + 2;
        __builtin_memset(dir, 0, 13);
        field_0x1c = 0;
        field_0x20 = 0;
        field_0x28 = 0;
        field_0x30 = 0;
        field_0x34 = 0;
    }
};

struct UnkShakernautFaceState_A3_b75 {
    u8 unk_0x0[0xc];
    u32 side;  // 0xc
    u8 unk_0x10[8];
    u8 dir[0xc];  // 0x18
};

struct UnkShakernautFaceActor_A3_b75 {
    u8 unk_0x0[0x220];
    UnkShakernautCtrl_b53* ctrl;  // 0x220
    u8 unk_0x228[0x298 - 0x228];
    void* target;  // 0x298
};

struct UnkShakernautFaceAI_A3_b75 {
    u8 unk_0x0[0x30];
    UnkShakernautFaceActor_A3_b75* actor;
    u8 unk_0x38[0x147f8 - 0x38];
    UnkShakernautFaceState_A3_b75 state;
};

void fn_71006b1ddc(UnkShakernautFaceAI_A3_b75* self, const UnkVec3* dir) {
    void* target = self->actor->target;
    if (!target)
        return;
    UnkShakernautQuery_A3_b75 query;
    query.dir[0] = dir->a;
    query.dir[1] = dir->b;
    query.dir[2] = dir->c;
    query.field_0x14 = 1;
    fn_71006f4b0c(target, &query, 0);
    goto start;
    UnkShakernautId_b53 a1;
    UnkShakernautId_b53 b1;
    UnkShakernautId_b53 a2;
    UnkShakernautId_b53 b2;
start:
    if (UnkShakernautCtrl_b53* ctrl = self->actor->ctrl) {
        if (*reinterpret_cast<const f32*>(&dir->a) > 0.001f) {
            a1.value = 8;
            fn_7101178564(ctrl, &a1, true, false);
            b1.value = 4;
            fn_71011785a8(ctrl, &b1, true, false);
            self->state.side = 1;
        } else {
            a2.value = 4;
            fn_7101178564(ctrl, &a2, true, false);
            b2.value = 8;
            fn_71011785a8(ctrl, &b2, true, false);
            self->state.side = 0;
        }
    }
    fn_710007b444(self->state.dir, dir);
}
