// sound::CMusicManager::CTunableMusicManager unit: adds an object, keyed by the string id fn_7100fed648 gives, to a
// chained hash set of objects unless one with that id is already there. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkIdNode_lfd9aa0;

struct UnkIdData_lfd9aa0 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdNode_lfd9aa0 {
    CStrId key;
    void* value;
    UnkIdNode_lfd9aa0* next;
    UnkIdNode_lfd9aa0* prev;
    UnkIdNode_lfd9aa0* chain;

    UnkIdNode_lfd9aa0(const CStrId& k, void* v) : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkIdSet_lfd9aa0 {
    base::UnkAllocator* allocator;  // 0x0
    u32 flags;                      // 0x8
    u32 count;                      // 0xc
    UnkIdNode_lfd9aa0** buckets;    // 0x10
    u32 bucket_count;               // 0x18
    u32 bucket_capacity;            // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_lfd9aa0* tail;  // 0x38
    UnkIdNode_lfd9aa0* head;  // 0x40

    UnkIdNode_lfd9aa0** BucketOf(const CStrId& id) {
        if (bucket_count == 0)
            return nullptr;
        auto* data = reinterpret_cast<UnkIdData_lfd9aa0*>(id.data);
        return &buckets[(data->hash_hi ^ data->hash_lo) % bucket_capacity];
    }

    bool Contains(const CStrId& id) {
        UnkIdNode_lfd9aa0** bucket = BucketOf(id);
        if (bucket == nullptr)
            return false;
        for (UnkIdNode_lfd9aa0* node = *bucket; node; node = node->chain)
            if (node->key.data == id.data)
                return true;
        return false;
    }

    void Insert(const CStrId& id, void* value) {
        UnkIdNode_lfd9aa0** bucket = BucketOf(id);
        UnkIdNode_lfd9aa0* old = *bucket;
        auto* node = static_cast<UnkIdNode_lfd9aa0*>(allocator->vf_0x40(sizeof(UnkIdNode_lfd9aa0), 8, flags));
        if (node)
            new (node) UnkIdNode_lfd9aa0(id, value);
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

struct UnkIdObject_lfd9aa0;

// The id the object is registered under.
const CStrId* fn_7100fed648(UnkIdObject_lfd9aa0* object);

struct UnkMusicManager_lfd9aa0 {
    u8 pad_0x0[0x130];
    UnkIdSet_lfd9aa0 set;  // 0x130
};

void fn_7100fd9aa0(UnkMusicManager_lfd9aa0* self, UnkIdObject_lfd9aa0* object) {
    if (!object)
        return;
    if (self->set.Contains(*fn_7100fed648(object)))
        return;
    self->set.Insert(*fn_7100fed648(object), object);
}
