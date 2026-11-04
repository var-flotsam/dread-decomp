// CShelmitAIComponent's unit (tools/tu_map.py): two timeline setters with one body. When there is an
// owner (+0x30) with a timeline (+0x2e8), each sets its own fixed id (g_unk_710164a720 or
// g_unk_710164a728) to the all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp).
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

struct UnkShelmitKey_b59a4;
extern UnkShelmitKey_b59a4 g_unk_710164a720;  // reached through the GOT
extern UnkShelmitKey_b59a4 g_unk_710164a728;  // reached through the GOT

struct UnkShelmitTimeline_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
};

static inline void SetAll_b59a4(void* p, UnkShelmitKey_b59a4* key) {
    UnkShelmitTimeline_b59a4* self = static_cast<UnkShelmitTimeline_b59a4*>(p);
    if (self->owner) {
        if (auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, reinterpret_cast<const u64*>(key), &mask.id, false, -1.0f);
        }
    }
}

void fn_71006ba4fc(void* self) {
    SetAll_b59a4(self, &g_unk_710164a720);
}

void fn_71006ba61c(void* self) {
    SetAll_b59a4(self, &g_unk_710164a728);
}
