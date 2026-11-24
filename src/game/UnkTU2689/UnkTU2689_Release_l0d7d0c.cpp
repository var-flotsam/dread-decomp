// Unit 2689 (the player movement): fn_71010d7d0c ends the held state (+0x18b0): unregisters the item at
// +0x18d8 from the item list, unlocks the owner's input, plays the timeline id of `kind` (after telling a
// "frozen" animation to leave it) and removes the unit's key from g_unk_71726af0a8. Names and layouts are
// placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAnimationComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkTU2732.h"
#include "game/UnkTimelineCalls.h"
#include "types.h"

using base::global::CStrId;

struct UnkStruct_710117af10;
struct UnkCls_710117af10;
struct UnkDoorKeyOwner_dae8a4;
extern CStrId g_unk_7172a03e20 __attribute__((visibility("hidden")));
extern CStrId g_unk_7172a03e38 __attribute__((visibility("hidden")));

struct UnkMovementOwner_l0d7d0c {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x40];
    void* animation;  // 0x220
    u8 pad_0x228[0xc0];
    UnkTimelineComponent* timeline;  // 0x2e8
};

// The timeline ids the movement plays.
struct UnkMovementIds_l0d7d0c {
    u8 pad_0x0[0x78];
    CStrId release;  // 0x78
    u8 pad_0x80[0x8];
    CStrId release_b;  // 0x88
    CStrId release_a;  // 0x90
};

struct UnkPlayerMovementD_b0204 {
    u8 pad_0x0[0x30];
    UnkMovementOwner_l0d7d0c* owner;  // 0x30
    u8 pad_0x38[0x788];
    UnkMovementIds_l0d7d0c* ids;  // 0x7c0
    u8 pad_0x7c8[0x10e8];
    bool held;  // 0x18b0
    u8 pad_0x18b1[0x3];
    u32 field_0x18b4;
    u32 field_0x18b8;
    u8 pad_0x18bc[0x1c];
    u8 item[0x8];  // 0x18d8
};
static_assert(__builtin_offsetof(UnkPlayerMovementD_b0204, held) == 0x18b0, "layout");
static_assert(__builtin_offsetof(UnkPlayerMovementD_b0204, item) == 0x18d8, "layout");

void fn_71010d7d0c(UnkPlayerMovementD_b0204* self, u32 kind) {
    if (!self->held)
        return;
    self->held = false;
    self->field_0x18b4 = 0;
    self->field_0x18b8 = 0;
    if (!g_unk_7172729910) {
        void* list = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(list);
        g_unk_7172729910 = list;
    }
    fn_710119ae24(g_unk_7172729910, self->item, true);
    if (UnkInputComponent* input = self->owner->input)
        fn_7100872460(input, false, false, &g_unk_7172a03e20);
    // At function scope: none of their stack slots is shared, as in the original.
    alignas(8) u32 value;
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    u64 key;
    auto playTimeline = [&](const CStrId& id) {
        if (UnkTimelineComponent* timeline = self->owner->timeline) {
            key = id.data->Key();
            value = UnkAll_SpitSend().value;
            fn_7100a2cb7c(timeline, &key, &value, false, -1.0f);
        }
    };
    switch (kind) {
    case 0: {
        static const CStrId s_frozen("frozen", true);
        if (self->owner->animation &&
            CAnimationComponent_IsPlayingActionNameContaining(static_cast<UnkStruct_710117af10*>(self->owner->animation),
                          reinterpret_cast<UnkCls_710117af10**>(const_cast<CStrId*>(&s_frozen)), false)) {
            void* animation = self->owner->animation;
            mode.id = 2;
            mask.id = UnkAll_SpitSend().value;
            CAnimationComponent_SendActionToTrack(animation, 0x6a83a50c8f7f1ee6, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                          &g_unk_7172769740, &mask);
        }
        playTimeline(self->ids->release);
        break;
    }
    case 1:
        playTimeline(self->ids->release_a);
        break;
    default:
        if (kind == 2 || kind == 3)
            playTimeline(self->ids->release_b);
        break;
    }
    CTunableAIManager_AddOrRemoveDoorCollisionFlagKey(reinterpret_cast<UnkDoorKeyOwner_dae8a4*>(g_unk_71726af0a8), false, &g_unk_7172a03e38);
}
