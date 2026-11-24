// UnkTU1428 (CMorphBallMovement's unit): the ball's rolling sounds for one step. Two states play a fixed
// sound of their own (started on first need); the others keep the material's rolling sound in step with the
// ground. The sound then fades in with the step and follows the owner. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/snd/UnkSound.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2523.h"
#include "game/sound/UnkSoundDesc.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkEventOwner_m_ovb0137;
struct UnkEntityWithSound;

extern const CFilePathStrId g_unk_7172739478 __attribute__((visibility("hidden")));
extern const CFilePathStrId g_unk_7172739480 __attribute__((visibility("hidden")));
extern const CFilePathStrId g_unk_7172739488 __attribute__((visibility("hidden")));

void fn_7100105ebc(char* p0, char* p1);

struct UnkRollSound_l945d00 {
    u8 pad_0x0[0x44];
    f32 pitch;  // 0x44
    u8 pad_0x48[0xec - 0x48];
    f32 volume;  // 0xec
    u8 pad_0xf0[0x11a - 0xf0];
    bool fading;  // 0x11a
};

struct UnkRollSoundRef_l945d00 {
    UnkRollSound_l945d00* sound;
    s64 serial;
};

struct UnkRollSoundHandle_l945d00 {
    UnkRollSoundRef_l945d00* ref;
    s64 serial;

    UnkRollSound_l945d00* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
    bool IsPlaying() const { return Get() != nullptr; }
};

struct UnkRollOwner_l945d00 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkRollPosition_l945d00 {
    f32 x, y, z;

    UnkRollPosition_l945d00(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}
};

struct UnkRollMovement_l945d00 {
    u8 pad_0x0[0x30];
    UnkRollOwner_l945d00* owner;  // 0x30
    u8 pad_0x38[0xfa - 0x38];
    bool field_0xfa;
    u8 pad_0xfb[0x7e4 - 0xfb];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2288 - 0x7e5];
    s32 state;  // 0x2288
    u8 pad_0x228c[0x2320 - 0x228c];
    UnkRollSoundHandle_l945d00 sound;  // 0x2320: the material's rolling sound
    UnkRollSoundHandle_l945d00 field_0x2330;
    u8 pad_0x2340[0x10];
    UnkRollSoundHandle_l945d00 field_0x2350;
    UnkRollSoundHandle_l945d00 field_0x2360;
    u8 preset[0x18];  // 0x2370
    u32 material;     // 0x2388
    u8 pad_0x238c[0x254c - 0x238c];
    bool field_0x254c;
};

static inline u32 MaterialOf(void* source) {
    return reinterpret_cast<u32 (*)(void*)>(&fn_7100798e88)(source);
}

static inline UnkRollSoundHandle_l945d00 Play(UnkRollOwner_l945d00* owner, void* preset, void* params, u32 material) {
    using PlayFn = UnkRollSoundHandle_l945d00 (*)(UnkRollOwner_l945d00*, void*, void*, u32);
    return reinterpret_cast<PlayFn>(&fn_7100d7c45c)(owner, preset, params, material);
}

static inline UnkRollSoundHandle_l945d00 Play(UnkRollOwner_l945d00* owner, UnkSoundDesc* desc) {
    using PlayFn = UnkRollSoundHandle_l945d00 (*)(UnkRollOwner_l945d00*, void*, void*, bool, bool, bool);
    return reinterpret_cast<PlayFn>(&fn_7100d7c374)(owner, desc, &g_unk_7101d470e0, true, true, true);
}

// Held between 0 and `max`.
static inline f32 Clamp(f32 v, f32 max) {
    f32 held = 0.0f;
    if (!(v <= 0.0f))
        held = !(v >= max) ? v : max;
    return held;
}

void fn_7100945d00(UnkRollMovement_l945d00* self, f32 dt) {
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.kind.field_0x4 = 9;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 5000.0f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;

    UnkRollSound_l945d00* sound;
    bool rolling;
    switch (self->state) {
    case 4:
        if (self->field_0x254c) {
            if (!self->field_0x2360.IsPlaying()) {
                s_desc.path = g_unk_7172739478;
                s_desc.field_0x38 = 1.0f;
                s_desc.field_0x44 = 1.3f;
                self->field_0x2360 = Play(self->owner, &s_desc);
            }
            sound = self->field_0x2360.Get();
            rolling = true;
        } else {
            if (!self->field_0x2350.IsPlaying()) {
                s_desc.path = g_unk_7172739480;
                s_desc.field_0x38 = 1.0f;
                s_desc.field_0x44 = 1.0f;
                self->field_0x2350 = Play(self->owner, &s_desc);
            }
            sound = self->field_0x2350.Get();
            rolling = false;
        }
        break;
    case 2:
        if (!self->field_0x2330.IsPlaying()) {
            s_desc.path = g_unk_7172739488;
            s_desc.field_0x38 = 0.6f;
            s_desc.field_0x44 = 0.1f;
            self->field_0x2330 = Play(self->owner, &s_desc);
        }
        sound = self->field_0x2330.Get();
        rolling = true;
        break;
    default: {
        bool stopped = !self->sound.IsPlaying();
        void* source = fn_710094c9ac(reinterpret_cast<UnkEventOwner_m_ovb0137*>(self->owner));
        if (!stopped) {
            if (source) {
                u32 material = MaterialOf(source);
                if (material != self->material) {
                    f32 volume = self->sound.Get()->volume;
                    f32 pitch = self->sound.Get()->pitch;
                    fn_7100105a80(reinterpret_cast<UnkSound*>(self->sound.Get()), 0.01f, 0.0f);
                    self->material = material;
                    self->sound = Play(self->owner, self->preset, nullptr, material);
                    if (UnkRollSound_l945d00* fresh = self->sound.Get()) {
                        fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(fresh), volume, false, 0.0f);
                        SetSoundVoicePitchOverDuration(reinterpret_cast<UnkInterp_7100105d34*>(self->sound.Get()), pitch, 0.0f);
                    }
                }
            }
        } else if (source) {
            self->material = MaterialOf(source);
            self->sound = Play(self->owner, self->preset, nullptr, MaterialOf(source));
        } else {
            self->sound = Play(self->owner, self->preset, nullptr, 0);
        }
        sound = self->sound.Get();
        rolling = true;
        break;
    }
    }
    if (!sound)
        return;

    if (sound->fading)
        fn_7100105cf0(reinterpret_cast<UnkPlayingSound_ovb0074*>(sound), self->field_0xfa ? 0.01f : 0.1f);
    f32 volume = 0.0f;
    if (!self->field_0x7e4)
        volume = Clamp(dt * 1.8f + sound->volume, 0.4f);
    fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), self->field_0xfa ? 0.4f : volume, false,
                  self->field_0xfa ? 0.01f : 0.1f);
    f32 pitch = rolling ? Clamp(dt * 1.8f + sound->pitch, 1.0f) : 1.0f;
    SetSoundVoicePitchOverDuration(reinterpret_cast<UnkInterp_7100105d34*>(sound), pitch, 0.2f);

    UnkRollOwner_l945d00* owner = self->owner;
    UnkRollPosition_l945d00 position(owner->x, owner->y, owner->z);
    fn_7100105ebc(reinterpret_cast<char*>(sound), reinterpret_cast<char*>(&position));
}
