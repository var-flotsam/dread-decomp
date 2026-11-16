// CAbilitySonar's unit (tools/tu_map.py unit 434): stops the sonar. In state 5 or 6 (state 7 defers to the
// holder at +0x20), sets one of the player's timeline ids by the holder's state and `on` to the all-flags mask
// (state 6 also removes two more ids); then fn_71002f9460(self, 0), resets the pair at +0x3cc and calls
// fn_7100335458. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkSelf;

extern const u64 g_unk_7101648fa0;
extern const u64 g_unk_7101648fa8;
extern const u64 g_unk_7101648fb0;
extern const u64 g_unk_7101648fb8;
extern const u64 g_unk_7101648fc0;
extern const u64 g_unk_7101648fc8;

struct UnkSonarStateHolder_l3350c4 {
    u8 pad_0x0[0x10];
    s32 state;  // 0x10
};

struct UnkSonarPair_l3350c4 {
    f32 a;
    f32 b;
};

struct UnkAbilitySonar_l3350c4 {
    u8 pad_0x0[0x20];
    UnkSonarStateHolder_l3350c4* state_holder;  // 0x20
    u8 pad_0x28[0xb4 - 0x28];
    s32 state;  // 0xb4
    u8 pad_0xb8[0x3cc - 0xb8];
    UnkSonarPair_l3350c4 field_0x3cc;  // 0x3cc

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
static_assert(__builtin_offsetof(UnkAbilitySonar_l3350c4, field_0x3cc) == 0x3cc, "layout");

// Defined in CAbilitySonar_Pulse_ovb0085.cpp.
struct UnkAbilitySonar_ovb0085;
void fn_7100335458(UnkAbilitySonar_ovb0085* self, bool on);

inline void SetMask(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

void fn_71003350c4(UnkAbilitySonar_l3350c4* self, bool on) {
    if (self->InState(6) || self->InState(5)) {
        if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
            if (auto* timeline = static_cast<UnkTimelineComponent*>(player->field_0x2e8)) {
                if (UnkSonarStateHolder_l3350c4* holder = self->state_holder) {
                    switch (holder->state) {
                    case 5:
                        if (on)
                            SetMask(timeline, &g_unk_7101648fa0);
                        else
                            SetMask(timeline, &g_unk_7101648fa8);
                        break;
                    case 6:
                        if (on)
                            SetMask(timeline, &g_unk_7101648fb0);
                        else
                            SetMask(timeline, &g_unk_7101648fb8);
                        fn_7100a2b7b8(timeline, &g_unk_7101648fc0);
                        fn_7100a2b7b8(timeline, &g_unk_7101648fc8);
                        break;
                    }
                }
            }
        }
    }
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
    self->field_0x3cc = {-0.001f, -0.001f};
    fn_7100335458(reinterpret_cast<UnkAbilitySonar_ovb0085*>(self), false);
}
