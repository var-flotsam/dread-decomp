// Unit 2648: erases the node a string id names from a chained hash map (hashed by the two words at +0x30 of
// the id's string): the node is unlinked from its bucket chain and from the list of all nodes, its value and
// id are destroyed and it is freed; returns the node before it in the list. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2647.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkHashedStr_l03f804 {
    u8 pad_0x0[0x30];
    u32 hash_lo;
    u32 hash_hi;
};

struct UnkIdNode_l03f804 {
    CStrId key;
    u8 value[0x5d0];         // 0x8
    UnkIdNode_l03f804* prev;   // 0x5d8
    UnkIdNode_l03f804* next;   // 0x5e0
    UnkIdNode_l03f804* chain;  // 0x5e8

    // The list links, through accessors (their stores may alias each other).
    UnkIdNode_l03f804*& Next() { return next; }
    UnkIdNode_l03f804*& Prev() { return prev; }
};
static_assert(__builtin_offsetof(UnkIdNode_l03f804, chain) == 0x5e8, "layout");

struct UnkIdMap_l03f804 {
    base::IAllocator* allocator;
    u8 pad_0x8[0x4];
    u32 count;                    // 0xc
    UnkIdNode_l03f804** buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_l03f804* tail;  // 0x38
    UnkIdNode_l03f804* head;  // 0x40
};

UnkIdNode_l03f804* fn_710103f804(UnkIdMap_l03f804* self, const CStrId* key) {
    UnkStringData* k = key->data;
    const UnkHashedStr_l03f804* h = reinterpret_cast<const UnkHashedStr_l03f804*>(k);
    u32 b = (h->hash_lo ^ h->hash_hi) % self->bucket_count;
    UnkIdNode_l03f804** slot = self->buckets + b;
    UnkIdNode_l03f804* n = *slot;
    if (!n)
        return nullptr;
    UnkIdNode_l03f804* before = nullptr;
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
    UnkIdNode_l03f804* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    fn_7101036e34(n->value);
    n->key.~CStrId();
    self->allocator->vf_0x48(n);
    --self->count;
    return result;
}
