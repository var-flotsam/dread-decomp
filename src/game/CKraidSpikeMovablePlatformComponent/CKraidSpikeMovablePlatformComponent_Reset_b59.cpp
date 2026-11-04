// CKraidSpikeMovablePlatformComponent: clears the timer at +0x280, keeps the owner's (+0x30) position
// at +0x270 and, when the owner's timeline (+0x2e8) knows the id g_unk_710164a2c0, sets that id to
// the all-flags mask (as the start, CKraidSpikeMovablePlatformComponent_Start_b59.cpp). Names and
// layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineKey_b59a4b;
extern UnkTimelineKey_b59a4b g_unk_710164a2c0;  // reached through the GOT

struct UnkSpikePlatformReset_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x270 - 0x38];
    f32 field_0x270;
    f32 field_0x274;
    f32 field_0x278;
    u8 pad_0x27c[4];
    f32 field_0x280;
};

void fn_710060887c(UnkSpikePlatformReset_b59a4* self) {
    UnkActor* owner = self->owner;
    self->field_0x280 = 0.0f;
    f32 x = owner->field_0x44;
    f32 y = owner->field_0x54;
    f32 z = owner->field_0x64;
    self->field_0x270 = x;
    self->field_0x274 = y;
    self->field_0x278 = z;
    const u64* key = reinterpret_cast<const u64*>(&g_unk_710164a2c0);
    auto* timeline = static_cast<UnkTimelineComponent*>(owner->field_0x2e8);
    if (timeline && fn_7100a2c13c(timeline, key)) {
        auto* t = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(t, key, &mask.id, false, -1.0f);
    }
}
