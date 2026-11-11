// CShakernautAIComponent's unit (tools/tu_map.py): a message handler override. After the base handler
// (fn_7101111dc8), a message with its flag at +0x20 set, when the owner (+0x30) has a timeline
// (+0x2e8) and its character class (+0x220) is missing or does not hold the track
// 0x6fbe6074ed7fc71d, sets the id g_unk_710164a6b0 to the all-flags mask (as
// src/game/CKraidAIComponent/fn_7100600c98.cpp). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2702.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkNailongAI_b46a2;
struct UnkNailongMsg_b46a2;

struct UnkShakernautKey_b59a4;
extern UnkShakernautKey_b59a4 g_unk_710164a6b0;  // reached through the GOT

struct UnkShakernautAI_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

struct UnkShakernautMsg_b59a4 {
    u8 pad_0x0[0x20];
    bool field_0x20;
};

u32 fn_71006b1fbc(UnkShakernautAI_b59a4* self, UnkShakernautMsg_b59a4* msg) {
    u32 result =
        fn_7101111dc8(reinterpret_cast<UnkNailongAI_b46a2*>(self), reinterpret_cast<UnkNailongMsg_b46a2*>(msg));
    if (msg->field_0x20) {
        auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        if (timeline) {
            auto* anim = static_cast<UnkAnimationComponent*>(self->owner->field_0x220);
            if (!anim || !fn_710117ae38(anim, 0x6fbe6074ed7fc71d, false)) {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a6b0), &mask.id, false, -1.0f);
            }
        }
    }
    return result;
}
