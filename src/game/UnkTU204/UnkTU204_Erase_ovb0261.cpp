// Unit 204 (tools/tu_map.py, no reliable name): erasure from a chained hash map keyed by string id
// (the layout of src/base/UnkUnit122/UnkChainedMapErase_b54.cpp with the list ends the other way
// round): the node is unlinked from its bucket and from the list of all nodes, its id released and the
// node freed; returns the node that followed it. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkIdMapNode_b0261 {
    CStrId key;                  // 0x0
    u64 value;                   // 0x8
    UnkIdMapNode_b0261* next;    // 0x10: the list of all nodes
    UnkIdMapNode_b0261* prev;    // 0x18
    UnkIdMapNode_b0261* chain;   // 0x20: the next node in the bucket

    UnkIdMapNode_b0261*& Next() { return next; }
    UnkIdMapNode_b0261*& Prev() { return prev; }
};

struct UnkIdMap_b0261 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdMapNode_b0261** buckets;  // 0x10
    u32 bucket_count;              // 0x18
    u32 bucket_capacity;           // 0x1c
    u8 pad_0x20[0x18];
    UnkIdMapNode_b0261* tail;  // 0x38
    UnkIdMapNode_b0261* head;  // 0x40

    UnkIdMapNode_b0261** At(u32 i) {
        if (!(i < bucket_count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }
};

UnkIdMapNode_b0261* fn_710016a024(UnkIdMap_b0261* self, const CStrId* key) {
    const u32* h = reinterpret_cast<const u32*>(reinterpret_cast<const char*>(key->data) + 0x30);
    UnkIdMapNode_b0261** bucket = self->At((h[0] ^ h[1]) % self->bucket_capacity);
    UnkIdMapNode_b0261* n = *bucket;
    if (!n)
        return nullptr;
    UnkIdMapNode_b0261* before = nullptr;
    while (n->key.data != key->data) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &before->chain : bucket) = n->chain;
    if (self->head == n)
        self->head = n->next;
    if (self->tail == n)
        self->tail = n->prev;
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    UnkIdMapNode_b0261* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    n->key.Reset();
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}
