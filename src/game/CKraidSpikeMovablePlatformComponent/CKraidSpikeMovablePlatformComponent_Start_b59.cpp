// CKraidSpikeMovablePlatformComponent: starts the platform once. Unless the flag at +0x260 is set, sets it,
// clears the timer at +0x264 and, when the owner's (+0x30) timeline (+0x2e8) knows the id
// g_unk_710164a2c8, sets that id to the all-flags mask (as src/game/CKraidAIComponent/fn_7100600c98.cpp).
// Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineKey_b59a4;
extern UnkTimelineKey_b59a4 g_unk_710164a2c8;  // reached through the GOT

struct UnkSpikePlatformStart_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x260 - 0x38];
    bool field_0x260;
    u8 pad_0x261[3];
    f32 field_0x264;
};

void fn_7100608bb4(UnkSpikePlatformStart_b59a4* self) {
    if (self->field_0x260)
        return;
    self->field_0x260 = true;
    self->field_0x264 = 0.0f;
    const u64* key = reinterpret_cast<const u64*>(&g_unk_710164a2c8);
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (timeline && fn_7100a2c13c(timeline, key)) {
        auto* t = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(t, key, &mask.id, false, -1.0f);
    }
}
