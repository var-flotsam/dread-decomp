// CAbilityShinespark unit: stops the shinespark. Unless the sub-state (+0x6b0, 6 when unset) is 1 or 6 and the
// ability is not in state 6, updates the player's timeline: in sub-states 3 to 5 sets one id when `on` and resets
// two more on the actor; in sub-state 2 runs fn_710032e220; when not `on`, swaps the ids back. Then
// fn_71002f9460(self, 0). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSelf;
struct UnkShinesparkSet_A2_b65;
void fn_710032e220(UnkShinesparkSet_A2_b65* self);

extern const u64 g_unk_7101648ec8;
extern const u64 g_unk_7101648f30;
extern const u64 g_unk_7101648f38;
extern const u64 g_unk_7101648f40;
extern const u64 g_unk_7101648f48;
extern const u64 g_unk_7101648f58;

struct UnkShinesparkStateHolder_l330ed0 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkShinesparkLink_l330ed0 {
    u8 pad_0x0[0x30];
    UnkActor* actor;
};

struct UnkAbilityShinespark_l330ed0 {
    u8 pad_0x0[0x20];
    UnkShinesparkStateHolder_l330ed0* state_holder;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;  // 0xb4
    u8 pad_0xb8[0xc0 - 0xb8];
    UnkShinesparkLink_l330ed0* link;  // 0xc0
    u8 pad_0xc8[0x6b0 - 0xc8];
    UnkShinesparkStateHolder_l330ed0* sub;  // 0x6b0

    // Whether the state (7 deferring to the holder's) is `s`.
    bool InState(s32 s) const {
        s32 current = state;
        if (current == 7) {
            if (!state_holder)
                return false;
            current = state_holder->state;
        }
        return current == s;
    }
};
static_assert(__builtin_offsetof(UnkAbilityShinespark_l330ed0, sub) == 0x6b0, "layout");

inline void SetMask(UnkTimelineComponent* timeline, const u64* id, base::reflection::UnkReflTypeIdArg& mask) {
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

void fn_7100330ed0(UnkAbilityShinespark_l330ed0* self, bool on) {
    s32 sub = self->sub ? self->sub->state : 6;
    if (self->InState(6) || (sub != 1 && sub != 6)) {
        if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
            if (auto* timeline = static_cast<UnkTimelineComponent*>(player->field_0x2e8)) {
                base::reflection::UnkReflTypeIdArg off_mask;
                base::reflection::UnkReflTypeIdArg on_mask;
                base::reflection::UnkReflTypeIdArg last_mask;
                bool charging = static_cast<u32>(sub - 3) < 3;
                if (!charging) {
                    if (sub == 2)
                        fn_710032e220(reinterpret_cast<UnkShinesparkSet_A2_b65*>(self));
                } else {
                    if (on)
                        SetMask(timeline, &g_unk_7101648f38, on_mask);
                    UnkActor* actor = self->link->actor;
                    fn_7100d7c828(actor, &g_unk_7101648f48, false);
                    fn_7100d7c828(actor, &g_unk_7101648f58, false);
                }
                if (!on) {
                    fn_7100a2b7b8(timeline, &g_unk_7101648ec8);
                    SetMask(timeline, &g_unk_7101648f30, off_mask);
                    fn_7100a2b7b8(timeline, &g_unk_7101648f38);
                    SetMask(timeline, &g_unk_7101648f40, last_mask);
                }
            }
        }
    }
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
}
