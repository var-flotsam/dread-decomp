// The sound system of unit 99 (base::snd::CSoundSystemATK): the command queue its setters feed, the
// stop command (fn_71000ffa68), the set-up (fn_71000f7800) and the listener's matrix (fn_71000f88b8).
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

// A string with 0x80 bytes of inline storage (as in src/game/GUI/CIngameMenu/fn_7100b2de48.cpp).
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

// ---- fn_71000ffa68: stopping a sound ----

// The game's handle on a playing sound, as the stop command reads it: the voice's id, a second
// id sent after the stop and whether the voice has one.
struct UnkSoundHandle {
    u8 pad_0x0[0xc4];
    s32 id;
    u8 pad_0xc8[0x24];
    s32 field_0xec;
    u8 pad_0xf0[0x30];
    bool field_0x120;
};
static_assert(__builtin_offsetof(UnkSoundHandle, field_0x120) == 0x120, "UnkSoundHandle placeholder layout");

// What the stop commands send: the voice's id, whether to skip the fade-out and the fade's length.
struct UnkSoundStop {
    s32 id;
    u32 immediate;
    f32 fade;
};

// The fade, at least 0.06 s (a NaN stays).
static inline f32 StopFade(f32 fade) { return !(fade <= 0.06f) ? fade : 0.06f; }

// Stops `sound` (command 0x17 when it has the second id, else 10). Unless `immediate` is set, a
// voice with the second id is then sent command 0x18.
// NON_MATCHING(minor): the stores of the 0x18 and 10 paths are scheduled after their argument moves.
void fn_71000ffa68(UnkSoundSystemATK* self, UnkSoundHandle* sound, bool immediate, f32 fade) {
    fn_7100132cd0();
    if (!sound)
        return;
    if (sound->field_0x120) {
        UnkSoundStop stop = {sound->id, immediate, StopFade(fade)};
        fn_71000faac4(self, 0x17, &stop);
        if (immediate)
            return;
        s32 next[2] = {sound->id, sound->field_0xec};
        fn_71000faac4(self, 0x18, next);
    } else {
        UnkSoundStop stop;
        stop.id = sound->id;
        stop.immediate = immediate;
        stop.fade = StopFade(fade);
        fn_71000faac4(self, 10, &stop);
    }
}

// ---- fn_71000f7800: setting up the sound system ----

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/UnkZeroVector3.h"

// The audio output types the check at the start uses. The matching build's SDK headers have no
// audio output; the port's runtime has neither these nor nn::os::SystemEvent, so those are spelled
// out here with the console's layout (an event and its state byte at +0x28).
namespace nn::audio {
struct AudioOutParameter {
    u32 sample_rate;
    u16 channel_count;
    u16 reserved;
};
struct AudioOut {
    u8 storage[0x168];
};
void InitializeAudioOutParameter(AudioOutParameter* parameter);
void CloseAudioOut(AudioOut* out);
}  // namespace nn::audio
#ifdef NNSDK
#include <nn/audio/audio_Common.h>
#else
namespace nn::os {
struct SystemEvent {
    u8 storage[0x28];
    u8 state;
};
}  // namespace nn::os
namespace nn::audio {
struct AudioDeviceName {
    char raw_name[0x100];
};
}  // namespace nn::audio
#endif
namespace nn::os {
void DestroySystemEvent(SystemEventType* event);
}  // namespace nn::os
namespace nn::audio {
s32 OpenDefaultAudioOut(AudioOut* out, nn::os::SystemEvent* event, const AudioOutParameter& parameter);
s32 SetAudioDeviceOutputVolume(const AudioDeviceName* device, f32 volume);
}  // namespace nn::audio

// An event the opener may set up, destroyed with the scope.
struct UnkScopedSystemEvent {
    nn::os::SystemEvent event;
    UnkScopedSystemEvent() { reinterpret_cast<u8*>(&event)[0x28] = 0; }
    ~UnkScopedSystemEvent() {
        if (reinterpret_cast<u8*>(&event)[0x28])
            nn::os::DestroySystemEvent(reinterpret_cast<nn::os::SystemEventType*>(&event));
    }
};

// Whether the console has an audio output to open (it is opened and closed again).
static inline bool CanOpenAudioOut() {
    nn::audio::AudioOut out;
    UnkScopedSystemEvent event;
    nn::audio::AudioOutParameter parameter;
    nn::audio::InitializeAudioOutParameter(&parameter);
    parameter.sample_rate = 0;
    parameter.channel_count = 0;
    s32 result = nn::audio::OpenDefaultAudioOut(&out, &event.event, parameter);
    if (result == 0)
        nn::audio::CloseAudioOut(&out);
    return result == 0;
}

// The sound engine's heaps: a sub-allocator over a static block, the engine's own heap object over
// the same memory at +0x40, and a hook run after every allocation (with the heap; it may replace
// the result at +0x48).
struct UnkAtkHeap {
    u64 field_0x0;
    base::UnkSubAllocator allocator;  // 0x8
    u8 heap[8];                       // 0x40, nn::atk's
    void* last;                       // 0x48
    void (*hook)(void* arg, UnkAtkHeap* heap);
    void* hook_arg;
    u64 field_0x60;
    u64 field_0x68;
};
static_assert(__builtin_offsetof(UnkAtkHeap, field_0x68) == 0x68, "UnkAtkHeap placeholder layout");

void* fn_71000813e4(base::UnkSubAllocator* self, u64 size, u64 align, u32 flags);  // src/base/UnkSubAllocator.cpp

static inline void* Allocate(UnkAtkHeap* heap, u64 size, u64 align) {
    heap->last = fn_71000813e4(&heap->allocator, size, align, 0xb);
    if (heap->hook)
        heap->hook(heap->hook_arg, heap);
    return heap->last;
}

// The sub-allocator for the engine's work buffers and the two engine heaps, with their memory
// (0x600000, 0x800000 and 0x6000000 bytes). The sound system is published in g_unk_7101d685c8.
extern base::UnkSubAllocator g_unk_7108b69000;
extern char g_unk_7101d69000[];
extern UnkAtkHeap g_unk_7108b69038;
extern char g_unk_7102369000[];
extern UnkAtkHeap g_unk_7108b690a8;
extern char g_unk_7102b69000[];
void fn_7100081500(base::UnkSubAllocator* self);  // shuts the sub-allocator down

// The nn::atk objects this sets up (static, in the library's data) and the library functions
// used, all of them the statically linked library's (status L); names are placeholders.
extern u8 g_unk_7108b6abc8[] __attribute__((visibility("hidden")));  // the sound archive player's block
extern u8 g_unk_7108b6acf8[] __attribute__((visibility("hidden")));  // the listeners and effects
extern u8 g_unk_7108b6ad30[] __attribute__((visibility("hidden")));  // the sound category's registry record
extern u8 g_unk_7108b6b1c8[] __attribute__((visibility("hidden")));  // the async thread
extern u8 g_unk_7108b6bd10[] __attribute__((visibility("hidden")));  // the sound archive
void fn_710120a2c0(void* params);
u64 fn_710120a344(void* params);
bool fn_710120ade0(void* params, void* buffer, u64 size);
void fn_710120b198(void* heap, char* begin, u64 size);  // as src/game_s1/fn_71000f8630.cpp declares it
void* fn_7101228d6c();
void fn_7101229134(void* self, void (*callback)(), void* arg);
void fn_710120b108(void (*callback)(), void* arg);
void fn_710120b148(void (*callback)(), void* arg);
bool fn_7101204810(void* archive, void* path);
u32 fn_710120fa40(void* object);
void fn_7101204aa8(void* archive, void* buffer, u64 size);
u64 fn_7101208d84(void* manager, void* archive);
void fn_7101208db0(void* manager, void* archive, void* buffer, u64 size);
u64 fn_71012079b0(void* player, void* archive);
s32 fn_71012079b8(void* player, void* archive);
u64 fn_7101207818(void* archive);
void fn_7101207598(void* player, void* archive, void* data_manager, void* buffer, u64 size, void* stream_buffer,
                   u64 stream_size, u64 x7);
u64 fn_7101206088(void* manager, void* archive);
void fn_71012060d8(void* manager, void* archive, void* buffer, u64 size);
void fn_710120621c(void* manager, void* player);
void fn_7101206010(void* listener, const void* matrix);
void fn_710120602c(void* listener, f32 value);
void fn_7101206034(void* listener, f32 value);
void fn_7101206024(void* listener, f32 value);
void fn_7101233798(void* effect, u32 value);
void fn_7101233c4c(void* effect, u32 value);
void fn_7101233cd4(void* effect, const u32* value, u32 x2);
void fn_7101233e14(void* effect, u64 value);
void fn_7101233e30(void* effect, u64 value);
void fn_7101233ebc(void* effect, f32 value);
void fn_7101233f2c(void* effect, u32 value);
u64 fn_7101233880(void* effect);
void fn_7101234ad0(void* effect, u32 value);
void fn_7101234be0(void* effect, u32 value);
void fn_7101234c50(void* effect, u32 value);
void fn_7101234cc0(void* effect, f32 value);
void fn_7101234d28(void* effect, u64 value);
void fn_7101234dcc(void* effect, u32 value);
u64 fn_71012346b8(void* effect);
void fn_7101234470(void* effect, u32 value);
void fn_7101234588(void* effect, u32 value);
u64 fn_7101234068(void* effect);
void* fn_710120806c(void* player, u32 id);
void fn_710120a0ac(void* player, u32 count);
void* fn_71001039f8();
void fn_7101214bd0(void* self, u32 mode, u32 x2);

// The engine's callbacks (game code) and the sound archive's path.
void fn_71000f6770();
void fn_71000f6880();
void fn_71000f6890();
extern void* g_unk_7101a1bb18;  // the archive's path

// This unit's other set-up steps.
void fn_71000f36f8(void* block);
void fn_71000f86c0(void* list, u32 count, u32 flags, base::UnkSubAllocator* allocator);
void fn_71000f88b8(UnkSoundSystemATK* self);
void fn_71000f8b30(UnkSoundSystemATK* self);
void fn_71000f9450(UnkSoundSystemATK* self);
void fn_7100104600(void* self);
void fn_71000f95b0(void* arg);
void fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 a, u32 b, void* arg);
void fn_7100066828(void* thread, u32 a);

// The resource registry's entry the sound category is added with (as src/game/UnkActorDefFactory.cpp
// lays it out: the category's record, no factory, three ids and two callbacks).
struct UnkSoundRegistryEntry {
    void* desc;
    void* factory;
    base::global::CStrId id;
    base::global::CFilePathStrId path_0x18;
    base::global::CFilePathStrId path_0x20;
    void* context_0x28;
    void (*function_0x30)();
    void* context_0x38;
    void (*function_0x40)();
};
static_assert(sizeof(UnkSoundRegistryEntry) == 0x48, "UnkSoundRegistryEntry layout");
struct UnkRegistry_7100d92158;
struct UnkEntry_7100d92158;
extern UnkRegistry_7100d92158* g_unk_7101d4bbc0;
void fn_710002002c(UnkRegistry_7100d92158* registry, const UnkEntry_7100d92158& entry);
void fn_71000204c4(void* context, void* arg);
void fn_71000204cc(void* context, void* arg);

// A word set to zero on first use (two of them, header statics).
struct UnkZeroWord {
    u32 value;
    UnkZeroWord() { value = 0; }
};
inline UnkZeroWord& SoundZeroWordA() {
    static UnkZeroWord s_word;
    return s_word;
}
inline UnkZeroWord& SoundZeroWordB() {
    static UnkZeroWord s_word;
    return s_word;
}

template <class T>
static inline T& Member(UnkSoundSystemATK* self, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<u8*>(self) + offset);
}


// Sets up the sound system: checks that an audio output can be opened, creates the engine's heaps
// and objects (the sound archive, its player and data manager, the listener, four effects), the
// sound system's own tables and the async thread, and sets the output volumes (the TV at half).
// False, with sound turned off (g_unk_7108b6ab60), when there is no output or the engine fails.
// NON_MATCHING(major): written from the original's behaviour, not compared instruction by
// instruction. The listener's direction is nn::util::SinCosEst of 0 inlined in the original; with
// a zero angle its polynomial gives exactly 0 and 1, which are written here directly.
bool fn_71000f7800(UnkSoundSystemATK* self) {
    if (!CanOpenAudioOut()) {
        g_unk_7108b6ab60 = 1;
        return false;
    }
    fn_7100081330(&g_unk_7108b69000, g_unk_7101d69000, g_unk_7101d69000 + 0x600000, "DLM", 0);
    u8 params[0x98];
    fn_710120a2c0(params);
    u64 size = fn_710120a344(params);
    void* work = fn_71000813e4(&g_unk_7108b69000, size, 4, 0xb);
    Member<void*>(self, 0x48) = work;
    if (!fn_710120ade0(params, work, size))
        goto fail;
    {
        UnkAtkHeap* heap = &g_unk_7108b69038;
        fn_7100081330(&heap->allocator, g_unk_7102369000, g_unk_7102369000 + 0x800000, "ATKHeap", 0);
        fn_710120b198(heap->heap, g_unk_7102369000, 0x800000);
        heap->field_0x60 = 0;
        heap->field_0x68 = 0;
        heap->hook_arg = nullptr;
        heap->last = nullptr;
        heap->hook = nullptr;
        UnkAtkHeap* big = &g_unk_7108b690a8;
        fn_7100081330(&big->allocator, g_unk_7102b69000, g_unk_7102b69000 + 0x6000000, "ATKHeap", 0);
        fn_710120b198(big->heap, g_unk_7102b69000, 0x6000000);
        big->last = nullptr;
        big->field_0x60 = 0;
        big->field_0x68 = 0;
        big->hook = nullptr;
        big->hook_arg = nullptr;
        for (u64 i = 0; i < 0x40; ++i) {
            void* block = Allocate(heap, 0x80, 8);
            fn_71000f36f8(block);
            Member<void*>(self, 0x400588 + i * 8) = block;
        }
        fn_71000f86c0(reinterpret_cast<u8*>(self) + 0x388, 0x80, 0xb, &heap->allocator);
        fn_7101229134(fn_7101228d6c(), fn_71000f6770, nullptr);
        fn_710120b108(fn_71000f6880, nullptr);
        fn_710120b148(fn_71000f6890, nullptr);
        {
            UnkSoundRegistryEntry entry{g_unk_7108b6ad30,
                                        nullptr,
                                        g_unk_7101d470e0,
                                        g_unk_7101d470e8,
                                        base::global::CFilePathStrId(base::global::UnkStrIdInline(), nullptr, true),
                                        nullptr,
                                        reinterpret_cast<void (*)()>(fn_71000204c4),
                                        nullptr,
                                        reinterpret_cast<void (*)()>(fn_71000204cc)};
            fn_710002002c(g_unk_7101d4bbc0, *reinterpret_cast<const UnkEntry_7100d92158*>(&entry));
        }
        u8* archive = g_unk_7108b6bd10;
        if (!fn_7101204810(archive, g_unk_7101a1bb18))
            goto fail;
        u8* block = g_unk_7108b6abc8;
        u32 archive_size = fn_710120fa40(archive + 0x2a0);
        void* buffer = fn_71000813e4(&g_unk_7108b69000, archive_size, 0x1000, 0xb);
        Member<void*>(self, 0x58) = buffer;
        fn_7101204aa8(archive, buffer, archive_size);
        u8* data_manager = block + 0x7f8;
        u64 dm_size = fn_7101208d84(data_manager, archive);
        void* dm_buffer = fn_71000813e4(&g_unk_7108b69000, dm_size, 8, 0xb);
        Member<void*>(self, 0x68) = dm_buffer;
        fn_7101208db0(data_manager, archive, dm_buffer, dm_size);
        u8* player = block + 0xa38;
        u64 stream_unit = fn_71012079b0(player, archive);
        s32 stream_count = fn_71012079b8(player, archive);
        u64 stream_size = stream_unit * stream_count;
        Member<void*>(self, 0x60) = Allocate(heap, stream_size, 0x1000);
        u64 player_size = fn_7101207818(archive);
        void* player_buffer = fn_71000813e4(&g_unk_7108b69000, player_size, 0x1000, 0xb);
        Member<void*>(self, 0x50) = player_buffer;
        fn_7101207598(player, archive, data_manager, player_buffer, player_size, Member<void*>(self, 0x60),
                      stream_size, 0);
        u8* manager = block + 0x108;
        u64 manager_size = fn_7101206088(manager, archive);
        void* manager_buffer = fn_71000813e4(&g_unk_7108b69000, manager_size, 0x1000, 0xb);
        Member<void*>(self, 0x78) = manager_buffer;
        fn_71012060d8(manager, archive, manager_buffer, manager_size);
        fn_710120621c(manager, block);
        *reinterpret_cast<u32*>(block + 0x148) = 0x20;
        *reinterpret_cast<f32*>(block + 0x150) = 5.6666665f;

        // The listener: at (0, 0, -3), up (0, 1, 0), facing along -z.
        Member<f32>(self, 0xa0) = 0.0f;
        Member<f32>(self, 0xa4) = 1.0f;
        Member<f32>(self, 0xa8) = 0.0f;
        Member<f32>(self, 0xac) = 0.0f;
        Member<f32>(self, 0xb0) = 0.0f;
        Member<f32>(self, 0xb4) = -3.0f;
        Member<f32>(self, 0xbc) = 0.0f;
        Member<f32>(self, 0xb8) = -0.0f;  // -sin(0)
        Member<f32>(self, 0xc0) = -1.0f;  // -cos(0)
        Member<f32>(self, 0xc4) = g_unk_710164779c.x;
        Member<f32>(self, 0xc8) = g_unk_710164779c.y;
        Member<f32>(self, 0xcc) = g_unk_710164779c.z;
        Member<bool>(self, 0xd0) = true;
        fn_71000f88b8(self);
        u8* listeners = g_unk_7108b6acf8;
        u8* listener = listeners + 0x1c8;
        {
            f32 position[4] = {Member<f32>(self, 0xc4), Member<f32>(self, 0xc8), Member<f32>(self, 0xcc), 0.0f};
            fn_7101206010(listener, position);
        }
        fn_710120602c(listener, 5.0f);
        fn_7101206034(listener, 5.0f);
        fn_7101206024(listener, 5.0f);
        // Swaps the heads of two intrusive lists (at +0x248 and +0), with their back links.
        {
            u8** head_a = reinterpret_cast<u8**>(listeners + 0x248);
            u8** head_b = reinterpret_cast<u8**>(listeners);
            u8* a = *head_a;
            *head_a = *head_b;
            *reinterpret_cast<u8***>(a + 8) = head_b;
            *reinterpret_cast<u8***>(*head_a + 8) = head_a;
            *head_b = a;
        }
        fn_7100104600(reinterpret_cast<u8*>(self) + 0x278);
        for (u64 i = 0x194; i < 0x1b4; i += 4)
            Member<u32>(self, i) = 0;
        for (u64 i = 0x1b4; i < 0x1d4; i += 4)
            Member<f32>(self, i) = 2.0f;
        for (u64 i = 0x1d4; i < 0x214; i += 4)
            Member<f32>(self, i) = 1.0f;

        // The four effects, each with a buffer from the engine heap.
        u8* effect = listeners + 0xb8;
        fn_7101233798(effect, 1);
        fn_7101233c4c(effect, 1);
        fn_7101233e14(effect, 3000000000u);
        fn_7101233ebc(effect, 0.6f);
        fn_7101233f2c(effect, 0);
        Member<void*>(self, 0x80) = Allocate(heap, fn_7101233880(effect), 0x40);
        effect = listeners + 0x140;
        fn_7101233798(effect, 1);
        u32 one = 1;
        fn_7101233cd4(effect, &one, 0);
        fn_7101233e14(effect, 3000000000u);
        fn_7101233e30(effect, 400000000);
        fn_7101233ebc(effect, 0.6f);
        fn_7101233f2c(effect, 0);
        Member<void*>(self, 0x88) = Allocate(heap, fn_7101233880(effect), 0x40);
        effect = listeners + 0x258;
        fn_7101233798(effect, 1);
        fn_7101234ad0(effect, 1);
        fn_7101234be0(effect, 3);
        fn_7101234c50(effect, 3);
        fn_7101234cc0(effect, 0.6f);
        fn_7101234d28(effect, 3000000000u);
        fn_7101234dcc(effect, 0);
        Member<void*>(self, 0x90) = Allocate(heap, fn_71012346b8(effect), 0x40);
        effect = listeners + 0x2f0;
        fn_7101233798(effect, 1);
        fn_7101234470(effect, 3);
        fn_7101234588(effect, 0);
        Member<void*>(self, 0x98) = Allocate(heap, fn_7101234068(effect), 0x40);

        Member<u32>(self, 0x348) = SoundZeroWordA().value;
        for (u64 i = 0x34c; i < 0x36c; i += 8)
            Member<u64>(self, i) = 0;
        Member<u32>(self, 0xf0) = SoundZeroWordB().value;
        for (u64 i = 0; i < 8; ++i) {
            Member<u32>(self, 0xf4 + i * 0x14) = 16000;
            Member<u32>(self, 0xf8 + i * 0x14) = 16000;
            Member<u32>(self, 0xfc + i * 0x14) = 16000;
        }
        fn_71000f8b30(self);
        fn_71000f9450(self);

        // The voice counts (the first eight handed to the player by sound kind), then zeroed tallies.
        static const u32 kCounts[0x13] = {0x20, 6,    8,    0x10, 0x10, 8,    0x10, 0x10, 0x20, 0x10,
                                          0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
        for (u64 i = 0; i < 0x13; ++i)
            Member<u32>(self, 0x2a8 + i * 4) = kCounts[i];
        for (u64 i = 0x2f4; i < 0x340; i += 4)
            Member<u32>(self, i) = 0;
        u8* sound_player = g_unk_7108b6b1c8 + 0x438;
        for (u32 i = 0; i < 8; ++i)
            fn_710120a0ac(fn_710120806c(sound_player, 0x4000000 + i), Member<u32>(self, 0x2a8 + i * 4));

        fn_71000668f8(&Member<nn::os::MutexType>(self, 0x400528));
        fn_71000668f8(&Member<nn::os::MutexType>(self, 0x400548));
        fn_71000668f8(&Member<nn::os::MutexType>(self, 0x400568));
        Member<u32>(self, 0x500) = 0;
        Member<u32>(self, 0x504) = 1;
        Member<u32>(self, 0x510) = 0;
        Member<u32>(self, 0x514) = 1;
        Member<u64>(self, 0x508) = 0;
        Member<u64>(self, 0x518) = 0;
        Member<u32>(self, 0x400520) = 0;
        Member<u32>(self, 0x400524) = 0xffffffff;
        fn_7100066744(g_unk_7108b6b1c8, "Sound Async Thread", fn_71000f95b0, 1, 4, nullptr);
        fn_7100066828(g_unk_7108b6b1c8, 0);
        Member<bool>(self, 0x347) = true;
        g_unk_7101d685c8 = reinterpret_cast<UnkIdMap*>(self);

        nn::audio::AudioDeviceName tv, jack, speaker, usb;
        __builtin_memcpy(tv.raw_name, "AudioTvOutput", sizeof("AudioTvOutput"));
        __builtin_memcpy(jack.raw_name, "AudioStereoJackOutput", sizeof("AudioStereoJackOutput"));
        __builtin_memcpy(speaker.raw_name, "AudioBuiltInSpeakerOutput", sizeof("AudioBuiltInSpeakerOutput"));
        __builtin_memcpy(usb.raw_name, "AudioUsbDeviceOutput", sizeof("AudioUsbDeviceOutput"));
        nn::audio::SetAudioDeviceOutputVolume(&tv, 0.5f);
        nn::audio::SetAudioDeviceOutputVolume(&jack, 1.0f);
        nn::audio::SetAudioDeviceOutputVolume(&speaker, 1.0f);
        nn::audio::SetAudioDeviceOutputVolume(&usb, 1.0f);
        if (Member<s32>(self, 0xdc) != 2) {
            Member<s32>(self, 0xdc) = 2;
            fn_7101214bd0(fn_71001039f8(), 2, 0);
        }
        return true;
    }
fail:
    g_unk_7108b6ab60 = 1;
    fn_7100081500(&g_unk_7108b69000);
    g_unk_7101d685c8 = nullptr;
    return false;
}

// ---- fn_71000f88b8: the listener's matrix ----

#include <math.h>

// Hands the listener its view matrix (nn::atk's Sound3DListener, a 4x3 matrix as four rows of four
// floats).
void fn_7101205f54(void* listener, const f32 (*matrix)[4]);

static inline void Normalize3(f32* v) {
    f32 s = 1.0f / sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    v[0] *= s;
    v[1] *= s;
    v[2] *= s;
}

// When the listener moved (+0xd0), rebuilds its view matrix: a right-handed look-at from the
// position at +0xac to the target at +0xb8 (both in centimetres, so scaled by 0.01) with the up
// vector at +0xa0, and hands it to the engine's listener.
// NON_MATCHING(major): written from the original's behaviour. The original is hand-vectorised: it
// normalises with two Newton steps on the reciprocal square root estimate (frsqrte/frsqrts), which
// can differ from 1/sqrtf in the last bits, and transposes the axes into the rows with zips and st2
// stores.
void fn_71000f88b8(UnkSoundSystemATK* self) {
    if (!Member<bool>(self, 0xd0))
        return;
    f32 p[3] = {Member<f32>(self, 0xac) * 0.01f, Member<f32>(self, 0xb0) * 0.01f, Member<f32>(self, 0xb4) * 0.01f};
    f32 t[3] = {Member<f32>(self, 0xb8) * 0.01f, Member<f32>(self, 0xbc) * 0.01f, Member<f32>(self, 0xc0) * 0.01f};
    f32 up[3] = {Member<f32>(self, 0xa0), Member<f32>(self, 0xa4), Member<f32>(self, 0xa8)};
    f32 z[3] = {p[0] - t[0], p[1] - t[1], p[2] - t[2]};
    Normalize3(up);
    Normalize3(z);
    f32 x[3] = {up[1] * z[2] - up[2] * z[1], up[2] * z[0] - up[0] * z[2], up[0] * z[1] - up[1] * z[0]};
    Normalize3(x);
    f32 y[3] = {z[1] * x[2] - z[2] * x[1], z[2] * x[0] - z[0] * x[2], z[0] * x[1] - z[1] * x[0]};
    f32 m[4][4];
    for (u32 i = 0; i < 3; ++i) {
        m[i][0] = x[i];
        m[i][1] = y[i];
        m[i][2] = z[i];
        m[i][3] = 0.0f;
    }
    m[3][0] = -(x[0] * p[0] + x[1] * p[1] + x[2] * p[2]);
    m[3][1] = -(y[0] * p[0] + y[1] * p[1] + y[2] * p[2]);
    m[3][2] = -(z[0] * p[0] + z[1] * p[1] + z[2] * p[2]);
    m[3][3] = 0.0f;
    fn_7101205f54(g_unk_7108b6acf8 + 0x1c8, m);
    Member<bool>(self, 0xd0) = false;
}
