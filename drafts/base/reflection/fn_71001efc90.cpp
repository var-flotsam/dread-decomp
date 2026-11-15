// Unit base::reflection: erases a key from a chained hash map that also keeps a doubly linked list of its
// nodes, notifies the owner, and returns the erased node's list predecessor.
// Names and layouts are placeholders.
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkMapNode_efc90 {
    u64 key;
    u8 pad_0x8[8];
    UnkMapNode_efc90* prev;   // 0x10
    UnkMapNode_efc90* next;   // 0x18
    UnkMapNode_efc90* chain;  // 0x20

    UnkMapNode_efc90*& Next() { return next; }
    UnkMapNode_efc90*& Prev() { return prev; }
};

struct UnkMapOwner_efc90;
struct UnkMapOwnerVtbl_efc90 {
    u8 pad_0x0[0x48];
    void (*vf_0x48)(UnkMapOwner_efc90* self, UnkMapNode_efc90* node);
};
struct UnkMapOwner_efc90 {
    UnkMapOwnerVtbl_efc90* vtable_0x0;
};

struct UnkMap_efc90 {
    UnkMapOwner_efc90* owner;  // 0x0
    u8 pad_0x8[4];
    u32 count;                  // 0xc
    UnkMapNode_efc90** buckets;  // 0x10
    u32 bucket_size;            // 0x18
    u32 bucket_count;           // 0x1c
    u8 pad_0x20[0x18];
    UnkMapNode_efc90* tail;  // 0x38
    UnkMapNode_efc90* head;  // 0x40
};

UnkMapNode_efc90* fn_71001efc90(UnkMap_efc90* self, const u64* keyPtr) {
    u64 key = *keyPtr;
    u32 b = ((u32)(key >> 32) ^ (u32)key) % self->bucket_count;
    if (self->bucket_size <= b)
        __assert_fail("i < m_iSize", "..\Base/global/RntVector.h", 0x15b, "operator[]");
    UnkMapNode_efc90** slot = self->buckets + b;
    UnkMapNode_efc90* before = nullptr;
    UnkMapNode_efc90* n = *slot;
    for (;; before = n, n = n->chain) {
        if (!n)
            return nullptr;
        if (n->key == key)
            break;
    }
    if (before)
        slot = &before->chain;
    *slot = n->chain;
    if (self->head == n)
        self->head = n->Prev();
    if (self->tail == n)
        self->tail = n->Next();
    if (n->Next())
        n->Next()->Prev() = n->Prev();
    UnkMapNode_efc90* result = nullptr;
    if (n->Prev()) {
        n->Prev()->Next() = n->Next();
        result = n->Prev();
    }
    self->owner->vtable_0x0->vf_0x48(self->owner, n);
    --self->count;
    return result;
}
