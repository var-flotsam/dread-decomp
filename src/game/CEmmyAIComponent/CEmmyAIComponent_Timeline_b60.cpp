// CEmmyAIComponent's unit (tools/tu_map.py): once a pending flag (+0x2a01c) is cleared, sets the
// id g_unk_7101649de0 to the all-flags mask on two timelines (+0x2e8): the one of the game
// manager's fn_7100defcd4 object and the one of the actor at the manager's +0x29a0 +0x10 (the set
// as src/game/CShelmitAIComponent/CShelmitAIComponent_Timeline_b59.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkOwner;

struct UnkEmmyKey_b60;
extern UnkEmmyKey_b60 g_unk_7101649de0 __attribute__((visibility("hidden")));

struct UnkEmmyLink_b60 {
    u8 pad_0x0[0x10];
    UnkActor* actor;
};

struct UnkEmmyAI_b60 {
    u8 pad_0x0[0x2a01c];
    bool pending;
};

void fn_7100541fd4(UnkEmmyAI_b60* self) {
    if (!self->pending)
        return;
    self->pending = false;
    base::reflection::UnkReflTypeIdArg mask;
    base::reflection::UnkReflTypeIdArg mask2;
    if (auto* actor = reinterpret_cast<UnkActor*>(fn_7100defcd4(g_unk_7101d47310))) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(actor->field_0x2e8)) {
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649de0), &mask.id, false, -1.0f);
        }
    }
    auto* link = *reinterpret_cast<UnkEmmyLink_b60**>(reinterpret_cast<char*>(g_unk_7101d47310) + 0x29a0);
    if (link && link->actor) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(link->actor->field_0x2e8)) {
            mask2.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_7101649de0), &mask2.id, false, -1.0f);
        }
    }
}
