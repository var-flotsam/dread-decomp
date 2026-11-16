// Unit 2604: erases the node a string id names from a chained hash map keyed by string id with a string-id
// value: the node is unlinked from its bucket chain and from the list of all nodes, its value and key are
// released and it is freed; returns the node before it in the list. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_lf8ede4 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdNode_lf8ede4 {
    CStrId key;
    CStrId value;             // 0x8
    u8 pad_0x10[0x18];
    UnkIdNode_lf8ede4* prev;   // 0x28
    UnkIdNode_lf8ede4* next;   // 0x30
    UnkIdNode_lf8ede4* chain;  // 0x38

    UnkIdNode_lf8ede4*& Next() { return next; }
    UnkIdNode_lf8ede4*& Prev() { return prev; }
};
static_assert(__builtin_offsetof(UnkIdNode_lf8ede4, chain) == 0x38, "layout");

struct UnkIdMap_lf8ede4 {
    base::UnkAllocator* allocator;
    u8 pad_0x8[0x4];
    u32 count;                    // 0xc
    UnkIdNode_lf8ede4** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_lf8ede4* tail;  // 0x38
    UnkIdNode_lf8ede4* head;  // 0x40
};

UnkIdNode_lf8ede4* fn_7100f8ede4(UnkIdMap_lf8ede4* self, const CStrId* key) {
    UnkStringData* k = key->data;
    const UnkHashedStr_lf8ede4* h = reinterpret_cast<const UnkHashedStr_lf8ede4*>(k);
    u32 b = (h->hash_lo ^ h->hash_hi) % self->bucket_count;
    UnkIdNode_lf8ede4** slot = self->buckets + b;
    UnkIdNode_lf8ede4* n = *slot;
    if (!n)
        return nullptr;
    UnkIdNode_lf8ede4* before = nullptr;
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
    UnkIdNode_lf8ede4* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    n->value.~CStrId();
    n->key.~CStrId();
    self->allocator->vf_0x48(n);
    --self->count;
    return result;
}
