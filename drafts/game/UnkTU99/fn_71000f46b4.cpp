// Unit 99 (base::snd::CSoundSystemATK): a playing voice's per-frame update. Its volume and pitch fade
// towards their targets, a voice faded out to silence is stopped, and the stream's loop points and play
// position are tracked so that the voice moves on to its next entry once the entry's loops are done.
// Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkStreamSound;
u32 fn_710121e69c(UnkAtkStreamSound* self, void* arg);
s64 fn_710121e6b4(UnkAtkStreamSound* self);
s64 fn_710121e6e4(UnkAtkStreamSound* self, u32 unit);
struct UnkSoundVoice_71000f4a68;
bool fn_71000f4a68(UnkSoundVoice_71000f4a68* self, bool stop);

// The voice's sound: ready (vf_0x28) and how many times its loop has played (+0x10c).
struct UnkVoiceSound_l0f46b4 {
    void** vtable;
    u8 pad_0x8[0x104];
    u32 loops;  // 0x10c

    bool vf_0x28() { return reinterpret_cast<bool (*)(UnkVoiceSound_l0f46b4*)>(vtable[0x28 / 8])(this); }
};

struct UnkVoicePlayer_l0f46b4 {
    u8 pad_0x0[0xb0];
    f32 volume;  // 0xb0
};

// What the stream reports about its loop.
struct UnkStreamInfo_l0f46b4 {
    u64 field_0x0;
    s64 loop_start;
    s64 loop_end;
    u8 pad_0x18[0x18];
};

struct UnkVoiceEntry_l0f46b4 {
    u32 id;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    s32 loop_start;  // 0x10
    s32 loop_end;    // 0x14
    s32 loops;       // 0x18, below 0 for endless
    u32 field_0x1c;
};

// Up to eight entries with their count in front; reaching past the end adds zeroed entries.
struct UnkVoiceEntries_l0f46b4 {
    u32 count;
    // Storage for the entries (as src/game/UnkTU99/UnkTU99_Start_l0f44cc.cpp keeps them).
    u8 storage[8 * sizeof(UnkVoiceEntry_l0f46b4)];

    UnkVoiceEntry_l0f46b4* Data() { return reinterpret_cast<UnkVoiceEntry_l0f46b4*>(storage); }

    UnkVoiceEntry_l0f46b4& At(u32 i) {
        if (i >= count) {
            memset(&Data()[count], 0, reinterpret_cast<u8*>(&Data()[i + 1]) - reinterpret_cast<u8*>(&Data()[count]));
            count = i + 1;
        }
        return Data()[i];
    }
};

// A value fading from `start` to `target` over `duration`.
struct UnkVoiceFade_l0f46b4 {
    f32 value;
    f32 start;
    f32 target;
    f32 duration;
    f32 time;
};

struct UnkVoice_l0f46b4 {
    UnkVoiceFade_l0f46b4 volume;  // 0x0
    UnkVoiceFade_l0f46b4 pitch;   // 0x14
    UnkVoiceSound_l0f46b4** handle;   // 0x28
    UnkVoicePlayer_l0f46b4* player;   // 0x30
    UnkVoiceEntries_l0f46b4 entries;  // 0x38
    s32 entry;                        // 0x13c
    s32 loop;                         // 0x140
    s64 position;                     // 0x148
    s64 next_position;                // 0x150
    bool looped;                      // 0x158
    u8 pad_0x159[0x13];
    bool playing;  // 0x16c
    u8 pad_0x16d[2];
    bool stopped;       // 0x16f
    bool stop_on_mute;  // 0x170
};
static_assert(__builtin_offsetof(UnkVoice_l0f46b4, entry) == 0x13c, "layout");
static_assert(__builtin_offsetof(UnkVoice_l0f46b4, stop_on_mute) == 0x170, "layout");

// The player's volume, never below zero.
static inline void SetVolume(f32* volume, f32 value) {
    *volume = !(value < 0.0f) ? value : 0.0f;
}

static inline f32 Fade(const UnkVoiceFade_l0f46b4& fade) {
    f32 t = fade.time / fade.duration;
    t = !(t >= 1.0f) ? t : 1.0f;
    return fade.start + t * (fade.target - fade.start);
}

void fn_71000f46b4(UnkVoice_l0f46b4* self, f32 dt, bool fade_up) {
    if (!self->playing || self->stopped)
        return;
    UnkVoiceSound_l0f46b4* sound = *self->handle;
    if (!sound || !sound->vf_0x28())
        return;

    f32 volume = self->volume.value;
    f32 target = self->volume.target;
    if (!(volume - 1e-5f <= target && volume + 1e-5f >= target) &&
        (target < volume || ((target > volume) & fade_up))) {
        self->volume.time += dt;
        if (self->volume.time < self->volume.duration)
            target = Fade(self->volume);
        self->volume.value = target;
        SetVolume(&self->player->volume, target);
    }

    if (self->stop_on_mute && self->volume.target <= 0.0f && self->volume.value <= 0.0f) {
        if (self->handle && *self->handle)
            fn_710120d54c(*self->handle, 2);
        if (self->player)
            fn_7101206630(reinterpret_cast<u8*>(self->player), 2);
        self->entry = -1;
        self->stopped = true;
        self->stop_on_mute = true;
    }

    f32 pitch = self->pitch.value;
    if (!(pitch - 1e-5f <= self->pitch.target) || !(pitch + 1e-5f >= self->pitch.target)) {
        sound = *self->handle;
        if (sound && sound->vf_0x28()) {
            self->pitch.time += dt;
            if (self->pitch.time < self->pitch.duration)
                self->pitch.value = Fade(self->pitch);
            else
                self->pitch.value = self->pitch.target;
            f32 value = self->pitch.value;
            if (*self->handle)
                fn_710120e660(reinterpret_cast<UnkEffect_b0184*>(*self->handle), value, 0);
            if (*self->handle)
                fn_710120e660(reinterpret_cast<UnkEffect_b0184*>(*self->handle), 0.0f, 1);
            if (*self->handle)
                fn_710120e660(reinterpret_cast<UnkEffect_b0184*>(*self->handle), 0.0f, 2);
            if (*self->handle)
                fn_710120e658(reinterpret_cast<UnkEffect_b0184*>(*self->handle), -self->pitch.value);
        }
    }

    if (self->entry < 0)
        return;
    u32 index = self->entry;
    self->entries.At(index);
    sound = *self->handle;
    if (!sound || !sound->vf_0x28())
        return;
    UnkAtkStreamSound* stream = reinterpret_cast<UnkAtkStreamSound*>(sound);
    UnkStreamInfo_l0f46b4 info;
    if (fn_710121e69c(stream, &info)) {
        UnkVoiceEntry_l0f46b4& entry = self->entries.Data()[index];
        if (entry.loop_end <= 0)
            entry.loop_end = (s32)info.loop_end;
        if (entry.loop_start <= 0)
            entry.loop_start = (s32)info.loop_start;
        self->position = fn_710121e6e4(stream, 1);
    }
    UnkVoiceEntry_l0f46b4& entry = self->entries.Data()[index];
    if (entry.loops < 0)
        return;
    s64 per_loop = self->position / (s64)(sound->loops + 1);
    s64 next = per_loop + self->position;
    self->next_position = next;
    if (next + per_loop * 2 >= entry.loop_end && !self->looped) {
        self->loop++;
        self->looped = true;
    }
    if (fn_710121e6b4(stream) == self->loop)
        self->looped = false;
    if (fn_710121e6b4(stream) > self->loop)
        self->loop = (s32)fn_710121e6b4(stream);
    if (self->loop >= entry.loops)
        fn_71000f4a68(reinterpret_cast<UnkSoundVoice_71000f4a68*>(self), (u32)entry.loops > 1);
}
