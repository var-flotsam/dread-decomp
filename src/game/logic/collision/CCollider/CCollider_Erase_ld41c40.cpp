// CCollider's unit (2503): erases the node a string id names from a chained hash map (hashed by the two words
// at +0x30 of the id's string): the node is unlinked from its bucket chain and from the list of all nodes, its
// refcounted value and id are released and it is freed; returns the node before it. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_ld41c40 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkRefNode_ld41c40 {
    CStrId key;
    base::UnkRefPtrMember value;  // 0x8
    UnkRefNode_ld41c40* prev;     // 0x10
    UnkRefNode_ld41c40* next;     // 0x18
    UnkRefNode_ld41c40* chain;    // 0x20

    UnkRefNode_ld41c40*& Next() { return next; }
    UnkRefNode_ld41c40*& Prev() { return prev; }
};

struct UnkRefMap_ld41c40 {
    base::UnkAllocator* allocator;
    u8 pad_0x8[0x4];
    u32 count;                     // 0xc
    UnkRefNode_ld41c40** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkRefNode_ld41c40* tail;  // 0x38
    UnkRefNode_ld41c40* head;  // 0x40
};

UnkRefNode_ld41c40* fn_7100d41c40(UnkRefMap_ld41c40* self, const CStrId* key) {
    UnkStringData* k = key->data;
    const UnkHashedStr_ld41c40* h = reinterpret_cast<const UnkHashedStr_ld41c40*>(k);
    u32 b = (h->hash_lo ^ h->hash_hi) % self->bucket_count;
    UnkRefNode_ld41c40** slot = self->buckets + b;
    UnkRefNode_ld41c40* n = *slot;
    if (!n)
        return nullptr;
    UnkRefNode_ld41c40* before = nullptr;
    if (n->key.data != k) {
        do {
            before = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key.data != k);
    }
    *(before ? &before->chain : slot) = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    UnkRefNode_ld41c40* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    n->value.~UnkRefPtrMember();
    n->key.~CStrId();
    self->allocator->vf_0x48(n);
    --self->count;
    return result;
}
