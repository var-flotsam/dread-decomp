// The sound system of unit 99 (base::snd::CSoundSystemATK): the command queue its setters feed.
#include <string.h>

#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntString.h"
#include "base/snd/UnkSoundSystemATK.h"

using base::snd::UnkSoundSystemATK;
extern void* g_unk_7101a1b750[];

// A flag that turns the queue off (its meaning is unverified); a direct (not GOT) access.
extern u8 g_unk_7108b6ab60 __attribute__((visibility("hidden")));
// The payload size of each command.
extern const s32 g_unk_7101647a0c[] __attribute__((visibility("hidden")));

// Probably whether the caller is on the sound thread (it compares a stored thread id with the
// current one); the result is not used here.
bool fn_7100132cd0();

void fn_71000faac4(UnkSoundSystemATK* self, s32 command, const void* payload) {
    // The hint only steers the block layout: the original keeps the return next to the test.
    if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
        return;
    fn_7100132cd0();
    s32 size = g_unk_7101647a0c[command];
    nn::os::MutexType* mutex = &self->field_0x400528;
    fn_710006691c(mutex);
    while (size + 4 + self->field_0x508 >= 0x100000) {
        fn_7100066920(mutex);
        nn::os::SleepThread(nn::TimeSpan::FromNanoSeconds(1000000));
        fn_710006691c(mutex);
    }
    u8* buffer = self->field_0x520[self->field_0x500];
    *reinterpret_cast<s32*>(buffer + self->field_0x508) = command;
    self->field_0x508 += 4;
    s32 sequence = self->field_0x400520++;
    *reinterpret_cast<s32*>(buffer + self->field_0x508) = sequence;
    self->field_0x508 += 4;
    memcpy(buffer + self->field_0x508, payload, size);
    self->field_0x508 += size;
    fn_7100066920(mutex);
}

// ---- fn_71000f63b0: the sanitised file-path id ----

using base::global::CFilePathStrId;
using base::global::CRntString;

// A string with 0x80 bytes of inline storage (as in src/game_aca57c/fn_7100b2de48.cpp).
struct UnkInlineString {
    u8 str[0x30];
    void** vtable;
    u8 buffer[0x80];
    u64 field_0xb8;
    u32 field_0xc0;

    UnkInlineString() {
        fn_710007e6e4(reinterpret_cast<CRntString*>(this), reinterpret_cast<base::UnkAllocator*>(&vtable));
        field_0xb8 = 0;
        field_0xc0 = 0;
        vtable = &g_unk_7101a1b750[2];
    }
    // The second string's order of stores differs in the original.
    explicit UnkInlineString(int) {
        fn_710007e6e4(reinterpret_cast<CRntString*>(this), reinterpret_cast<base::UnkAllocator*>(&vtable));
        field_0xb8 = 0;
        vtable = &g_unk_7101a1b750[2];
        field_0xc0 = 0;
    }
    ~UnkInlineString() {
        fn_710007ed40(reinterpret_cast<CRntString*>(this));
        fn_710007ecd4(reinterpret_cast<CRntString*>(this));
    }
};
static_assert(sizeof(UnkInlineString) == 0xc8, "UnkInlineString layout");

// A node of the id map's chains (placeholder names; 0x28 bytes).
struct UnkIdNode {
    u64 key;
    CFilePathStrId id;  // the sanitised id
    UnkIdNode* field_0x10;
    UnkIdNode* prev;  // the previous node of the node list
    UnkIdNode* next;  // the next node of the bucket's chain
};
static_assert(sizeof(UnkIdNode) == 0x28, "UnkIdNode layout");

// The id map (the layout of base/UnkHashMap.h, placeholder names).
struct UnkIdMap {
    base::UnkAllocator* allocator;  // +0x0
    u32 flags;                      // +0x8
    u32 count;                      // +0xc
    UnkIdNode** buckets;            // +0x10
    u32 used;                       // +0x18
    u32 bucket_count;               // +0x1c
    u8 pad_0x20[0x18];
    UnkIdNode* last;     // +0x38
    UnkIdNode* head;     // +0x40: the list's first node
};
static_assert(__builtin_offsetof(UnkIdMap, head) == 0x40, "UnkIdMap layout");

extern UnkIdMap* g_unk_7101d685c8;
// The scratch buffer the sanitised name is built in.
extern char g_unk_7108b6b910[] __attribute__((visibility("hidden")));

// NON_MATCHING(minor): same behaviour; the original builds the new id in a stack slot and moves it into the
// result with a plain store (no count update, no destructor) where ours copy-constructs it and destroys the
// local, reloads the list's last node after linking, and swaps two registers (key and old head) in the insertion.
CFilePathStrId fn_71000f63b0(const CFilePathStrId& id) {
    fn_7100132cd0();
    if (g_unk_7108b6ab60 == 1)
        return g_unk_7101d470e8;
    char* name = reinterpret_cast<char*>(id.data);
    if (id.data == &g_unk_7101d47200->empty)
        return id;
    UnkIdMap* map = g_unk_7101d685c8;
    if (map && map->used) {
        u32 key32 = *reinterpret_cast<u64*>(name + 0x30);
        u64 key = *reinterpret_cast<u64*>(name + 0x30);
        UnkIdNode** bucket = key32 % map->bucket_count + map->buckets;
        if (bucket) {
            UnkIdNode* head = *bucket;
            for (UnkIdNode* n = head; n; n = n->next) {
                if (n->key == key) {
                    UnkIdNode* found = head;
                    while (found->key != key)
                        found = found->next;
                    return found->id;
                }
            }
        }
    }
    typedef s32 __attribute__((may_alias)) alias_s32;
    ++*reinterpret_cast<alias_s32*>(name + 0x44);
    char lower[128];
    const CRntString* source_name = reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(id.data) + 0x10);
    UnkInlineString text;
    fn_710007eaa0(reinterpret_cast<CRntString*>(&text), source_name);
    lower[0] = 0;
    u64 full_length = __builtin_strlen(*reinterpret_cast<const char**>(&text));
    if (static_cast<int>(full_length) != 0) {
        u64 length = full_length & 0xffffffff;
        u64 i = 0;
        do {
            u8 c = (*reinterpret_cast<const u8**>(&text))[i];
            lower[i] = static_cast<u32>(c - 'A') < 26u ? c + 32 : c;
        } while (++i != length);
        lower[length] = 0;
    }
    {
        UnkInlineString other(0);
        fn_710007e77c(reinterpret_cast<CRntString*>(&other), lower);
        fn_710007eaa0(reinterpret_cast<CRntString*>(&text), reinterpret_cast<CRntString*>(&other));
    }
    u64 remaining = *reinterpret_cast<u32*>(&text.str[8]);
    const char* source = "";
    if (*reinterpret_cast<const char**>(&text))
        source = *reinterpret_cast<const char**>(&text);
    strcpy(g_unk_7108b6b910, source);
    if (remaining != 0) {
        u8* p = reinterpret_cast<u8*>(g_unk_7108b6b910);
        do {
            if (0x19 < static_cast<u32>(*p - 0x61) && 9 < (static_cast<u32>(*p - 0x30) & 0xff))
                *p = '_';
            ++p;
            --remaining;
        } while (remaining != 0);
    }
    CFilePathStrId made(g_unk_7108b6b910, true);
    if (UnkIdMap* m = g_unk_7101d685c8) {
        u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(id.data) + 0x30);
        UnkIdNode** bucket = m->used ? m->buckets + static_cast<u32>(key) % m->bucket_count : nullptr;
        UnkIdNode* old_head = *bucket;
        UnkIdNode* n = static_cast<UnkIdNode*>(m->allocator->vf_0x40(sizeof(UnkIdNode), 8, m->flags));
        if (n) {
            n->key = key;
            new (&n->id) CFilePathStrId(made);
            n->field_0x10 = nullptr;
            n->prev = nullptr;
            n->next = nullptr;
        }
        n->next = old_head;
        *bucket = n;
        UnkIdNode** link = &m->head;
        if (!*link) {
            *link = n;
        } else if (m->last) {
            n->prev = m->last;
            link = &m->last->field_0x10;
            *link = n;
        }
        m->last = n;
        ++m->count;
    }
    return made;
}

// ---- fn_71000ffe2c: the voice commands ----

// A voice the sound system keeps (placeholder names): its id and the values the commands set.
struct UnkSoundVoice {
    u8 pad_0x0[0xc4];
    s32 id;
    s32 field_0xc8;
    u8 pad_0xcc[0x48];
    s32 field_0x114;
    u8 pad_0x118;
    bool field_0x119;
};

// The members of the sound system the voice commands touch (base/snd/UnkSoundSystemATK.h spells
// out the queue's): a three-word setting per slot, two lists of voices and the mutex that guards
// them.
struct UnkSoundSystemVoices {
    u8 pad_0x0[0x214];
    s32 field_0x214[1][3];
    u8 pad_0x220[0x1d0];
    UnkSoundVoice** field_0x3f0;
    u32 field_0x3f8;
    u8 pad_0x3fc[0x1c];
    UnkSoundVoice** field_0x418;
    u32 field_0x420;
    u8 pad_0x424[0x400568 - 0x424];
    nn::os::MutexType field_0x400568;
};
static_assert(__builtin_offsetof(UnkSoundSystemVoices, field_0x420) == 0x420, "UnkSoundSystemVoices placeholder layout");
static_assert(__builtin_offsetof(UnkSoundSystemVoices, field_0x400568) == 0x400568, "UnkSoundSystemVoices placeholder layout");

// Runs `set` on the first voice of the list with the payload's id (its first word).
template <class F>
static inline void ForVoice(UnkSoundVoice** voices, u32 count, const s32* payload, F set) {
    for (UnkSoundVoice **it = voices, **end = voices + count; it != end; ++it) {
        if ((*it)->id == payload[0]) {
            set(*it);
            break;
        }
    }
}

void fn_7100100088(UnkSoundSystemATK* self, const s32* payload);

// Applies a voice command (a payload of words, the voice's id first) under the voice mutex: 0 sets
// a voice's flag at +0x119 (payload[1] picks the list), 1 is fn_7100100088's, 2 sets a slot's three
// words, 3 a voice's +0xc8 (payload[2] picks the list) and 4 a voice's +0x114 in the second list.
// Nothing happens while the global flag g_unk_7108b6ab60 is set.
void fn_71000ffe2c(UnkSoundSystemATK* self, u32 command, const s32* payload) {
    if (!payload)
        return;
    UnkSoundSystemVoices* voices = reinterpret_cast<UnkSoundSystemVoices*>(self);
    switch (command) {
    case 0: {
        if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
            return;
        fn_7100132cd0();
        nn::os::MutexType* mutex = &voices->field_0x400568;
        fn_710006691c(mutex);
        auto set = [payload](UnkSoundVoice* voice) { voice->field_0x119 = payload[2] != 0; };
        if (payload[1])
            ForVoice(voices->field_0x418, voices->field_0x420, payload, set);
        else
            ForVoice(voices->field_0x3f0, voices->field_0x3f8, payload, set);
        fn_7100066920(mutex);
        return;
    }
    case 1:
        fn_7100100088(self, payload);
        return;
    case 2: {
        if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
            return;
        fn_7100132cd0();
        nn::os::MutexType* mutex = &voices->field_0x400568;
        fn_710006691c(mutex);
        s32* slot = voices->field_0x214[payload[0]];
        slot[0] = payload[1];
        slot[1] = payload[2];
        slot[2] = payload[3];
        fn_7100066920(mutex);
        return;
    }
    case 3: {
        if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
            return;
        fn_7100132cd0();
        nn::os::MutexType* mutex = &voices->field_0x400568;
        fn_710006691c(mutex);
        auto set = [payload](UnkSoundVoice* voice) { voice->field_0xc8 = payload[1]; };
        if (payload[2])
            ForVoice(voices->field_0x418, voices->field_0x420, payload, set);
        else
            ForVoice(voices->field_0x3f0, voices->field_0x3f8, payload, set);
        fn_7100066920(mutex);
        return;
    }
    case 4: {
        if (__builtin_expect(g_unk_7108b6ab60 & 1, 1))
            return;
        fn_7100132cd0();
        nn::os::MutexType* mutex = &voices->field_0x400568;
        fn_710006691c(mutex);
        ForVoice(voices->field_0x418, voices->field_0x420, payload,
                 [payload](UnkSoundVoice* voice) { voice->field_0x114 = payload[1]; });
        fn_7100066920(mutex);
        return;
    }
    }
}

// ---- fn_7100100b1c: releasing a sound file ----

// A loaded file of the sound system's map (placeholder names): its id's hash, how many users it
// has, and the next node of its bucket's chain.
struct UnkSoundFileNode {
    u64 key;
    s32 users;
    u8 pad_0xc[0x14];
    UnkSoundFileNode* next;
};

// The members of the sound system the file functions touch: the keys of the loaded files and the
// map of them by key.
struct UnkSoundSystemFiles {
    u8 pad_0x0[0x490];
    u64* field_0x490;   // the keys
    u32 field_0x498;    // their count
    u8 pad_0x49c[0x2c];
    UnkSoundFileNode** field_0x4c8;  // the buckets
    u32 field_0x4d0;                 // nodes in use
    u32 field_0x4d4;                 // the bucket count
};
static_assert(__builtin_offsetof(UnkSoundSystemFiles, field_0x4d4) == 0x4d4, "UnkSoundSystemFiles placeholder layout");

// A file-path id built from a string out of line (fn_710001a270) and released like one.
struct UnkBuiltPathId {
    union {
        CFilePathStrId id;
    };

    explicit UnkBuiltPathId(const CRntString& str) { fn_710001a270(&id, str, true); }
    ~UnkBuiltPathId() { id.~CFilePathStrId(); }
};

void fn_710007f564(void* self, const char* str);  // appends a string

// Storage for a file-path id whose release is written out by hand.
union UnkPathIdSlot {
    CFilePathStrId id;
    UnkPathIdSlot() {}
    ~UnkPathIdSlot() {}
};

// The text of a file-path id's pooled data (a CRntString at +0x10), with the count of its reads at
// +0x44 (a word that may alias the id: the original reloads it after each increment).
static inline const CRntString& PathString(const CFilePathStrId& id) {
    typedef s32 __attribute__((may_alias)) alias_s32;
    ++*reinterpret_cast<alias_s32*>(reinterpret_cast<char*>(id.data) + 0x44);
    return *reinterpret_cast<const CRntString*>(reinterpret_cast<char*>(id.data) + 0x10);
}
static inline const char* PathChars(const CFilePathStrId& id) {
    const char* s = PathString(id).data;
    return s ? s : g_unk_710162e2ed;
}

// Releases the sound file `path` names (made absolute with a leading '/', then sanitised by
// fn_71000f63b0): when the map knows it, the sound thread is told (command 30 with its key), and
// the file's last user also takes its key off the list. Whether the map knew it; true while the
// global flag g_unk_7108b6ab60 is set.
// NON_MATCHING(minor): block order only: the flag-set path (`mov w20, #1`) is placed just before the
// shared epilogue (the unlock call then branches over it) where the original places it right after
// the flag test with a branch to the epilogue; the instructions are the same.
bool fn_7100100b1c(UnkSoundSystemATK* self, const CFilePathStrId& path) {
    bool found = true;
    if (!(g_unk_7108b6ab60 & 1)) {
        fn_7100132cd0();
        UnkSoundSystemFiles* files = reinterpret_cast<UnkSoundSystemFiles*>(self);
        nn::os::MutexType* mutex = &reinterpret_cast<UnkSoundSystemVoices*>(self)->field_0x400568;
        fn_710006691c(mutex);
        found = false;
        {
            CRntString name(PathString(path));
            if (path.data != &g_unk_7101d47200->empty && PathChars(path)[0] != '/') {
                fn_710007e77c(&name, "/");
                fn_710007f564(&name, PathChars(path));
            }
            // The sanitised id, released by hand on its data pointer read once: the original knows
            // that nothing below changes the id (its fn_71000f63b0 does not keep the address).
            UnkPathIdSlot slot;
            new (&slot.id) CFilePathStrId(fn_71000f63b0(UnkBuiltPathId(name).id));
            base::global::UnkStringData* data = slot.id.data;
            if (files->field_0x4d0) {
                u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
                u32 key32 = key;
                UnkSoundFileNode** bucket = key32 % files->field_0x4d4 + files->field_0x4c8;
                if (bucket) {
                    for (UnkSoundFileNode* node = *bucket; node; node = node->next) {
                        if (node->key == key) {
                            u64 command = key;
                            fn_71000faac4(self, 0x1e, &command);
                            UnkSoundFileNode* n = *(key32 % files->field_0x4d4 + files->field_0x4c8);
                            while (n->key != key)
                                n = n->next;
                            if (--n->users == 0) {
                                u32 count = files->field_0x498;
                                for (u32 i = 0; i < count; ++i) {
                                    u64* keys = files->field_0x490;
                                    if (keys[i] == key) {
                                        memcpy(&keys[i], &keys[i + 1], (count - i - 1) * sizeof(u64));
                                        --files->field_0x498;
                                        break;
                                    }
                                }
                            }
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
                if (data && data != &pool->empty) {
                    if (--data->ref_count == 0) {
                        fn_71000803a4(pool, data);
                        pool = g_unk_7101d47200;
                    }
                }
                slot.id.data = &pool->empty;
            }
        }
        fn_7100066920(mutex);
    }
    return found;
}

// ---- fn_71000fe1f8: a playing stream's position ----

// A stream the sound system plays (placeholder name): fn_71000f4e44 gives the hash of its file's
// id (0 while it has none), fn_71000f4b14 how far it has played (its position over its rate).
struct UnkSoundStream;
u64 fn_71000f4e44(UnkSoundStream* stream);
f32 fn_71000f4b14(UnkSoundStream* stream);

// A node of the sound system's list of streams.
struct UnkSoundStreamNode {
    u8 pad_0x0[0x8];
    UnkSoundStream* stream;
    UnkSoundStreamNode* next;
};

struct UnkSoundSystemStreams {
    u8 pad_0x0[0x400810];
    UnkSoundStreamNode* field_0x400810;  // the first node
};

// How far the first stream of the file `path` names (sanitised by fn_71000f63b0) has played,
// under the voice mutex; 0 when no stream plays it or while the global flag g_unk_7108b6ab60 is set.
// The sanitised id is released by hand, as in fn_7100100b1c.
// NON_MATCHING(minor): block order only: the flag-set path (`fmov s8, wzr`) is placed just before
// the shared epilogue (the unlock call then branches over it) where the original places it right
// after the flag test with a branch to the epilogue; the instructions are the same.
f32 fn_71000fe1f8(UnkSoundSystemATK* self, const CFilePathStrId& path) {
    f32 position;
    if (g_unk_7108b6ab60 & 1) {
        position = 0.0f;
    } else {
        fn_7100132cd0();
        nn::os::MutexType* mutex = &reinterpret_cast<UnkSoundSystemVoices*>(self)->field_0x400568;
        fn_710006691c(mutex);
        UnkPathIdSlot slot;
        new (&slot.id) CFilePathStrId(fn_71000f63b0(path));
        base::global::UnkStringData* data = slot.id.data;
        u64 key = *reinterpret_cast<u64*>(reinterpret_cast<char*>(data) + 0x30);
        if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
            if (data && data != &pool->empty) {
                if (--data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47200;
                }
            }
            slot.id.data = &pool->empty;
        }
        position = 0.0f;
        for (UnkSoundStreamNode* node = reinterpret_cast<UnkSoundSystemStreams*>(self)->field_0x400810;
             node; node = node->next) {
            UnkSoundStream* stream = node->stream;
            if (stream && fn_71000f4e44(stream) == key) {
                position = fn_71000f4b14(stream);
                break;
            }
        }
        fn_7100066920(mutex);
    }
    return position;
}

// ---- fn_71000f68bc: the sound system's constructor ----

#include "base/UnkBlockFreeList.h"
#include "base/UnkChainedMap.h"
#include "base/UnkVector.h"

// A sub-object at 0x278 whose constructor (fn_71001045c0) builds the array at its +8.
struct UnkSoundSub_0x278 {
    u8 pad_0x0[0xc8];
};
void fn_71001045c0(UnkSoundSub_0x278* self);

// An array of words with a growth step and the room for `n` of them reserved.
static inline void UnkInitWords(base::UnkVector<u64>* v, u32 step, u32 n) {
    v->data = nullptr;
    v->count = 0;
    v->capacity = 0;
    v->step = step;
    v->field_0x14 = true;
    v->flags = 0xb;
    v->allocator = fn_7100083990();
    v->Reserve(n);
}

// The members from 0x388: a word, the block free list and an array.
struct UnkSoundSub_0x388 {
    u64 field_0x0;
    u8 pad_0x8[8];
    UnkBlockFreeList list;
    base::UnkVector<u64> field_0x38;
};

// What the constructor sets (placeholder names; the command queue of base/snd/UnkSoundSystemATK.h
// lies between 0x500 and 0x400788 and is left alone).
struct UnkSoundSystemMembers {
    UnkChainedMap field_0x0;
    u8 pad_0x48[0x58];
    u64 field_0xa0[6];
    u8 pad_0xd0;
    bool field_0xd1;
    u8 pad_0xd2[2];
    f32 field_0xd4;
    f32 field_0xd8;
    u32 field_0xdc;
    f32 field_0xe0;
    f32 field_0xe4;
    u32 field_0xe8;
    u8 pad_0xec[4];
    u64 field_0xf0[20];
    u32 field_0x190;
    u8 pad_0x194[0xe4];
    UnkSoundSub_0x278 field_0x278;
    bool field_0x340[7];
    u8 pad_0x347;
    u32 field_0x348;
    u8 pad_0x34c[0x20];
    bool field_0x36c;
    u8 pad_0x36d[3];
    u32 field_0x370;
    u8 pad_0x374[4];
    f32 field_0x378;
    u8 pad_0x37c[4];
    u32 field_0x380;
    u8 pad_0x384[4];
    UnkSoundSub_0x388 field_0x388;
    u8 pad_0x3e8[8];
    base::UnkVector<u64> field_0x3f0;
    base::UnkVector<u64> field_0x418;
    base::UnkVector<u64> field_0x440;
    base::UnkVector<u64> field_0x468;
    base::UnkVector<u64> field_0x490;
    UnkChainedMap field_0x4b8;
    u8 pad_0x500[0x400788 - 0x500];
    UnkChainedMap field_0x400788;
    UnkChainedMap field_0x4007d0;  // the streams (fn_71000fe1f8 walks it)
    base::UnkVector<u64> field_0x400818;
    base::UnkVector<u64> field_0x400840;
    u8 pad_0x400868[8];
    u64 field_0x400870;
    u64 field_0x400878;
};
static_assert(__builtin_offsetof(UnkSoundSystemMembers, field_0x278) == 0x278, "UnkSoundSystemMembers layout");
static_assert(__builtin_offsetof(UnkSoundSystemMembers, field_0x388) == 0x388, "UnkSoundSystemMembers layout");
static_assert(__builtin_offsetof(UnkSoundSystemMembers, field_0x4b8) == 0x4b8, "UnkSoundSystemMembers layout");
static_assert(__builtin_offsetof(UnkSoundSystemMembers, field_0x400818) == 0x400818, "UnkSoundSystemMembers layout");
static_assert(__builtin_offsetof(UnkSoundSystemMembers, field_0x400870) == 0x400870, "UnkSoundSystemMembers layout");

// Builds the sound system's maps (11 buckets each), arrays (four reserved: three for 32 words
// growing by 16, one for 0x800 growing by 0x200, and one more of those at the end), the block free
// list at 0x398 and the sub-object at 0x278, and sets its scalar members (1.0 scales, a 4).
// NON_MATCHING(minor): the same stores and calls; the original addresses the arrays at 0x3c0-0x468
// and the maps at 0x4b8 and 0x400788 through a register holding the member's address (and merges the
// maps' zeroed words into other store pairs), so the instructions from 0x3c0's array on are scheduled
// and registered differently.
void fn_71000f68bc(UnkSoundSystemATK* object) {
    UnkSoundSystemMembers* self = reinterpret_cast<UnkSoundSystemMembers*>(object);
    new (&self->field_0x0) UnkChainedMap();
    self->field_0x0.ResizeBuckets(11);
    for (u32 i = 0; i < 6; ++i)
        self->field_0xa0[i] = 0;
    self->field_0xd1 = true;
    self->field_0xd4 = 1.0f;
    self->field_0xd8 = 1.0f;
    self->field_0xdc = 4;
    self->field_0xe0 = 1.0f;
    self->field_0xe4 = 1.0f;
    self->field_0xe8 = 0;
    for (u32 i = 0; i < 20; ++i)
        self->field_0xf0[i] = 0;
    self->field_0x190 = 0;
    fn_71001045c0(&self->field_0x278);
    self->field_0x348 = 0;
    self->field_0x36c = false;
    self->field_0x370 = 0;
    for (u32 i = 0; i < 7; ++i)
        self->field_0x340[i] = false;
    self->field_0x378 = 1.0f;
    self->field_0x380 = 1;
    UnkSoundSub_0x388* sub = &self->field_0x388;
    sub->field_0x0 = 0;
    fn_710008377c(&sub->list);
    new (&sub->field_0x38) base::UnkVector<u64>(0xf);
    UnkInitWords(&self->field_0x3f0, 0x10, 0x20);
    UnkInitWords(&self->field_0x418, 0x10, 0x20);
    UnkInitWords(&self->field_0x440, 0x10, 0x20);
    UnkInitWords(&self->field_0x468, 0x200, 0x800);
    new (&self->field_0x490) base::UnkVector<u64>(0xf);
    new (&self->field_0x4b8) UnkChainedMap();
    self->field_0x4b8.ResizeBucketsPtrFirst(11);
    new (&self->field_0x400788) UnkChainedMap();
    self->field_0x400788.ResizeBucketsPtrFirst(11);
    new (&self->field_0x4007d0) UnkChainedMap();
    self->field_0x4007d0.ResizeBucketsPtrFirst(11);
    UnkInitWords(&self->field_0x400818, 0x200, 0x800);
    new (&self->field_0x400840) base::UnkVector<u64>(0xf);
    self->field_0x400870 = 0;
    self->field_0x400878 = 0;
}
