// CAbilityEnergyWave's unit (tools/tu_map.py): sets the fixed id g_unk_7101648cf8 on the owner's (+0x30)
// timeline (+0x2e8) to the all-flags mask, with no null checks (the checked form is
// src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbilityLayout.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkEnergyWaveKey_b60a2;
extern UnkEnergyWaveKey_b60a2 g_unk_7101648cf8;  // reached through the GOT

void fn_7100302180(CAbilityComponent* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->Owner<UnkActor>()->field_0x2e8);
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101648cf8), &mask.id, false, -1.0f);
}
