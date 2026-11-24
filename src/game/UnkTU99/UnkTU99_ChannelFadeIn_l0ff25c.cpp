// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_71000ff25c masks the channels named by a bit
// mask, starting each named channel's fade towards `level` over `time` and fading every voice of both voice
// lists on a named channel to that level. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/tunable/CTunable.h"
#include "game/UnkSndChannelFade_l0ff4e4.h"
#include "game/UnkSndChannels_l0ff4e4.h"
#include "game/UnkSndVoice_l0ff4e4.h"
#include "game/UnkSoundHandle_b46a1.h"
#include "game/UnkTU101.h"
#include "game/UnkTU104.h"
#include "game/UnkTU99.h"

static inline s32 ChannelOf(UnkInterp_7100105f94* voice) {
    return fn_710010aae0(reinterpret_cast<UnkSndVoice_l0ff4e4*>(voice)->channel);
}

void fn_71000ff25c(void* player, UnkSoundHandle_b46a1 handle, s32 level, f32 time) {
    UnkSndChannels_l0ff4e4* self = static_cast<UnkSndChannels_l0ff4e4*>(player);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    const u32* bits = &handle.value;
    u32 mask = self->mask;
    self->mask = *bits | mask;
    if (*bits & 0x01)
        self->fades[0].Start(level, time);
    if (*bits & 0x02)
        self->fades[1].Start(level, time);
    if (*bits & 0x04)
        self->fades[2].Start(level, time);
    if (*bits & 0x08)
        self->fades[3].Start(level, time);
    if (*bits & 0x10)
        self->fades[4].Start(level, time);
    if (*bits & 0x20)
        self->fades[5].Start(level, time);
    if (*bits & 0x40)
        self->fades[6].Start(level, time);
    if (*bits & 0x80)
        self->fades[7].Start(level, time);
    UnkInterp_7100105f94** it = self->voices_a.data;
    UnkInterp_7100105f94** end = it + self->voices_a.count;
    for (; it != end; ++it) {
        UnkInterp_7100105f94* voice = *it;
        s32 channel = ChannelOf(voice);
        if (*bits & (1u << channel))
            SetSoundLevelOverTime(voice, level, time);
    }
    it = self->voices_b.data;
    end = it + self->voices_b.count;
    for (; it != end; ++it) {
        UnkInterp_7100105f94* voice = *it;
        s32 channel = ChannelOf(voice);
        if (*bits & (1u << channel))
            SetSoundLevelOverTime(voice, level, time);
    }
    fn_7100066920(mutex);
}
