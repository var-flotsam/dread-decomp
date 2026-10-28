// Copies of fn_7100d6ea28 (src/base/reflection/fn_7100b80bbc.cpp), which the binary
// repeats at other addresses: fn_7100242bcc, fn_7100120e3c.
// Clear of a hash map keyed by string ids whose nodes are also chained in a list: unless it is
// already empty, releases each node's key and frees the node, then empties the buckets and the
// list. Three identical copies.
#include "base/UnkDestructors.h"
#include "types.h"

struct UnkIdMapNode {
    base::UnkStrIdMember key;
    u64 value;
    UnkIdMapNode* next;

    __attribute__((always_inline)) ~UnkIdMapNode() {}
};

struct UnkIdHashMap {
    base::UnkAllocator* allocator;
    u32 field_0x8;
    u32 count;
    UnkIdMapNode** buckets;
    u32 field_0x18;
    u32 bucket_count;
    u8 unk_0x20[0x18];
    UnkIdMapNode* tail;
    UnkIdMapNode* head;
};

__attribute__((always_inline)) static inline void Clear(UnkIdHashMap* self) {
    if (self->count == 0)
        return;
    for (UnkIdMapNode* node = self->head; node;) {
        UnkIdMapNode* next = node->next;
        node->~UnkIdMapNode();
        self->allocator->vf_0x48(node);
        node = next;
    }
    __builtin_memset(self->buckets, 0, (u64)self->bucket_count * sizeof(UnkIdMapNode*));
    self->count = 0;
    self->tail = nullptr;
    self->head = nullptr;
}

void fn_7100242bcc(UnkIdHashMap* self) { Clear(self); }

void fn_7100120e3c(UnkIdHashMap* self) { Clear(self); }

