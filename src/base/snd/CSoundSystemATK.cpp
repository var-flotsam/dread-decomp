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
