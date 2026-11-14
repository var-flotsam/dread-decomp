// CAbilityEnergyWave's unit (tools/tu_map.py): once the timer at +0xe0 reaches the tunable's limit (+0x54 of
// g_unk_71726af8b0) the key g_unk_7101648cf8 is set on the owner's (+0x30) timeline (+0x2e8) to the all-flags mask;
// the timer is cleared and the multi-lockon entry (fn_71003182d8, found by fn_7100300970) is told (slot 0xa0) to
// stop when its state is 6 (state at +0xb4, or while that is 7 the state of its object at +0x20). Names are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CAbilityMultiLockon.h"
#include "game/UnkActor.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

struct UnkEnergyWaveKey_b60a2;
extern UnkEnergyWaveKey_b60a2 g_unk_7101648cf8;  // reached through the GOT
CAbility* fn_7100300970(CAbilityComponent* self, const CStrId* id);

void fn_71002fe318(CAbilityComponent* self) {
    if (self->timer >= *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_71726af8b0) + 0x54)) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->Owner<UnkActor>()->field_0x2e8);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101648cf8), &mask.id, false, -1.0f);
    }
    self->timer = 0.0f;
    u64 id = fn_71003182d8();
    CAbility* entry = fn_7100300970(self, reinterpret_cast<const CStrId*>(id));
    if (!entry)
        return;
    s32 state = entry->state;
    if (state == 7) {
        if (!entry->state_holder)
            return;
        state = entry->state_holder->state;
    }
    if (state == 6)
        entry->Virtual(0xa0, true);
}
