// CAbilityOpticCamouflage's unit (tools/tu_map.py): in state 6 (state 7 defers to the ability at +0x20) or when
// turning off, on the player's timeline (+0x2e8) removes the id vf_0xd0 returns when it is there, then sets the id
// vf_0xe0 (`on`) or vf_0xe8 returns to the all-flags mask; in every case ends with fn_71002f9460(self, 0). The
// shape of fn_71002fa078 (src/game/CAbility/CAbility_Toggle_A2_b62.cpp). Names are placeholders.
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
struct UnkAbilityState_b62a2o {
    u8 pad_0x0[0x10];
    s32 field_0x10;
};
}  // namespace

struct UnkAbility_b62a2o;
struct UnkAbilityVtbl_b62a2o {
    u8 pad_0x0[0xd0];
    u64 (*vf_0xd0)(UnkAbility_b62a2o*);
    u8 pad_0xd8[0x8];
    u64 (*vf_0xe0)(UnkAbility_b62a2o*);
    u64 (*vf_0xe8)(UnkAbility_b62a2o*);
};

struct UnkAbility_b62a2o {
    UnkAbilityVtbl_b62a2o* vtbl;
    u8 pad_0x8[0x20 - 0x8];
    UnkAbilityState_b62a2o* field_0x20;
    u8 pad_0x28[0xb4 - 0x28];
    s32 field_0xb4;
};

void fn_7100323b60(UnkAbility_b62a2o* self, bool on) {
    s32 state = self->field_0xb4;
    if (state == 7)
        state = self->field_0x20 ? self->field_0x20->field_0x10 : 7;
    if (state == 6 || !on) {
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
    fn_71002f9460(reinterpret_cast<UnkSelf*>(self), 0);
}
