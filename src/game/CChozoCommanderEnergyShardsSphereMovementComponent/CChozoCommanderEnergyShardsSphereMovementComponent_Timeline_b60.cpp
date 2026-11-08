// CChozoCommanderEnergyShardsSphereMovementComponent's unit (tools/tu_map.py): after the base call
// (fn_71010d1a90), when the owner's (+0x30) +0x1b8 object is in state 2 (+0x70) and the owner has a
// timeline (+0x2e8) that does not know the fixed id 0x793b324db7dc1890 yet, sets g_unk_7101649720 to
// the all-flags mask. Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"
#include "game/UnkMovementComponent.h"

struct UnkShardsSphere_b60a2;

struct UnkShardsSphereKey_b60a2;
extern UnkShardsSphereKey_b60a2 g_unk_7101649720;  // reached through the GOT

struct UnkShardsSphereState_b60a2 {
    u8 pad_0x0[0x70];
    s32 field_0x70;
};

struct UnkShardsSphere_b60a2 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_710045a754(UnkShardsSphere_b60a2* self, f32 dt) {
    fn_71010d1a90(reinterpret_cast<UnkMovementComponent*>(self), dt);
    UnkActor* owner = self->owner;
    auto* state = static_cast<UnkShardsSphereState_b60a2*>(owner->field_0x1b8);
    if (!state || state->field_0x70 != 2)
        return;
    auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8);
    if (!timeline)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id = 0x793b324db7dc1890ull;
    if (fn_7100a2be94(timeline, &id))
        return;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649720), &mask.id, false, -1.0f);
}
