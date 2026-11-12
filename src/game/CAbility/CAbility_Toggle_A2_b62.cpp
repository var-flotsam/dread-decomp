// CAbility's unit (tools/tu_map.py): in state 6 (state 7 defers to the ability at +0x20), on the player's
// timeline (+0x2e8) removes the id vf_0xd0 returns when it is there, then sets the id vf_0xe0 (`on`) or vf_0xe8
// returns to the all-flags mask; in every case ends with fn_71002f9460(self, 0). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbility.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkPlayer;
struct UnkSelf;

namespace {
struct UnkAbilityState_b62a2 {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};
}  // namespace

struct UnkAbility_b62a2;
struct UnkAbilityVtbl_b62a2 {
    u8 pad_0x0[0xd0];
    u64 (*vf_0xd0)(UnkAbility_b62a2*);
    u8 pad_0xd8[0x8];
    u64 (*vf_0xe0)(UnkAbility_b62a2*);
    u64 (*vf_0xe8)(UnkAbility_b62a2*);
};

struct UnkAbility_b62a2 {
    UnkAbilityVtbl_b62a2* vtbl;
    u8 pad_0x8[0x20 - 0x8];
    UnkAbilityState_b62a2* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 field_0xb4;
};

void fn_71002fa078(UnkAbility_b62a2* self, bool on) {
    s32 state = self->field_0xb4;
    if (state == 7) {
        if (!self->field_0x20)
            goto done;
        state = self->field_0x20->field_0x10;
    }
    if (state == 6) {
        if (auto* player = reinterpret_cast<UnkActor*>(fn_7100dec788(g_unk_7101d47310))) {
            if (auto* timeline = static_cast<UnkTimelineComponent*>(player->field_0x2e8)) {
                // Jumped over: the mask gets no lifetime markers, so the ids share the other slot.
                goto start;
                base::reflection::UnkReflTypeIdArg mask;
            start:
                {
                    u64 id = self->vtbl->vf_0xd0(self);
                    if (fn_7100a2cfe8(timeline, &id)) {
                        id = self->vtbl->vf_0xd0(self);
                        fn_7100a2b7b8(timeline, &id);
                    }
                }
                if (on) {
                    u64 key = self->vtbl->vf_0xe0(self);
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
                } else {
                    u64 key = self->vtbl->vf_0xe8(self);
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &key, &mask.id, false, -1.0f);
                }
            }
        }
    }
done:
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
}

// On the timeline (+0x2e8) of the +0x30 actor of the +0xc0 object: when the id vf_0xe0 returns is known, removes
// it and sets the id vf_0xe8 returns; then in state 6 removes the id vf_0xd8 returns and sets vf_0xe8's again.
struct UnkAbilityHolder_b62a2 {
    u8 pad_0x0[0x30];
    UnkActor* field_0x30;
};

struct UnkAbility2_b62a2;
struct UnkAbility2Vtbl_b62a2 {
    u8 pad_0x0[0xd8];
    u64 (*vf_0xd8)(UnkAbility2_b62a2*);
    u64 (*vf_0xe0)(UnkAbility2_b62a2*);
    u64 (*vf_0xe8)(UnkAbility2_b62a2*);
};

struct UnkAbility2_b62a2 {
    UnkAbility2Vtbl_b62a2* vtbl;
    u8 pad_0x8[0x20 - 0x8];
    UnkAbilityState_b62a2* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 field_0xb4;
    u8 pad_0xb8[0x8];
    UnkAbilityHolder_b62a2* field_0xc0;
};

// Takes the `kind` its callers set (w1) like fn_71002f9f18, without using it; the original leaves it
// untouched up to its virtual getters (tools/forwarded_args.py).
void fn_71002f9bb4(UnkAbility2_b62a2* self, s32 /*kind*/) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->field_0xc0->field_0x30->field_0x2e8);
    if (!timeline)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id = self->vtbl->vf_0xe0(self);
    if (fn_7100a2be94(timeline, &id)) {
        id = self->vtbl->vf_0xe0(self);
        fn_7100a2b7b8(timeline, &id);
        id = self->vtbl->vf_0xe8(self);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
    s32 state = self->field_0xb4;
    if (state == 7) {
        if (!self->field_0x20)
            return;
        state = self->field_0x20->field_0x10;
    }
    if (state != 6)
        return;
    id = self->vtbl->vf_0xd8(self);
    fn_7100a2b7b8(timeline, &id);
    id = self->vtbl->vf_0xe8(self);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
}
