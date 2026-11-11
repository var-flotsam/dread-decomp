// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): fn_71000feff8 sets a level for the
// voices on the channels named by a bit mask. The mask's bits are added to the system's mask, each named
// channel's level is set to `id`, every voice of both voice lists on a named channel is faded to that
// level's value, and the sound thread is told (command 0x1f) while any channel is masked. Names and layouts
// are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"

using base::snd::UnkSoundSystemATK;

// fn_71000faac4 (include/base/snd/UnkSoundSystemATK.h) under a local name whose payload parameter is
// marked noescape, as src/base/snd/CSoundSystemATK/CSoundSystemATK_Setters.cpp does.
void QueueSoundCommand(UnkSoundSystemATK* self, s32 command, __attribute__((noescape)) const void* payload) asm(
    "_Z13fn_71000faac4PN4base3snd17UnkSoundSystemATKEiPKv");

struct UnkSoundHandle_b46a1 {
    u32 value;
    UnkSoundHandle_b46a1(const UnkSoundHandle_b46a1& other) : value(other.value) {}
};
extern const f32 g_unk_71016479f8[] __attribute__((visibility("hidden")));  // level values

struct UnkSndChannelVoice_o0301 {
    u8 pad_0x0[0xb0];
    s32 channel;  // 0xb0
};

struct UnkSndChannels_o0301 {
    u8 pad_0x0[0x348];
    u32 mask;       // 0x348
    s32 levels[8];  // 0x34c
    u8 pad_0x36c[0x3f0 - 0x36c];
    base::UnkVector<UnkSndChannelVoice_o0301*> voices_a;  // 0x3f0
    base::UnkVector<UnkSndChannelVoice_o0301*> voices_b;  // 0x418
    u8 pad_0x440[0x400568 - 0x440];
    nn::os::MutexType mutex;  // 0x400568
};

// The level of a channel, addressed from raw bytes (the original forms the address again each time).
static inline s32 LevelOf(UnkSndChannels_o0301* self, s32 channel) {
    return *reinterpret_cast<s32*>(reinterpret_cast<u8*>(self) + static_cast<s64>(channel) * 4 + 0x34c);
}

void fn_71000feff8(void* player, u32 id, UnkSoundHandle_b46a1 handle, f32 value) {
    UnkSndChannels_o0301* self = static_cast<UnkSndChannels_o0301*>(player);
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    // read through a plain pointer: the original reads the handle bits again after each level store
    const u32* bits = &handle.value;
    u32 mask = self->mask;
    self->mask = *bits | mask;
    if (*bits & 0x01)
        self->levels[0] = id;
    if (*bits & 0x02)
        self->levels[1] = id;
    if (*bits & 0x04)
        self->levels[2] = id;
    if (*bits & 0x08)
        self->levels[3] = id;
    if (*bits & 0x10)
        self->levels[4] = id;
    if (*bits & 0x20)
        self->levels[5] = id;
    if (*bits & 0x40)
        self->levels[6] = id;
    if (*bits & 0x80)
        self->levels[7] = id;
    UnkSndChannelVoice_o0301** it = self->voices_b.data;
    UnkSndChannelVoice_o0301** end = it + self->voices_b.count;
    for (; it != end; ++it) {
        UnkSndChannelVoice_o0301* voice = *it;
        s32 channel = voice->channel;
        if (*bits & (1u << channel))
            fn_7100105ef4(voice, g_unk_71016479f8[LevelOf(self, channel)], value, true);
    }
    it = self->voices_a.data;
    end = it + self->voices_a.count;
    for (; it != end; ++it) {
        UnkSndChannelVoice_o0301* voice = *it;
        s32 channel = voice->channel;
        if (*bits & (1u << channel))
            fn_7100105ef4(voice, g_unk_71016479f8[LevelOf(self, channel)], value, true);
    }
    if (self->mask) {
        alignas(8) u32 payload = 2;
        QueueSoundCommand(reinterpret_cast<UnkSoundSystemATK*>(self), 0x1f, &payload);
    }
    fn_7100066920(mutex);
}
