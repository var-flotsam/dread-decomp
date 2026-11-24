// Unit 99 of tools/tu_map.py (base::snd::CSoundSystemATK::CTunableSoundSystemATK): loads the packed
// sound definitions file into one buffer and indexes its three sections by id in hash maps.
// Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkStrPair.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/UnkDefaultAllocator.h"
#include "game/UnkTU56.h"
#include "game/UnkTU58.h"
#include "base/global/timeline/CEvent.h"
#include "types.h"

extern base::UnkSubAllocator g_unk_7108b69000;

// A node of a chained hash map keyed by an 8-byte id: in the list of all nodes and in its bucket.
template <class V>
struct UnkSoundDefNode_l0f8b30 {
    u64 key;
    V value;
    UnkSoundDefNode_l0f8b30* next;   // 0x10: the list of all nodes
    UnkSoundDefNode_l0f8b30* prev;   // 0x18
    UnkSoundDefNode_l0f8b30* chain;  // 0x20: the next node in the bucket

    UnkSoundDefNode_l0f8b30(const u64* id, V v) : key(*id), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

template <class V>
struct UnkSoundDefMap_l0f8b30 {
    using Node = UnkSoundDefNode_l0f8b30<V>;

    base::IAllocator* node_allocator;  // 0x0
    u32 node_flags;                      // 0x8
    u32 count;                           // 0xc
    Node** buckets;                      // 0x10 (a base::global::CRntVector)
    u32 bucket_count;                    // 0x18
    u32 bucket_capacity;                 // 0x1c
    u32 step;                            // 0x20
    bool field_0x24;                     // 0x24
    u32 bucket_flags;                    // 0x28
    base::IAllocator* allocator;       // 0x30
    Node* tail;                          // 0x38
    Node* head;                          // 0x40

    explicit UnkSoundDefMap_l0f8b30(base::UnkSubAllocator* heap)
        : node_allocator(reinterpret_cast<base::IAllocator*>(heap)), node_flags(0xe), count(0),
          buckets(nullptr), bucket_count(0), bucket_capacity(0), step(10), field_0x24(true),
          bucket_flags(0xe), allocator(reinterpret_cast<base::IAllocator*>(heap)), tail(nullptr),
          head(nullptr) {}

    // From the sound sub-allocator.
    static void* operator new(size_t size, base::UnkSubAllocator* heap) {
        return fn_71000813e4(heap, size, 8, 0xb);
    }

    void Resize(u32 n) {
        u32 c = bucket_count;
        if (c < n) {
            Node** p;
            if (bucket_capacity < n) {
                p = static_cast<Node**>(allocator->vf_0x40((u64)n * sizeof(Node*), 8, bucket_flags));
                if (buckets) {
                    memcpy(p, buckets, (u64)bucket_count * sizeof(Node*));
                    allocator->vf_0x48(buckets);
                }
                c = bucket_count;
                buckets = p;
                bucket_capacity = n;
            } else {
                p = buckets;
            }
            memset(p + c, 0, (u64)(n - c) * sizeof(Node*));
        } else if (c <= n) {
            return;
        }
        bucket_count = n;
    }

    void Insert(const u64* id, V value) {
        Node** slot = nullptr;
        if (bucket_count)
            slot = &buckets[*reinterpret_cast<const u32*>(id) % bucket_capacity];
        Node* first = *slot;
        Node* n = static_cast<Node*>(node_allocator->vf_0x40(sizeof(Node), 8, node_flags));
        if (n)
            new (n) Node(id, value);
        n->chain = first;
        *slot = n;
        if (!head) {
            head = n;
        } else if (tail) {
            n->prev = tail;
            tail->next = n;
        }
        tail = n;
        ++count;
    }
};

using UnkSoundDefPtrMap_l0f8b30 = UnkSoundDefMap_l0f8b30<const void*>;

// A definition in the third section: its id first, then its data.
struct UnkSoundDefEntry_l0f8b30 {
    u64 id;
    u8 pad_0x8[0x90];
    u32 extra_count;  // 0x98: 8-byte items after the entry
    u32 pad_0x9c;
    const u8* tail_data;  // 0xa0
    void* field_0xa8;
};
static_assert(__builtin_offsetof(UnkSoundDefEntry_l0f8b30, tail_data) == 0xa0, "entry layout");

struct UnkSoundDefTables_l0f8b30 {
    UnkSoundDefPtrMap_l0f8b30* entries;  // 0x0: the third section
    u8* data;                            // 0x8: the file's contents after its first word
    UnkSoundDefPtrMap_l0f8b30* first;    // 0x10
    UnkSoundDefPtrMap_l0f8b30* second;   // 0x18
};

struct UnkTunableSoundSystem_l0f8b30 {
    u8 pad_0x0[0x4b8];
    UnkSoundDefMap_l0f8b30<u32> ids;  // 0x4b8
    u8 pad_0x500[0x400868 - 0x500];
    UnkSoundDefTables_l0f8b30 defs;  // 0x400868
};
static_assert(__builtin_offsetof(UnkTunableSoundSystem_l0f8b30, defs) == 0x400868, "layout");

// The bucket count for `n`: itself when prime, else the next prime above it.
static inline u32 BucketCount(u32 n) {
    if (!CEvent_IsPrime(n))
        n = CEvent_GetNextPrimeAtOrAbove(n);
    return n;
}

void fn_71000f8b30(UnkTunableSoundSystem_l0f8b30* self) {
    UnkSoundDefTables_l0f8b30& defs = self->defs;
    base::global::CRntFile file;
    fn_710007be8c(&file, 0x16, nullptr);
    {
        UnkStrPair_0x10 path;
        fn_710008d680(&path, "system/snd/sm_sounddefinitions_packed.bmsstoc");
        {
            base::global::CRntString mode("rb");
            fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &path, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&path));
    }
    u32 count = 0;
    fn_710007c70c(&file, &count, 4);
    u32 size = fn_710007c1a4(&file) - 4;
    u8* data = static_cast<u8*>(fn_71000813e4(&g_unk_7108b69000, size, 0x40, 0xb));
    defs.data = data;
    fn_710007c70c(&file, data, size);
    fn_710007c144(&file);

    // First section: `count` blocks of a length and the data it covers, each starting with its id.
    UnkSoundDefPtrMap_l0f8b30* first = new (&g_unk_7108b69000) UnkSoundDefPtrMap_l0f8b30(&g_unk_7108b69000);
    first->Resize(BucketCount(count / 4));
    defs.first = first;
    u32 pos = 0;
    for (u32 i = 0; pos < size && i < count; i++) {
        u32 len = *reinterpret_cast<u32*>(defs.data + pos);
        pos += 4;
        const u64* id = reinterpret_cast<const u64*>(defs.data + pos);
        defs.first->Insert(id, id);
        self->ids.Insert(id, 0);
        pos += len;
    }

    // Second section: a count, then blocks as in the first.
    u32 count2 = *reinterpret_cast<u32*>(defs.data + pos);
    pos += 4;
    UnkSoundDefPtrMap_l0f8b30* second = new (&g_unk_7108b69000) UnkSoundDefPtrMap_l0f8b30(&g_unk_7108b69000);
    second->Resize(BucketCount(count2 / 4));
    defs.second = second;
    for (u32 i = 0; i < count2 && pos < size; i++) {
        u32 len = *reinterpret_cast<u32*>(defs.data + pos);
        pos += 4;
        const u64* id = reinterpret_cast<const u64*>(defs.data + pos);
        defs.second->Insert(id, id);
        self->ids.Insert(id, 0);
        pos += len;
    }

    // Third section: a count, then entries each followed by its data and its 8-byte items.
    u32 count3 = *reinterpret_cast<u32*>(defs.data + pos);
    pos += 4;
    UnkSoundDefPtrMap_l0f8b30* entries = new (&g_unk_7108b69000) UnkSoundDefPtrMap_l0f8b30(&g_unk_7108b69000);
    entries->Resize(BucketCount(count3 / 4));
    defs.entries = entries;
    for (u32 i = 0; pos < size && i < count3; i++) {
        u32 len = *reinterpret_cast<u32*>(defs.data + pos);
        pos += 4;
        UnkSoundDefEntry_l0f8b30* entry = reinterpret_cast<UnkSoundDefEntry_l0f8b30*>(defs.data + pos);
        u32 extra = entry->extra_count;
        pos += len;
        entry->tail_data = defs.data + pos;
        entry->field_0xa8 = nullptr;
        defs.entries->Insert(&entry->id, entry);
        self->ids.Insert(&entry->id, 0);
        pos += extra * 8;
    }
    fn_710007c0e4(&file);
}
