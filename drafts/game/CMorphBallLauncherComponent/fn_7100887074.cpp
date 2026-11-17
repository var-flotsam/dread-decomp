// CMorphBallLauncherComponent's unit: drops the two held references (+0x310, +0x318), sends the owner's
// animation (+0x220) one request, then, unless the flag at +0x300 is set, a second one and sets the track's
// speed from +0x308; finally gives the owner's timeline one or two keys. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkDestructors.h"
#include "game/CAnimationComponent.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::UnkRefCounted;

__attribute__((visibility("hidden"))) extern const u64 g_unk_710164ae88;
__attribute__((visibility("hidden"))) extern const u64 g_unk_710164ae90;
__attribute__((visibility("hidden"))) extern const u64 g_unk_710164ae98;

struct UnkMblOwner_l887074 {
    u8 pad_0x0[0x220];
    void* animation;  // 0x220
};

struct UnkMblParams_l887074 {
    u8 pad_0x0[0x64];
    u32 field_0x64;
};

struct UnkMblTrack_l887074 {
    u8 pad_0x0[0x164];
    u32 field_0x164;
    bool field_0x168;
};

struct UnkMorphBallLauncher_l887074 {
    u8 pad_0x0[0x30];
    UnkMblOwner_l887074* owner;
    u8 pad_0x38[0x300 - 0x38];
    bool field_0x300;
    UnkMblParams_l887074* params;  // 0x308
    UnkRefCounted* field_0x310;
    UnkRefCounted* field_0x318;
};
static_assert(__builtin_offsetof(UnkMorphBallLauncher_l887074, field_0x318) == 0x318, "layout");

static inline void Release(UnkRefCounted*& ref) {
    if (ref) {
        if (--ref->ref_count == 0)
            delete ref;
        ref = nullptr;
    }
}

void fn_7100887074(UnkMorphBallLauncher_l887074* self, s32 kind) {
    Release(self->field_0x310);
    Release(self->field_0x318);

    // Every local at function scope: none of their stack slots is shared, as in the original.
    base::reflection::UnkReflTypeIdArg mode, mask, mode2, mask2;
    void* animation = self->owner->animation;
    mode.id = 0;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(animation, 0x6909d27d2c787fba, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740,
                  &mask);

    if (!self->field_0x300 || kind != 5) {
        const u64* key = &g_unk_710164ae90;
        if (!self->field_0x300) {
            const u64 kind2 = 0x71b15e29dc542a11;
            void* animation2 = self->owner->animation;
            mode2.id = 0;
            mask2.id = UnkAll_SpitSend().value;
            fn_7101176690(animation2, 0x190088acaa81dde2, kind2, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                          &mask2);
            UnkMblTrack_l887074* track =
                reinterpret_cast<UnkMblTrack_l887074*>(fn_71011771e0(self->owner->animation, kind2));
            track->field_0x164 = self->params->field_0x64;
            track->field_0x168 = true;
            key = &g_unk_710164ae88;
        }
        fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), key, false);
    }
    fn_7100d7c828(reinterpret_cast<UnkActor*>(self->owner), &g_unk_710164ae98, false);
}
