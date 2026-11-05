// CCooldownXBossAIComponent's unit (tools/tu_map.py): updates the member at +0x15130 (fn_71004c7a88),
// then, when the owner's (+0x30) timeline (+0x2e8) does not know the id g_unk_7101649b98 yet, sets it
// to the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkCooldownXBossMember_b60a2;
void fn_71004c7a88(UnkCooldownXBossMember_b60a2* self);

struct UnkCooldownXBossKey_b60a2;
extern UnkCooldownXBossKey_b60a2 g_unk_7101649b98;  // reached through the GOT

struct UnkCooldownXBossTimeline_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x15130 - 0x38];
    u8 field_0x15130[1];
};

void fn_71004d3f44(UnkCooldownXBossTimeline_b60a2* self) {
    fn_71004c7a88(reinterpret_cast<UnkCooldownXBossMember_b60a2*>(self->field_0x15130));
    const u64* key = reinterpret_cast<const u64*>(&g_unk_7101649b98);
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && !fn_7100a2be94(timeline, key)) {
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, key, &mask.id, false, -1.0f);
    }
}
