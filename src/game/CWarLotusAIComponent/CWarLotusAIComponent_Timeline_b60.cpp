// CWarLotusAIComponent's unit (tools/tu_map.py): picks one of two timeline ids. When the owner's (+0x30)
// timeline (+0x2e8) does not know the first hashed entry, g_unk_710164a818 is set to the all-flags mask;
// otherwise, when it does not know the second one, g_unk_710164a820 is (as
// src/game/CXParasiteAIComponent.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkWarLotusKey_b60a4;
extern UnkWarLotusKey_b60a4 g_unk_710164a818;  // reached through the GOT
extern UnkWarLotusKey_b60a4 g_unk_710164a820;  // reached through the GOT

struct UnkWarLotusTimeline_b60a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

void fn_71006e0acc(UnkWarLotusTimeline_b60a4* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline)
        return;
    // One stack slot holds the hash and then the mask.
    union {
        u64 hash;
        base::reflection::UnkReflTypeIdArg mask;
    } v;
    v.hash = 0xfa26b9beb0699ae3ULL;
    if (!fn_7100a2be94(timeline, &v.hash)) {
        v.mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a818), &v.mask.id, false, -1.0f);
    } else {
        v.hash = 0xd1e64d791178b452ULL;
        if (fn_7100a2be94(timeline, &v.hash))
            return;
        v.mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(&g_unk_710164a820), &v.mask.id, false, -1.0f);
    }
}
