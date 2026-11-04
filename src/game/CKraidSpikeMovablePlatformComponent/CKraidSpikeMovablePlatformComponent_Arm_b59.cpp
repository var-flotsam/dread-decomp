// CKraidSpikeMovablePlatformComponent: arms the platform once (flag at +0x268). When the owner's (+0x30)
// timeline (+0x2e8) knows the id, sets it to the all-flags mask (as the start,
// CKraidSpikeMovablePlatformComponent_Start_b59.cpp): g_unk_710164a2d0 when the platform's delay
// (+0x94 of the config at +0x250) is -1 (Kraid's default), else g_unk_710164a2d8. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkActor.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

struct UnkTimelineKey_b59a4c;
extern UnkTimelineKey_b59a4c g_unk_710164a2d0;  // reached through the GOT
extern UnkTimelineKey_b59a4c g_unk_710164a2d8;  // reached through the GOT

struct UnkSpikePlatformArmConfig_b59a4 {
    u8 pad_0x0[0x94];
    f32 field_0x94;
};

struct UnkSpikePlatformArm_b59a4 {
    u8 pad_0x0[0x30];
    UnkActor* owner;
    u8 pad_0x38[0x250 - 0x38];
    UnkSpikePlatformArmConfig_b59a4* config;
    u8 pad_0x258[0x10];
    bool field_0x268;
};

void fn_7100608674(UnkSpikePlatformArm_b59a4* self) {
    if (self->field_0x268)
        return;
    self->field_0x268 = true;
    // Declared past a goto so that they get no lifetime markers: the two masks keep their own slots.
    goto armed;
    base::reflection::UnkReflTypeIdArg mask_default;
    base::reflection::UnkReflTypeIdArg mask_own;
armed:
    f32 delay = self->config->field_0x94;
    auto* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
    if (delay == -1.0f) {
        const u64* key = reinterpret_cast<const u64*>(&g_unk_710164a2d0);
        if (timeline && fn_7100a2c13c(timeline, key)) {
            auto* t = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
            mask_default.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(t, key, &mask_default.id, false, -1.0f);
        }
    } else {
        const u64* key = reinterpret_cast<const u64*>(&g_unk_710164a2d8);
        if (timeline && fn_7100a2c13c(timeline, key)) {
            auto* t = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8);
            mask_own.id = UnkAll_SpitSend().value;
            fn_7100a2cb7c(t, key, &mask_own.id, false, -1.0f);
        }
    }
}
