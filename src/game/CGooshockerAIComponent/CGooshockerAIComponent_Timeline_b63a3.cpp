// CGooshockerAIComponent's unit (tools/tu_map.py unit 854): fn_710059e4f0 sets one of two fixed
// ids (g_unk_710164a060 when the flag is set, else g_unk_710164a068) on the owner's timeline
// (+0x2e8) to the all-flags mask, as src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CGooshockerAIComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkGooshockerKey_b63a3;
extern UnkGooshockerKey_b63a3 g_unk_710164a060;  // reached through the GOT
extern UnkGooshockerKey_b63a3 g_unk_710164a068;  // reached through the GOT

struct UnkGooshockerState;
struct UnkGooshockerAI_b63a3 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

// MATCHES with this bool signature; tracked src/game/CGooshockerAIComponent/CGooshockerAIComponent_Start_b48.cpp
// declares (UnkGooshockerState*, u32), with which the branch comes out tbnz (layout swapped).
void fn_710059e4f0(UnkGooshockerState* state, bool on) {
    auto* self = reinterpret_cast<UnkGooshockerAI_b63a3*>(state);
    goto start;
    base::reflection::UnkReflTypeIdArg maskOn;
start:
    if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        if (on) {
            maskOn.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a060), &maskOn.id, false, -1.0f);
        } else {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a068), &mask.id, false, -1.0f);
        }
    }
}
