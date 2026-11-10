// A unit of tools/tu_map.py (2648, no reliable name): a chained hash map keyed by a string id (the
// bucket is the string's 64-bit hash at +0x30, halves xored, modulo the bucket capacity) whose
// 0x5f0-byte nodes hold the key, a polymorphic 0x5d0-byte value, the links of the list of all nodes
// and the link of the bucket chain. Its clear, and the two inserts that differ in how the value is
// built. Names and layouts are placeholders.
#include <new>
#include <string.h>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

void fn_7101036e34(void* value);  // the value's destructor
void fn_7101041d34(void* value, const void* arg);  // builds a value
void fn_7101036fb8(void* value, const void* arg);  // builds a value from another

struct UnkStringHashView_b0248 {
    u8 pad_0x0[0x30];
    u64 hash;
};

// A string id whose release decrements and tests the count in two steps.
struct UnkNodeKey_b0248 {
    UnkStringData* data;
    void Release() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkVirtualValue_b0248 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

struct UnkNode_b0248 {
    UnkNodeKey_b0248 key;  // 0x0
    u8 value[0x5d0];       // 0x8
    UnkNode_b0248* next;   // 0x5d8: the list of all nodes
    UnkNode_b0248* prev;   // 0x5e0
    UnkNode_b0248* chain;  // 0x5e8: the next node in the bucket

    UnkVirtualValue_b0248* Object() { return reinterpret_cast<UnkVirtualValue_b0248*>(value); }
    // The list links, through accessors (their stores may alias each other).
    UnkNode_b0248*& Next() { return next; }
    UnkNode_b0248*& Prev() { return prev; }
};

struct UnkMap_b0248 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkNode_b0248** buckets;  // 0x10
    u32 bucket_count;         // 0x18
    u32 bucket_capacity;      // 0x1c
    u8 pad_0x20[0x18];
    UnkNode_b0248* tail;  // 0x38
    UnkNode_b0248* head;  // 0x40

    UnkNode_b0248** Bucket(const CStrId& key) {
        const UnkStringHashView_b0248* view = reinterpret_cast<const UnkStringHashView_b0248*>(key.data);
        u32 index = ((u32)(view->hash >> 32) ^ (u32)view->hash) % bucket_capacity;
        // keeps the bucket array's load after the remainder
        asm("" : "+r"(index));
        return &buckets[index];
    }
};

void fn_710103fa90(UnkMap_b0248* self) {
    for (UnkNode_b0248* n = self->head; n; n = n->next)
        n->Object()->vf_0x28();
    if (self->count) {
        UnkNode_b0248* n = self->head;
        while (n) {
            UnkNode_b0248* next = n->next;
            fn_7101036e34(n->value);
            n->key.Release();
            self->allocator->vf_0x48(n);
            n = next;
        }
        memset(self->buckets, 0, (u64)self->bucket_capacity * 8);
        self->count = 0;
        self->tail = nullptr;
        self->head = nullptr;
    }
}

// Adds a node for `key` at the head of its bucket and the tail of the list, its value built by Init.
template <void (*Init)(void*, const void*)>
__attribute__((always_inline)) static inline void Insert(UnkMap_b0248* self, const CStrId* key, const void* arg) {
    UnkNode_b0248** slot = nullptr;
    if (self->bucket_count)
        slot = self->Bucket(*key);
    UnkNode_b0248* chain = *slot;
    UnkNode_b0248* n = static_cast<UnkNode_b0248*>(self->allocator->vf_0x40(sizeof(UnkNode_b0248), 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        Init(n->value, arg);
        n->chain = nullptr;
        n->prev = nullptr;
        n->next = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        n->Prev() = self->tail;
        self->tail->Next() = n;
    }
    self->tail = n;
    ++self->count;
}

void fn_7101042390(UnkMap_b0248* self, const CStrId* key, const void* arg) {
    Insert<fn_7101041d34>(self, key, arg);
}

void fn_710104249c(UnkMap_b0248* self, const CStrId* key, const void* arg) {
    Insert<fn_7101036fb8>(self, key, arg);
}
