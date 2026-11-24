// Unit 1428 (CMorphBallMovement hint): updates the rolling sound of the mode in use: on the first call
// with no sound playing, stops and forgets all four mode sounds; while the sound plays, fades its
// volume and pitch down over time (or sets them back) and moves it to the owner. Names and layouts are
// placeholders.
#include "game/UnkTU101.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

// The playing sound as this unit sees it (its fades are the interpolators fn_7100105d34 and
// fn_7100105da8 drive).
struct UnkRollSound_93fab8 {
    u8 pad_0x0[0x8];
    f32 level;  // 0x8
    u8 pad_0xc[0x38];
    f32 pitch;  // 0x44
    u8 pad_0x48[0xa4];
    f32 volume;  // 0xec
};

// The sound manager's reference block of a handle.
struct UnkSoundRef {
    UnkRollSound_93fab8* sound;
    s64 serial;
};

inline UnkRollSound_93fab8* SoundOf_93fab8(const UnkSoundHandle& handle) {
    if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
        return handle.ref->sound;
    return nullptr;
}

struct UnkSoundPosition_93fab8 {
    f32 x;
    f32 y;
    f32 z;
};

// The owner's sound component (fn_7100d7c558 stops a handle's sound through it) and its position.
struct UnkRollSoundOwner_93fab8 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x250];
    void* sounds;  // 0x2b8
};

struct UnkMorphBallMovement_93fab8 {
    u8 pad_0x0[0x30];
    UnkRollSoundOwner_93fab8* owner;  // 0x30
    u8 pad_0x38[0x2250];
    u32 mode;  // 0x2288
    u8 pad_0x228c[0x94];
    UnkSoundHandle sound_default;  // 0x2320
    UnkSoundHandle sound_mode2;    // 0x2330
    u8 pad_0x2340[0x10];
    UnkSoundHandle sound_mode4;      // 0x2350
    UnkSoundHandle sound_mode4_alt;  // 0x2360
    u8 pad_0x2370[0x1dc];
    bool use_alt;  // 0x254c
};
static_assert(__builtin_offsetof(UnkMorphBallMovement_93fab8, use_alt) == 0x254c, "use_alt");

// Stops the handle's sound with a fade (the definition is generated: src/lifted/).
void fn_7100d7c558(UnkRollSoundOwner_93fab8* owner, UnkSoundHandle* handle, f32 fade, f32 delay);
// Moves the sound to a position (the definition is generated: src/lifted/).
void fn_7100105ebc(UnkRollSound_93fab8* sound, const UnkSoundPosition_93fab8* position);

void fn_710093fab8(UnkMorphBallMovement_93fab8* self, f32 dt, bool rolling) {
    UnkSoundHandle handle = g_unk_7108b6c460;
    switch (self->mode) {
    case 4:
        handle = !self->use_alt ? self->sound_mode4 : self->sound_mode4_alt;
        break;
    case 2:
        handle = self->sound_mode2;
        break;
    default:
        handle = self->sound_default;
        break;
    }

    if (self->owner->sounds && (!rolling || !SoundOf_93fab8(handle))) {
        fn_7100d7c558(self->owner, &self->sound_mode4_alt, 0.0f, 0.0f);
        fn_7100d7c558(self->owner, &self->sound_mode4, 0.0f, 0.0f);
        fn_7100d7c558(self->owner, &self->sound_mode2, 0.0f, 0.0f);
        fn_7100d7c558(self->owner, &self->sound_default, 0.0f, 0.0f);
        self->sound_default = g_unk_7108b6c460;
        self->sound_mode2 = g_unk_7108b6c460;
        self->sound_mode4 = g_unk_7108b6c460;
        self->sound_mode4_alt = g_unk_7108b6c460;
    }

    UnkRollSound_93fab8* sound = SoundOf_93fab8(handle);
    if (!sound || !(sound->level > 0.0f))
        return;

    f32 volume;
    f32 pitch;
    if (rolling) {
        f32 v = sound->volume + dt * -8.0f;
        volume = !(v <= 0.0f) ? (!(v >= 0.4f) ? v : 0.4f) : 0.0f;
        fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), volume, false, 0.2f);
        f32 p = SoundOf_93fab8(handle)->pitch + dt * -12.0f;
        if (p <= 0.3f)
            pitch = 0.3f;
        else
            pitch = !(p >= 1.1f) ? p : 1.1f;
    } else {
        volume = 0.0f;
        fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(sound), volume, false, 0.2f);
        pitch = 0.4f;
    }
    SetSoundVoicePitchOverDuration(reinterpret_cast<UnkInterp_7100105d34*>(SoundOf_93fab8(handle)), pitch, 0.2f);

    UnkRollSound_93fab8* playing = SoundOf_93fab8(handle);
    UnkRollSoundOwner_93fab8* owner = self->owner;
    UnkSoundPosition_93fab8 position = {owner->x, owner->y, owner->z};
    fn_7100105ebc(playing, &position);
    if (!(volume > 0.0f))
        fn_7100d7c558(self->owner, &handle, 0.2f, 0.0f);
}
