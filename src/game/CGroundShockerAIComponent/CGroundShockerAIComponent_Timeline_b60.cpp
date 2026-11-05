// CGroundShockerAIComponent's unit (tools/tu_map.py): when there is an owner (+0x30) with a
// timeline (+0x2e8), sets its fixed id (g_unk_710164a098) to the all-flags mask (as
// src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkGroundShockerKey_b60;
extern UnkGroundShockerKey_b60 g_unk_710164a098;  // reached through the GOT

struct UnkGroundShockerAI_b60 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_71005a4510(UnkGroundShockerAI_b60* self) {
    if (self->owner) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a098), &mask.id, false, -1.0f);
        }
    }
}
