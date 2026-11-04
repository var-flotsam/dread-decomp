// CVulkranAIComponent's unit (tools/tu_map.py): two timeline setters with one body, as the pair in
// CVulkranAIComponent_Timeline_b59.cpp. When the owner's (+0x30) timeline (+0x2e8) and the object at
// +0x146c8 are both there, each sets the id its first getter returns for that object (fn_71006dabd8
// or fn_71006dabec) to the all-flags mask, and while the object's word at +0x6e0 is zero also the id
// of its second getter (fn_71006dac00 or fn_71006dac14). fn_71006dd25c is called from
// CVulkranAIComponent_b40.cpp. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

u64 fn_71006dabd8(void* self);
u64 fn_71006dabec(void* self);
u64 fn_71006dac00(void* self);
u64 fn_71006dac14(void* self);

struct UnkVulkranAIComponent;

struct UnkVulkranHolder_b59a4 {
    u8 pad_0x0[0x6e0];
    u32 field_0x6e0;
};

struct UnkVulkranTimeline2_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x146c8 - 0x38];
    UnkVulkranHolder_b59a4* field_0x146c8;
};

template <u64 (*First)(void*), u64 (*Second)(void*)>
static inline void SetAll2_b59a4(UnkVulkranTimeline2_b59a4* self) {
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (!timeline || !self->field_0x146c8)
        return;
    base::reflection::UnkReflTypeIdArg mask;
    u64 id = First(self->field_0x146c8);
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    if (self->field_0x146c8->field_0x6e0 == 0) {
        id = Second(self->field_0x146c8);
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, &id, &mask.id, false, -1.0f);
    }
}

void fn_71006dd0c0(UnkVulkranTimeline2_b59a4* self) {
    SetAll2_b59a4<fn_71006dabd8, fn_71006dac00>(self);
}

void fn_71006dd25c(UnkVulkranAIComponent* self) {
    SetAll2_b59a4<fn_71006dabec, fn_71006dac14>(reinterpret_cast<UnkVulkranTimeline2_b59a4*>(self));
}
