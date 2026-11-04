// Unit 99 (the sound system's): stop a playing voice. The voice must be in one of the system's two
// lists of live voices (at +0x3f0 with a flag, command 1; at +0x418, command 3); it is queued for the
// sound thread, taken out of the list (a base::UnkVector), its key use and slot counters are dropped and it goes back to
// the voice pool. Names and layouts are placeholders.
#include <cstring>

#include "base/UnkVector.h"
#include "base/snd/UnkSoundSystemATK.h"

bool fn_7100132cd0();

struct UnkKeyUseTable;  // src/game/UnkUnit100/UnkUnit100Release.cpp
void fn_7100104ae4(UnkKeyUseTable* self, const u32* key);
void fn_7100104ef4();
struct UnkSoundVoice2_b56;
void fn_7100104f08(void* voice);
void fn_710010519c(UnkSoundVoice2_b56* voice, void* system, u32 seq, bool flag);

struct UnkSoundVoice2_b56 {
    u8 pad_0x0[0xb4];
    u32 key;      // +0xb4
    s32 group;    // +0xb8
    s32 channel;  // +0xbc
    u8 pad_0xc0[0x4];
    u32 id;  // +0xc4
};

struct UnkVoicePool_b56;
void* fn_7100083804(UnkVoicePool_b56* self);
u64 fn_7100083854(UnkVoicePool_b56* self, UnkSoundVoice2_b56* voice);
void fn_7100083834(UnkVoicePool_b56* self, UnkSoundVoice2_b56* voice);

struct UnkVoiceStat_b56 {
    u64 field_0x0;
    u64 released;
};
struct UnkVoiceStats_b56;
UnkVoiceStat_b56& fn_7100103da0(UnkVoiceStats_b56* self, u32 index);

using UnkVoiceList_b56 = base::UnkVector<UnkSoundVoice2_b56*>;

static inline bool Contains_b56(UnkVoiceList_b56& list, UnkSoundVoice2_b56* voice) {
    for (u32 i = 0; i < list.count; i++)
        if (list.data[i] == voice)
            return true;
    return false;
}

static inline void Remove_b56(UnkVoiceList_b56& list, UnkSoundVoice2_b56* voice) {
    for (u32 i = 0; i < list.count; i++) {
        if (list.data[i] == voice) {
            memcpy(&list.data[i], &list.data[i + 1], (list.count - i - 1) * sizeof(list.data[0]));
            --list.count;
            return;
        }
    }
}

struct UnkSoundSystemVoices_b56 {
    u8 pad_0x0[0x278];
    u8 keys[0x7c];      // +0x278
    s32 groups[8];      // +0x2f4
    s32 channels[0x1b]; // +0x314
    u32 next_seq;  // +0x380
    u8 pad_0x384[0x14];
    u8 pool[0x28];       // +0x398
    u8 stats[0x28];      // +0x3c0
    u64 released;        // +0x3e8
    UnkVoiceList_b56 flagged;  // +0x3f0
    UnkVoiceList_b56 plain;  // +0x418
};
static_assert(__builtin_offsetof(UnkSoundSystemVoices_b56, groups) == 0x2f4, "");
static_assert(__builtin_offsetof(UnkSoundSystemVoices_b56, channels) == 0x314, "");
static_assert(__builtin_offsetof(UnkSoundSystemVoices_b56, pool) == 0x398, "");
static_assert(__builtin_offsetof(UnkSoundSystemVoices_b56, flagged) == 0x3f0, "");
static_assert(__builtin_offsetof(UnkSoundSystemVoices_b56, plain) == 0x418, "");

static inline void ReturnVoice_b56(UnkSoundSystemVoices_b56* self, UnkSoundVoice2_b56* voice) {
    UnkVoicePool_b56* pool = reinterpret_cast<UnkVoicePool_b56*>(self->pool);
    u64 index = fn_7100083854(pool, voice);
    fn_7100103da0(reinterpret_cast<UnkVoiceStats_b56*>(self->stats), index).released++;
    fn_7100083834(pool, voice);
    self->released++;
}

struct UnkVoiceFlagPayload_b56 {
    u32 id;
    u32 flag;
};

bool fn_71000fbf2c(UnkSoundSystemVoices_b56* self, UnkSoundVoice2_b56* voice, bool flag) {
    fn_7100132cd0();
    if (!voice || !Contains_b56(self->flagged, voice))
        return false;
    fn_7100104ef4();
    UnkVoiceFlagPayload_b56 payload = {voice->id, flag};
    fn_71000faac4(reinterpret_cast<base::snd::UnkSoundSystemATK*>(self), 1, &payload);
    Remove_b56(self->flagged, voice);
    fn_7100104ae4(reinterpret_cast<UnkKeyUseTable*>(self->keys), &voice->key);
    self->groups[voice->group]--;
    self->channels[voice->channel]--;
    ReturnVoice_b56(self, voice);
    return true;
}

bool fn_71000fc074(UnkSoundSystemVoices_b56* self, UnkSoundVoice2_b56* voice) {
    fn_7100132cd0();
    if (!voice || !Contains_b56(self->plain, voice))
        return false;
    fn_7100104ef4();
    UnkVoiceFlagPayload_b56 payload;
    payload.id = voice->id;
    fn_71000faac4(reinterpret_cast<base::snd::UnkSoundSystemATK*>(self), 3, &payload);
    Remove_b56(self->plain, voice);
    fn_7100104ae4(reinterpret_cast<UnkKeyUseTable*>(self->keys), &voice->key);
    ReturnVoice_b56(self, voice);
    return true;
}

// Takes a voice from the pool (while the count of free ones at +0x3e8 allows), constructs it, starts
// it as a plain voice with the next sequence number (command 2) and adds it to the plain list.
UnkSoundVoice2_b56* fn_71000fd89c(UnkSoundSystemVoices_b56* self) {
    fn_7100132cd0();
    void* mem = nullptr;
    if (self->released != 0) {
        self->released--;
        mem = fn_7100083804(reinterpret_cast<UnkVoicePool_b56*>(self->pool));
    }
    fn_7100104f08(mem);
    UnkSoundVoice2_b56* voice = static_cast<UnkSoundVoice2_b56*>(mem);
    if (!voice)
        return nullptr;
    fn_710010519c(voice, self, self->next_seq++, true);
    UnkVoiceFlagPayload_b56 payload;
    payload.id = voice->id;
    fn_71000faac4(reinterpret_cast<base::snd::UnkSoundSystemATK*>(self), 2, &payload);
    self->plain.PushBack(voice);
    return voice;
}
