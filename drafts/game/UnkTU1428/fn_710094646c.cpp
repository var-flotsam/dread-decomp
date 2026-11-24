// UnkTU1428 (CMorphBallMovement's unit): keeps the rolling sound in step with the ground's material,
// restarting it on the new material with the old one's settings, then sets its volume, pitch and position.
// Names and layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game/CCollisionMaterialCacheComponent.h"
#include "game/UnkTU101.h"
#include "game/UnkTU1428.h"
#include "game/UnkTU2523.h"
#include "types.h"

struct UnkEventOwner_m_ovb0137;
struct UnkEntityWithSound;

struct UnkRollSound_l94646c {
    u8 pad_0x0[0x44];
    f32 pitch;  // 0x44
    u8 pad_0x48[0xec - 0x48];
    f32 volume;  // 0xec
    u8 pad_0xf0[0x11a - 0xf0];
    bool fading;  // 0x11a
};

struct UnkRollSoundRef_l94646c {
    UnkRollSound_l94646c* sound;
    s64 serial;
};

struct UnkRollSoundHandle_l94646c {
    UnkRollSoundRef_l94646c* ref;
    s64 serial;

    UnkRollSound_l94646c* Get() const {
        if (ref && ref->serial == serial && serial != -1)
            return ref->sound;
        return nullptr;
    }
};

struct UnkRollOwner_l94646c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkRollPosition_l94646c {
    f32 x, y, z;
};

struct UnkRollMovement_l94646c {
    u8 pad_0x0[0x30];
    UnkRollOwner_l94646c* owner;  // 0x30
    u8 pad_0x38[0xfa - 0x38];
    bool field_0xfa;
    u8 pad_0xfb[0x7e4 - 0xfb];
    bool field_0x7e4;
    u8 pad_0x7e5[0x2320 - 0x7e5];
    UnkRollSoundHandle_l94646c sound;  // 0x2320
    u8 pad_0x2330[0x2370 - 0x2330];
    u8 preset[0x18];  // 0x2370
    u32 material;     // 0x2388
};

static inline u32 MaterialOf(void* source) {
    return reinterpret_cast<u32 (*)(void*)>(&fn_7100798e88)(source);
}

static inline UnkRollSoundHandle_l94646c Play(UnkRollOwner_l94646c* owner, void* preset, void* params, u32 material) {
    using PlayFn = UnkRollSoundHandle_l94646c (*)(UnkRollOwner_l94646c*, void*, void*, u32);
    return reinterpret_cast<PlayFn>(&fn_7100d7c45c)(owner, preset, params, material);
}

void fn_7100105ebc(char* p0, char* p1);

void fn_710094646c(UnkRollMovement_l94646c* self) {
    UnkRollSound_l94646c* sound = self->sound.Get();
    void* source = fn_710094c9ac(reinterpret_cast<UnkEventOwner_m_ovb0137*>(self->owner));
    if (sound) {
        if (source) {
            u32 material = MaterialOf(source);
            if (material != self->material) {
                f32 volume = self->sound.Get()->volume;
                f32 pitch = self->sound.Get()->pitch;
                fn_7100105a80(reinterpret_cast<UnkSound*>(self->sound.Get()), 0.01f, 0.0f);
                self->material = material;
                self->sound = Play(self->owner, self->preset, nullptr, material);
                if (UnkRollSound_l94646c* fresh = self->sound.Get()) {
                    fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(fresh), volume, false, 0.01f);
                    SetSoundVoicePitchOverDuration(reinterpret_cast<UnkInterp_7100105d34*>(self->sound.Get()), pitch, 0.01f);
                }
            }
        }
    } else if (source) {
        self->material = MaterialOf(source);
        self->sound = Play(self->owner, self->preset, nullptr, MaterialOf(source));
    } else {
        self->sound = Play(self->owner, self->preset, nullptr, 0);
    }

    UnkRollSound_l94646c* current = self->sound.Get();
    if (!current)
        return;
    if (current->fading)
        fn_7100105cf0(reinterpret_cast<UnkPlayingSound_ovb0074*>(current), self->field_0xfa ? 0.01f : 0.1f);
    bool muffled = self->field_0x7e4;
    fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(self->sound.Get()),
                  self->field_0xfa || !muffled ? 0.2f : 0.0f, false, self->field_0xfa ? 0.01f : 0.1f);

    f32 pitch = self->sound.ref->sound->pitch;
    if (pitch + 0.001f < 0.3f || pitch - 0.001f > 0.3f)
        SetSoundVoicePitchOverDuration(reinterpret_cast<UnkInterp_7100105d34*>(self->sound.Get()), 0.3f, 0.3f);

    UnkRollOwner_l94646c* owner = self->owner;
    UnkRollPosition_l94646c position = {owner->x, owner->y, owner->z};
    fn_7100105ebc(reinterpret_cast<char*>(self->sound.Get()), reinterpret_cast<char*>(&position));
}
