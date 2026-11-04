// CVulkranAIComponent's unit (tools/tu_map.py): two timeline setters with one body. When the owner's
// (+0x30) timeline (+0x2e8) and the object at +0x146c8 are both there, each sets the id its own
// getter returns for that object (fn_71006dac84 or fn_71006dac70) to the all-flags mask (as
// src/game/CKraidAIComponent/fn_7100600c98.cpp). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

u64 fn_71006dac70(void* self);
u64 fn_71006dac84(void* self);

struct UnkVulkranTimeline_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    void* field_0x146c8;
};

template <u64 (*Id)(void*)>
static inline void SetAll_b59a4(UnkVulkranTimeline_b59a4* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    void* holder;
    if (timeline && (holder = self->field_0x146c8)) {
        base::reflection::UnkReflTypeIdArg mask;
        u64 id = Id(holder);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
}

void fn_71006dcb94(UnkVulkranTimeline_b59a4* self) {
    SetAll_b59a4<fn_71006dac84>(self);
}

void fn_71006dcc90(UnkVulkranTimeline_b59a4* self) {
    SetAll_b59a4<fn_71006dac70>(self);
}
