// Units 122, 150 and 225 of tools/tu_map.py: three identical erasures from a chained hash map by
// 64-bit key (the bucket is the key's low 32 bits modulo the bucket capacity; the
// layout of base/UnkChainedMap.h with the list ends the other way round): the node is unlinked from
// its bucket and from the list of all nodes and freed; returns the node that followed it in the
// list. One instantiation per map type. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "types.h"

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

struct UnkMapNode_b54a1 {
    u64 key;
    u64 value;
    UnkMapNode_b54a1* next;   // 0x10: the list of all nodes
    UnkMapNode_b54a1* prev;   // 0x18
    UnkMapNode_b54a1* chain;  // 0x20: the next node in the bucket

    // The list links, through accessors (their stores may alias each other).
    UnkMapNode_b54a1*& Next() { return next; }
    UnkMapNode_b54a1*& Prev() { return prev; }
};

template <int Tag>
struct UnkChainedMap_b54a1 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkMapNode_b54a1** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;            // 0x18
    u32 bucket_capacity;         // 0x1c
    u8 pad_0x20[0x18];
    UnkMapNode_b54a1* tail;  // 0x38
    UnkMapNode_b54a1* head;  // 0x40

    static u32 Hash(const u64& k) { return (u32)k; }

    UnkMapNode_b54a1** At(u32 i) {
        if (!(i < bucket_count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }

    __attribute__((always_inline)) UnkMapNode_b54a1* Erase(const u64* key) {
        UnkMapNode_b54a1** bucket = At(Hash(*key) % bucket_capacity);
        UnkMapNode_b54a1* n = *bucket;
        if (!n)
            return nullptr;
        UnkMapNode_b54a1* before = nullptr;
        while (n->key != *key) {
            before = n;
            n = n->chain;
            if (!n)
                return nullptr;
        }
        asm("" : "+r"(before));  // keeps &before->chain out of the search loop
        *(before ? &before->chain : bucket) = n->chain;
        if (head == n)
            head = n->next;
        if (tail == n)
            tail = n->prev;
        if (n->Prev())
            n->Prev()->Next() = n->Next();
        UnkMapNode_b54a1* r = nullptr;
        if (n->Next()) {
            n->Next()->Prev() = n->Prev();
            r = n->Next();
        }
        allocator->vf_0x48(n);
        --count;
        return r;
    }
};


UnkMapNode_b54a1* fn_7100142504(UnkChainedMap_b54a1<3>* self, const u64* key) { return self->Erase(key); }
UnkMapNode_b54a1* fn_710014bca8(UnkChainedMap_b54a1<4>* self, const u64* key) { return self->Erase(key); }
UnkMapNode_b54a1* fn_710019e268(UnkChainedMap_b54a1<5>* self, const u64* key) { return self->Erase(key); }
