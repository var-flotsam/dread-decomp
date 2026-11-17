// Unit 99 (CTunableSoundSystemATK): starts a sound with its list of up to eight tracks on a voice: the one
// already playing its path (fn_71000fd778) or a new one (fn_71000fd89c), each track's path looked up in the
// map at +0x4b8, and the start sent as command 0x13; returns the voice's handle, the empty handle when the
// queue is off or a path is unknown. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU99.h"
#include "game/operator_new.h"
#include "game/sound/UnkSoundHandle.h"

using base::global::CFilePathStrId;
using base::snd::UnkSoundSystemATK;

struct UnkSoundDesc_ovb0074;
struct UnkSoundSystemVoices_b56;

struct UnkVoiceStat_b56 {
    u64 field_0x0;
    u64 released;
};
struct UnkVoiceStats_b56;
UnkVoiceStat_b56& fn_7100103da0(UnkVoiceStats_b56* self, u32 index);
UnkSoundVoice2_b56* fn_71000fd89c(UnkSoundSystemVoices_b56* self);
UnkSoundHandle fn_71000fd778(UnkSoundSystemATK* self, const CFilePathStrId& path);
extern const f32 g_unk_71016479f8[] __attribute__((visibility("hidden")));  // level values

// A voice as the start sets it up.
struct UnkStartVoice_l0fdb1c {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u8 pad_0xc[0x90 - 0xc];
    CFilePathStrId path;  // 0x90
    u8 pad_0x98[0xb0 - 0x98];
    s32 group;  // 0xb0
    u8 pad_0xb4[0xc4 - 0xb4];
    u32 field_0xc4;
    u8 pad_0xc8[0xec - 0xc8];
    u32 field_0xec;
    u8 pad_0xf0[0xfc - 0xf0];
    u32 field_0xfc;
    u32 field_0x100;
    u8 pad_0x104[4];
    f32 field_0x108;
    u8 pad_0x10c[4];
    u32 field_0x110;
    u8 pad_0x114[4];
    bool field_0x118;
    bool field_0x119;
    u8 pad_0x11a[6];
    bool field_0x120;
    u8 pad_0x121;
    bool field_0x122;
};
static_assert(__builtin_offsetof(UnkStartVoice_l0fdb1c, field_0x122) == 0x122, "layout");

// What a handle points at: the voice and its serial.
struct UnkStartRef_l0fdb1c {
    UnkStartVoice_l0fdb1c* voice;
    s64 serial;
};

struct UnkStartDesc_l0fdb1c {
    s32 group;
    u8 pad_0x4[4];
    CFilePathStrId path;  // 0x8
    u8 pad_0x10[0x24 - 0x10];
    u32 field_0x24;
    u8 pad_0x28[4];
    u32 field_0x2c;
    u32 field_0x30;
    u32 field_0x34;
    u32 field_0x38;
};

struct UnkStartTrackDesc_l0fdb1c {
    CFilePathStrId path;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

struct UnkStartTrackList_l0fdb1c {
    UnkStartTrackDesc_l0fdb1c* data;
    u32 count;
};

struct UnkStartTrack_l0fdb1c {
    u64 key;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};

// Command 0x13's payload.
struct UnkStartCommand_l0fdb1c {
    u32 field_0x0;
    s32 group;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 track_count;
    UnkStartTrack_l0fdb1c tracks[8];
};

struct UnkStartNode_l0fdb1c {
    u64 key;
    void* value;
    u8 pad_0x10[0x10];
    UnkStartNode_l0fdb1c* chain;  // 0x20
};

struct UnkStartSystem_l0fdb1c {
    u8 pad_0x0[0xe8];
    u32 active_groups;  // 0xe8
    u8 pad_0xec[0x348 - 0xec];
    u32 faded_groups;      // 0x348
    s32 group_level[0x13];  // 0x34c
    u8 pad_0x398_[0x398 - 0x34c - 0x13 * 4];
    u8 pool[0x28];  // 0x398
    u8 stats[8];    // 0x3c0
    u8 pad_0x3c8[0x4c8 - 0x3c8];
    UnkStartNode_l0fdb1c** buckets;  // 0x4c8
    u32 used;                        // 0x4d0
    u32 bucket_count;                // 0x4d4
    u8 pad_0x4d8[0x400568 - 0x4d8];
    nn::os::MutexType mutex;  // 0x400568

    // Whether the path map has a node for `key`.
    bool HasPath(u64 key) {
        if (!used)
            return false;
        UnkStartNode_l0fdb1c** bucket = static_cast<u32>(key) % bucket_count + buckets;
        if (!bucket)
            return false;
        for (UnkStartNode_l0fdb1c* n = *bucket; n; n = n->chain) {
            if (n->key == key)
                return true;
        }
        return false;
    }

    UnkSoundHandle HandleOf(UnkStartVoice_l0fdb1c* voice) {
        u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(pool),
                                  reinterpret_cast<UnkSoundVoice2_b56*>(voice));
        UnkVoiceStat_b56& stat = fn_7100103da0(reinterpret_cast<UnkVoiceStats_b56*>(stats), index);
        return {reinterpret_cast<UnkSoundRef*>(&stat), static_cast<s64>(stat.released)};
    }
};
static_assert(__builtin_offsetof(UnkStartSystem_l0fdb1c, pool) == 0x398, "layout");
static_assert(__builtin_offsetof(UnkStartSystem_l0fdb1c, mutex) == 0x400568, "layout");

// The sanitised form of a sound's file path.
static inline CFilePathStrId SoundPath(const CFilePathStrId& path) {
    fn_7100132cd0();
    return fn_71000f63b0(path);
}

static inline UnkStartVoice_l0fdb1c* VoiceOf(const UnkSoundHandle& handle) {
    UnkStartRef_l0fdb1c* ref = reinterpret_cast<UnkStartRef_l0fdb1c*>(handle.ref);
    if (ref && handle.serial != -1 && ref->serial == handle.serial)
        return ref->voice;
    return nullptr;
}

// The start itself, under the system's mutex.
static UnkSoundHandle Start(UnkStartSystem_l0fdb1c* self, const UnkStartDesc_l0fdb1c* desc,
                            const UnkStartTrackList_l0fdb1c* list) {
    UnkSoundSystemATK* atk = reinterpret_cast<UnkSoundSystemATK*>(self);
    void* member = self;
    CFilePathStrId path = g_unk_7101d470e8;
    u32 count = list->count;
    if (count == 0)
        return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    path = SoundPath(desc->path);
    if (!self->HasPath(path.data->Key()))
        return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};

    UnkStartVoice_l0fdb1c* voice = VoiceOf(fn_71000fd778(atk, desc->path));
    if (!voice || voice->field_0x122) {
        voice = reinterpret_cast<UnkStartVoice_l0fdb1c*>(
            fn_71000fd89c(static_cast<UnkSoundSystemVoices_b56*>(member)));
        if (!voice)
            return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    }
    if (self->active_groups & (1 << desc->group)) {
        voice->field_0x119 = true;
        return self->HandleOf(voice);
    }

    voice->path = path;
    voice->field_0x120 = true;
    voice->group = desc->group;
    voice->field_0xec = desc->field_0x2c;
    u32 level = desc->field_0x2c;
    voice->field_0x4 = level;
    voice->field_0x8 = level;
    voice->field_0x0 = level;
    voice->field_0xfc = desc->field_0x30;
    voice->field_0x100 = desc->field_0x34;
    voice->field_0x108 = 0.0f;
    voice->field_0x110 = desc->field_0x38;
    voice->field_0x118 = true;

    UnkStartCommand_l0fdb1c command = {voice->field_0xc4, desc->group,      desc->field_0x30,
                                       desc->field_0x2c,  desc->field_0x24, count};
    for (u64 i = 0; i != count; ++i) {
        const UnkStartTrackDesc_l0fdb1c& source = list->data[i];
        CFilePathStrId track_path = fn_71000f63b0(source.path);
        u64 key = track_path.data->Key();
        if (!self->HasPath(key))
            return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
        UnkStartTrack_l0fdb1c& track = command.tracks[i];
        track.key = key;
        track.field_0x8 = list->data[i].field_0x8;
        track.field_0xc = list->data[i].field_0xc;
        track.field_0x10 = list->data[i].field_0x10;
    }
    fn_71000faac4(atk, 0x13, &command);

    voice->field_0x119 = false;
    s32 group = desc->group;
    if (self->faded_groups & (1 << group))
        fn_7100105ef4(voice, g_unk_71016479f8[self->group_level[group]], 0.0f, true);
    return self->HandleOf(voice);
}

UnkSoundHandle fn_71000fdb1c(void* member, UnkSoundDesc_ovb0074* desc, void* tracks) {
    if (g_unk_7108b6ab60 == 1)
        return {g_unk_7108b6c460.ref, g_unk_7108b6c460.serial};
    UnkStartSystem_l0fdb1c* self = static_cast<UnkStartSystem_l0fdb1c*>(member);
    fn_7100132cd0();
    fn_710006691c(&self->mutex);
    UnkSoundHandle result = Start(self, reinterpret_cast<const UnkStartDesc_l0fdb1c*>(desc),
                                  static_cast<const UnkStartTrackList_l0fdb1c*>(tracks));
    fn_7100066920(&self->mutex);
    return result;
}
