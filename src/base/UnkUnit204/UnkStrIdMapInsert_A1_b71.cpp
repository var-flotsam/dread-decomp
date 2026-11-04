// Units 204 and 259 of tools/tu_map.py (no class hint): adds a node to a chained hash map keyed by a string id
// (the bucket is the string's 64-bit hash at +0x30, halves xored, modulo the bucket capacity): the
// node holds a copy of the id and an 8-byte (unit 204) or 4-byte (unit 259) value, goes at the head of its bucket chain and the tail
// of the list of all nodes (the layout of src/base/reflection/UnkUnit271_PairMapInsert_b61.cpp).
// Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

// The string data's hash (+0x30), as the bucket function reads it.
struct UnkStringHashView_A1b71 {
    u8 pad_0x0[0x30];
    u64 hash;
};

template <class V>
struct UnkStrIdNode_A1b71 {
    CStrId key;
    V value;
    UnkStrIdNode_A1b71* next;   // 0x10: the list of all nodes
    UnkStrIdNode_A1b71* prev;   // 0x18
    UnkStrIdNode_A1b71* chain;  // 0x20: the next node in the bucket

    UnkStrIdNode_A1b71*& Next() { return next; }
    UnkStrIdNode_A1b71*& Prev() { return prev; }
};

template <class V>
struct UnkStrIdMap_A1b71 {
    using Node = UnkStrIdNode_A1b71<V>;
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    Node** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;              // 0x18
    u32 bucket_capacity;           // 0x1c
    u8 pad_0x20[0x18];
    Node* tail;  // 0x38
    Node* head;  // 0x40

    Node** At(u32 i) {
        if (bucket_count <= i)
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }
};

// The node's fields are cleared in one of two orders (the chain link first or last).
template <class V, bool ChainFirst>
__attribute__((always_inline)) static inline void Insert(UnkStrIdMap_A1b71<V>* self, const CStrId* key,
                                                         const V* value) {
    using Node = UnkStrIdNode_A1b71<V>;
    Node** slot = nullptr;
    if (self->bucket_count) {
        u64 h = reinterpret_cast<const UnkStringHashView_A1b71*>(key->data)->hash;
        slot = self->At(((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity);
    }
    Node* chain = *slot;
    auto* n = static_cast<Node*>(self->allocator->vf_0x40(0x28, 8, self->flags));
    if (n) {
        new (&n->key) CStrId(*key);
        n->value = *value;
        if (ChainFirst)
            n->chain = nullptr;
        n->next = nullptr;
        n->prev = nullptr;
        if (!ChainFirst)
            n->chain = nullptr;
    }
    n->chain = chain;
    *slot = n;
    if (!self->head) {
        self->head = n;
    } else if (self->tail) {
        n->Prev() = self->tail;
        self->tail->Next() = n;
    }
    self->tail = n;
    ++self->count;
}

void fn_710016a1cc(UnkStrIdMap_A1b71<u64>* self, const CStrId* key, const u64* value) {
    Insert<u64, false>(self, key, value);
}

void fn_71001d7cf0(UnkStrIdMap_A1b71<u32>* self, const CStrId* key, const u32* value) {
    Insert<u32, true>(self, key, value);
}
