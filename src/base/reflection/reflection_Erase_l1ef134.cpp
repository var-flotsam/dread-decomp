// Unit base::reflection: erases a key from a chained hash map that also keeps a doubly linked list of its
// nodes, frees the node through its allocator (vtable slot 0x48), and returns the erased node's list predecessor.
// Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkMapNode_ef134 {
    u64 key;
    u8 pad_0x8[8];
    UnkMapNode_ef134* prev;   // 0x10
    UnkMapNode_ef134* next;   // 0x18
    UnkMapNode_ef134* chain;  // 0x20

    UnkMapNode_ef134*& Next() { return next; }
    UnkMapNode_ef134*& Prev() { return prev; }
};

struct UnkMapOwner_ef134;
struct UnkMapOwnerVtbl_ef134 {
    u8 pad_0x0[0x48];
    void (*vf_0x48)(UnkMapOwner_ef134* self, UnkMapNode_ef134* node);
};
struct UnkMapOwner_ef134 {
    UnkMapOwnerVtbl_ef134* vtable_0x0;
};

struct UnkMap_ef134 {
    UnkMapOwner_ef134* owner;  // 0x0
    u8 pad_0x8[4];
    u32 count;                  // 0xc
    UnkMapNode_ef134** buckets;  // 0x10
    u32 bucket_size;            // 0x18
    u32 bucket_count;           // 0x1c
    u8 pad_0x20[0x18];
    UnkMapNode_ef134* tail;  // 0x38
    UnkMapNode_ef134* head;  // 0x40
};

UnkMapNode_ef134* fn_71001ef134(UnkMap_ef134* self, const u64* keyPtr) {
    u64 key = *keyPtr;
    u32 b = ((u32)(key >> 32) ^ (u32)key) % self->bucket_count;
    if (self->bucket_size <= b)
        __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
    UnkMapNode_ef134** slot = self->buckets + b;
    UnkMapNode_ef134* n = *slot;
    if (!n)
        return nullptr;
    UnkMapNode_ef134* before = nullptr;
    if (n->key != key) {
        do {
            before = n;
            n = n->chain;
            if (!n)
                return nullptr;
        } while (n->key != key);
    }
    *(before ? &before->chain : slot) = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    UnkMapNode_ef134* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    self->owner->vtable_0x0->vf_0x48(self->owner, n);
    --self->count;
    return result;
}

