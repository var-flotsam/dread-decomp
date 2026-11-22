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
#include "game/UnkSystem_b0184.h"
#include "game/UnkTU99.h"
#include "game/UnkTU101.h"
#include "base/tunable/CTunable.h"
#include "game/UnkVoice_b0184.h"

using base::snd::UnkSoundSystemATK;


struct UnkInterp_7100105d34;
struct UnkPlayingSound_ovb0074;

// Applies a fade to the first list's voices in mask.
// NON_MATCHING(minor): the same instructions in another order or with other registers.
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
