// CHyperBeamGun's unit: stops the hyper beam: while charging, sends the stop to the owner's animation
// (unless the stop track plays already), switches the two beam objects off, clears the shot's timeline ids
// and sets the one of the gun's state, fades the charge sounds and the rumble out and tells the game
// manager (HYPER_BEAM_COMMANDERX). Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU134.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU50.h"
#include "game/UnkTimelineCalls.h"
#include "game/UnkTimelineComponent.h"
#include "types.h"

using base::global::CStrId;

struct CHyperBeamGunState_ovb0127;

// The timeline ids of the shot.
extern const u64 g_unk_710164acb0;
extern const u64 g_unk_710164acb8;
extern const u64 g_unk_710164acc0;
extern const u64 g_unk_710164ace0;
extern const u64 g_unk_710164ace8;
extern const u64 g_unk_710164acf0;

// A handle: the reference and the serial it was taken with; the object is gone once the reference's
// serial differs (or the handle's is -1).
template <class T>
struct UnkBeamStopHandle_l8458f8 {
    struct Ref {
        T* target;
        s64 serial;
    };
    Ref* ref;
    s64 serial;

    T* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->target;
        return nullptr;
    }
};

struct UnkBeamStopGun_l8458f8 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x1874 - 0x20];
    u32 field_0x1874;
    u8 pad_0x1878[0x1890 - 0x1878];
    UnkSwitchObject* field_0x1890;
    UnkSwitchObject* field_0x1898;
    u8 pad_0x18a0;
    bool field_0x18a1;
    u8 pad_0x18a2[6];
    UnkBeamStopHandle_l8458f8<UnkSound> field_0x18a8;
    u8 pad_0x18b8[0x20];
    UnkBeamStopHandle_l8458f8<UnkSound> field_0x18d8;
    UnkBeamStopHandle_l8458f8<void> field_0x18e8;
    bool charging;  // 0x18f8
    bool field_0x18f9;
    u8 pad_0x18fa[8];
    bool field_0x1902;
};
static_assert(__builtin_offsetof(UnkBeamStopGun_l8458f8, field_0x1874) == 0x1874, "layout");
static_assert(__builtin_offsetof(UnkBeamStopGun_l8458f8, field_0x18a8) == 0x18a8, "layout");
static_assert(__builtin_offsetof(UnkBeamStopGun_l8458f8, field_0x1902) == 0x1902, "layout");

// Sets the timeline value `id` to all flags (after a delay of -1).
static inline void SetAll(UnkTimelineComponent* timeline, const u64* id) {
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7100a2cb7c(timeline, id, &mask.id, false, -1.0f);
}

static inline void Clear(UnkTimelineComponent* timeline, const u64* id) {
    if (fn_7100a2be94(timeline, id))
        fn_7100a2b7b8(timeline, id);
}

static inline void SetUnlessSet(UnkTimelineComponent* timeline, const u64* id) {
    if (!fn_7100a2be94(timeline, id))
        SetAll(timeline, id);
}

void fn_71008458f8(CHyperBeamGunState_ovb0127* state, bool released) {
    UnkBeamStopGun_l8458f8* self = reinterpret_cast<UnkBeamStopGun_l8458f8*>(state);
    if (self->charging &&
        !fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x8b433509a5380892ull, false)) {
        if (fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0x33963557fd146e7aull,
                          false) ||
            fn_710117ae38(static_cast<UnkAnimationComponent*>(self->owner->field_0x220), 0xc5ab5dad085632f3ull,
                          false))
            UnkSpitSend(self->owner->field_0x220, 0xe0a181c51453ea91ull, 0xf4061ff25a2163d6ull);
        else
            UnkSpitSend(self->owner->field_0x220, 0x2b58fe4f8b7b9dfdull, 0xf4061ff25a2163d6ull);
    }
    self->field_0x18f9 = false;
    if (self->field_0x1890)
        fn_7100146dac(self->field_0x1890, false);
    if (self->field_0x1898)
        fn_7100145e94(self->field_0x1898, false);

    if (UnkTimelineComponent* timeline = static_cast<UnkTimelineComponent*>(self->owner->field_0x2e8)) {
        Clear(timeline, &g_unk_710164acc0);
        Clear(timeline, &g_unk_710164acb8);
        Clear(timeline, &g_unk_710164acb0);
        if (self->field_0x18a1) {
            SetAll(timeline, &g_unk_710164ace8);
        } else if (self->charging) {
            SetUnlessSet(timeline, &g_unk_710164ace0);
        } else if (released) {
            fn_7100a2b7b8(timeline, &g_unk_710164ace8);
            SetAll(timeline, &g_unk_710164ace8);
        } else if (self->field_0x1902) {
            SetUnlessSet(timeline, &g_unk_710164acf0);
        } else {
            SetUnlessSet(timeline, &g_unk_710164ace8);
        }
    }

    if (UnkSound* sound = self->field_0x18a8.Get())
        fn_7100105a80(sound, 0.2f, 0.1f);
    fn_7100ed6af0(g_unk_71726af0b0, self->field_0x1874, 3);
    {
        UnkGameManager* gm = g_unk_7101d47310;
        CStrId name("HYPER_BEAM_COMMANDERX", true);
        fn_7100e03a78(gm, &name);
    }
    if (UnkSound* sound = self->field_0x18d8.Get())
        fn_7100105a80(sound, 0.2f, 0.0f);
    if (void* rumble = self->field_0x18e8.Get())
        fn_7100072328(rumble, 0.2f, 0.0f);
}
