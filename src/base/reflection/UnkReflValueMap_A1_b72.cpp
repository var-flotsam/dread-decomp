// Unit 83 of tools/tu_map.py (base::reflection names): erasing from a chained hash map from string
// ids to reflected values (bucket = the string's 64-bit hash at +0x30, halves xored, modulo
// the bucket capacity). A node holds the id, the value (released by fn_71000a1c60), the list
// links (next +0x20, prev +0x28) and the bucket chain (+0x30). Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;

struct UnkHashView_A1b72d {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkReflValueNode_A1b72 {
    CStrId key;
    UnkReflValue value;            // 0x8
    UnkReflValueNode_A1b72* next;   // 0x20: the list of all nodes
    UnkReflValueNode_A1b72* prev;   // 0x28
    UnkReflValueNode_A1b72* chain;  // 0x30: the next node in the bucket

    UnkReflValueNode_A1b72*& Next() { return next; }
    UnkReflValueNode_A1b72*& Prev() { return prev; }
};

struct UnkReflValueMap_A1b72 {
    using Node = UnkReflValueNode_A1b72;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;            // 0xc
    Node** buckets;       // 0x10
    u32 bucket_count;     // 0x18
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40
};

// Erases the node of `key`; returns the node after it in the list.
UnkReflValueNode_A1b72* fn_71000d6de0(UnkReflValueMap_A1b72* self, const CStrId* key) {
    using Node = UnkReflValueNode_A1b72;
    auto* k = key->data;
    u64 h = reinterpret_cast<const UnkHashView_A1b72d*>(k)->hash;
    u32 i = ((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity;
    asm("" : "+r"(i));  // the original computes the index before loading the buckets
    Node** bucket = &self->buckets[i];
    Node* n = *bucket;
    if (!n)
        return nullptr;
    Node* before = nullptr;
    while (n->key.data != k) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &before->chain : bucket) = n->chain;
    if (self->head == n)
        self->head = n->Next();
    if (self->tail == n)
        self->tail = n->Prev();
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    Node* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    fn_71000a1c60(&n->value);
    reinterpret_cast<base::UnkStrIdMember*>(&n->key)->~UnkStrIdMemberOf();
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}
