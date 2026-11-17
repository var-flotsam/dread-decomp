// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK's range): the sound system's maps of ids and
// what it does with them. fn_7100101e78 and fn_71001021d4 add an id to the map at +0x400788 and the one
// at +0x4007d0 (a node whose value is still null) and tell the sound thread through the second command
// queue (fn_71000fbaf8, command 0); fn_7100101fb0 and fn_7100102310 take an id out again (the first also
// drops its sound's use of a key, the second frees the sound from the engine heap); fn_71001030d8 sets
// a sound's volume from a level (as fn_7100102918, src/base/snd/CSoundSystemATK/UnkSoundVolumeById_b52.cpp,
// does for the first map); fn_71000fe418 finds the playing voice of a file path. Names and layouts are
// placeholders.
#include <math.h>
#include <string.h>

#include <new>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"
#include "base/tunable/CTunable.h"
#include "game/UnkTU101.h"
#include "game/UnkTU58.h"
#include "game/UnkTU99.h"
#include "game/operator_new.h"
#include "game/sound/UnkSoundHandle.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::snd::UnkSoundSystemATK;

void fn_71000fbaf8(UnkSoundSystemATK* sys, s32 command, const void* payload);

// A sound the system plays: the path id's data (its hash is at +0x30), three flags and a level.
struct UnkSndVoice_b0185 {
    u8 pad_0x0[0x90];
    UnkStringData* path;  // 0x90
    u8 pad_0x98[0x6c];
    f32 field_0x104;
    u8 pad_0x108[0x10];
    u8 field_0x118;
    u8 pad_0x119;
    u8 field_0x11a;
    u8 field_0x11b;
};
static_assert(__builtin_offsetof(UnkSndVoice_b0185, field_0x11b) == 0x11b, "layout");

// The sound system's maps of ids to sounds (and of keys to their records).
template <class K>
using UnkSndMap_b0185 = base::global::CRntDictionary<K, void*>;
template <class K>
using UnkSndNode_b0185 = typename base::global::CRntDictionary<K, void*>::TNode;
static_assert(sizeof(UnkSndMap_b0185<u32>) == 0x48, "layout");

struct UnkSndSys_b0185 {
    u8 pad_0x0[0x418];
    base::global::CRntVector<UnkSndVoice_b0185*> voices_b;  // 0x418
    u8 pad_0x440[0x400568 - 0x440];
    nn::os::MutexType mutex;  // 0x400568
    u8 pad_0x400568[0x400788 - 0x400568 - sizeof(nn::os::MutexType)];
    UnkSndMap_b0185<u32> sounds_a;  // 0x400788
    UnkSndMap_b0185<u32> sounds_b;  // 0x4007d0
    u8 pad_0x400818[0x400878 - 0x400818];
    UnkSndMap_b0185<u64>* map_d;  // 0x400878
};
static_assert(__builtin_offsetof(UnkSndSys_b0185, sounds_b) == 0x4007d0, "layout");
static_assert(__builtin_offsetof(UnkSndSys_b0185, mutex) == 0x400568, "layout");
static_assert(__builtin_offsetof(UnkSndSys_b0185, map_d) == 0x400878, "layout");

static inline UnkSoundSystemATK* Atk(UnkSndSys_b0185* self) { return reinterpret_cast<UnkSoundSystemATK*>(self); }

// Storage for a file-path id whose release is written out by hand (as src/base/snd/CSoundSystemATK.cpp).
union UnkPathIdSlot_b0185 {
    CFilePathStrId id;
    UnkPathIdSlot_b0185() {}
    ~UnkPathIdSlot_b0185() {}
};

// The hash of a pooled path id's data.
static inline u64 Hash_b0185(UnkStringData* data) {
    return *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
}

// Releases the id in `slot` (whose data pointer was read once as `data`) like its destructor.
#define RELEASE_ID_b0185(slot, data)                            \
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) { \
        if (data && data != &pool->empty) {                     \
            if (--data->ref_count == 0) {                       \
                fn_71000803a4(pool, data);                      \
                pool = g_unk_7101d47200;                        \
            }                                                   \
        }                                                       \
        slot.id.data = &pool->empty;                            \
    }

// ---- the id maps ----

// Looks `key` up in an id map (the bucket is the key's low 32 bits modulo the bucket count).
template <class K>
static inline __attribute__((always_inline)) UnkSndNode_b0185<K>* FindNode_b0185(UnkSndMap_b0185<K>& map, K key) {
    if (!map.buckets.count)
        return nullptr;
    UnkSndNode_b0185<K>** bucket = static_cast<u32>(key) % map.buckets.capacity + map.buckets.data;
    if (!bucket)
        return nullptr;
    UnkSndNode_b0185<K>* n = *bucket;
    for (; n; n = n->bucket_next) {
        if (n->key == key)
            break;
    }
    return n;
}

// Takes the node for `key` out of its bucket and the list of all nodes and frees it (the layout of
// src/base/UnkUnit122/UnkChainedMapErase_b54.cpp, on 32-bit keys).
template <class K>
static inline __attribute__((always_inline)) void EraseNode_b0185(UnkSndMap_b0185<K>& m, const K& key) {
    typedef UnkSndNode_b0185<K> Node;
    Node** bucket = static_cast<u32>(key) % m.buckets.capacity + m.buckets.data;
    Node* n = *bucket;
    if (!n)
        return;
    if (n->key == key) {
        *bucket = n->bucket_next;
    } else {
        Node* before;
        do {
            before = n;
            n = n->bucket_next;
            if (!n)
                return;
        } while (n->key != key);
        before->bucket_next = n->bucket_next;
    }
    if (m.first == n)
        m.first = n->next;
    if (m.last == n)
        m.last = n->Prev();
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    m.allocator->vf_0x48(n);
    --m.count;
}

// A command's payload for the second queue: an id and two words.
struct UnkSndIdPayload_b0185 {
    u32 id;
    u32 a;
    u32 b;
};

void fn_7100101e78(UnkSndSys_b0185* self, const u32* id) {
    u32 key = *id;
    if (FindNode_b0185(self->sounds_a, key))
        return;
    nn::os::GetCurrentThread();
    UnkSndIdPayload_b0185 payload = {key, 0, 0};
    fn_71000fbaf8(Atk(self), 0, &payload);
    self->sounds_a.Insert(*id, nullptr);
}

void fn_71001021d4(UnkSndSys_b0185* self, const u32* id) {
    u32 key = *id;
    if (FindNode_b0185(self->sounds_b, key))
        return;
    nn::os::GetCurrentThread();
    UnkSndIdPayload_b0185 payload = {key, 1, 0};
    fn_71000fbaf8(Atk(self), 0, &payload);
    self->sounds_b.Insert(*id, nullptr);
}

// ---- a sound's volume from a level ----

void fn_71000f5348(void* sound, f32 volume);
void fn_71000f5360(void* sound, s32 a, f32 b);

struct UnkSndLevel_b0185 {
    u32 id;
    u32 level;
};

void fn_71001030d8(UnkSndSys_b0185* self, const UnkSndLevel_b0185* params) {
    UnkSndNode_b0185<u32>* node = FindNode_b0185(self->sounds_b, params->id);
    u32 level = params->level;
    f32 volume;
    if (level < 80)
        volume = -0x1.ba90c0p-1f;
    else if (level >= 16000)
        volume = -0x1.9999a0p-4f;
    else
        volume = logf((f32)level) * 0x1.2776c6p-3f + -0x1.fc7f0ep-2f + -1.0f;
    if (!node->value)
        return;
    f32 clamped = volume <= -1.0f ? -1.0f : (volume >= 0.0f ? 0.0f : volume);
    fn_71000f5348(node->value, clamped);
    fn_71000f5360(node->value, -1, 0.0f);
}

// ---- removing a sound's id ----

struct UnkFreeHookHost_b0185 {
    u64 field_0x0;
    u8 sub[0x58];  // a base::UnkSubAllocator
    void (*on_free)(void* user, UnkFreeHookHost_b0185* self);
    void* user;
};

struct UnkAtkHeap;
void fn_71000f4414(void* sound);

// Takes the id out of the second map and frees its sound from the engine heap.
void fn_7100102310(UnkSndSys_b0185* self, const u32* id) {
    if (!self->sounds_b.buckets.count)
        return;
    UnkSndMap_b0185<u32>& m = self->sounds_b;
    UnkSndNode_b0185<u32>* node = FindNode_b0185(m, *id);
    if (!node)
        return;
    void* sound = node->value;
    nn::os::GetCurrentThread();
    if (sound) {
        fn_71000f4414(sound);
        UnkFreeHookHost_b0185* heap = reinterpret_cast<UnkFreeHookHost_b0185*>(&g_unk_7108b69038);
        if (heap->on_free)
            heap->on_free(heap->user, heap);
        fn_7100081420(reinterpret_cast<base::UnkSubAllocator*>(heap->sub), sound);
    }
    u32 key = *id;
    nn::os::GetCurrentThread();
    UnkSndIdPayload_b0185 payload = {key, 1, 1};
    fn_71000fbaf8(Atk(self), 0, &payload);
    EraseNode_b0185(m, *id);
}

// A sound as the removal reads it: its key at +0x48.
struct UnkSndSound_b0185 {
    u8 pad_0x0[0x48];
    u64 key;
};

// The record a key maps to: how many sounds use it, at +0x14.
struct UnkSndKeyUse_b0185 {
    u8 pad_0x0[0x14];
    u32 users;
};

void fn_71000f3768(UnkSndSound_b0185* sound);

// Takes the id out of the first map, dropping its sound's use of its key (the map at +0x400878).
void fn_7100101fb0(UnkSndSys_b0185* self, const u32* id) {
    if (!self->sounds_a.buckets.count)
        return;
    UnkSndMap_b0185<u32>& m = self->sounds_a;
    UnkSndNode_b0185<u32>* node = FindNode_b0185(m, *id);
    if (!node)
        return;
    UnkSndSound_b0185* sound = static_cast<UnkSndSound_b0185*>(node->value);
    nn::os::GetCurrentThread();
    if (sound) {
        UnkSndMap_b0185<u64>& uses = *self->map_d;
        u64 sound_key = sound->key;
        UnkSndKeyUse_b0185* use = nullptr;
        if (uses.buckets.count) {
            UnkSndNode_b0185<u64>** bucket = static_cast<u32>(sound_key) % uses.buckets.capacity + uses.buckets.data;
            if (bucket) {
                for (UnkSndNode_b0185<u64>* p = *bucket; p; p = p->bucket_next) {
                    if (p->key == sound_key) {
                        UnkSndNode_b0185<u64>* q = *bucket;
                        while (q->key != sound_key)
                            q = q->bucket_next;
                        use = static_cast<UnkSndKeyUse_b0185*>(q->value);
                        break;
                    }
                }
            }
        }
        fn_71000f3768(sound);
        nn::os::GetCurrentThread();
        if (use && use->users >= 2)
            --use->users;
    }
    u32 key = *id;
    nn::os::GetCurrentThread();
    UnkSndIdPayload_b0185 payload = {key, 0, 1};
    fn_71000fbaf8(Atk(self), 0, &payload);
    EraseNode_b0185(m, *id);
}

// ---- the playing sound of a path ----

struct UnkSoundRef;

struct UnkPlayingHandle_ovb0074 {
    void* ref;
    u64 serial;
};

struct UnkVoicePool_b56;
struct UnkSoundVoice2_b56;

// A slot of the voice statistics: what the handle of a voice points at.
struct UnkVoiceStat_b0185 {
    u64 field_0x0;
    u64 released;
};

struct UnkSndPlaying_b0185 {
    u8 pad_0x0[0x398];
    u8 pool[0x28];              // 0x398
    UnkVoiceStat_b0185* stats;  // 0x3c0
};

// The handle of the first voice of the file path `*path` (sanitised by fn_71000f63b0) that is requested
// and not stopped, with its level below zero: its slot in the voice statistics and that slot's serial;
// the empty handle when the queue is off, and an invalid one (null, ~0) when no voice qualifies.
UnkPlayingHandle_ovb0074 fn_71000fe418(void* member, UnkStringData** path, u32 flags) {
    UnkSndSys_b0185* self = static_cast<UnkSndSys_b0185*>(member);
    if (g_unk_7108b6ab60 == 1)
        return {g_unk_7108b6c460.ref, static_cast<u64>(g_unk_7108b6c460.serial)};
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    UnkPathIdSlot_b0185 slot;
    new (&slot.id) CFilePathStrId(fn_71000f63b0(*reinterpret_cast<const CFilePathStrId*>(path)));
    UnkStringData* data = slot.id.data;
    u64 key = Hash_b0185(data);
    RELEASE_ID_b0185(slot, data)
    UnkPlayingHandle_ovb0074 result = {nullptr, ~0ull};
    UnkSndVoice_b0185** it = self->voices_b.data;
    UnkSndVoice_b0185** end = it + self->voices_b.count;
    for (; it != end; ++it) {
        UnkSndVoice_b0185* v = *it;
        if (Hash_b0185(v->path) != key)
            continue;
        if (!(v->field_0x118 || v->field_0x11b || v->field_0x11a))
            continue;
        if (v->field_0x104 < 0.0f) {
            UnkSndPlaying_b0185* playing = reinterpret_cast<UnkSndPlaying_b0185*>(self);
            u32 index = fn_7100083854(reinterpret_cast<UnkVoicePool_b56*>(playing->pool),
                                      reinterpret_cast<UnkSoundVoice2_b56*>(v));
            UnkVoiceStat_b0185* stat = playing->stats + index;
            result = {stat, stat->released};
            break;
        }
    }
    fn_7100066920(mutex);
    return result;
}
