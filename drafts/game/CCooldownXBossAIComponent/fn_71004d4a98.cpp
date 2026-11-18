// CCooldownXBossAIComponent's unit (tools/tu_map.py): after the base handler and fn_71004d28cc, when the
// finished entry is the one at +0x15510 in phase 2: in a CCharClassAttackTrack, resets layers 5 and 4 of the
// boss's and its target's character classes and starts the attack at +0x15480; otherwise, high enough, picks a
// random wait and sends one of two alternating hashes. Names and layouts are placeholders.
#include <stdlib.h>
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAIAttackComponent.h"
#include "game/CAnimationComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

struct UnkKraidUpdateAI_b62a4;
void fn_71003ad414(UnkKraidUpdateAI_b62a4* self);

extern base::global::CStrId g_unk_71726d8740 __attribute__((visibility("hidden")));
extern const u64 g_unk_7101649bb0;
extern const u64 g_unk_7101649ba8;

struct UnkCooldownXHeightSlot_l4d4a98 {
    u8 pad_0x0[0x54];
    f32 height;  // 0x54
};

struct UnkCooldownXOwner_l4d4a98 {
    u8 pad_0x0[0x1b8];
    UnkCooldownXHeightSlot_l4d4a98* field_0x1b8;
    u8 pad_0x1c0[0x60];
    UnkAnimationComponent* field_0x220;
    u8 pad_0x228[0x50];
    UnkCommanderPlayer_b67a2* player;  // 0x278
};

struct UnkCooldownXTuning_l4d4a98 {
    u8 pad_0x0[0xc68];
    f32 wait_min;  // 0xc68
    f32 wait_max;  // 0xc6c
};

struct UnkCooldownXAI_l4d4a98 {
    u8 pad_0x0[0x30];
    UnkCooldownXOwner_l4d4a98* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    s32 phase;  // 0x1140
    u8 pad_0x1144[0x149a0 - 0x1144];
    UnkCooldownXTuning_l4d4a98* tuning;  // 0x149a0
    u8 pad_0x149a8[0x15480 - 0x149a8];
    u8 entry_0x15480[0x90];
    u8 entry_0x15510[0x15684 - 0x15510];
    f32 wait;    // 0x15684
    s32 last;    // 0x15688
    s32 choice;  // 0x1568c
};
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d4a98, choice) == 0x1568c, "layout");

void fn_71004d28cc(UnkCooldownXAI_l4d4a98* self);

static inline f32 RandUnit() { return (rand() & 0x7fff) * (1.0 / 32767.0); }

void fn_71004d4a98(UnkCooldownXAI_l4d4a98* self, void* entry) {
    fn_71003ad414(reinterpret_cast<UnkKraidUpdateAI_b62a4*>(self));
    fn_71004d28cc(self);
    if (entry != self->entry_0x15510 || self->phase != 2)
        return;
    if (fn_710117b348(self->owner->field_0x220, fn_7100ceda9c())) {
        UnkActorTarget* mine = reinterpret_cast<UnkActorTarget*>(self->owner->field_0x220);
        UnkActorTarget* theirs =
            reinterpret_cast<UnkActorTarget*>(fn_7100d7c990(reinterpret_cast<UnkActor*>(self->owner))->field_0x220);
        if (mine) {
            fn_710117662c(mine, 5, 1, false, false, -1.0f);
            fn_7101178e08(mine, 4, false, false, -1.0f);
        }
        if (theirs) {
            fn_7101178e08(theirs, 4, false, false, -1.0f);
            fn_710117662c(theirs, 5, 1, false, false, -1.0f);
        }
        fn_7101134420(self->owner->player, self->entry_0x15480, true, 0, &g_unk_71726d8740, -1.0f);
        return;
    }
    if (!(self->owner->field_0x1b8->height > 2500.0f) || self->wait != 0.0f)
        return;
    f32 lo = self->tuning->wait_min;
    f32 hi = self->tuning->wait_max;
    f32 t = RandUnit();
    self->wait = lo + (hi - lo) * t;
    UnkAnimationComponent* cls = self->owner->field_0x220;
    if (fn_710117b348(cls, fn_7100c0d1d8()))
        return;
    if (self->last == self->choice && self->choice == 0) {
        self->last = 0;
        self->choice = 1;
    } else {
        int r = rand();
        self->last = self->choice;
        self->choice = r & 1;
    }
    UnkSpitSend(cls, self->choice ? g_unk_7101649bb0 : g_unk_7101649ba8, 0xf4061ff25a2163d6);
}
