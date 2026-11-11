// best draft, differs (see results/b0185.json)
// Unit 99 (base::snd::CSoundSystemATK, the sound system's): scratch of batch b0185.
#include <math.h>
#include <string.h>

#include <new>

#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::snd::UnkSoundSystemATK;

bool fn_7100132cd0();
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
void fn_71000fbaf8(UnkSoundSystemATK* sys, s32 command, const void* payload);

// ---- the sound system's members these functions touch (placeholder names) ----

// A sound the system plays: the path id's data (its hash is at +0x30), three flags and a level.
struct UnkSndVoice_b0185 {
    u8 pad_0x0[0x90];
    UnkStringData* path;  // 0x90
    u8 pad_0x98[0x6c];
    f32 field_0x104;
    u8 pad_0x108[0x8];
    u8 pad_0x110[0x8];
    u8 field_0x118;
    u8 pad_0x119;
    u8 field_0x11a;
    u8 field_0x11b;
};
static_assert(__builtin_offsetof(UnkSndVoice_b0185, field_0x11b) == 0x11b, "layout");

template <class K>
struct UnkSndNode_b0185 {
    K key;  // 0x0
    void* value;  // 0x8
    UnkSndNode_b0185* next;   // 0x10: the list of all nodes
    UnkSndNode_b0185* prev;   // 0x18
    UnkSndNode_b0185* chain;  // 0x20: the next node of the bucket

    UnkSndNode_b0185*& Next() { return next; }
    UnkSndNode_b0185*& Prev() { return prev; }
};

template <class K>
struct UnkSndMap_b0185 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkSndNode_b0185<K>** buckets;  // 0x10
    u32 used;                       // 0x18
    u32 bucket_count;               // 0x1c
    u8 pad_0x20[0x18];
    UnkSndNode_b0185<K>* tail;  // 0x38
    UnkSndNode_b0185<K>* head;  // 0x40
};
static_assert(sizeof(UnkSndMap_b0185<u32>) == 0x48, "layout");

struct UnkSndFileNode_b0185 {
    u64 key;
    s32 users;  // 0x8
    u8 pad_0xc[0x14];
    UnkSndFileNode_b0185* chain;  // 0x20
};

struct UnkSndSys_b0185 {
    u8 pad_0x0[0x348];
    u32 field_0x348;
    s32 field_0x34c[8];
    u8 pad_0x36c[0x3f0 - 0x36c];
    base::UnkVector<UnkSndVoice_b0185*> voices_a;  // 0x3f0
    base::UnkVector<UnkSndVoice_b0185*> voices_b;  // 0x418
    base::UnkVector<UnkSndVoice_b0185*> active;    // 0x440
    base::UnkVector<u64> file_keys;                // 0x468
    u8 pad_0x490[0x4c8 - 0x490];
    UnkSndFileNode_b0185** files;  // 0x4c8
    u32 files_used;                // 0x4d0
    u32 files_buckets;             // 0x4d4
    u8 pad_0x4d8[0x400568 - 0x4d8];
    nn::os::MutexType mutex;  // 0x400568
    u8 pad_0x400568[0x400788 - 0x400568 - sizeof(nn::os::MutexType)];
    UnkSndMap_b0185<u32> sounds_a;  // 0x400788
    UnkSndMap_b0185<u32> sounds_b;  // 0x4007d0
    base::UnkVector<u64> keys_c;    // 0x400818
    base::UnkVector<u64> keys_d;    // 0x400840
    UnkSndMap_b0185<u64>* map_c;    // 0x400868
    u8 pad_0x400870[8];
    UnkSndMap_b0185<u64>* map_d;    // 0x400878
};
static_assert(__builtin_offsetof(UnkSndSys_b0185, sounds_b) == 0x4007d0, "layout");
static_assert(__builtin_offsetof(UnkSndSys_b0185, mutex) == 0x400568, "layout");

static inline UnkSoundSystemATK* Atk(UnkSndSys_b0185* self) { return reinterpret_cast<UnkSoundSystemATK*>(self); }

// ---- fn_71000fd308 ----

// The flag that turns the queue off, read as a bool.
#define QUEUE_OFF_b0185 (g_unk_7108b6ab60 != 0)

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
#define RELEASE_ID_b0185(slot, data)                                      if (base::global::UnkStringPool* pool = g_unk_7101d47200) {              if (data && data != &pool->empty) {                                       if (--data->ref_count == 0) {                                             fn_71000803a4(pool, data);                                            pool = g_unk_7101d47200;                                          }                                                                 }                                                                     slot.id.data = &pool->empty;                                      }

u32 fn_71000fd308(UnkSndSys_b0185* self, const CFilePathStrId* id) {
    u32 n = 0;
    if (QUEUE_OFF_b0185)
        return n;
    fn_7100132cd0();
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    UnkPathIdSlot_b0185 slot;
    new (&slot.id) CFilePathStrId(fn_71000f63b0(*id));
    UnkStringData* data = slot.id.data;
    u64 key = Hash_b0185(data);
    RELEASE_ID_b0185(slot, data)
    UnkSndVoice_b0185** it = self->voices_a.data;
    UnkSndVoice_b0185** end = it + self->voices_a.count;
    for (; it != end; ++it) {
        if (Hash_b0185((*it)->path) == key)
            ++n;
    }
    fn_7100066920(mutex);
    return n;
}

// ---- the id maps ----

// Looks `key` up in an id map (the bucket is the key's low 32 bits modulo the bucket count).
template <class K>
static inline __attribute__((always_inline)) UnkSndNode_b0185<K>* FindNode_b0185(UnkSndMap_b0185<K>& map, K key) {
    if (!map.used)
        return nullptr;
    UnkSndNode_b0185<K>** bucket = static_cast<u32>(key) % map.bucket_count + map.buckets;
    if (!bucket)
        return nullptr;
    UnkSndNode_b0185<K>* n = *bucket;
    for (; n; n = n->chain) {
        if (n->key == key)
            break;
    }
    return n;
}

// Adds a node for `key` (its value null) to the map, at the head of its bucket and at the end of the list.
template <class K>
static inline __attribute__((always_inline)) void AddNode_b0185(UnkSndMap_b0185<K>& m, const K& key) {
    UnkSndNode_b0185<K>** bucket = m.used ? static_cast<u32>(key) % m.bucket_count + m.buckets : nullptr;
    UnkSndNode_b0185<K>* old_head = *bucket;
    UnkSndNode_b0185<K>* n = static_cast<UnkSndNode_b0185<K>*>(m.allocator->vf_0x40(sizeof(UnkSndNode_b0185<K>), 8, m.flags));
    if (n) {
        n->key = key;
        n->value = nullptr;
        n->next = nullptr;
        n->prev = nullptr;
        n->chain = nullptr;
    }
    n->chain = old_head;
    *bucket = n;
    UnkSndNode_b0185<K>** link = &m.head;
    if (!*link) {
        *link = n;
    } else if (m.tail) {
        // the original reloads the tail after storing the back link
        UnkSndNode_b0185<K>* last = m.tail;
        __builtin_memcpy(&n->prev, &last, sizeof last);
        link = &m.tail->next;
        *link = n;
    }
    m.tail = n;
    ++m.count;
}

// fn_71000faac4 under a local name whose payload parameter is marked noescape (as
// src/base/snd/CSoundSystemATK/CSoundSystemATK_Setters.cpp does): in the original unit the definition is
// visible, so the payload is known not to be captured and a tail call may follow.
void QueueSoundCommand_b0185(UnkSoundSystemATK* self, s32 command, __attribute__((noescape)) const void* payload) asm(
    "_Z13fn_71000faac4PN4base3snd17UnkSoundSystemATKEiPKv");

// A command's payload for the second queue: an id and two words.
struct UnkSndIdPayload_b0185 {
    u32 id;
    u32 a;
    u32 b;
};



// ---- fn_71001030d8: a sound's volume from a level ----

void fn_71000f5348(void* sound, f32 volume);
void fn_71000f5360(void* sound, s32 a, f32 b);

struct UnkSndLevel_b0185 {
    u32 id;
    u32 level;
};


// ---- removing a sound's id ----

// Takes the node for `key` out of its bucket and the list of all nodes and frees it (the layout of
// src/base/UnkUnit122/UnkChainedMapErase_b54.cpp, on 32-bit keys). The list links go through accessors
// (their stores may alias each other).
template <class K>
static inline __attribute__((always_inline)) void EraseNode_b0185(UnkSndMap_b0185<K>& m, const K& key) {
    typedef UnkSndNode_b0185<K> Node;
    Node** bucket = static_cast<u32>(key) % m.bucket_count + m.buckets;
    Node* n = *bucket;
    if (!n)
        return;
    if (n->key == key) {
        *bucket = n->chain;
    } else {
        Node* before;
        do {
            before = n;
            n = n->chain;
            if (!n)
                return;
        } while (n->key != key);
        before->chain = n->chain;
    }
    if (m.head == n)
        m.head = n->next;
    if (m.tail == n)
        m.tail = n->Prev();
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    m.allocator->vf_0x48(n);
    --m.count;
}

struct UnkFreeHookHost_b0185 {
    u64 field_0x0;
    u8 sub[0x58];  // a base::UnkSubAllocator
    void (*on_free)(void* user, UnkFreeHookHost_b0185* self);
    void* user;
};

struct UnkAtkHeap;
extern UnkAtkHeap g_unk_7108b69038;
void fn_71000f4414(void* sound);
void fn_7100081420(base::UnkSubAllocator* self, void* ptr);


// ---- fn_7100101fb0: removing a sound's id and dropping its key's use ----

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


// ---- fn_7100103210: counting a sound against a key ----

// A tracked sound: its id, how often it was registered (+0x14) and the key it belongs to (+0x18).
struct UnkSndTracked_b0185 {
    u64 id;
    u8 pad_0x8[0xc];
    u32 hits;
    u64 key;
};


// ---- fn_71000f9450: the tunable values mirrored into the sound system ----

struct UnkTunable;
extern UnkTunable g_unk_7101d68580;
extern f32 g_unk_7101a1bd10 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd14 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd18 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd1c __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd20 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd24 __attribute__((visibility("hidden")));
extern f32 g_unk_7101a1bd28 __attribute__((visibility("hidden")));

static inline const f32* Tunables_b0185() {
    return reinterpret_cast<const f32*>(reinterpret_cast<const u8*>(&g_unk_7101d68580) + 0x24);
}

// A member of the sound system addressed through an integer: the original reloads the compared values
// after the stores to such members (a plain offset from the pointer is known not to alias the globals).
template <class T>
static inline T& Member_b0185(void* self, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<u64>(self) + offset);
}


// ---- fn_71001004ec: installing an effect ----

#include <nn/util.h>

extern u8 g_unk_7108b6af50[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6ae38[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6afe8[] __attribute__((visibility("hidden")));
extern u8 g_unk_7108b6adb0[] __attribute__((visibility("hidden")));
bool fn_7101234dc4(void* effect);
void fn_7101234dcc(void* effect, u32 value);
u64 fn_71012346b8(void* effect);
bool fn_7101233f24(void* effect);
void fn_7101233f2c(void* effect, u32 value);
u64 fn_7101233880(void* effect);
bool fn_7101234580(void* effect);
void fn_7101234588(void* effect, u32 value);
u64 fn_7101234068(void* effect);
void fn_710120b230(u32 bus, void* effect, void* buffer, u64 size);

struct UnkSndEffects_b0185 {
    u8 pad_0x0[0x80];
    void* delay_buffer;   // 0x80
    void* chorus_buffer;  // 0x88
    void* reverb_buffer;  // 0x90
    void* i3dl2_buffer;   // 0x98
};


// ---- the sound files by key ----

// Looks the file `key` up in the system's map of loaded files.
static inline UnkSndFileNode_b0185* FindFile_b0185(UnkSndSys_b0185* self, u64 key) {
    if (!self->files_used)
        return nullptr;
    UnkSndFileNode_b0185** bucket = static_cast<u32>(key) % self->files_buckets + self->files;
    if (!bucket)
        return nullptr;
    UnkSndFileNode_b0185* n = *bucket;
    for (; n; n = n->chain) {
        if (n->key == key)
            break;
    }
    return n;
}

// The node of a file the map is known to hold.
static inline UnkSndFileNode_b0185* GetFile_b0185(UnkSndSys_b0185* self, u64 key) {
    UnkSndFileNode_b0185** bucket = static_cast<u32>(key) % self->files_buckets + self->files;
    UnkSndFileNode_b0185* n = *bucket;
    while (n->key != key)
        n = n->chain;
    return n;
}


// ---- releasing and retaining a sound file ----

// Drops `key` from the list of loaded files' keys.
static inline void RemoveKey_b0185(base::UnkVector<u64>& keys, u64 key) {
    u32 count = keys.count;
    for (u32 i = 0; i < count; ++i) {
        u64* data = keys.data;
        if (data[i] == key) {
            memcpy(&data[i], &data[i + 1], (count - i - 1) * sizeof(u64));
            --keys.count;
            break;
        }
    }
}

// A command's payload for the file commands: the key and, for the release, a flag.
struct UnkSndFilePayload_b0185 {
    u64 key;
    u32 flag;
};



// ---- fn_71000fe418: the playing sound of a path ----

struct UnkSoundRef;
struct UnkSoundHandle {
    UnkSoundRef* ref;
    s64 serial;
};
extern const UnkSoundHandle g_unk_7108b6c460;  // the empty handle

struct UnkPlayingHandle_ovb0074 {
    void* ref;
    u64 serial;
};

struct UnkVoicePool_b56;
struct UnkSoundVoice2_b56;
u64 fn_7100083854(UnkVoicePool_b56* self, UnkSoundVoice2_b56* voice);

// A slot of the voice statistics: what the handle of a voice points at.
struct UnkVoiceStat_b0185 {
    u64 field_0x0;
    u64 released;
};

struct UnkSndPlaying_b0185 {
    u8 pad_0x0[0x398];
    u8 pool[0x28];                     // 0x398
    UnkVoiceStat_b0185* stats;         // 0x3c0
};


// ---- fn_71000fec3c: starting or stopping the fading of the voices ----

void fn_7100105bac(UnkSndVoice_b0185* sound, f32 value);
struct UnkPlayingSound_ovb0074;
void fn_7100105c00(UnkPlayingSound_ovb0074* sound, f32 value);


// ---- fn_71000feff8: a level for the voices on some channels ----

struct UnkSoundHandle_b46a1 {
    u32 value;
    UnkSoundHandle_b46a1(const UnkSoundHandle_b46a1& other) : value(other.value) {}
};
extern const f32 g_unk_71016479f8[] __attribute__((visibility("hidden")));
void fn_7100105ef4(void* sound, f32 time, f32 step, bool arg3);

struct UnkSndChannelVoice_b0185 {
    u8 pad_0x0[0xb0];
    s32 channel;  // 0xb0
};

struct UnkSndChannels_b0185 {
    u8 pad_0x0[0x348];
    u32 mask;            // 0x348
    s32 levels[8];       // 0x34c
    u8 pad_0x36c[0x3f0 - 0x36c];
    base::UnkVector<UnkSndChannelVoice_b0185*> voices_a;  // 0x3f0
    base::UnkVector<UnkSndChannelVoice_b0185*> voices_b;  // 0x418
    u8 pad_0x440[0x400568 - 0x440];
    nn::os::MutexType mutex;  // 0x400568
};

// The level of a channel, addressed from raw bytes (the original forms the address again each time).
static inline s32 LevelOf_b0185(UnkSndChannels_b0185* self, s32 channel) {
    return *reinterpret_cast<s32*>(reinterpret_cast<u8*>(self) + static_cast<s64>(channel) * 4 + 0x34c);
}


// ---- fn_71000f44cc: restarting a voice with new entries ----

struct UnkSoundCommand;

#pragma pack(push, 4)
// One entry of a voice's list (32 bytes, 28 of them copied).
struct UnkSndEntry_b0185 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u32 field_0x18;
    u32 field_0x1c;

    UnkSndEntry_b0185& operator=(const UnkSndEntry_b0185& other) {
        __builtin_memcpy(this, &other, 0x1c);
        return *this;
    }
};

// The list of entries: a count and room for eight.
struct UnkSndEntries_b0185 {
    u32 count;
    UnkSndEntry_b0185 entries[8];

    __attribute__((always_inline)) UnkSndEntries_b0185& operator=(const UnkSndEntries_b0185& other) {
        if (this != &other) {
            u32 n = count;
            if (n > other.count) {
                for (u32 i = 0; i < other.count; ++i)
                    entries[i] = other.entries[i];
            } else {
                for (u32 i = 0; i < n; ++i)
                    entries[i] = other.entries[i];
            }
            // the entries past the old count are copied whole
            for (u32 i = n; i < other.count; ++i) {
                __builtin_memcpy(&entries[i], &other.entries[i], sizeof(UnkSndEntry_b0185));
                count = i + 1;
            }
            count = other.count;
        }
        return *this;
    }
};
#pragma pack(pop)

void fn_710120d54c(void* sound, u32 frames);
void fn_7101209300(void** handle);
void fn_7101206630(u8* player, u32 frames);
void fn_7101206578(u8* player);
void fn_7101206444(u8* player, void* target);

struct UnkSndRestart_b0185 {
    u8 pad_0x0[0x28];
    void** handle;  // 0x28
    u8* player;     // 0x30
    UnkSndEntries_b0185 entries;  // 0x38
    u8 pad_0x13c_[0x13c - 0x38 - sizeof(UnkSndEntries_b0185)];
    s32 entry;  // 0x13c
    u8 pad_0x140[0x16c - 0x140];
    bool enabled;  // 0x16c
    u8 field_0x16d[3];
    bool field_0x170_16f;
};


// ---- fn_71001010e8: dropping a use of a loaded sound ----

extern UnkAtkHeap g_unk_7108b690a8;
extern u8 g_unk_7108b6b3c0[] __attribute__((visibility("hidden")));
void fn_7101209274(void* manager, void* sound, u32 value);

// A loaded sound the system counts uses of: its key, how many users it has left (+0x10), the keys of
// what it was loaded for (+0xa0) and the engine's sound (+0xa8).
struct UnkSndLoaded_b0185 {
    u64 key;
    u8 pad_0x8[4];
    u32 field_0xc;
    u32 users;       // 0x10
    u32 field_0x14;
    u8 pad_0x18[0x80];
    u32 key_count;   // 0x98
    u8 pad_0x9c[4];
    u64* keys;       // 0xa0
    void* sound;     // 0xa8
};

struct UnkSndUses_b0185 {
    u8 pad_0x0[0x18];
    u64 field_0x18;
};

struct UnkSndLoadedSys_b0185 {
    u8 pad_0x0[0x400840];
    base::UnkVector<u64> loaded;  // 0x400840
    u8 pad_0x400868[0x400878 - 0x400868];
    UnkSndMap_b0185<u64>* uses;  // 0x400878
};

// Frees an engine sound out of the heap (its free hook, then the sub-allocator).
static inline void FreeSound_b0185(void* sound) {
    UnkFreeHookHost_b0185* heap = reinterpret_cast<UnkFreeHookHost_b0185*>(&g_unk_7108b690a8);
    if (heap->on_free)
        heap->on_free(heap->user, heap);
    fn_7100081420(reinterpret_cast<base::UnkSubAllocator*>(heap->sub), sound);
}

// Takes `key` out of the list of loaded keys by moving the last one over it.
static inline void RemoveLoaded_b0185(base::UnkVector<u64>& keys, u64 key) {
    for (u32 i = 0; i < keys.count; ++i) {
        if (keys.data[i] == key) {
            u32 last = keys.count - 1;
            if (last > i)
                keys.data[i] = keys.data[last];
            keys.count = last;
            return;
        }
    }
}

