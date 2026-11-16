// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_71000ff4e4 releases the channels
// named by a bit mask, starting each named channel's fade towards level 0 over `time` and fading every voice
// of both voice lists on a named channel to level 0. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU104.h"
#include "game/UnkTU99.h"

struct UnkSoundHandle_b46a1 {
    u32 value;
    UnkSoundHandle_b46a1(const UnkSoundHandle_b46a1& other) : value(other.value) {}
};

struct UnkSndChannelFade_l0ff4e4 {
    s32 current;  // 0x0
    s32 from;     // 0x4
    s32 to;       // 0x8
    f32 time;     // 0xc
    f32 elapsed;  // 0x10

    void Start(s32 level, f32 duration) {
        s32 target = fn_710010aaa0(level);
        from = current;
        to = target;
        time = duration;
        elapsed = 0;
    }
};

struct UnkSndChannels_l0ff4e4 {
    u8 pad_0x0[0xf0];
    u32 mask;                            // 0xf0
    UnkSndChannelFade_l0ff4e4 fades[8];  // 0xf4
    u8 pad_0x194[0x3f0 - 0x194];
    base::global::CRntVector<UnkInterp_7100105f94*> voices_a;  // 0x3f0
    base::global::CRntVector<UnkInterp_7100105f94*> voices_b;  // 0x418
    u8 pad_0x440[0x400568 - 0x440];
    nn::os::MutexType mutex;  // 0x400568
};

struct UnkSndVoice_l0ff4e4 {
    u8 pad_0x0[0xb0];
    s32 channel;  // 0xb0
};

static inline s32 ChannelOf(UnkInterp_7100105f94* voice) {
    return fn_710010aae0(reinterpret_cast<UnkSndVoice_l0ff4e4*>(voice)->channel);
}

void fn_71000ff4e4(void* player, UnkSoundHandle_b46a1 handle, f32 time) {
    UnkSndChannels_l0ff4e4* self = static_cast<UnkSndChannels_l0ff4e4*>(player);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    const u32* bits = &handle.value;
    u32 mask = self->mask;
    self->mask = mask & ~*bits;
    if (*bits & 0x01)
        self->fades[0].Start(0, time);
    if (*bits & 0x02)
        self->fades[1].Start(0, time);
    if (*bits & 0x04)
        self->fades[2].Start(0, time);
    if (*bits & 0x08)
        self->fades[3].Start(0, time);
    if (*bits & 0x10)
        self->fades[4].Start(0, time);
    if (*bits & 0x20)
        self->fades[5].Start(0, time);
    if (*bits & 0x40)
        self->fades[6].Start(0, time);
    if (*bits & 0x80)
        self->fades[7].Start(0, time);
    UnkInterp_7100105f94** it = self->voices_a.data;
    UnkInterp_7100105f94** end = it + self->voices_a.count;
    for (; it != end; ++it) {
        UnkInterp_7100105f94* voice = *it;
        s32 channel = ChannelOf(voice);
        if (*bits & (1u << channel))
            fn_7100105f94(voice, 0, time);
    }
    it = self->voices_b.data;
    end = it + self->voices_b.count;
    for (; it != end; ++it) {
        UnkInterp_7100105f94* voice = *it;
        s32 channel = ChannelOf(voice);
        if (*bits & (1u << channel))
            fn_7100105f94(voice, 0, time);
    }
    fn_7100066920(mutex);
}
