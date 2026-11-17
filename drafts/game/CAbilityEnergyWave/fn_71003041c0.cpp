// CAbilityEnergyWave's unit (tools/tu_map.py): the update of the ability's charge (+0x3d8). While it
// runs and the owner may charge, the charge fills over the tunable's time, with the charge timeline keys,
// the charging sounds, the charging-loop rumble and the end-of-charge rumble; else it is dropped. Then the
// base update (fn_71002f8de4). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/snd/UnkSound.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAbilityLayout.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU39.h"
#include "game/UnkTU50.h"
#include "game/UnkTimelineCalls.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::math::CVector3D;

struct UnkShotManager_b0011;
UnkSoundHandle fn_7100ed6a10(UnkShotManager_b0011* self, u32 type, u32 variant);
bool fn_7100dadf20(UnkEmmyRegistry_b55* registry);
struct UnkSmAbility_A2_b63;
void fn_71002f8de4(UnkSmAbility_A2_b63* self, f32 dt);
void fn_7100306498(void* self, const s32* value);

struct UnkHyperBeamEffectRef;
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;
};

extern u64 g_unk_7101648d48;  // the charge-start timeline key
extern u64 g_unk_7101648d50;  // the charge-full timeline key

// The ability's tunables as far as the charge reads them.
struct UnkSprTunable_l3041c0 {
    u8 pad_0x0[0x30];
    f32 chargeTime;  // 0x30
};

struct UnkSprGun_l3041c0 {
    u8 pad_0x0[0x128];
    bool enabled;  // 0x128
};

struct UnkSprModeState_l3041c0 {
    u8 pad_0x0[0x10];
    u32 mode;  // 0x10
};

struct UnkSprModes_l3041c0 {
    u8 pad_0x0[0x50];
    UnkSprModeState_l3041c0* current;  // 0x50
};

struct UnkSprOwner_l3041c0 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x80];
    UnkSprGun_l3041c0* gun;      // 0x2a8
    UnkSprModes_l3041c0* modes;  // 0x2b0
    u8 pad_0x2b8[0x30];
    UnkTimelineComponent* timeline;  // 0x2e8
};

// A handle on an object that lives in a slot: valid while the slot still has the handle's id.
struct UnkSlotHandle_l3041c0 {
    struct Slot {
        void* object;
        u64 id;
    };
    Slot* slot;
    u64 id;

    void* Get() const { return slot && slot->id == id && id != (u64)-1 ? slot->object : nullptr; }
    bool IsValid() const { return Get() != nullptr; }
};

// What a rumble is played from (the effect description of fn_7100d7c614).
struct UnkSprRumbleDesc_l3041c0 {
    CFilePathStrId path;
    u64 field_0x8;
    u32 field_0x10;
    CVector3D field_0x14;
    f32 field_0x20[4];
    f32 field_0x30;
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    bool field_0x40;
    bool field_0x41;

    UnkSprRumbleDesc_l3041c0()
        : path(g_unk_7101d470e8), field_0x8(0), field_0x10(0x7fffffff), field_0x14(g_unk_710164779c),
          field_0x20{0.0f, 0.0f, 1.0f, 1.0f}, field_0x30(0.5f), field_0x34(0.0f), field_0x38(0.0f),
          field_0x3c(1.0f), field_0x40(false), field_0x41(false) {}
};

struct UnkEnergyWaveSpr_l3041c0 : CAbility {
    u8 pad_0x390[0x48];
    f32 charge;         // 0x3d8
    bool resetCharge;   // 0x3dc
    u8 pad_0x3dd[0x3];
    UnkSlotHandle_l3041c0 sound;    // 0x3e0
    UnkSlotHandle_l3041c0 rumble;   // 0x3f0
    UnkSlotHandle_l3041c0 rumble2;  // 0x400
    u8 chargeListeners[0x80];  // 0x410
    u8 modeListeners[0x80];    // 0x490
    u8 stateListeners[0x80];   // 0x510

    UnkSprOwner_l3041c0* Owner() const { return OwnerOf<UnkSprOwner_l3041c0>(component); }

    void NotifyCharge() { fn_710003152c(chargeListeners, &charge); }

    void StopSound() {
        if (sound.IsValid()) {
            fn_7100105a80(static_cast<UnkSound*>(sound.Get()), 0.2f, 0.0f);
            sound = reinterpret_cast<const UnkSlotHandle_l3041c0&>(g_unk_7108b6c460);
        }
    }

    static void StopRumble(UnkSlotHandle_l3041c0& handle) {
        if (handle.IsValid()) {
            fn_7100072328(handle.Get(), 0.2f, 0.0f);
            handle = reinterpret_cast<const UnkSlotHandle_l3041c0&>(g_unk_7101d586c0);
        }
    }
};
static_assert(__builtin_offsetof(UnkEnergyWaveSpr_l3041c0, charge) == 0x3d8, "layout");
static_assert(__builtin_offsetof(UnkEnergyWaveSpr_l3041c0, stateListeners) == 0x510, "layout");

static inline UnkShotManager_b0011* Sounds() { return static_cast<UnkShotManager_b0011*>(g_unk_71726af0b0); }

static inline bool IsAboutZero(f32 value, f32 tolerance) {
    return value - tolerance <= 0.0f && value + tolerance >= 0.0f;
}

void fn_71003041c0(UnkEnergyWaveSpr_l3041c0* self, f32 dt) {
    UnkSprOwner_l3041c0* owner = self->Owner();
    bool running = static_cast<u32>(self->State()) > 1;
    bool sprMode = self->State() == 6;
    if (self->resetCharge) {
        self->charge = 0.0f;
        self->NotifyCharge();
    }
    self->resetCharge = false;

    bool gunEnabled = owner && owner->gun && owner->gun->enabled;
    s32 mode = 0;
    if (self->State() == 6)
        mode = gunEnabled ? 1 : 2;
    fn_7100306498(self->stateListeners, &mode);
    fn_71000303c0(reinterpret_cast<base::UnkDeferredSet<base::UnkDeferredCallback<bool>>*>(self->modeListeners),
                  &sprMode);

    f32 previous = self->charge;
    if (running) {
        UnkAnimationComponent* animation = owner->animation;
        if ((animation && fn_710117ae38(animation, 0x2376ca3f9e17e420, false)) || !sprMode ||
            (gunEnabled && !fn_710117b348(self->Owner()->animation, fn_7100c3ce68()))) {
            if (self->charge != 0.0f) {
                self->charge = 0.0f;
                self->NotifyCharge();
                fn_7100ed6af0(Sounds(), 0xb, 1);
                fn_7100ed6af0(Sounds(), 0xb, 2);
                UnkEnergyWaveSpr_l3041c0::StopRumble(self->rumble);
                UnkEnergyWaveSpr_l3041c0::StopRumble(self->rumble2);
            }
        } else if (owner->modes && owner->modes->current && owner->modes->current->mode == 2 &&
                   fn_7100dadf20(g_unk_71726af0a8) && self->charge < 1.0f) {
            UnkTimelineComponent* timeline = self->Owner()->timeline;
            if (timeline && IsAboutZero(self->charge, 0.001f)) {
                base::reflection::UnkReflTypeIdArg mask;
                mask.id = UnkAll_SpitSend().value;
                fn_7100a2cb7c(timeline, &g_unk_7101648d48, &mask.id, false, -1.0f);
            }
            self->charge += dt / reinterpret_cast<UnkSprTunable_l3041c0*>(&g_unk_71726af060)->chargeTime;
            if (self->charge > 1.0f) {
                self->charge = 1.0f;
                if (timeline) {
                    base::reflection::UnkReflTypeIdArg mask;
                    mask.id = UnkAll_SpitSend().value;
                    fn_7100a2cb7c(timeline, &g_unk_7101648d50, &mask.id, false, -1.0f);
                }
                fn_7100ed6a10(Sounds(), 0xb, 1);
                fn_7100ed6a10(Sounds(), 0xb, 2);
            }
            self->NotifyCharge();
        }
    } else {
        self->charge = 0.0f;
        self->NotifyCharge();
    }

    if (self->charge == 1.0f) {
        self->StopSound();
        fn_7100ed6af0(Sounds(), 0xb, 0);
        UnkEnergyWaveSpr_l3041c0::StopRumble(self->rumble);
        if (self->charge != previous) {
            UnkSprRumbleDesc_l3041c0 desc;
            static CFilePathStrId s_endCharge("weapons/spr_endcharge.nvib", true);
            desc.path = s_endCharge;
            desc.field_0x3c = 1.0f;
            desc.field_0x30 = 0.5f;
            desc.field_0x40 = true;
            desc.field_0x10 = 0;
            desc.field_0x20[0] = 5000.0f;
            desc.field_0x20[1] = 500.0f;
            desc.field_0x20[2] = 1.0f;
            desc.field_0x20[3] = 1.0f;
            fn_7100d7c614(reinterpret_cast<UnkActor*>(owner), reinterpret_cast<const UnkHyperBeamEffectDesc*>(&desc),
                          &g_unk_7101d470e0, 0, 1, 1);
        }
    } else {
        void* playing = self->sound.Get();
        if (self->charge > 0.0f) {
        if (!playing) {
            UnkSoundHandle sound = fn_7100ed6a10(Sounds(), 0xb, 0);
            self->sound.slot = reinterpret_cast<UnkSlotHandle_l3041c0::Slot*>(sound.ref);
            self->sound.id = sound.serial;
        }
        if (!self->rumble.IsValid()) {
            UnkSprRumbleDesc_l3041c0 desc;
            static CFilePathStrId s_chargingLoop("weapons/chargingloop_spr.nvib", true);
            desc.path = s_chargingLoop;
            desc.field_0x3c = 1.0f;
            desc.field_0x30 = 0.5f;
            desc.field_0x40 = true;
            desc.field_0x41 = false;
            desc.field_0x10 = 0;
            desc.field_0x20[0] = 0.0f;
            desc.field_0x20[1] = 1.0f;
            desc.field_0x20[2] = 0.6f;
            desc.field_0x20[3] = 1.0f;
            UnkHyperBeamEffectHandle rumble =
                fn_7100d7c614(reinterpret_cast<UnkActor*>(owner), reinterpret_cast<const UnkHyperBeamEffectDesc*>(&desc),
                              &g_unk_7101d470e0, 0, 1, 1);
            self->rumble.slot = reinterpret_cast<UnkSlotHandle_l3041c0::Slot*>(rumble.ref);
            self->rumble.id = rumble.serial;
        }
        } else {
            if (playing) {
                fn_7100105a80(static_cast<UnkSound*>(self->sound.Get()), 0.2f, 0.0f);
                self->sound = reinterpret_cast<const UnkSlotHandle_l3041c0&>(g_unk_7108b6c460);
            }
            fn_7100ed6af0(Sounds(), 0xb, 0);
            UnkEnergyWaveSpr_l3041c0::StopRumble(self->rumble);
        }
    }
    fn_71002f8de4(reinterpret_cast<UnkSmAbility_A2_b63*>(self), dt);
}
