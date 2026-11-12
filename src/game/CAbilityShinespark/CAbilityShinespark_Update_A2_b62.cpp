// CAbilityShinespark's unit (tools/tu_map.py): after the ability step fn_71002f9bb4, looks at the sub-state at
// +0x6b0: in state 2 removes g_unk_7101648e88 and g_unk_7101648ea8 from the actor's (+0x30 of +0xc0) timeline
// (+0x2e8) and sets g_unk_7101648f30 to the all-flags mask; in states 3 and 4, when the actor has its +0x210, sends
// its +0x220 the id at +0x30 of the id table's entry at +0x1d78 (as fn_710032f9b0,
// src/game/CAbilityShinespark/CAbilityShinespark_Send_b55.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkAbility2_b62a2;
void fn_71002f9bb4(UnkAbility2_b62a2* self, s32 kind);  // src/game/CAbility/CAbility_Toggle_A2_b62.cpp
extern const u64 g_unk_7101648e88;
extern const u64 g_unk_7101648ea8;  // reached through the GOT
extern const u64 g_unk_7101648f30;  // reached through the GOT

struct UnkIdHolder_b62a2 {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkShinesparkActor_b62a2 {
    u8 pad_0x0[0x210];
    void* field_0x210;
    u8 pad_0x218[0x8];
    void* field_0x220;
    u8 pad_0x228[0x2e8 - 0x228];
    void* field_0x2e8;
};

struct UnkShinesparkLink_b62a2 {
    u8 pad_0x0[0x30];
    UnkShinesparkActor_b62a2* actor;
};

struct UnkShinesparkSub_b62a2 {
    u8 pad_0x0[0x10];
    s32 state;
};

struct UnkShinespark_b62a2 {
    u8 pad_0x0[0xc0];
    UnkShinesparkLink_b62a2* link;
    u8 pad_0xc8[0x6b0 - 0xc8];
    UnkShinesparkSub_b62a2* sub;
};

// `kind` (w1) reaches the base (fn_71002f9bb4) untouched in the original (tools/forwarded_args.py).
void fn_7100330c3c(UnkShinespark_b62a2* self, s32 kind) {
    fn_71002f9bb4(reinterpret_cast<UnkAbility2_b62a2*>(self), kind);
    // Jumped over: the masks and the send's mode get no lifetime markers, so they keep their own slots.
    goto start;
    base::reflection::UnkReflTypeIdArg mask;
    base::reflection::UnkReflTypeIdArg send_mode;
    base::reflection::UnkReflTypeIdArg send_mask;
start:
    UnkShinesparkSub_b62a2* sub = self->sub;
    if (!sub)
        return;
    if (sub->state == 2) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->link->actor->field_0x2e8)) {
            fn_7100a2b7b8(timeline, &g_unk_7101648e88);
            fn_7100a2b7b8(timeline, &g_unk_7101648ea8);
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &g_unk_7101648f30, &mask.id, false, -1.0f);
        }
        sub = self->sub;
        if (!sub)
            return;
    }
    // The original reads the state again on both paths.
    asm("" : "+r"(sub));
    if (static_cast<u32>(sub->state - 3) > 1)
        return;
    UnkShinesparkActor_b62a2* actor = self->link->actor;
    if (actor->field_0x210) {
        void* target = actor->field_0x220;
        u64 id = reinterpret_cast<UnkIdHolder_b62a2*>(GameManagerIdTable().field_0x1d78.data)->id;
        send_mode.id = 0;
        send_mask.id = UnkAll_SpitSend().value;
        fn_7101176690(target, id, 0xf4061ff25a2163d6, &send_mode, &g_unk_710164b958, &g_unk_7172769740, &send_mask);
    }
}
