// CAccessPointComponent unit (tools/tu_map.py): a chained hash map keyed by string ids (the bucket is
// the string's 64-bit hash at +0x30, halves xored, modulo the bucket capacity) with engine arrays of
// string ids as values; nodes keep the list links at +0x30 / +0x38 and the bucket chain at +0x40 (the
// map of src/game/CAccessPointComponent/CAccessPointComponent_MapErase_A5_b74.cpp). The "set by key"
// move slot (fn_7100a7534c) moves the given array into the found value with the array's move
// assignment; the copying insert (fn_7100a75548) adds a node holding copies of the key and of the
// array (base::UnkStrIdVec's copy constructor). Names and layouts are placeholders.
#include <new>

#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;


struct UnkIdElem_ovb0060 : base::global::UnkStrIdBase {
    ~UnkIdElem_ovb0060() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
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

using UnkIdVector_ovb0060 = base::UnkVector<UnkIdElem_ovb0060>;

struct UnkHashView_ovb0060 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkIdVectorNode_ovb0060 {
    base::global::UnkStringData* key;
    UnkIdVector_ovb0060 value;  // 0x8
    UnkIdVectorNode_ovb0060* next;   // 0x30
    UnkIdVectorNode_ovb0060* prev;   // 0x38
    UnkIdVectorNode_ovb0060* chain;  // 0x40
};

struct UnkIdVectorMap_ovb0060 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdVectorNode_ovb0060** buckets;  // 0x10
    u32 bucket_count;                   // 0x18
    u32 bucket_capacity;                // 0x1c
};


void fn_7100a7534c(UnkIdVectorMap_ovb0060* self, const CStrId* key, UnkIdVector_ovb0060* value) {
    if (!self->bucket_count)
        return;
    auto* k = key->data;
    u64 h = reinterpret_cast<const UnkHashView_ovb0060*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    UnkIdVectorNode_ovb0060** bucket = &self->buckets[i];
    if (!bucket)
        return;
    UnkIdVectorNode_ovb0060* n = *bucket;
    if (!n)
        return;
    while (n->key != k) {
        n = n->chain;
        if (!n)
            return;
    }
    n->value = static_cast<UnkIdVector_ovb0060&&>(*value);
}

// The copying insert: a node holding a copy of the key and of a string-id vector
// (base::UnkStrIdVec's copy constructor) goes at the head of its bucket chain and the tail of the
// list of all nodes.
struct UnkIdVecNode_ovb0060 {
    CStrId key;
    base::UnkStrIdVec value;      // 0x8
    UnkIdVecNode_ovb0060* next;   // 0x30
    UnkIdVecNode_ovb0060* prev;   // 0x38
    UnkIdVecNode_ovb0060* chain;  // 0x40

    UnkIdVecNode_ovb0060(const CStrId& k, const base::UnkStrIdVec& v)
        : key(k), value(v), next(nullptr), prev(nullptr), chain(nullptr) {}
};

struct UnkIdVecMap_ovb0060 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdVecNode_ovb0060** buckets;  // 0x10
    u32 bucket_count;                // 0x18
    u32 bucket_capacity;             // 0x1c
    u8 pad_0x20[0x18];
    UnkIdVecNode_ovb0060* tail;  // 0x38
    UnkIdVecNode_ovb0060* head;  // 0x40
};

UnkIdVecNode_ovb0060* fn_7100a75548(UnkIdVecMap_ovb0060* self, const CStrId* key, const base::UnkStrIdVec* value) {
    UnkIdVecNode_ovb0060** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkHashView_ovb0060*>(key->data)->hash;
        u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
        slot = &self->buckets[i];
    }
    UnkIdVecNode_ovb0060* chain = *slot;
    auto* n = static_cast<UnkIdVecNode_ovb0060*>(self->allocator->vf_0x40(0x48, 8, self->flags));
    if (n)
        new (n) UnkIdVecNode_ovb0060(*key, *value);
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        __builtin_memcpy(&n->prev, &self->tail, sizeof(n->prev));
        self->tail->next = n;
    }
    self->tail = n;
    ++self->count;
    return n;
}
