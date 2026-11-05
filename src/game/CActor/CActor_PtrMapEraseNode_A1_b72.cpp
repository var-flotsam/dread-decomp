// CActor's unit (tools/tu_map.py unit 341): erases an object pointer's node from a chained hash map
// (hashed by the two words at +0x30 of the object; the layout of src/game/CActor/CActor_PtrMapErase_b61.cpp):
// the node is unlinked from its bucket chain (+0x40) and the list of all nodes (prev +0x30, next +0x38),
// destroyed with fn_710024515c and freed through the map's allocator. Returns the node before it in
// the list. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

struct UnkHashedObject_A1b72 {
    u8 pad_0x0[0x30];
    u32 field_0x30;
    u32 field_0x34;
};

struct UnkPtrNode_A1b72 {
    UnkHashedObject_A1b72* key;
    u8 pad_0x8[0x28];
    UnkPtrNode_A1b72* prev;   // 0x30
    UnkPtrNode_A1b72* next;   // 0x38
    UnkPtrNode_A1b72* chain;  // 0x40

    // The list links, through accessors (their stores may alias each other).
    UnkPtrNode_A1b72*& Next() { return next; }
    UnkPtrNode_A1b72*& Prev() { return prev; }
};

struct UnkPtrMap_A1b72 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;                    // 0xc
    UnkPtrNode_A1b72** buckets;   // 0x10
    u32 field_0x18;
    u32 bucket_count;  // 0x1c
    u8 pad_0x20[0x18];
    UnkPtrNode_A1b72* tail;  // 0x38
    UnkPtrNode_A1b72* head;  // 0x40
};

void fn_710024515c(UnkPtrNode_A1b72* node);  // the node's destructor

UnkPtrNode_A1b72* fn_710024536c(UnkPtrMap_A1b72* self, UnkHashedObject_A1b72* const* key) {
    UnkHashedObject_A1b72* k = *key;
    u32 b = (k->field_0x30 ^ k->field_0x34) % self->bucket_count;
    UnkPtrNode_A1b72** bucket = self->buckets + b;
    UnkPtrNode_A1b72* n = *bucket;
    if (!n)
        return nullptr;
    UnkPtrNode_A1b72* before = nullptr;
    while (n->key != k) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &before->chain : bucket) = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    UnkPtrNode_A1b72* r = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        r = n->Prev();
    }
    fn_710024515c(n);
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}
