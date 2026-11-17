// Unit 2689 (the player movement): fn_71010d80e8 ends the hold state (+0x18a4): clears its timers, unlocks the
// owner's input and plays the timeline ids of `kind` (or of the owner's dead state, which ends there), then
// removes the unit's key from g_unk_71726af0a8. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInputComponent.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct UnkPlayerMovementD_b0204;
struct UnkDoorKeyOwner_dae8a4;
extern const u64 g_unk_710164cef8;  // reached through the GOT
extern const u64 g_unk_710164cf00;  // reached through the GOT
extern const u64 g_unk_710164cf10;  // reached through the GOT
extern CStrId g_unk_7172a03e18 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a03e40 __attribute__((visibility("hidden")));

struct UnkUnholdLife_l0d80e8 {
    u8 pad_0x0[0x70];
    s32 state;  // 0x70
};

struct UnkUnholdOwner_l0d80e8 {
    u8 pad_0x0[0x1b8];
    UnkUnholdLife_l0d80e8* life;  // 0x1b8
    u8 pad_0x1c0[0x18];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x40];
    void* animation;  // 0x220
    u8 pad_0x228[0xc0];
    UnkTimelineComponent* timeline;  // 0x2e8
};

struct UnkUnholdMovement_l0d80e8 {
    u8 pad_0x0[0x30];
    UnkUnholdOwner_l0d80e8* owner;  // 0x30
    u8 pad_0x38[0x186c];
    bool holding;  // 0x18a4
    u8 pad_0x18a5[0x3];
    f32 field_0x18a8;
    f32 field_0x18ac;
};
static_assert(__builtin_offsetof(UnkUnholdMovement_l0d80e8, holding) == 0x18a4, "layout");

void fn_71010d80e8(UnkPlayerMovementD_b0204* movement, u32 kind) {
    auto* self = reinterpret_cast<UnkUnholdMovement_l0d80e8*>(movement);
    if (!self->holding)
        return;
    self->holding = false;
    self->field_0x18a8 = 0.0f;
    self->field_0x18ac = 0.0f;
    if (UnkInputComponent* input = self->owner->input)
        fn_7100872460(input, false, false, &g_unk_7172a03e18);
    // At function scope: none of their stack slots is shared, as in the original.
    base::reflection::UnkReflTypeIdArg value;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    // Sets the timeline value of `id` to the all-bits mask.
    auto playTimeline = [&](UnkTimelineComponent* timeline, const u64* id) {
        value.id = UnkAll_SpitSend().value;
        fn_7100a2cb7c(timeline, id, &value.id, false, -1.0f);
    };
    if (self->owner->life->state == 2) {
        fn_7100a2b7b8(self->owner->timeline, &g_unk_710164cf00);
        playTimeline(self->owner->timeline, &g_unk_710164cef8);
        return;
    }
    if (kind == 0) {
        if (UnkTimelineComponent* timeline = self->owner->timeline)
            playTimeline(timeline, &g_unk_710164cf10);
        if (void* animation = self->owner->animation) {
            mode.id = 2;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(animation, 0xde253bad2ff7f4ae, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
    } else if (kind == 2 || kind == 3) {
        if (UnkTimelineComponent* timeline = self->owner->timeline)
            playTimeline(timeline, &g_unk_710164cf10);
    } else if (kind == 1) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            fn_7100a2b7b8(timeline, &g_unk_710164cf00);
            playTimeline(self->owner->timeline, &g_unk_710164cef8);
        }
    }
    fn_7100dae8a4(reinterpret_cast<UnkDoorKeyOwner_dae8a4*>(g_unk_71726af0a8), false, &g_unk_7172a03e40);
}
