// CCooldownXBossWindTunnelAttack's unit (tools/tu_map.py): when the owner's (+0x8) timeline (+0x2e8)
// knows the id g_unk_7101649b40, sets that id to the all-flags mask (as
// src/game/CKraidSpikeMovablePlatformComponent/CKraidSpikeMovablePlatformComponent_Start_b59.cpp).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWindTunnelKey_b60a2;
extern UnkWindTunnelKey_b60a2 g_unk_7101649b40;  // reached through the GOT

struct UnkWindTunnelTimeline_b60a2 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
};

void fn_71004cbc28(UnkWindTunnelTimeline_b60a2* self) {
    const u64* key = reinterpret_cast<const u64*>(&g_unk_7101649b40);
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && fn_7100a2c13c(timeline, key)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
}
