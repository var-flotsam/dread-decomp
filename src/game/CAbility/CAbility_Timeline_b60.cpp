// CAbility's unit (tools/tu_map.py): in state 6 (state 7 defers to the ability at +0x20, as
// src/game/CAbility/CAbility_StateChecks.cpp), on the timeline (+0x2e8) of the +0x30 actor of the
// +0xc0 object, removes the id vf_0xe8 returns and sets the id vf_0xd8 returns to the all-flags mask.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkAbility429;
void fn_710030171c(UnkAbility429* self, f32 amount);
struct UnkAbility_b60a2;
void fn_71003020f0(UnkAbility429* part, UnkAbility_b60a2* ability);

namespace {
struct UnkAbilityState_b60a2 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};
}  // namespace

struct UnkAbilityHolder_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

struct UnkAbilityVtbl_b60a2 {
    u8 pad_0x0[0xc0];
    f32 (*vf_0xc0)(UnkAbility_b60a2*);
    u8 pad_0xc8[0x8];
    u64 (*vf_0xd0)(UnkAbility_b60a2*);
    u64 (*vf_0xd8)(UnkAbility_b60a2*);
    u8 pad_0xe0[0x8];
    u64 (*vf_0xe8)(UnkAbility_b60a2*);
};

struct UnkAbility_b60a2 {
    UnkAbilityVtbl_b60a2* vtbl;
    u8 pad_0x8[0x20 - 0x8];
    UnkAbilityState_b60a2* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 field_0xb4;
    u8 pad_0xb8[0x8];
    UnkAbilityHolder_b60a2* field_0xc0;
    u8 pad_0xc8[0xd8 - 0xc8];
    s32 field_0xd8;
    u8 pad_0xdc[0xe4 - 0xdc];
    s32 field_0xe4;
};

// Takes the `kind` its callers set (w1) like fn_71002f9f18, without using it; the original leaves it
// untouched up to its virtual getters (tools/forwarded_args.py).
void fn_71002f9de8(UnkAbility_b60a2* self, s32 /*kind*/) {
    s32 state = self->field_0xb4;
    if (state == 7) {
        if (!self->field_0x20)
            return;
        state = self->field_0x20->field_0x10;
    }
    if (state != 6)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->field_0xc0->field_0x30->field_0x2e8);
    if (!timeline)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id = self->vtbl->vf_0xe8(self);
    fn_7100a2b7b8(timeline, &id);
    id = self->vtbl->vf_0xd8(self);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
}

// Enters `kind`: clears +0xd8; for kind 4, sets the id vf_0xd0 returns on the player's timeline to the
// all-flags mask; then passes a positive vf_0xc0 amount to fn_710030171c and itself to fn_71003020f0
// (both on +0xc0), and clears +0xe4.
void fn_71002f9f18(UnkAbility_b60a2* self, s32 kind) {
    self->field_0xd8 = 0;
    if (kind == 4) {
        if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
            if (auto* timeline = static_cast<UnkTimelineComponent*>(player->field_0x2e8)) {
                base::reflection::UnkReflTypeIdArg mask;
                u64 id = self->vtbl->vf_0xd0(self);
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
            }
        }
    }
    f32 amount = self->vtbl->vf_0xc0(self);
    if (amount > 0.0f)
        fn_710030171c(reinterpret_cast<UnkAbility429*>(self->field_0xc0), amount);
    fn_71003020f0(reinterpret_cast<UnkAbility429*>(self->field_0xc0), self);
    self->field_0xe4 = 0;
}
