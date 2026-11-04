// CGoliathXBurstProjectionBombMovement's unit (tools/tu_map.py): while a delay (+0x500) is positive,
// only flags a wait (+0x4fc) and clears +0x4f8; otherwise, with bounces left (+0x504), uses one up,
// sets +0x4f5, clears the wait and +0x4f8 and sets the id g_unk_710164a018 on the owner's (+0x30)
// timeline (+0x2e8) to the all-flags mask (as
// src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkBurstBombKey_b60;
extern UnkBurstBombKey_b60 g_unk_710164a018;  // reached through the GOT

struct UnkBurstBombMovement_b60 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x4f5 - 0x38];
    bool field_0x4f5;
    u8 pad_0x4f6[2];
    u32 field_0x4f8;
    bool waiting;
    u8 pad_0x4fd[3];
    f32 delay;
    u32 bounces;
};

void fn_710058b4ac(UnkBurstBombMovement_b60* self) {
    if (!(self->delay <= 0.0f)) {
        self->waiting = true;
        self->field_0x4f8 = 0;
        return;
    }
    if (self->bounces != 0) {
        --self->bounces;
        UnkActor* owner = self->owner;
        self->field_0x4f5 = true;
        self->waiting = false;
        self->field_0x4f8 = 0;
        if (auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a018), &mask.id, false, -1.0f);
        }
    }
}
