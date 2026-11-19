// CShakernautDoubleGroundShockAttack unit (tools/tu_map.py unit 1041): moves the attack to its next state
// when the current animation ends, handing each new state's animation id to fn_71006a4d74; states 7 and 8
// first tell the CShakernautAIComponent (fn_71006b1228) and state 10 ends the attack (fn_71006a45e8).
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CShakernautDoubleGroundShockAttack.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern CStrId g_unk_71727057f0 __attribute__((visibility("hidden")));
extern CStrId g_unk_71727057f8 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705800 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705808 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705820 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705828 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705830 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172705848 __attribute__((visibility("hidden")));

struct UnkShakernautOwner_l6a5258 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkShakernautShockAttack_l6a5258 {
    void** vtable;
    UnkShakernautOwner_l6a5258* owner;  // 0x8
    u8 pad_0x10[0x30 - 0x10];
    u32 state;  // 0x30
    s32 field_0x34;
};

struct UnkShakernautShockAttack_ovb0020;
void fn_71006a4d74(UnkShakernautShockAttack_ovb0020* self, const CStrId* id, bool flag);
void fn_71006b1228(UnkReflObject* ai, bool flag);
void fn_71006a45e8(UnkShakernautShockAttack_l6a5258* self, u32 state);

// Plays a copy of the animation id (the copy is the caller's argument).
static inline void Play(UnkShakernautShockAttack_l6a5258* self, CStrId animation, bool flag) {
    fn_71006a4d74(reinterpret_cast<UnkShakernautShockAttack_ovb0020*>(self), &animation, flag);
}

// Whether the state changed.
static inline bool SetState(UnkShakernautShockAttack_l6a5258* self, u32 state) {
    if (self->state == state)
        return false;
    self->state = state;
    return true;
}

static inline void EnterState(UnkShakernautShockAttack_l6a5258* self, u32 state, const CStrId& animation) {
    if (SetState(self, state))
        Play(self, animation, false);
}

void fn_71006a5258(UnkShakernautShockAttack_l6a5258* self) {
    switch (self->state) {
    case 1:
        if (self->field_0x34 == 1)
            Play(self, g_unk_71727057c0[0], true);
        else
            Play(self, g_unk_71727057c0[1], true);
        SetState(self, 2);
        break;
    case 2:
        EnterState(self, 3, g_unk_71727057f0);
        break;
    case 3:
        EnterState(self, 5, g_unk_7172705800);
        break;
    case 4:
        EnterState(self, 6, g_unk_7172705808);
        break;
    case 7:
    case 8: {
        UnkReflObject* ai = self->owner->ai;
        if (ai && !fn_710009d50c(ai->vf_0x0(), fn_71006ad0a4()))
            ai = nullptr;
        if (self->state == 7) {
            fn_71006b1228(ai, true);
            EnterState(self, 9, g_unk_7172705820);
        } else {
            fn_71006b1228(ai, false);
            EnterState(self, 10, g_unk_7172705828);
        }
        break;
    }
    case 9:
        EnterState(self, 11, g_unk_7172705830);
        break;
    case 10:
        fn_71006a45e8(self, 12);
        break;
    case 11:
        EnterState(self, 4, g_unk_71727057f8);
        break;
    case 12:
        Play(self, g_unk_7172705848, true);
        break;
    }
}
