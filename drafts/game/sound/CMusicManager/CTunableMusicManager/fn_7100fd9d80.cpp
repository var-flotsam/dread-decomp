// sound::CMusicManager::CTunableMusicManager unit: adds an object, keyed by the string id at its start, to a
// chained hash set of objects unless one with that id is already there. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdNode_lfd9d80;

struct UnkIdData_lfd9d80 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdNode_lfd9d80 {
    CStrId key;
    void* value;
    UnkIdNode_lfd9d80* next;
    UnkIdNode_lfd9d80* prev;
    UnkIdNode_lfd9d80* chain;

    UnkIdNode_lfd9d80(const CStrId& k, void* v) : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkIdSet_lfd9d80 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkIdNode_lfd9d80** buckets;    // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_lfd9d80* tail;  // 0x38
    UnkIdNode_lfd9d80* head;  // 0x40

    UnkIdNode_lfd9d80** BucketOf(const CStrId& id) {
        if (bucket_count == 0)
            return nullptr;
        auto* data = reinterpret_cast<UnkIdData_lfd9d80*>(id.data);
        return &buckets[(data->hash_hi ^ data->hash_lo) % bucket_capacity];
    }

    bool Contains(const CStrId& id) {
        UnkIdNode_lfd9d80** bucket = BucketOf(id);
        if (bucket == nullptr)
            return false;
        for (UnkIdNode_lfd9d80* node = *bucket; node; node = node->chain)
            if (node->key.data == id.data)
                return true;
        return false;
    }

    void Insert(const CStrId& id, void* value) {
        UnkIdNode_lfd9d80** bucket = BucketOf(id);
        UnkIdNode_lfd9d80* old = *bucket;
        auto* node = static_cast<UnkIdNode_lfd9d80*>(allocator->vf_0x40(sizeof(UnkIdNode_lfd9d80), 8, flags));
        if (node)
            new (node) UnkIdNode_lfd9d80(id, value);
        node->chain = old;
        *bucket = node;
        if (!head)
            head = node;
        else if (tail) {
            node->prev = tail;
            tail->next = node;
        }
        tail = node;
        ++count;
    }
};

struct UnkIdObject_lfd9d80 {
    CStrId id;
};

struct UnkMusicManager_lfd9d80 {
    u8 pad_0x0[0x178];
    UnkIdSet_lfd9d80 set;  // 0x178
};

void fn_7100fd9d80(UnkMusicManager_lfd9d80* self, UnkIdObject_lfd9d80* object) {
    if (!object)
        return;
    if (self->set.Contains(object->id))
        return;
    self->set.Insert(object->id, object);
}
