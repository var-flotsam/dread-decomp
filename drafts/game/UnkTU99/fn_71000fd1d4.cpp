// Unit 99 (the sound system's, reflected as base::snd::CSoundSystemATK::CTunableSoundSystemATK): the
// setters that apply a value to every voice of one of the sound system's two voice lists whose group
// bit is in a mask (fn_71000fea74 and its siblings), the effect-slot setters (fn_7100100734,
// fn_71000fb9f4), a value moving towards a target (fn_71000fbe1c, fn_71000f5260) and the frame-time
// accumulator (fn_71000f6770). Names and layouts are placeholders.
#include <nn/os.h>
#include <nn/util.h>

#include "base/UnkDefaultAllocator.h"
#include "base/snd/UnkSound.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::snd::UnkSoundSystemATK;

bool fn_7100132cd0();
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));

struct UnkInterp_7100105d34;
void fn_7100105d34(UnkInterp_7100105d34*, f32, bool, f32);
struct UnkPlayingSound_ovb0074;
void fn_7100105bac(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105c00(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105c9c(UnkPlayingSound_ovb0074* sound, f32 value);
void fn_7100105cf0(UnkPlayingSound_ovb0074* sound, f32 value);

// A voice: the group bit it answers to is 1 << shift.
struct UnkVoice_b0184 {
    u8 pad_0x0[0xb0];
    u32 shift;  // 0xb0
};

// The members of the sound system these functions touch: two lists of voices and the mutex that
// guards them.
struct UnkSystem_b0184 {
    u8 pad_0x0[0xe8];
    u32 field_0xe8;
    u8 pad_0xec[0x3f0 - 0xec];
    UnkVoice_b0184** list_0x3f0;
    u32 count_0x3f8;
    u8 pad_0x3fc[0x1c];
    UnkVoice_b0184** list_0x418;
    u32 count_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType mutex;  // 0x400568
};

// Applies a fade to the first list's voices in mask.
void fn_71000fd1d4(void* member, const u32* mask, f32 fade) {
    UnkSystem_b0184* self = reinterpret_cast<UnkSystem_b0184*>(member);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    for (UnkVoice_b0184 **it = self->list_0x3f0, **end = it + self->count_0x3f8; it != end; ++it) {
        u32 shift = (*it)->shift;
        if (*mask & (1u << shift))
            fn_7100105d34(reinterpret_cast<UnkInterp_7100105d34*>(*it), 0.0f, false, fade);
    }
    fn_7100066920(&self->mutex);
}

