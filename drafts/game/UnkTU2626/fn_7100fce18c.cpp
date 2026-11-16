// Unit 2626: plays a sound through the sound manager and, when it started (its object is not the empty
// handle's), passes the object the value at +0x8 (fn_7100106024) and keeps the handle with its path's
// hash in the paired array at +0x10. Names and layouts are placeholders.
#include <cstring>

#include "base/global/CRntVector.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU101.h"
#include "game/UnkTU23.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

// As src/base/snd/CSoundSystem/CSoundSystem_Groups_ovb0074.cpp defines them.
struct UnkPlayingSound_ovb0074 {
    u8 pad_0x0[0xd8];
    u32 variant;  // 0xd8
    u8 pad_0xdc[0x11b - 0xdc];
    bool field_0x11b;
};

struct UnkPlayingRef_ovb0074 {
    UnkPlayingSound_ovb0074* sound;
    u64 serial;
};

struct UnkPlayingHandle_ovb0074 {
    UnkPlayingRef_ovb0074* ref;
    u64 serial;

    bool IsValid() const { return ref && serial != ~0ull && ref->serial == serial; }
    UnkPlayingSound_ovb0074* Get() const { return IsValid() ? ref->sound : nullptr; }
};

struct UnkSoundMgr;
struct UnkSoundObject_b50;

// The sound a handle names while its reference block still has the handle's serial.
static inline UnkPlayingSound_ovb0074* SoundOf(const UnkPlayingHandle_ovb0074& handle) {
    UnkPlayingRef_ovb0074* ref = handle.ref;
    if (ref && ref->serial == handle.serial && handle.serial != ~0ull)
        return ref->sound;
    return nullptr;
}

// The sound's path id as the description holds it: its string, with the string's hash at +0x30.
struct UnkSoundPath_lfce18c {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkSoundDesc_lfce18c {
    s32 group;
    UnkSoundPath_lfce18c* path;  // 0x8
};

// Handles of the sounds started, and their paths' hashes, in two arrays of one count.
struct UnkPlayingList_lfce18c {
    UnkPlayingHandle_ovb0074* handles;
    u64* keys;
    u32 count;     // 0x10
    u32 capacity;  // 0x14
    u32 step;
    u32 flags;     // 0x1c
    base::UnkAllocator* allocator;  // 0x20

    void Reserve(u32 n) {
        if (count >= n)
            return;
        UnkPlayingHandle_ovb0074* new_handles =
            static_cast<UnkPlayingHandle_ovb0074*>(allocator->vf_0x40((u64)n * sizeof(UnkPlayingHandle_ovb0074), 8, flags));
        u64* new_keys = static_cast<u64*>(allocator->vf_0x40((u64)n * sizeof(u64), 8, flags));
        if (UnkPlayingHandle_ovb0074* old = handles) {
            UnkPlayingHandle_ovb0074* dst = new_handles;
            for (u32 i = 0; i < count; ++i, ++old, ++dst)
                *dst = *old;
            allocator->vf_0x48(handles);
        }
        if (keys) {
            memcpy(new_keys, keys, (u64)count * sizeof(u64));
            allocator->vf_0x48(keys);
        }
        handles = new_handles;
        keys = new_keys;
        capacity = n;
    }

    void Add(u64 key, const UnkPlayingHandle_ovb0074& handle) {
        if (count == capacity)
            Reserve(capacity + step);
        keys[count] = key;
        handles[count] = handle;
        ++count;
    }
};

struct UnkSoundPlayer_lfce18c {
    u64 field_0x0;
    u64 field_0x8;
    UnkPlayingList_lfce18c playing;  // 0x10
};

UnkPlayingHandle_ovb0074 fn_7100fce18c(UnkSoundPlayer_lfce18c* self, UnkSoundDesc_lfce18c* desc) {
    UnkPlayingHandle_ovb0074 handle =
        fn_7100108620(reinterpret_cast<UnkSoundSystemPlayView_ovb0074*>(g_unk_7101d4d820),
                      reinterpret_cast<UnkSoundDesc_ovb0074*>(desc));
    const UnkPlayingHandle_ovb0074& empty = reinterpret_cast<const UnkPlayingHandle_ovb0074&>(g_unk_7108b6c460);
    if (SoundOf(handle) != SoundOf(empty)) {
        fn_7100106024(reinterpret_cast<UnkSoundObject_b50*>(SoundOf(handle)), &self->field_0x8);
        self->playing.Add(desc->path->hash, handle);
    }
    return handle;
}
